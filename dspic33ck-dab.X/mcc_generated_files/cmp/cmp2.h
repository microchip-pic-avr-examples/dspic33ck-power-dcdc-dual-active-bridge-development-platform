/**
 * CMP2 Generated Driver Header File 
 * 
 * @file      cmp2.h
 *            
 * @ingroup   cmpdriver
 *            
 * @brief     This is the generated driver header file for the CMP2 driver
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

#ifndef CMP2_H
#define CMP2_H

// Section: Included Files

#include <stddef.h>
#include <stdbool.h>
#include "cmp_interface.h"

// Section: Data Type Definitions

/**
 @ingroup  cmpdriver
 @brief    Structure object of type CMP_INTERFACE with the 
           custom name given by the user in the Melody Driver User interface. 
           The default name e.g. CMP_DAC1 can be changed by the 
           user in the CMP user interface. 
           This allows defining a structure with application specific name 
           using the 'Custom Name' field. Application specific name allows the 
           API Portability.
*/
extern const struct CMP_INTERFACE DAC_DEBUG;

/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_Initialize API
 */
#define DAC_DEBUG_Initialize CMP2_Initialize
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_Deinitialize API
 */
#define DAC_DEBUG_Deinitialize CMP2_Deinitialize
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_StatusGet API
 */
#define DAC_DEBUG_StatusGet CMP2_StatusGet
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_Enable API
 */
#define DAC_DEBUG_Enable CMP2_Enable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_Disable API
 */
#define DAC_DEBUG_Disable CMP2_Disable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_DACEnable API
 */
#define DAC_DEBUG_DACEnable CMP2_DACEnable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_DACDisable API
 */
#define DAC_DEBUG_DACDisable CMP2_DACDisable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_DACDataWrite API
 */
#define DAC_DEBUG_DACDataWrite CMP2_DACDataWrite
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_DACDisable API
 */
#define DAC_DEBUG_DACDisable CMP2_DACDisable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_EventCallbackRegister API
 */
#define DAC_DEBUG_EventCallbackRegister CMP2_EventCallbackRegister
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP2_Tasks API
 */
#define DAC_DEBUG_Tasks CMP2_Tasks

// Section: CMP2 Module APIs

/**
 * @ingroup  cmpdriver
 * @brief    Initialize the CMP2 module
 * @param    none
 * @return   none  
 */

void CMP2_Initialize(void);

/**
 * @ingroup  cmpdriver
 * @brief    Deinitializes the CMP2 to POR values
 * @param    none
 * @return   none  
 */
void CMP2_Deinitialize(void);
        
/**
 * @ingroup  cmpdriver
 * @brief    Returns the comparator output status 
 * @param    none
 * @return   true   - Comparator output is high
 * @return   false  - Comparator output is low
 */
bool CMP2_StatusGet(void);

/**
 * @ingroup  cmpdriver
 * @brief    Enables the common DAC module
 * @param    none
 * @return   none 
 */
void CMP2_Enable(void);
    
/**
 * @ingroup  cmpdriver
 * @brief    Disables the common DAC module
 * @param    none
 * @return   none  
 */
void CMP2_Disable(void);

/**
 * @ingroup  cmpdriver
 * @brief    Enables the individual DAC module
 * @param    none
 * @return   none 
 */
void CMP2_DACEnable(void);
    
/**
 * @ingroup  cmpdriver
 * @brief    Disables the individual DAC module
 * @param    none
 * @return   none
 */
void CMP2_DACDisable(void);

/**
 * @ingroup    cmpdriver
 * @brief      CMP DAC Data write to register
 * @param[in]  value - DAC Data write value
 * @return     none 
 */
void CMP2_DACDataWrite(size_t value);

/**
 * @ingroup    cmpdriver
 * @brief      This function can be used to override default callback and to 
 *             define custom callback for CMP2 Event event
 * @param[in]  handler - Address of the callback function.  
 * @return     none  
 */
void CMP2_EventCallbackRegister(void (*handler)(void));

/**
 * @ingroup  cmpdriver
 * @brief    This is the default callback with weak attribute. 
 *           The user can override and implement the default callback without 
 *           weak attribute or can register a custom callback function using  
 *           CMP2_EventCallbackRegister.
 * @param    none
 * @return   none  
 */
void CMP2_EventCallback(void);

/**
 * @ingroup  cmpdriver
 * @brief    The Task function can be called in the main application using the High Speed
 *           Comparator, when interrupts are not used.  This would thus introduce the 
 *           polling mode feature of the Analog Comparator.
 * @param    none
 * @return   none  
 */
void CMP2_Tasks(void);

#endif //CMP2_H

/**
  End of File
*/

