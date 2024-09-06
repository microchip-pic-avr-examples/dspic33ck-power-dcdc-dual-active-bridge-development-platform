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
#include "dcdt/dev_pwrctrl_dcdt.h"

/******************************************************************************
 * @ingroup dev-pwrctrl-properties-public
 * @brief Global data object for a DAB Converter
 * 
 * @details The 'dab' data object holds all status, control and monitoring values 
 * of the POWER_CONTROL_t power controller. 
 ******************************************************************************/
POWER_CONTROL_t dab;    // Declare DAB converter data structure

// PRIVATE FUNCTIONS
static void Dev_PwrCtrl_StartUpInitialize(void);

extern void Dev_PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance);

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-public
 * @brief  Initialize the power control parameters
 * @return void
 * 
 * @details This function initialize the power control PWM instances, settings, 
 * start-up configuration and control loop configuration. 
 *********************************************************************************/
void Dev_PwrCtrl_Initialize(void)
{
    dab.Properties.VPriReference = 0;
    dab.Properties.VSecReference = 0;
    dab.Properties.IReference = 0;
    dab.Properties.PwrReference = 0;
    
    // Initialize the PWM instance that the user initialize in MCC
    // for the power control driver
    dab.Pwm.Primary_1 = PWM_GENERATOR_1;
    dab.Pwm.Primary_2 = PWM_GENERATOR_3;
    dab.Pwm.Secondary_1 = PWM_GENERATOR_2;
    dab.Pwm.Secondary_2 = PWM_GENERATOR_4;
    
    // Initialize the Control Period and Control Phase during start-up
    dab.Pwm.ControlPeriod = MAX_PWM_PERIOD;
    dab.Pwm.ControlPhase = 0;
    dab.Pwm.DeadTimeHigh = PG1DTH; // ToDo: remove the register dependency
    dab.Pwm.DeadTimeLow = PG1DTL; // ToDo: remove the register dependency
    dab.Pwm.PBVPeriodTarget = MAX_PWM_PERIOD;
    dab.Pwm.PBVControlPhaseTarget = 0;
    
    // Initialize the DAB to charging state
    dab.PowerDirection = PWR_CTRL_CHARGING;
    
    // Initialize Start-Up ramp settings
    Dev_PwrCtrl_StartUpInitialize();
            
    // Initialize Power Control Loop
    Dev_PwrCtrl_ControlLoopInitialize();
    
    // Disable PWM output by setting the PWM override bits to High
    Dev_PwrCtrl_PWM_Disable(&dab); 
    
    // Enable PWM peripheral
    PWM_Enable();
    
    // Update the Period, Duty Cycle and Phases of the PWMs based on
    // the given Control period and Control Phase
    Dev_PwrCtrl_PWM_Update(&dab);
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-public
 * @brief  Executes the power control state machine
 * @return void
 * 
 * @details After initialization this function has to be called on a deterministic, 
 *  constant time base. Each execution step, this function will call the power control
 *  state machine.
 *********************************************************************************/
void Dev_PwrCtrl_Execute(void)
{
    //ToDo: review this later
    // short circuit fault checks (primary and secondary over current via comparators)
//    Drv_PwrCtrl_Fault_ShortCircuit(&dab);
    
    Dev_PwrCtrl_StateMachine(&dab);
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief  Initializes the control loop
 * @return void
 * 
 * @details This function initializes the control loop necessary to run the close loop
 * operation of the converter. 
 *********************************************************************************/
void Dev_PwrCtrl_ControlLoopInitialize(void)
{
    // Initialize voltage loop compensator
    Dev_PwrCtrl_VComp_Initialize();
    
    // Initialize current loop compensator
    Dev_PwrCtrl_IComp_Initialize();
    
    // Initialize power loop compensator
    Dev_PwrCtrl_PComp_Initialize();
    
    dab.VLoop.Enable = false;
    dab.VLoop.AgcFactor = 0x7FFF;
    dab.VLoop.Feedback = 0;
    dab.VLoop.Output = 0;
    dab.VLoop.Reference = 0;

    dab.ILoop.Enable = false;
    dab.ILoop.AgcFactor = 0x7FFF;
    dab.ILoop.Feedback = 0;
    dab.ILoop.Output = 0;
    dab.ILoop.Reference = 0;
    
    dab.PLoop.Enable = false;
    dab.PLoop.AgcFactor = 0x7FFF;
    dab.PLoop.Feedback = 0;
    dab.PLoop.Output = 0;
    dab.PLoop.Reference = 0;
    
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief  Initialize the StartUp Ramp configuration
 * @return void
 * 
 * @details This function initializes the Ramp Up configuration for voltage, current
 * and power. For each parameters that would be ramped up, pointer to the reference,
 * reference target pointer, increment/decrement step size, delay for each increment/decremnt
 * counter for each execution need to be defined. When the reference becomes equal to 
 * the reference targe, the rampComplete data member will be set. 
 *********************************************************************************/
static void Dev_PwrCtrl_StartUpInitialize(void)
{
    // Initialize Voltage ramp-up settings
    dab.VRamp.ptrReference = (uint16_t*)&dab.VLoop.Reference;
    dab.VRamp.ptrReferenceTarget = &dab.Properties.VSecReference;
    dab.VRamp.StepSize = 1;
    dab.VRamp.Delay = 0;
    dab.VRamp.Counter = 0;
    dab.VRamp.RampComplete = 0;
    
    //Initialize Current ramp-up settings
    dab.IRamp.ptrReference = (uint16_t*)&dab.ILoop.Reference;
    dab.IRamp.ptrReferenceTarget = (uint16_t*)&dab.Properties.IReference;
    dab.IRamp.StepSize = 1;
    dab.IRamp.Delay = 0;
    dab.IRamp.Counter = 0;
    dab.IRamp.RampComplete = 0;
    
    //Initialize Power ramp-up settings
    dab.PRamp.ptrReference = (uint16_t*)&dab.PLoop.Reference;
    dab.PRamp.ptrReferenceTarget = &dab.Properties.PwrReference;
    dab.PRamp.StepSize = 1;
    dab.PRamp.Delay = 0;
    dab.PRamp.Counter = 0;
    dab.PRamp.RampComplete = 0;
    
#if (OPEN_LOOP_PBV)
    // Initialize Voltage ramp-up settings for Period control
    dab.VRamp.ptrReference = &dab.Pwm.ControlPeriod;
    dab.VRamp.ptrReferenceTarget = &dab.Pwm.PBVPeriodTarget;
    
    //Initialize Current ramp-up settings for Phase control
    dab.IRamp.ptrReference = &dab.Pwm.ControlPhase;
    dab.IRamp.ptrReferenceTarget = &dab.Pwm.PBVControlPhaseTarget;
#endif
}
