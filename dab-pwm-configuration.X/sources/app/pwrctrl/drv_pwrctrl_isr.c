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

#include "config/user_params_to_ints.h"
#include "adc/adc1.h"
#include "config/peripheral_custom/cfg_custom_adc.h"
#include "driver_chiplayer/mcc_extenstion/drv_mcc_extension.h"
#include "drv_pwrctrl_fault.h"
#include "drv_pwrctrl_types.h"
#include "drv_pwrctrl_utils.h"
#include "dcdt/smps_control_.h"
#include "system/pins.h"
#include "dcdt/drv_pwrctrl_dcdt.h"

#include "timer/sccp1.h"

extern CLLC_t cllc;

/*********************************************************************************
 * @ingroup 
 * @fn      void SCCP1_TimeoutCallback(void)
 * @brief   ISR in which the control loop is executed. 
 * @param   none
 * @return  none
 * @details
 * execute control loop
 * manage shared ADC core
 **********************************************************************************/
void SCCP1_TimeoutCallback(void)
{          
    // manage shared ADC core
    static enum ADC_CHANNEL adcChannel = VSEC;
    static uint16_t sample_divider_5v = 0;
    static uint16_t sample_divider_temperature = 0;
    
    switch (adcChannel)
    {
        case VSEC:            
            cllc.adc.vsec = ADC1_ConversionResultGet(VSEC);
#ifndef FAULT_VSEC_OV_DISABLE            
            Drv_PwrCtrl_Fault_Vsec_OV();
#endif // #ifndef FAULT_VSEC_OC_DISABLE   
            // next channel
            DRV_ADC_SetChannel(ADC_CHANNEL_IPRI_CT);
            adcChannel = IPRI_CT;
            break;

        case IPRI_CT:
            cllc.adc.ipri_ct = ADC1_ConversionResultGet(IPRI_CT);    
#ifndef FAULT_IPRI_OC_DISABLE
            Drv_PwrCtrl_Fault_Ipri_OC();
#endif // #ifndef FAULT_IPRI_OC_DISABLE
            
            // next channel
            DRV_ADC_SetChannel(ADC_CHANNEL_VPRI);
            adcChannel = VPRI;
            break;

        case VPRI:            
            cllc.adc.vpri = ADC1_ConversionResultGet(VPRI);
#ifndef FAULT_VPRI_OV_DISABLE                
            Drv_PwrCtrl_Fault_Vpri_OV();
#endif   // #ifndef FAULT_VPRI_OV_DISABLE           
            
            // next channel
            sample_divider_5v++;
            if (sample_divider_5v >= ADC_5V_SAMPLE_DIV)
            {
                sample_divider_5v = 0;
                DRV_ADC_SetChannel(ADC_CHANNEL_VRAIL_5V);
                adcChannel = VRAIL_5V;                
            }
            else
            {
                DRV_ADC_SetChannel(ADC_CHANNEL_VSEC); 
                adcChannel = VSEC;                
            }
            break;

        case VRAIL_5V:
            cllc.adc.vrail_5v = ADC1_ConversionResultGet(VRAIL_5V);
            sample_divider_temperature++;
            if (sample_divider_temperature > ADC_TEMP_SAMPLE_DIV)
            {
                sample_divider_temperature = 0;
                DRV_ADC_SetChannel(ADC_CHANNEL_TEMP);
                adcChannel = TEMP;
            }
            else
            {
                DRV_ADC_SetChannel(ADC_CHANNEL_VSEC); 
                adcChannel = VSEC;                
            }
            break;

        case TEMP:
            cllc.adc.temperature = ADC1_ConversionResultGet(TEMP);

            // next channel
            DRV_ADC_SetChannel(ADC_CHANNEL_VSEC);
            adcChannel = VSEC;
            break;

        default:
            DRV_ADC_SetChannel(ADC_CHANNEL_VSEC); 
            adcChannel = VSEC;
            break;
    }
    
    // read dedicated core ADC results, these are triggered via a PWM
    // so we are just reading the latest values
    cllc.adc.isec_avg = ADC1_ConversionResultGet(ISEC_AVG);
    cllc.adc.isec_ct = ADC1_ConversionResultGet(ISEC_CT);
    
    // current loop compensator
#ifndef OPEN_LOOP_PBV
    //TODO: clean this all up
    int16_t feedback = ((int16_t)cllc.adc.isec_avg - cllc.isec_sensor_offset); 
    if (feedback < 0)
    {
        feedback = 0;
    }
    // scale input to compensator x16 to add some resolution
    // current sensor has low gain (31 ADC codes per amp)
    cllc.iloop.feedback = feedback << 4;    // TODO: add symbolic reference and some explanation
    SMPS_Controller2P2ZUpdate(&icomp_2p2z,
                              &cllc.iloop.feedback,
                              cllc.iloop.reference,
                              &cllc.iloop.output);
    
    // compensator output must be scaled to match the range of PGxPER values
    // that corresponds to the min and max switching frequency
    // this only works if comp max = 32767 (see DCDT settings)
    // and if (PGxPER_MAX - PGxPER_MIN) exceeds 32767 
    
#ifdef OPEN_LOOP_POTI
    // use poti on ADC current sensor pin (stored in cllc.adc.isec_avg) to control frequency
    // this is only meant for running on the digital power development board
    // to map ADC reading (0 to 4095) to same range as compensator output (0 to 32767)
    // multiply ADC reading by 8
    cllc.iloop.output = cllc.adc.isec_avg << 3;
#endif // #ifdef OPEN_LOOP_POTI
        
    uint16_t scalar = PGxPER_MAX - PGxPER_MIN;
    uint32_t compOutScaled_32bit = __builtin_muluu(cllc.iloop.output, scalar);
    uint16_t compOutScaled_16bit = (uint16_t)(compOutScaled_32bit >> 15);
    cllc.pwm.pgxper = PGxPER_MIN + compOutScaled_16bit;
    
//TODO: maybe can remove this clamp check, shouldn't be necessary
    if (cllc.pwm.pgxper > PGxPER_MAX)
    {
        cllc.pwm.pgxper = PGxPER_MAX;
    }
    else if (cllc.pwm.pgxper < PGxPER_MIN)
    {
        cllc.pwm.pgxper = PGxPER_MIN;
    }
    
    Drv_PwrCtrl_PWM_Update(); 
#endif // #ifndef OPEN_LOOP_PBV
    
    // secondary over current fault handler
#ifndef FAULT_ISEC_OC_DISABLE      
    Drv_PwrCtrl_Fault_Isec_OC();
#endif // #ifndef FAULT_ISEC_OC_DISABLE
    
    // trigger shared ADC core - do as late as possible to allow ADC input time to settle
    DRV_ADC_TriggerSoftwareConversion();     
}