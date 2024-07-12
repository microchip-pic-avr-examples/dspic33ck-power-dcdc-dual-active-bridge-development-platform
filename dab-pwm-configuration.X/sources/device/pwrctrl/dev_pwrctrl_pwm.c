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

#include <xc.h> 
#include <stdbool.h>
#include "pwm_hs/pwm.h"
#include "dev_pwrctrl_typedef.h"

/*********************************************************************************
 * @ingroup 
 * @fn     void Drv_PwrCtrl_PWM_Enable(void)
 * @brief  enable PWM outputs for all full bridges
 * @param   none
 * @return  none
 * @details
 * turn on all potentially active PWMs
 * note that this is an API function
 **********************************************************************************/
void Dev_PwrCtrl_PWM_Enable(POWER_CONTROL_t* pcInstance)
{
        
    // turn on PWM outputs by disabling the output override on each high and low PWM output
    PWM_OverrideHighDisable(pcInstance->Pwm.Primary_1);
    PWM_OverrideHighDisable(pcInstance->Pwm.Primary_2);
    PWM_OverrideHighDisable(pcInstance->Pwm.Secondary_1);
    PWM_OverrideHighDisable(pcInstance->Pwm.Secondary_2);
           
    PWM_OverrideLowDisable(pcInstance->Pwm.Primary_1);
    PWM_OverrideLowDisable(pcInstance->Pwm.Primary_2);
    PWM_OverrideLowDisable(pcInstance->Pwm.Secondary_1);
    PWM_OverrideLowDisable(pcInstance->Pwm.Secondary_2); 
    
}


/*********************************************************************************
 * @ingroup 
 * @fn     void Drv_PwrCtrl_PWM_Disable(void)
 * @brief  disable PWM outputs for all full bridges
 * @param   none
 * @return  none
 * @details
 * turn off active all potentially active PWMs
 * note that this is an API function
 **********************************************************************************/
void Dev_PwrCtrl_PWM_Disable(POWER_CONTROL_t* pcInstance)
{
    // turn on all PWM pin output overrides, this sets PWM pin to 0
    // even if PWM peripheral is enabled and running in the background
    PWM_OverrideHighEnable(pcInstance->Pwm.Primary_1); 
    PWM_OverrideLowEnable(pcInstance->Pwm.Primary_1); 

    PWM_OverrideHighEnable(pcInstance->Pwm.Primary_2); 
    PWM_OverrideLowEnable(pcInstance->Pwm.Primary_2); 

    PWM_OverrideHighEnable(pcInstance->Pwm.Secondary_1); 
    PWM_OverrideLowEnable(pcInstance->Pwm.Secondary_1); 

    PWM_OverrideHighEnable(pcInstance->Pwm.Secondary_2); 
    PWM_OverrideLowEnable(pcInstance->Pwm.Secondary_2);   
    
    // set update request of the last PWM in the cascade to update all pwm registers
    PWM_SoftwareUpdateRequest(pcInstance->Pwm.Secondary_2);
    
}

/*********************************************************************************
 * @ingroup 
 * @fn     void Drv_PwrCtrl_PWM_Primary_Enable(void)
 * @brief  enable PWM outputs for primary full bridge
 * @param   none
 * @return  none
 * @details
 * turn on all primary side PWMs
 * note that this is an API function
 **********************************************************************************/
void Dev_PwrCtrl_PWM_Primary_Enable(POWER_CONTROL_t* pcInstance)
{
 
    // turn on primary side PWM outputs by disabling the output overrides
    PWM_OverrideHighDisable(pcInstance->Pwm.Primary_1);
    PWM_OverrideLowDisable(pcInstance->Pwm.Primary_1);
    PWM_OverrideHighDisable(pcInstance->Pwm.Primary_2);
    PWM_OverrideLowDisable(pcInstance->Pwm.Primary_2);
    
    // turn off secondary PWM outputs by enabling the output overrides
    PWM_OverrideHighEnable(pcInstance->Pwm.Secondary_1);
    PWM_OverrideLowEnable(pcInstance->Pwm.Secondary_1);
    PWM_OverrideHighEnable(pcInstance->Pwm.Secondary_2);
    PWM_OverrideLowEnable(pcInstance->Pwm.Secondary_2);
}

