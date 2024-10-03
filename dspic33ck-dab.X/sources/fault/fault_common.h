
/**
 * @file      fault_common.h
 * @ingroup   fault-common 
 * @brief     Contains fault generic public functions.
 */

#ifndef FAULT_COMMON_H
#define FAULT_COMMON_H

#include <stdint.h>
#include <stdbool.h>

/***********************************************************************************
 * @ingroup  fault-common
 * @brief    The Fault object data structure contains a list of elements/variables that are used to detect/check a fault. 
 * @details
 * These fault object data structure variables are initialized in the power controller initialization source file. 
 * The address of this structure variable is passed as an argument into the fault handler functions which are called 
 * by the power controller system state machine. These fault handler functions are defined in the fault_common.c file. 
 **********************************************************************************/
typedef struct
{
   int16_t  Val1_Threshold;     ///< Fault threshold limit: Range of -32768 to 32767
   int16_t  Val1_Hysteresis;    ///< Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   int16_t  Val2_Threshold;     ///< Fault threshold limit: Range of -32768 to 32767
   int16_t  Val2_Hysteresis;    ///< Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   uint16_t  CounterSet;        ///< Number of consecutive fault events before fault becomes active.  
   uint32_t  CounterReset;      ///< Number of consecutive events when input outside hysteresis limit in order to remove fault, set to 0 to disable Hys 
   uint16_t  FaultCounter;      ///< Internal counter for activating/removing fault 
   uint16_t  FaultCode;         ///< Code that can be used to display fault (1st fault occurred) to global variable
   uint8_t   FaultActive;       ///< Set/Cleared inside flt check loop        
   uint8_t   FaultLatch;        ///< Flag indicating if FAULT has been tripped
}FAULT_OBJ_T;   

typedef void(*FAULT_CALLBACK)(void);

void FAULT_Init(FAULT_OBJ_T *faultInput,
                int16_t  threshold, int16_t  hysLimit,
                uint16_t thresholdCnt, uint32_t hysCnt);

void FAULT_InitRange(FAULT_OBJ_T *faultInput,
                int16_t  thresholdMin, int16_t  hysLimitMin,
                int16_t  thresholdMax, int16_t  hysLimitMax,
                uint16_t thresholdCnt, uint32_t hysCnt);

void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt);
void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit);
bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback);
bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback);
bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback);
bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback);


#endif 