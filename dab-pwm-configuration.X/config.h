/* 
 * File:   config.h
 * Author: M15690
 *
 * Created on July 2, 2024, 10:40 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#define PWM_CLOCK_HIGH_RESOLUTION true 
#define MAXIMUM_SWITCHING_FREQUENCY     (float)230.0e+3 ///< Maximum Switching frequency in [Hz]
#define MINIMUM_SWITCHING_FREQUENCY     (float) 65.0e+3 ///< Minimum Switching frequency in [Hz]

#define MINIMUM_CONTROL_PHASE 0 ///< Minimum control phase in degrees phase
#define MAXIMUM_CONTROL_PHASE 180 ///< Maximum control phase in degrees phase

#endif	/* CONFIG_H */

