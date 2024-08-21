/**
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

#include "xc.h"
#include "dev_led.h"
#include "system/pins.h"

/*********************************************************************************
 * @ingroup dev-led-enums-private
 * @{
 * @brief enumerating away privately
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
uint8_t ledstates[NUM_OF_LEDS];     ///< array that stores the led states 
uint8_t ledtimer[NUM_OF_LEDS];      ///< array that stores led  blink times 


uint8_t LEDIterationsCounter[NUM_OF_LEDS];
/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_Init
 * @param  void
 * @return nothing
 * @brief this function contains the initialization
 * @details
 *   This function initalizes the LED to normal blink interval
 * @note
 *   not really needed to be called at startup, but its nice be consistent
 **********************************************************************************/
void Dev_LED_Initialize(void)
{
    //Peripherals are initialized by MCC
    Dev_LED_Blink(LED_PIM_RED);
    Dev_LED_Blink(LED_BOARD_GREEN);
    Dev_LED_Blink(LED_BOARD_RED);
}

/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id 
 * @return nothing
 * @brief Switch on the LED with the ID defined in the header file
 * @details
 *   This function turns on the led
 **********************************************************************************/
void Dev_LED_On(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledstates[led_id] == LED_STATE_ON)
        return;
    ledstates[led_id] = LED_STATE_ON;
    switch(led_id)
    {
        case LED_PIM_RED:           LED_DP_PIM_on(); break;
        case LED_BOARD_RED:         LED_Board_Red_On(); break;
        case LED_BOARD_GREEN:       LED_Board_Green_On(); break;
        default: Nop(); break;
    }
}


/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id
 * @return nothing
 * @brief Switch off the LED with the ID defined in the header file
 * @details
 *   This function turns off the led
 **********************************************************************************/
void Dev_LED_Off(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledstates[led_id] == LED_STATE_OFF)
        return;
    ledstates[led_id] = LED_STATE_OFF;
    switch(led_id)
    {
        case LED_PIM_RED:           LED_DP_PIM_on(); break;
        case LED_BOARD_RED:         LED_Board_Red_Off(); break;
        case LED_BOARD_GREEN:       LED_Board_Green_Off(); break;
        default: Nop(); break;
    }
}


/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id
 * @return nothing
 * @brief toggle the LED state with the ID defined in the header file
 * @details
 *   This function toggles the led 
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
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id
 * @return nothing
 * @brief sets the LED with the ID defined in the header file in normal blinking mode
 * @details
 *   This function blinks in normal blink as defined in header
 **********************************************************************************/
void Dev_LED_Blink(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK;
}

/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id
 * @return nothing
 * @brief sets the LED with the ID defined in the header file in normal blinking mode
 * @details
 *   This function blinks in normal blink as defined in header
 **********************************************************************************/
void Dev_LED_Blink_Iter(uint8_t led_id, uint8_t count)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_COUNT;
    LEDIterationsCounter[led_id] = count;
}


/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id
 * @return nothing
 * @brief sets the LED with the ID defined in the header file in slow blinking mode
 * @details
 *   This function blinks in slow blink as defined in header
 **********************************************************************************/
void Dev_LED_Blink_Slow(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_SLOW;
}


/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  led_id
 * @return nothing
 * @brief sets the LED with the ID defined in the header file in fast blinking mode
 * @details
 *   This function blinks in slow blink as defined in header
 **********************************************************************************/
void Dev_LED_Blink_Fast(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_FAST;
}


/***********************************************************************************
 * @ingroup dev-led-public-functions
 * @fn     Dev_LED_On
 * @param  void
 * @return nothing
 * @brief this function needs to be called every 100ms
 *        it contains the code to update the status of the LEDs
 * @details
 *   this function needs to be called every 100ms. This updates the status of the LED. 
 *   The intervals defined in the header file are used to determine the frequency of blinky
 **********************************************************************************/
void Dev_LED_Task_100ms()
{
    uint8_t led_index=0;
    uint8_t timermax=0; 

    for (led_index = 0; led_index < NUM_OF_LEDS; led_index++)
    {
        if (ledstates[led_index] > LED_STATE_ON)
        {
            if (ledstates[led_index] == LED_STATE_BLINK_SLOW)
                timermax = BLINK_SLOW_INTERVAL;
            else if (ledstates[led_index] == LED_STATE_BLINK_FAST)
                timermax = BLINK_FAST_INTERVAL;
            else    // normal speed
                timermax = BLINK_INTERVAL;
            if (++ledtimer[led_index] >= timermax)
            {
                ledtimer[led_index] = 0;
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

/*******************************************************************************
 * end of file
 *******************************************************************************/