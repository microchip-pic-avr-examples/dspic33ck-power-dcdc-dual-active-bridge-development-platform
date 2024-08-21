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

#include "dev_pwrctrl.h"
#include "dev_pwrctrl_utils.h"

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the power control State
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetState(uint16_t reference)
{    
    dab.State = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the current controller reference
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetIReference(uint16_t reference)
{    
    dab.Properties.IReference = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the secondary voltage controller reference
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetVSecReference(uint16_t reference)
{    
    dab.Properties.VSecReference = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the power controller reference
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPwrReference(uint16_t reference)
{    
    dab.Properties.PwrReference = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the target period
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPeriodTarget(uint16_t reference)
{    
    dab.Pwm.PBVPeriodTarget = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the control period
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPeriod(uint16_t reference)
{    
    dab.Pwm.ControlPeriod = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the target PWM phase
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetPhaseTarget(uint16_t reference)
{    
    dab.Pwm.PBVControlPhaseTarget = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the dead time high
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetDeadTimeHigh(uint16_t reference)
{    
    dab.Pwm.DeadTimeHigh = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the dead time low
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetDeadTimeLow(uint16_t reference)
{    
    dab.Pwm.DeadTimeLow = reference;
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to set the power control enable bit
 * @return  none
 **********************************************************************************/
void Dev_PwrCtrl_SetEnable(bool enable)
{ 
  dab.Properties.Enable = enable; 
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for primary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vpri(void)
{
    return(dab.Data.VPriVoltage);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for secondary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vsec(void)
{
    return(dab.Data.VSecVoltage);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for primary current transformer current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Ipri_ct(void)
{
    return(dab.Data.ISensePrimary);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for secondary current transformer current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Isec_ct(void)
{
    return(dab.Data.ISenseSecondary);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for secondary average current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Isec_avg(void)
{
    return(dab.Data.ISecAverageRectified);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for temperature
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Temperature(void)
{
    return(dab.Data.Temperature);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the raw ADC value for 5V rail
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vrail_5V(void)
{
    return(dab.Data.VRail_5V);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the PWM period
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Period(void)
{
    return(dab.Pwm.ControlPeriod);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the PWM duty cycle
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_DutyCycle(void)
{
    return(dab.Pwm.ControlDutyCycle);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the PWM control phase
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Phase(void)
{
    return(dab.Pwm.ControlPhase);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the current state of the state machine
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_State(void)
{
    return(dab.State);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the power control status
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Status(void)
{
    return(dab.Status.value);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the state of the power control enable bit
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_EnableFlag(void)
{
    return(dab.Properties.Enable);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the PWM period target
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_PwmprdTarget(void)
{
    return(dab.Pwm.PBVPeriodTarget);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the current reference
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IReference(void)
{
    return(dab.Properties.IReference);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the current loop control reference
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopReference(void)
{
    return(dab.ILoop.Reference);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the current loop control output
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopOutput(void)
{
    return(dab.ILoop.Output);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the current loop control feedback
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopFeedback(void)
{
    return(dab.ILoop.Feedback);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the current sensor offset
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_CurrentSensorOffset(void)
{
    return(dab.Data.ISecSensorOffset);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the secondary power
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_SecPower(void)
{
    return(dab.Data.SecPower);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the average value of the sampled ADC for secondary voltage
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Vsec(void)
{
    return(VsecAveraging.AverageValue);
}

/*********************************************************************************
 * @ingroup dev-pwrctrl-api-public
 * @brief   API function to get the average value of the sampled ADC for secondary current
 * @return  none
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAveraging_Isec(void)
{
    return(IsecAveraging.AverageValue);
}