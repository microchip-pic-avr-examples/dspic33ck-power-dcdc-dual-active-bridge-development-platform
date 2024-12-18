
#ifndef MACROS_H
#define	MACROS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types
#include <math.h> // include standard math functions library

#include "useful_macros.h"
#include "config.h"
#include "hardware.h"


//device specific setup
// DO NOT MODIFY
#define PWM_CLOCK_FREQUENCY     (float)4.0e+9 ///< PWM High Resolution Clock Frequency in [Hz]

#define ADC_RESOLUTION          4095 ///< ADC number of ticks for 12-bit ADC resolution
#define ADC_REFERENCE           (float)3.3  ///< ADC reference value
#define ADC_SCALER              (float)(1.0 / ADC_RESOLUTION) ///< ADC scaling for each ADC tick
//#define PHASE_DEGREES           360U    ///< Maximum Phase degree 
#define PHASE_180_SCALER        (float)(1.0 / 180)

/***********************************************************************************
 * @ingroup pwm-macros
 * @{ 
 * @brief Conversion macros for user-declarations of PWM parameters
 * 
 * @details
 * These conversion macros are used to convert user settings defined as physical 
 * quantities into binary (integer) numbers, which will be written to registers and
 * variables and used in calculations throughout the firmware.
 ***********************************************************************************/

#define PWM_CLOCK_PERIOD       (float)(1.0 / PWM_CLOCK_FREQUENCY) ///< PWM Clock Period in [sec]
#define MAX_SWITCHING_PERIOD   (float)(1.0/MINIMUM_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define MIN_SWITCHING_PERIOD   (float)(1.0/MAXIMUM_SWITCHING_FREQUENCY)   ///< Switching period in [sec]
#define MAX_PWM_PERIOD         (uint16_t)(float)(MAX_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter
#define MIN_PWM_PERIOD         (uint16_t)(float)(MIN_SWITCHING_PERIOD / PWM_CLOCK_PERIOD) ///< This sets the switching period of the converter
#define PERIOD_RANGE           (uint16_t)(MAX_PWM_PERIOD - MIN_PWM_PERIOD)  ///< This is the calculation of Period range. NOTE: only use in Debugging moode using digital power development board.
#define ADC_PERIOD_RANGE       (uint16_t)(float)(PERIOD_RANGE / ADC_RESOLUTION) ///< This is the calculation for ADC equivalent with Period range. NOTE: only use in Debugging moode using digital power development board.

//#define CONTROL_PHASE_RAD      (uint16_t)(float)(PHASE_DEGREES / 180) ///< This sets the maximum PWM phase of the converter
//#define MIN_PER_CONTROL_PHASE  (uint16_t)(float)(MIN_PWM_PERIOD / CONTROL_PHASE_RAD)
//#define MAX_PER_CONTROL_PHASE  (uint16_t)(float)(MAX_PWM_PERIOD / CONTROL_PHASE_RAD)

#define MIN_PWM_DEAD_TIME   (uint16_t)(MINIMUM_DEADTIME / (float)PWM_CLOCK_PERIOD) ///< Minimum dead time [tick = 250ps]
#define MAX_PWM_DEAD_TIME   (uint16_t)(MAXIMUM_DEADTIME / (float)PWM_CLOCK_PERIOD) ///< Maximum dead time [tick = 250ps]

#define MIN_PHASE_SHIFTED_PULSE   (uint16_t)(MINIMUM_PHASESHIFTED_PULSE / (float)PWM_CLOCK_PERIOD) ///< Maximum dead time [tick = 250ps]

#define DEGREES_PHASE_10x                   (10u)   
#define DEGREES_PHASE_SCALER                (10u)
#define DEGREES_PHASE_SCALING_10            ((pow(2.0, DEGREES_PHASE_SCALER)) / DEGREES_PHASE_10x)
#define DEGREES_PHASE_FACTOR                ((PRI_TO_SEC_PHASE_DEGREES_LIMIT * ((DEGREES_PHASE_SCALER)* DEGREES_PHASE_10x)))
/** @} */ // end of group pwm-macros ~~~~~~~~~~~~~~~~~~~~

//------------------------------------------------------------------------------
// fault related
//------------------------------------------------------------------------------
// convert a threshold in engineering units (volts,amps etc,) or amps to ADC threshold
// formula is:
// integer threshold = (volts or amps threshold)*gain + offset
// convert fault thresholds and delays into integers that can be used by the firmware
/******************************************************************************
 * @ingroup secondary-current-feedback-macros
 * @{
 ******************************************************************************/
