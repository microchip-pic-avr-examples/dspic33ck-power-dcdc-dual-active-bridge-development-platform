/**
 * CMP3 Generated Driver Source File
 * 
 * @file      cmp3.c
 *            
 * @ingroup   cmpdriver
 *            
 * @brief     This is the generated driver source file for CMP3 driver
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
#include "../cmp3.h"

// Section: File specific functions

static void (*CMP3_EventHandler)(void) = NULL;

// Section: Driver Interface
const struct DAC_DC_INTERFACE dac3_dc_interface = {
    .Enable = &CMP3_DACEnable,
    .Disable = &CMP3_DACDisable,
    .DataWrite = &CMP3_DACDataWrite,
};

const struct CMP_INTERFACE CMP_IPRI_SC = {
    .Initialize = &CMP3_Initialize,
    .Deinitialize = &CMP3_Deinitialize,
    .Enable = &CMP3_Enable,
    .Disable = &CMP3_Disable,
    .StatusGet = &CMP3_StatusGet,
    
    .EventCallbackRegister = &CMP3_EventCallbackRegister,
    .Tasks = NULL,
    .cmp_dac_dc_interface = &dac3_dc_interface
};

// Section: CMP3 Module APIs

void CMP3_Initialize(void)
{           
    // Comparator Register settings
    DACCTRL1L = 0x77; //FCLKDIV 1:8; CLKDIV 1:4; CLKSEL FVCO/2; DACSIDL disabled; DACON disabled; 
    DACCTRL2H = 0x8A; //SSTIME 138; 
    DACCTRL2L = 0x55; //TMODTIME 85; 
    DAC3CONH = 0x0; //TMCB 0; 
    DAC3CONL = 0xA108; //HYSSEL None; HYSPOL Rising Edge; INSEL CMP3B; CMPPOL Non Inverted; FLTREN enabled; DACOEN disabled; CBE disabled; IRQM Rising edge detect; DACEN enabled; 

    //Slope Settings
    DAC3DATH = 0xF32; //DACDATH 3890; 
    DAC3DATL = 0xCD; //DACDATL 205; 
    SLP3CONH = 0x0; //PSE Negative; TWME disabled; HME disabled; SLOPEN disabled; 
    SLP3CONL = 0x0; //SLPSTRT None; SLPSTOPB None; SLPSTOPA None; HCFSEL None; 
    SLP3DAT = 0x0; //SLPDAT 0; 
    
    CMP3_EventCallbackRegister(&CMP3_EventCallback);
    
    // Clearing IF flag before enabling the interrupt.
    IFS4bits.CMP3IF = 0;
    // Enabling CMP3 interrupt.
    IEC4bits.CMP3IE = 1;
    
    DACCTRL1Lbits.DACON = 1;
}

void CMP3_Deinitialize(void)
{ 
    DACCTRL1Lbits.DACON = 0;
    
    IFS4bits.CMP3IF = 0;
    IEC4bits.CMP3IE = 0;
    
    // Comparator Register settings
    DACCTRL1L = 0x0;
    DACCTRL2H = 0x8A;
    DACCTRL2L = 0x55;
    DAC3CONH = 0x0;
    DAC3CONL = 0x0;

    //Slope Settings
    DAC3DATH = 0x0;
    DAC3DATL = 0x0;
    SLP3CONH = 0x0;
    SLP3CONL = 0x0;
    SLP3DAT = 0x0;
}

bool CMP3_StatusGet(void)
{
    return (DAC3CONLbits.CMPSTAT);
}

void CMP3_Enable(void)
{
    DACCTRL1Lbits.DACON = 1;
}

void CMP3_Disable(void)
{
    DACCTRL1Lbits.DACON = 0;
}

void CMP3_DACEnable(void)
{
    DAC3CONLbits.DACEN = 1;
}

void CMP3_DACDisable(void)
{
    DAC3CONLbits.DACEN = 0;
}

void CMP3_DACDataWrite(size_t value)
{
    DAC3DATHbits.DACDATH = value;
}

void CMP3_EventCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CMP3_EventHandler = handler;
    }
}

void __attribute__ ((weak)) CMP3_EventCallback(void)
{ 
   
} 

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CMP3Interrupt(void)
{
    // CMP3 callback function 
    if(NULL != CMP3_EventHandler)
    {
        (*CMP3_EventHandler)();
    }
    
    // clear the CMP3 interrupt flag
    IFS4bits.CMP3IF = 0;
}

/**
 End of File
*/