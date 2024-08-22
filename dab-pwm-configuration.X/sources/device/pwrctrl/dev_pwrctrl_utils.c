/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */
#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h>

#include "dev_pwrctrl_typedef.h"
#include "dev_pwrctrl_utils.h" // include standard definition data types

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-public
 * @brief  
 * @param
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
uint16_t Dev_PwrCtrl_UpdateAverage(AVERAGING_t* data, uint16_t adcReading)
{
    data->Accumulator += adcReading;
    if(++data->Counter >= data->AveragingCount){    
        data->AverageValue = (uint16_t)(__builtin_divud(data->Accumulator, data->Counter));
        data->Accumulator = 0;
        data->Counter = 0;
    }
    return(data->AverageValue);
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-public
 * @brief  
 * @param
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
bool Dev_PwrCtrl_RampReference(START_UP_RAMP_t* rampUp)
{ 
  if (++rampUp->Counter > rampUp->Delay)   
  {
    rampUp->Counter = 0;
    
    if(*rampUp->ptrReference ==  *rampUp->ptrReferenceTarget)
    {
        rampUp->RampComplete = true;
    }
    else if ((*rampUp->ptrReference + rampUp->StepSize) < *rampUp->ptrReferenceTarget)
    {
      *rampUp->ptrReference += rampUp->StepSize;
    }
    else if ((*rampUp->ptrReference - rampUp->StepSize) > *rampUp->ptrReferenceTarget)
    {
      *rampUp->ptrReference -= rampUp->StepSize;
    }
    else
    {
      *rampUp->ptrReference =  *rampUp->ptrReferenceTarget;
      rampUp->RampComplete = true;
    }
  }
  
  return (rampUp->RampComplete);
}

