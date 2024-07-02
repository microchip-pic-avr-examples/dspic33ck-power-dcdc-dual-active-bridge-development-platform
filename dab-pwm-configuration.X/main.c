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
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/pwm_hs/pwm.h"

#include "hal.h"

/*
    Main application
*/




/**
 * TP45_H/47_L - PWM1
 * TP42_H/40_L - PWM2
 * TP37_L/41_H - PWM3 (output swap)
 * TP43_L/44_H - PWM4 (output swap)
 */
int main(void)
{
    SYSTEM_Initialize();
    
    PG2SPCILbits.PSS = 0x17;    //PWM Event A
    PG2SPCILbits.TERM = 1;      // auto terminate
    PG2SPCILbits.TSYNCDIS = 1;  //termination of latched PCI occurs immediately
    PWMEVTA = 0x2080;
    PG2CONHbits.TRGMOD = 1;     // re-triggerable
    PG3IOCONLbits.SWAP = 1;     // swap output for PWM3
    PG4IOCONLbits.SWAP = 1;     // swap output for PWM3

    PG1TRIGC = 2000;    // value for Trigger PG2
    PG1TRIGA = 1000;    // value for Trigger PG3
    PG2TRIGB = 2000;    // value for Trigger PG4
    
    // test PWM period
    for(uint16_t ctr = 1; ctr<5; ctr++){
    PWM_PeriodSet(ctr, MIN_PWM_PERIOD);
    PWM_SoftwareUpdateRequest(ctr);
    }
    PWM_Enable();
    
    while(1)
    {
        
        
    }    
}