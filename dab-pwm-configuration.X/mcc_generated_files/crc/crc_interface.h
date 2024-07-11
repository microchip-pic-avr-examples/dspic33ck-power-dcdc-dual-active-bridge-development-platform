/**
 * CRC Generated Driver Interface Header File
 * 
 * @file      crc_interface.h
 *            
 * @defgroup  crcdriver CRC Driver
 *            
 * @brief     16-bit Programmable Cyclic Redundancy Check generator using dsPIC MCUs
 *            
 * @skipline @version   Firmware Driver Version 1.2.2
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

#ifndef CRC_INTERFACE_H
#define CRC_INTERFACE_H

// Section: Included Files

#include <stdbool.h>
#include "crc_types.h"

// Section: Data Type Definitions
        
/**
 @ingroup  crcdriver
 @struct   CRC_INTERFACE
 @brief    Structure containing the function pointers of CRC driver
*/

struct CRC_INTERFACE
{
    void (*Initialize)(void);
    ///< Pointer to \ref CRC_Initialize
    
    void (*Deinitialize)(void);
    ///< Pointer to \ref CRC_Deinitialize
    
    void (*SeedSet)(uint32_t seed, enum CRC_SEED_METHOD seedMethod, enum CRC_SEED_DIRECTION seedDirection);
    ///< Pointer to \ref CRC_SeedSet
    
    void (*CalculateBufferStart)(void *buffer, uint32_t sizeBytes);
    ///< Pointer to \ref CRC_CalculateBufferStart
    
    void (*CalculateProgramStart)(uint32_t startAddr, uint32_t sizeBytes);
    ///< Pointer to \ref CRC_CalculateProgramStart
    
    bool (*CalculationIsDone)(void);
    ///< Pointer to \ref CRC_CalculationIsDone
    
    uint32_t (*CalculationResultGet)(bool reverse, uint32_t xorValue);
    ///< Pointer to \ref CRC_CalculationResultGet
    
    uint32_t (*CalculationResultRawGet)(void);
    ///< Pointer to \ref CRC_CalculationResultRawGet
    
    uint32_t (*CalculationResultReverseGet)(void);
    ///< Pointer to \ref CRC_CalculationResultReverseGet   
    
    uint32_t (*CalculationResultXORGet)(uint32_t xorValue);
    ///< Pointer to \ref CRC_CalculationResultXORGet
    
    void (*EventCallbackRegister)(void (*CallbackHandler)(void));
    ///< Pointer to \ref CRC_EventCallbackRegister
    
    void (*Tasks)(void);
    ///< Pointer to \ref CRC_Tasks (Supported only in polling mode)
};

#endif  //CRC_INTERFACE_H
/**
 End of File
*/