#define ISEC_OC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(ISEC_OC_THRES_TRIG_AMPS,ISEC_CT_SNS_GAIN,ISEC_CT_SNS_OFS))
#define ISEC_OC_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(ISEC_OC_THRES_CLEAR_AMPS,ISEC_CT_SNS_GAIN,ISEC_CT_SNS_OFS))
#define ISEC_OC_T_BLANK_TRIG                ((uint16_t)(_rnd(ISEC_OC_T_BLANK_TRIG_SEC / ISEC_OC_TICK_SEC)))
#define ISEC_OC_T_BLANK_CLEAR               ((uint16_t)(_rnd(ISEC_OC_T_BLANK_CLEAR_SEC / ISEC_OC_TICK_SEC)))

#define ISEC_SC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(ISEC_SC_THRES_TRIG_AMPS,ISEC_CT_SNS_GAIN,ISEC_CT_SNS_OFS))  
#define ISEC_LOAD_STEP_CLAMP                (UNITS_FROM_ENG_TO_ADC(ISEC_LOAD_STEP_CLAMPING_AMPS,ISEC_AVG_SNS_GAIN,0))

//------------------------------------------------------------------------------
// parameters related to secondary current sensor calibration
//------------------------------------------------------------------------------
// allowing for +/-5% from ideal
#define ISEC_AVG_SENSOR_OFFSET_LIMIT_HIGH      (_rnd((ISEC_AVG_SNS_OFS*1.05)/3.3*4096.0))
#define ISEC_AVG_SENSOR_OFFSET_LIMIT_LOW       (_rnd((ISEC_AVG_SNS_OFS*0.95)/3.3*4096.0))

