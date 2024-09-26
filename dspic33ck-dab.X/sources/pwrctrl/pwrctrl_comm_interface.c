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
 * @file      pwrctrl_comm_interface.c
 * @ingroup   pwrctrl-comm   
 * @brief     Contains power control interface with communication/gui.
 */

#include "pwrctrl.h"

extern AVERAGING_t vPrimAveraging;
extern AVERAGING_t vSecAveraging;
extern AVERAGING_t iSecAveraging;

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the power control State
 * @return  value   Control phase between primary and secondary bridge multiplied 
 *  by 10 for more degrees resolution 
 **********************************************************************************/
uint16_t PwrCtrl_GetPhase_P2SDegree(void)
{
    return(dab.Pwm.ControlPhase_P2S_Degreex10);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the power control State
 * @return  void
 **********************************************************************************/
void Dev_PwrCtrl_SetState(uint16_t reference)
{    
    dab.State = reference;
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the current controller reference
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetIReference(uint16_t reference)
{    
    dab.Properties.IReference = reference;
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the secondary voltage controller reference
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetVSecReference(uint16_t reference)
{    
    dab.Properties.VSecReference = reference;
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the power controller reference
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetPwrReference(uint16_t reference)
{    
    dab.Properties.PwrReference = reference;
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the target period
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetPeriodTarget(uint16_t reference)
{    
    dab.Pwm.PBVPeriodTarget = reference;
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the target PWM phase
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetPhaseTarget(uint16_t reference)
{    
    dab.Pwm.PBVControlPhaseTarget = reference;
}
/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the target prim to sec phase which will steer other modulation
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetP2SPhaseTarget(uint16_t reference)
{    
    dab.Pwm.ControlPhase_P2S_Target = reference;
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to set the power control enable bit
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetEnable(bool enable)
{ 
  dab.Properties.Enable = enable; 
}


/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the raw ADC value for primary current transformer current
 * @return  value   Primary current transformer ADC value
 **********************************************************************************/
uint16_t PwrCtrl_GetAdc_Ipri_ct(void)
{
    return(dab.Data.ISensePrimary);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the raw ADC value for secondary current transformer current
 * @return  value   Secondary current transformer ADC value
 **********************************************************************************/
uint16_t PwrCtrl_GetAdc_Isec_ct(void)
{
    return(dab.Data.ISenseSecondary);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the raw ADC value for secondary average current
 * @return  value   Secondary current average ADC value
 **********************************************************************************/
uint16_t PwrCtrl_GetAdc_Isec_avg(void)
{
    return(dab.Data.ISecAverageRectified);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the raw ADC value for temperature
 * @return  value   temperature ADC value
 **********************************************************************************/
uint16_t PwrCtrl_GetAdc_Temperature(void)
{
    return(dab.Data.Temperature);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the raw ADC value for 5V rail
 * @return  value   5V rail ADC value
 **********************************************************************************/
uint16_t PwrCtrl_GetAdc_Vrail_5V(void)
{
    return(dab.Data.VRail_5V);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the PWM period
 * @return  value   DAB period
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Period(void)
{
    return(dab.Pwm.ControlPeriod);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the PWM duty cycle
 * @return  value   DAB duty cycle
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_DutyCycle(void)
{
    return(dab.Pwm.ControlDutyCycle);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the current state of the state machine
 * @return  value   DAB state machine
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_State(void)
{
    return(dab.State);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the power control status
 * @return  value   DAB power control status
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Status(void)
{
    return(dab.Status.value);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the state of the power control enable bit
 * @return  true    flag bit is enabled
 * @return  false   flag bit is disabled 
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_EnableFlag(void)
{
    return(dab.Properties.Enable);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the PWM period target
 * @return  value   DAB PWM period target value
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_PwmprdTarget(void)
{
    return(dab.Pwm.PBVPeriodTarget);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the secondary power
 * @return  value   Secondary Power value
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_SecPower(void)
{
    return(dab.Data.SecPower);
}
/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the average value of the sampled ADC for primary voltage
 * @return  value   Primary voltage average
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Vprim(void)
{
    return(vPrimAveraging.AverageValue);
}
/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the average value of the sampled ADC for secondary voltage
 * @return  value   Secondary voltage average
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Vsec(void)
{
    return(vSecAveraging.AverageValue);
}

/*********************************************************************************
 * @ingroup pwrctrl-comm
 * @brief   API function to get the average value of the sampled ADC for secondary current
 * @return  value   Secondary Current Average Value
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Isec(void)
{
    return(iSecAveraging.AverageValue);
}