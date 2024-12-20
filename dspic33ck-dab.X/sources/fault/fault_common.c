
/**
 * @file      fault_common.h
 * @ingroup   fault-common 
 * @brief     Contains fault generic public functions.
 */

#include "fault_common.h"
#include "system/pins.h"
#include <stdbool.h>
#include <stddef.h>

/*******************************************************************************
 * @ingroup  fault-common
 * @brief    This function initializes the fault object data structure with the 
 *   values that required to detect a fault.  
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    threshold  sets the fault allowed limit
 * @param    hysLimit   hysteresis limit values
 * @param    thresholdCnt  threshold counter (increments when threshold limit is reached)    
 * @param    hysCnt  hysteresis counter increments when hysteresis limit is reached                    
 * @return   void
 * @details	   
 * This function initializes the fault structure with the thresholds and counter limits 
 * which are required to detect a fault.
 *******************************************************************************/
void FAULT_Init(FAULT_OBJ_T *faultInput,
                int16_t  threshold, int16_t  hysLimit,
                uint16_t thresholdCnt, uint32_t hysCnt)
{
  faultInput->Val1_Threshold = threshold;
  faultInput->Val1_Hysteresis = hysLimit;
  faultInput->CounterSet = thresholdCnt;
  faultInput->CounterReset = hysCnt;
  faultInput->FaultCode = 0U;
  faultInput->FaultActive = 0;
}

/*******************************************************************************
 * @ingroup  fault-common
 * @brief    This function initializes the fault object data structure with the 
 *   values that required to detect a fault at a particular range.  
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    thresholdMin  sets the fault allowed minimum limit
 * @param    thresholdMax  sets the fault allowed maximum limit
 * @param    hysLimitMin   minimum hysteresis limit value
 * @param    hysLimitMax   maximum hysteresis limit values
 * @param    thresholdCnt  threshold counter (increments when threshold limit is reached)    
 * @param    hysCnt  hysteresis counter increments when hysteresis limit is reached                    
 * @return   void
 * 
 * @details	 This function initializes the fault structure with the thresholds  
 *  and counter limits which are required to detect a fault.
 *******************************************************************************/
void FAULT_InitRange(FAULT_OBJ_T *faultInput,
                int16_t  thresholdMin, int16_t  hysLimitMin,
                int16_t  thresholdMax, int16_t  hysLimitMax,
                uint16_t thresholdCnt, uint32_t hysCnt)
{
  faultInput->Val1_Threshold = thresholdMin;
  faultInput->Val1_Hysteresis = hysLimitMin;
  faultInput->Val2_Threshold = thresholdMax;
  faultInput->Val2_Hysteresis = hysLimitMax;
  faultInput->CounterSet = thresholdCnt;
  faultInput->CounterReset = hysCnt;
  faultInput->FaultCode = 0U;
  faultInput->FaultActive = 0;
  faultInput->FaultLatch = 0;
}

/*******************************************************************************
 * @ingroup  fault-common
 * @brief    This function initializes/sets the fault structure with the counter limits.
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    fltThresholdCnt  threshold counter (increments when threshold limit is reached)    
 * @param    fltHysCnt  hysteresis counter increments when hysteresis limit is reached   
 * @return   void 
 * 
 * @details	 This function initializes the CounterSet, CounterReset variables 
 *  of the fault object data structure. These counter values together with 
 *  thresholds are used to set and reset the fault.
 *******************************************************************************/
void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt)
{
   faultInput->CounterSet = fltThresholdCnt;
   faultInput->CounterReset = fltHysCnt;
}

/*******************************************************************************
 * @ingroup  fault-common
 * @brief    This function initializes/sets the fault structure with the threshold limits.
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    fltThreshold  sets the fault allowed limit
 * @param    fltHysLimit   hysteresis limit values  
 * @return   void 
 * 
 * @details	  This function initializes/sets the val1_Threshold, val1_Hysteresis 
 *  variables of the fault object data structure. These threshold values together 
 *  with counter limits are used to set and reset the fault.
 *******************************************************************************/
void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit)
{
   faultInput->Val1_Threshold = fltThreshold;
   faultInput->Val1_Hysteresis = fltHysLimit;
}

/*******************************************************************************
 * @ingroup  fault-common
 * @brief    This function compares the fault source value with the threshold 
 *  limits and sets (or) resets the fault active state accordingly.
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    faultSource value being evaluated for fault limits
 * @param    callback   function executed when fault trips 
 * @return   1 when fault is active 
 * @return   0 when fault is not active
 *   
 * @details This function returns fault active state to TRUE, if input fault 
 *  source is greater than the set threshold limit for a time equivalent to 
 *  set_counter value. Also, after the fault active state becomes TRUE, if 
 *  callback function pointer is non-null, then the routine will call the 
 *  callback function which quickly shutdown the system, if needed. Also the 
 *  function returns fault active state to FALSE, if input fault source is 
 *  less than the hysteresis limit value for a time equivalent to 
 *  reset_counter value.
 *******************************************************************************/
bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
{

  if ((faultSource >= faultInput->Val1_Threshold) && (faultInput->FaultActive == false))
  {
    faultInput->FaultCounter++;

    if (faultInput->FaultCounter > faultInput->CounterSet)
    {
      faultInput->FaultLatch = true;
      
      if (callback != NULL)
      {
        callback(); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->FaultActive = true;
      faultInput->FaultCounter = 0U;
    }
  } 
  else if ((faultSource < (faultInput->Val1_Hysteresis)) && (faultInput->FaultActive == true))
  {
    faultInput->FaultCounter++;
    if (faultInput->FaultCounter > faultInput->CounterReset)
    {
      faultInput->FaultCounter = 0U;
      faultInput->FaultActive = false;
    }
  }
  else
  {
    faultInput->FaultCounter = 0U;
  }

  return (faultInput->FaultActive);

}

/*******************************************************************************
 * @ingroup  fault-common
 * @brief    This function compares the fault source value with the threshold 
 *  limits and sets (or) resets the fault active state accordingly.
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    faultSource value being evaluated for fault limits
 * @param    callback   function executed when fault trips 
 * @return   1 when fault is active 
 * @return   0 when fault is not active
 * 
 * @details This function returns fault active state to TRUE, if input fault 
 *  source is less than the set threshold limit for a time equivalent to 
 *  set_counter value. Also, after the fault active state becomes TRUE, if 
 *  callback function pointer is non-null, then the routine will call the 
 *  callback function which quickly shutdown the system, if needed. Also the 
 *  function returns fault active state to FALSE, if input fault source is 
 *  greater than the hysteresis limit value for a time equivalent to 
 *  reset_counter value.

 *******************************************************************************/
bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
{

  if ((faultSource <= faultInput->Val1_Threshold) && (faultInput->FaultActive == false))
  {
    faultInput->FaultCounter++;
    if (faultInput->FaultCounter > faultInput->CounterSet)
    {
       faultInput->FaultLatch = true;
        
      if (callback != NULL)
      {
        callback(); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->FaultActive = true;
      faultInput->FaultCounter = 0U;
    }
  }
  else if ((faultSource > (faultInput->Val1_Hysteresis)) && (faultInput->FaultActive == true))
  {
    faultInput->FaultCounter++;
    if (faultInput->FaultCounter > faultInput->CounterReset)
    {
      faultInput->FaultCounter = 0U;
      faultInput->FaultActive = false;
    }
  }
  else
  {
    faultInput->FaultCounter = 0U;
  }

  return (faultInput->FaultActive);

}

/*******************************************************************************
 * @ingroup  fault-common
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    faultBit   bit being checked by fault function
 * @param    callback   function executed when fault trips 
 * @return   1 when fault is active 
 * @return   0 when fault is not active
 * 
 * @details This function returns fault active state to TRUE, if the fault bit 
 *  is set for a time equivalent to set_counter value. Also, after the fault 
 *  active state becomes TRUE, if callback function pointer is non-null, then 
 *  the routine will call the callback function which quickly shutdown the system, 
 *  If needed. Also the function returns fault active state to FALSE, if the 
 *  fault bit is reset for a time equivalent to reset_counter value.
 *******************************************************************************/
bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback)
{
  if ((faultBit) && (faultInput->FaultActive == false))
  {
    faultInput->FaultCounter++;

    if (faultInput->FaultCounter > faultInput->CounterSet)
    {
      faultInput->FaultLatch = true;
      
      if (callback != NULL)
      {
        callback(); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->FaultActive = true;
      faultInput->FaultCounter = 0U;
    }
  }
  else if ((!faultBit) && (faultInput->FaultActive == true))
  {
    faultInput->FaultCounter++;
    if (faultInput->FaultCounter > faultInput->CounterReset)
    {
      faultInput->FaultCounter = 0U;
      faultInput->FaultActive = false;
    }
  } else
  {
    faultInput->FaultCounter = 0U;
  }

  return (faultInput->FaultActive);
}

/*******************************************************************************
 * @ingroup  fault-common
 * @param    faultInput Pointer to a fault data object of type FAULT_OBJ_T
 * @param    inputValue value being evaluated by the function
 * @param    callback   function executed when fault trips 
 * @return   1 when fault is active 
 * @return   0 when fault is not active
 * 
 * @details This function returns fault active state to TRUE, if the input value 
 *  is out of threshold limits range for a time equivalent to set_counter value.
 *  Also, after the fault active state becomes TRUE, if callback function pointer 
 *  is non-null, then the routine will call the callback function which quickly 
 *  shutdown the system, If needed. Also the function returns fault active state 
 *  to FALSE, if the input value is within the hysteresis limits range for a time 
 *  equivalent to reset_counter value.
 *******************************************************************************/
bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback)
{
    if (faultInput->FaultActive == false)
    {
        if ((inputValue <= faultInput->Val1_Threshold) ||
            (inputValue >= faultInput->Val2_Threshold))
        {
            if (++faultInput->FaultCounter >= faultInput->CounterSet)
            {
                faultInput->FaultLatch = true;
                
                if (callback != NULL)
                {
                    callback(); // Call Back function used to quickly shutdown system if needed when fault detected
                }
                faultInput->FaultActive = true;
                faultInput->FaultCounter = 0U;
            }
        }
        else
        {    
            faultInput->FaultCounter = 0U;
        }    
    }
    else
    {
        if ((inputValue > faultInput->Val1_Hysteresis) &&
            (inputValue < faultInput->Val2_Hysteresis))
        {
            if (++faultInput->FaultCounter > faultInput->CounterReset)
            {
                faultInput->FaultCounter = 0U;
                faultInput->FaultActive = false;
            }
        }
        else
        {   
            faultInput->FaultCounter = 0U;
        }    
    }
    return (faultInput->FaultActive);
}