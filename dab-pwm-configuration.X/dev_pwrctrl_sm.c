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
                               
#include "driver/power_control/drv_pwrctrl_typedef.h"

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_StateMachine(void)
 * @brief   power controller state machine
 * @param   none
 * @return  none 
 * @details
 *
 **********************************************************************************/
void Drv_PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance)
{
    // short circuit fault checks (primary and secondary over current via comparators)
    //ToDo: Add This
//    Drv_PwrCtrl_Fault_ShortCircuit();
  
    switch (pcInstance->State)
    {
        case PCS_INIT:
            PCS_INIT_handler();
            break;    

        case PCS_WAIT_IF_FAULT_ACTIVE:
            PCS_WAIT_IF_FAULT_ACTIVE_handler();
            break;

        case PCS_STANDBY:
            PCS_STANDBY_handler();
            break;

        case PCS_SOFT_START:
            PCS_SOFT_START_handler();
            break;

        case PCS_UP_AND_RUNNING:
            PCS_UP_AND_RUNNING_handler();
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
static __inline__ void PCS_INIT_handler(void)
{
#ifndef OPEN_LOOP_POTI
    // if OPEN_LOOP_POTI is defined, we are running on the digital power development board
    // so in this case ignore the current sensor calibration
    //ToDo: Add This
    if (Dev_CurrentSensor_Get_CalibrationStatus())
#endif // #ifndef OPEN_LOOP_POTI
    {
        // current sensor calibration is complete. Update the offset in the cllc structure.
        //ToDo: Add This
        cllc.isec_sensor_offset = Dev_CurrentSensor_Get_Offset();
        cllc.state = PCS_WAIT_IF_FAULT_ACTIVE;
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
static __inline__ void PCS_WAIT_IF_FAULT_ACTIVE_handler(void)
{
    if ((cllc.faultFlags.value == 0) && (Drv_PwrCtrl_Fault_SC_Faults_Clear()))
    {
        cllc.status.bits.fault = 0;  
        cllc.state = PCS_STANDBY; // next state
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
static __inline__ void PCS_STANDBY_handler(void)
{
    if (cllc.faultFlags.value)
    {
        cllc.enable = 0;  // for now, user has to manually re-start converter after a fault
        cllc.state = PCS_WAIT_IF_FAULT_ACTIVE; // next state
    }
    else if (cllc.enable) // this flag is generally set externally (via PBV GUI for example)
    {
        cllc.faultFlags.value = 0;
        cllc.faultFlagsLatched.value = 0;    
#ifdef OPEN_LOOP_PBV
        cllc.pwm.pgxper = PGxPER_INIT;   
#endif // #ifdef OPEN_LOOP_PBV
        cllc.status.bits.running = 1;
        
        // current loop reference init
        cllc.iloop.reference = 0;
        
        // interrupts temporarily disabled to avoid a conflict with
        // with the compensator update function SMPS_Controller2P2ZUpdate() 
        // called from a higher priority interrupt
        INTERRUPT_GlobalDisable();            
        
        SMPS_Controller2P2ZInitialize(&icomp_2p2z); 
        INTERRUPT_GlobalEnable();   
        
        Drv_PwrCtrl_Fault_ClearHardwareFaults();
        Drv_PwrCtrl_PWM_Primary_Enable(); // enable primary side PWMs only 
        cllc.state = PCS_SOFT_START;   // next state
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
static __inline__ void PCS_SOFT_START_handler(void)
{
  if (cllc.faultFlags.value)
  {
    cllc.enable = false;  // for now, user has to manually re-start converter after a fault
    cllc.state = PCS_WAIT_IF_FAULT_ACTIVE; 
  }
  else if (!cllc.enable) 
  {
    Drv_PwrCtrl_PWM_Disable();
    cllc.status.bits.running = 0;
    cllc.state = PCS_STANDBY;
  }
  else
  {    
    // soft start the converter
#ifdef OPEN_LOOP_PBV
    uint16_t* ptr_reference = (uint16_t*)&cllc.pwm.pgxper;
    uint16_t* ptr_referenceTarget = (uint16_t*)&cllc.pwm.pgxperTarget;
#else
    uint16_t* ptr_reference = (uint16_t*)&cllc.iloop.reference;
    uint16_t* ptr_referenceTarget = (uint16_t*)&cllc.iloop.referenceTarget;
#endif // #ifdef OPEN_LOOP_PBV
    uint16_t step = 1;
    uint16_t delay = 12; // TODO: parameterize this into a soft-start time
    bool rampComplete = false;
    rampComplete = Drv_PwrCtrl_RampReference(ptr_reference, ptr_referenceTarget, step, delay);
#ifdef OPEN_LOOP_PBV    
    Drv_PwrCtrl_PWM_Update(); // update PWM registers, which will in turn update frequency
#endif  // #ifdef OPEN_LOOP_PBV 
    if (rampComplete)
    {
      cllc.state = PCS_UP_AND_RUNNING;  // next state
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
static __inline__ void PCS_UP_AND_RUNNING_handler(void)
{
    if (cllc.faultFlags.value)
    {
        cllc.enable = false;  // for now, user has to manually re-start converter after a fault
        cllc.state = PCS_WAIT_IF_FAULT_ACTIVE; 
    }
    else
    {
        if (!cllc.enable)
        {
            Drv_PwrCtrl_PWM_Disable();
            cllc.status.bits.running = 0;            
            cllc.state = PCS_STANDBY;
        }
#ifdef OPEN_LOOP_PBV
        else if (cllc.pwm.pgxper != cllc.pwm.pgxperTarget)
#else
        else if (cllc.iloop.reference != cllc.iloop.referenceTarget)  
#endif // #ifdef OPEN_LOOP_PBV
        {
            cllc.state = PCS_SOFT_START;
        }
    }
} 



