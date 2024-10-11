
/**
 * @file    dev_temp.h
 * @brief   temperature device driver
 * @author  M70027
 * @date    04/03/2024
 */

/**
 * @file      dev_temp.h
 * @ingroup   dev-temp  
 * @brief     Contains temperature public functions and macros.
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

/*********************************************************************************
 * @ingroup dev-temp-sys-spec-macros
 * @{
 * @brief   defines the temperature threshold raw
 **********************************************************************************/
#define MAX_NUM_SAMPLES_TEMP_BUFFER                         64      ///< average calculated using 64 samples
#define MAX_NUM_SAMPLES_TEMP_BUFFER_AS_EXP_OF_TWO            6      ///< for bit shifting
/** @} */ // end of dev-temp-sys-spec-macros

/***********************************************************************************
 * @ingroup dev-temp-data-type
 * @brief   A custom data type for temperature measurement
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
 * @brief   A custom data type for ntc look up table
 **********************************************************************************/
struct TEMP_NTC_LUT_s {
    uint16_t temperature;
    int8_t temperatureCelsius;
    uint16_t ADC_Val;
};
typedef struct TEMP_NTC_LUT_s TEMP_NTC_LUT_t;

/*********************************************************************************
 * @ingroup dev-temp-data-type
 * @brief   dev temp data object pointer external
 * @details pointer to the Object 'dev_Temp_data_t', providing access to all control
 *          and monitoring values. To access it, add the header file to the project, and 
 *          then the pointer can be used to access the dev_temp_data_type members.
 **********************************************************************************/
extern TEMP_SETTINGS_t* devTempDataPtr;
extern TEMP_SETTINGS_t devTempData;

/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/
void Dev_Temp_Initialize(void);
void Dev_Temp_Task_100ms(void);
int8_t Dev_Temp_Get_Temperature_Celcius();
uint16_t Dev_Temp_AverageValue(void);
uint8_t Dev_Temp_Is_Over_Temperature();
void Dev_Temp_Get_ADC_Sample(void); 
#endif	/* DEV_TEMP_H */

