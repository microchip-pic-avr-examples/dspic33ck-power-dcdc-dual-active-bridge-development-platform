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

#define ADC_RESOLUTION          4095 ///< ADC number of ticks for 12-bit ADC resolution
#define ADC_REFERENCE           (float)3.3



#define PWM_CLOCK_PERIOD    (float)(1.0 / PWM_CLOCK_FREQUENCY) ///< PWM Clock Period in [sec]

#define MAX_SWITCHING_PERIOD   (float)(1.0/MINIMUM_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define MIN_PWM_PERIOD         (uint16_t)(float)(MAX_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter

#define MIN_SWITCHING_PERIOD   (float)(1.0/MAXIMUM_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define MAX_PWM_PERIOD         (uint16_t)(float)(MIN_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter

#define MIN_CONTROL_PHASE      (uint16_t)(ADC_RESOLUTION * MINIMUM_CONTROL_PHASE / ADC_REFERENCE) ///< This sets the minimum PWM phase of the converter
#define MAX_CONTROL_PHASE      (uint16_t)(ADC_RESOLUTION * MAXIMUM_CONTROL_PHASE / ADC_REFERENCE) ///< This sets the minimum PWM phase of the converter

#endif	/* MACROS_H */

