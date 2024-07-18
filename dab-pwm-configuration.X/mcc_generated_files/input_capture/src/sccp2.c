/**
 * SCCP2-InputCapture Generated Driver Source File
 * 
 * @file 	  sccp2.c
 * 
 * @ingroup   mccpdriver
 * 
 * @brief 	  This is the generated driver source file for SCCP2-InputCapture driver
 *
 * @skipline @version   Firmware Driver Version 1.3.0
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
#include "../sccp2.h"

// Section: File specific functions

static void (*SCCP2_InputCaptureHandler)(void) = NULL;

// Section: Driver Interface

const struct INPUT_CAPTURE_INTERFACE FB_VDD_PWM = {
    .Initialize          = &SCCP2_InputCapture_Initialize,
    .Deinitialize        = &SCCP2_InputCapture_Deinitialize,
    .Start               = &SCCP2_InputCapture_Start,
    .Stop                = &SCCP2_InputCapture_Stop,
    .InputCapture_CallbackRegister = &SCCP2_InputCapture_CallbackRegister,
    .Tasks               = &SCCP2_InputCapture_Tasks,
    .DataRead            = &SCCP2_InputCapture_DataRead,
    .HasBufferOverflowed = &SCCP2_InputCapture_HasBufferOverflowed,
    .IsBufferEmpty       = &SCCP2_InputCapture_IsBufferEmpty,
    .OverflowFlagClear   = &SCCP2_InputCapture_OverflowFlagClear,
};

// Section: Driver Interface Function Definitions

void SCCP2_InputCapture_Initialize(void)
{
    // MOD Every falling edge; CCSEL enabled; TMR32 16 Bit; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; CCPSLP disabled; CCPSIDL disabled; CCPON disabled; 
    CCP2CON1L = 0x12; //The module is disabled, till other settings are configured.
    //SYNC CLC2; ALTSYNC disabled; ONESHOT disabled; TRIGEN enabled; IOPS Each Time Base Period Match; RTRGEN disabled; OPSRC Timer Interrupt Event; 
    CCP2CON1H = 0x91;
    //ASDG 0x0; SSDG disabled; ASDGM disabled; PWMRSEN disabled; 
    CCP2CON2L = 0x0;
    //ICSEL None; AUXOUT Disabled; ICGSM Level-Sensitive mode; OCAEN disabled; OENSYNC disabled; 
    CCP2CON2H = 0x0;
    //PSSACE Tri-state; POLACE disabled; OSCNT None; OETRIG disabled; 
    CCP2CON3H = 0x0;
    //ICOV disabled; ICDIS disabled; SCEVT disabled; ASEVT disabled; TRCLR disabled; TRSET disabled; ICGARM disabled; 
    CCP2STATL = 0x0;
    //TMRL 0x0000; 
    CCP2TMRL = 0x0;
    //TMRH 0x0000; 
    CCP2TMRH = 0x0;
    //PRL 0; 
    CCP2PRL = 0x0;
    //PRH 0; 
    CCP2PRH = 0x0;
    //CMPA 0; 
    CCP2RA = 0x0;
    //CMPB 0; 
    CCP2RB = 0x0;
    //BUFL 0x0000; 
    CCP2BUFL = 0x0;
    //BUFH 0x0000; 
    CCP2BUFH = 0x0;
    
    SCCP2_InputCapture_CallbackRegister(&SCCP2_InputCapture_Callback);


    CCP2CON1Lbits.CCPON = 1; //Enable Module

}

void SCCP2_InputCapture_Deinitialize(void)
{
    CCP2CON1Lbits.CCPON = 0;
    
    
    CCP2CON1L = 0x0;
    CCP2CON1H = 0x0;
    CCP2CON2L = 0x0;
    CCP2CON2H = 0x100;
    CCP2CON3H = 0x0;
    CCP2STATL = 0x0;
    CCP2TMRL = 0x0;
    CCP2TMRH = 0x0;
    CCP2PRL = 0xFFFF;
    CCP2PRH = 0xFFFF;
    CCP2RA = 0x0;
    CCP2RB = 0x0;
    CCP2BUFL = 0x0;
    CCP2BUFH = 0x0;
}

void SCCP2_InputCapture_Start(void)
{
    
    CCP2CON1Lbits.CCPON = 1;
}

void SCCP2_InputCapture_Stop(void)
{
    CCP2CON1Lbits.CCPON = 0;
    
}

void SCCP2_InputCapture_CallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        SCCP2_InputCaptureHandler = handler;
    }
}

void __attribute__ ((weak)) SCCP2_InputCapture_Callback (void)
{ 

} 

void SCCP2_InputCapture_Tasks(void)
{
    if(IFS1bits.CCP2IF == 1)
    {
        if(NULL != SCCP2_InputCaptureHandler)
        {
            (*SCCP2_InputCaptureHandler)();
        }
        IFS1bits.CCP2IF = 0;
    }
}

uint32_t SCCP2_InputCapture_DataRead(void)
{
    uint32_t captureVal = 0xFFFFFFFFU;

    captureVal = CCP2BUFL;
    captureVal |= ((uint32_t)CCP2BUFH <<16);

    return(captureVal);
}

bool SCCP2_InputCapture_HasBufferOverflowed(void)
{
    return(CCP2STATLbits.ICOV);
}

bool SCCP2_InputCapture_IsBufferEmpty(void)
{
    return(!CCP2STATLbits.ICBNE);
}

void SCCP2_InputCapture_OverflowFlagClear(void)
{
    CCP2STATLbits.ICOV = 0;
}

/**
 End of File
*/
