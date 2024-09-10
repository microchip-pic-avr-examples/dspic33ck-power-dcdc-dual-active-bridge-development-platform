/* 
 * File:   drv_custom_config_adc.h
 * Author: M15690
 *
 * Created on September 10, 2024, 9:58 AM
 */

#ifndef DRV_CUSTOM_CONFIG_ADC_H
#define	DRV_CUSTOM_CONFIG_ADC_H


#include "adc/adc1.h"
#include "drv_mcc_extension_adc.h"

// ADC channels numbers for shared ADC
#define ADC_CHANNEL_VSEC        (2)
#define ADC_CHANNEL_IPRI_CT     (7)
#define ADC_CHANNEL_VPRI        (10)
#define ADC_CHANNEL_TEMP        (14)
#define ADC_CHANNEL_VRAIL_5V    (19)

// ADC channel numbers for dedicated ADC
#define ADC_CHANNEL_ISEC_CT     (0)
#define ADC_CHANNEL_ISEC_AVG    (1)

static __inline__ void Cfg_custom_adc(void)
{
    // use ADC filter 0 for secondary current averaging
    ADC_Filter_Mode(0, ADC_FILTER_MODE_AVERAGING);
    ADC_Filter_Averaging(0, ADC_FILTER_OVRSAM_AVG_8x);
    ADC_Filter_Input(0, ADC_CHANNEL_ISEC_CT);
    ADC_Filter_Enable(0, true);
    
    // use ADC filter 1 for secondary voltage averaging
    ADC_Filter_Mode(1, ADC_FILTER_MODE_AVERAGING);
    ADC_Filter_Averaging(1, ADC_FILTER_OVRSAM_AVG_8x);
    ADC_Filter_Input(1, ADC_CHANNEL_VSEC);
    ADC_Filter_Enable(1, true);
    
    // use ADC filter 1 for secondary voltage averaging
    ADC_Filter_Mode(1, ADC_FILTER_MODE_AVERAGING);
    ADC_Filter_Averaging(1, ADC_FILTER_OVRSAM_AVG_8x);
    ADC_Filter_Input(1, ADC_CHANNEL_VSEC);
    ADC_Filter_Enable(1, true);
    
}


#endif	/* DRV_CUSTOM_CONFIG_ADC_H */

