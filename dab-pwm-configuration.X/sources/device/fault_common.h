
#ifndef FAULT_H
#define FAULT_H

#include <stdint.h>
#include <stdbool.h>

/***********************************************************************************
 * @ingroup  Fault_Handler_API
 * @struct   FAULT_OBJ_T
 * @brief    The Fault object data structure contains a list of elements/variables that are used to detect/check a fault. 
 * @details
 * These fault object data structure variables are initialized in the power controller initialization source file. 
 * The address of this structure variable is passed as an argument into the fault handler functions which are called 
 * by the power controller system state machine. These fault handler functions are defined in the fault_common.c file. 
 **********************************************************************************/
typedef struct
{
   int16_t  val1_Threshold;     // Fault threshold limit: Range of -32768 to 32767
   int16_t  val1_Hysteresis;    // Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   int16_t  val2_Threshold;     // Fault threshold limit: Range of -32768 to 32767
   int16_t  val2_Hysteresis;    // Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   uint16_t  CounterSet;        // Number of consecutive fault events before fault becomes active.  
   uint32_t  CounterReset;      // Number of consecutive events when input outside hysteresis limit in order to remove fault, set to 0 to disable Hys 
   uint16_t  fltCounter;        // Internal counter for activating/removing fault 
   uint16_t  fltCode;           // Code that can be used to display fault (1st fault occurred) to global variable
   uint8_t   faultActive;       // Set/Cleared inside flt check loop        
}FAULT_OBJ_T;   

typedef void(*FAULT_CALLBACK)(FAULT_OBJ_T* faultInput);


/*******************************************************************************
 * @ingroup  Fault_Handler_API
 * @fn       void FAULT_Init(FAULT_OBJ_T *faultInput,int16_t  threshold, int16_t  hysLimit,uint16_t thresholdCnt, uint32_t hysCnt)
 * @brief    This function initializes the fault object data structure with the values that required to detect a fault.  
 * @param    faultInput (a pointer to the fault object), threshold and Hysteresis limit values, set & reset counter values.  
 * @return   Return type is void. 
 * @details	   
 * This function initializes the fault structure with the thresholds and counter limits 
 * which are required to detect a fault.
 *******************************************************************************/
void FAULT_Init(FAULT_OBJ_T *faultInput,
                int16_t  threshold, int16_t  hysLimit,
                uint16_t thresholdCnt, uint32_t hysCnt);

void FAULT_InitRange(FAULT_OBJ_T *faultInput,
                int16_t  thresholdMin, int16_t  hysLimitMin,
                int16_t  thresholdMax, int16_t  hysLimitMax,
                uint16_t thresholdCnt, uint32_t hysCnt);


/*******************************************************************************
 * @ingroup  Fault_Handler_API
 * @fn       void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt)
 * @brief    This function initializes/sets the fault structure with the counter limits.
 * @param    faultInput (a pointer to the fault object), Set & Reset counter values are arguments to this function.  
 * @return   Return type is void. 
 * @details	    
 * This function initializes the CounterSet, CounterReset variables of the fault object data structure.
 * These counter values together with thresholds are used to set and reset the fault.
 *******************************************************************************/
void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt);


/*******************************************************************************
 * @ingroup  Fault Handler API
 * @fn       void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit)
 * @brief    This function initializes/sets the fault structure with the threshold limits.
 * @param    faultInput (a pointer to the fault object) and threshold limits are arguments to this function.  
 * @return   Return type is void. 
 * @details	    
 * This function initializes/sets the val1_Threshold, val1_Hysteresis variables of the fault object data structure.
 * These threshold values together with counter limits are used to set and reset the fault.
 *******************************************************************************/
void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit);


/*******************************************************************************
 * @ingroup  Fault_Handler_API
 * @fn       bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
 * @brief    This function compares the fault source value with the threshold limits and sets (or) resets the fault active state accordingly.
 * @param    faultInput (a pointer to the fault object), input fault source and callback function pointer are arguments to this function.  
 * @return   Returns true if fault is active otherwise it returns false. 
 * @details	     
 * This function returns fault active state to TRUE, if input fault source is greater than the set threshold limit
 * for a time equivalent to set_counter value. Also, after the fault active state becomes TRUE, if callback function
 * pointer is non-null, then the routine will call the callback function which quickly shutdown the system, if needed.
 * Also the function returns fault active state to FALSE, if input fault source is less than the hysteresis limit value
 * for a time equivalent to reset_counter value.\n
 * Ex: In case of input over voltage fault, If Vin > 18V for 0.1s, then input OV fault is set. If Vin < 17.5V for 0.1s,
 * then input OV fault is reset.             
 *******************************************************************************/
bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback);


/*******************************************************************************
 * @ingroup  Fault_Handler_API
 * @fn       bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
 * @brief    This function compares the fault source value with the threshold limits and sets (or) resets the fault active state accordingly.
 * @param    faultInput (a pointer to the fault object), input fault source and callback function pointer are arguments to this function.  
 * @return   Returns true if fault is active otherwise it returns false. 
 * @details	     
 * This function returns fault active state to TRUE, if input fault source is less than the set threshold limit
 * for a time equivalent to set_counter value. Also, after the fault active state becomes TRUE, if callback function
 * pointer is non-null, then the routine will call the callback function which quickly shutdown the system, if needed.
 * Also the function returns fault active state to FALSE, if input fault source is greater than the hysteresis limit value
 * for a time equivalent to reset_counter value.\n
 * Ex: In case of input under voltage fault, If Vin < 6.5V for 0.1s, then input UV fault is set. If Vin > 7V for 0.1s,
 * then input UV fault is reset.
 *******************************************************************************/
bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback);


/*******************************************************************************
 * @ingroup  Fault_Handler_API
 * @fn       bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback)
 * @param    faultInput (a pointer to the fault object), fault bit and callback function pointer are arguments to this function.  
 * @return   Returns true if fault is active otherwise it returns false. 
 * @details	     
 * This function returns fault active state to TRUE, if the fault bit is set for a time equivalent
 * to set_counter value. Also, after the fault active state becomes TRUE, if callback function
 * pointer is non-null, then the routine will call the callback function which quickly shutdown the system, If needed.
 * Also the function returns fault active state to FALSE, if the fault bit is reset for a time equivalent
 * to reset_counter value.
 *******************************************************************************/
bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback);


/*******************************************************************************
 * @ingroup  Fault_Handler_API
 * @fn       bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback)
 * @param    faultInput (a pointer to the fault object), input value and callback function pointer are arguments to this function.  
 * @return   Returns true if fault is active otherwise it returns false. 
 * @details	     
 * This function returns fault active state to TRUE, if the input value is out of threshold limits range 
 * for a time equivalent to set_counter value. Also, after the fault active state becomes TRUE, if callback function
 * pointer is non-null, then the routine will call the callback function which quickly shutdown the system, If needed.
 * Also the function returns fault active state to FALSE, if the input value is within the hysteresis limits range
 * for a time equivalent to reset_counter value.
 *******************************************************************************/
bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback);


#endif 