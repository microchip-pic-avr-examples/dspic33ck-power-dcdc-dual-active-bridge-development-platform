/**
 * MCCP-InputCapture Generated Driver Interface Header File
 * 
 * @file 	  input_capture_interface.h
 * 
 * @defgroup  inputcapturedriver Input Capture Driver
 * 
 * @brief 	  Input Capture Driver is a 16-bit timer or 32-bit timer that is useful in 
 *            applications requiring frequency (time period) and pulse measurement 
 *            using dsPIC MCUs.
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

#ifndef INPUT_CAPTURE_INTERFACE_H
#define INPUT_CAPTURE_INTERFACE_H

// Section: Included Files

#include <stdbool.h>
#include <stdint.h>

// Section: Data Type Definitions
        
/**
 @ingroup  inputcapturedriver
 @struct   INPUT_CAPTURE_INTERFACE
 @brief    Structure containing the function pointers of MCCP-InputCapture driver
*/
struct INPUT_CAPTURE_INTERFACE
{   
    void (*Initialize)(void);													
    ///< Pointer to SCCPx_InputCapture_Initialize e.g. \ref SCCP1_InputCapture_Initialize
    
    void (*Deinitialize)(void);                                                 
    ///< Pointer to SCCPx_InputCapture_Deinitialize e.g. \ref SCCP1_InputCapture_Deinitialize
    
    void (*Start)(void);                                                        
    ///< Pointer to SCCPx_InputCapture_Start e.g. \ref SCCP1_InputCapture_Start
    
    void (*Stop)(void);                                                         
    ///< Pointer to SCCPx_InputCapture_Stop e.g. \ref SCCP1_InputCapture_Stop
    
    void (*InputCapture_CallbackRegister)(void (*CallbackHandler)(void));   
    ///< Pointer to SCCPx_InputCapture_CallbackRegister e.g. \ref SCCP1_InputCapture_CallbackRegister
    
    void (*Tasks)(void);                                                        
    ///< Pointer to SCCPx_InputCapture_Tasks e.g. \ref SCCP1_InputCapture_Tasks (Supported only in polling mode)
    
    uint32_t (*DataRead)(void);                                                 
    ///< Pointer to SCCPx_InputCapture_DataRead e.g. \ref SCCP1_InputCapture_DataRead
    
    bool (*HasBufferOverflowed)(void);                                          
    ///< Pointer to SCCPx_InputCapture_HasBufferOverflowed e.g. \ref SCCP1_InputCapture_HasBufferOverflowed
    
    bool (*IsBufferEmpty)(void);                                                
    ///< Pointer to SCCPx_InputCapture_IsBufferEmpty e.g. \ref SCCP1_InputCapture_IsBufferEmpty
    
    void (*OverflowFlagClear)(void);                                            
    ///< Pointer to SCCPx_InputCapture_OverflowFlagClear e.g. \ref SCCP1_InputCapture_OverflowFlagClear
};

#endif //INPUT_CAPTURE_INTERFACE_H


