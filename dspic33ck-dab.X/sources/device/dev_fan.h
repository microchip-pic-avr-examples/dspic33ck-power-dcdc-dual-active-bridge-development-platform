
/**
 * @file    dev_fan.h
 * @brief   fan device driver
 * @author  M70027
 * @date    05/02/2024
 */

/**
 * @file      dev_fan.h
 * @ingroup   dev-fan  
 * @brief     Contains fan public functions.
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

/** @} */ // end of dev-fan-sccp-abstraction-macros

/*********************************************************************************
 * @ingroup dev-fan-speed-limit-macros
 * @{
 * @brief   macros defining the speed limits
 **********************************************************************************/
#define MAX_SPEED_PERCENT       96  ///< upper limit to fan speed 
#define INIT_SPEED_PERCENT      1  ///<  intial start fan value 
#define MIN_TICK                1   ///< fastest possible tick. needed for override flag. 
#define STEP_SIZE               1   ///< in percent. needed for overide flag.

/** @} */ // end of dev-fan-speed-limit-macros

/***********************************************************************************
 * @ingroup dev-fan-data-type
 * @brief   A custom data type for fan
 **********************************************************************************/
struct FAN_DATA_s {
    uint8_t     CurrentSpeedPercent;     ///< Current Speed in percentage
    uint8_t     TargetSpeedPercent;      ///< targetted Speed in percentage
    uint16_t    MaxSpeedPercent;         ///< based on period value (CCXPRL). Use this to limit the fan max speed
    uint16_t    Tick;                    ///< set the execution rate in multiples of 100ms
    uint16_t    StepSizePercent;         ///< set the increment rate per tick in percentage
    uint16_t    TargetSpeedRaw;          ///< targetted value raw.
    uint16_t    CurrentSpeedRaw;         ///< current value raw.
    uint16_t    MaxSpeedRaw;             ///< based on period value (CCXPRL).
    uint8_t     OverrideFlag;             ///< override flag. sets tick value to 1, step_size to 20 and target to 100%
};
typedef struct FAN_DATA_s FAN_DATA_t;



/*********************************************************************************
 * @ingroup dev-fan
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
void Dev_Fan_Initialize(void);
void Dev_Fan_Task_100ms(void);
void Dev_Fan_Task_1s(void); 
void Dev_Fan_Set_Override(void);           
void Dev_Fan_Set_Speed(uint8_t TargetSpeedPercent);


#endif	/* DEV_FAN_H */

