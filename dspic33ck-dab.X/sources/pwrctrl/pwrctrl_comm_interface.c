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
 * @file      dev_pwrctrl_comm_interface.c
 * @ingroup   dev-pwrctrl-comm   
 * @brief     Contains power control interface with communication/gui.
 */

#include "pwrctrl.h"

extern AVERAGING_t VprimAveraging;
extern AVERAGING_t VsecAveraging;
extern AVERAGING_t IsecAveraging;

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the power control State
 * @return  value   Control phase between primary and secondary bridge multiplied 
 *  by 10 for more degrees resolution 
 **********************************************************************************/
uint16_t PwrCtrl_GetPhase_P2SDegree(void)
{
    return(dab.Pwm.ControlPhase_P2S_Degreex10);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get some unit test debug purpose value. Do not use otherwise
 * @return  true Fault has been detected
 * @return  false Fault is not detected
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_DbgValue(void)
{
    dab.dbg=dab.Fault.FaultDetected;
    return(dab.dbg);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the power control State
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetState(uint16_t reference)
{    
    dab.State = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the current controller reference
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetIReference(uint16_t reference)
{    
    dab.Properties.IReference = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the secondary voltage controller reference
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetVSecReference(uint16_t reference)
{    
    dab.Properties.VSecReference = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the power controller reference
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPwrReference(uint16_t reference)
{    
    dab.Properties.PwrReference = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the target period
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPeriodTarget(uint16_t reference)
{    
    dab.Pwm.PBVPeriodTarget = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the target PWM phase
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPhaseTarget(uint16_t reference)
{    
    dab.Pwm.PBVControlPhaseTarget = reference;
}
/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the target prim to sec phase which will steer other modulation
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetP2SPhaseTarget(uint16_t reference)
{    
    dab.Pwm.ControlPhase_P2S_Target = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to set the power control enable bit
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetEnable(bool enable)
{ 
  dab.Properties.Enable = enable; 
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for primary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vpri(void)
{
    return(dab.Data.VPriVoltage);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for secondary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vsec(void)
{
    return(dab.Data.VSecVoltage);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for primary current transformer current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Ipri_ct(void)
{
    return(dab.Data.ISensePrimary);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for secondary current transformer current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Isec_ct(void)
{
    return(dab.Data.ISenseSecondary);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for secondary average current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Isec_avg(void)
{
    return(dab.Data.ISecAverageRectified);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for temperature
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Temperature(void)
{
    return(dab.Data.Temperature);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the raw ADC value for 5V rail
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vrail_5V(void)
{
    return(dab.Data.VRail_5V);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the PWM period
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Period(void)
{
    return(dab.Pwm.ControlPeriod);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the PWM duty cycle
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_DutyCycle(void)
{
    return(dab.Pwm.ControlDutyCycle);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the current state of the state machine
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_State(void)
{
    return(dab.State);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the power control status
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Status(void)
{
    return(dab.Status.value);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the state of the power control enable bit
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_EnableFlag(void)
{
    return(dab.Properties.Enable);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the PWM period target
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_PwmprdTarget(void)
{
    return(dab.Pwm.PBVPeriodTarget);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the secondary power
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_SecPower(void)
{
    return(dab.Data.SecPower);
}
/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the average value of the sampled ADC for primary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Vprim(void)
{
    return(VprimAveraging.AverageValue);
}
/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the average value of the sampled ADC for secondary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Vsec(void)
{
    return(VsecAveraging.AverageValue);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-comm
 * @brief   API function to get the average value of the sampled ADC for secondary current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Isec(void)
{
    return(IsecAveraging.AverageValue);
}