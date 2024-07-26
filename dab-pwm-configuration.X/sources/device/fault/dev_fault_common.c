
#include "dev_fault_common.h"
#include "system/pins.h"
#include <stdbool.h>
#include <stddef.h>


void FAULT_Init(FAULT_OBJ_T *faultInput,
                int16_t  threshold, int16_t  hysLimit,
                uint16_t thresholdCnt, uint32_t hysCnt)
{
  faultInput->val1_Threshold = threshold;
  faultInput->val1_Hysteresis = hysLimit;
  faultInput->CounterSet = thresholdCnt;
  faultInput->CounterReset = hysCnt;
  faultInput->fltCode = 0U;
  faultInput->FaultActive = 0;
}


void FAULT_InitRange(FAULT_OBJ_T *faultInput,
                int16_t  thresholdMin, int16_t  hysLimitMin,
                int16_t  thresholdMax, int16_t  hysLimitMax,
                uint16_t thresholdCnt, uint32_t hysCnt)
{
  faultInput->val1_Threshold = thresholdMin;
  faultInput->val1_Hysteresis = hysLimitMin;
  faultInput->val2_Threshold = thresholdMax;
  faultInput->val2_Hysteresis = hysLimitMax;
  faultInput->CounterSet = thresholdCnt;
  faultInput->CounterReset = hysCnt;
  faultInput->fltCode = 0U;
  faultInput->FaultActive = 0;
  faultInput->FaultLatch = 0;
}


void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt)
{
   faultInput->CounterSet = fltThresholdCnt;
   faultInput->CounterReset = fltHysCnt;
}


void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit)
{
   faultInput->val1_Threshold = fltThreshold;
   faultInput->val1_Hysteresis = fltHysLimit;
}


bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
{

  if ((faultSource >= faultInput->val1_Threshold) && (faultInput->FaultActive == false))
  {
    faultInput->fltCounter++;

    if (faultInput->fltCounter > faultInput->CounterSet)
    {
      faultInput->FaultLatch = true;
      
      if (callback != NULL)
      {
        callback(); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->FaultActive = true;
      faultInput->fltCounter = 0U;
    }
  } 
  else if ((faultSource < (faultInput->val1_Hysteresis)) && (faultInput->FaultActive == true))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterReset)
    {
      faultInput->fltCounter = 0U;
      faultInput->FaultActive = false;
    }
  }
  else
  {
    faultInput->fltCounter = 0U;
  }

  return (faultInput->FaultActive);

}


bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
{

  if ((faultSource <= faultInput->val1_Threshold) && (faultInput->FaultActive == false))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterSet)
    {
       faultInput->FaultLatch = true;
        
      if (callback != NULL)
      {
        callback(); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->FaultActive = true;
      faultInput->fltCounter = 0U;
    }
  }
  else if ((faultSource > (faultInput->val1_Hysteresis)) && (faultInput->FaultActive == true))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterReset)
    {
      faultInput->fltCounter = 0U;
      faultInput->FaultActive = false;
    }
  }
  else
  {
    faultInput->fltCounter = 0U;
  }

  return (faultInput->FaultActive);

}

bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback)
{
  if ((faultBit) && (faultInput->FaultActive == false))
  {
    faultInput->fltCounter++;

    if (faultInput->fltCounter > faultInput->CounterSet)
    {
      faultInput->FaultLatch = true;
      
      if (callback != NULL)
      {
        callback(); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->FaultActive = true;
      faultInput->fltCounter = 0U;
    }
  }
  else if ((!faultBit) && (faultInput->FaultActive == true))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterReset)
    {
      faultInput->fltCounter = 0U;
      faultInput->FaultActive = false;
    }
  } else
  {
    faultInput->fltCounter = 0U;
  }

  return (faultInput->FaultActive);
}


bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback)
{
    if (faultInput->FaultActive == false)
    {
        if ((inputValue <= faultInput->val1_Threshold) ||
            (inputValue >= faultInput->val2_Threshold))
        {
            if (++faultInput->fltCounter >= faultInput->CounterSet)
            {
                faultInput->FaultLatch = true;
                
                if (callback != NULL)
                {
                    callback(); // Call Back function used to quickly shutdown system if needed when fault detected
                }
                faultInput->FaultActive = true;
                faultInput->fltCounter = 0U;
            }
        }
        else
        {    
            faultInput->fltCounter = 0U;
        }    
    }
    else
    {
        if ((inputValue > faultInput->val1_Hysteresis) &&
            (inputValue < faultInput->val2_Hysteresis))
        {
            if (++faultInput->fltCounter > faultInput->CounterReset)
            {
                faultInput->fltCounter = 0U;
                faultInput->FaultActive = false;
            }
        }
        else
        {   
            faultInput->fltCounter = 0U;
        }    
    }
    return (faultInput->FaultActive);
}