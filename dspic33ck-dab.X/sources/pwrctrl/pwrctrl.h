
/**
 * @file dev_pwrctrl.h     
 * @ingroup pwrctrl   
 * @brief Contains public functions and data types relevant for power control interface.    
 */

#ifndef APP_PWRCTRL_H
#define	APP_PWRCTRL_H

#include "pwrctrl_typedef.h"
#include "pwrctrl_common.h"
#include "pwrctrl_comm_interface.h"
#include "pwrctrl_isr.h"
#include "pwrctrl_pwm.h"

extern POWER_CONTROL_t dab;

void PwrCtrl_Initialize(void);
void PwrCtrl_Execute(void);
void Dev_PwrCtrl_Suspend(void);
void Dev_PwrCtrl_ControlLoopInitialize(void);
void PwrCtrl_Reset(void);

#endif	/* APP_PWRCTRL_H */

