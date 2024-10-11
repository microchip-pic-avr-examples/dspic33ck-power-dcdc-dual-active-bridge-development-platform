/* 
 * File:   pwrctrl_common.h
 * Author: M15690
 *
 * Created on July 11, 2024, 9:20 PM
 */

/**
 * @file pwrctrl_common.h     
 * @ingroup pwrctrl-common
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

extern uint16_t PwrCtrl_UpdateAverage(AVERAGING_t* data, uint16_t adcReading);
extern bool PwrCtrl_RampReference(START_UP_RAMP_t* rampUp);

#endif	/* DEV_PWRCTRL_UTILS_H */

