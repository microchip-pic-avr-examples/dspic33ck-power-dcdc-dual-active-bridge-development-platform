
/**
 * @file    drv_custom_config_adc.h     
 * @ingroup driver-layer-adc   
 * @brief   Contains ADC initialization that was not supported by MCC generated code.  
 */

#ifndef DRV_CUSTOM_CONFIG_ADC_H
#define	DRV_CUSTOM_CONFIG_ADC_H


#include "adc/adc1.h"
#include "drv_mcc_extension_adc.h"

/******************************************************************************
 * @ingroup driver-layer-adc 
 * @{
 ******************************************************************************/
// ADC channels numbers for shared ADC
#define ADC_CHANNEL_VSEC        (2)     ///< ADC2 is routed to Secondary Voltage
#define ADC_CHANNEL_IPRI_CT     (7)     ///< ADC7 is routed to Primary CT Current
#define ADC_CHANNEL_VPRI        (10)    ///< ADC10 is routed to Primary Voltage
#define ADC_CHANNEL_TEMP        (14)    ///< ADC14 is routed to Temperature
#define ADC_CHANNEL_VRAIL_5V    (19)    ///< ADC19 is routed to 5V rail

// ADC channel numbers for dedicated ADC
#define ADC_CHANNEL_ISEC_CT     (0)     ///< ADC0 is routed to Secondary CT Current
#define ADC_CHANNEL_ISEC_AVG    (1)     ///< ADC1 is routed to Secondary Average Current
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/*********************************************************************************
 * @ingroup driver-layer-adc 
 * @brief   Customize configuration for ADC
 * @return  void
 *
 * @details This function initializes the ADC module to use the ADC Filter mode
 * (averaging mode) to some of the ADC being sampled in the application. 
 **********************************************************************************/
static __inline__ void DRV_Config_Custom_ADC(void)
{
    
    // use ADC filter 1 for primary voltage averaging
    ADC_Filter_Mode(0, ADC_FILTER_MODE_AVERAGING);
    ADC_Filter_Averaging(0, ADC_FILTER_OVRSAM_AVG_4x);
    ADC_Filter_Input(0, ADC_CHANNEL_VPRI);
    ADC_Filter_Enable(0, true);
    
    // use ADC filter 1 for secondary voltage averaging
    ADC_Filter_Mode(1, ADC_FILTER_MODE_AVERAGING);
    ADC_Filter_Averaging(1, ADC_FILTER_OVRSAM_AVG_4x);
    ADC_Filter_Input(1, ADC_CHANNEL_VSEC);
    ADC_Filter_Enable(1, true);
    
}


#endif	/* DRV_CUSTOM_CONFIG_ADC_H */

