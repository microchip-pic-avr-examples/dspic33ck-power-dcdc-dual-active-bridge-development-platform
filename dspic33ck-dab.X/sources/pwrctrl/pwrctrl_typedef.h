/**
 * @file pwrctrl_typedefs.h     
 * @ingroup pwrctrl-data-types   
 * @brief Contains the power control data structure.    
 */

#ifndef DEV_PWRCTRL_TYPEDEF_H
#define	DEV_PWRCTRL_TYPEDEF_H

#include "fault/fault_typedef.h"

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Enumeration of power controller state machine states
 * @details
 * These are all of the states for the power controller state machine
 ***********************************************************************************/
enum  PWR_CTRL_STATES_e
{
  PWRCTRL_STATE_INITIALIZE,           ///< power converter control state #0:  initializes object properties and clears status bits
  PWRCTRL_STATE_FAULT_DETECTION,      ///< power converter control state #1:  checks if a fault occurs
  PWRCTRL_STATE_STANDBY,              ///< power converter control state #2:  waiting for status bits allowing power supply to run
  PWRCTRL_STATE_SOFT_START,           ///< power converter control state #3: performs soft-start sequence 
  PWRCTRL_STATE_ONLINE                ///< power converter control state #4: output is in regulation and power is OK (normal continuous operation)
};
typedef enum PWR_CTRL_STATES_e PWR_CTRL_STATE_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Power converter switch-node specifications
 * @details
 *   This data structure is used to set the converter switch-node specifications declaring which
 *  PWM channel is used as well as its switching period, duty cycle and phase-shift.
 ***********************************************************************************/
struct SWITCH_NODE_s
{
    uint16_t ControlPeriod;         ///< Control period value from control loop
    uint16_t ControlDutyCycle;      ///< Control Duty Cycle calculation based on Control Period
    uint16_t ControlPhase;          ///< Control phase value from control loop
    uint16_t ControlPhase_P2S_Degreex10; ///< Control phase value from control loop in 10 x degree, integer
    uint16_t ControlPhase_P2S_Target;     ///< Control phase value from control loop in degrees
    uint16_t DeadTimeHigh;          ///< Deadtime High settings for PWM
    uint16_t DeadTimeLow;          ///< Deadtime High settings for PWM
    uint16_t PBVPeriodTarget;       ///< Power Board Visualizer Set Control target 
    uint16_t PBVControlPhaseTarget; ///< Power Board Visualizer Set Control phase
    uint16_t LowPowerSlowMode;       ///< future development
};
typedef struct SWITCH_NODE_s SWITCH_NODE_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Publicly accessible data buffer of most recent runtime data values
 * @details
 *   This data structure is used to store the feedback values of the most 
 * recent runtime data values.
 ***********************************************************************************/
struct FEEDBACK_SETTINGS_s
{
    uint16_t VPriVoltage;       ///< Data value for input voltage 
    uint16_t VSecVoltage;       ///< Data value for output voltage
    uint16_t ISensePrimary;     ///< Data value for primary current as measured with CT
    uint16_t ISenseSecondary;   ///< Data value for secondary current as measured with CT
    uint16_t ISecAverage;       ///< Data value for average secondary current as measured with isolated current sensor
    uint16_t ISecAverageRectified;///< Data value for average secondary current rectified
    int16_t  SecPower;          ///< Data value for Secondary power (Watt)
    uint16_t Temperature;       ///< Data value for temperature
    uint16_t VRail_5V;          ///< Data value for 5V auxiliary rail
    uint16_t ISecSensorOffset;  ///< Offset of the secondary current sensor
};
typedef struct FEEDBACK_SETTINGS_s FEEDBACK_SETTINGS_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief Power converter status flags
 * @details
 *      This data structure is used to indicate when the there is a fault or
 * running power supply.
 **********************************************************************************/
struct STATUS_FLAGS_s {
    union {
        struct {      
            unsigned Running         : 1; ///< Bit 0: Power converter is running
            unsigned FaultActive     : 1; ///< Bit 1: Power converter fault is active
            unsigned notUsed         : 13;///< Bit 2 - 15: Not used 
        } __attribute__((packed)) bits; 
        uint16_t value;
    };
};
typedef struct STATUS_FLAGS_s STATUS_FLAGS_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief Stores data related to the ramping up/down of the reference
 * @details This data structure manages the ramping up/down of the reference depending 
 *  on the target. "reference" is the actual reference currently being used while
 *  "targetReference" can be different that reference during soft-start.
 **********************************************************************************/
