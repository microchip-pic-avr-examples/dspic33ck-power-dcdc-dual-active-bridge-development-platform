/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */


#include "dev_pwrctrl_typedef.h"
#include "timer/sccp1.h"
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "config/macros.h"
#include "dev_pwrctrl_pwm.h"

extern POWER_CONTROL_t dab;

/*********************************************************************************
 * @ingroup 
 * @fn      void SCCP1_TimeoutCallback(void)
 * @brief   ISR in which the control loop is executed. 
 * @param   none
 * @return  none
 * @details
 * execute control loop
 * manage shared ADC core
 * ADC fault protection
 **********************************************************************************/
void ControlLoop_Interrupt(void)
{          
    // read dedicated core ADC results, these are triggered via PWM1 trigger 1
    dab.Adc.isec_avg = ADC1_ConversionResultGet(ISEC_AVG); // used for control
    dab.Adc.isec_ct = ADC1_ConversionResultGet(ISEC_CT); // used for protection
    
    // read shared ADC core results
    // these are all triggered by PWM1 trigger 1
    dab.Adc.vsec = ADC1_ConversionResultGet(VSEC);
    dab.Adc.ipri_ct = ADC1_ConversionResultGet(IPRI_CT);   
    dab.Adc.vpri = ADC1_ConversionResultGet(VPRI);
    
    // TODO: move the handler for these 2 signals to a lower frequency process
    dab.Adc.vrail_5v = ADC1_ConversionResultGet(VRAIL_5V);
    dab.Adc.temperature = ADC1_ConversionResultGet(TEMP);
    
////    ToDo: relocated to fault task
//    // secondary over current fault handler
//#ifndef FAULT_ISEC_OC_DISABLE      
//    Drv_PwrCtrl_Fault_Isec_OC();
//#endif // #ifndef FAULT_ISEC_OC_DISABLE
//    
//    // secondary over voltage fault handler
//#ifndef FAULT_VSEC_OV_DISABLE            
//    Drv_PwrCtrl_Fault_Vsec_OV();
//#endif // #ifndef FAULT_VSEC_OC_DISABLE   
//    
//    // primary over current fault handler
//#ifndef FAULT_IPRI_OC_DISABLE
//    Drv_PwrCtrl_Fault_Ipri_OC();
//#endif // #ifndef FAULT_IPRI_OC_DISABLE
//    
//    // primary over voltage fault handler
//#ifndef FAULT_VPRI_OV_DISABLE                
//    Drv_PwrCtrl_Fault_Vpri_OV();
//#endif   // #ifndef FAULT_VPRI_OV_DISABLE
//    
//#ifndef DISABLE_VOLTAGE_LOOP
//    // voltage loop
////    static uint16_t runVoltageLoop = 0;
//    //ToDo: Add this later
////    if (++runVoltageLoop > VLOOP_ILOOP_EXE_RATIO)  
////    {
////        dab.vloop.feedback = dab.Adc.vsec;
////        SMPS_Controller2P2ZUpdate(&icomp_2p2z,
////                                  &dab.vloop.feedback,
////                                  dab.vloop.reference,
////                                  &dab.vloop.output);
////        runVoltageLoop = 0;
////    }
//#endif // #ifndef DISABLE_VOLTAGE_LOOP
//    
//    // current loop compensator
//#ifndef OPEN_LOOP_PBV
//    //TODO: clean this all up
//    int16_t feedback = ((int16_t)dab.Adc.isec_avg - dab.Adc.isec_sensor_offset); 
//    if (feedback < 0)
//    {
//        feedback = 0;
//    }
//    // scale input to compensator x16 to add some resolution
//    // current sensor has low gain (31 ADC codes per amp)
//    dab.iloop.feedback = feedback << 4;    // TODO: add symbolic reference and some explanation
//    //ToDo: add this later
////    SMPS_Controller2P2ZUpdate(&icomp_2p2z,
////                              &dab.iloop.feedback,
////                              dab.iloop.reference,
////                              &dab.iloop.output);
//    
//    // compensator output must be scaled to match the range of PGxPER values
//    // that corresponds to the min and max switching frequency
//    // this only works if comp max = 32767 (see DCDT settings)
//    // and if (PGxPER_MAX - PGxPER_MIN) exceeds 32767 
//    
//#ifdef OPEN_LOOP_POTI
//    // use poti on ADC current sensor pin (stored in cllc.adc.isec_avg) to control frequency
//    // this is only meant for running on the digital power development board
//    // to map ADC reading (0 to 4095) to same range as compensator output (0 to 32767)
//    // multiply ADC reading by 8
//    dab.iloop.output = dab.Adc.isec_avg << 3;
//#endif // #ifdef OPEN_LOOP_POTI
//        
//    uint16_t scalar = MAX_PWM_PERIOD - MIN_PWM_PERIOD;
//    uint32_t compOutScaled_32bit = __builtin_muluu(dab.iloop.output, scalar);
//    uint16_t compOutScaled_16bit = (uint16_t)(compOutScaled_32bit >> 15);
//    dab.Pwm.ControlPeriod = MIN_PWM_PERIOD + compOutScaled_16bit;
//    

    //ToDo: remove this Test PWM with PBV
    dab.Pwm.ControlPeriod = dab.Pwm.PBVPeriodTarget;
            
    if (dab.Pwm.ControlPeriod >= MAX_PWM_PERIOD)
        dab.Pwm.ControlPeriod = MAX_PWM_PERIOD;
    else if (dab.Pwm.ControlPeriod <= MIN_PWM_PERIOD)
        dab.Pwm.ControlPeriod = MIN_PWM_PERIOD;
       
    Dev_PwrCtrl_PWM_Update(&dab);
    
//#endif // #ifndef OPEN_LOOP_PBV
    
}