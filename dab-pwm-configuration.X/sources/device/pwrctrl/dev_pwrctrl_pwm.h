/* 
 * File:   drv_pwrctrl_pwm.h
 * Author: M15690
 *
 * Created on July 8, 2024, 4:42 PM
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H

#include "dev_pwrctrl_typedef.h"

void Dev_PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance);
void Dev_PwrCtrl_PWM_Enable(void);
void Dev_PwrCtrl_PWM_Disable(void);

#endif	/* DRV_PWRCTRL_PWM_H */

