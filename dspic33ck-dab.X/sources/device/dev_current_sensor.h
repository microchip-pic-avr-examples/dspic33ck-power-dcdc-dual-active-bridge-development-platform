
/**
 * @file      dev_current_sensor.h
 * @ingroup   dev-current-sensor   
 * @brief     Contains current sensor public functions.
 */

#ifndef DEV_CURRENT_SENSOR_H
#define	DEV_CURRENT_SENSOR_H


void Dev_CurrentSensorOffsetCal(void);
void Dev_CurrentSensor_Clr_Offset(void);

extern uint16_t Dev_CurrentSensor_Get_CalibrationStatus(void);
extern uint16_t Dev_CurrentSensor_Get_Offset(void);

#endif	/* DEV_CURRENT_SENSOR_H */

