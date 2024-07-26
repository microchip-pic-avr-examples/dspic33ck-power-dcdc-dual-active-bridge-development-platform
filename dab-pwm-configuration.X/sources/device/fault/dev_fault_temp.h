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
 * @file    dev_temp.h
 * @brief   temperature device driver
 * @author  M70027
 * @date    04/03/2024
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEV_TEMP_H
#define	DEV_TEMP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

/*********************************************************************************
 * @ingroup dev-temp-abstraction-macros
 * @
 * @brief   These have to be renamed to make the temp device driver abstract
 **********************************************************************************/
#include "adc/adc1.h"
#define IsConversionComplete        ADC1_IsConversionComplete       
#define GetResult                   ADC1_ConversionResultGet        
#define ADC_TEMP_CHANNEL            TEMP             ///< adc pin/channel name
#define ADC_SOFTWARE_TRIGGER        ADC1_SoftwareTriggerEnable

/*********************************************************************************
 * @ingroup dev-temp-buffer-define-macros
 * @
 * @brief   averaging filter depth 
 **********************************************************************************/
#define MAX_NUM_SAMPLES_TEMP_BUFFER                         64      ///< average calculated using 64 samples
#define MAX_NUM_SAMPLES_TEMP_BUFFER_AS_EXP_OF_TWO            6      ///< for bit shifting
/** @} */ // end of dev-temp-buffer-define-macros

/*********************************************************************************
 * @ingroup dev-temp-sys-spec-macros
 * @
 * @brief   defines the temperature threshold raw
 **********************************************************************************/
#define MAX_TEMPERATURE_THRESHOLD_RAW           2528      ///< max temperature threshold RAW
#define TEMPERATURE_HYSTERISIS_RAW              100
#define TEMPERATURE_HYSTERISIS_HALF             (TEMPERATURE_HYSTERISIS_RAW/2)
#define FAULT_PERSISTENCE_COUNT_TEMP            4
#define OVER_TEMP_UPPER_THRESHOLD_WITH_HYST     MAX_TEMPERATURE_THRESHOLD_RAW + TEMPERATURE_HYSTERISIS_HALF
#define OVER_TEMP_LOWER_THRESHOLD_WITH_HYST     MAX_TEMPERATURE_THRESHOLD_RAW - TEMPERATURE_HYSTERISIS_HALF
/** @} */ // end of dev-temp-sys-spec-macros

/***********************************************************************************
 * @ingroup dev-temp-data-type
 * @struct  dev_Temp_data_t
 * @brief   a custom data type for temperature measurement
 **********************************************************************************/
struct TEMP_SETTINGS_s {
    uint8_t     OverTemperatureFlag;          ///< Over temperature flag
    uint8_t     SensorLost;                   ///< if sensor is present      
    uint16_t    AdcReading;                   ///< temperature value raw sampled from ADC
    uint16_t    AdcAverage;                   ///< temperature 
    uint16_t    TemperatureCelcius;           ///< temperature celcius.
    uint16_t    TempBuffer[MAX_NUM_SAMPLES_TEMP_BUFFER];   ///< buffer for calcualting average.
    uint16_t    BufferFull;                   ///< flag to monitor if full then allow averaging.
    uint16_t    ResetBuffer;                  ///< reset Temp_buffer
    uint16_t    BufferIndex;                  ///< buffer index
};
typedef struct TEMP_SETTINGS_s TEMP_SETTINGS_t;

/***********************************************************************************
 * @ingroup dev-temp-data-type
 * @struct  temp_ntc_t
 * @brief   a custom data type for ntc look up table
 **********************************************************************************/
struct TEMP_NTC_LUT_s {
    uint16_t temperature;
    uint16_t ADC_Val;
};
typedef struct TEMP_NTC_LUT_s TEMP_NTC_LUT_t;

/*********************************************************************************
 * @ingroup dev-temp-data-type
 * @var     dev_temp_data_ptr
 * @brief   dev temp data object pointer external
 * @details pointer to the Object 'dev_Temp_data_t', providing access to all control
 *          and monitoring values. To access it, add the header file to the project, and 
 *          then the pointer can be used to access the dev_temp_data_type members.
 **********************************************************************************/
extern TEMP_SETTINGS_t* devTempDataPtr;


/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/
void Dev_Temp_Initialize(void);
void Dev_Temp_Task_100ms(void);
uint16_t Dev_Temp_Get_Temperature_Celcius();
uint16_t Dev_Temp_Get_Temperature_Raw();
uint8_t Dev_Temp_Is_Over_Temperature();
#endif	/* DEV_TEMP_H */

