
/**
 * @file      fault.h
 * @ingroup   fault 
 * @brief     Contains public fault functions.
 */

#ifndef DRV_PWRCTL_FAULT_H
#define	DRV_PWRCTL_FAULT_H


#include <stddef.h>

#include "cmp/cmp1.h"
#include "cmp/cmp3.h"

#include "fault.h"
#include "pwrctrl/pwrctrl_pwm.h"
#include "pwrctrl/pwrctrl_typedef.h"
#include "pwrctrl/pwrctrl_common.h"
#include "config/config.h"


//------------------------------------------------------------------------------
// functions with external linkage
//------------------------------------------------------------------------------
void Fault_Initialize(void);
void Fault_Execute(void);

void Dev_Fault_ClearHardwareFaults(void);
void Fault_Reset(void);
void Fault_Handler(void);
void Fault_Execute_100ms(void); 

#endif	/* DRV_PWRCTL_FAULT_H */

