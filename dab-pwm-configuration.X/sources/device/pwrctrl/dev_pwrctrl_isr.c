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

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dev_pwrctrl.h"
#include "timer/sccp1.h"
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "config/macros.h"
#include "config/config.h"
#include "dev_pwrctrl_pwm.h"
#include "device/fault/dev_fault.h"
#include "dcdt/dev_pwrctrl_dcdt.h"
#include "system/pins.h"
#include "dev_pwrctrl_utils.h"

// PRIVATE FUNCTIONS
void Dev_PwrCtrl_PrimToSecPHDegree(void);
void Dev_PwrCtrl_DeadTimeAdjust(void);
void Dev_PwrCtrl_PeriodModulator(void);
void Dev_PwrCtrl_UpdateADConverterData(void);
void Dev_PwrCtrl_ControlLoopExecute(void);


/*******************************************************************************
 * @ingroup dev-pwrctrl-isr-methods-public
 * @brief  Executes the power conveter's control loop
 * @return void
 * 
 * @details This interrupt function is a timing interrupt executed every 100KHz
 *   and calls an ADC function that updates the DAB data member with ADC raw values. 
 *   The fault check is also called in this function to detect any fault events.
 *   It is followed by the control loop execution for Voltage, Current and Power
 *   Loop Controllers. The end result of the control loop is handed over to the 
 *   PWM distribution. 
 *********************************************************************************/

void ControlLoop_Interrupt_CallBack(void)
{      
    GPIO_1_SetHigh();
    
    // Update the ADC data member
    Dev_PwrCtrl_UpdateADConverterData();
    
    // Execute the fault detection
    Dev_Fault_Execute();
    
    #if(OPEN_LOOP_PBV == false)
 
    // Execute Power Converter Control Loop
    Dev_PwrCtrl_ControlLoopExecute();
    
    // Measure Primary to Secondary phase in degrees
    Dev_PwrCtrl_PrimToSecPHDegree(); 
    
    // Adjust DeadTime based on Primary to Secondary phase in degrees
    Dev_PwrCtrl_DeadTimeAdjust();
    
    #if(PERIOD_MODULATION_DEMO == true)
    Dev_PwrCtrl_PeriodModulator();
    #endif   
    
    #endif

    #if(DAC_DEBUG == true)
    // This is for DAC debugging purposes
//    CMP2_DACDataWrite(dab.VLoop.Reference);
//    CMP2_DACDataWrite(dab.ILoop.Reference << 2);
    CMP2_DACDataWrite(dab.PLoop.Reference >> 2);
    #endif

    #if (true == DPDB_TEST_RUN)

    // Connect TP11 -> Pot1 & TP12 -> Pot2 in digital Power Development Board
    uint16_t Pot1 = dab.Data.ISecAverage; 
    uint16_t Pot2 = dab.Data.ISenseSecondary; 
    
    // Calculate the Frequency based on the Potentiometer 1 voltage
    dab.Pwm.ControlPeriod = (uint16_t)(MIN_PWM_PERIOD + (Pot1 * ADC_PERIOD_RANGE)); 
    
    // Change the control Phase with Potentiometer 2
    dab.Pwm.ControlPhase = (uint16_t)(Pot2 * ADC_SCALER * (dab.Pwm.ControlPeriod >> 1));
    
    #endif

    DPD_TP31_SetHigh();
    // Update PWM Properties
    Dev_PwrCtrl_PWM_Update(&dab);
    
    DPD_TP31_SetLow();
    
    GPIO_1_SetLow();
    
}
