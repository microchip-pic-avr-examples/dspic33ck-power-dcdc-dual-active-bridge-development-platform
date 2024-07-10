/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

#ifndef DRV_MC_EXTENSION_ADC_H
#define	DRV_MC_EXTENSION_ADC_H

#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#define ADC_ANINPUT_COUNT   (20)

/*********************************************************************************
 * @ingroup 
 * @def     DRV_ADC_SetChannel
 * @brief   Set ADC conversion channel
 * @details
 * Peripheral Change ADC input channel
// **********************************************************************************/
#define DRV_ADC_SetChannel(channel)           (ADCON3Lbits.CNVCHSEL = channel)
//
///*********************************************************************************
// * @ingroup GROUP_NAME
// * @def     MACRO_NAME
// * @brief   ADD_SHORT_DESCRIPTION
// * @details
// *  ADD_DESCRIPTION_HERE
// **********************************************************************************/
#define DRV_ADC_TriggerSoftwareConversion()   (ADCON3Lbits.CNVRTCH = 1) 


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     MACRO_NAME
 * @brief   ADD_SHORT_DESCRIPTION
 * @details
 *  ADD_DESCRIPTION_HERE
 **********************************************************************************/
enum ADTRIG_TRGSRC_e {

    ADTRIGx_TRGSRC_ADTRG31    = 0b11111, // ADC Trigger #31 (PPS input)
    ADTRIGx_TRGSRC_LSWTRG     = 0b00010, // Level software trigger
    ADTRIGx_TRGSRC_CSWTRG     = 0b00001, // Common Software Trigger
    ADTRIGx_TRGSRC_PTG        = 0b11110, // AD trigger source: PTG
    ADTRIGx_TRGSRC_CLC2       = 0b11101, // AD trigger source: CLC2
    ADTRIGx_TRGSRC_CLC1       = 0b11100, // AD trigger source: CLC1
    ADTRIGx_TRGSRC_MCCP9      = 0b11011, // AD trigger source: MCCP9
    ADTRIGx_TRGSRC_SCCP7      = 0b11010, // AD trigger source: SCCP7
    ADTRIGx_TRGSRC_SCCP6      = 0b11001, // AD trigger source: SCCP6
    ADTRIGx_TRGSRC_SCCP5      = 0b11000, // AD trigger source: SCCP5
    ADTRIGx_TRGSRC_SCCP4      = 0b10111, // AD trigger source: SCCP4
    ADTRIGx_TRGSRC_SCCP3      = 0b10110, // AD trigger source: SCCP3
    ADTRIGx_TRGSRC_SCCP2      = 0b10101, // AD trigger source: SCCP2
    ADTRIGx_TRGSRC_SCCP1      = 0b10100, // AD trigger source: SCCP1
    ADTRIGx_TRGSRC_PWM8_TRIG2 = 0b10011, // AD trigger source: PWM8 Trigger 2
    ADTRIGx_TRGSRC_PWM8_TRIG1 = 0b10010, // AD trigger source: PWM8 Trigger 1
    ADTRIGx_TRGSRC_PWM7_TRIG2 = 0b10001, // AD trigger source: PWM7 Trigger 2
    ADTRIGx_TRGSRC_PWM7_TRIG1 = 0b10000, // AD trigger source: PWM7 Trigger 1
    ADTRIGx_TRGSRC_PWM6_TRIG2 = 0b01111, // AD trigger source: PWM6 Trigger 2
    ADTRIGx_TRGSRC_PWM6_TRIG1 = 0b01110, // AD trigger source: PWM6 Trigger 1
    ADTRIGx_TRGSRC_PWM5_TRIG2 = 0b01101, // AD trigger source: PWM5 Trigger 2
    ADTRIGx_TRGSRC_PWM5_TRIG1 = 0b01100, // AD trigger source: PWM5 Trigger 1
    ADTRIGx_TRGSRC_PWM4_TRIG2 = 0b01011, // AD trigger source: PWM4 Trigger 2
    ADTRIGx_TRGSRC_PWM4_TRIG1 = 0b01010, // AD trigger source: PWM4 Trigger 1
    ADTRIGx_TRGSRC_PWM3_TRIG2 = 0b01001, // AD trigger source: PWM3 Trigger 2
    ADTRIGx_TRGSRC_PWM3_TRIG1 = 0b01000, // AD trigger source: PWM3 Trigger 1
    ADTRIGx_TRGSRC_PWM2_TRIG2 = 0b00111, // AD trigger source: PWM2 Trigger 2
    ADTRIGx_TRGSRC_PWM2_TRIG1 = 0b00110, // AD trigger source: PWM2 Trigger 1
    ADTRIGx_TRGSRC_PWM1_TRIG2 = 0b00101, // AD trigger source: PWM1 Trigger 2
    ADTRIGx_TRGSRC_PWM1_TRIG1 = 0b00100, // AD trigger source: PWM1 Trigger 1
    ADTRIGx_TRGSRC_NONE       = 0b00000 // No trigger is selected
        
};
typedef enum ADTRIG_TRGSRC_e ADTRIG_TRGSRC_t;

/*
 * ************************************************************************************************
 * Summary:
 * Configures interrupt trigger source setting of an individual analog input
 *
 * Parameters:
 *     - inputIndex: index of channel (0 for AN0, 1 for AN1, etc)
 *     - triggerSource: trigger source
 *
 * Returns: 
 *      0: failure
 *      1: success
 * 
 * Description:
 * This function configures the trigger source selection for corresponding analog inputs by
 * extracting the user setting from the function parameter "triggerSource" 
 * 
 * ***********************************************************************************************/
static __inline__ uint16_t ADC_ADInput_SetTriggerSource(uint16_t inputIndex, ADTRIG_TRGSRC_t triggerSource)
{
    uint16_t fres = 1;
    uint8_t *regptr;
    
    // Check if given ADC input index is within available range
    if (inputIndex >= ADC_ANINPUT_COUNT) return(0);

    // Determine register set offset
    regptr = (uint8_t *)&ADTRIG0L; // get register block base address
    regptr += (uint8_t)inputIndex;   // add offset
    *regptr = (uint8_t)triggerSource;
    
    // Check if WRITE operation was successful
    fres &= (bool)((*regptr == (uint8_t)triggerSource));  

    return(1);
}


#endif	/* DRV_MC_EXTENSION_ADC_H */

