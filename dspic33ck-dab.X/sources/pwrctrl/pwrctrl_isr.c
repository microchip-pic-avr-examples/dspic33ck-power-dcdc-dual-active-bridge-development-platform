
/**
 * @file      pwrctrl_isr.c
 * @ingroup   pwrctrl-isr   
 * @brief     Contains Control loop interrupt Callback that acquires the ADC raw 
 *  data and process it in the control loop, and use the control output for the 
 *  PWM distribution for this converter
 */

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// MCC header files
#include "timer/sccp1.h"
#include "adc/adc1.h"
#include "system/pins.h"

// DAB header files
#include "config/macros.h"
#include "config/config.h"
#include "pwrctrl.h"
#include "fault/fault.h"

// PRIVATE FUNCTIONS
void PwrCtrl_PrimToSecPHDegree(void);
void PwrCtrl_DeadTimeAdjust(void);
void PwrCtrl_PeriodModulator(void);
void PwrCtrl_UpdateADConverterData(void);
void PwrCtrl_ControlLoopExecute(void);
void PwrCtrl_10KHzVPLoopPrepareData(void);

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  Executes the power converter control loop
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
    PwrCtrl_UpdateADConverterData();
    
    // Execute the fault detection
    Fault_Execute();
    
    // Performs data averaging necessary for control loop
    // also Enabling interleaved the Vloop (10KHz execution) 
    // and Ploop (10KHz execution) enable bit  
    PwrCtrl_10KHzVPLoopPrepareData();
    
    #if defined (OPEN_LOOP_PBV) && (OPEN_LOOP_PBV == false)
 
//    if(dab.Status.bits.Running == 1){ 
    // Execute Power Converter Control Loop
        PwrCtrl_ControlLoopExecute(); 
//    }
    
    // Measure Primary to Secondary phase in degrees
    PwrCtrl_PrimToSecPHDegree(); 
    
    // Adjust DeadTime based on Primary to Secondary phase in degrees
    PwrCtrl_DeadTimeAdjust();
    
    #if defined (PERIOD_MODULATION_DEMO) && (PERIOD_MODULATION_DEMO == true)
    PwrCtrl_PeriodModulator();
    #endif   
    
    #endif

    #if defined (DAC_DEBUG) && (DAC_DEBUG == true)
    // This is for DAC debugging purposes
//    CMP2_DACDataWrite(variable_to_test_here);
    #endif

    #if defined (OPEN_LOOP_POTI) && (OPEN_LOOP_POTI == true)

    // Connect TP11 -> Pot1 & TP12 -> Pot2 in digital Power Development Board
    uint16_t Pot1 = dab.Data.ISecAverage; 
    uint16_t Pot2 = dab.Data.ISenseSecondary; 
    
    // Calculate the Frequency based on the Potentiometer 1 voltage
    dab.Pwm.ControlPeriod = (uint16_t)(MIN_PWM_PERIOD + (Pot1 * ADC_PERIOD_RANGE)); 
    
    // Change the control Phase with Potentiometer 2
    dab.Pwm.ControlPhase = (uint16_t)(Pot2 * ADC_SCALER * (dab.Pwm.ControlPeriod >> 1));
    
    #endif

    if(dab.Status.bits.Running == 0)
    { 
    // Suppress Power Converter Control Loop values
        dab.Pwm.ControlPhase = 0;
    }
    // Update PWM Properties
    PwrCtrl_PWM_Update(&dab);
    
    // Enable the ADC sampling
    ADC1_SoftwareTriggerEnable();

    GPIO_1_SetLow();
}
