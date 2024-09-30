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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef HARDWARE_H
#define	HARDWARE_H

//------------------------------------------------------------------------------
// hardware constants (voltage & current sense, gains and offsets)
//------------------------------------------------------------------------------
/******************************************************************************
 * @ingroup primary-voltage-feedback
 * @{
 ******************************************************************************/
// VPRI_SNS_GAIN: VPRI_SNS = VPRI * VPRI_SNS_GAIN + VPRI_SNS_OFS
#if defined (VPRI_OPTOCOUPLER_POLARITY) && (VPRI_OPTOCOUPLER_POLARITY ==  true)
    #define VPRI_SNS_GAIN               (float)(-3.600e-3)   // V/V  
    #define VPRI_SNS_OFS                (3.3183)   // V
#else
    #define VPRI_SNS_GAIN               (float)(3.600e-3)   // V/V  
    #define VPRI_SNS_OFS                (0.0)   // V
#endif

#define VPRI_VOLTAGE_GAIN            (float)0.2229 ////< scaling for voltage vs ADC value

/** @} */ // end of group primary-voltage-feedback ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-voltage-feedback
 * @{
 ******************************************************************************/
// VSEC_SNS = VSEC * VSEC_SNS_GAIN
#define VSEC_SNS_GAIN               (3.18e-3)   // V/V
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~


/******************************************************************************
 * @ingroup primary-current-feedback
 * @{
 ******************************************************************************/
// IPRI_CT_SNS = IPRI_CT * IPRI_CT_SNS_GAIN + IPRI_CT_SNS_OFS
// note that the IPRI_CT_SNS is rectified via hardware
#define IPRI_CT_SNS_GAIN            (82.5e-3)  // V/A
#define IPRI_CT_SNS_OFS             (0.0)      // V
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup secondary-current-feedback
 * @{
 ******************************************************************************/
// ISEC_CT_SNS = ISEC_CT * ISEC_CT_SNS_GAIN + ISEC_CT_SNS_OFS
// note that the ISEC_CT_SNS is rectified on the power board
#define ISEC_CT_SNS_GAIN            (66.0e-3)   // V/A
#define ISEC_CT_SNS_OFS             (0.0)     // V

// ISEC_AVG_SNS = ISEC_AVG * ISEC_AVG_SNS_GAIN + ISEC_AVG_SNS_OFS
#define ISEC_AVG_SNS_GAIN           (25.0e-3)   // V/A
#define ISEC_AVG_SNS_OFS            (1.65)      // V
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

/******************************************************************************
 * @ingroup vaux-voltage-feedback
 * @{
 ******************************************************************************/
// VRAIL_5V_SNS = VRAIL_5V * VRAIL_5V_SNS_GAIN
#define VRAIL_5V_SNS_GAIN           (0.5)       // V/V
/** @} */ // end of group ~~~~~~~~~~~~~~~~~~~~

#endif	/* HARDWARE_H */

