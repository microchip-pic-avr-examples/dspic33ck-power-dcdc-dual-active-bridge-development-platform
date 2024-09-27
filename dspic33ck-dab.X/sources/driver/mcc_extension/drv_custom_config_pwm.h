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

/**
 * @file    drv_custom_config_pwm.h     
 * @ingroup driver-layer-pwm   
 * @brief   Contains PWM initialization that was not supported by MCC generated code.  
 */

#ifndef DRV_CONFIG_CUSTOM_PWM_H
#define	DRV_CONFIG_CUSTOM_PWM_H

#include "pwrctrl/pwrctrl_typedef.h"
#include "drv_mcc_extension_pwm.h"
#include "config/macros.h"
#include "pwm_hs/pwm.h"

/*********************************************************************************
 * @ingroup driver-layer-pwm  
 * @brief   Contains PWM initialization that was not supported by MCC generated code.
 * @return  void
 *
 * @details This PWM initialization configures the PWM modules to be retriggerable 
 * for the cascaded PWM approach. Additionally, two PWM outputs were swapped to 
 * support the board's functionality.
 **********************************************************************************/
static __inline__ void DRV_Config_Custom_PWM(void)
{
    //PWMs needs to be re-triggerable for cascaded PWM scheme
    PWM_Trigger_Mode(PWM_PRI_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_SEC_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_PRI_2, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_SEC_2, PWM_TRIG_MODE_RETRIGGERABLE);
    
    // PWM Swap output based on the hardware
    PWM_Swap_PWMxL_and_PWMxH(PWM_PRI_2, true);
    PWM_Swap_PWMxL_and_PWMxH(PWM_SEC_2, true);
    
    // Fault mode is Latched mode
    PWM_PCI_Fault_AcceptanceCriteria(PWM_PRI_1, PWM_PCI_ACCEPT_LATCHED);     
    PWM_PCI_Fault_AcceptanceCriteria(PWM_SEC_1, PWM_PCI_ACCEPT_LATCHED);    
    PWM_PCI_Fault_AcceptanceCriteria(PWM_PRI_2, PWM_PCI_ACCEPT_LATCHED);     
    PWM_PCI_Fault_AcceptanceCriteria(PWM_SEC_2, PWM_PCI_ACCEPT_LATCHED);    
    
    // Stop PWMs straight away upon detection of a fault
    PWM_PCI_Fault_TimetoTerminateAfterEvent(PWM_PRI_1, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
    PWM_PCI_Fault_TimetoTerminateAfterEvent(PWM_SEC_1, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
    PWM_PCI_Fault_TimetoTerminateAfterEvent(PWM_PRI_2, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
    PWM_PCI_Fault_TimetoTerminateAfterEvent(PWM_SEC_2, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
    
    PWM_Fault_DataMode(PWM_PRI_1, PWM_FAULT_EVENT_PWML);
    PWM_Fault_DataMode(PWM_SEC_1, PWM_FAULT_EVENT_PWML);
    PWM_Fault_DataMode(PWM_PRI_2, PWM_FAULT_EVENT_PWML);
    PWM_Fault_DataMode(PWM_SEC_2, PWM_FAULT_EVENT_PWML);
    
}

#endif	/* DRV_MCC_CONFIG_PWM_H */

