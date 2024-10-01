
/*
 * File:   dev_Temp.c
 * Author: M70027
 *
 * Created on Jan 17, 2024, 13:30 PM
 */

/**
 * @file      dev_temp.h
 * @ingroup   dev-temp  
 * @brief     Contains temperature initialization and execution functions.
 */

#include "dev_temp.h"
#include <stdlib.h>

#include "config/hardware.h"
#include "config/macros.h"
#include "pwrctrl/pwrctrl_comm_interface.h"
#include "pwrctrl/pwrctrl.h"

/*******************************************************************************
 * @ingroup dev-temp
 * @brief Data Object of temperature settings
 * 
 * @details The 'devTempData' data object holds the settings for temperature
 *  sensor.
 *******************************************************************************/
TEMP_SETTINGS_t devTempData;
TEMP_SETTINGS_t* devTempDataPtr = &devTempData;

// Private Function Call Prototypes
static uint16_t Average_Temp_ADC_Samples(void);
static uint16_t Temp_Calculate_Average(uint16_t * buffer, uint16_t size);


/***********************************************************************************
 * @ingroup dev-temp-public-functions
 * @fn      Dev_Temp_Init
 * @param   void
 * @return  nothing
 * @brief   this function initializes to the dev_temp_data object and fault objects
 * @details
 *   
 **********************************************************************************/
/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Initializes the temperature data structure
 * @return void
 * 
 * @details This function initializes the temperature data structure.
 *********************************************************************************/
void Dev_Temp_Initialize(void){
    devTempData.OverTemperatureFlag = 0;        ///< Over temperature flag
    devTempData.SensorLost = 0;                 ///< if Sensor is present  
    devTempData.AdcReading = 0;                 ///< temperature raw
    devTempData.AdcAverage = 0;                 ///< temperature raw
    devTempData.TemperatureCelcius = 0;         ///< temperature celcius.
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  This converts the raw values to temperature celcius as per device 
 *          lookup table
 * @return void
 * 
 * @details This function takes in the current adc values and get the closest 
 *  high temperature value in the lookup table. i.e. if the averaged value is 
 *  3195, it will return the temperature value corresponding to 3079 i.e. 65. 
 *  This value is offset by 40 degrees to avoid working with negative.
 *********************************************************************************/
int8_t Dev_Temp_Get_Temperature_Celcius(void){
    
    TEMP_NTC_LUT_t point0;
        
    devTempData.AdcAverage = Average_Temp_ADC_Samples();
    
    if (devTempData.AdcAverage == 0)  return 0;
    
    if (devTempData.BufferFull)
        point0.temperature =  (__builtin_mulsu(TEMPERATURE_FACTOR, devTempData.AdcAverage) >> 15) + TEMPERATURE_OFFSET;
    
    point0.temperatureCelsius = point0.temperature - TEMPERATURE_PBV_OFFSET_CELCIUS;
    
    return point0.temperatureCelsius;
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Returns the status of the Over Temperature flag
 * @return true Over temperature is detected
 * @return false Over temperature is not detected
 * 
 * @details This function returns the status of the Over Temperature flag.
 *********************************************************************************/
uint8_t Dev_Temp_Is_Over_Temperature(void) {
    return devTempData.OverTemperatureFlag;
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Gets the temperature ADC sample
 * @return void
 * 
 * @details This function gets the temperature ADC sample and adds it to the 
 *  temperature buffer for averaging use. 
 *********************************************************************************/
void Dev_Temp_Get_ADC_Sample(void) 
{
    // making sure that array does not go out of bounds. 
    if (devTempData.BufferIndex >= (MAX_NUM_SAMPLES_TEMP_BUFFER - 1))   ///< store last 128 values and then average
    {      
        devTempData.BufferIndex = 0;
        devTempData.BufferFull = 1;    ///< flag to monitor if full then do averaging.
    }
        
    // use API from power controller to read temperature
    devTempData.AdcReading = PwrCtrl_GetAdc_Temperature();            
    devTempData.TempBuffer[devTempData.BufferIndex++] = devTempData.AdcReading;
    
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Returns the temperature average value
 * @return void
 * 
 * @details This function returns the average result of the temperature ADC samples. 
 *********************************************************************************/
static uint16_t Average_Temp_ADC_Samples(void) {
    if (devTempData.BufferFull)
        return Temp_Calculate_Average(devTempData.TempBuffer, MAX_NUM_SAMPLES_TEMP_BUFFER);
    else 
        return 0;
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Averages the temperature ADC samples
 * @return void
 * 
 * @details This function averages the temperature ADC samples.
 *********************************************************************************/
static uint16_t Temp_Calculate_Average(uint16_t * buffer, uint16_t length) {
    uint16_t index = 0;
    uint32_t sum = 0;
    for (index = 0; index <length; index++)
        sum +=buffer[index];
    return (sum >> MAX_NUM_SAMPLES_TEMP_BUFFER_AS_EXP_OF_TWO);
}

/*******************************************************************************
 * end of file
 *******************************************************************************/