/*********************************************************************************
 * @fn     void Drv_PwrCtrl_PWM_Secondary_Enable(void)
 * @brief  enable PWM output pins for secondary full bridge
 * @param   none
 * @return  none
 * @details
 * turn on all secondary side PWMs pins
 * note that this is an API function
 **********************************************************************************/
void Dev_PwrCtrl_PWM_Secondary_Enable(POWER_CONTROL_t* pcInstance)
{
    // turn on secondary side PWM outputs by disabling the PWM pin override
    PWM_OverrideHighDisable(pcInstance->Pwm.Secondary_1);
    PWM_OverrideLowDisable(pcInstance->Pwm.Secondary_1);
    PWM_OverrideHighDisable(pcInstance->Pwm.Secondary_2);
    PWM_OverrideLowDisable(pcInstance->Pwm.Secondary_2);  
}


/*********************************************************************************
 * @fn     void Drv_PwrCtrl_PWM_Update(void)
 * @brief  update PWM registers
 * @param   none
 * @return  none
 * @details
 * update period, duty cycle and phase registers for primary and secondary side PWMs
 **********************************************************************************/
void Dev_PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance)
{   
     // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
    // (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    uint16_t PeriodMask = 0x7; 
    
    // Mask the calculated frequency bits [2:0] to make the cascaded/synchronous
    // PWM scheme reliable (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    pcInstance->Pwm.ControlPeriod = pcInstance->Pwm.ControlPeriod & ~(PeriodMask);
    
    // calculate Duty Cycle for 50%
    pcInstance->Pwm.ControlDutyCycle = (pcInstance->Pwm.ControlPeriod >> 1);

    // Calculate the DAB Primary to Secondary Phase ((Frequency / 4) - (Control Phase /2))
    uint16_t PrimarySecondaryPhase = (pcInstance->Pwm.ControlDutyCycle >> 1) - 
            (pcInstance->Pwm.ControlPhase >> 1);
    
    // Calculate the bridge delay ((Frequency / 2) - Primary to Secondary Phase + Control Phase)
    // Note that in the cascaded PWM, the reference phase of the client PWM, is its trigger source
    uint16_t PrimaryPhaseDelay = (pcInstance->Pwm.ControlDutyCycle - PrimarySecondaryPhase) + 
            pcInstance->Pwm.ControlPhase;
    
    // Set the PWM trigger with the calculated PWM phases
    PWM_TriggerCCompareValueSet(pcInstance->Pwm.Primary_1, PrimarySecondaryPhase);
    PWM_TriggerCCompareValueSet(pcInstance->Pwm.Secondary_1, PrimaryPhaseDelay);
    PWM_TriggerCCompareValueSet(pcInstance->Pwm.Primary_2, PrimarySecondaryPhase);

    // Set the PWM Duty Cycle at 50% with the given Frequency
    PWM_DutyCycleSet(pcInstance->Pwm.Primary_1, pcInstance->Pwm.ControlDutyCycle);
    PWM_DutyCycleSet(pcInstance->Pwm.Secondary_1, pcInstance->Pwm.ControlDutyCycle);
    PWM_DutyCycleSet(pcInstance->Pwm.Primary_2, pcInstance->Pwm.ControlDutyCycle);
    PWM_DutyCycleSet(pcInstance->Pwm.Secondary_2, pcInstance->Pwm.ControlDutyCycle);
    
    // Set the PWM Frequency
    PWM_PeriodSet(pcInstance->Pwm.Primary_1, pcInstance->Pwm.ControlPeriod);
    PWM_PeriodSet(pcInstance->Pwm.Secondary_1, pcInstance->Pwm.ControlPeriod);
    PWM_PeriodSet(pcInstance->Pwm.Primary_2, pcInstance->Pwm.ControlPeriod);
    PWM_PeriodSet(pcInstance->Pwm.Secondary_2, pcInstance->Pwm.ControlPeriod);

    // Set the Update bit of the last PWM in the cascaded approach to broadcast
    // it to the other PWMs
    PWM_SoftwareUpdateRequest(pcInstance->Pwm.Secondary_2);
    
}