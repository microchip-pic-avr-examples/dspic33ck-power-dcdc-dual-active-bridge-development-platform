/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "system/system.h"
#include "pwm_hs/pwm.h"
#include "adc/adc1.h"
#include "timer/tmr1.h"
#include "driver_chiplayer/mcc_extenstion/drv_mcc_extension.h"

#include "config/hal.h"

/*
    Main application
*/


void Timer1_Interrupt (void){
    
    // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
    // (please refer to the device FRM)
    uint16_t PeriodMask = 0x7; 
    uint16_t ControlFrequency = 0;
    uint16_t ControlDutyCycle = 0;
    uint16_t ControlPhase = 0;

    //Calculate the Frequency based on the Potentiometer 2 voltage
    ControlFrequency = (uint16_t)(MIN_PWM_PERIOD + (ADC1_ConversionResultGet(Pot2An0) * ADC_PERIOD_RANGE)); 
    
    // Maximum frequency clamping
    if(ControlFrequency > MAX_PWM_PERIOD)
        ControlFrequency = MAX_PWM_PERIOD;
    
    // Minimum frequency clamping
    if(ControlFrequency < MIN_PWM_PERIOD)
        ControlFrequency = MIN_PWM_PERIOD;

    // Mask the calculated frequency bits [2:0] to make the cascaded/synchronous
    // PWM scheme reliable (please refer to the PWM FRM)
    ControlFrequency = ControlFrequency & ~(PeriodMask);
    
    // calculate Duty Cycle for 50%
    ControlDutyCycle = (ControlFrequency >> 1);
    
    // Scale the potentiometer 1 reading with the Frequency
    ControlPhase = (uint16_t)(ADC1_ConversionResultGet(Pot1An0) * ADC_SCALER * ControlDutyCycle);
    
    // Calculate the DAB Primary to Secondary Phase ((Frequency / 4) - (Control Phase /2))
    uint16_t PrimarySecondaryPhase = (ControlDutyCycle >> 1) - (ControlPhase >> 1);
    
    // Calculate the bridge delay ((Frequency / 2) - Primary to Secondary Phase + Control Phase)
    // Note that in the cascaded PWM, the reference phase of the client PWM, is its trigger source
    uint16_t PrimaryPhaseDelay = (ControlDutyCycle - PrimarySecondaryPhase) + ControlPhase;
    
    // Set the PWM trigger with the calculated PWM phases
    PWM_TriggerCCompareValueSet(PWM_GENERATOR_1, PrimarySecondaryPhase);
    PWM_TriggerCCompareValueSet(PWM_GENERATOR_2, PrimaryPhaseDelay);
    PWM_TriggerCCompareValueSet(PWM_GENERATOR_3, PrimarySecondaryPhase);

    // Set the PWM Duty Cycle at 50% with the given Frequency
    PWM_DutyCycleSet(PWM_GENERATOR_1, ControlDutyCycle);
    PWM_DutyCycleSet(PWM_GENERATOR_2, ControlDutyCycle);
    PWM_DutyCycleSet(PWM_GENERATOR_3, ControlDutyCycle);
    PWM_DutyCycleSet(PWM_GENERATOR_4, ControlDutyCycle);
    
    // Set the PWM Frequency
    PWM_PeriodSet(PWM_GENERATOR_1, ControlFrequency);
    PWM_PeriodSet(PWM_GENERATOR_2, ControlFrequency);
    PWM_PeriodSet(PWM_GENERATOR_3, ControlFrequency);
    PWM_PeriodSet(PWM_GENERATOR_4, ControlFrequency);

    // Set the Update bit of the last PWM in the cascaded approach to broadcast
    // it to the other PWMs
    PWM_SoftwareUpdateRequest(PWM_GENERATOR_4);
    
}

/*******************************************************************************
 * @ingroup 
 * @brief  Application main function executed after device comes out of RESET
 * @details 
 * This function is the starting point of the firmware. It is called after the
 * device is coming out of RESET, starting to execute code. 
 * 
 *********************************************************************************/

/**
 * TP45_H/47_L - PWM1
 * TP42_H/40_L - PWM2 
 * TP37_L/41_H - PWM3 (output swap)
 * TP43_L/44_H - PWM4 (output swap)
 */
int main(void)
{
    SYSTEM_Initialize();
    
    TMR1_TimeoutCallbackRegister(&Timer1_Interrupt);
    
    //needed for cascaded PWM
    PWM_Trigger_Mode(PWM_GENERATOR_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_GENERATOR_2, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_GENERATOR_3, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_GENERATOR_4, PWM_TRIG_MODE_RETRIGGERABLE);
    
    PWM_Swap_PWMxL_and_PWMxH(PWM_GENERATOR_3, true);
    PWM_Swap_PWMxL_and_PWMxH(PWM_GENERATOR_4, true);

    PWM_Enable();
    
    while(1)
    {
    }    
}