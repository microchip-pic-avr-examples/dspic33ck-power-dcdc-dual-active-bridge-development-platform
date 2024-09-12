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

#include <stdbool.h>
#include "config/macros.h"
#include "adc/adc1.h"

// number of measurement to calculate the average over
#define SENSOR_OFFSET_NUM_MEASUREMENTS      (1000)

/***********************************************************************************
 * @ingroup 
 * @struct SENSOR_OFFSET_CAL_s
 * @extends 
 * @brief stores data related to sensor calibration, including data related to  sensor
 * offset 
 * @details
 *  
 **********************************************************************************/
struct SENSOR_OFFSET_CAL_s
{  
  uint32_t  Accumulator; ///< used for sensor offset measurement at startup
  uint16_t  MeasurementCounter; ///< used for  sensor offset measurement at startup
  uint16_t  Offset; ///< measured offset 
  bool      CalibrationComplete; ///< set to true if the sensor is calibrated
  uint16_t  LimitHigh;  ///< upper bound of what offset should be, offset needs to be < limitHigh and > limitLow for sensorCalibrated = true
  uint16_t  LimitLow; ///< low bound of what offset should be
};
typedef struct SENSOR_OFFSET_CAL_s SENSOR_OFFSET_CAL_t;

// create object to store current sensor 
SENSOR_OFFSET_CAL_t isecAvgCurrentSensor = {.CalibrationComplete = false,
                                               .Accumulator = 0,
                                               .MeasurementCounter = 0,
                                               .LimitHigh = ISEC_AVG_SENSOR_OFFSET_LIMIT_HIGH,
                                               .LimitLow = ISEC_AVG_SENSOR_OFFSET_LIMIT_LOW};

/*********************************************************************************
 * @ingroup 
 * @fn      void Dev_SensorOffsetCal(SENSOR_OFFSET_CAL_t* sensor, uint16_t adcReading)
 * @brief   measure offset of a sensor
 * @param   pointer to object of type SENSOR_OFFSET_CAL_t (store all data related to sensor offset), most recent ADC reading
 * @return  
 * @details
 * only using pointer arugment here as may have more than 1 sensor on a system, makes the function more generic
 **********************************************************************************/
static void __inline__ Dev_SensorOffsetCal(SENSOR_OFFSET_CAL_t* sensor, uint16_t adcReading)
{
    // current sensor offset is not measured
    sensor->Accumulator += adcReading;
    if (++sensor->MeasurementCounter >= SENSOR_OFFSET_NUM_MEASUREMENTS)
    {
        // average to determine the offset      
        uint16_t offset = (uint16_t) (__builtin_divud(sensor->Accumulator, sensor->MeasurementCounter));

        sensor->Accumulator = 0;
        sensor->MeasurementCounter = 0;

        // check if result is within acceptable limits
        if ((offset < sensor->LimitHigh) && (offset > sensor->LimitLow))
        {
            // measured offset is reasonable. Update stored value and exit
            sensor->Offset = offset;
            sensor->CalibrationComplete = true;
        }
    }    
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_CurrentSensorOffsetCal()
 * @brief   measure offset of current sensors
 * @param   None
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_CurrentSensorOffsetCal(void)
{
    if (!isecAvgCurrentSensor.CalibrationComplete)
    {
        Dev_SensorOffsetCal(&isecAvgCurrentSensor, ADC1_ConversionResultGet(ISEC_AVG));
    }
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Dev_CurrentSensor_Get_CalibrationStatus()
 * @brief   API function to read calibration status of current sensor
 * @param   none
 * @return  1 if calibration is complete, 0 otherwise
 * @details
 * API function
 **********************************************************************************/ 
uint16_t Dev_CurrentSensor_Get_CalibrationStatus(void)
{
    return(isecAvgCurrentSensor.CalibrationComplete);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Dev_CurrentSensor_Get_Offset()
 * @brief   API function to read the current sensor offset
 * @param   none
 * @return  ADC code corresponding to current sensor offset
 * @details
 * API function
 **********************************************************************************/ 
uint16_t Dev_CurrentSensor_Get_Offset(void)
{
    return(isecAvgCurrentSensor.Offset); 
}
/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Dev_CurrentSensor_Clr_Offset()
 * @brief   API function to clear the current sensor offset calibration flag
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/ 
void Dev_CurrentSensor_Clr_Offset(void)
{
    isecAvgCurrentSensor.CalibrationComplete=false; 
}