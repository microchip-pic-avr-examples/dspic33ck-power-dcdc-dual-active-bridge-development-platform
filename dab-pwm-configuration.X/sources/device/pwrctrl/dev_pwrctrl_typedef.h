/* 
 * File:   drv_pwrctrl_typedef.h
 * Author: M15690
 *
 * Created on July 8, 2024, 4:58 PM
 */

#ifndef DRV_PWRCTRL_TYPEDEF_H
#define	DRV_PWRCTRL_TYPEDEF_H

#include "device/fault/dev_fault_common.h"

/***********************************************************************************
 * @ingroup 
 * @enum    PWR_CTRL_STATE_e
 * @brief   power controller state machine states
 * @details
 * These are all of the states for the power controller state machine
 ***********************************************************************************/
enum  PWR_CTRL_STATE_e
{
  PCS_INIT                  = 0,    
  PCS_WAIT_IF_FAULT_ACTIVE  = 1,    
  PCS_STANDBY               = 2,     
  PCS_SOFT_START            = 3, 
  PCS_UP_AND_RUNNING        = 4 
};

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
    uint16_t PBVPeriodTarget;       ///< Power Board Visualizer Set Control target 
    uint16_t PBVControlPhase;       ///< Power Board Visualizer Set Control phase
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
    uint16_t VInputVoltage; ///< Feedback Data for input voltage 
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

/***********************************************************************************
 * @ingroup 
 * @struct STATUS_FLAGS_s
 * @extends 
 * @brief cllc status flags
 * @details
 *  
 **********************************************************************************/
struct STATUS_FLAGS_s
{
    union
    {
        struct
        {      
            unsigned running         : 1; ///> Bit 0: CLLC is running
            unsigned fault           : 1; ///> Bit 1: fault present
            unsigned notUsed         : 13; 
        } __attribute__((packed)) bits; 
        uint16_t value;
    };
};
typedef struct STATUS_FLAGS_s STATUS_FLAGS_t;

/***********************************************************************************
 * @ingroup 
 * @struct FAULT_FLAGS_s
 * @extends 
 * @brief Fault flags for cllc
 * @details
 *  
 **********************************************************************************/
struct FAULT_FLAGS_s
{
    union
    {
        struct
        {
            unsigned vPri_ov      : 1;  ///> bit  0
            unsigned vPri_uv      : 1;  ///> bit  1
            unsigned vSec_ov      : 1;  ///> bit  2
            unsigned vSec_uv      : 1;  ///> bit  3
            unsigned iPri_oc      : 1;  ///> bit  4
            unsigned iSec_oc      : 1;  ///> bit  5 
            unsigned i_sc         : 1;  ///> bit  6, primary or secondary short circuit fault
            unsigned notUsed      : 9;  ///> bits 7 to 15
        } __attribute__((packed)) bits; 
        uint16_t value;
    };
};
typedef struct FAULT_FLAGS_s FAULT_FLAGS_t;

/***********************************************************************************
 * @ingroup 
 * @struct FAULT_s
 * @extends 
 * @brief stores all data related to the fault objects
 * @details
 *  
 **********************************************************************************/
struct FAULT_s
{
    FAULT_OBJ_T ipri_oc; 
    FAULT_OBJ_T isec_oc;
    FAULT_OBJ_T vpri_ov;
    FAULT_OBJ_T vsec_ov;
    FAULT_OBJ_T i_sc; // primary or secondary short circuit
};
typedef struct FAULT_s FAULT_t;

struct FAULT_SETTINGS_s
{
    FAULT_t Object;                     ///< Fault Objects
    FAULT_FLAGS_t Flags;                ///< Fault Flags
    FAULT_FLAGS_t FaultFlagsLatched;    ///< Latch faults in here so can be read by PBV GUI
};
typedef struct FAULT_SETTINGS_s FAULT_SETTINGS_t;

/***********************************************************************************
 * @ingroup 
 * @struct CONTROLLER_s
 * @extends 
 * @brief stores data related to controller
 * @details
 * "output" is the output of the controller
 * "reference" is the actual reference currently being used
 * "targetReference" can be different that reference during soft-start etc.
 **********************************************************************************/
struct CONTROLLER_s
{
  int16_t reference;        ///< actual reference
  int16_t referenceTarget;  ///< target reference, can be changed via GUI
  uint16_t feedback;        ///< coming 
  uint16_t output;          ///< controller output
};
typedef struct CONTROLLER_s CONTROLLER_t;

struct POWER_CONTROL_s
{
    STATUS_FLAGS_t      Status; ///< Power Supply status flags
    PWR_CTRL_STATE_t    State;  ///< Power Control State ID
    SWITCH_NODE_t       Pwm;    ///< Switch node settings
    FEEDBACK_SETTINGS_t Adc;    ///< ADC feedback channel settings
    FAULT_SETTINGS_t    Fault;  ///< Fault flags and settings 
    CONTROLLER_t        iloop;  ///< structure for current controller data
    CONTROLLER_t        vloop;  ///< structure for voltage controller data
    bool enable;                ///< control flag, set to 1 to run the power supply
};
typedef struct POWER_CONTROL_s POWER_CONTROL_t;

#endif	/* DRV_PWRCTRL_TYPEDEF_H */

