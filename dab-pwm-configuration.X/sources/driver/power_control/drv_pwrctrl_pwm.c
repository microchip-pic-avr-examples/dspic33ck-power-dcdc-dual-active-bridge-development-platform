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
#include "drv_pwrctrl_typedef.h"

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
void Drv_PwrCtrl_PWM_Enable(POWER_CONTROL_t* pcInstance)
{
//    PG1IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG1IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
//    PG2IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG2IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
//    PG3IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG3IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
//    PG4IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG4IOCONHbits.PENL = 1; // WMxL Output Port Enable: PWM generator controls the PWMxL output pin
        
    // turn on PWM outputs by disabling the output override on each high and low PWM output
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[0].Primary.PwmChannel);
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[1].Primary.PwmChannel);
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[0].Secondary.PwmChannel);
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[1].Secondary.PwmChannel);
           
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[0].Primary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[1].Primary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[0].Secondary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[1].Secondary.PwmChannel); 
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
void Drv_PwrCtrl_PWM_Disable(POWER_CONTROL_t* pcInstance)
{
    // turn on all PWM pin output overrides, this sets PWM pin to 0
    // even if PWM peripheral is enabled and running in the background
    PWM_OverrideHighEnable(pcInstance->SwitchNodes[0].Primary.PwmChannel); 
    PWM_OverrideLowEnable(pcInstance->SwitchNodes[0].Primary.PwmChannel); 

    PWM_OverrideHighEnable(pcInstance->SwitchNodes[1].Primary.PwmChannel); 
    PWM_OverrideLowEnable(pcInstance->SwitchNodes[1].Primary.PwmChannel); 

    PWM_OverrideHighEnable(pcInstance->SwitchNodes[0].Secondary.PwmChannel); 
    PWM_OverrideLowEnable(pcInstance->SwitchNodes[0].Secondary.PwmChannel); 

    PWM_OverrideHighEnable(pcInstance->SwitchNodes[1].Secondary.PwmChannel); 
    PWM_OverrideLowEnable(pcInstance->SwitchNodes[1].Secondary.PwmChannel);   
    
    // set update request of the last PWM in the cascade to update all pwm registers
    PWM_SoftwareUpdateRequest(pcInstance->SwitchNodes[1].Secondary.PwmChannel);
    
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
void Drv_PwrCtrl_PWM_Primary_Enable(POWER_CONTROL_t* pcInstance)
{
//    PG1IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG1IOCONHbits.PENL = 1; // PWMxL Output Port Enable: PWM generator controls the PWMxL output pin
//    PG3IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG3IOCONHbits.PENL = 1; // PWMxL Output Port Enable: PWM generator controls the PWMxL output pin
 
    // turn on primary side PWM outputs by disabling the output overrides
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[0].Primary.PwmChannel);
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[1].Primary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[0].Primary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[1].Primary.PwmChannel);
    
    // turn off secondary PWM outputs by enabling the output overrides
    PWM_OverrideHighEnable(pcInstance->SwitchNodes[0].Secondary.PwmChannel);
    PWM_OverrideHighEnable(pcInstance->SwitchNodes[1].Secondary.PwmChannel);
    PWM_OverrideLowEnable(pcInstance->SwitchNodes[0].Secondary.PwmChannel);
    PWM_OverrideLowEnable(pcInstance->SwitchNodes[1].Secondary.PwmChannel);
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
void Drv_PwrCtrl_PWM_Secondary_Enable(POWER_CONTROL_t* pcInstance)
{
//    PG2IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG2IOCONHbits.PENL = 1; // PWMxL Output Port Enable: PWM generator controls the PWMxL output pin
//    PG4IOCONHbits.PENH = 1; // PWMxH Output Port Enable: PWM generator controls the PWMxH output pin
//    PG4IOCONHbits.PENL = 1; // PWMxL Output Port Enable: PWM generator controls the PWMxL output pin
 
    // turn on secondary side PWM outputs by disabling the PWM pin override
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[0].Secondary.PwmChannel);
    PWM_OverrideHighDisable(pcInstance->SwitchNodes[1].Secondary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[0].Secondary.PwmChannel);
    PWM_OverrideLowDisable(pcInstance->SwitchNodes[1].Secondary.PwmChannel);  
}