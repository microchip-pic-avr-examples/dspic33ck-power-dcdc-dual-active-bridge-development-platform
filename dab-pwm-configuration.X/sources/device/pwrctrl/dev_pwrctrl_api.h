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

#ifndef DRV_PWRCTRL_API_H
#define	DRV_PWRCTRL_API_H

// "set" APIs
void Dev_PwrCtrl_SetState(uint16_t reference);
void Dev_PwrCtrl_SetReferenceTarget(uint16_t reference);
void Dev_PwrCtrl_SetPeriod(uint16_t reference);
void Dev_PwrCtrl_SetPhaseTarget(uint16_t reference);
void Dev_PwrCtrl_SetEnable(bool enable);
void Dev_PwrCtrl_Set_IloopReferenceTarget(uint16_t referenceTarget);
void Dev_PwrCtrl_SetDeadTimeHigh(uint16_t reference);
void Dev_PwrCtrl_SetDeadTimeLow(uint16_t reference);

// "get" APIs
uint16_t Dev_PwrCtrl_GetAdc_Vpri(void);
uint16_t Dev_PwrCtrl_GetAdc_Vsec(void);
uint16_t Dev_PwrCtrl_GetAdc_Ipri_ct(void);
uint16_t Dev_PwrCtrl_GetAdc_Isec_ct(void);
uint16_t Dev_PwrCtrl_GetAdc_Isec_avg(void);
uint16_t Dev_PwrCtrl_GetAdc_Temperature(void);
uint16_t Dev_PwrCtrl_GetAdc_Vrail_5V(void);
uint16_t Dev_PwrCtrl_Get_Period(void);
uint16_t Dev_PwrCtrl_Get_PwmprdTarget(void);
uint16_t Dev_PwrCtrl_Get_State(void);
uint16_t Dev_PwrCtrl_Get_Status(void);
uint16_t Dev_PwrCtrl_Get_EnableFlag(void);
uint16_t Dev_PwrCtrl_Get_IloopReferenceTarget(void);
uint16_t Dev_PwrCtrl_Get_IloopReference(void);
uint16_t Dev_PwrCtrl_Get_IloopOutput(void);
uint16_t Dev_PwrCtrl_Get_IloopFeedback(void);
uint16_t Dev_PwrCtrl_Get_CurrentSensorOffset(void);
uint16_t Dev_PwrCtrl_Get_DutyCycle(void);
uint16_t Dev_PwrCtrl_Get_Phase(void);

#endif	/* DRV_PWRCTRL_API_H */

