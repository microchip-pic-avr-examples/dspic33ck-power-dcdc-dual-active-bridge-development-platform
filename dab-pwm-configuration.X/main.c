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

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/pwm_hs/pwm.h"
#include "mcc_generated_files/adc/adc1.h"
#include "mcc_generated_files/timer/tmr1.h"

#include "sources/config/hal.h"

/*
    Main application
*/

uint16_t ControlFrequency = 0;
uint16_t ControlDutyCycle =0;
float ControlPhaseGain = 0;
uint16_t ControlPhase = 0;

void Timer1_Interrupt (void){
    
        ControlFrequency = (uint16_t)(MIN_PWM_PERIOD + (ADC1_ConversionResultGet(Pot2An0) * ADC_PERIOD_RANGE)); 
       
//        ControlPhaseGain = (float)(ADC1_ConversionResultGet(Pot1An0) / ADC_RESOLUTION);
//        ControlPhase = (uint16_t)(ControlDutyCycle * ControlPhaseGain);
        
        // frequency clamping
        if(ControlFrequency > MAX_PWM_PERIOD)
            ControlFrequency = MAX_PWM_PERIOD;
        
        if(ControlFrequency < MIN_PWM_PERIOD)
            ControlFrequency = MIN_PWM_PERIOD;

        ControlFrequency = ControlFrequency & ~(0x7);
        
        ControlDutyCycle = (ControlFrequency >> 1);

        uint16_t DummyPhaseAdded = ControlDutyCycle >> 3;
        PG1TRIGC = (ControlDutyCycle >> 1) + DummyPhaseAdded;    // value for Trigger PG3 - S
        PG2TRIGC = (ControlDutyCycle >> 1) + DummyPhaseAdded;    // value for Trigger PG4 - S
        PG3TRIGC = ControlDutyCycle + DummyPhaseAdded;    // value for Trigger PG2 - Primary phase

    
        for(uint16_t ctr = 1; ctr<5; ctr++){
        // change PWM frequency and duty cycle
        PWM_DutyCycleSet(ctr, ControlDutyCycle);
        PWM_PeriodSet(ctr, ControlFrequency);
        }
        PG4STATbits.UPDREQ = 1;
}

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
    
    //previous implemetation
//    PG2SPCILbits.PSS = 0x18;    // PWM Event B
//    PG2SPCILbits.TERM = 1;      // auto terminate
//    PG2SPCILbits.TSYNCDIS = 1;  // termination of latched PCI occurs immediately
//    PWMEVTBbits.EVTBSTRD = 1;   // Event output signal pulse width is not stretched
//    PWMEVTBbits.EVTBSEL = 0b1001; // ADC Trigger 2 signal
//    PWMEVTBbits.EVTBPGS = 0;    // PWM Event Source: PWM Generator 1
//    PG2CONHbits.TRGMOD = 1;     // retriggerable
//    PG1TRIGB = 2000;  // value for Trigger PG2 - Primary phase
    
    //needed for cascaded PWM
    PG1CONHbits.TRGMOD = 1;     // retriggerable
    PG2CONHbits.TRGMOD = 1;     // retriggerable
    PG3CONHbits.TRGMOD = 1;     // retriggerable
    PG4CONHbits.TRGMOD = 1;     // retriggerable
    
    PG3IOCONLbits.SWAP = 1;     // swap output for PWM3
    PG4IOCONLbits.SWAP = 1;     // swap output for PWM4

    
    PWM_Enable();
    
    while(1)
    {
    }    
}