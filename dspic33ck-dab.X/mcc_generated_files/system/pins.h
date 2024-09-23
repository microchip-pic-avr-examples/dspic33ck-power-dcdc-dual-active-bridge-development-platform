/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC10 GPIO Pin which has a custom name of GPIO_1 to High
 * @pre      The RC10 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define GPIO_1_SetHigh()          (_LATC10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC10 GPIO Pin which has a custom name of GPIO_1 to Low
 * @pre      The RC10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_1_SetLow()           (_LATC10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC10 GPIO Pin which has a custom name of GPIO_1
 * @pre      The RC10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_1_Toggle()           (_LATC10 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC10 GPIO Pin which has a custom name of GPIO_1
 * @param    none
 * @return   none  
 */
#define GPIO_1_GetValue()         _RC10

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC10 GPIO Pin which has a custom name of GPIO_1 as Input
 * @param    none
 * @return   none  
 */
#define GPIO_1_SetDigitalInput()  (_TRISC10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC10 GPIO Pin which has a custom name of GPIO_1 as Output
 * @param    none
 * @return   none  
 */
#define GPIO_1_SetDigitalOutput() (_TRISC10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC11 GPIO Pin which has a custom name of GPIO_2 to High
 * @pre      The RC11 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define GPIO_2_SetHigh()          (_LATC11 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC11 GPIO Pin which has a custom name of GPIO_2 to Low
 * @pre      The RC11 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_2_SetLow()           (_LATC11 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC11 GPIO Pin which has a custom name of GPIO_2
 * @pre      The RC11 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_2_Toggle()           (_LATC11 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC11 GPIO Pin which has a custom name of GPIO_2
 * @param    none
 * @return   none  
 */
#define GPIO_2_GetValue()         _RC11

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC11 GPIO Pin which has a custom name of GPIO_2 as Input
 * @param    none
 * @return   none  
 */
#define GPIO_2_SetDigitalInput()  (_TRISC11 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC11 GPIO Pin which has a custom name of GPIO_2 as Output
 * @param    none
 * @return   none  
 */
#define GPIO_2_SetDigitalOutput() (_TRISC11 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC14 GPIO Pin which has a custom name of DPD_TP31 to High
 * @pre      The RC14 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define DPD_TP31_SetHigh()          (_LATC14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC14 GPIO Pin which has a custom name of DPD_TP31 to Low
 * @pre      The RC14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define DPD_TP31_SetLow()           (_LATC14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC14 GPIO Pin which has a custom name of DPD_TP31
 * @pre      The RC14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define DPD_TP31_Toggle()           (_LATC14 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC14 GPIO Pin which has a custom name of DPD_TP31
 * @param    none
 * @return   none  
 */
#define DPD_TP31_GetValue()         _RC14

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC14 GPIO Pin which has a custom name of DPD_TP31 as Input
 * @param    none
 * @return   none  
 */
#define DPD_TP31_SetDigitalInput()  (_TRISC14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC14 GPIO Pin which has a custom name of DPD_TP31 as Output
 * @param    none
 * @return   none  
 */
#define DPD_TP31_SetDigitalOutput() (_TRISC14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD2 GPIO Pin which has a custom name of IO_RD2 to High
 * @pre      The RD2 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RD2_SetHigh()          (_LATD2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD2 GPIO Pin which has a custom name of IO_RD2 to Low
 * @pre      The RD2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RD2_SetLow()           (_LATD2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD2 GPIO Pin which has a custom name of IO_RD2
 * @pre      The RD2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RD2_Toggle()           (_LATD2 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD2 GPIO Pin which has a custom name of IO_RD2
 * @param    none
 * @return   none  
 */
#define IO_RD2_GetValue()         _RD2

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD2 GPIO Pin which has a custom name of IO_RD2 as Input
 * @param    none
 * @return   none  
 */
#define IO_RD2_SetDigitalInput()  (_TRISD2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD2 GPIO Pin which has a custom name of IO_RD2 as Output
 * @param    none
 * @return   none  
 */
#define IO_RD2_SetDigitalOutput() (_TRISD2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD7 GPIO Pin which has a custom name of LED_GREEN to High
 * @pre      The RD7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetHigh()          (_LATD7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD7 GPIO Pin which has a custom name of LED_GREEN to Low
 * @pre      The RD7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetLow()           (_LATD7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD7 GPIO Pin which has a custom name of LED_GREEN
 * @pre      The RD7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_GREEN_Toggle()           (_LATD7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD7 GPIO Pin which has a custom name of LED_GREEN
 * @param    none
 * @return   none  
 */
#define LED_GREEN_GetValue()         _RD7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD7 GPIO Pin which has a custom name of LED_GREEN as Input
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetDigitalInput()  (_TRISD7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD7 GPIO Pin which has a custom name of LED_GREEN as Output
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetDigitalOutput() (_TRISD7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD8 GPIO Pin which has a custom name of LED_RED to High
 * @pre      The RD8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_RED_SetHigh()          (_LATD8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD8 GPIO Pin which has a custom name of LED_RED to Low
 * @pre      The RD8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_RED_SetLow()           (_LATD8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD8 GPIO Pin which has a custom name of LED_RED
 * @pre      The RD8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_RED_Toggle()           (_LATD8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD8 GPIO Pin which has a custom name of LED_RED
 * @param    none
 * @return   none  
 */
#define LED_RED_GetValue()         _RD8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD8 GPIO Pin which has a custom name of LED_RED as Input
 * @param    none
 * @return   none  
 */
#define LED_RED_SetDigitalInput()  (_TRISD8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD8 GPIO Pin which has a custom name of LED_RED as Output
 * @param    none
 * @return   none  
 */
#define LED_RED_SetDigitalOutput() (_TRISD8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of FAULT to High
 * @pre      The RD9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define FAULT_SetHigh()          (_LATD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of FAULT to Low
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define FAULT_SetLow()           (_LATD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD9 GPIO Pin which has a custom name of FAULT
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define FAULT_Toggle()           (_LATD9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD9 GPIO Pin which has a custom name of FAULT
 * @param    none
 * @return   none  
 */
#define FAULT_GetValue()         _RD9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of FAULT as Input
 * @param    none
 * @return   none  
 */
#define FAULT_SetDigitalInput()  (_TRISD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of FAULT as Output
 * @param    none
 * @return   none  
 */
#define FAULT_SetDigitalOutput() (_TRISD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD12 GPIO Pin which has a custom name of TP_PIM to High
 * @pre      The RD12 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP_PIM_SetHigh()          (_LATD12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD12 GPIO Pin which has a custom name of TP_PIM to Low
 * @pre      The RD12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP_PIM_SetLow()           (_LATD12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD12 GPIO Pin which has a custom name of TP_PIM
 * @pre      The RD12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP_PIM_Toggle()           (_LATD12 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD12 GPIO Pin which has a custom name of TP_PIM
 * @param    none
 * @return   none  
 */
#define TP_PIM_GetValue()         _RD12

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD12 GPIO Pin which has a custom name of TP_PIM as Input
 * @param    none
 * @return   none  
 */
#define TP_PIM_SetDigitalInput()  (_TRISD12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD12 GPIO Pin which has a custom name of TP_PIM as Output
 * @param    none
 * @return   none  
 */
#define TP_PIM_SetDigitalOutput() (_TRISD12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD15 GPIO Pin which has a custom name of LED_DP_PIM to High
 * @pre      The RD15 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetHigh()          (_LATD15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD15 GPIO Pin which has a custom name of LED_DP_PIM to Low
 * @pre      The RD15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetLow()           (_LATD15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD15 GPIO Pin which has a custom name of LED_DP_PIM
 * @pre      The RD15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_Toggle()           (_LATD15 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD15 GPIO Pin which has a custom name of LED_DP_PIM
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_GetValue()         _RD15

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD15 GPIO Pin which has a custom name of LED_DP_PIM as Input
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetDigitalInput()  (_TRISD15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD15 GPIO Pin which has a custom name of LED_DP_PIM as Output
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetDigitalOutput() (_TRISD15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
