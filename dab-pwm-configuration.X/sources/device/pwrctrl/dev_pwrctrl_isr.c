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


#include "dev_pwrctrl.h"
#include "timer/sccp1.h"
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "config/macros.h"
#include "dev_pwrctrl_pwm.h"
#include "device/fault/dev_fault.h"
#include "dcdt/dev_pwrctrl_dcdt.h"

void Dev_PwrCtrl_UpdateConverterData(void);

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
    // Update the ADC data member
    Dev_PwrCtrl_UpdateConverterData();
    
    // Execute the fault detection
    Dev_Fault_Execute();
        
    // Execute the Voltage Loop Control
    //ToDo: Check with Lorant if the time execution of Vloop
    if(dab.PowerDirection == PWR_CTRL_CHARGING)
    { 
        VMC_2p2z.KfactorCoeffsB = 0x7FFF;
        VMC_2p2z.maxOutput =  0x7FFF;
        
        dab.VLoop.Feedback = dab.Adc.VSecVoltage << 4;
        dab.VLoop.Reference = dab.VLoop.Reference << 4;
        
        XFT_SMPS_Controller2P2ZUpdate(&VMC_2p2z, &dab.VLoop.Feedback,
                dab.VLoop.Reference, dab.VLoop.Output);
    }
    
    // Execute the Current Loop Control
    dab.ILoop.Feedback = dab.Adc.ISenseSecondary << 3;
    //adaptive gain factor
    IMC_2p2z.KfactorCoeffsB = dab.ILoop.AgcFactor;
    //refresh limits
    IMC_2p2z.maxOutput =  0x7FFF;
    
    //mixing stage from voltage loop 10khz
    uint32_t RefBuf = (uint32_t)dab.ILoop.Reference * (uint32_t)(dab.VLoop.Output & 0x7FFF);
    dab.ILoop.Reference = (uint16_t)(RefBuf>>12) ; //15-3
    
    XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback, 
            dab.ILoop.Reference, &dab.ILoop.Output);    
    
    
    #if (true == DPDB_TEST_RUN)

    // Connect TP11 -> Pot1 & TP12 -> Pot2 in digital Power Development Board
    uint16_t Pot1 = dab.Adc.ISecAverage; 
    uint16_t Pot2 = dab.Adc.ISenseSecondary; 
    
    // Calculate the Frequency based on the Potentiometer 1 voltage
    dab.Pwm.ControlPeriod = (uint16_t)(MIN_PWM_PERIOD + (Pot1 * ADC_PERIOD_RANGE)); 
    
    // Change the control Phase with Potentiometer 2
    dab.Pwm.ControlPhase = (uint16_t)(Pot2 * ADC_SCALER * (dab.Pwm.ControlPeriod >> 1));
    
    #endif
       
    // Update PWM Properties
    Dev_PwrCtrl_PWM_Update(&dab);
}




void Dev_PwrCtrl_UpdateConverterData (void)
{
    // read dedicated core ADC results, these are triggered via PWM1 trigger 1
    dab.Adc.ISecAverage = ADC1_ConversionResultGet(ISEC_AVG); // used for control
    dab.Adc.ISenseSecondary = ADC1_ConversionResultGet(ISEC_CT); // used for protection
    
    // read shared ADC core results
    // these are all triggered by PWM1 trigger 1
    dab.Adc.VSecVoltage = ADC1_ConversionResultGet(VSEC);
    dab.Adc.ISensePrimary = ADC1_ConversionResultGet(IPRI_CT);   
    
    dab.Adc.VPriVoltage = ADC1_ConversionResultGet(VPRI);

    dab.Adc.VRail_5V = ADC1_ConversionResultGet(VRAIL_5V);
    dab.Adc.Temperature = ADC1_ConversionResultGet(TEMP);
}