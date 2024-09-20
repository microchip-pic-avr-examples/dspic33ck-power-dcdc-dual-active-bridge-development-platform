/**
 * @file dev_pwrctrl_pwm.h     
 * @ingroup dev-pwrctrl-pwm   
 * @brief Contains public functions for PWM.    
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H

#include "dev_pwrctrl_typedef.h"

void Dev_PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance);
void Dev_PwrCtrl_PWM_Enable(void);
void Dev_PwrCtrl_PWM_Disable(void);

#endif	/* DRV_PWRCTRL_PWM_H */

