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
#include "dev_fault_common.h"
#include "app/pwrctrl/drv_pwrctrl_typedef.h"
#include "config/macros.h"
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

extern POWER_CONTROL_t dab;

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
//void App_PwrCtrl_FaultInit(void)
//{
//    FAULT_Init(&dab.Fault.Object.ipri_oc, IPRI_OC_THRES_TRIG, IPRI_OC_THRES_CLEAR, IPRI_OC_T_BLANK_TRIG, IPRI_OC_T_BLANK_CLEAR); 
//    FAULT_Init(&dab.Fault.Object.isec_oc, ISEC_OC_THRES_TRIG, ISEC_OC_THRES_CLEAR, ISEC_OC_T_BLANK_TRIG, ISEC_OC_T_BLANK_CLEAR);  
//    FAULT_Init(&dab.Fault.Object.vpri_ov, VPRI_OV_THRES_TRIG, VPRI_OV_THRES_CLEAR, VPRI_OV_T_BLANK_TRIG, VPRI_OV_T_BLANK_CLEAR);   
//    FAULT_Init(&dab.Fault.Object.vsec_ov, VSEC_OV_THRES_TRIG, VSEC_OV_THRES_CLEAR, VSEC_OV_T_BLANK_TRIG, VSEC_OV_T_BLANK_CLEAR);
//    FAULT_Init(&dab.Fault.Object.i_sc, 0,0,0,I_SC_T_BLANK_CLEAR);
//    
//    // initialize fault protection
//    Drv_PwrCtrl_FaultInit();
//    
//    #ifndef FAULT_SHORT_CCT_DISABLE
//    // initialize short circuit fault protection with comparators
//    Drv_PwrCtrl_Fault_EnableShortCircuitProtection();
//    #endif // #ifndef FAULT_SHORT_CCT_DISABLE
//    
//    // clear the fault PCI for each PWM
//    Drv_PwrCtrl_Fault_ClearHardwareFaults(&dab);
//}
//
//void App_Fault_Execute(void)
//{
//    
//    // secondary over current fault handler
//    #ifndef FAULT_ISEC_OC_DISABLE      
//    Drv_PwrCtrl_Fault_Isec_OC();
//    #endif // #ifndef FAULT_ISEC_OC_DISABLE
//    
//    // secondary over voltage fault handler
//    #ifndef FAULT_VSEC_OV_DISABLE            
//    Drv_PwrCtrl_Fault_Vsec_OV();
//    #endif // #ifndef FAULT_VSEC_OC_DISABLE   
//    
//    // primary over current fault handler
//    #ifndef FAULT_IPRI_OC_DISABLE
//    Drv_PwrCtrl_Fault_Ipri_OC();
//    #endif // #ifndef FAULT_IPRI_OC_DISABLE
//    
//    // primary over voltage fault handler
//    #ifndef FAULT_VPRI_OV_DISABLE                
//    Drv_PwrCtrl_Fault_Vpri_OV();
//    #endif   // #ifndef FAULT_VPRI_OV_DISABLE
//    
//}
//
