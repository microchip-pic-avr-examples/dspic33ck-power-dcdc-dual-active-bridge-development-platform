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
 * @file      dev_pwrctrl.c
 * @ingroup   dev-pwrctrl   
 * @brief     Contains power control initialization including control loop 
 *  initialization and start-up initialization, and the power control execution.
 */

#include <xc.h>

#include "config/macros.h"
#include "pwm_hs/pwm_hs_types.h"
#include "pwm_hs/pwm.h"
#include "pwrctrl_typedef.h"
#include "pwrctrl_pwm.h"
#include "fault/dev_fault.h"
#include "dcdt/pwrctrl_dcdt.h"

/******************************************************************************
 * @ingroup dev-pwrctrl
 * @brief Global data object for a DAB Converter
 * 
 * @details The 'dab' data object holds all status, control and monitoring values 
 * of the POWER_CONTROL_t power controller. 
 ******************************************************************************/
POWER_CONTROL_t dab;    // Declare DAB converter data structure

// PRIVATE FUNCTIONS
static void PwrCtrl_StartUpInitialize(void);
static void PwrCtrl_ControlLoopInitialize(void);

// PUBLIC FUNCTIONS
extern void PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance);

/*******************************************************************************
 * @ingroup dev-pwrctrl
 * @brief  Initialize the power control parameters
 * @return void
 * 
 * @details This function initialize the power control PWM instances, settings, 
 * start-up configuration and control loop configuration. 
 *******************************************************************************/
void PwrCtrl_Initialize(void)
{   
    // Clear dab references
    dab.Properties.VPriReference = 0;
    dab.Properties.VSecReference = 0;
    dab.Properties.IReference = 0;
    dab.Properties.PwrReference = 0;
    
    // Initialize the Control Period and Control Phase during start-up
    dab.Pwm.ControlPeriod = MAX_PWM_PERIOD;
    dab.Pwm.DeadTimeHigh = MINIMUM_DEADTIME; 
    dab.Pwm.DeadTimeLow = MINIMUM_DEADTIME; 
    dab.Pwm.PBVPeriodTarget = MAX_PWM_PERIOD;
    
    // Initialize the DAB to charging state
    dab.PowerDirection = PWR_CTRL_CHARGING;
    
    // Initialize Start-Up ramp settings
    PwrCtrl_StartUpInitialize();
            
    // Initialize Power Control Loop
    PwrCtrl_ControlLoopInitialize();
    
    // Disable PWM output by setting the PWM override bits to High
    PwrCtrl_PWM_Disable(); 
    
    // Enable PWM peripheral
    PWM_Enable();
    
    // Update the Period, Duty Cycle and Phases of the PWMs based on
    // the given Control period and Control Phase
    PwrCtrl_PWM_Update(&dab);
}

/*******************************************************************************
 * @ingroup dev-pwrctrl
 * @brief  Executes the power control state machine
 * @return void
 * 
 * @details After initialization this function has to be called on a deterministic, 
 *  constant time base. Each execution step, this function will call the power control
 *  state machine.
 *********************************************************************************/
void PwrCtrl_Execute(void)
{
    // Execute the state machine
    PwrCtrl_StateMachine(&dab);
}

/*******************************************************************************
 * @ingroup dev-pwrctrl
 * @brief  Initializes the control loop
 * @return void
 * 
 * @details This function initializes the control loop necessary to run the close loop
 * operation of the converter. 
 *********************************************************************************/
void PwrCtrl_ControlLoopInitialize(void)
{
    // Initialize voltage loop compensator
    PwrCtrl_VComp_Initialize();
    
    // Initialize current loop compensator
    PwrCtrl_IComp_Initialize();
    
    // Initialize power loop compensator
    PwrCtrl_PComp_Initialize();
    
    // Voltage loop properties initialize 
    dab.VLoop.Enable = false;
    dab.VLoop.AgcFactor = 0x7FFF;
    dab.VLoop.Feedback = 0;
    dab.VLoop.Output = 0;
    dab.VLoop.Reference = 0;

    // Current loop properties initialize
    dab.ILoop.Enable = false;
    dab.ILoop.AgcFactor = 0x7FFF;
    dab.ILoop.Feedback = 0;
    dab.ILoop.Output = 0;
    dab.ILoop.Reference = 0;
    
    // Power loop properties initialize
    dab.PLoop.Enable = false;
    dab.PLoop.AgcFactor = 0x7FFF;
    dab.PLoop.Feedback = 0;
    dab.PLoop.Output = 0;
    dab.PLoop.Reference = 0;
    
}

/*******************************************************************************
 * @ingroup dev-pwrctrl
 * @brief  Initialize the StartUp Ramp configuration
 * @return void
 * 
 * @details This function initializes the Ramp Up configuration for voltage, current
 * and power. For each parameters that would be ramped up, pointer to the reference,
 * reference target pointer, increment/decrement step size, delay for each increment/decremnt
 * counter for each execution need to be defined. When the reference becomes equal to 
 * the reference targe, the rampComplete data member will be set. 
 *********************************************************************************/
static void PwrCtrl_StartUpInitialize(void)
{
    // Initialize Voltage ramp-up settings
    dab.VRamp.ptrReference = (uint16_t*)&dab.VLoop.Reference;
    dab.VRamp.ptrReferenceTarget = &dab.Properties.VSecReference;
    dab.VRamp.StepSize = 1;
    dab.VRamp.Delay = 20;
    dab.VRamp.Counter = 0;
    dab.VRamp.RampComplete = 0;
    
    //Initialize Current ramp-up settings
    dab.IRamp.ptrReference = (uint16_t*)&dab.ILoop.Reference;
    dab.IRamp.ptrReferenceTarget = (uint16_t*)&dab.Properties.IReference;
    dab.IRamp.StepSize = 1;
    dab.IRamp.Delay = 60;
    dab.IRamp.Counter = 0;
    dab.IRamp.RampComplete = 0;
    
    //Initialize Power ramp-up settings
    dab.PRamp.ptrReference = (uint16_t*)&dab.PLoop.Reference;
    dab.PRamp.ptrReferenceTarget = &dab.Properties.PwrReference;
    dab.PRamp.StepSize = 1;
    dab.PRamp.Delay = 4;
    dab.PRamp.Counter = 0;
    dab.PRamp.RampComplete = 0;
    
#if (OPEN_LOOP_PBV == true)
    // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
    // (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    uint16_t PeriodMask = 0x7; 
    
    // Initialize Voltage ramp-up settings for Period control
    dab.Pwm.ControlPeriod = dab.Pwm.ControlPeriod & ~(PeriodMask);
    dab.Pwm.PBVPeriodTarget = dab.Pwm.PBVPeriodTarget & ~(PeriodMask);
    
    dab.VRamp.ptrReference = &dab.Pwm.ControlPeriod;
    dab.VRamp.ptrReferenceTarget = &dab.Pwm.PBVPeriodTarget;
    dab.VRamp.StepSize = 0x7;
    dab.VRamp.Delay = 0;
    
    //Initialize Current ramp-up settings for Phase control
    dab.Pwm.ControlPhase = dab.Pwm.ControlPhase & ~(PeriodMask);
    dab.Pwm.PBVControlPhaseTarget = dab.Pwm.PBVControlPhaseTarget & ~(PeriodMask);
    
    dab.IRamp.ptrReference = &dab.Pwm.ControlPhase;
    dab.IRamp.ptrReferenceTarget = &dab.Pwm.PBVControlPhaseTarget;
    dab.IRamp.StepSize = 0x7;
    dab.IRamp.Delay = 0;
    
#endif
}
