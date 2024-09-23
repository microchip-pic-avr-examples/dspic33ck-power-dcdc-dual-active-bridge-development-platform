/**
 * CMP2 Generated Driver Source File
 * 
 * @file      cmp2.c
 *            
 * @ingroup   cmpdriver
 *            
 * @brief     This is the generated driver source file for CMP2 driver
 *            
 * @skipline @version   Firmware Driver Version 1.2.1
 *
 * @skipline @version   PLIB Version 1.4.2
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
#include "../cmp2.h"

// Section: File specific functions

static void (*CMP2_EventHandler)(void) = NULL;

// Section: Driver Interface
const struct DAC_DC_INTERFACE dac2_dc_interface = {
    .Enable = &CMP2_DACEnable,
    .Disable = &CMP2_DACDisable,
    .DataWrite = &CMP2_DACDataWrite,
};

const struct CMP_INTERFACE DAC_DEBUG = {
    .Initialize = &CMP2_Initialize,
    .Deinitialize = &CMP2_Deinitialize,
    .Enable = &CMP2_Enable,
    .Disable = &CMP2_Disable,
    .StatusGet = &CMP2_StatusGet,
    
    .EventCallbackRegister = &CMP2_EventCallbackRegister,
    .Tasks = &CMP2_Tasks,
    .cmp_dac_dc_interface = &dac2_dc_interface
};

// Section: CMP2 Module APIs

void CMP2_Initialize(void)
{           
    // Comparator Register settings
    DACCTRL1L = 0x40; //FCLKDIV 1:1; CLKDIV 1:1; CLKSEL FVCO/2; DACSIDL disabled; DACON disabled; 
    DACCTRL2H = 0x8A; //SSTIME 138; 
    DACCTRL2L = 0x55; //TMODTIME 85; 
    DAC2CONH = 0x0; //TMCB 0; 
    DAC2CONL = 0x8200; //HYSSEL None; HYSPOL Rising Edge; INSEL CMP2A; CMPPOL Non Inverted; FLTREN disabled; DACOEN enabled; CBE disabled; IRQM Interrupts are disabled; DACEN enabled; 

    //Slope Settings
    DAC2DATH = 0xCD; //DACDATH 205; 
    DAC2DATL = 0xCD; //DACDATL 205; 
    SLP2CONH = 0x0; //PSE Negative; TWME disabled; HME disabled; SLOPEN disabled; 
    SLP2CONL = 0x0; //SLPSTRT None; SLPSTOPB None; SLPSTOPA None; HCFSEL None; 
    SLP2DAT = 0x0; //SLPDAT 0; 
    
    CMP2_EventCallbackRegister(&CMP2_EventCallback);
    
    
    DACCTRL1Lbits.DACON = 1;
}

void CMP2_Deinitialize(void)
{ 
    DACCTRL1Lbits.DACON = 0;
    
    
    // Comparator Register settings
    DACCTRL1L = 0x0;
    DACCTRL2H = 0x8A;
    DACCTRL2L = 0x55;
    DAC2CONH = 0x0;
    DAC2CONL = 0x0;

    //Slope Settings
    DAC2DATH = 0x0;
    DAC2DATL = 0x0;
    SLP2CONH = 0x0;
    SLP2CONL = 0x0;
    SLP2DAT = 0x0;
}

bool CMP2_StatusGet(void)
{
    return (DAC2CONLbits.CMPSTAT);
}

void CMP2_Enable(void)
{
    DACCTRL1Lbits.DACON = 1;
}

void CMP2_Disable(void)
{
    DACCTRL1Lbits.DACON = 0;
}

void CMP2_DACEnable(void)
{
    DAC2CONLbits.DACEN = 1;
}

void CMP2_DACDisable(void)
{
    DAC2CONLbits.DACEN = 0;
}

void CMP2_DACDataWrite(size_t value)
{
    DAC2DATHbits.DACDATH = value;
}

void CMP2_EventCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CMP2_EventHandler = handler;
    }
}

void __attribute__ ((weak)) CMP2_EventCallback(void)
{ 
   
} 

void CMP2_Tasks(void)
{
    if(IFS4bits.CMP2IF == 1)
    {
        // CMP2 callback function 
        if(NULL != CMP2_EventHandler)
        {
            (*CMP2_EventHandler)();
        }
    
        // clear the CMP2 interrupt flag
        IFS4bits.CMP2IF = 0;
    }
}

/**
 End of File
*/