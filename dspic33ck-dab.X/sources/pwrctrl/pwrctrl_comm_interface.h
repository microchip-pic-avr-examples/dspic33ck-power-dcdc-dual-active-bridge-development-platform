
/**
 * @file pwrctrl_comm_interface.h     
 * @ingroup pwrctrl-comm   
 * @brief Contains public functions relevant for power control communication interface.    
 */

#ifndef DRV_PWRCTRL_API_H
#define	DRV_PWRCTRL_API_H
#include "pwrctrl_typedef.h"
// "set" APIs
void Dev_PwrCtrl_SetState(uint16_t reference);
void PwrCtrl_SetPeriodTarget(uint16_t reference);
void PwrCtrl_SetPhaseTarget(uint16_t reference);
void PwrCtrl_SetP2SPhaseTarget(uint16_t reference);
void PwrCtrl_SetEnable(PWR_CTRL_CHARGE_STATE_t setVal);
void PwrCtrl_SetVSecReference(uint16_t reference);
void PwrCtrl_SetIReference(uint16_t reference);
void PwrCtrl_SetPwrReference(uint16_t reference);

// "get" APIs
uint16_t PwrCtrl_GetPhase_P2SDegree(void);
uint16_t Dev_PwrCtrl_GetAdc_Vpri(void);
uint16_t Dev_PwrCtrl_GetAdc_Vsec(void);
uint16_t PwrCtrl_GetAdc_Ipri_ct(void);
uint16_t PwrCtrl_GetAdc_Isec_ct(void);
uint16_t PwrCtrl_GetAdc_Isec_avg(void);
uint16_t PwrCtrl_GetAdc_Temperature(void);
uint16_t PwrCtrl_GetAdc_Vrail_5V(void);
uint16_t Dev_PwrCtrl_Get_Period(void);
uint16_t Dev_PwrCtrl_Get_PwmprdTarget(void);
uint16_t Dev_PwrCtrl_Get_State(void);
uint16_t Dev_PwrCtrl_Get_PowerDir(void);
uint16_t Dev_PwrCtrl_Get_Status(void);
uint16_t Dev_PwrCtrl_Get_EnableFlag(void);
uint16_t Dev_PwrCtrl_Get_DutyCycle(void);
uint16_t Dev_PwrCtrl_Get_SecPower(void);
uint16_t Dev_PwrCtrl_GetAveraging_Vprim(void);
uint16_t Dev_PwrCtrl_GetAveraging_Vsec(void);
uint16_t Dev_PwrCtrl_GetAveraging_Isec(void);
        
#endif	/* DRV_PWRCTRL_API_H */

