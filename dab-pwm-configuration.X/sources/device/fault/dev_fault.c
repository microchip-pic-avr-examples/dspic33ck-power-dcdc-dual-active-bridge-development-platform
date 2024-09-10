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
#include "device/dev_temp.h"
#include "device/pwrctrl/dev_pwrctrl.h"
#include "dev_fault.h"
#include "system/pins.h"
#include "dev_fault_api.h"

// PRIVATE FUNCTIONS
static void Dev_Fault_EnableShortCircuitProtection(void);

/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief  Handles the fault trip by turning off the power control switching
 * @return void
 * 
 * @details This function handles the occurence of fault when one of the fault
 *  condition trips. It shuts down the operation of the power control, set the 
 *  FaultActive bit and clear the Running bit indicating that power converter
 *  has been turned-off. A fault pin is also set to low to blocked the PWM signal
 *  as a hardware protection.  
 *********************************************************************************/
void Dev_Fault_Handler(void)
{
    // Drive the fault pin to Low when Fault trips
    FAULT_SetLow();
    
    // Turn off PWM output
    Dev_PwrCtrl_PWM_Disable();
     
    // set the fault active bit
    dab.Status.bits.FaultActive = 1;
    
    // clear the running bit
    dab.Status.bits.Running = 0;    
    
}


/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief   Initialize the fault objects
 * @return void
 * 
 * @details This function initializes the fault objects following each respective
 *  fault thresholds. 
 *********************************************************************************/
void Dev_Fault_Initialize(void)
{
    // Initialize Primary Over Current Protection
    FAULT_Init(&dab.Fault.Object.IPrimaryOCP, IPRI_OC_THRES_TRIG, 
            IPRI_OC_THRES_CLEAR, IPRI_OC_T_BLANK_TRIG, IPRI_OC_T_BLANK_CLEAR); 
    
    // Initialize Secondary Over Current Protection
    FAULT_Init(&dab.Fault.Object.ISecondaryOCP, ISEC_OC_THRES_TRIG, 
            ISEC_OC_THRES_CLEAR, ISEC_OC_T_BLANK_TRIG, ISEC_OC_T_BLANK_CLEAR);  
    
    // Initialize Primary Over Voltage Protection
    FAULT_Init(&dab.Fault.Object.VPrimaryOVP, VPRI_OV_THRES_TRIG, 
            VPRI_OV_THRES_CLEAR, VPRI_OV_T_BLANK_TRIG, VPRI_OV_T_BLANK_CLEAR);   
    
    // Initialize Secondary Over Voltage Protection
    FAULT_Init(&dab.Fault.Object.VSecondaryOVP, VSEC_OV_THRES_TRIG, 
            VSEC_OV_THRES_CLEAR, VSEC_OV_T_BLANK_TRIG, VSEC_OV_T_BLANK_CLEAR);
    
    // Initialize 5V Rail instability Protection
    FAULT_Init(&dab.Fault.Object.VRail_5V, VRAIL_5V_UV_THRES_TRIG, 
            VRAIL_5V_UV_THRES_CLEAR, VRAIL_5V_UV_T_BLANK_TRIG, VRAIL_5V_UV_T_BLANK_CLEAR);
    
    // Initialize Over Temperature Protection
    FAULT_Init(&dab.Fault.Object.PowerSupplyOTP, MAX_TEMPERATURE_THRESHOLD_RAW,         
            OVER_TEMP_UPPER_THRESHOLD_WITH_HYST,FAULT_PERSISTENCE_COUNT_TEMP, FAULT_PERSISTENCE_COUNT_TEMP); 
    
    //ToDo: need to check this
#if (FAULT_SHORT_CCT == true)
    // Initialize short circuit fault protection with comparators
    Dev_Fault_EnableShortCircuitProtection();
#endif 
    // Clear the fault PCI for each PWM
    Dev_Fault_ClearHardwareFaults(); 
    
}

/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief   Executes the fault handlers
 * @return void
 * 
 * @details This function evaluates if any of the fault objects has been tripped. 
 *  When fault detection occurs, the power converter will shutdown thus turn-off
 *  the power converter. 
 *********************************************************************************/
