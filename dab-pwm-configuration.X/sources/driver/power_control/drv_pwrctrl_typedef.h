/* 
 * File:   drv_pwrctrl_typedef.h
 * Author: M15690
 *
 * Created on July 8, 2024, 4:58 PM
 */

#ifndef DRV_PWRCTRL_TYPEDEF_H
#define	DRV_PWRCTRL_TYPEDEF_H

#include "config/config.h"

/***********************************************************************************
 * @ingroup 
 * @enum    PWR_CTRL_STATE_e
 * @brief   power controller state machine states
 * @details
 * These are all of the states for the power controller state machine
 ***********************************************************************************/
typedef enum
{
  PCS_INIT                  = 0,    
  PCS_WAIT_IF_FAULT_ACTIVE  = 1,    
  PCS_STANDBY               = 2,     
  PCS_SOFT_START            = 3, 
  PCS_UP_AND_RUNNING        = 4,  
} PWR_CTRL_STATE_e;
typedef enum PWR_CTRL_STATE_e PWR_CTRL_STATE_t;

struct SWITCH_NODE_s
{
    uint16_t Primary_1;             ///< Primary switch-node configuration
    uint16_t Primary_2;             ///< Primary switch-node configuration
    uint16_t Secondary_1;           ///< Secondary switch-node configuration
    uint16_t Secondary_2;           ///< Secondary switch-node configuration
    uint16_t ControlPeriod;         ///< Control period value from control loop
    uint16_t ControlDutyCycle;      ///< Control Duty Cycle calculation based on Control Period
    uint16_t ControlPhase;          ///< Control phase value from control loop
};
typedef struct SWITCH_NODE_s SWITCH_NODE_t;

/***********************************************************************************
 * @ingroup 
 * @struct ADC_s
 * @extends 
 * @brief stores all data related to the ADC readings
 * @details
 *  
 **********************************************************************************/
struct FEEDBACK_SETTINGS_s
{
    uint16_t vsec; ///< secondary side voltage
    uint16_t ipri_ct; ///< primary current as measured with CT
    uint16_t vpri; ///< primary side voltage
    uint16_t temperature; ///< temperature
    uint16_t vrail_5v; ///< 5V aux rail
    uint16_t isec_ct; ///< secondary current as measured with CT
    uint16_t isec_avg; ///< average secondary current as measured with isolated current sensor
    
    uint16_t isec_sensor_offset; ///< ADC code corresponding to the offset of the secondary current sensor
};

typedef struct FEEDBACK_SETTINGS_s FEEDBACK_SETTINGS_t;

struct POWER_CONTROL_s
{
    PWR_CTRL_STATE_t    State;  ///< Power Control State ID
    SWITCH_NODE_t       Pwm;    ///< LLC converter switch node settings
    FEEDBACK_SETTINGS_t Adc;    ///< ADC feedback channel settings
};
typedef struct POWER_CONTROL_s POWER_CONTROL_t;

#endif	/* DRV_PWRCTRL_TYPEDEF_H */

