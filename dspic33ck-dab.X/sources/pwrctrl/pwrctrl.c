
/**
 * @file      pwrctrl.c
 * @ingroup   pwrctrl   
 * @brief     Contains power control initialization including control loop 
 *  initialization and start-up initialization, and the power control execution.
 */

#include <xc.h>

//MCC header files
#include "pwm_hs/pwm_hs_types.h"
#include "pwm_hs/pwm.h"

//DAB header files
#include "config/macros.h"
#include "pwrctrl.h"
#include "fault/fault.h"
#include "dcdt/pwrctrl_dcdt.h"

/******************************************************************************
 * @ingroup pwrctrl
 * @brief Global data object for a DAB Converter
 * 
 * @details The 'dab' data object holds all status, control and monitoring values 
 * of the POWER_CONTROL_t power controller. 
 ******************************************************************************/
POWER_CONTROL_t dab;    // Declare DAB converter data structure, singleton pattern

// PRIVATE FUNCTIONS
static void PwrCtrl_StartUpInitialize(void);
static void PwrCtrl_ControlLoopInitialize(void);

// PUBLIC FUNCTIONS
extern void PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance);

/*******************************************************************************
 * @ingroup pwrctrl
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
    dab.PowerDirection = PWR_CTRL_STOP;
    
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
 * @ingroup pwrctrl
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
 * @ingroup pwrctrl
 * @brief  Resets the power control properties
 * @return void
 * 
 * @details This function resets the power control properties including the 
 *  PWM properties and control loop references.
 *********************************************************************************/
void PwrCtrl_Reset(void)
{
    //set the period to maximum  
    dab.Pwm.ControlPeriod = MAX_PWM_PERIOD;
    dab.Pwm.ControlPhase = dab.Pwm.DeadTimeLow;
    dab.Pwm.PBVPeriodTarget = MAX_PWM_PERIOD;
    dab.Pwm.PBVControlPhaseTarget = dab.Pwm.DeadTimeLow;
    
    // Reset the power control references
    dab.Properties.VPriReference = 0;
    dab.Properties.VSecReference = 0;
    dab.Properties.IReference = 0;
    dab.Properties.PwrReference = 0;

    // Initialize current loop reference to 0, to be controlled externally
    dab.ILoop.Reference = 0;
    // Initialize power loop reference to 0, to be controlled externally
    dab.PLoop.Reference = 0;
    // Initialize voltage loop reference to current secondary voltage
    dab.VLoop.Reference = 0;
    
    // Set the AGC to 1
    dab.VLoop.AgcFactor = 0x7FFF;
    dab.ILoop.AgcFactor = 0x7FFF;
    dab.PLoop.AgcFactor = 0x7FFF;
    
    // Reset Control Loop Histories
    PwrCtrl_ResetControlLoopHistories();     
}


/*******************************************************************************
 * @ingroup pwrctrl
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
 * @ingroup pwrctrl
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
    dab.VRamp.ptrReference = &dab.VLoop.Reference;
    

    if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
    {    
        dab.VRamp.ptrReferenceTarget = &dab.Properties.VPriReference;
    }
    else
    {
        dab.VRamp.ptrReferenceTarget = &dab.Properties.VSecReference;
    }    
    
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
    
    #if defined (OPEN_LOOP_PBV) && (OPEN_LOOP_PBV == true)
    // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
    // (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    uint16_t PeriodMask = 0x7; 
    
    // Initialize Voltage ramp-up settings for Period control
    dab.Pwm.ControlPeriod = dab.Pwm.ControlPeriod & ~(PeriodMask);
    dab.Pwm.PBVPeriodTarget = dab.Pwm.PBVPeriodTarget & ~(PeriodMask);
    
    dab.VRamp.ptrReference = &dab.Pwm.ControlPeriod;
    dab.VRamp.ptrReferenceTarget = &dab.Pwm.PBVPeriodTarget;
    dab.VRamp.StepSize = 0xE;
    dab.VRamp.Delay = 0;
    
    //Initialize Current ramp-up settings for Phase control
    dab.Pwm.ControlPhase = dab.Pwm.ControlPhase & ~(PeriodMask);
    dab.Pwm.PBVControlPhaseTarget = dab.Pwm.PBVControlPhaseTarget & ~(PeriodMask);
    
    dab.IRamp.ptrReference = &dab.Pwm.ControlPhase;
    dab.IRamp.ptrReferenceTarget = &dab.Pwm.PBVControlPhaseTarget;
    dab.IRamp.StepSize = 0xE;
    dab.IRamp.Delay = 0;
    
#endif
}

