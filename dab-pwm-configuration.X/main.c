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
uint16_t ControlDutyCycle = 0;

uint16_t ControlPhase = 0;

void Timer1_Interrupt (void){
    
        ControlFrequency = (uint16_t)(MIN_PWM_PERIOD + (ADC1_ConversionResultGet(Pot2An0) * ADC_PERIOD_RANGE)); 
        
        // frequency clamping
        if(ControlFrequency > MAX_PWM_PERIOD)
            ControlFrequency = MAX_PWM_PERIOD;
        
        if(ControlFrequency < MIN_PWM_PERIOD)
            ControlFrequency = MIN_PWM_PERIOD;

        ControlFrequency = ControlFrequency & ~(0x7);
        
        ControlDutyCycle = (ControlFrequency >> 1);
        ControlPhase = (uint16_t)(ADC1_ConversionResultGet(Pot1An0) * ADC_SCALER * ControlDutyCycle);
        
        uint16_t PrimarySecondaryPhase = (ControlDutyCycle >> 1) - (ControlPhase >> 1);
        uint16_t PrimaryPhaseDelay = (ControlDutyCycle - PrimarySecondaryPhase) + ControlPhase;
        
        PG1TRIGC = PrimarySecondaryPhase;   
        PG2TRIGC = PrimaryPhaseDelay;       
        PG3TRIGC = PrimarySecondaryPhase;   
    
        for(uint16_t ctr = 1; ctr<5; ctr++){
        // change PWM frequency and duty cycle
        PWM_DutyCycleSet(ctr, ControlDutyCycle);
        PWM_PeriodSet(ctr, ControlFrequency);
        }
        PG4STATbits.UPDREQ = 1;
}

/**
 * TP45_H/47_L - PWM1
 * TP42_L/40_H - PWM2 (output swap)
 * TP37_L/41_H - PWM3 (output swap)
 * TP43_L/44_H - PWM4 (output swap)
 */
int main(void)
{
    SYSTEM_Initialize();
    
    TMR1_TimeoutCallbackRegister(&Timer1_Interrupt);
    
    //needed for cascaded PWM
    PG1CONHbits.TRGMOD = 1;     // retriggerable
    PG2CONHbits.TRGMOD = 1;     // retriggerable
    PG3CONHbits.TRGMOD = 1;     // retriggerable
    PG4CONHbits.TRGMOD = 1;     // retriggerable
    
//    PG2IOCONLbits.SWAP = 1;     // swap output for PWM2
    PG3IOCONLbits.SWAP = 1;     // swap output for PWM3
    PG4IOCONLbits.SWAP = 1;     // swap output for PWM4

    PWM_Enable();
    
    while(1)
    {
    }    
}