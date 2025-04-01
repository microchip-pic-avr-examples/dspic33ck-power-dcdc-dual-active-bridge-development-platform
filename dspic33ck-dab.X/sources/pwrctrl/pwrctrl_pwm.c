
/**
 * @file      pwrctrl_pwm.c
 * @ingroup   pwrctrl-pwm   
 * @brief     Contains DAB control phase calculation between primary and 
 *  secondary, and the PWM distribution.
 */

#include <xc.h> 
#include <stdbool.h>
#include "pwrctrl.h"
#include "pwm_hs/pwm.h"
#include "pwrctrl_typedef.h"
#include "../driver/mcc_extension/drv_mcc_extension_pwm.h"
/*******************************************************************************
 * @ingroup pwrctrl-pwm
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
    uint16_t PrimarySecondaryPhase;
    uint16_t PrimaryPhaseDelay;
    
    
    
    // Mask the calculated frequency bits [2:0] to make the cascaded/synchronous
    // PWM scheme reliable (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    pcInstance->Pwm.ControlPeriod = pcInstance->Pwm.ControlPeriod & ~(PeriodMask);
    
    // Calculate Duty Cycle for 50%
    pcInstance->Pwm.ControlDutyCycle = (pcInstance->Pwm.ControlPeriod >> 1);
    
    // Maximum Clamping for control phase
    if(pcInstance->Pwm.ControlPhase >  pcInstance->Pwm.ControlDutyCycle)
    {
        pcInstance->Pwm.ControlPhase =  pcInstance->Pwm.ControlDutyCycle;
    }
    
    
    if(dab.PowerDirection == PWR_CTRL_CHARGING)
    {

        PrimarySecondaryPhase = (pcInstance->Pwm.ControlPhase >> 1);


        PrimaryPhaseDelay = (pcInstance->Pwm.ControlPhase >> 1);//  (pcInstance->Pwm.ControlDutyCycle - PrimarySecondaryPhase) + pcInstance->Pwm.ControlPhase;            

    }
    else   
    if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
    {

        PrimarySecondaryPhase = pcInstance->Pwm.ControlDutyCycle - (pcInstance->Pwm.ControlPhase >> 1);

        PrimaryPhaseDelay = 3*(pcInstance->Pwm.ControlPhase >> 1);//(pcInstance->Pwm.ControlDutyCycle - PrimarySecondaryPhase) + pcInstance->Pwm.ControlPhase;
        
    }
    else
    {
        PrimarySecondaryPhase = 0;
        PrimaryPhaseDelay =0;
    }
    
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
 * @ingroup pwrctrl-pwm
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
 * @ingroup pwrctrl-pwm
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


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  Forward/Reverse Direction related swap pattern on PWM output
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function swaps H and L of the physical PWM output by setting the swap
 *  bits of the PWM module.
 *********************************************************************************/
void PwrCtrl_PWM_DAB_DirectionSpecificSwapCfg(PWR_CTRL_CHARGE_STATE_t direction)
{   
    // Turn-Off PWM outputs by enabling the output override 
    // on each high and low PWM output
    PwrCtrl_PWM_Disable();
 
    PWM_Disable();
 
    if(direction == PWR_CTRL_CHARGING)
    {
//    forward / charger mode        
        PWM_Swap_PWMxL_and_PWMxH(PWM_PRI_1, false);
        PWM_Swap_PWMxL_and_PWMxH(PWM_PRI_2, false);
        PWM_Swap_PWMxL_and_PWMxH(PWM_SEC_1, false);
        PWM_Swap_PWMxL_and_PWMxH(PWM_SEC_2, false);        
    }    
    if(direction == PWR_CTRL_DISCHARGING)
    {
//     reverse / discharger mode
        PWM_Swap_PWMxL_and_PWMxH(PWM_PRI_1, false);
        PWM_Swap_PWMxL_and_PWMxH(PWM_PRI_2, true);
        PWM_Swap_PWMxL_and_PWMxH(PWM_SEC_1, true);
        PWM_Swap_PWMxL_and_PWMxH(PWM_SEC_2, false);       
    }  
    
 PWM_Enable();
    // Set update request of the last PWM in the cascade 
    // to update all pwm registers
    PWM_SoftwareUpdateRequest(PWM_SEC_2);
    
}

