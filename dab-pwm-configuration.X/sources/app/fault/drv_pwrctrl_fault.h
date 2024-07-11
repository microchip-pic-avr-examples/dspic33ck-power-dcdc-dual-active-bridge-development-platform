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

#ifndef DRV_PWRCTL_FAULT_H
#define	DRV_PWRCTL_FAULT_H


#include <stddef.h>

#include "drv_pwrctrl_types.h"
#include "drv_pwrctrl_utils.h"
#include "config/user_params.h"
#include "cmp/cmp1.h"
#include "cmp/cmp3.h"

extern CLLC_t cllc;

//------------------------------------------------------------------------------
// functions with external linkage
//------------------------------------------------------------------------------
void Drv_PwrCtrl_FaultInit(void);
void Drv_PwrCtrl_Fault_EnableShortCircuitProtection(void);
void Drv_PwrCtrl_Fault_ClearHardwareFaults(void);

//------------------------------------------------------------------------------
// inline functions calls below, but keep in a separate header file
// for tidiness and for execution time efficiency
//------------------------------------------------------------------------------

/*********************************************************************************
 * @ingroup 
 * @fn      void __inline__ Drv_PwrCtrl_Fault_Action(void)
 * @brief   common action to take when a fault occurs
 * @param   none
 * @return  none
 * @details
 **********************************************************************************/
void __inline__ Drv_PwrCtrl_Fault_Action(void)
{
    // PWMs will have already stopped that this point
    // purpose here is to log the fault and set flags so that 
    // the state machine can be reset
    // stop converter, set flags
    Drv_PwrCtrl_PWM_Disable();
    cllc.status.bits.fault = 1;
    cllc.status.bits.running = 0;
    cllc.faultFlagsLatched.value = cllc.faultFlags.value;    
}

/*********************************************************************************
 * @ingroup 
 * @fn      void __inline__ Fault_Vsec_OV_Test(void)
 * @brief   check that Vsec is within limits
 * @param   none
 * @return  none
 * @details
 **********************************************************************************/
void __inline__ Drv_PwrCtrl_Fault_Vsec_OV(void)
{
  cllc.faultFlags.bits.vSec_ov = FAULT_CheckMax(&cllc.faultObjs.vsec_ov, cllc.adc.vsec, NULL);
  if (cllc.faultFlags.bits.vSec_ov)
  {
    Drv_PwrCtrl_Fault_Action();  
  }
}

/*********************************************************************************
 * @ingroup 
 * @fn      void __inline__ Fault_Vpri_OV_Test(void)
 * @brief   check that Vpri is within limits
 * @param   none
 * @return  none
 * @details
 **********************************************************************************/
void __inline__ Drv_PwrCtrl_Fault_Vpri_OV(void)
{
  cllc.faultFlags.bits.vPri_ov = FAULT_CheckMax(&cllc.faultObjs.vpri_ov, cllc.adc.vpri, NULL);
  if (cllc.faultFlags.bits.vPri_ov)
  {
      Drv_PwrCtrl_Fault_Action();  
  }
}

/*********************************************************************************
 * @ingroup 
 * @fn      void __inline__ Drv_PwrCtrl_Fault_Isec_OC(void)
 * @brief   Isec over current fault handler
 * @param   none
 * @return  none
 * @details
 **********************************************************************************/
void __inline__ Drv_PwrCtrl_Fault_Isec_OC(void)
{
  cllc.faultFlags.bits.iSec_oc = FAULT_CheckMax(&cllc.faultObjs.isec_oc, cllc.adc.isec_ct, NULL);
  if (cllc.faultFlags.bits.iSec_oc)
  {
      Drv_PwrCtrl_Fault_Action();  
  }
}

/*********************************************************************************
 * @ingroup 
 * @fn      void __inline__ Drv_PwrCtrl_Fault_Ipri_OC(void)
 * @brief   Ipri over current fault handler
 * @param   none
 * @return  none
 * @details
 **********************************************************************************/
void __inline__ Drv_PwrCtrl_Fault_Ipri_OC(void)
{
  cllc.faultFlags.bits.iPri_oc = FAULT_CheckMax(&cllc.faultObjs.ipri_oc, cllc.adc.ipri_ct, NULL);
  if (cllc.faultFlags.bits.iPri_oc)
  {
      Drv_PwrCtrl_Fault_Action();  
  }
}

/*********************************************************************************
 * @ingroup 
 * @fn      void __inline__ Drv_PwrCtrl_Fault_ShortCircuit(void)
 * @brief   primary or secondary short circuit fault
 * @param   none
 * @return  none
 * @details
 * This is implemented with comparators -> CLC -> PWM PCI fault inputs
 **********************************************************************************/
void __inline__ Drv_PwrCtrl_Fault_ShortCircuit(void)
{
    // Check the FLTEVT bit in PWM1. If this is set, the fault PCI mechanism was triggered
    // assume that this has been triggered for all PWMs (so we don't check the FLTEVT bit for each PWM)
    cllc.faultFlags.bits.i_sc = FAULT_CheckBit(&cllc.faultObjs.i_sc, PG1STATbits.FLTEVT, NULL);
    if (cllc.faultFlags.bits.i_sc)
    {
      Drv_PwrCtrl_Fault_Action();   
      
      // clear fault bits in PWM peripherals
      // to allow PWM to re-start when fault is no longer present
      Drv_PwrCtrl_Fault_ClearHardwareFaults();      
    }
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t __inline__ Drv_PwrCtrl_Fault_SC_Faults_Clear(void)
 * @brief   check if short circuit faults are clear before re-starting the converter
 * @param   none
 * @return  1 if both comparators are 0, 0 otherwise
 * @details
 **********************************************************************************/
uint16_t __inline__ Drv_PwrCtrl_Fault_SC_Faults_Clear(void)
{
#ifndef FAULT_SHORT_CCT_DISABLE
    uint16_t sc_faults_clear;
    uint16_t isec_sc = CMP_ISEC_SC_StatusGet();
    uint16_t ipri_sc = CMP_IPRI_SC_StatusGet();
 
    if (!isec_sc && !ipri_sc)
    {
        sc_faults_clear = 1;
    }
    else
    {
        sc_faults_clear = 0;
    }
    return (sc_faults_clear);
    
#else 
    return (1);
#endif // #ifndef FAULT_SHORT_CCT_DISABLE
}




#endif	/* DRV_PWRCTL_FAULT_H */

