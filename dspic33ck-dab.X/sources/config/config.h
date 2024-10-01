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
#define PERIOD_MODULATION_DEMO   false  ///< Modulates the period; for further development
#define DCDC400V_SYSTEM           true  ///< When true, operates the DAB with protection thresholds for 400V output
#define DCDC800V_SYSTEM           false  ///< When true, operates the DAB with protection thresholds for 800V output
#define ENABLE_VLOOP_AGC          false ///< This is an optional feature (not needed for battery application)
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

#if defined(PERIOD_MODULATION_DEMO) && (PERIOD_MODULATION_DEMO == true)
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
#if(DCDC400V_SYSTEM)
    #define VPRI_OV_THRES_TRIG_VOLTS            (float)700.0
    #define VPRI_OV_THRES_CLEAR_VOLTS           (float)600.0
#elif(DCDC800V_SYSTEM)
    #define VPRI_OV_THRES_TRIG_VOLTS            (float)900.0
    #define VPRI_OV_THRES_CLEAR_VOLTS           (float)800.0
#endif
#define VPRI_OV_T_BLANK_TRIG_SEC            (float)  0.0
#define VPRI_OV_T_BLANK_CLEAR_SEC           (float)  0.0
#define VPRI_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#if(DCDC400V_SYSTEM)
    #define VPRI_UV_THRES_TRIG_VOLTS            (float)300.0
    #define VPRI_UV_THRES_CLEAR_VOLTS           (float)200.0
#elif(DCDC800V_SYSTEM)
    #define VPRI_UV_THRES_TRIG_VOLTS            (float)450.0
    #define VPRI_UV_THRES_CLEAR_VOLTS           (float)300.0
#endif
#define VPRI_UV_T_BLANK_TRIG_SEC            (float)  0.0
#define VPRI_UV_T_BLANK_CLEAR_SEC           (float)  0.0
#define VPRI_UV_TICK_SEC                    (T_ADC_SAMPLE_SECS)
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-voltage-feedback
 * @{
 ******************************************************************************/
#if(DCDC400V_SYSTEM)
#define VSEC_OV_THRES_TRIG_VOLTS            (float)500.0
#define VSEC_OV_THRES_CLEAR_VOLTS           (float) 20.0
#elif(DCDC800V_SYSTEM)
#define VSEC_OV_THRES_TRIG_VOLTS            (float)950.0
#define VSEC_OV_THRES_CLEAR_VOLTS           (float) 20.0
#endif
#define VSEC_OV_T_BLANK_TRIG_SEC            (float)  1.0e-3
#define VSEC_OV_T_BLANK_CLEAR_SEC           (float) 10.0e-3
#define VSEC_OV_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#if(DCDC400V_SYSTEM)
#define VSEC_UV_THRES_TRIG_VOLTS            (float)100.0
#define VSEC_UV_THRES_CLEAR_VOLTS           (float) 50.0
#elif(DCDC800V_SYSTEM)
#define VSEC_UV_THRES_TRIG_VOLTS            (float)550.0
#define VSEC_UV_THRES_CLEAR_VOLTS           (float) 50.0
#endif
#define VSEC_UV_T_BLANK_TRIG_SEC            (float)  0.0
#define VSEC_UV_T_BLANK_CLEAR_SEC           (float)  0.0
#define VSEC_UV_TICK_SEC                    (T_ADC_SAMPLE_SECS)

#define VSEC_LOAD_STEP_CLAMPING_VOLTS       (float) 16
#define VPRIM_LOAD_STEP_CLAMPING_VOLTS       (float) 16
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
#define TEMPERATURE_PBV_OFFSET_CELCIUS          40u     ///< To allow the PBV to support up to -40C, an offset was added
#define OTP_THRES_TRIG_CELCIUS             (float) 80.0 + TEMPERATURE_PBV_OFFSET_CELCIUS ///< Degrees Celcius
#define OTP_THRES_CLEAR_CELCIUS            (float) 70.0 + TEMPERATURE_PBV_OFFSET_CELCIUS  ///< Degrees Celcius
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

#if((OPEN_LOOP_POTI == true) || (OPEN_LOOP_PBV == true))
#define CURRENT_CALIBRATION             false  ///< Enable current calibration
#else
#define CURRENT_CALIBRATION             true  ///< Enable current calibration
#endif
#define VPRI_OPTOCOUPLER_POLARITY       false ///< false if not inverted; true if inverted 

/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

//------------------------------------------------------------------------------
// fault disables (for debug only)
// during debug mode, Current calibration needs to be set to false
//------------------------------------------------------------------------------
#define FAULT_VPRI_OV   true
#define FAULT_VPRI_UV   false
#define FAULT_ISEC_OC   true
#define FAULT_IPRI_OC   true
#define FAULT_VSEC_OV   true
#define FAULT_PS_OTP    true
#define FAULT_SHORT_CCT true
#define FAULT_VRAIL_5V  true
#define LOAD_DISCONNECT true
//------------------------------------------------------------------------------

#endif	/* CONFIG_H */

