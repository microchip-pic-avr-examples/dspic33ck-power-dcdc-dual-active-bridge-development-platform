/**
 * CLC2 Generated Driver Header File
 * 
 * @file      clc2.h
 *            
 * @ingroup   clcdriver
 *            
 * @brief     This is the generated driver header file for the CLC2 driver
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

#ifndef CLC2_H
#define CLC2_H

// Section: Included Files

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "clc_interface.h"

// Section: Data Type Definitions

/**
 @ingroup  clcdriver
 @brief    Structure object of type CLC_INTERFACE with the 
           custom name given by the user in the Melody Driver User interface. 
           The default name e.g. CLC1 can be changed by the 
           user in the CLC user interface. 
           This allows defining a structure with application specific name 
           using the 'Custom Name' field. Application specific name allows the 
           API Portability.
*/
extern const struct CLC_INTERFACE CLC2;
// Section: Driver Interface Functions

/**
 * @ingroup  clcdriver
 * @brief    Initializes the CLC2 module
 * @param    none
 * @return   none  
 */
void CLC2_Initialize(void);

/**
 * @ingroup  clcdriver
 * @brief    Deinitializes the CLC2 to POR values
 * @param    none
 * @return   none  
 */
void CLC2_Deinitialize(void);

/**
 * @ingroup  clcdriver
 * @brief    Returns output pin status of the CLC2 module
 * @pre      \ref CLC2_Initialize function should be
 *           called before calling this function.
 * @param    none
 * @return   Output pin status  
 */
bool CLC2_OutputStatusGet(void);

/**
 * @ingroup  clcdriver
 * @brief    This inline function enables the CLC2 module
 * @pre      \ref CLC2_Initialize function should be
 *           called before calling this function.
 * @param    none
 * @return   none  
 */
inline static void CLC2_Enable(void)
{
    CLC2CONLbits.LCEN = 1;
}

/**
 * @ingroup  clcdriver
 * @brief    This inline function disables the CLC2 module
 * @pre      \ref CLC2_Initialize function should be
 *           called before calling this function.
 * @param    none
 * @return   none  
 */
inline static void CLC2_Disable(void)
{
    CLC2CONLbits.LCEN = 0;
}

/**
 * @ingroup    clcdriver
 * @brief      This function can be used to override default callback and to 
 *             define custom callback for CLC2 PositiveEdge event
 * @param[in]  handler - Address of the callback function.  
 * @return     none  
 */
void CLC2_PositiveEdgeCallbackRegister(void (*handler)(void));

/**
 * @ingroup  clcdriver
 * @brief    This is the default callback with weak attribute. 
 *           The user can override and implement the default callback without weak attribute
 *           or can register a custom callback function using  CLC2_PositiveEdgeCallbackRegister
 * @param    none
 * @return   none  
 */
void CLC2_PositiveEdgeCallback(void);

/**
 * @ingroup    clcdriver
 * @brief      This function can be used to override default callback and to 
 *             define custom callback for CLC2 NegativeEdge event
 * @param[in]  handler - Address of the callback function.  
 * @return     none  
 */
void CLC2_NegativeEdgeCallbackRegister(void (*handler)(void));

/**
 * @ingroup  clcdriver
 * @brief    This is the default callback with weak attribute. 
 *           The user can override and implement the default callback without weak attribute
 *           or can register a custom callback function using  CLC2_NegativeEdgeCallbackRegister
 * @param    none
 * @return   none  
 */
void CLC2_NegativeEdgeCallback(void);


/**
 * @ingroup  clcdriver
 * @brief    Implements the tasks for polled implementations where the hardware
 *           supports dedicated event for Positive edge, Negative edge or both.
 * Note:     If both Positive Edge and Negative Edge uses polling mode then Callback 
 *           function of positive edge takes the priority if both event occurred. 
 * @pre      \ref CLC2_Initialize function should be
 *           called before calling this function.
 * @param    none
 * @return   none  
 */
void CLC2_Tasks(void);

#endif  // CLC2_H
/**
 End of File
*/



