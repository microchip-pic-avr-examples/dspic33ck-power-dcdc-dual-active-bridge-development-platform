/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */
#ifndef CONFIG_H
#define	CONFIG_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

/******************************************************************************
 * @ingroup special-options
 * @{
 ******************************************************************************/
#define PERIOD_MODULATION_DEMO   false  ///< modulates the period; for further development
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

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
#define T_ADC_SAMPLE_SECS                   (float) 10.0e-6 ///< ADC sampling rate

#define IPRI_OC_THRES_TRIG_AMPS             (float) 39.0 ///< Primary over current trigger threshold
#define IPRI_OC_THRES_CLEAR_AMPS            (float)  2.0
#define IPRI_OC_T_BLANK_TRIG_SEC            (float) 50.0e-6
#define IPRI_OC_T_BLANK_CLEAR_SEC           (float)  0.0
#define IPRI_OC_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#define IPRI_SC_THRES_TRIG_AMPS             (float) 39.0
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-current-feedback
 * @{
 ******************************************************************************/
#define ISEC_OC_THRES_TRIG_AMPS             (float) 49.0
#define ISEC_OC_THRES_CLEAR_AMPS            (float)  2.0
#define ISEC_OC_T_BLANK_TRIG_SEC            (float) 50.0e-6
#define ISEC_OC_T_BLANK_CLEAR_SEC           (float)  0.0
#define ISEC_OC_TICK_SEC                    (T_ADC_SAMPLE_SECS) 

#define ISEC_SC_THRES_TRIG_AMPS             (float) 49.0
#define ISEC_LOAD_STEP_CLAMPING_AMPS        (float) 1.5 
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup primary-voltage-feedback
 * @{
 ******************************************************************************/
#define VPRI_OV_THRES_TRIG_VOLTS            (float)700.0
#define VPRI_OV_THRES_CLEAR_VOLTS           (float)600.0
#define VPRI_OV_T_BLANK_TRIG_SEC            (float)  0.0
#define VPRI_OV_T_BLANK_CLEAR_SEC           (float)  0.0
#define VPRI_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-voltage-feedback
 * @{
 ******************************************************************************/
#define VSEC_OV_THRES_TRIG_VOLTS            (float)490.0
#define VSEC_OV_THRES_CLEAR_VOLTS           (float) 20.0
#define VSEC_OV_T_BLANK_TRIG_SEC            (float)  1.0e-3
#define VSEC_OV_T_BLANK_CLEAR_SEC           (float) 10.0e-3
#define VSEC_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#define VSEC_UV_THRES_TRIG_VOLTS            (float)100.0
#define VSEC_UV_THRES_CLEAR_VOLTS           (float) 50.0
#define VSEC_UV_T_BLANK_TRIG_SEC            (float)  0.0
#define VSEC_UV_T_BLANK_CLEAR_SEC           (float)  0.0
#define VSEC_UV_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#define VSEC_LOAD_STEP_CLAMPING_VOLTS       (float) 16
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup vaux-voltage-feedback
 * @{
 ******************************************************************************/
#define VRAIL_5V_UV_THRES_TRIG_VOLTS        (float)  3.5
#define VRAIL_5V_UV_THRES_CLEAR_VOLTS       (float)  3.8
#define VRAIL_5V_UV_T_BLANK_TRIG_SEC        (float) 50.0e-6
#define VRAIL_5V_UV_T_BLANK_CLEAR_SEC       (float)  0.0
#define VRAIL_5V_UV_TICK_SEC                (T_ADC_SAMPLE_SECS)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup temperature-feedback
 * @{
 ******************************************************************************/
#define MAX_TEMPERATURE_THRESHOLD_RAW           2528u      ///< max temperature threshold RAW value equivalent to 80 degrees Celsius
#define TEMPERATURE_HYSTERISIS_RAW              100u       ///< temperature hysteresis threshold RAW value  
#define FAULT_PERSISTENCE_COUNT_TEMP            4u
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup control-loop
 * @{
 ******************************************************************************/
#define VPLOOP_ILOOP_EXE_RATIO           (uint16_t)(10 / 2) ///< ratio of ILOOP /VPLOOP execution [100KHz / 10KHz], the divide 2 is for interleaved VLoop and PLoop
#define AGC_MINIMUM_VIN_THRESHOLD       160u ///< Minimum VIN threshold to activate AGC in [V]  
#define AGC_MINIMUM_ISEC_THRESHOLD      7 ///< Minimum current threshold to activate AGC in [A]
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~


//------------------------------------------------------------------------------
// control (for debug only)
//------------------------------------------------------------------------------
/******************************************************************************
 * @ingroup special-options
 * @{
 ******************************************************************************/
#define DAC_DEBUG           false    ///< test values with PIM DAC output
#define OPEN_LOOP_POTI      false    ///< test the code with Digital Power Development Board; runs in openloop using potentiometer
#define OPEN_LOOP_PBV       false    ///< use Power Board visualizer to change the parameters in Open loop operation

#if(OPEN_LOOP_POTI || OPEN_LOOP_PBV)
#define CURRENT_CALIBRATION             false  ///< Enable current calibration
#else
#define CURRENT_CALIBRATION             true  ///< Enable current calibration
#endif
#define VPRI_OPTOCOUPLER_POLARITY       false ///< false if not inverted; true if inverted 

/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

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

