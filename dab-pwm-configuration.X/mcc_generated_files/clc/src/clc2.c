/**
 * CLC2 Generated Driver Source File
 * 
 * @file      clc2.c
 *            
 * @ingroup   clcdriver
 *            
 * @brief     This is the generated driver source file for CLC2 driver
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
#include "../clc2.h"

// Section: File specific functions

static void (*CLC2_PositiveEdgeHandler)(void) = NULL;
static void (*CLC2_NegativeEdgeHandler)(void) = NULL;

// Section: File specific data type definitions

//Defines an object for CLC_INTERFACE

const struct CLC_INTERFACE CLC2 = {
    .Initialize         = &CLC2_Initialize,
    .Deinitialize       = &CLC2_Deinitialize,
    .Enable             = &CLC2_Enable,
    .Disable            = &CLC2_Disable,
    .OutputStatusGet    =&CLC2_OutputStatusGet,
    .PositiveEdgeCallbackRegister = &CLC2_PositiveEdgeCallbackRegister,
    .NegativeEdgeCallbackRegister = &CLC2_NegativeEdgeCallbackRegister,
    .Tasks              = &CLC2_Tasks,
};
// Section: Driver Interface Function Definitions

void CLC2_Initialize(void)
{
    // Set the CLC2 to the options selected in the User Interface

    // LCEN enabled; LCINTN disabled; LCPOL non_inverted; LCOUT Low; LCINTP disabled; LCMODE AND-OR; LCOE enabled; 
    CLC2CONL = 0x8080;
    // LCG1POL non_inverted; LCG2POL non_inverted; LCG3POL non_inverted; LCG4POL non_inverted; 
    CLC2CONH = 0x0;
    // LCD2S CLCINB I/O pin; LCD1S CLCINA I/O pin; LCD4S CLCIND pin; LCD3S CLCINC I/O pin; 
    CLC2SELL = 0x5000;
    // LCG1D1T enabled; LCG1D3T enabled; LCG2D2T enabled; LCG1D2T disabled; LCG2D1T enabled; LCG2D4T disabled; LCG1D4T disabled; LCG2D3T disabled; LCG1D1N disabled; LCG1D3N disabled; LCG2D2N disabled; LCG1D2N disabled; LCG2D1N disabled; LCG2D4N disabled; LCG1D4N disabled; LCG2D3N disabled; 
    CLC2GLSL = 0xA22;
    // LCG3D1T enabled; LCG3D2T disabled; LCG4D1T enabled; LCG3D3T enabled; LCG4D2T enabled; LCG3D4T disabled; LCG4D3T disabled; LCG4D4T disabled; LCG3D1N disabled; LCG3D2N disabled; LCG4D1N disabled; LCG3D3N disabled; LCG4D2N disabled; LCG3D4N disabled; LCG4D3N disabled; LCG4D4N disabled; 
    CLC2GLSH = 0xA22;
    
    CLC2_PositiveEdgeCallbackRegister(&CLC2_PositiveEdgeCallback);
    CLC2_NegativeEdgeCallbackRegister(&CLC2_NegativeEdgeCallback);
    
    CLC2_Enable();
    
}

void CLC2_Deinitialize(void)
{
    CLC2_Disable();
    
    CLC2CONL = 0x0;
    CLC2CONH = 0x0;
    CLC2SELL = 0x0;
    CLC2GLSL = 0x0;
    CLC2GLSH = 0x0;
}

void CLC2_PositiveEdgeCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CLC2_PositiveEdgeHandler = handler;
    }
}

void __attribute__ ((weak)) CLC2_PositiveEdgeCallback ( void )
{ 

} 

void CLC2_NegativeEdgeCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CLC2_NegativeEdgeHandler = handler;
    }
}

void __attribute__ ((weak)) CLC2_NegativeEdgeCallback ( void )
{ 

} 



void CLC2_Tasks( void )
{
    if(IFS7bits.CLC2PIF == 1)
    {
        if(NULL != CLC2_PositiveEdgeHandler)
        {
            (*CLC2_PositiveEdgeHandler)();
        }
        
        // Clear the CLC2 Positive Edge Interrupt flag
        IFS7bits.CLC2PIF = 0;
    }
    if(IFS11bits.CLC2NIF == 1)
    {
        if(NULL != CLC2_NegativeEdgeHandler)
        {
            (*CLC2_NegativeEdgeHandler)();
        }
        
        // Clear the CLC2 Negative Edge Interrupt flag
        IFS11bits.CLC2NIF = 0;
    }
}

bool CLC2_OutputStatusGet(void)
{
    return(CLC2CONLbits.LCOUT);

}
/**
 End of File
*/