/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup primary-current-feedback-macros
 * @{
 ******************************************************************************/
#define IPRI_OC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(IPRI_OC_THRES_TRIG_AMPS,IPRI_CT_SNS_GAIN,IPRI_CT_SNS_OFS))
#define IPRI_OC_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(IPRI_OC_THRES_CLEAR_AMPS,IPRI_CT_SNS_GAIN,IPRI_CT_SNS_OFS))
#define IPRI_OC_T_BLANK_TRIG                ((uint16_t)(_rnd(IPRI_OC_T_BLANK_TRIG_SEC / IPRI_OC_TICK_SEC)))      
#define IPRI_OC_T_BLANK_CLEAR               ((uint16_t)(_rnd(IPRI_OC_T_BLANK_CLEAR_SEC / IPRI_OC_TICK_SEC))) 

#define IPRI_SC_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(IPRI_SC_THRES_TRIG_AMPS,IPRI_CT_SNS_GAIN,IPRI_CT_SNS_OFS))
#define I_SC_T_BLANK_CLEAR                  ((uint16_t)(_rnd(I_SC_T_BLANK_CLEAR_SEC / I_SC_TICK_SEC)))
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-voltage-feedback-macros
 * @{
 ******************************************************************************/
#define VSEC_OV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VSEC_OV_THRES_TRIG_VOLTS,VSEC_SNS_GAIN,0.0))
#define VSEC_OV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VSEC_OV_THRES_CLEAR_VOLTS,VSEC_SNS_GAIN,0.0))
#define VSEC_OV_T_BLANK_TRIG                ((uint16_t)(_rnd(VSEC_OV_T_BLANK_TRIG_SEC / VSEC_OV_TICK_SEC)))  
#define VSEC_OV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VSEC_OV_T_BLANK_CLEAR_SEC / VSEC_OV_TICK_SEC)))  

#define VSEC_UV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VSEC_UV_THRES_TRIG_VOLTS,VSEC_SNS_GAIN,0.0))
#define VSEC_UV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VSEC_UV_THRES_CLEAR_VOLTS,VSEC_SNS_GAIN,0.0))
#define VSEC_UV_T_BLANK_TRIG                ((uint16_t)(_rnd(VSEC_UV_T_BLANK_TRIG_SEC / VSEC_UV_TICK_SEC)))    
#define VSEC_UV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VSEC_UV_T_BLANK_CLEAR_SEC / VSEC_UV_TICK_SEC)))    

#define VSEC_LOAD_STEP_CLAMP                (UNITS_FROM_ENG_TO_ADC(VSEC_LOAD_STEP_CLAMPING_VOLTS,VSEC_SNS_GAIN,0.0))
#define VPRIM_LOAD_STEP_CLAMP                (UNITS_FROM_ENG_TO_ADC( VPRIM_LOAD_STEP_CLAMPING_VOLTS,VPRI_SNS_GAIN,0.0))
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup primary-voltage-feedback-macros
 * @{
 ******************************************************************************/
#define VPRI_OV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VPRI_OV_THRES_TRIG_VOLTS,VPRI_SNS_GAIN,VPRI_SNS_OFS))
#define VPRI_OV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VPRI_OV_THRES_CLEAR_VOLTS,VPRI_SNS_GAIN,VPRI_SNS_OFS))
#define VPRI_OV_T_BLANK_TRIG                ((uint16_t)(_rnd(VPRI_OV_T_BLANK_TRIG_SEC / VSEC_OV_TICK_SEC))) 
#define VPRI_OV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VPRI_OV_T_BLANK_CLEAR_SEC / VSEC_OV_TICK_SEC))) 

#define VPRI_UV_THRES_TRIG                  (UNITS_FROM_ENG_TO_ADC(VPRI_UV_THRES_TRIG_VOLTS,VPRI_SNS_GAIN,VPRI_SNS_OFS))
#define VPRI_UV_THRES_CLEAR                 (UNITS_FROM_ENG_TO_ADC(VPRI_UV_THRES_CLEAR_VOLTS,VPRI_SNS_GAIN,VPRI_SNS_OFS))
#define VPRI_UV_T_BLANK_TRIG                ((uint16_t)(_rnd(VPRI_UV_T_BLANK_TRIG_SEC / VSEC_UV_TICK_SEC))) 
#define VPRI_UV_T_BLANK_CLEAR               ((uint16_t)(_rnd(VPRI_UV_T_BLANK_CLEAR_SEC / VSEC_UV_TICK_SEC))) 


#define VPRI_SCALER                         (10u)
#define VPRI_FACTOR                         (uint16_t)((1 / VPRI_VOLTAGE_GAIN) * VPRI_SCALER) 

/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup vaux-voltage-feedback-macros
 * @{
 ******************************************************************************/
#define VRAIL_5V_UV_THRES_TRIG              (UNITS_FROM_ENG_TO_ADC(VRAIL_5V_UV_THRES_TRIG_VOLTS,VRAIL_5V_SNS_GAIN,0.0))
#define VRAIL_5V_UV_THRES_CLEAR             (UNITS_FROM_ENG_TO_ADC(VRAIL_5V_UV_THRES_CLEAR_VOLTS,VRAIL_5V_SNS_GAIN,0.0))
#define VRAIL_5V_UV_T_BLANK_TRIG            ((uint16_t)(_rnd(VRAIL_5V_UV_T_BLANK_TRIG_SEC / VRAIL_5V_UV_TICK_SEC)))  
#define VRAIL_5V_UV_T_BLANK_CLEAR           ((uint16_t)(_rnd(VRAIL_5V_UV_T_BLANK_CLEAR_SEC / VRAIL_5V_UV_TICK_SEC)))  
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup temperature-feedback-macros
 * @{
 ******************************************************************************/

#define TEMPERATURE_FACTOR              (int16_t)(TEMPERATURE_GAIN * pow(2.0, 15))

#define L_TEMPERATURE_THRESHOLD        (uint16_t)(NOMINAL_TEMPERATURE_THRESHOLD - TEMPERATURE_HYSTERESIS)
#define H_TEMPERATURE_THRESHOLD        (uint16_t)(NOMINAL_TEMPERATURE_THRESHOLD + TEMPERATURE_HYSTERESIS)

/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup control-loop-macros
 * @{
 ******************************************************************************/
#define POWER_SCALER                        (14u)
#define POWER_FACTOR                        (uint16_t)(((ADC_REFERENCE * ADC_REFERENCE * pow(2.0, POWER_SCALER))) / ((ADC_RESOLUTION * VSEC_SNS_GAIN) * (ADC_RESOLUTION * ISEC_AVG_SNS_GAIN))) 

#define AGC_VOLTAGE_FACTOR                      (uint32_t)(AGC_MINIMUM_VIN_THRESHOLD * pow(2.0, 15))
#define AGC_MINIMUM_CURRENT_THRESHOLD                   (UNITS_FROM_ENG_TO_ADC(AGC_MINIMUM_ISEC_THRESHOLD,ISEC_AVG_SNS_GAIN,0.0))
#define AGC_CURRENT_FACTOR                      (uint32_t)(AGC_MINIMUM_CURRENT_THRESHOLD * pow(2.0, 15))
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~


#endif	/* MACROS_H */