struct START_UP_RAMP_s 
{
    uint16_t Counter;          ///< Soft-Start Execution Counter. This setting is set/cleared by the device driver and is 'read only'.
    uint16_t Delay;           ///< Soft-Start Period (POD, RAMP PERIOD, PGD, etc.)
    uint16_t StepSize;         ///< Size/value of one reference increment/decrement or this period
    uint16_t* ptrReference;     ///< pointer the reference variable
    uint16_t* ptrReferenceTarget;///< pointer to the ramp-up reference target
    bool RampComplete;          ///< indicates when ramp-up is complete
};
typedef struct START_UP_RAMP_s START_UP_RAMP_t; 


/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief Stores data related to the control loop properties
 * @details This data structure manages the control loop properties. The 
 * "output" is the output of the controller, "reference" is the actual reference 
 * currently being used, and "targetReference" is the desirable reference that
 * the control loop should achieved.
 **********************************************************************************/
struct CONTROLLER_s
{
  int16_t Reference;        ///< actual reference
  uint16_t Feedback;        ///< coming 
  uint16_t Output;          ///< controller output
  int16_t AgcFactor;        ///< Adaptive gain control
  bool Enable;              ///< Enable control loop
};
typedef struct CONTROLLER_s CONTROLLER_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Enumeration of power controller charging state
 * @details
 * These is the enumeration of the DAB's charging state
 ***********************************************************************************/
enum  PWR_CTRL_CHARGE_STATE_e
{
    PWR_CTRL_STOP,
    PWR_CTRL_CHARGING,      ///< power converter is in charging mode
    PWR_CTRL_DISCHARGING    ///< power converter is in discharging mode
};
typedef enum PWR_CTRL_CHARGE_STATE_e PWR_CTRL_CHARGE_STATE_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Collection of power control properties
 * @details This data structure can be use to initialize the desired output voltage,
 *  current and power. THe bits like charging state and if the power supply 
 *  is enabled can be monitored in this structure. 
 ***********************************************************************************/
struct PWR_CTRL_PROPERTIES_s 
{
    uint16_t VSecReference;    ///< User secondary-voltage port reference setting used to control the power converter output voltage in buck mode
    uint16_t VPriReference;    ///< User primary-voltage port reference setting used to control the power converter output voltage in boost mode
    int16_t  IReference;       ///< User current reference setting used to control the converter controller
    uint16_t PwrReference;     ///< User power reference setting used to control the  converter controller
    
    //bool ChargingState;        ///< Control Flag: When set, enables PWM output while keeping control loop disabled; Skips closed loop ramp up states and directly jumps to state Online 
    bool Enable;               ///< Control Flag: When set, enables the converter triggering a startup sequence; When cleared, forces teh power supply to reset and turn off

};  
typedef struct PWR_CTRL_PROPERTIES_s PWR_CTRL_PROPERTIES_t;  

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Power control API structure
 * @details This data structure contains all the properties of the power control.  
 ***********************************************************************************/
struct POWER_CONTROL_s
{
    STATUS_FLAGS_t      Status; ///< Power Supply status flags, running or fault
    PWR_CTRL_STATE_t    State;  ///< Power Control SM State ID
    PWR_CTRL_PROPERTIES_t Properties;    ///< Power Control properties    
    SWITCH_NODE_t       Pwm;    ///< Switch node settings
    FEEDBACK_SETTINGS_t Data;   ///< Feedback channel settings
    FAULT_SETTINGS_t    Fault;  ///< Fault flags and settings 
    START_UP_RAMP_t     VRamp;  ///< Voltage ramp-up settings
    START_UP_RAMP_t     IRamp;  ///< Current ramp-up settings
    START_UP_RAMP_t     PRamp;  ///< Power ramp-up settings
    CONTROLLER_t        ILoop;  ///< structure for current controller data
    CONTROLLER_t        VLoop;  ///< structure for voltage controller data
    CONTROLLER_t        PLoop;  ///< structure for power controller data
    PWR_CTRL_CHARGE_STATE_t    PowerDirection;  ///< defines if the power converter is in charging or discharging mode   
};
typedef struct POWER_CONTROL_s POWER_CONTROL_t;

#endif	/* DEV_PWRCTRL_TYPEDEF_H */

