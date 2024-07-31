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

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetState(uint16_t reference)
{    
    dab.State = reference;
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetILoopReferenceTarget(uint16_t reference)
{    
    dab.ILoop.ReferenceTarget = reference;
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetVLoopReferenceTarget(uint16_t reference)
{    
    dab.VLoop.ReferenceTarget = reference;
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetPeriodTarget(uint16_t reference)
{    
    dab.Pwm.PBVPeriodTarget = reference;
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetPeriod(uint16_t reference)
{    
    dab.Pwm.ControlPeriod = reference;
}

void Dev_PwrCtrl_SetPhaseTarget(uint16_t reference)
{    
    dab.Pwm.PBVControlPhaseTarget = reference;
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetDeadTimeHigh(uint16_t reference)
{    
    dab.Pwm.DeadTimeHigh = reference;
}

void Dev_PwrCtrl_SetDeadTimeLow(uint16_t reference)
{    
    dab.Pwm.DeadTimeLow = reference;
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetEnable(bool enable)
 * @brief   API function to set or clear the enable control bit
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Dev_PwrCtrl_SetEnable(bool enable)
{ 
  dab.Enable = enable; 
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Vpri(void)
 * @brief   API function to read the ADC measurement of primary voltage
 * @param   none
 * @return  ADC reading of primary voltage
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vpri(void)
{
    return(dab.Adc.VPriVoltage);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Vsec(void)
 * @brief   API function to read the ADC measurement of secondary voltage
 * @param   none
 * @return  ADC reading of secondary voltage
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vsec(void)
{
    return(dab.Adc.VSecVoltage);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Ipri_ct(void)
 * @brief   API function to read the ADC measurement of primary current (as measured by CT)
 * @param   none
 * @return  ADC reading of primary current
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Ipri_ct(void)
{
    return(dab.Adc.ISensePrimary);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Isec_ct(void)
 * @brief   API function to read the read ADC measurement of secondary current (as measured by CT)
 * @param   none
 * @return  ADC reading of secondary current
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Isec_ct(void)
{
    return(dab.Adc.ISenseSecondary);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Isec_avg(void)
 * @brief   API function to read the ADC measurement of secondary current (as measured by Acienna sensor)
 * @param   none
 * @return  ADC reading of average secondary current (as measured by Acienna current sensor)
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Isec_avg(void)
{
    return(dab.Adc.ISecAverage);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Temperature(void)
 * @brief   API function to read the ADC measurement of temperature
 * @param   none
 * @return  ADC reading of temperature
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Temperature(void)
{
    return(dab.Adc.Temperature);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_GetAdc_Vrail_5V(void)
 * @brief   API function to read the ADC measurement of 5V rail
 * @param   none
 * @return  ADC reading of 5V rail
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_GetAdc_Vrail_5V(void)
{
    return(dab.Adc.VRail_5V);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_Pwmprd(void)
 * @brief   API function to read the PWMPRD setting
 * @param   none
 * @return  PWMPER setting
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Period(void)
{
    return(dab.Pwm.ControlPeriod);
}


/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_Pwmprd(void)
 * @brief   API function to read the PWMPRD setting
 * @param   none
 * @return  PWMPER setting
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_DutyCycle(void)
{
    return(dab.Pwm.ControlDutyCycle);
}


/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_Pwmprd(void)
 * @brief   API function to read the PWMPRD setting
 * @param   none
 * @return  PWMPER setting
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Phase(void)
{
    return(dab.Pwm.ControlPhase);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_State(void)
 * @brief   API function to read the state of the power controller 
 * @param   none
 * @return  state
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_State(void)
{
    return(dab.State);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_Status(void)
 * @brief   API function to read the power controller status flags
 * @param   none
 * @return  status flags
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_Status(void)
{
    return(dab.Status.value);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_EnableFlag(void)
 * @brief   API function to read enable control bit
 * @param   none
 * @return  enable control bit
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_EnableFlag(void)
{
    return(dab.Enable);
}


/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_PwmprdTarget(void)
 * @brief   API function to read the PWMPRD target setting
 * @param   none
 * @return  PWMPER target setting
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_PwmprdTarget(void)
{
    return(dab.Pwm.PBVPeriodTarget);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_IloopReferenceTarget(void)
 * @brief   API function to get the current loop reference target
 * @param   none
 * @return  current compensator reference target
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopReferenceTarget(void)
{
    return(dab.ILoop.ReferenceTarget);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_IloopReference(void)
 * @brief   API function to get the current loop reference 
 * @param   none
 * @return  current compensator reference
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopReference(void)
{
    return(dab.ILoop.Reference);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_IloopOutput(void)
 * @brief   API function to get the current loop output 
 * @param   none
 * @return  current compensator output
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopOutput(void)
{
    return(dab.ILoop.Output);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_IloopFeedback(void)
 * @brief   API function to get the current loop feedback value 
 * @param   none
 * @return  current compensator feedback 
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_IloopFeedback(void)
{
    return(dab.ILoop.Feedback);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_CurrentSensorOffset(void)
 * @brief   API function to get the current sensor offset
 * @param   none
 * @return  current sensor offset
 * @details
 * API function
 **********************************************************************************/
uint16_t Dev_PwrCtrl_Get_CurrentSensorOffset(void)
{
    return(dab.Adc.isec_sensor_offset);
}
