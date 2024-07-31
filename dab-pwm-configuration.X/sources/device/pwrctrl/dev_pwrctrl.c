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
#include "dev_pwrctrl_typedef.h"
#include "dev_pwrctrl_pwm.h"
#include "device/fault/dev_fault.h"

POWER_CONTROL_t dab;

static void Dev_PwrCtrl_ControlLoopInitialize(void);
extern void Dev_PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance);

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_Initialize(void)
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
    dab.Pwm.DeadTimeHigh = PG1DTH; // ToDo: remove the register dependency
    dab.Pwm.DeadTimeHigh = PG1DTL; // ToDo: remove the register dependency
    dab.Pwm.PBVPeriodTarget = MIN_PWM_PERIOD;
    dab.Pwm.PBVControlPhaseTarget = 0;
    
    //Initialize the DAB to charging state
    dab.PowerDirection =  PWR_CTRL_CHARGING;
    
    //Initialize Power Control Loop
    Dev_PwrCtrl_ControlLoopInitialize();
    
    // Disable PWM output by setting the PWM override bits to High
    Dev_PwrCtrl_PWM_Disable(&dab); 
    
    //Enable PWM peripheral
    PWM_Enable();
    
    // Update the Period, Duty Cycle and Phases of the PWMs based on
    // the given Control period and Control Phase
    Dev_PwrCtrl_PWM_Update(&dab);
}

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_Execute(void)
{
    //ToDo: review this later
    // short circuit fault checks (primary and secondary over current via comparators)
//    Drv_PwrCtrl_Fault_ShortCircuit(&dab);
    
    Dev_PwrCtrl_StateMachine(&dab);
}

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_Suspend(void)
{
    //Disable PWM peripheral
    PWM_Disable();
}

void Dev_PwrCtrl_ControlLoopInitialize(void)
{
    // Initialize voltage loop compensator
    Dev_PwrCtrl_VComp_Initialize();
    
    // Initialize current loop compensator
    Dev_PwrCtrl_IComp_Initialize();
    
    dab.VLoop.AgcFactor = 0x7FFF;
    dab.VLoop.Feedback = 0;
    dab.VLoop.Output = 0;
    dab.VLoop.Reference = 0;
    dab.VLoop.ReferenceTarget = 0;
    
    dab.ILoop.AgcFactor = 0x7FFF;
    dab.ILoop.Feedback = 0;
    dab.ILoop.Output = 0;
    dab.ILoop.Reference = 0;
    dab.ILoop.ReferenceTarget = 0;
}