void Dev_Fault_Execute(void)
{
    // secondary over current fault handler
    #if (FAULT_ISEC_OC)      
    FAULT_CheckMax(&dab.Fault.Object.ISecondaryOCP, dab.Data.ISenseSecondary, &Dev_Fault_Handler);
    #endif 
    
    // secondary over voltage fault handler
    #if (FAULT_VSEC_OV)            
    FAULT_CheckMax(&dab.Fault.Object.VSecondaryOVP, dab.Data.VSecVoltage, &Dev_Fault_Handler);
    #endif    
    
    // primary over current fault handler
    #if(FAULT_IPRI_OC)
    FAULT_CheckMax(&dab.Fault.Object.IPrimaryOCP, dab.Data.ISensePrimary, &Dev_Fault_Handler);
    #endif 
    
    // primary over voltage fault handler
    #if (FAULT_VPRI_OV)      
    FAULT_CheckMax(&dab.Fault.Object.VPrimaryOVP, dab.Data.VPriVoltage, &Dev_Fault_Handler);
    #endif  

    // primary over voltage fault handler
    #if (FAULT_VRAIL_5V)                
    FAULT_CheckMax(&dab.Fault.Object.VRail_5V, dab.Data.VRail_5V, &Dev_Fault_Handler);
    #endif  
    
    // Hardware short circuit
    if(CMP1_StatusGet() || CMP3_StatusGet()){
        dab.Fault.Object.ISenseSCP.FaultActive = 1;
        dab.Fault.Object.ISenseSCP.FaultLatch = 1;
    }
    
    // Identify the fault that trips
    dab.Fault.FaultDetected = Dev_Fault_GetFlags();
}


/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief   Clears the fault object flag bits
 * @return void
 * 
 * @details This function clears the fault status bits for Fault Active and 
 *  Fault Latched. A fault pin is also drive to high to allow PWM signal drive. 
 *********************************************************************************/
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
    dab.Fault.Object.ISenseSCP.FaultActive = 0;
    
    // Clear fault Objects FaultLatch bit
    dab.Fault.Object.ISenseSCP.FaultLatch = 0;
    dab.Fault.Object.IPrimaryOCP.FaultLatch = 0;
    dab.Fault.Object.ISecondaryOCP.FaultLatch = 0;
    dab.Fault.Object.VPrimaryOVP.FaultLatch = 0;
    dab.Fault.Object.VSecondaryOVP.FaultLatch = 0;
    dab.Fault.Object.ISenseSCP.FaultLatch = 0;
    
    //ToDo: remove this after testing LEB
    // initialize thresholds of comparators used for short circuit protection
  CMP_ISEC_SC_DACDataWrite(ISEC_SC_THRES_TRIG);   
  CMP_IPRI_SC_DACDataWrite(IPRI_SC_THRES_TRIG);  
    
    // Clear the fault PCI for each PWM
    Dev_Fault_ClearHardwareFaults(); 
}

/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief   Enable Short circuit hardware protection
 * @return void
 * 
 * @details This function setup the short circuit protection threshold and
 *  turns on the DAC (Digital-to-Analog) module. This hardware protection use
 *  Comparator DACs to detect short circuit.   
 *********************************************************************************/
static void Dev_Fault_EnableShortCircuitProtection(void)
{
  // on dsPIC33CK DP-PIM:
  // CMP1B used for short circuit protection on the secondary side 
  // CMP3B used for short circuit protection on the primary side

  // initialize thresholds of comparators used for short circuit protection
  CMP_ISEC_SC_DACDataWrite(ISEC_SC_THRES_TRIG);   
  CMP_IPRI_SC_DACDataWrite(IPRI_SC_THRES_TRIG);   
  
  // turn on comparator DACs 
  CMP_IPRI_SC_DACEnable();
  CMP_ISEC_SC_DACEnable();
  
}

/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief   Clear hardware faults 
 * @return void
 * 
 * @details This function clears the hardware faults handled by PCI(PWM Control Input)
 *  block. 
 *********************************************************************************/
void Dev_Fault_ClearHardwareFaults(void)
{
  uint16_t pwmIndex;
  for (pwmIndex = 1; pwmIndex <= 4; pwmIndex++)
  {
    PWM_FaultModeLatchClear(pwmIndex);
    PWM_GeneratorEventStatusClear(pwmIndex, PWM_GENERATOR_INTERRUPT_FAULT);
  }
}

/*******************************************************************************
 * @ingroup dev-fault-methods-public
 * @brief   Fault evaluation for Temperature
 * @return void
 * 
 * @details This function checks if the board temperature is within the nominal
 *  temperature range. When temperature exceeds the limit, the power control
 *  can trip or perform temperature derating. 
 *********************************************************************************/
void Dev_Fault_Temp_100ms(void) 
{
    Dev_Temp_Get_ADC_Sample();
    
    #if(FAULT_PS_OTP)
    if(FAULT_CheckMin(&dab.Fault.Object.PowerSupplyOTP, devTempData.AdcReading, &Dev_Fault_Handler))
    {
       devTempData.OverTemperatureFlag = 1; //for over temperature
    }
    else
    {
        devTempData.OverTemperatureFlag = 0; //for over temperature
    }
    #endif
}