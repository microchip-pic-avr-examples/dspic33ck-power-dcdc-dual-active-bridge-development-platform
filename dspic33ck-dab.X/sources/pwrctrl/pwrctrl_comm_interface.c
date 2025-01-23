
/**
 * @file      pwrctrl_comm_interface.c
 * @ingroup   pwrctrl-comm   
 * @brief     Contains power control interface with communication/gui.
 */

#include "pwrctrl.h"



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
    PwrCtrl_PWM_Disable();
    
    dab.State = reference;   
    //forcing change in SM state without proper context awareness can have dangerous glitches and consequences.
    //switch off PWM signals for safe guard
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
 * @brief   API function to set the primary voltage controller reference, in reverse mode
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetVPriReference(uint16_t reference)
{    
    dab.Properties.VPriReference = reference;
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
 * @brief   API function to set the power control state forward, reverse or stop
 * @return  void
 **********************************************************************************/
void PwrCtrl_SetEnable(PWR_CTRL_CHARGE_STATE_t setVal)
{ 
    if(setVal) dab.Properties.Enable = true; 
    else dab.Properties.Enable = false; 
    
    if(dab.PowerDirection!=setVal)
    {    
        if(dab.PowerDirection)//if converter running force stop before new mode
        {
            dab.Properties.Enable = false; 
            dab.PowerDirection = PWR_CTRL_STOP;
             Dev_PwrCtrl_SetState(0);
        }
        else //proceed with starting selected mode
        {
            if(setVal) dab.Properties.Enable = true; 
            else dab.Properties.Enable = false; 
            dab.PowerDirection =  setVal;
        }
    }
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
 * @brief   API function to get the direction of configure power flow. 
 * @return  value   DAB power direction
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_PowerDir(void)
{
    return(dab.PowerDirection);
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