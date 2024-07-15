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

#include "useful_macros.h"
#include "config.h"
#include "hardware.h"


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
#define PHASE_180_SCALER        (float)(1.0 / 180)

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


//------------------------------------------------------------------------------
// fault related
//------------------------------------------------------------------------------
// convert a threshold in enginnering units (volts,amps etc,) or amps to ADC threshold
// formula is:
// integer threshold = (volts or amps threshold)*gain + offset
// convert fault thresholds and delays into integers that can be used by the firmware
#define ISEC_OC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(ISEC_OC_THRES_TRIG_AMPS,ISEC_CT_SNS_GAIN,ISEC_CT_SNS_OFS))
#define ISEC_OC_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(ISEC_OC_THRES_CLEAR_AMPS,ISEC_CT_SNS_GAIN,ISEC_CT_SNS_OFS))
#define ISEC_OC_T_BLANK_TRIG                ((uint16_t)(_rnd(ISEC_OC_T_BLANK_TRIG_SEC / ISEC_OC_TICK_SEC)))
#define ISEC_OC_T_BLANK_CLEAR               ((uint16_t)(_rnd(ISEC_OC_T_BLANK_CLEAR_SEC / ISEC_OC_TICK_SEC)))

#define IPRI_OC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(IPRI_OC_THRES_TRIG_AMPS,IPRI_CT_SNS_GAIN,IPRI_CT_SNS_OFS))
#define IPRI_OC_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(IPRI_OC_THRES_CLEAR_AMPS,IPRI_CT_SNS_GAIN,IPRI_CT_SNS_OFS))
#define IPRI_OC_T_BLANK_TRIG                ((uint16_t)(_rnd(IPRI_OC_T_BLANK_TRIG_SEC / IPRI_OC_TICK_SEC)))      
#define IPRI_OC_T_BLANK_CLEAR               ((uint16_t)(_rnd(IPRI_OC_T_BLANK_CLEAR_SEC / IPRI_OC_TICK_SEC)))      

#define VSEC_OV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VSEC_OV_THRES_TRIG_VOLTS,VSEC_SNS_GAIN,0.0))
#define VSEC_OV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VSEC_OV_THRES_CLEAR_VOLTS,VSEC_SNS_GAIN,0.0))
#define VSEC_OV_T_BLANK_TRIG                ((uint16_t)(_rnd(VSEC_OV_T_BLANK_TRIG_SEC / VSEC_OV_TICK_SEC)))  
#define VSEC_OV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VSEC_OV_T_BLANK_CLEAR_SEC / VSEC_OV_TICK_SEC)))  

#define VSEC_UV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VSEC_UV_THRES_TRIG_VOLTS,VPRI_SNS_GAIN,0.0))
#define VSEC_UV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VSEC_UV_THRES_CLEAR_VOLTS,VPRI_SNS_GAIN,0.0))
#define VSEC_UV_T_BLANK_TRIG                ((uint16_t)(_rnd(VSEC_UV_T_BLANK_TRIG_SEC / VSEC_UV_TICK_SEC)))    
#define VSEC_UV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VSEC_UV_T_BLANK_CLEAR_SEC / VSEC_UV_TICK_SEC)))    

#define VPRI_OV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VPRI_OV_THRES_TRIG_VOLTS,VPRI_SNS_GAIN,VPRI_SNS_OFS))
#define VPRI_OV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VPRI_OV_THRES_CLEAR_VOLTS,VPRI_SNS_GAIN,VPRI_SNS_OFS))
#define VPRI_OV_T_BLANK_TRIG                ((uint16_t)(_rnd(VPRI_OV_T_BLANK_TRIG_SEC / VSEC_OV_TICK_SEC))) 
#define VPRI_OV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VPRI_OV_T_BLANK_CLEAR_SEC / VSEC_OV_TICK_SEC))) 


#define ISEC_SC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(ISEC_SC_THRES_TRIG_AMPS,ISEC_CT_SNS_GAIN,ISEC_CT_SNS_OFS))     
#define IPRI_SC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(IPRI_SC_THRES_TRIG_AMPS,IPRI_CT_SNS_GAIN,IPRI_CT_SNS_OFS))
#define I_SC_T_BLANK_CLEAR                  ((uint16_t)(_rnd(I_SC_T_BLANK_CLEAR_SEC / I_SC_TICK_SEC)))

// convert I_SC_LEB_TIME (seconds) into an integer that can be loaded into PGxDC register (assume PWM is in High resolution mode so resolution is 250ps)
#define I_SC_LEB_TIME_PGxDC                   ((uint16_t)(_rnd(I_SC_LEB_TIME / 250.0e-12)))

//------------------------------------------------------------------------------
// parameters related to secondary current sensor calibration
//------------------------------------------------------------------------------
// allowing for +/-5% from ideal
#define ISEC_AVG_SENSOR_OFFSET_LIMIT_HIGH      (_rnd((ISEC_AVG_SNS_OFS*1.05)/3.3*4096.0))
#define ISEC_AVG_SENSOR_OFFSET_LIMIT_LOW       (_rnd((ISEC_AVG_SNS_OFS*0.95)/3.3*4096.0))

#endif	/* MACROS_H */

