/* 
 * File:   dev_pwrctrl_utils.h
 * Author: M15690
 *
 * Created on July 11, 2024, 9:20 PM
 */

/**
 * @file dev_pwrctrl_utils.h     
 * @ingroup dev-pwrctrl-utils   
 * @brief Contains the power control reusable functions.    
 */

#ifndef DEV_PWRCTRL_UTILS_H
#define	DEV_PWRCTRL_UTILS_H

struct AVERAGING_s
{  
  uint32_t  Accumulator; 
  uint16_t  Counter; 
  uint16_t  AveragingCount;
  uint16_t  AverageValue;
};
typedef struct AVERAGING_s AVERAGING_t;

extern uint16_t Dev_PwrCtrl_UpdateAverage(AVERAGING_t* data, uint16_t adcReading);
extern bool Dev_PwrCtrl_RampReference(START_UP_RAMP_t* rampUp);

#endif	/* DEV_PWRCTRL_UTILS_H */

