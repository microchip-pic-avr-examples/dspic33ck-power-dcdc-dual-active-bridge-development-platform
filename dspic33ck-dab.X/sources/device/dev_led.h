/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/**
 * @file    dev_led.h
 * @brief   led device driver
 * @author  M70027
 * @date    05/02/2024
 */
 
/**
 * @file      dev_led.h
 * @ingroup   dev-led  
 * @brief     Contains LED public functions.
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEV_LED_H
#define	DEV_LED_H

#include <xc.h>     // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types

/*********************************************************************************
 * @ingroup dev-led-public-macros
 * @{
 * @def     NUM_OF_LEDs
 * @brief   This defines the number of leds in the system. 
 * @details
 * This sets the number of LEDs in the system. note, that for each led three associated 
 * functions must be decalred that is on, off and toggle for the driver to work.
 * **********************************************************************************/
#define NUM_OF_LEDS     3

/** @} */ //

/*********************************************************************************
 * @ingroup dev-led-porting-macros
 * @{
 * @brief   abstracting away led mcc function calls.
 **********************************************************************************/
//module API                mapped to   MCC custom labels
#define LED_Board_Red_On                LED_RED_SetHigh
#define LED_Board_Red_Off               LED_RED_SetLow
#define LED_Board_Red_Toggle_State      LED_RED_Toggle

#define LED_Board_Green_On              LED_GREEN_SetHigh
#define LED_Board_Green_Off             LED_GREEN_SetLow
#define LED_Board_Green_Toggle_State    LED_GREEN_Toggle

#define LED_DP_PIM_on                   LED_DP_PIM_SetHigh
#define LED_DP_PIM_off                  LED_DP_PIM_SetLow
#define LED_DP_PIM_Toggle_State         LED_DP_PIM_Toggle

/** @} */ // end of dev-led-porting-macros

/*********************************************************************************
 * @ingroup dev-led-enums-public
 * @{
 * @brief enumerating away 
 **********************************************************************************/
#define LED_PIM_RED                 0  //labeling
#define LED_BOARD_RED               1
#define LED_BOARD_GREEN             2


#define BLINK_100ms_INTERVAL       1 //blink is ticked from 100ms Task. Adjust if faster Tick rate is used
#define BLINK_SLOW_INTERVAL        (BLINK_100ms_INTERVAL*20) //( *100ms)    2 sec
#define BLINK_FAST_INTERVAL        (BLINK_100ms_INTERVAL*2)  //             0.5
#define BLINK_INTERVAL             (BLINK_100ms_INTERVAL*5)  //             0.5                      

/** @} */ // end of dev-led-enums-public


/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/

void Dev_LED_Initialize(void);
void Dev_LED_On(uint8_t led_id);
void Dev_LED_Off(uint8_t led_id);
void Dev_LED_Toggle(uint8_t led_id);
void Dev_LED_Blink(uint8_t led_id);
void Dev_LED_Blink_Slow(uint8_t led_id);
void Dev_LED_Blink_Fast(uint8_t led_id);
//void Dev_LED_Blink_Iter(uint8_t led_id, uint8_t count);
void Dev_LED_Task_100ms();


#endif	/* DEV_LED_H */

