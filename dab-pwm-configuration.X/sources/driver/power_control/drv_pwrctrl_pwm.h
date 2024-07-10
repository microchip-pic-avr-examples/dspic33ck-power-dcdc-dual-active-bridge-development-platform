/* 
 * File:   drv_pwrctrl_pwm.h
 * Author: M15690
 *
 * Created on July 8, 2024, 4:42 PM
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H



void Drv_PwrCtrl_PWM_Enable(POWER_CONTROL_t* pcInstance);
void Drv_PwrCtrl_PWM_Disable(POWER_CONTROL_t* pcInstance);
void Drv_PwrCtrl_PWM_Primary_Enable(POWER_CONTROL_t* pcInstance);
void Drv_PwrCtrl_PWM_Secondary_Enable(POWER_CONTROL_t* pcInstance);
void Drv_PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance);



#endif	/* DRV_PWRCTRL_PWM_H */

