/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
 * @file      pwrctrl_sm.c
 * @ingroup   pwrctrl-sm   
 * @brief     Contains power control state machine functions that is 
 *  executed every 100us.
 */

#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types
                               
// DAB header files
#include "pwrctrl.h"
#include "config/macros.h"
#include "device/device.h"
#include "fault/fault.h"
#include "dcdt/pwrctrl_dcdt.h"

//PRIVATE FUNCTIONS
static void PCS_INIT_handler(POWER_CONTROL_t* pcInstance);
static void PCS_WAIT_IF_FAULT_ACTIVE_handler(POWER_CONTROL_t* pcInstance);
static void PCS_STANDBY_handler(POWER_CONTROL_t* pcInstance);
static void PCS_SOFT_START_handler(POWER_CONTROL_t* pcInstance);
static void PCS_UP_AND_RUNNING_handler(POWER_CONTROL_t* pcInstance);

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Manages the power control state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function manages the state machine of the converter. There are
 *  five states in this function as follows:
 *      - STATE_INITIALIZE - This state resets the conditional flag bits, 
 *  ensures PWM output is disabled and run the initial current calibration offset.
 *      - STATE_FAULT_DETECTION -  This state checks if there is fault event that occurred.
 *      - STATE_STANDBY This state waits until there is no fault event that has occurred 
 *  and when the power control enable bit is set.
 *      - STATE_SOFT_START - This state gradually ramps up the references of the power control.
 *  The control loop references are gradually incremented until in reached the 
 *  desired power control reference.
 *      - STATE_ONLINE -  This state keeps checks if there is fault event that occurred,
 *  if power control Enable has been disabled and if there is changes in the 
 *  power control references.
 * 
 *********************************************************************************/
void PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance)
{ 
    switch (pcInstance->State)
    {
        case PWRCTRL_STATE_INITIALIZE:
            PCS_INIT_handler(pcInstance);
            break;    

        case PWRCTRL_STATE_FAULT_DETECTION:
            PCS_WAIT_IF_FAULT_ACTIVE_handler(pcInstance);
            break;

        case PWRCTRL_STATE_STANDBY:
            PCS_STANDBY_handler(pcInstance);
            break;

        case PWRCTRL_STATE_SOFT_START:
            PCS_SOFT_START_handler(pcInstance);
            break;

        case PWRCTRL_STATE_ONLINE:
            PCS_UP_AND_RUNNING_handler(pcInstance);
            break;

        default:
            pcInstance->State = PWRCTRL_STATE_INITIALIZE;
            break;      
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes function for initialze state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function resets the conditional flag bits, ensures PWM output is
 *  disabled and run the initial current calibration offset. After this, the 
 *  state machine moves to checking the fault handler.
 *********************************************************************************/
static void PCS_INIT_handler(POWER_CONTROL_t* pcInstance)
{
    #if (CURRENT_CALIBRATION == true)    
    // Execute current sensor offset calibration
    Dev_CurrentSensorOffsetCal();
    
    // Checks if the calibration is complete
    if (Dev_CurrentSensor_Get_CalibrationStatus())
    #endif
    {
        // Current sensor calibration is complete. Update the offset of the current sensor
        pcInstance->Data.ISecSensorOffset = Dev_CurrentSensor_Get_Offset();
        
        // Ensure PWM output is disabled
        PwrCtrl_PWM_Disable();
        
        // Reset fault objects status bits
        Fault_Reset();
        
        // Clear power control fault active bit
        pcInstance->Status.bits.FaultActive = 0;
        // Clear power control running bit
        pcInstance->Status.bits.Running = 0;
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // Next State assigned to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }   
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes the fault handler state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function checks if there is fault event that occurred. When
 *  there is no fault event, the state machine moves to StandBy state. 
 * 
 *********************************************************************************/
static void PCS_WAIT_IF_FAULT_ACTIVE_handler(POWER_CONTROL_t* pcInstance)
{   
    if (pcInstance->Fault.FaultDetected == 0)
    {
        pcInstance->Status.bits.FaultActive = 0;
        pcInstance->State = PWRCTRL_STATE_STANDBY; // next state
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes Standby State machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function waits until there is no fault event that has occurred 
 *  and when the power control enable bit is set. When Enable bit is set,  
 *  reset the fault objects status bits, reset PWM control settings, enable
 *  the power control running bit, enable PWM physical output, initialize 
 *  control loop references and then move to the next state STATE_SOFT_START. 
 * 
 * @note    In this application the power control enable bit is controlled 
 *  externally by Power Board Visualizer.  
 * 
 *********************************************************************************/
static void PCS_STANDBY_handler(POWER_CONTROL_t* pcInstance)
{
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
    
    // NOTE: Power control enable is controlled externally 
    else if (pcInstance->Properties.Enable)
    {
        // Reset fault objects status bits
        Fault_Reset();
            
        // Reset the power control properties and control loop histories
        PwrCtrl_Reset();

        // Enable current control loop
        pcInstance->ILoop.Enable = true;

        // Update PWM distribution
        PwrCtrl_PWM_Update(&dab);
        
        // Enable PWM physical output
        PwrCtrl_PWM_Enable();
        
        // Enable power control running bit
        pcInstance->Status.bits.Running = 1;
        
        // Next State assigned to STATE_SOFT_START
        pcInstance->State = PWRCTRL_STATE_SOFT_START;
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes the power control soft start state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function gradually ramps up the references of the power control.
 *  The control loop references are gradually incremented until in reached the 
 *  desired power control reference. When this is achieved, the next state will  
 *  be assigned to STATE_ONLINE. 
 * 
 *********************************************************************************/
static void PCS_SOFT_START_handler(POWER_CONTROL_t* pcInstance)
{
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
  
    // Check if Enable bit has been cleared
    else if (!pcInstance->Properties.Enable) 
    {
        // Disable PWM physical output
        PwrCtrl_PWM_Disable();
        
        // Clear power control running bit
        pcInstance->Status.bits.Running = 0;
        
        // State back to STATE_STANDBY
        pcInstance->State = PWRCTRL_STATE_STANDBY; 
    }
    
    else
    {   
        // Ramp Up the Voltage, Current and Power reference
        PwrCtrl_RampReference(&pcInstance->VRamp);
        PwrCtrl_RampReference(&pcInstance->IRamp);
        PwrCtrl_RampReference(&pcInstance->PRamp);

        // Check if ramp up is complete
        if ((pcInstance->VRamp.RampComplete) && (pcInstance->IRamp.RampComplete)
              && (pcInstance->PRamp.RampComplete))
            // Next State assigned to STATE_ONLINE
            pcInstance->State = PWRCTRL_STATE_ONLINE; 

    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes the Online state
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function keeps checks if there is fault event that occurred,
 *  if power control Enable has been disabled and if there is changes in the 
 *  power control references. 
 *********************************************************************************/
static void PCS_UP_AND_RUNNING_handler(POWER_CONTROL_t* pcInstance)
{    
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
    
    else
    {
        // Check if Enable bit has been cleared
        if (!pcInstance->Properties.Enable) 
        {
            // Disable PWM physical output
            PwrCtrl_PWM_Disable();

            // Clear power control running bit
            pcInstance->Status.bits.Running = 0;

            // State back to STATE_INITIALIZE
            pcInstance->State = PWRCTRL_STATE_INITIALIZE; 
        }
        
    #if (OPEN_LOOP_PBV == true)
        else if ((pcInstance->Pwm.ControlPeriod & ~(0x7)) != (pcInstance->Pwm.PBVPeriodTarget& ~(0x7)) || 
                (pcInstance->Pwm.ControlPhase != pcInstance->Pwm.PBVControlPhaseTarget))
            pcInstance->State = PWRCTRL_STATE_SOFT_START;
    #endif
            
        // Check if there is change in power control references    
        else if ((pcInstance->ILoop.Reference != pcInstance->Properties.IReference) ||
                (pcInstance->VLoop.Reference != pcInstance->Properties.VSecReference) ||
                (pcInstance->PLoop.Reference != pcInstance->Properties.PwrReference))
            
            // State back to STATE_SOFT_START
            pcInstance->State = PWRCTRL_STATE_SOFT_START;
    }
} 
