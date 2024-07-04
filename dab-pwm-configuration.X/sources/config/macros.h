/* 
 * File:   macros.h
 * Author: M15690
 *
 * Created on July 2, 2024, 10:41 AM
 */

#ifndef MACROS_H
#define	MACROS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "config.h"

//device specific 
#if (PWM_CLOCK_HIGH_RESOLUTION)
#define PWM_CLOCK_FREQUENCY     (float)4.0e+9 ///< PWM Clock Frequency in [Hz]
#else
#define PWM_CLOCK_FREQUENCY     (float)500.0e+6 ///< PWM Clock Frequency in [Hz]
#endif

#define ADC_RESOLUTION          4095U ///< ADC number of ticks for 12-bit ADC resolution
#define ADC_REFERENCE           (float)3.3
#define ADC_SCALER              (float)(1.0 / ADC_RESOLUTION)
#define PHASE_DEGREES           360U

#define PWM_CLOCK_PERIOD       (float)(1.0 / PWM_CLOCK_FREQUENCY) ///< PWM Clock Period in [sec]
#define MAX_SWITCHING_PERIOD   (float)(1.0/MINIMUM_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define MAX_PWM_PERIOD         (uint16_t)(float)(MAX_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter
#define MIN_SWITCHING_PERIOD   (float)(1.0/MAXIMUM_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define MIN_PWM_PERIOD         (uint16_t)(float)(MIN_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter
#define PERIOD_RANGE           (uint16_t)(MAX_PWM_PERIOD - MIN_PWM_PERIOD)  
#define ADC_PERIOD_RANGE       (uint16_t)(float)(PERIOD_RANGE / ADC_RESOLUTION)

#define CONTROL_PHASE_RAD      (uint16_t)(float)(PHASE_DEGREES / MAXIMUM_CONTROL_PHASE) ///< This sets the maximum PWM phase of the converter
#define MIN_PER_CONTROL_PHASE  (uint16_t)(float)(MIN_PWM_PERIOD / CONTROL_PHASE_RAD)
#define MAX_PER_CONTROL_PHASE  (uint16_t)(float)(MAX_PWM_PERIOD / CONTROL_PHASE_RAD)

#endif	/* MACROS_H */

