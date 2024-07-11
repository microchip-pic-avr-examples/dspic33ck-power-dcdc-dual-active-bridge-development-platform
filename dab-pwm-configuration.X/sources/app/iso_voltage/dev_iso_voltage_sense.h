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

/**
 * @file    dev_iso_voltage_sense.h
 * @brief   isolated voltage device driver
 * @author  M70027
 * @date    15/02/2024
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEV_ISO_VOLTAGE_SENSE_H
#define	DEV_ISO_VOLTAGE_SENSE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdlib.h>

/*********************************************************************************
 * @ingroup dev-vin-sccp-abstraction-macros
 * @
 * @brief   These have to be renamed to make the Vin device driver abstract
 **********************************************************************************/
#include "input_capture/sccp2.h"
#define InputCapture_CallbackRegister   SCCP2_InputCapture_CallbackRegister     ///< registering the callback function on interrupt
#define InputCapture_bufferoverflow     SCCP2_InputCapture_HasBufferOverflowed  ///< 
#define InputCapture_BufferEmpty        SCCP2_InputCapture_IsBufferEmpty        ///< 
#define CCP_TIMER_HIGH_REG              CCP2TMRH                                ///< 
#define CCP_TIMER_LOW_REG               CCP2TMRL                                ///< 
#define CCP_INTERRUPT_FLAG              IFS1bits.CCP2IF                         ///< abstracting away interrupt flag
#define CCP_BUF_L                       CCP2BUFL                                ///< 
#define CCP_INPUT_CAPTURE_BUFFER        4                                       ///< depth of sccp peripheral input capture buffer
/** @} */ // end of dev-vin-sccp-abstraction-macros

/*********************************************************************************
 * @ingroup dev-vin-sccp-avg-buffer-define-macros
 * @
 * @brief   averaging filter depth 
 **********************************************************************************/

//#define MAX_AVG_SHIFTING 6
//#define MAX_NUM_SAMPLES (1<<(MAX_AVG_SHIFTING))

#define MAX_NUM_SAMPLES_ISO_BUFFER                 128          ///< buffer depth for average calculation. should be a power of two
#define MAX_NUM_SAMPLES_ISO_BUFFER_AS_EXP_OF_TWO   7            ///< 2 to the power of N = MAX_NUM_SAMPLES. Used for buffer averaging.
/** @} */ // end of dev-vin-sccp-avg-buffer-define-macros

/*********************************************************************************
 * @ingroup dev-vin-sccp-sys-spec-macros
 * @
 * @brief   defines the voltage limits, scaling factors of the system. 
 * @details 
 * SCCP capture counting at 100 Mhz. 
 * DC waveform being generated at 15.66 khz resulting in a period of 63.25ms.   
 * in 63.25ms, a counter running at 100MHz would count up to 63.25ms/0.01ms = 6325 
 * this would correspond to 100% DC.
 * 6325 corresponds to 1024 ADC value. that is multiplier of 6.325
 * now in the case of DAB input voltage = 0.002276 * Vin 
 * so RAW value divided by 6.325 gets us ADC sample. 
 * ADC sample * 4.096 / 1024 gets us voltage on pin.
 * so the multiplier converting RAW to Vin
 * Vin = (RAW/6.23) * 4.096 / (1024 * 0.002276)
 * Vin = 0.282 * RAW 
 **********************************************************************************/
#define OV_THRESHOLD_VOLT       900                                               ///< Application Specfic. TBD
#define OV_THRESHOLD_SCALING    0.282                                              ///< Application Specfic. TBD
#define OV_THRESHOLD_RAW        (OV_THRESHOLD_VOLT / OV_THRESHOLD_SCALING )             ///< example . Must be calcualted based on Scaling factor

#define OV_THRESHOLD_HYST 140               ///< example. Application Specific. Hystersis range. To be determined
#define OV_THRESHOLD_HYST_HALF (OV_THRESHOLD_HYST/2) ///<  

#define OV_UPPER_THRESHOLD_WITH_HYST  OV_THRESHOLD_RAW + OV_THRESHOLD_HYST_HALF
//#define OV_THRESHOLD_HYST 140    
#define OV_LOWER_THRESHOLD_WITH_HYST  OV_THRESHOLD_RAW - OV_THRESHOLD_HYST_HALF

#define UV_THRESHOLD_VOLT 500               ///< Application Specfic. TBD
#define UV_THRESHOLD_SCALING 0.282                ///< Application Specfic. TBD
#define UV_THRESHOLD_RAW  (UV_THRESHOLD_VOLT / UV_THRESHOLD_SCALING)              ///< example . Must be calcualted based on Scaling factor)

