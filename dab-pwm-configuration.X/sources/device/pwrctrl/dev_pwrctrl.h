
/**
 * @file dev_pwrctrl.h     
 * @ingroup dev-pwrctrl   
 * @brief Contains public functions and data types relevant for power control interface.    
 */

#ifndef APP_PWRCTRL_H
#define	APP_PWRCTRL_H

#include "dev_pwrctrl_typedef.h"
#include "dev_pwrctrl_utils.h"

extern POWER_CONTROL_t dab;

void Dev_PwrCtrl_Initialize(void);
void Dev_PwrCtrl_Execute(void);
void Dev_PwrCtrl_Suspend(void);
void Dev_PwrCtrl_ControlLoopInitialize(void);

#endif	/* APP_PWRCTRL_H */

