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

#include "config/macros.h"
#include "pwm_hs/pwm_hs_types.h"
#include "driver/power_control/drv_pwrctrl_typedef.h"

POWER_CONTROL_t dab;

void App_PwrCtrl_Initialize(void)
{
    // Initialize the PWM instance that the user initialize in MCC
    // for the power control driver
    dab.SwitchNodes[0].Primary.PwmChannel = PWM_GENERATOR_1;
    dab.SwitchNodes[0].Secondary.PwmChannel = PWM_GENERATOR_2;
    dab.SwitchNodes[1].Primary.PwmChannel = PWM_GENERATOR_3;
    dab.SwitchNodes[1].Secondary.PwmChannel = PWM_GENERATOR_4;
    
    // Initialize PWM period
    dab.SwitchNodes[0].Primary.Period = MAX_PWM_PERIOD;
    dab.SwitchNodes[1].Primary.Period = MAX_PWM_PERIOD;
    dab.SwitchNodes[0].Secondary.Period = MAX_PWM_PERIOD;
    dab.SwitchNodes[1].Secondary.Period = MAX_PWM_PERIOD;
    
    // Initialize PWM duty cycle based on Period
    dab.SwitchNodes[0].Primary.DutyCycle = dab.SwitchNodes[0].Primary.Period >> 1;
    dab.SwitchNodes[1].Primary.DutyCycle = dab.SwitchNodes[1].Primary.Period >> 1;
    dab.SwitchNodes[0].Secondary.DutyCycle = dab.SwitchNodes[0].Secondary.Period >> 1;
    dab.SwitchNodes[1].Secondary.DutyCycle = dab.SwitchNodes[1].Secondary.Period >> 1;
    
    // Initialize PWM phase shift 90 degrees phase difference
    // between the cascaded PWM
    dab.SwitchNodes[0].Primary.PhaseShift = dab.SwitchNodes[0].Primary.DutyCycle >> 1;
    dab.SwitchNodes[1].Primary.PhaseShift = dab.SwitchNodes[1].Primary.DutyCycle >> 1;
    dab.SwitchNodes[0].Secondary.PhaseShift = dab.SwitchNodes[0].Secondary.DutyCycle >> 1;
    dab.SwitchNodes[1].Secondary.PhaseShift = dab.SwitchNodes[1].Secondary.DutyCycle >> 1;
}

void App_PwrCtrl_Enable(void)
{
    
}

void App_PwrCtrl_Resume(void)
{
    
}

void App_PwrCtrl_Execute(void)
{
    
}

void App_PwrCtrl_Suspend(void)
{
    
}