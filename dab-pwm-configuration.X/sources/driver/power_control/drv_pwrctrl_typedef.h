/* 
 * File:   drv_pwrctrl_typedef.h
 * Author: M15690
 *
 * Created on July 8, 2024, 4:58 PM
 */

#ifndef DRV_PWRCTRL_TYPEDEF_H
#define	DRV_PWRCTRL_TYPEDEF_H

#include "config/config.h"

#ifndef NO_OF_PHASES
#define NO_OF_PHASES 1U
#pragma message "warning: macro 'NO_OF_PHASES' has not been found. Number of phases has been set to default=1"
#endif


struct SWITCH_NODE_SETTINGS_s
{
    uint16_t PwmChannel;    ///< PWM generator instance used
    uint16_t DutyCycle;     ///< PWM Duty Cycle during normal operation
    uint16_t Period;        ///< PWM Period during normal opration
    uint16_t PhaseShift;    ///< PWM Phase Shift during normal operation
};
typedef struct SWITCH_NODE_SETTINGS_s SWITCH_NODE_SETTINGS_t;

struct SWITCH_NODE_s
{
    SWITCH_NODE_SETTINGS_t Primary;        ///< Primary switch-node configuration
    SWITCH_NODE_SETTINGS_t Secondary;      ///< Secondary switch-node configuration
};
typedef struct SWITCH_NODE_s SWITCH_NODE_t;


struct POWER_CONTROL_s
{
    SWITCH_NODE_t SwitchNodes[NO_OF_PHASES]; ///< LLC converter switch node settings
};
typedef struct POWER_CONTROL_s POWER_CONTROL_t;

#endif	/* DRV_PWRCTRL_TYPEDEF_H */

