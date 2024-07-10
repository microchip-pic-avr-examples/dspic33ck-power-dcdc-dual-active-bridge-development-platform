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
#include "pwm_hs/pwm.h"
#include "driver/power_control/drv_pwrctrl_typedef.h"
#include "driver/power_control/drv_pwrctrl_pwm.h"

POWER_CONTROL_t dab;

void App_PwrCtrl_Initialize(void)
{
    // Initialize the PWM instance that the user initialize in MCC
    // for the power control driver
    dab.Pwm.Primary_1 = PWM_GENERATOR_1;
    dab.Pwm.Primary_2 = PWM_GENERATOR_3;
    dab.Pwm.Secondary_1 = PWM_GENERATOR_2;
    dab.Pwm.Secondary_2 = PWM_GENERATOR_4;
    
    // Initialize the Control Period and Control Phase during start-up
    dab.Pwm.ControlPeriod = MIN_PWM_PERIOD;
    dab.Pwm.ControlPhase = 0;
   
    // set all PWM output pins to 0
    Drv_PwrCtrl_PWM_Disable(&dab); 
    
    // Update the Period, Duty Cycle and Phases of the PWMs based on
    // the given Control period and Control Phase
    Drv_PwrCtrl_PWM_Update(&dab);
    
}

void App_PwrCtrl_Enable(void)
{
    //Enable PWM peripheral
    PWM_Enable();
    
    //Enable PWM output
    Drv_PwrCtrl_PWM_Enable(&dab);
}


void App_PwrCtrl_Execute(void)
{
    
}

void App_PwrCtrl_Suspend(void)
{
    
}

/*********************************************************************************
 * @ingroup 
 * @fn     void Drv_PwrCtrl_Init(void)
 * @brief  umbrella initialization function for power controller 
 * @param   none
 * @return  none
 * @details
 **********************************************************************************/
//void Drv_PwrCtrl_Init(void)
//{
//    // initialize the current loop compensator
//    Drv_PwrCtrl_Icomp_Init();
//    
//           
//    // initialize fault protection
//    Drv_PwrCtrl_FaultInit();
//#ifndef FAULT_SHORT_CCT_DISABLE
//    // initialize short circuit fault protection with comparators
//    Drv_PwrCtrl_Fault_EnableShortCircuitProtection();
//#endif // #ifndef FAULT_SHORT_CCT_DISABLE
//    // clear the fault PCI for each PWM
//    Drv_PwrCtrl_Fault_ClearHardwareFaults();    
//}