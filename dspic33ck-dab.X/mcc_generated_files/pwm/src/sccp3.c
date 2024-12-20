/**
 * SCCP3 Generated Driver Source File
 * 
 * @file 	  sccp3.c
 * 
 * @ingroup   pwmdriver
 * 
 * @brief 	  This is the generated driver source file for SCCP3 driver
 *
 * @skipline @version   Firmware Driver Version 2.1.1
 *
 * @skipline @version   PLIB Version 1.6.4
 *
 * @skipline  Device : dsPIC33CK256MP506
*/

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

// Section: Included Files

#include <xc.h>
#include <stddef.h>
#include "../sccp3.h"

// Section: File specific functions

static void (*SCCP3_PWMHandler)(void) = NULL;

// Section: Driver Interface

const struct PWM_INTERFACE FAN_PWM = {
    .Initialize          = &SCCP3_PWM_Initialize,
    .Deinitialize        = &SCCP3_PWM_Deinitialize,
    .Enable              = &SCCP3_PWM_Enable,
    .Disable             = &SCCP3_PWM_Disable,
    .PeriodSet           = &SCCP3_PWM_PeriodSet,
    .DutyCycleSet        = &SCCP3_PWM_DutyCycleSet,
    .SoftwareTriggerSet  = &SCCP3_PWM_SoftwareTriggerSet,
    .DeadTimeSet         = NULL,
    .OutputModeSet       = NULL,
    .CallbackRegister = &SCCP3_PWM_CallbackRegister,
    .Tasks               = &SCCP3_PWM_Tasks
};

// Section: SCCP3 Module APIs

void SCCP3_PWM_Initialize (void)
{
    // MOD Dual Edge Compare, Buffered(PWM); CCSEL disabled; TMR32 16 Bit; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; CCPSLP disabled; CCPSIDL disabled; CCPON disabled; 
    CCP3CON1L = 0x5;
    // SYNC None; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; IOPS Each Time Base Period Match; RTRGEN disabled; OPSRC Timer Interrupt Event; 
    CCP3CON1H = 0x0;
    // ASDG 0x0; SSDG disabled; ASDGM disabled; PWMRSEN disabled; 
    CCP3CON2L = 0x0;
    // ICSEL ; AUXOUT Disabled; ICGSM Level-Sensitive mode; OCAEN enabled; OENSYNC disabled; 
    CCP3CON2H = 0x100;
    // PSSACE Tri-state; POLACE disabled; OSCNT None; OETRIG disabled; 
    CCP3CON3H = 0x0;
    // ICOV disabled; ICDIS disabled; SCEVT disabled; ASEVT disabled; TRCLR disabled; TRSET disabled; ICGARM disabled; 
    CCP3STATL = 0x0;
    // TMRL 0x0000; 
    CCP3TMRL = 0x0;
    // TMRH 0x0000; 
    CCP3TMRH = 0x0;
    // PRL 5000; 
    CCP3PRL = 0x1388;
    // PRH 0; 
    CCP3PRH = 0x0;
    // CMPA 0; 
    CCP3RA = 0x0;
    // CMPB 0; 
    CCP3RB = 0x0;
    // BUFL 0x0000; 
    CCP3BUFL = 0x0;
    // BUFH 0x0000; 
    CCP3BUFH = 0x0;
    SCCP3_PWM_CallbackRegister(&SCCP3_PWM_Callback);
    
    CCP3CON1Lbits.CCPON = 1; //Enable Module
}

void SCCP3_PWM_Deinitialize (void)
{
    CCP3CON1Lbits.CCPON = 0;
    
    CCP3CON1L = 0x0;
    CCP3CON1H = 0x0;
    CCP3CON2L = 0x0;
    CCP3CON2H = 0x100;
    CCP3CON3H = 0x0;
    CCP3STATL = 0x0;
    CCP3TMRL = 0x0;
    CCP3TMRH = 0x0;
    CCP3PRL = 0xFFFF;
    CCP3PRH = 0xFFFF;
    CCP3RA = 0x0;
    CCP3RB = 0x0;
    CCP3BUFL = 0x0;
    CCP3BUFH = 0x0;
}

void SCCP3_PWM_Enable( void )
{
    CCP3CON1Lbits.CCPON = 1;
}


void SCCP3_PWM_Disable( void )
{
    CCP3CON1Lbits.CCPON = 0;
}

void SCCP3_PWM_PeriodSet(size_t periodCount)
{
    CCP3PRL = periodCount;
}

void SCCP3_PWM_DutyCycleSet(size_t dutyCycleCount)
{
    CCP3RB = dutyCycleCount;
}

void SCCP3_PWM_SoftwareTriggerSet( void )
{
    CCP3STATLbits.TRSET = 1;
}

void SCCP3_PWM_CallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        SCCP3_PWMHandler = handler;
    }
}

void __attribute__ ((weak)) SCCP3_PWM_Callback ( void )
{ 

} 


void SCCP3_PWM_Tasks( void )
{    
    if(IFS2bits.CCT3IF == 1)
    {
        // SCCP3 callback function 
        if(NULL != SCCP3_PWMHandler)
        {
            (*SCCP3_PWMHandler)();
        }
        IFS2bits.CCT3IF = 0;
    }
}
/**
 End of File
*/
