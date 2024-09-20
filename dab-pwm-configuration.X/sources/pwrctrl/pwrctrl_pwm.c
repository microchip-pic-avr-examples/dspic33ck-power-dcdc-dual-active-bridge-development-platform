/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/**
 * @file      dev_pwrctrl_pwm.c
 * @ingroup   dev-pwrctrl-pwm   
 * @brief     Contains DAB control phase calculation between primary and 
 *  secondary, and the PWM distribution.
 */

#include <xc.h> 
#include <stdbool.h>
#include "pwm_hs/pwm.h"
#include "pwrctrl_typedef.h"

/*******************************************************************************
 * @ingroup dev-pwrctrl-pwm
 * @brief  PWM distribution for DAB converter
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function distributes the calculated control phase for the 
 *  half bridges of the DAB converter. PWM1 and PWM3 drives the primary half 
 *  bridges and PWM2 and PWM4 drives the secondary half bridges.
 *********************************************************************************/
void PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance)
{   
    // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
    // (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    uint16_t PeriodMask = 0x7; 
    
    // Mask the calculated frequency bits [2:0] to make the cascaded/synchronous
    // PWM scheme reliable (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    pcInstance->Pwm.ControlPeriod = pcInstance->Pwm.ControlPeriod & ~(PeriodMask);
    
    // Calculate Duty Cycle for 50%
    pcInstance->Pwm.ControlDutyCycle = (pcInstance->Pwm.ControlPeriod >> 1);
    
    // Maximum Clamping for control phase
    if(pcInstance->Pwm.ControlPhase > pcInstance->Pwm.ControlPeriod){
        pcInstance->Pwm.ControlPhase = pcInstance->Pwm.ControlPeriod;
    }

    // Calculate primary to secondary phase as half of the control phase
    uint16_t PrimarySecondaryPhase = (pcInstance->Pwm.ControlPhase >> 1);
    
    // Compensate the added Dead-time 
    PrimarySecondaryPhase += pcInstance->Pwm.DeadTimeLow >> 1;
    
    // Calculate the Bridge Delay ((Frequency / 2) - Primary to Secondary Phase + Control Phase)
    // Note that in the cascaded PWM, the reference phase of the client PWM, is its trigger source
    uint16_t PrimaryPhaseDelay = (pcInstance->Pwm.ControlDutyCycle - PrimarySecondaryPhase) + 
            pcInstance->Pwm.ControlPhase;
    
    // Set the PWM trigger with the calculated PWM phases
    PWM_TriggerCCompareValueSet(PWM_PRI_1, PrimarySecondaryPhase);
    PWM_TriggerCCompareValueSet(PWM_SEC_1, PrimaryPhaseDelay);
    PWM_TriggerCCompareValueSet(PWM_PRI_2, PrimarySecondaryPhase);

    // Set the PWM Duty Cycle at 50% with the given Frequency
    PWM_DutyCycleSet(PWM_PRI_1, pcInstance->Pwm.ControlDutyCycle);
    PWM_DutyCycleSet(PWM_SEC_1, pcInstance->Pwm.ControlDutyCycle);
    PWM_DutyCycleSet(PWM_PRI_2, pcInstance->Pwm.ControlDutyCycle);
    PWM_DutyCycleSet(PWM_SEC_2, pcInstance->Pwm.ControlDutyCycle);
    
    // Set the PWM Frequency
    PWM_PeriodSet(PWM_PRI_1, pcInstance->Pwm.ControlPeriod);
    PWM_PeriodSet(PWM_SEC_1, pcInstance->Pwm.ControlPeriod);
    PWM_PeriodSet(PWM_PRI_2, pcInstance->Pwm.ControlPeriod);
    PWM_PeriodSet(PWM_SEC_2, pcInstance->Pwm.ControlPeriod);

    // Set the PWM Low DeadTime
    PWM_DeadTimeLowSet(PWM_PRI_1, pcInstance->Pwm.DeadTimeLow);
    PWM_DeadTimeLowSet(PWM_SEC_1, pcInstance->Pwm.DeadTimeLow);
    PWM_DeadTimeLowSet(PWM_PRI_2, pcInstance->Pwm.DeadTimeLow);
    PWM_DeadTimeLowSet(PWM_SEC_2, pcInstance->Pwm.DeadTimeLow);
    
    // Set the PWM High DeadTime
    PWM_DeadTimeHighSet(PWM_PRI_1, pcInstance->Pwm.DeadTimeHigh);
    PWM_DeadTimeHighSet(PWM_SEC_1, pcInstance->Pwm.DeadTimeHigh);
    PWM_DeadTimeHighSet(PWM_PRI_2, pcInstance->Pwm.DeadTimeHigh);
    PWM_DeadTimeHighSet(PWM_SEC_2, pcInstance->Pwm.DeadTimeHigh);
    
    // Set the Update bit of the last PWM in the cascaded approach to broadcast
    // it to the other PWMs
    PWM_SoftwareUpdateRequest(PWM_SEC_2);
    
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-pwm
 * @brief  Enable the PWM output
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t 
 * @return void
 * 
 * @details This function turn on the physical PWM outputs by clearing the
 *  output override of the PWM module. 
 *********************************************************************************/
void PwrCtrl_PWM_Enable(void)
{      
    // Turn-On PWM outputs by disabling the output override 
    // on each high and low PWM output
    PWM_OverrideHighDisable(PWM_PRI_1);
    PWM_OverrideHighDisable(PWM_SEC_1);
    PWM_OverrideHighDisable(PWM_PRI_2);
    PWM_OverrideHighDisable(PWM_SEC_2);
           
    PWM_OverrideLowDisable(PWM_PRI_1);
    PWM_OverrideLowDisable(PWM_SEC_1);
    PWM_OverrideLowDisable(PWM_PRI_2);
    PWM_OverrideLowDisable(PWM_SEC_2); 
    
    // Set update request of the last PWM in the cascade 
    // to update all PWM registers
    PWM_SoftwareUpdateRequest(PWM_SEC_2);

}


/*******************************************************************************
 * @ingroup dev-pwrctrl-pwm
 * @brief  Disable the PWM output
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function disable the physical PWM output by setting the override
 *  bits of the PWM module.
 *********************************************************************************/
void PwrCtrl_PWM_Disable(void)
{   
    // Turn-Off PWM outputs by enabling the output override 
    // on each high and low PWM output
    PWM_OverrideHighEnable(PWM_PRI_1);
    PWM_OverrideHighEnable(PWM_SEC_1);
    PWM_OverrideHighEnable(PWM_PRI_2);
    PWM_OverrideHighEnable(PWM_SEC_2);
           
    PWM_OverrideLowEnable(PWM_PRI_1);
    PWM_OverrideLowEnable(PWM_SEC_1);
    PWM_OverrideLowEnable(PWM_PRI_2);
    PWM_OverrideLowEnable(PWM_SEC_2); 
    
    // Set update request of the last PWM in the cascade 
    // to update all pwm registers
    PWM_SoftwareUpdateRequest(PWM_SEC_2);
    
}
