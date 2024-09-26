
/*
 * File:   dev_Temp.c
 * Author: M70027
 *
 * Created on Jan 17, 2024, 13:30 PM
 */

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

/**
 * @file      dev_temp.h
 * @ingroup   dev-temp  
 * @brief     Contains temperature initialization and execution functions.
 */

#include "dev_temp.h"
#include <stdlib.h>

#include "fault/fault_common.h"
#include "pwrctrl/pwrctrl_comm_interface.h"
#include "pwrctrl/pwrctrl.h"

/*********************************************************************************
 * @ingroup dev-temp
 * @brief   Lookup table for NTC device measurement
 * @details This array contains the loock-up table for a particular temperature
 *  device. As an example: 
 *          NTC device on the DAB board is NTCALUG02A103G. The resistance vs 
 *          temperature value can be gotten through data sheet. The voltage 
 *          divider has R1 = 3.3K, and NTC is R2. The ADC reference is also 
 *          3.3 volt. From this, the resistance values from the data sheet are 
 *          transformed into ADC values that would be sampled corresponding to 
 *          the temperature. 
 *          from -40 t0 125 . temperature offset by 40 to avoid negative
 **********************************************************************************/
static TEMP_NTC_LUT_t ntcLookUpTable[34] = 
{
    {.ADC_Val = 4056,   .temperature = 0  },
    {.ADC_Val = 4040,   .temperature = 5  },
    {.ADC_Val = 4020,   .temperature = 10 },
    {.ADC_Val = 3994,   .temperature = 15 },
    {.ADC_Val = 3961,   .temperature = 20 },
    {.ADC_Val = 3918,   .temperature = 25 },
    {.ADC_Val = 3865,   .temperature = 30 },
    {.ADC_Val = 3799,   .temperature = 35 },
    {.ADC_Val = 3719,   .temperature = 40 },
    {.ADC_Val = 3624,   .temperature = 45 },
    {.ADC_Val = 3513,   .temperature = 50 },
    {.ADC_Val = 3385,   .temperature = 55 },
    {.ADC_Val = 3240,   .temperature = 60 },
    {.ADC_Val = 3079,   .temperature = 65 },
    {.ADC_Val = 2905,   .temperature = 70 },
    {.ADC_Val = 2721,   .temperature = 75 },
    {.ADC_Val = 2528,   .temperature = 80 },
    {.ADC_Val = 2332,   .temperature = 85 },
    {.ADC_Val = 2136,   .temperature = 90 },
    {.ADC_Val = 1944,   .temperature = 95 },
    {.ADC_Val = 1759,   .temperature = 100},
    {.ADC_Val = 1583,   .temperature = 105},
    {.ADC_Val = 1418,   .temperature = 110},
    {.ADC_Val = 1265,   .temperature = 115},
    {.ADC_Val = 1126,   .temperature = 120},
    {.ADC_Val = 1000,   .temperature = 125},
    {.ADC_Val = 886,    .temperature = 130},
    {.ADC_Val = 785,    .temperature = 135},
    {.ADC_Val = 694,    .temperature = 140},
    {.ADC_Val = 614,    .temperature = 145},
    {.ADC_Val = 544,    .temperature = 150},
    {.ADC_Val = 482,    .temperature = 155},
    {.ADC_Val = 427,    .temperature = 160},
    {.ADC_Val = 379,    .temperature = 165},
};

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
    
    uint8_t index = 0;
    
    TEMP_NTC_LUT_t point0;
        
    devTempData.AdcAverage = Average_Temp_ADC_Samples();
    
    if (devTempData.AdcAverage == 0)  return 0;
    
    if (devTempData.AdcAverage > 4056) 
        return ntcLookUpTable[0].temperature - 40; 
    
    if (devTempData.AdcAverage < 379) 
        return ntcLookUpTable[33].temperature - 40;
    
    for (index = 0; index < 32; index++) {
        if (devTempData.AdcAverage > ntcLookUpTable[index].ADC_Val) {
            point0 = ntcLookUpTable[index];           
            break;
        } 
    }
    point0.temperatureCelsius=point0.temperature-40;
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