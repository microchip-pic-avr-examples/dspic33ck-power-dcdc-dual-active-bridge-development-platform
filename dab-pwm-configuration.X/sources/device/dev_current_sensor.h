/* 
 * File:   dev_current_sensor.h
 * Author: M63731
 *
 * Created on 07 June 2024, 13:59
 */

#ifndef DEV_CURRENT_SENSOR_H
#define	DEV_CURRENT_SENSOR_H


extern void Dev_CurrentSensorOffsetCal(void);
extern uint16_t Dev_CurrentSensor_Get_CalibrationStatus(void);
extern uint16_t Dev_CurrentSensor_Get_Offset(void);
extern void Dev_CurrentSensor_Clr_Offset(void);
#endif	/* DEV_CURRENT_SENSOR_H */