#define UV_THRESHOLD_HYST 140               ///< example. Application Specific. Hystersis range. To be determined
#define UV_THRESHOLD_HYST_HALF (OV_THRESHOLD_HYST/2)

#define UV_UPPER_THRESHOLD_WITH_HYST  UV_THRESHOLD_RAW + UV_THRESHOLD_HYST_HALF
//#define UV_THRESHOLD_HYST 140    
#define UV_LOWER_THRESHOLD_WITH_HYST  UV_THRESHOLD_RAW - UV_THRESHOLD_HYST_HALF
        
/** @} */ // end of dev-vin-sccp-sys-spec-macros

/*********************************************************************************
 * @ingroup dev-vin-sccp-fault-counts-macros
 * @
 * @brief   defines the count for faults as well as missing interrupts which would register as faults
 * Interrupt count requires the number of interrupts needed in the the task frequency
 * In this example total number of interrupt = 15.6Khz * 2 (both pos and neg) / 4 (buffer depth) * 0.001(for 1ms task) =  8  
 * Fault persisitence count is the number of ISR callbacks where the fault persists. . 
 **********************************************************************************/
#define INTERRUPT_COUNTS_MIN    6           ///< Application Specfic. less than 6 and system declares that input is not present
#define INTERRUPT_COUNTS_MAX    9           ///< Application Specfic. more than 9 and system declates that input is not present
#define FAULT_PERSISTENCE_COUNT  8          ///<fault persisits for how many clock cycles
/** @} */ // end of dev-vin-sccp-fault-counts-macros

/***********************************************************************************
 * @ingroup dev-iso-voltage-data-type
 * @struct  dev_iso_voltage_data_t
 * @brief   a custom data type for isolated voltage measurement
 **********************************************************************************/

typedef struct iso_voltage {
    uint8_t     error_flag;                 ///< error_flag    
    uint8_t     over_voltage_flag;          ///< Over voltage flag
    uint8_t     under_voltage_flag;         ///< under voltage flag
    uint8_t     no_signal_flag;             ///< if no signal is present 
    uint16_t    voltage_raw;                ///< voltage value raw
    uint16_t    voltage_averaged;           ///< voltage averaged raw
    uint16_t    voltage_buffer[MAX_NUM_SAMPLES_ISO_BUFFER]; ///< buffer for averaging
    uint16_t    voltage_buffer_idx;         ///< current index of averaging buffer
    uint8_t     buffer_full;                ///< flag to monitor if full then allow averaging.
    uint8_t     reset_buffer;               ///< reset voltage_buffer
} dev_iso_voltage_data_t;

/*********************************************************************************
 * @ingroup dev-iso-voltate-data-type
 * @var     dev_iso_voltage_data_ptr
 * @brief   dev isoloated voltage data object pointer external
 * @details pointer to the Object 'dev_iso_voltage_data', providing access to all the fan control
 *          and monitoring values. To access it, add the header file to the project, and 
 *          then the pointer can be used to access the dev_iso_voltage_data members.
 **********************************************************************************/
extern dev_iso_voltage_data_t* dev_iso_voltage_data_ptr;

/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/

void Dev_Iso_Voltage_Sens_Init(void);
void Dev_Iso_Voltage_Task_1ms(void);
uint16_t Dev_Get_Iso_Voltage(void);     
uint16_t Dev_Get_Iso_Voltage_avg(void); 
uint8_t Dev_Iso_Voltage_Is_OV (void);   
uint8_t Dev_Iso_Voltage_Is_UV (void);   
void Dev_Reset_Average_Buffer(void);        

#endif	/* DEV_ISO_VOLTAGE_SENSE_H */


//inline uint16_t Dev_Get_Iso_Voltage_avg_inline (void) {
//    //if (dev_iso_voltage_data_ptr->buffer_full)
//    //{
//        uint16_t index = 0;
//        uint32_t sum = 0;
//        for (index = 0; index <MAX_NUM_SAMPLES; index++)
//            sum +=dev_iso_voltage_data_ptr->voltage_buffer[index];
//        return (sum >> MAX_NUM_SAMPLES_AS_EXP_OF_TWO);
//    //}
//    //else 
//    //    return 0;
//}

//inline uint16_t Dev_Get_Iso_Voltage_avg_inline (void){
//     if (dev_iso_voltage_data_ptr->buffer_full)
//        return Iso_Calculate_Average(dev_iso_voltage_data_ptr->voltage_buffer, MAX_NUM_SAMPLES);
//    else 
//        return 0;
//}
