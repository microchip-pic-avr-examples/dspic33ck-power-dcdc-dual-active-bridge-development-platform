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
#include "device/pwrctrl/dev_pwrctrl_pwm.h"
#include "device/pwrctrl/dev_pwrctrl_typedef.h"
#include "device/pwrctrl/dev_pwrctrl_utils.h"
#include "config/config.h"


//------------------------------------------------------------------------------
// functions with external linkage
//------------------------------------------------------------------------------
void Dev_Fault_Initialize(void);
void Dev_Fault_Execute(void);

void Drv_PwrCtrl_Fault_EnableShortCircuitProtection(void);
void Dev_Fault_ClearHardwareFaults(void);
void Dev_Fault_Reset(void);
void Dev_Fault_Handler(void);

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t __inline__ Drv_PwrCtrl_Fault_SC_Faults_Clear(void)
 * @brief   check if short circuit faults are clear before re-starting the converter
 * @param   none
 * @return  1 if both comparators are 0, 0 otherwise
 * @details
 **********************************************************************************/
uint16_t __inline__ Drv_PwrCtrl_Fault_SC_Faults_Clear(POWER_CONTROL_t* pcInstance)
{
#if (FAULT_SHORT_CCT)
    uint16_t sc_faults_clear;
    uint16_t isec_sc = CMP_ISEC_SC_StatusGet();
    uint16_t ipri_sc = CMP_IPRI_SC_StatusGet();
 
    if (!isec_sc && !ipri_sc)
    {
        sc_faults_clear = 1;
    }
    else
    {
        sc_faults_clear = 0;
    }
    return (sc_faults_clear);
    
#else 
    return (1);
#endif 
}




#endif	/* DRV_PWRCTL_FAULT_H */

