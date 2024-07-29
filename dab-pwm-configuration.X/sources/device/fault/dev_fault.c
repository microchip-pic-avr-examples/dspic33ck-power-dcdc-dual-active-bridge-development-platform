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
#include "dev_vin_isolated.h"
#include "dev_fault_temp.h"
#include "device/pwrctrl/dev_pwrctrl.h"
#include "dev_fault.h"
#include "system/pins.h"
#include "dev_fault_api.h"


void Dev_Fault_Handler(void)
{
    // Drive the fault pin to Low when Fault trips
    FAULT_SetLow();
    
    // Turn off PWM output
    Dev_PwrCtrl_PWM_Disable(&dab);
    
    dab.Status.bits.FaultActive = 1;
    dab.Status.bits.Running = 0;
}


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
void Dev_Fault_Initialize(void)
{
    FAULT_Init(&dab.Fault.Object.IPrimaryOCP, IPRI_OC_THRES_TRIG, IPRI_OC_THRES_CLEAR, IPRI_OC_T_BLANK_TRIG, IPRI_OC_T_BLANK_CLEAR); 
    FAULT_Init(&dab.Fault.Object.ISecondaryOCP, ISEC_OC_THRES_TRIG, ISEC_OC_THRES_CLEAR, ISEC_OC_T_BLANK_TRIG, ISEC_OC_T_BLANK_CLEAR);  
    FAULT_Init(&dab.Fault.Object.VPrimaryOVP, VPRI_OV_THRES_TRIG, VPRI_OV_THRES_CLEAR, VPRI_OV_T_BLANK_TRIG, VPRI_OV_T_BLANK_CLEAR);   
    FAULT_Init(&dab.Fault.Object.VSecondaryOVP, VSEC_OV_THRES_TRIG, VSEC_OV_THRES_CLEAR, VSEC_OV_T_BLANK_TRIG, VSEC_OV_T_BLANK_CLEAR);
    FAULT_Init(&dab.Fault.Object.ISenseSCP, 0,0,0,I_SC_T_BLANK_CLEAR);
    
    Dev_Temp_Initialize();
    
    //#ifndef FAULT_SHORT_CCT_DISABLE
//    // initialize short circuit fault protection with comparators
    Drv_PwrCtrl_Fault_EnableShortCircuitProtection();
//#endif // #ifndef FAULT_SHORT_CCT_DISABLE
//    // clear the fault PCI for each PWM
    Dev_Fault_ClearHardwareFaults(); 
    
}

void Dev_Fault_Execute(void)
{
    // secondary over current fault handler
    #if (FAULT_ISEC_OC)      
    FAULT_CheckMax(&dab.Fault.Object.ISecondaryOCP, dab.Adc.ISenseSecondary, &Dev_Fault_Handler);
    #endif 
    
    // secondary over voltage fault handler
    #if (FAULT_VSEC_OV)            
    FAULT_CheckMax(&dab.Fault.Object.VSecondaryOVP, dab.Adc.ISenseSecondary, &Dev_Fault_Handler);
    #endif    
    
    // primary over current fault handler
    #if(FAULT_IPRI_OC)
    FAULT_CheckMax(&dab.Fault.Object.IPrimaryOCP, dab.Adc.ISenseSecondary, &Dev_Fault_Handler);
    #endif 
    
    // primary over voltage fault handler
    #if (FAULT_VPRI_OV)                
    FAULT_CheckMax(&dab.Fault.Object.VPrimaryOVP, dab.Adc.ISenseSecondary, &Dev_Fault_Handler);
    #endif  

    // Identify the fault that trips
    dab.Fault.FaultDetected = Dev_Fault_GetFlags();
}



void Dev_Fault_Reset(void)
{
    // Drive the fault pin to high to allow PWM signal drive
    FAULT_SetHigh();
    
    // Clear fault Objects FaultActive bit
    dab.Fault.Object.ISenseSCP.FaultActive = 0;
    dab.Fault.Object.IPrimaryOCP.FaultActive = 0;
    dab.Fault.Object.ISecondaryOCP.FaultActive = 0;
    dab.Fault.Object.VPrimaryOVP.FaultActive = 0;
    dab.Fault.Object.VSecondaryOVP.FaultActive = 0;
    
    // Clear fault Objects FaultLatch bit
    dab.Fault.Object.ISenseSCP.FaultLatch = 0;
    dab.Fault.Object.IPrimaryOCP.FaultLatch = 0;
    dab.Fault.Object.ISecondaryOCP.FaultLatch = 0;
    dab.Fault.Object.VPrimaryOVP.FaultLatch = 0;
    dab.Fault.Object.VSecondaryOVP.FaultLatch = 0;
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
void Dev_Fault_ClearHardwareFaults(void)
{
  uint16_t pwmIndex;
  for (pwmIndex = 1; pwmIndex <= 4; pwmIndex++)
  {
    PWM_FaultModeLatchClear(pwmIndex);
    PWM_GeneratorEventStatusClear(pwmIndex, PWM_GENERATOR_INTERRUPT_FAULT);
  }
}

