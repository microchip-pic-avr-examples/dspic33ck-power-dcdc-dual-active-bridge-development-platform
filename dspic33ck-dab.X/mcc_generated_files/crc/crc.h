/**
 * CRC Generated Driver Header File 
 * 
 * @file      crc.h
 *            
 * @ingroup   crcdriver
 *            
 * @brief     This is the generated driver header file for the CRC driver
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

#ifndef CRC_H
#define CRC_H

// Section: Included Files

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "crc_types.h"
#include "crc_interface.h"
// Section: Data Type Definitions
/**
 * @ingroup  crcdriver
 * @brief    Structure object of type CRC_INTERFACE with the custom name given by 
 *           the user in the Melody Driver User interface. The default name e.g. 
 *           CRC can be changed by the user in the CRC user interface. 
 *           This allows defining a structure with application specific name using 
 *           the 'Custom Name' field. Application specific name allows the API Portability.
*/

extern const struct CRC_INTERFACE CRC;

// Section: CRC Module APIs

/**
 * @ingroup  crcdriver
 * @brief    Initializes the CRC module.
 *           This function sets the polynomial and data width; data and seed shift; 
 *           updates the polynomial and shifts the seed value. After the function is called, 
 *           the CRC module is ready to calculate the CRC of a data buffer.
 * @param    none
 * @return   none 
 */
void CRC_Initialize(void);

/**
 * @ingroup  crcdriver
 * @brief    Deinitializes CRC to POR values
 * @param    none
 * @return   none 
 */
void CRC_Deinitialize(void);

/**
 * @ingroup    crcdriver
 * @brief      Sets the CRC seed with method and direction
 * @pre        The CRC module needs to be initialized with the desired settings.  Please refer to the CRC initialization functions
 * @param[in]  seed - Set the seed value of the CRC calculation
 * @param[in]  seedmethod - Set the seed method, direct or indirect
 * @param[in]  seeddirection - Set the seed MSB or LSB direction, ignored if the seed method is indirect
 * @return     none  
 */
void CRC_SeedSet(uint32_t seed, enum CRC_SEED_METHOD seedMethod, enum CRC_SEED_DIRECTION seedDirection);

/**
 * @ingroup    crcdriver
 * @brief      CRC module calculation on a buffer in data space
 * @pre        The CRC module needs to be initialized with the desired settings
 * @param[in]  buffer - Address of the desired data in data space
 * @param[in]  sizeBytes - Size of the buffer  
 * @return     none  
 */
void CRC_CalculateBufferStart(void *buffer, uint32_t sizeBytes);

/**
 * @ingroup    crcdriver
* @brief       Starts the CRC calculation on a buffer in program space
 * @pre        The CRC module needs to be initialized with the desired settings.
 * @param[in]  startAddr - Starting address of the program space, Each program 
 *             instruction is 3 bytes, the caller should account for that in the size.
 * @param[in]  sizeBytes - Size of the buffer in multiple of 3   
 * @return     none  
 */
void CRC_CalculateProgramStart(uint32_t startAddr, uint32_t sizeBytes);

/**
 * @ingroup    crcdriver
 * @brief      This function can be used to override default callback and to 
 *             define custom callback for CRC Event event
 * @param[in]  handler - Address of the callback function 
 * @return     none  
 */
void CRC_EventCallbackRegister(void (*handler)(void));

/**
 * @ingroup  crcdriver
 * @brief    This is the default callback with weak attribute. 
 *           The user can override and implement the default callback without 
 *           weak attribute or can register a custom callback function using  
 *           CRC_EventCallbackRegister.
 * @param    none
 * @return   none 
 */
void CRC_EventCallback(void);

/**
 * @ingroup  crcdriver
 * @brief    This function cycles through the CRC calculations.  
 *           This function will load the CRC module FIFO with the buffer data.
 * @pre      The CRC module needs to be initialized with the desired settings.  
 *           Please refer to the CRC initialization functions. 
 *           The caller needs to call the function to start the calculation.
 * @param    none
 * @return   none 
 */
void CRC_Tasks(void);

/**
 * @ingroup  crcdriver
 * @brief    Returns the CRC calculation complete status           
 * @pre      The CRC module needs to be initialized with the desired settings.
 *           \ref CRC_CalculateBufferStart or \ref CRC_CalculateProgramStart must be called for the associated
 *			 function to work.
 * @param    none
 * @return   true  - the CRC calculation is complete and result is available in \ref CRC_CalculationResultRawGet
 * @return   false - the CRC calculation in progress
 */
bool CRC_CalculationIsDone(void);

/**
 * @ingroup    crcdriver
 * @brief      Gets the CRC result if the calculation is done
 * @pre        The CRC module needs to be initialized with the desired settings. 
 *             \ref CRC_CalculationIsDone must be called for the associated function to work.
 * @param[in]  reverse  - Reverses the CRC calculated value if true
 * @param[in]  xorValue -  Set the value that will be XOR'd with the CRC result after reversed if desired
 * @return     Returns the CRC result for the module 
 */
uint32_t CRC_CalculationResultGet(bool reverse, uint32_t xorValue);

/**
 * @ingroup  crcdriver
 * @brief    Gets the CRC raw result if the calculation is done
 * @pre      The CRC module needs to be initialized with the desired settings.  
 *           \ref CRC_CalculationIsDone must be called for the associated function to work.
 * @param    none
 * @return   Returns the CRC result for the module  
 */
uint32_t CRC_CalculationResultRawGet(void);

/**
 * @ingroup  crcdriver
 * @brief    Gets the CRC reversed value of result if the calculation is done
 * @pre      The CRC module needs to be initialized with the desired settings.  
 *           \ref CRC_CalculationIsDone must be called for the associated function to work.
 * @param    none
 * @return   Returns the CRC result for the module   
 */
uint32_t CRC_CalculationResultReverseGet(void);

/**
 * @ingroup    crcdriver
 * @brief      Gets the CRC XOR'd value of the result if the calculation is done
 * @pre        The CRC module needs to be initialized with the desired settings.  
 *             \ref CRC_CalculationIsDone must be called for the associated function to work.
 * @param[in]  xorValue - A value that will be XOR'd with the CRC result after reversed if desired
 * @return     Returns the CRC result for the module   
 */
uint32_t CRC_CalculationResultXORGet(uint32_t xorValue);

#endif  //CRC_H
/**
 End of File
*/
