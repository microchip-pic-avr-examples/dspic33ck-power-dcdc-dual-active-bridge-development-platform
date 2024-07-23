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
        case PCS_INIT:
            PCS_INIT_handler(pcInstance);
            break;    

        case PCS_WAIT_IF_FAULT_ACTIVE:
            PCS_WAIT_IF_FAULT_ACTIVE_handler(pcInstance);
            break;

        case PCS_STANDBY:
            PCS_STANDBY_handler(pcInstance);
            break;

        case PCS_SOFT_START:
            PCS_SOFT_START_handler(pcInstance);
            break;

        case PCS_UP_AND_RUNNING:
            PCS_UP_AND_RUNNING_handler(pcInstance);
            break;

        default:
            pcInstance->State = PCS_INIT;
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
    if (Dev_CurrentSensor_Get_CalibrationStatus())
    #endif
    {
        // current sensor calibration is complete. Update the offset of the current sensor
        pcInstance->Adc.isec_sensor_offset = Dev_CurrentSensor_Get_Offset();
        pcInstance->State = PCS_WAIT_IF_FAULT_ACTIVE;
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
    if ((pcInstance->Fault.Flags.value == 0) && (Drv_PwrCtrl_Fault_SC_Faults_Clear(pcInstance)))
    {
        pcInstance->Status.bits.fault = 0;
        pcInstance->State = PCS_STANDBY; // next state
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
    if (pcInstance->Fault.Flags.value)
    {
        pcInstance->enable = 0; // for now, user has to manually re-start converter after a fault
        pcInstance->State = PCS_WAIT_IF_FAULT_ACTIVE; // next state
    }
    else if (pcInstance->enable) // this flag is generally set externally (via PBV GUI for example)
    {
        pcInstance->Fault.Flags.value = 0;
        pcInstance->Fault.FaultFlagsLatched.value = 0;    
        
        // ToDo; Check with Cormac
//#ifdef OPEN_LOOP_PBV
//        pcInstance->Pwm.ControlPeriod = PGxPER_INIT;   
//#endif 
        pcInstance->Status.bits.running = 1;
        
        // current loop reference init
        pcInstance->iloop.reference = 0;
        
        // interrupts temporarily disabled to avoid a conflict with
        // with the compensator update function SMPS_Controller2P2ZUpdate() 
        // called from a higher priority interrupt
        INTERRUPT_GlobalDisable();            
        
        //ToDo: add this later
//        SMPS_Controller2P2ZInitialize(&icomp_2p2z); 
        INTERRUPT_GlobalEnable();   
        
        Drv_PwrCtrl_Fault_ClearHardwareFaults();
//        Dev_PwrCtrl_PWM_Primary_Enable(pcInstance); // enable primary side PWMs only 
        Dev_PwrCtrl_PWM_Enable(pcInstance);
        
        // reset the PWM settings in Standby mode
        pcInstance->Pwm.ControlPeriod = MIN_PWM_PERIOD;
        pcInstance->Pwm.ControlPhase = 0;
        pcInstance->Pwm.PBVPeriodTarget = MIN_PWM_PERIOD;
        pcInstance->Pwm.PBVControlPhaseTarget = 0;
    
        pcInstance->State = PCS_SOFT_START;   // next state
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
  if (pcInstance->Fault.Flags.value)
  {
      pcInstance->enable = false;  // for now, user has to manually re-start converter after a fault
      pcInstance->State = PCS_WAIT_IF_FAULT_ACTIVE; 
  }
  
  else if (!pcInstance->enable) 
  {
    Dev_PwrCtrl_PWM_Disable(pcInstance);
    pcInstance->Status.bits.running = 0;
    pcInstance->State = PCS_STANDBY; 
  }
  else
  {    
      
    uint16_t step = 8;  // cannot be below 8 because of the masking in 
    uint16_t delay = 0; // TODO: parameterize this into a soft-start time
    bool rampComplete = false;
    bool rampCompletePhase = false;
    
    // soft start the converter
#ifdef OPEN_LOOP_PBV
    uint16_t* ptr_reference = (uint16_t*)&pcInstance->Pwm.ControlPeriod;
    uint16_t* ptr_referenceTarget = (uint16_t*)&pcInstance->Pwm.PBVPeriodTarget;
    uint16_t* ptrControlPhaseReference = (uint16_t*)&pcInstance->Pwm.ControlPhase;
    uint16_t* ptrControlPhaseReferenceTarget = (uint16_t*)&pcInstance->Pwm.PBVControlPhaseTarget;
    
    rampCompletePhase =  Dev_PwrCtrl_RampReference(ptrControlPhaseReference, ptrControlPhaseReferenceTarget, 1, delay);
    
    if(rampCompletePhase == true)
#else
    uint16_t* ptr_reference = (uint16_t*)&pcInstance->iloop.reference;
    uint16_t* ptr_referenceTarget = (uint16_t*)&pcInstance->iloop.referenceTarget;
#endif // #ifdef OPEN_LOOP_PBV
    
    rampComplete = Dev_PwrCtrl_RampReference(ptr_reference, ptr_referenceTarget, step, delay);
    
    // ToDo: Check with Cormac, I think this is not needed as it is already called in the interrupt
//#ifdef OPEN_LOOP_PBV    
//    Dev_PwrCtrl_PWM_Update(&dab); // update PWM registers, which will in turn update frequency
//#endif  // #ifdef OPEN_LOOP_PBV 
    
    if (rampComplete)
    {
      pcInstance->State = PCS_UP_AND_RUNNING;  // next state
    }
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
    if (pcInstance->Fault.Flags.value)
    {
        pcInstance->enable = false;  // for now, user has to manually re-start converter after a fault
        pcInstance->State = PCS_WAIT_IF_FAULT_ACTIVE; 
    }
    else
    {
        if (!pcInstance->enable )
        {
            Dev_PwrCtrl_PWM_Disable(pcInstance);
            pcInstance->Status.bits.running = 0;            
            pcInstance->State = PCS_STANDBY;
        }
        
    #ifdef OPEN_LOOP_PBV
        else if ((pcInstance->Pwm.ControlPeriod != pcInstance->Pwm.PBVPeriodTarget) || 
                (pcInstance->Pwm.ControlPhase != pcInstance->Pwm.PBVControlPhaseTarget))
    #else
            
        else if (pcInstance->iloop.reference != pcInstance->iloop.referenceTarget)  
    #endif // #ifdef OPEN_LOOP_PBV
        {
            pcInstance->State = PCS_SOFT_START;
        }
    }
} 

