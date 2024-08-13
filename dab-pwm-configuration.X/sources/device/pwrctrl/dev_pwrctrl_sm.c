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
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types
                               
#include "system/interrupt.h"

#include "config/macros.h"
#include "dev_pwrctrl_typedef.h"
#include "dev_pwrctrl_pwm.h"
#include "dev_pwrctrl_utils.h"
#include "device/dev_current_sensor.h"
#include "device/fault/dev_fault.h"

static __inline__ void PCS_INIT_handler(POWER_CONTROL_t* pcInstance);
static __inline__ void PCS_WAIT_IF_FAULT_ACTIVE_handler(POWER_CONTROL_t* pcInstance);
static __inline__ void PCS_STANDBY_handler(POWER_CONTROL_t* pcInstance);
static __inline__ void PCS_SOFT_START_handler(POWER_CONTROL_t* pcInstance);
static __inline__ void PCS_UP_AND_RUNNING_handler(POWER_CONTROL_t* pcInstance);


/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_StateMachine(void)
 * @brief   power controller state machine
 * @param   none
 * @return  none 
 * @details
 *
 **********************************************************************************/
void Dev_PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance)
{ 
    switch (pcInstance->State)
    {
        case PWR_CNTRL_STATE_INITIALIZE:
            PCS_INIT_handler(pcInstance);
            break;    

        case PWR_CNTRL_STATE_FAULT_DETECTION:
            PCS_WAIT_IF_FAULT_ACTIVE_handler(pcInstance);
            break;

        case PWR_CNTRL_STATE_STANDBY:
            PCS_STANDBY_handler(pcInstance);
            break;

        case PWR_CNTRL_STATE_SOFT_START:
            PCS_SOFT_START_handler(pcInstance);
            break;

        case PWR_CNTRL_STATE_ONLINE:
            PCS_UP_AND_RUNNING_handler(pcInstance);
            break;

        default:
            pcInstance->State = PWR_CNTRL_STATE_INITIALIZE;
            break;      
    }
}

/*********************************************************************************
 * @ingroup 
 * @fn      static __inline__ void PCS_INIT_handler(void)
 * @brief   initialize state handler
 * @param   none
 * @return  none
 * @details
 *
 **********************************************************************************/
static __inline__ void PCS_INIT_handler(POWER_CONTROL_t* pcInstance)
{
    //ToDo: Mention this to COrmac
    //Change the if define to do emulation test
    #if (CURRENT_CALIBRATION == true)
    // if OPEN_LOOP_POTI is defined, we are running on the digital power development board
    // so in this case ignore the current sensor calibration
    
    Dev_CurrentSensorOffsetCal();
    
    if (Dev_CurrentSensor_Get_CalibrationStatus())
    #endif
    {
        // current sensor calibration is complete. Update the offset of the current sensor
        pcInstance->Data.ISecSensorOffset = Dev_CurrentSensor_Get_Offset();
        
        Dev_PwrCtrl_PWM_Disable(pcInstance);
        Dev_Fault_Reset();
        pcInstance->Status.bits.FaultActive = 0;
        pcInstance->Status.bits.Running = 0;
        pcInstance->Properties.Enable = 0;
        
        pcInstance->ILoop.Enable = true;
        
        pcInstance->State = PWR_CNTRL_STATE_FAULT_DETECTION;
    }   
}

/*********************************************************************************
 * @ingroup 
 * @fn      static __inline__ void PCS_WAIT_IF_FAULT_ACTIVE_handler(void)
 * @brief   wait here until no faults are present
 * @param   none
 * @return  none
 * @details
 *
 **********************************************************************************/
static __inline__ void PCS_WAIT_IF_FAULT_ACTIVE_handler(POWER_CONTROL_t* pcInstance)
{   
    if ((pcInstance->Fault.FaultDetected == 0) && (Drv_PwrCtrl_Fault_SC_Faults_Clear(pcInstance)))
    {
        pcInstance->Status.bits.FaultActive = 0;
        pcInstance->State = PWR_CNTRL_STATE_STANDBY; // next state
    }
}

/*********************************************************************************
 * @ingroup 
 * @fn      static __inline__ void PCS_STANDBY_handler(void)
 * @brief   standby state handler
 * @param   none
 * @return  none
 * @details
 * if CLLC has been instructed to run via run flag, then proceed
 *
 **********************************************************************************/
static __inline__ void PCS_STANDBY_handler(POWER_CONTROL_t* pcInstance)
{
    if (pcInstance->Fault.FaultDetected)
    {
        pcInstance->Properties.Enable = 0; // for now, user has to manually re-start converter after a fault
        pcInstance->State = PWR_CNTRL_STATE_FAULT_DETECTION; // next state
    }
    else if (pcInstance->Properties.Enable) // this flag is generally set externally (via PBV GUI for example)
    {
        Dev_Fault_Reset();

        // reset the PWM settings in Standby mode
        pcInstance->Pwm.ControlPeriod = MAX_PWM_PERIOD;
        pcInstance->Pwm.ControlPhase = 0;
        pcInstance->Pwm.PBVPeriodTarget = MAX_PWM_PERIOD;
        pcInstance->Pwm.PBVControlPhaseTarget = 0;

        pcInstance->Status.bits.Running = 1;
        
        //ToDo: Check with Cormac
        // interrupts temporarily disabled to avoid a conflict with
        // with the compensator update function SMPS_Controller2P2ZUpdate() 
        // called from a higher priority interrupt
        INTERRUPT_GlobalDisable();            
        
        INTERRUPT_GlobalEnable();   
        
        Dev_Fault_ClearHardwareFaults();

        Dev_PwrCtrl_PWM_Enable(pcInstance);
    
        // current loop reference initialize
        pcInstance->ILoop.Reference = 0;
        
        // initialize voltage loop reference to current secondary voltage
        pcInstance->VLoop.Reference = pcInstance->Data.VSecVoltage;
        
        pcInstance->State = PWR_CNTRL_STATE_SOFT_START;   // next state
    }
}

