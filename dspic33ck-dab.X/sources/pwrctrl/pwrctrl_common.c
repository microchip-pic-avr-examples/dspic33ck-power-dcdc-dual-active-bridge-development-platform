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

/**
 * @file      pwrctrl_common.c
 * @ingroup   pwrctrl-common   
 * @brief     Contains generic functions that handles power control ramp Up/Down 
 *  and the averaging generic function.
 */

#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h>

#include "pwrctrl.h"

/*******************************************************************************
 * @ingroup pwrctrl-common
 * @brief  Average the raw data over number of samples
 * @param  data     Pointer to averaging data object of type AVERAGING_t
 * @param  sample   data that will be averaged 
 * @return AverageValue     Returns the average value
 * 
 * @details This function averages the data over number of samples.
 *********************************************************************************/
uint16_t PwrCtrl_UpdateAverage(AVERAGING_t* data, uint16_t sample)
{
    data->Accumulator += sample;
    if(++data->Counter >= data->AveragingCount){    
        data->AverageValue = (uint16_t)(__builtin_divud(data->Accumulator, data->Counter));
        data->Accumulator = 0;
        data->Counter = 0;
    }
    return(data->AverageValue);
}

/*******************************************************************************
 * @ingroup pwrctrl-common
 * @brief   Softly increment / decrement to the set reference target
 * @param   rampUp     Pointer to start-up ramp data object of type START_UP_RAMP_t
 * @return  RampComplete Indicates if the ramp-up/down is done
 * 
 * @details This function increments or decrements the reference with its defined 
 *  step size to meet the reference target. Once the value reached the reference 
 *  target, the RampComplete will be set. 
 *********************************************************************************/
bool PwrCtrl_RampReference(START_UP_RAMP_t* rampUp)
{ 
    rampUp->RampComplete = false;
    
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

