
/**
 * @file    mcc_custom_config.h     
 * @ingroup driver-layer 
 * @brief   Contain the MCC Custom peripheral configuration that is not supported
 *  by MCC Melody tool. 
 */
#include <xc.h>
#include "driver/mcc_extension/drv_mcc_extension.h"
#include "driver/mcc_extension/drv_custom_config_pwm.h"
#include "driver/mcc_extension/drv_custom_config_adc.h"
#include "pwrctrl/pwrctrl.h"

/*******************************************************************************
 * @ingroup driver-layer 
 * @brief  Contains custom peripheral initialization 
 * @return void
 * 
 * @details This function initializes peripheral settings that are needed in the
 *  application but is not supported by the generated code through MCC Melody tool. 
 * 
 *********************************************************************************/

void MCC_Custom_User_Config (void)
{
    // Initialize PWM as re-triggerable for the cascaded PWM scheme
    // and setup the PWM fault PCI for hardware fault
    DRV_Config_Custom_PWM();
    
}
