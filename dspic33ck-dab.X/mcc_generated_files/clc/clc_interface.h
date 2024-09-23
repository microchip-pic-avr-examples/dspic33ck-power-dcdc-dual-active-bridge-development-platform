/**
 * CLC Generated Driver Interface Header File
 * 
 * @file      clc_interface.h
 *            
 * @defgroup  clcdriver CLC Driver
 *            
 * @brief     Configurable Logic Cell driver allows the user to specify 
 *            combinations of signals as inputs to a logic function and 
 *            to use the logic output to control other peripherals or 
 *            I/O pins using dsPIC MCUs.
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

#ifndef CLC_INTERFACE_H
#define CLC_INTERFACE_H

// Section: Data Type Definitions

/**
 @ingroup  clcdriver
 @struct   CLC_INTERFACE
 @brief    Structure containing the function pointers of CLC driver
*/
struct CLC_INTERFACE
{   
    void (*Initialize)(void);       
    ///< Pointer to CLCx_Initialize e.g. \ref CLC1_Initialize
    
    void (*Deinitialize)(void);     
    ///< Pointer to CLCx_Deinitialize e.g. \ref CLC1_Deinitialize
    
    void (*Enable)(void);           
    ///< Pointer to CLCx_Enable e.g. \ref CLC1_Enable
    
    void (*Disable)(void);          
    ///< Pointer to CLCx_Disable e.g. \ref CLC1_Disable
    
    bool (*OutputStatusGet)(void);  
    ///< Pointer to CLCx_OutputStatusGet e.g. \ref CLC1_OutputStatusGet
    
    void (*EdgeCallbackRegister)(void (*CallbackHandler)(void)); 
    ///< Pointer to CLCx_EdgeCallbackRegister e.g. \ref CLC1_EdgeCallbackRegister
   
    void (*PositiveEdgeCallbackRegister)(void (*CallbackHandler)(void)); 
    ///< Pointer to CLCx_PositiveEdgeCallbackRegister e.g. \ref CLC1_PositiveEdgeCallbackRegister
    
    void (*NegativeEdgeCallbackRegister)(void (*CallbackHandler)(void)); 
    ///< Pointer to CLCx_NegativeEdgeCallbackRegister e.g. \ref CLC1_NegativeEdgeCallbackRegister
    
    void (*Tasks)(void);     
    ///< Pointer to CLCx_Tasks e.g. \ref CLC1_Tasks (Supported only in polling mode)

};


#endif //CLC_INTERFACE_H


