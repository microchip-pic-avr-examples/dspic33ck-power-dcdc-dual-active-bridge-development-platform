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
 * @file    dev_fan.h
 * @brief   fan device driver
 * @author  M70027
 * @date    05/02/2024
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEV_FAN_H
#define	DEV_FAN_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types

#include    "pwm/sccp3.h" 
/*********************************************************************************
 * @ingroup dev-fan-sccp-abstraction-macros
 * @{
 * @brief   These have to be renamed to make the fan device driver abstract
 **********************************************************************************/

#define     MULTIPLIER      (CCP3PRL/100)       ///< 1 percent equals to max period / 100
#define     MAX_PERIOD      CCP3PRL            ///< maximum duty cycle is max period

/** @} */ // end of dev-fan-sccp-abstraction-macros

/*********************************************************************************
 * @ingroup dev-fan-speed-limit-macros
 * @{
 * @brief   macros defining the speed limits
 **********************************************************************************/
#define MAX_SPEED_PERCENT       90  ///< limit the speed to 90 percent. done on compile time
#define INIT_SPEED_PERCENT      50  ///< start fan at 50
#define MIN_TICK                1   ///< fastest possible tick. needed for override flag. 
#define STEP_SIZE               5   ///< in percent. needed for overide flag.

/** @} */ // end of dev-fan-speed-limit-macros

/***********************************************************************************
 * @ingroup dev-fan-data-type
 * @struct  dev_fan_data_t
 * @brief   a custom data type for fan
 **********************************************************************************/
struct FAN_DATA_s {
    uint8_t     CurrentSpeedPercent;     ///< Current Speed in percentage
    uint8_t     TargetSpeedPercent;      ///< targetted Speed in percentage
    uint16_t    MaxSpeedPercent;         ///< based on period value (CCXPRL). Use this to limit the fan max speed
    uint16_t    Tick;                      ///< set the execution rate in multiples of 100ms
    uint16_t    StepSizePercent;         ///< set the increment rate per tick in percentage
    uint16_t    TargetSpeedRaw;          ///< targetted value raw.
    uint16_t    CurrentSpeedRaw;         ///< current value raw.
    uint16_t    MaxSpeedRaw;             ///< based on period value (CCXPRL).
    uint8_t     OverrideFlag;             ///< override flag. sets tick value to 1, step_size to 20 and target to 100%
};
typedef struct FAN_DATA_s FAN_DATA_t;



/*********************************************************************************
 * @ingroup dev-fan-data-type
 * @var     dev_fan_data_ptr
 * @brief   Fan data object pointer external
 * @details pointer to the Object 'dev_fan_data', providing access to all the fan control
 *          and monitoring values. To access it, add the header file to the project, and 
 *          then the pointer can be used to access the dev_fan_data members.
 **********************************************************************************/
extern FAN_DATA_t* devFanDataPtr;

/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/
void Dev_Fan_Init(void);
void Dev_Fan_Task_100ms(void);

//void Dev_Fan_Set_Speed (uint8_t target_speed_percent);       //TODO: INLINE 
void Dev_Fan_Set_Override();           
void Dev_Fan_Set_Speed(uint8_t TargetSpeedPercent);


#endif	/* DEV_FAN_H */

