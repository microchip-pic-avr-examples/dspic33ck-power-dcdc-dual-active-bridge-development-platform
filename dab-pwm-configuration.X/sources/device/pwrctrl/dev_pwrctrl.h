/* 
 * File:   app_pwrctrl.h
 * Author: M15690
 *
 * Created on 10 July 2024, 14:46
 */

#ifndef APP_PWRCTRL_H
#define	APP_PWRCTRL_H

#include "dev_pwrctrl_typedef.h"
#include "dev_pwrctrl_utils.h"

extern POWER_CONTROL_t dab;
extern AVERAGING_t IsecAveraging;

void Dev_PwrCtrl_Initialize(void);
void Dev_PwrCtrl_Execute(void);
void Dev_PwrCtrl_Suspend(void);
void Dev_PwrCtrl_ControlLoopInitialize(void);

#endif	/* APP_PWRCTRL_H */

