/* 
 * File:   drv_mcc_config_pwm.h
 * Author: M15690
 *
 * Created on 10 July 2024, 14:50
 */

#ifndef DRV_CONFIG_CUSTOM_PWM_H
#define	DRV_CONFIG_CUSTOM_PWM_H

#include "driver/power_control/drv_pwrctrl_typedef.h"
#include "driver/mcc_extension/drv_mcc_extension_pwm.h"

static __inline__ void DRV_Config_Custom_PWM(POWER_CONTROL_t* pcInstance)
{
    //PWMs needs to be re-triggerable for cascaded PWM scheme
    PWM_Trigger_Mode(pcInstance->Pwm.Primary_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(pcInstance->Pwm.Primary_2, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(pcInstance->Pwm.Secondary_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(pcInstance->Pwm.Secondary_2, PWM_TRIG_MODE_RETRIGGERABLE);
    
    // PWM Swap output based on the hardware
    PWM_Swap_PWMxL_and_PWMxH(pcInstance->Pwm.Primary_2, true);
    PWM_Swap_PWMxL_and_PWMxH(pcInstance->Pwm.Secondary_2, true);
}

#endif	/* DRV_MCC_CONFIG_PWM_H */

