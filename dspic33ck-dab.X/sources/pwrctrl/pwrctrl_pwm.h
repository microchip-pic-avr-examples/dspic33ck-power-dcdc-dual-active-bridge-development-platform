/**
 * @file pwrctrl_pwm.h     
 * @ingroup pwrctrl-pwm   
 * @brief Contains public functions for PWM.    
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H

#include "pwrctrl_typedef.h"

void PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance);
void PwrCtrl_PWM_Enable(void);
void PwrCtrl_PWM_Disable(void);

#endif	/* DRV_PWRCTRL_PWM_H */

