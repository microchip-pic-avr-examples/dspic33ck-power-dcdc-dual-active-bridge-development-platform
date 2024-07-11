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

#include "drv_pwrctrl_types.h"

extern CLLC_t cllc;

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_SetReference(uint16_t reference)
 * @brief   API function to set the power controller reference
 * @param   none
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Drv_PwrCtrl_SetReferenceTarget(uint16_t reference)
{    
    cllc.pwm.pgxperTarget = reference;
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
void Drv_PwrCtrl_SetEnable(bool enable)
{ 
  cllc.enable = enable; 
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
uint16_t Drv_PwrCtrl_GetAdc_Vpri(void)
{
    return(cllc.adc.vpri);
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
uint16_t Drv_PwrCtrl_GetAdc_Vsec(void)
{
    return(cllc.adc.vsec);
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
uint16_t Drv_PwrCtrl_GetAdc_Ipri_ct(void)
{
    return(cllc.adc.ipri_ct);
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
uint16_t Drv_PwrCtrl_GetAdc_Isec_ct(void)
{
    return(cllc.adc.isec_ct);
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
uint16_t Drv_PwrCtrl_GetAdc_Isec_avg(void)
{
    return(cllc.adc.isec_avg);
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
uint16_t Drv_PwrCtrl_GetAdc_Temperature(void)
{
    return(cllc.adc.temperature);
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
uint16_t Drv_PwrCtrl_GetAdc_Vrail_5V(void)
{
    return(cllc.adc.vrail_5v);
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
uint16_t Drv_PwrCtrl_Get_Pwmprd(void)
{
    return(cllc.pwm.pgxper);
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
uint16_t Drv_PwrCtrl_Get_State(void)
{
    return(cllc.state);
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
uint16_t Drv_PwrCtrl_Get_Status(void)
{
    return(cllc.status.value);
}

/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_FaultFlags(void)
 * @brief   API function to read the fault flags
 * @param   none
 * @return  fault flags
 * @details
 * API function
 **********************************************************************************/
uint16_t Drv_PwrCtrl_Get_FaultFlags(void)
{
    return(cllc.faultFlags.value);
}


/*********************************************************************************
 * @ingroup 
 * @fn      uint16_t Drv_PwrCtrl_Get_FaultFlagsLatched(void)
 * @brief   API function to read the latched version of the fault flags
 * @param   none
 * @return  latched fault flags
 * @details
 * API function
 **********************************************************************************/
uint16_t Drv_PwrCtrl_Get_FaultFlagsLatched(void)
{
    return(cllc.faultFlagsLatched.value);
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
uint16_t Drv_PwrCtrl_Get_EnableFlag(void)
{
    return(cllc.enable);
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
uint16_t Drv_PwrCtrl_Get_PwmprdTarget(void)
{
    return(cllc.pwm.pgxperTarget);
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
uint16_t Drv_PwrCtrl_Get_IloopReferenceTarget(void)
{
    return(cllc.iloop.referenceTarget);
}

/*********************************************************************************
 * @ingroup 
 * @fn      void Drv_PwrCtrl_Set_IloopReferenceTarget(uint16_t)
 * @brief   API function to set the current loop reference
 * @param   current compensator reference
 * @return  none
 * @details
 * API function
 **********************************************************************************/
void Drv_PwrCtrl_Set_IloopReferenceTarget(uint16_t referenceTarget)
{
    cllc.iloop.referenceTarget = referenceTarget;
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
uint16_t Drv_PwrCtrl_Get_IloopReference(void)
{
    return(cllc.iloop.reference);
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
uint16_t Drv_PwrCtrl_Get_IloopOutput(void)
{
    return(cllc.iloop.output);
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
uint16_t Drv_PwrCtrl_Get_IloopFeedback(void)
{
    return(cllc.iloop.feedback);
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
uint16_t Drv_PwrCtrl_Get_CurrentSensorOffset(void)
{
    return(cllc.isec_sensor_offset);
}
