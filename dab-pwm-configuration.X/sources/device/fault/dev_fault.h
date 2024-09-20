/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef DRV_PWRCTL_FAULT_H
#define	DRV_PWRCTL_FAULT_H


#include <stddef.h>

#include "cmp/cmp1.h"
#include "cmp/cmp3.h"

#include "dev_fault.h"
#include "pwrctrl/pwrctrl_pwm.h"
#include "pwrctrl/pwrctrl_typedef.h"
#include "pwrctrl/pwrctrl_utils.h"
#include "config/config.h"


//------------------------------------------------------------------------------
// functions with external linkage
//------------------------------------------------------------------------------
void Dev_Fault_Initialize(void);
void Dev_Fault_Execute(void);

void Dev_Fault_ClearHardwareFaults(void);
void Dev_Fault_Reset(void);
void Dev_Fault_Handler(void);
void Dev_Fault_Temp_100ms(void); 

#endif	/* DRV_PWRCTL_FAULT_H */

