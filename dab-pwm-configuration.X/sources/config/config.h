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

#define PERIOD_MODULATION_DEMO   false  ///< modulates the period; for further development
#define CURRENT_CALIBRATION     true    ///< Enable current calibration
#define VPRI_OPTOCOUPLER_POLARITY       false ///< false if not inverted; true if inverted 

/******************************************************************************
 * @ingroup pwm-fundamentals
 * @{
 ******************************************************************************/
#define MAXIMUM_SWITCHING_FREQUENCY     (float)230.0e+3 ///< Maximum Switching frequency in [Hz]
#define MINIMUM_SWITCHING_FREQUENCY     (float) 65.0e+3 ///< Minimum Switching frequency in [Hz]

#define MINIMUM_DEADTIME                (float)150e-9 ///< Minimum Deadtime in seconds [sec]
#define MAXIMUM_DEADTIME                (float)500e-9 ///< Maximum Deadtime in seconds [sec]
#define MINIMUM_PHASESHIFTED_PULSE      (float) 80e-9 ///< Minimum phase shifted pulse in seconds [sec]

#define PRI_TO_SEC_PHASE_DEGREES_LIMIT  90u ///< Maximum Limit for primary to secondary phase in degrees [deg]

#if (PERIOD_MODULATION_DEMO == true)
#define PRI_TO_SEC_PHASE_TARGET         830u ///< Primary to Secondary phase target before switching to period modulation
#define PERIODSTEP                      (2<<3) ///< period increment with 3LSBs needs to be 0 based from PWM FRM with cascaded PWM 
#define PHASETIMESTEP                   (1<<3) ///< Phase increment with 3LSBs needs to be 0 based from PWM FRM with cascaded PWM 
#endif
/** @} */ // end of group pwm-fundamentals ~~~~~~~~~~~~~~~~~~~~


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

/******************************************************************************
 * @ingroup primary-current-feedback
 * @{
 ******************************************************************************/
// T_ADC_SAMPLE_SECS: smallest period between ADC samples
#define T_ADC_SAMPLE_SECS                   (10.0e-6) // ADC sampling rate

#define IPRI_OC_THRES_TRIG_AMPS             (39.0)
#define IPRI_OC_THRES_CLEAR_AMPS            (2.0)
#define IPRI_OC_T_BLANK_TRIG_SEC            (50.0e-6)
#define IPRI_OC_T_BLANK_CLEAR_SEC           (0.0)
#define IPRI_OC_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#define IPRI_SC_THRES_TRIG_AMPS             (39.0)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-current-feedback
 * @{
 ******************************************************************************/
#define ISEC_OC_THRES_TRIG_AMPS             (49.0)
#define ISEC_OC_THRES_CLEAR_AMPS            (2.0)
#define ISEC_OC_T_BLANK_TRIG_SEC            (50.0e-6)
#define ISEC_OC_T_BLANK_CLEAR_SEC           (0.0)
#define ISEC_OC_TICK_SEC                    (T_ADC_SAMPLE_SECS) 

#define ISEC_SC_THRES_TRIG_AMPS             (49.0)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup primary-voltage-feedback
 * @{
 ******************************************************************************/
#define VPRI_OV_THRES_TRIG_VOLTS            (700.0)
#define VPRI_OV_THRES_CLEAR_VOLTS           (600.0)
#define VPRI_OV_T_BLANK_TRIG_SEC            (0.0)
#define VPRI_OV_T_BLANK_CLEAR_SEC           (0.0)
#define VPRI_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-voltage-feedback
 * @{
 ******************************************************************************/
#define VSEC_OV_THRES_TRIG_VOLTS            (490.0)
#define VSEC_OV_THRES_CLEAR_VOLTS           (20.0)
#define VSEC_OV_T_BLANK_TRIG_SEC            (1.0e-3)
#define VSEC_OV_T_BLANK_CLEAR_SEC           (10.0e-3)
#define VSEC_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#define VSEC_UV_THRES_TRIG_VOLTS            (100.0)
#define VSEC_UV_THRES_CLEAR_VOLTS           (50.0)
#define VSEC_UV_T_BLANK_TRIG_SEC            (0.0)
#define VSEC_UV_T_BLANK_CLEAR_SEC           (0.0)
#define VSEC_UV_TICK_SEC                    (T_ADC_SAMPLE_SECS)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup vaux-voltage-feedback
 * @{
 ******************************************************************************/
#define VRAIL_5V_UV_THRES_TRIG_VOLTS        (3.5)
#define VRAIL_5V_UV_THRES_CLEAR_VOLTS       (3.8)
#define VRAIL_5V_UV_T_BLANK_TRIG_SEC        (50.0e-6)
#define VRAIL_5V_UV_T_BLANK_CLEAR_SEC       (0.0)
#define VRAIL_5V_UV_TICK_SEC                (T_ADC_SAMPLE_SECS)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup control-loop
 * @{
 ******************************************************************************/
#define VPLOOP_ILOOP_EXE_RATIO           (uint16_t)(10 / 2) ///< ratio of ILOOP /VPLOOP execution [100KHz / 10KHz], the divide 2 is for interleaved VLoop and PLoop
#define AGC_MINIMUM_VIN_THRESHOLD       160u ///< Minimum VIN threshold to activate AGC in [V]  
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~


//------------------------------------------------------------------------------
// control (for debug only)
//------------------------------------------------------------------------------
#define DAC_DEBUG           false    ///< test values with PIM DAC output
#define OPEN_LOOP_POTI      false    ///< test the code with Digital Power Development Board; runs in openloop using potentiometer
#define OPEN_LOOP_PBV       false    ///< use Power Board visualizer to change the parameters in Open loop operation

//------------------------------------------------------------------------------
// fault disables (for debug only)
//------------------------------------------------------------------------------
#define FAULT_VPRI_OV   true
#define FAULT_ISEC_OC   true
#define FAULT_IPRI_OC   true
#define FAULT_VSEC_OV   true
#define FAULT_PS_OTP    true
#define FAULT_SHORT_CCT true
#define FAULT_VRAIL_5V  true
//------------------------------------------------------------------------------

#endif	/* CONFIG_H */

