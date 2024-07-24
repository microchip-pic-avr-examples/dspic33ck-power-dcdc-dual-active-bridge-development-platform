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


#define DPDB_TEST_RUN   false    ///< test the code with Digital Power Development Board
#define OPEN_LOOP_PBV   true    ///< use Power Board visualizer to change the parameters
//------------------------------------------------------------------------------
// fault disables (for debug only)
//------------------------------------------------------------------------------
#define FAULT_ISEC_OC   true
#define FAULT_IPRI_OC   false
#define FAULT_VSEC_OV   false
#define FAULT_VPRI_OV   false
#define FAULT_SHORT_CCT false


#define PWM_CLOCK_HIGH_RESOLUTION       true 
#define MAXIMUM_SWITCHING_FREQUENCY     (float)230.0e+3 ///< Maximum Switching frequency in [Hz]
#define MINIMUM_SWITCHING_FREQUENCY     (float) 65.0e+3 ///< Minimum Switching frequency in [Hz]

#define MAXIMUM_CONTROL_PHASE           180 ///< Maximum control phase in degrees phase


// T_ADC_SAMPLE_SECS: smallest period between ADC samples
#define T_ADC_SAMPLE_SECS       (16.5e-6) // ADC sampling rate

// temperature sample taken every T_ADC_TEMP_SAMPLE_DIVIDER samples 
#define T_ADC_TEMP_SAMPLE_DIVIDER       (1000)

// 5V rail sample taken every T_ADC_5V_SAMPLE_DIVIDER samples
#define T_ADC_5V_SAMPLE_DIVIDER         (100)

//------------------------------------------------------------------------------
// fault related parameters
//------------------------------------------------------------------------------
// #defines for fault thresholds in human readable form (SI or derived SI units)

// all fault parameters follow the same convention
// for example, for ISEC_CT over current protection
// if fault clear: 
// if ISEC_CT > ISEC_OC_THRES_TRIG_AMPS for longer than ISEC_OC_T_BLANK_TRIG_SEC
//      then set fault
// if fault set:
// if ISEC_CT <= ISEC_OC_THRES_CLEAR_AMPS for longer than ISEC_OC_T_BLANK_CLEAR_SEC
//      then clear fault
// ISEC_OC_TICK_SEC is the rate at which the fault handler is invoked
// it is required for the firmware to measure trigger and clear blanking times
#define ISEC_OC_THRES_TRIG_AMPS             (27.0)
#define ISEC_OC_THRES_CLEAR_AMPS            (2.0)
#define ISEC_OC_T_BLANK_TRIG_SEC            (50.0e-6)
#define ISEC_OC_T_BLANK_CLEAR_SEC           (0.0)
#define ISEC_OC_TICK_SEC                    (T_ADC_SAMPLE_SECS) 

#define IPRI_OC_THRES_TRIG_AMPS             (27.0)
#define IPRI_OC_THRES_CLEAR_AMPS            (2.0)
#define IPRI_OC_T_BLANK_TRIG_SEC            (50.0e-6)
#define IPRI_OC_T_BLANK_CLEAR_SEC           (0.0)
#define IPRI_OC_TICK_SEC                    (T_ADC_SAMPLE_SECS*3.0)

#define VSEC_OV_THRES_TRIG_VOLTS            (490.0)
#define VSEC_OV_THRES_CLEAR_VOLTS           (20.0)
#define VSEC_OV_T_BLANK_TRIG_SEC            (1.0e-3)
#define VSEC_OV_T_BLANK_CLEAR_SEC           (10.0e-3)
#define VSEC_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS*3.0)

#define VSEC_UV_THRES_TRIG_VOLTS            (100.0)
#define VSEC_UV_THRES_CLEAR_VOLTS           (50.0)
#define VSEC_UV_T_BLANK_TRIG_SEC            (0.0)
#define VSEC_UV_T_BLANK_CLEAR_SEC           (0.0)
#define VSEC_UV_TICK_SEC                    (T_ADC_SAMPLE_SECS*3.0)

#define VPRI_OV_THRES_TRIG_VOLTS            (700.0)
#define VPRI_OV_THRES_CLEAR_VOLTS           (600.0)
#define VPRI_OV_T_BLANK_TRIG_SEC            (0.0)
#define VPRI_OV_T_BLANK_CLEAR_SEC           (0.0)
#define VPRI_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS*3.0)

// short-circuit protection. Implemented with comparators and CLC, using PWM fault mechanism
// so works independently of firmware. 
// If fault occurs, PWM stops immediately (done by peripheral)
// after fault has tripped, fault input needs to be low for I_SC_T_BLANK_CLEAR_SEC for fault to be cleared
// both fault inputs come from comparator outputs
// both comparator outputs are routed through CLC to the PWM Fault PCI input
// so we have no way of differentiating between a primary or secondary SC fault
// hence the fault handler and fault blanking time for both faults is combined
// thresholds can be different for both primary and secondary side short-circuit faults, as 
// use different comparators
#define IPRI_SC_THRES_TRIG_AMPS          (35.0)
#define ISEC_SC_THRES_TRIG_AMPS          (35.0)
#define I_SC_T_BLANK_CLEAR_SEC           (2.0)
#define I_SC_TICK_SEC                    (100.0e-6)

// LEB (Leading Edge Blanking) on rising and falling edges of PWM1H, current spike seen due to 
// common mode noise (noise coupled from primary to secondary)
//#define I_SC_LEB_DISABLED       // #define this to disable LEB  

// I_SC_LEB_TIME: time for fault blanking of short circuit faults after rising and falling edges of PWM1H
// Short circuit faults are ignored during this time
#define I_SC_LEB_TIME         (300.0e-9)


//------------------------------------------------------------------------------
// control (for debug only)
//------------------------------------------------------------------------------
// OPEN_LOOP_PBV: define this to run in complete open loop mode using PBV to control frequency
//#define OPEN_LOOP_PBV   

// OPEN_LOOP_POTI: define this to run in complete open loop mode using poti to control frequency
// this is meant for the digital power development board only!!
// connect a poti to edge connector 11 and vary poti to control the frequency
//#define OPEN_LOOP_POTI  

#endif	/* CONFIG_H */

