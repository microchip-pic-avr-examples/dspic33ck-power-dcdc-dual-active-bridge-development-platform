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

#include "xc.h"
#include <stdbool.h>
#include <stddef.h>

#include "cmp/cmp1.h"
#include "cmp/cmp3.h"
#include "pwm_hs/pwm.h"

#include "config/macros.h"
#include "dev_fault_common.h"
#include "dev_pwrctrl_typedef.h"

extern POWER_CONTROL_t dab;

void Drv_PwrCtrl_Fault_EnableShortCircuitProtection(void);
void Drv_PwrCtrl_Fault_ClearHardwareFaults(void);

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_FaultInit(void)
 * @brief   initialize application specific fault objects
 * @param   None
 * @return  None 
 * @details
 * initialize fault objects
 * This is an API function
 **********************************************************************************/
void Dev_PwrCtrlFault_Initialize(void)
{
    FAULT_Init(&dab.Fault.Object.ipri_oc, IPRI_OC_THRES_TRIG, IPRI_OC_THRES_CLEAR, IPRI_OC_T_BLANK_TRIG, IPRI_OC_T_BLANK_CLEAR); 
    FAULT_Init(&dab.Fault.Object.isec_oc, ISEC_OC_THRES_TRIG, ISEC_OC_THRES_CLEAR, ISEC_OC_T_BLANK_TRIG, ISEC_OC_T_BLANK_CLEAR);  
    FAULT_Init(&dab.Fault.Object.vpri_ov, VPRI_OV_THRES_TRIG, VPRI_OV_THRES_CLEAR, VPRI_OV_T_BLANK_TRIG, VPRI_OV_T_BLANK_CLEAR);   
    FAULT_Init(&dab.Fault.Object.vsec_ov, VSEC_OV_THRES_TRIG, VSEC_OV_THRES_CLEAR, VSEC_OV_T_BLANK_TRIG, VSEC_OV_T_BLANK_CLEAR);
    FAULT_Init(&dab.Fault.Object.i_sc, 0,0,0,I_SC_T_BLANK_CLEAR);
    
    //#ifndef FAULT_SHORT_CCT_DISABLE
//    // initialize short circuit fault protection with comparators
    Drv_PwrCtrl_Fault_EnableShortCircuitProtection();
//#endif // #ifndef FAULT_SHORT_CCT_DISABLE
//    // clear the fault PCI for each PWM
    Drv_PwrCtrl_Fault_ClearHardwareFaults();    
    
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_Fault_EnableShortCircuitProtection(void)
 * @brief   set short circuit protection thresholds, enable comparators
 * @param   none
 * @return  none 
 * @details
 * This is an API function
 **********************************************************************************/
void Drv_PwrCtrl_Fault_EnableShortCircuitProtection(void)
{
  // on dsPIC33CK DP-PIM:
  // CMP1B used for short circuit protection on the secondary side 
  // CMP3B used for short circuit protection on the primary side

  // initialize thresholds of comparators used for short circuit protection
  CMP_ISEC_SC_DACDataWrite(ISEC_SC_THRES_TRIG);   
  CMP_IPRI_SC_DACDataWrite(IPRI_SC_THRES_TRIG);   
  
  // turn on comparator DACs 
  CMP_ISEC_SC_DACEnable();
  CMP_ISEC_SC_DACEnable();
}


/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_Fault_ClearHardwareFaults(void)
 * @brief   clear hardware faults
 * @param   none
 * @return  none 
 * @details
 * This is an API function
 **********************************************************************************/
void Drv_PwrCtrl_Fault_ClearHardwareFaults(void)
{
  uint16_t pwmIndex;
  for (pwmIndex = 1; pwmIndex <= 4; pwmIndex++)
  {
    PWM_FaultModeLatchClear(pwmIndex);
    PWM_GeneratorEventStatusClear(pwmIndex, PWM_GENERATOR_INTERRUPT_FAULT);
  }
}
