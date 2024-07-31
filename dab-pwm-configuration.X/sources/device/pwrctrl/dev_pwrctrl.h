/* 
 * File:   app_pwrctrl.h
 * Author: M15690
 *
 * Created on 10 July 2024, 14:46
 */

#ifndef APP_PWRCTRL_H
#define	APP_PWRCTRL_H

#include "device/pwrctrl/dev_pwrctrl_typedef.h"

extern POWER_CONTROL_t dab;

extern void Dev_PwrCtrl_Initialize(void);
extern void Dev_PwrCtrl_Execute(void);
extern void Dev_PwrCtrl_Suspend(void);

#endif	/* APP_PWRCTRL_H */

