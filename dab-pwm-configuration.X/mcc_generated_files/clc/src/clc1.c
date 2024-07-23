/**
 * CLC1 Generated Driver Source File
 * 
 * @file      clc1.c
 *            
 * @ingroup   clcdriver
 *            
 * @brief     This is the generated driver source file for CLC1 driver
 *            
 * @skipline @version   Firmware Driver Version 1.2.1
 *
 * @skipline @version   PLIB Version 1.3.0
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
#include <stddef.h>
#include "../clc1.h"

// Section: File specific functions

static void (*CLC1_PositiveEdgeHandler)(void) = NULL;
static void (*CLC1_NegativeEdgeHandler)(void) = NULL;

// Section: File specific data type definitions

//Defines an object for CLC_INTERFACE

const struct CLC_INTERFACE CLC1 = {
    .Initialize         = &CLC1_Initialize,
    .Deinitialize       = &CLC1_Deinitialize,
    .Enable             = &CLC1_Enable,
    .Disable            = &CLC1_Disable,
    .OutputStatusGet    =&CLC1_OutputStatusGet,
    .PositiveEdgeCallbackRegister = &CLC1_PositiveEdgeCallbackRegister,
    .NegativeEdgeCallbackRegister = &CLC1_NegativeEdgeCallbackRegister,
    .Tasks              = &CLC1_Tasks,
};
// Section: Driver Interface Function Definitions

void CLC1_Initialize(void)
{
    // Set the CLC1 to the options selected in the User Interface

    // LCEN enabled; LCINTN disabled; LCPOL non_inverted; LCOUT Low; LCINTP disabled; LCMODE OR-XOR; LCOE enabled; 
    CLC1CONL = 0x8081;
    // LCG1POL non_inverted; LCG2POL non_inverted; LCG3POL non_inverted; LCG4POL non_inverted; 
    CLC1CONH = 0x0;
    // LCD2S CMP1 output; LCD1S CLCINA I/O pin; LCD4S CMP3 output; LCD3S CLCINC I/O pin; 
    CLC1SELL = 0x2020;
    // LCG1D1T disabled; LCG1D3T disabled; LCG2D2T enabled; LCG1D2T disabled; LCG2D1T disabled; LCG2D4T disabled; LCG1D4T disabled; LCG2D3T disabled; LCG1D1N disabled; LCG1D3N disabled; LCG2D2N disabled; LCG1D2N disabled; LCG2D1N disabled; LCG2D4N disabled; LCG1D4N disabled; LCG2D3N disabled; 
    CLC1GLSL = 0x800;
    // LCG3D1T disabled; LCG3D2T disabled; LCG4D1T disabled; LCG3D3T disabled; LCG4D2T disabled; LCG3D4T disabled; LCG4D3T disabled; LCG4D4T enabled; LCG3D1N disabled; LCG3D2N disabled; LCG4D1N disabled; LCG3D3N disabled; LCG4D2N disabled; LCG3D4N disabled; LCG4D3N disabled; LCG4D4N disabled; 
    CLC1GLSH = 0x8000;
    
    CLC1_PositiveEdgeCallbackRegister(&CLC1_PositiveEdgeCallback);
    CLC1_NegativeEdgeCallbackRegister(&CLC1_NegativeEdgeCallback);
    
    CLC1_Enable();
    
}

void CLC1_Deinitialize(void)
{
    CLC1_Disable();
    
    CLC1CONL = 0x0;
    CLC1CONH = 0x0;
    CLC1SELL = 0x0;
    CLC1GLSL = 0x0;
    CLC1GLSH = 0x0;
}

void CLC1_PositiveEdgeCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CLC1_PositiveEdgeHandler = handler;
    }
}

void __attribute__ ((weak)) CLC1_PositiveEdgeCallback ( void )
{ 

} 

void CLC1_NegativeEdgeCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CLC1_NegativeEdgeHandler = handler;
    }
}

void __attribute__ ((weak)) CLC1_NegativeEdgeCallback ( void )
{ 

} 



void CLC1_Tasks( void )
{
    if(IFS7bits.CLC1PIF == 1)
    {
        if(NULL != CLC1_PositiveEdgeHandler)
        {
            (*CLC1_PositiveEdgeHandler)();
        }
        
        // Clear the CLC1 Positive Edge Interrupt flag
        IFS7bits.CLC1PIF = 0;
    }
    if(IFS11bits.CLC1NIF == 1)
    {
        if(NULL != CLC1_NegativeEdgeHandler)
        {
            (*CLC1_NegativeEdgeHandler)();
        }
        
        // Clear the CLC1 Negative Edge Interrupt flag
        IFS11bits.CLC1NIF = 0;
    }
}

bool CLC1_OutputStatusGet(void)
{
    return(CLC1CONLbits.LCOUT);

}
/**
 End of File
*/