/*********************************************************************************
 * @ingroup 
 * @fn      static __inline__ void PCS_SOFT_START_handler(void)
 * @brief   soft-start handler
 * @param   none
 * @return  none
 * @details
 * manage the soft-start of the controller
 *
 **********************************************************************************/
static __inline__ void PCS_SOFT_START_handler(POWER_CONTROL_t* pcInstance)
{
  if (pcInstance->Fault.FaultDetected)
  {
      pcInstance->Properties.Enable = false;  // for now, user has to manually re-start converter after a fault
      pcInstance->State = PWR_CNTRL_STATE_FAULT_DETECTION; 
  }
  
  else if (!pcInstance->Properties.Enable) 
  {
    Dev_PwrCtrl_PWM_Disable(pcInstance);
    pcInstance->Status.bits.Running = 0;
    pcInstance->State = PWR_CNTRL_STATE_STANDBY; 
  }
  else
  {    
      
    // Soft start the converter
#if (OPEN_LOOP_PBV)
    bool rampComplete = false;
    bool rampCompletePhase = false;
    uint16_t step = 8;  // cannot be below 8 because of the masking
    uint16_t delay = 0; // TODO: parameterize this into a soft-start time
    uint16_t* ptr_reference = (uint16_t*)&pcInstance->Pwm.ControlPeriod;
    uint16_t* ptr_referenceTarget = (uint16_t*)&pcInstance->Pwm.PBVPeriodTarget;
    uint16_t* ptrControlPhaseReference = (uint16_t*)&pcInstance->Pwm.ControlPhase;
    uint16_t* ptrControlPhaseReferenceTarget = (uint16_t*)&pcInstance->Pwm.PBVControlPhaseTarget;
    
    rampCompletePhase =  Dev_PwrCtrl_RampReference(ptrControlPhaseReference, ptrControlPhaseReferenceTarget, 1, delay);
    
    if(rampCompletePhase == true)
        rampComplete = Dev_PwrCtrl_RampReference(ptr_reference, ptr_referenceTarget, step, delay);
    
#else
    uint16_t step = 1;  
    uint16_t delay = 0;
    bool rampIComplete = 0;
    bool rampVComplete = 0;
    bool rampPComplete = 0;
    uint16_t* ptrIreference = (uint16_t*)&pcInstance->ILoop.Reference;
    uint16_t* ptrIreferenceTarget = (uint16_t*)&pcInstance->Properties.IReference;
    uint16_t* ptrVreference = (uint16_t*)&pcInstance->VLoop.Reference;
    uint16_t* ptrVreferenceTarget = (uint16_t*)&pcInstance->Properties.VSecReference;
    uint16_t* ptrPreference = (uint16_t*)&pcInstance->PLoop.Reference;
    uint16_t* ptrPreferenceTarget = (uint16_t*)&pcInstance->Properties.PwrReference;
    
    rampVComplete = Dev_PwrCtrl_RampReference(ptrVreference, ptrVreferenceTarget, step, delay);
    rampIComplete = Dev_PwrCtrl_RampReference(ptrIreference, ptrIreferenceTarget, step, delay);
    rampPComplete = Dev_PwrCtrl_RampReference(ptrPreference, ptrPreferenceTarget, step, delay);
    
    if (rampIComplete && rampVComplete && rampPComplete)
        
#endif 
     
        pcInstance->State = PWR_CNTRL_STATE_ONLINE;  // next state

  }
}

/*********************************************************************************
 * @ingroup 
 * @fn      static __inline__ void PCS_UP_AND_RUNNING_handler(void)
 * @brief   handler for when converter is online
 * @param   none
 * @return  none
 * @details
 * manages the converter when it is online
 *
 **********************************************************************************/
static __inline__ void PCS_UP_AND_RUNNING_handler(POWER_CONTROL_t* pcInstance)
{
    if (pcInstance->Fault.FaultDetected)
    {
        pcInstance->Properties.Enable = false;  // for now, user has to manually re-start converter after a fault
        pcInstance->State = PWR_CNTRL_STATE_FAULT_DETECTION; 
    }
    else
    {
        if (!pcInstance->Properties.Enable)
        {
            Dev_PwrCtrl_PWM_Disable(pcInstance);
            pcInstance->Status.bits.Running = 0;            
            pcInstance->State = PWR_CNTRL_STATE_STANDBY;
        }
        
    #if (OPEN_LOOP_PBV == true)
        else if ((pcInstance->Pwm.ControlPeriod != pcInstance->Pwm.PBVPeriodTarget) || 
                (pcInstance->Pwm.ControlPhase != pcInstance->Pwm.PBVControlPhaseTarget))
    #else
            
        else if ((pcInstance->ILoop.Reference != pcInstance->Properties.IReference) ||
                (pcInstance->VLoop.Reference != pcInstance->Properties.VSecReference) ||
                (pcInstance->PLoop.Reference != pcInstance->Properties.PwrReference))
    #endif 
        {
            pcInstance->State = PWR_CNTRL_STATE_SOFT_START;
        }
    }
} 

