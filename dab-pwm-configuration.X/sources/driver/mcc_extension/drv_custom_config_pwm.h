/* 
 * File:   drv_mcc_config_pwm.h
 * Author: M15690
 *
 * Created on 10 July 2024, 14:50
 */

#ifndef DRV_CONFIG_CUSTOM_PWM_H
#define	DRV_CONFIG_CUSTOM_PWM_H

#include "device/pwrctrl/dev_pwrctrl_typedef.h"
#include "drv_mcc_extension_pwm.h"
#include "config/macros.h"
#include "pwm_hs/pwm.h"

static __inline__ void DRV_Config_Custom_PWM(void)
{
    //PWMs needs to be re-triggerable for cascaded PWM scheme
    PWM_Trigger_Mode(PWM_PRI_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_SEC_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_PRI_2, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_SEC_2, PWM_TRIG_MODE_RETRIGGERABLE);
    
    // PWM Swap output based on the hardware
    PWM_Swap_PWMxL_and_PWMxH(PWM_PRI_2, true);
    PWM_Swap_PWMxL_and_PWMxH(PWM_SEC_2, true);
    
}

#endif	/* DRV_MCC_CONFIG_PWM_H */

