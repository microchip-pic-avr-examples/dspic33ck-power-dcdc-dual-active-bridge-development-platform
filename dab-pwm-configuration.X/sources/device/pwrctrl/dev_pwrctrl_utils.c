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
#include <stddef.h> // include standard definition data types

/*********************************************************************************
 * @ingroup 
 * @fn      bool Drv_PwrCtrl_RampReference(uint16_t* reference, uint16_t* referenceTarget, uint16_t step, uint16_t delay)
 * @brief   ramp reference
 * @param   none
 * @return  none
 * @details
 * ramp compensator reference linearly
 *
 **********************************************************************************/
bool Dev_PwrCtrl_RampReference(uint16_t* ptr_reference, 
                               uint16_t* ptr_referenceTarget, 
                               uint16_t step, 
                               uint16_t delay)
{
  static uint16_t SoftstartCounter = 0; 
  bool ramp_complete = false;
  
  if (++SoftstartCounter > delay)   
  {
    SoftstartCounter = 0;
    //ToDo: Discuss to Cormac floor in the code
    if(*ptr_reference ==  *ptr_referenceTarget)
        ramp_complete = true;
    
    else if ((*ptr_reference + step) < *ptr_referenceTarget)
    {
      *ptr_reference += step;
    }
    else if ((*ptr_reference - step) > *ptr_referenceTarget)
    {
      *ptr_reference -= step;
    }
    else
    {
      // reference close enough to target, move on
      *ptr_reference = *ptr_referenceTarget;
      ramp_complete = true; // reference is at target already
    }
  }
  
  return (ramp_complete);
}


