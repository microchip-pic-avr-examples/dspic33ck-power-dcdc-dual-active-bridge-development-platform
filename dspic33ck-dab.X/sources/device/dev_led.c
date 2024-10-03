/*
 * @ingroup   version-information
 * @file      dev_led.c
 * @details   <b>Description</b><br>This file has all the logic for led blinkys.
 * @author    M70027
 * @author    M70027
 * @version   1.0
 * @date      2024
 * @warning   Improper use can crash your application.
 * @copyright 
 */

/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */


/**
 * @file      dev_led.c
 * @ingroup   dev-led   
 * @brief     Contains LED initialization and execution functions.
 */
#include "xc.h"
#include "dev_led.h"
#include "system/pins.h"

/*********************************************************************************
 * @ingroup dev-led
 * @{
 * @brief These are the macros indicating the status of the LED. 
 **********************************************************************************/

#define LED_STATE_OFF 0
#define LED_STATE_ON 1
#define LED_STATE_BLINK 2
#define LED_STATE_BLINK_SLOW 3
#define LED_STATE_BLINK_FAST 4
#define LED_STATE_BLINK_COUNT 5

/** @} */ // end of dev-led-enums-private

/***********************************************************************************
 * Private Variables
 **********************************************************************************/
uint8_t ledStates[NUM_OF_LEDS];     ///< array that stores the led states 
uint8_t ledTimer[NUM_OF_LEDS];      ///< array that stores led  blink times 


uint8_t ledIterationsCounter[NUM_OF_LEDS];
/***********************************************************************************
 * @ingroup dev-led
 * @brief This function contains the LED initialization
 * @return  void
 * 
 * @details
 *   This function initializes the LED to normal blink interval.
 * @note
 *   Not really needed to be called at startup, but its nice be consistent
 **********************************************************************************/
void Dev_LED_Initialize(void)
{
    //Peripherals are initialized by MCC
    Dev_LED_Blink(LED_PIM_RED);
    Dev_LED_Blink(LED_BOARD_GREEN);
    Dev_LED_Blink(LED_BOARD_RED);
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Switch on the LED 
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function turns on the led with the ID defined in the header file
 **********************************************************************************/
void Dev_LED_On(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledStates[led_id] == LED_STATE_ON)
        return;
    ledStates[led_id] = LED_STATE_ON;
    switch(led_id)
    {
        case LED_PIM_RED:           LED_DP_PIM_on(); break;
        case LED_BOARD_RED:         LED_Board_Red_On(); break;
        case LED_BOARD_GREEN:       LED_Board_Green_On(); break;
        default: Nop(); break;
    }
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Switch off the LED 
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function turns off the led with the ID defined in the header file
 **********************************************************************************/
void Dev_LED_Off(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledStates[led_id] == LED_STATE_OFF)
        return;
    ledStates[led_id] = LED_STATE_OFF;
    switch(led_id)
    {
        case LED_PIM_RED:           LED_DP_PIM_off(); break;
        case LED_BOARD_RED:         LED_Board_Red_Off(); break;
        case LED_BOARD_GREEN:       LED_Board_Green_Off(); break;
        default: Nop(); break;
    }
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Toggles the LED state 
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function toggles the led with the ID defined in the header file
 **********************************************************************************/
void Dev_LED_Toggle(uint8_t led_id)
{
    switch(led_id)
    {
        case LED_PIM_RED:           LED_DP_PIM_Toggle_State(); break;
        case LED_BOARD_RED:         LED_Board_Red_Toggle_State(); break;
        case LED_BOARD_GREEN:       LED_Board_Green_Toggle_State(); break;
        default: Nop(); break;
    }
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Set the LED in blinking mode
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function makes the LED blink.
 **********************************************************************************/
void Dev_LED_Blink(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledStates[led_id] = LED_STATE_BLINK;
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Set the LED in blinking mode in number of LED iterations
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function blinks the LED instance in a particular number of blink counts
 **********************************************************************************/
void Dev_LED_Blink_Iter(uint8_t led_id, uint8_t count)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledStates[led_id] = LED_STATE_BLINK_COUNT;
    ledIterationsCounter[led_id] = count;
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Set the LED in SLOW blinking mode 
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function makes the LED blink in slow mode. 
 **********************************************************************************/
void Dev_LED_Blink_Slow(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledStates[led_id] = LED_STATE_BLINK_SLOW;
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief Set the LED in FAST blinking mode 
 * @param  led_id LED ID instance
 * @return void
 * 
 * @details This function makes the LED blink in fast mode. 
 **********************************************************************************/
void Dev_LED_Blink_Fast(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledStates[led_id] = LED_STATE_BLINK_FAST;
}

/***********************************************************************************
 * @ingroup dev-led
 * @brief This function needs to be called every 100ms and contains the code to 
 *  update the status of the LEDs
 * @return void
 * 
 * @details This function needs to be called every 100ms. This updates the status 
 *  of the LED. The intervals defined in the header file are used to determine 
 *  the frequency of blinky LED.
 **********************************************************************************/
void Dev_LED_Task_100ms()
{
    uint8_t led_index=0;
    uint8_t timermax=0; 

    for (led_index = 0; led_index < NUM_OF_LEDS; led_index++)
    {
        if (ledStates[led_index] > LED_STATE_ON)
        {
            if (ledStates[led_index] == LED_STATE_BLINK_SLOW)
                timermax = BLINK_SLOW_INTERVAL;
            else if (ledStates[led_index] == LED_STATE_BLINK_FAST)
                timermax = BLINK_FAST_INTERVAL;
            else    // normal speed
                timermax = BLINK_INTERVAL;
            if (++ledTimer[led_index] >= timermax)
            {
                ledTimer[led_index] = 0;
                switch(led_index)
                {
                    case LED_PIM_RED:           LED_DP_PIM_Toggle_State(); break;
                    case LED_BOARD_RED:         LED_Board_Red_Toggle_State(); break;
                    case LED_BOARD_GREEN:       LED_Board_Green_Toggle_State(); break;
                    default: Nop(); break;
                }
            }
        }
    }
}
