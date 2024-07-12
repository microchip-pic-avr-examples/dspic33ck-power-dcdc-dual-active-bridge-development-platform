/* 
 * File:   dev_pwrctrl_utils.h
 * Author: M15690
 *
 * Created on July 11, 2024, 9:20 PM
 */

#ifndef DEV_PWRCTRL_UTILS_H
#define	DEV_PWRCTRL_UTILS_H


extern bool Drv_PwrCtrl_RampReference(uint16_t* ptr_reference, 
                               uint16_t* ptr_referenceTarget, 
                               uint16_t step, 
                               uint16_t delay);

#endif	/* DEV_PWRCTRL_UTILS_H */

