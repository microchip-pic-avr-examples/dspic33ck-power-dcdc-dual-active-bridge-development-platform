

/**
 * @file      dev_current_sensor.c
 * @ingroup   dev-current-sensor   
 * @brief     Contains current sensor initialization and calibration evaluation. 
 */

#include <stdbool.h>
#include "config/macros.h"
#include "adc/adc1.h"

#define SENSOR_OFFSET_NUM_MEASUREMENTS      (1000) ///< Number of measurement to calculate the average over

/***********************************************************************************
 * @ingroup dev-current-sensor
 * @brief Stores data related to sensor calibration, including data related to  sensor
 * offset 
 * @details This data structure contains the collection of current sensor
 *  properties that the user can initialize and monitor.  
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

/*******************************************************************************
 * @ingroup dev-current-sensor
 * @brief Data Object of sensor offset calibration
 * 
 * @details The 'isecAvgCurrentSensor' data object holds the averaging parameter
 *  of the sensor offset calibration.
 *******************************************************************************/
SENSOR_OFFSET_CAL_t isecAvgCurrentSensor = {.CalibrationComplete = false,
                                               .Accumulator = 0,
                                               .MeasurementCounter = 0,
                                               .LimitHigh = ISEC_AVG_SENSOR_OFFSET_LIMIT_HIGH,
                                               .LimitLow = ISEC_AVG_SENSOR_OFFSET_LIMIT_LOW};

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @brief   Measure the sensor offset
 * @param   sensor      pointer to object of type SENSOR_OFFSET_CAL_t 
 * @param   adcReading  most recent ADC reading
 * @return  void
 * 
 * @details This function takes number of samples of adc sample for averaging
 *  before acquiring the sensor offset value and checks if it is in the acceptable 
 *  range before setting the calibration bit complete. 
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
 * @ingroup dev-current-sensor
 * @brief   Measures the currents sensor offset 
 * @return  void
 * 
 * @details This function continuously evaluate the current sensor ADC value 
 *  until the calibration has been completed. 
 **********************************************************************************/
void Dev_CurrentSensorOffsetCal(void)
{
    if (!isecAvgCurrentSensor.CalibrationComplete)
    {
        Dev_SensorOffsetCal(&isecAvgCurrentSensor, ADC1_ConversionResultGet(ISEC_AVG));
    }
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @brief   API function to get the calibration status
 * @return  true    calibration complete
 * @return  false   calibration is not complete
 **********************************************************************************/
uint16_t Dev_CurrentSensor_Get_CalibrationStatus(void)
{
    return(isecAvgCurrentSensor.CalibrationComplete);
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @brief   API function to get the sensor offset
 * @return  value   current sensor offset value
 **********************************************************************************/
uint16_t Dev_CurrentSensor_Get_Offset(void)
{
    return(isecAvgCurrentSensor.Offset); 
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @brief   API function to clear the sensor offset
 * @return  none
 **********************************************************************************/
void Dev_CurrentSensor_Clr_Offset(void)
{
    isecAvgCurrentSensor.CalibrationComplete=false; 
}