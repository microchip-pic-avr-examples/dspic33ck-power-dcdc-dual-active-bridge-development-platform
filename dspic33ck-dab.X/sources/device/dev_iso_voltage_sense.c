/*
 * File:   dev_iso_voltage_sense.c
 * Author: M70027
 *
 * Created on Jan 17, 2024, 13:30 PM
 */

#include "dev_iso_voltage_sense.h"
#include "../sources/fault/fault_common.h"

/*********************************************************************************
 * @ingroup dev-vin-sccp-private-variables
 * @var     dev_iso_voltage_data
 * @brief   vin interface object
 * @details 'dev_iso_voltage_data'. is the object providing access to all the vin control
 *          and monitoring values.
 **********************************************************************************/
dev_iso_voltage_data_t  dev_iso_voltage_data;
dev_iso_voltage_data_t* dev_iso_voltage_data_ptr = &dev_iso_voltage_data;

/*********************************************************************************
 * @ingroup dev-vin-sccp-private-variables
 * @var     interrupt_counter
 * @brief   counts the interrupts during one task callback
 * @details 
 *          gets incremements in tasks. and is compared with dev-vin-sccp-fault-counts-macros
 **********************************************************************************/
static uint16_t interrupt_counter = 0;

/*********************************************************************************
 * @ingroup dev-vin-sccp-private-variables
 * @var     FAULT_OBJ_T Vin_Fault_Min
 * @brief   fault object for checking Under voltage condition
 * @details 
 * 
 **********************************************************************************/
FAULT_OBJ_T Vin_Fault_Min;

/*********************************************************************************
 * @ingroup dev-vin-sccp-private-variables
 * @var     FAULT_OBJ_T Vin_Fault_Max
 * @brief   fault object for checking Overvoltage condition
 * @details 
 * 
 **********************************************************************************/
FAULT_OBJ_T Vin_Fault_Max;

/***********************************************************************************
 * Private Function Call Prototypes
 **********************************************************************************/
void Dev_Iso_Voltage_Sens_Callback(void);   ///< local isr callback
void Dev_Iso_Voltage_Sens_Calculate_Average(void);
uint16_t Iso_Calculate_Average(uint16_t * buffer, uint16_t length);
void default_faultcallback_max(FAULT_OBJ_T* faultInput); 
void default_faultcallback_min(FAULT_OBJ_T* faultInput);

/***********************************************************************************
 * Public Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn     Dev_Iso_Voltage_Sens_Init
 * @param  void
 * @return nothing
 * @brief this function initializes to the dev_iso_voltage_data object and fault objects, and links the sccp2
 * @details
 *   
 **********************************************************************************/
void Dev_Iso_Voltage_Sens_Init(){
    dev_iso_voltage_data.over_voltage_flag = 0;          ///< Over voltage flag
    dev_iso_voltage_data.under_voltage_flag = 0;         ///< under voltage flag
    dev_iso_voltage_data.no_signal_flag = 0;             ///< if no signal is present  
    dev_iso_voltage_data.buffer_full = 0;                ///< flag to monitor if full then do averaging.
    dev_iso_voltage_data.voltage_buffer_idx = 0;         ///< current index of averaging buffer
    
    FAULT_Init(
            &Vin_Fault_Min,                             ///< fault object
            UV_THRESHOLD_RAW,                           ///< threshold against which raw values will be compared
            UV_LOWER_THRESHOLD_WITH_HYST,               ///< threshold plus hystersis
            FAULT_PERSISTENCE_COUNT,                    ///< number of ISR counts for which the fault should clear
            FAULT_PERSISTENCE_COUNT                     ///< number of ISR counts for which the fault should clear
            ) ; 
    
    FAULT_Init(
            &Vin_Fault_Max, 
            OV_THRESHOLD_RAW,               
            OV_UPPER_THRESHOLD_WITH_HYST,
            FAULT_PERSISTENCE_COUNT,
            FAULT_PERSISTENCE_COUNT ) ;
            
    InputCapture_CallbackRegister(Dev_Iso_Voltage_Sens_Callback);
}
/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn      Dev_Iso_Voltage_Task_1ms
 * @param   void
 * @return  nothing
 * @brief   this function needs to be called every 1ms
 *          it checks if the signal is present. by checking how many interrupts have been 
 * registered in 1 ms.

 **********************************************************************************/
void Dev_Iso_Voltage_Task_1ms(void) {
    
    if (interrupt_counter < INTERRUPT_COUNTS_MIN || interrupt_counter > INTERRUPT_COUNTS_MAX)
        dev_iso_voltage_data.no_signal_flag = 1;
    else 
        dev_iso_voltage_data.no_signal_flag = 0;
    
    interrupt_counter = 0;
    //GPIO_1_Toggle();
}

/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn      Dev_Reset_Average_Buffer
 * @param   void
 * @return  nothing
 * @brief   this resets the buffer index. This can be used to flush out the buffer values

 **********************************************************************************/
void Dev_Reset_Average_Buffer() {
    dev_iso_voltage_data.voltage_buffer_idx  = 0;
    dev_iso_voltage_data.buffer_full = 0;
}

/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn      Dev_Get_Iso_Voltage_avg
 * @param   void
 * @return  averaged buffer
 * @brief   this function returns the averaged buffer of voltage samples

 **********************************************************************************/
uint16_t Dev_Get_Iso_Voltage_avg(void) 
{
    if (dev_iso_voltage_data.buffer_full)
        return Iso_Calculate_Average(dev_iso_voltage_data.voltage_buffer, MAX_NUM_SAMPLES_ISO_BUFFER);
    else 
        return 0;
}

/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn      Dev_Get_Iso_Voltage
 * @param   void
 * @return  uint16_t 
 * @brief   this function returns the raw voltage value. (in terms of Duty Cycle)
 **********************************************************************************/
uint16_t Dev_Get_Iso_Voltage(void) 
{
        return dev_iso_voltage_data.voltage_raw; 
}


/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn      Dev_Iso_Voltage_Is_OV
 * @param   void
 * @return  uint8_t
 * @brief   this function returns the overvoltage flag
 **********************************************************************************/
uint8_t Dev_Iso_Voltage_Is_OV(void) 
{
        return dev_iso_voltage_data.over_voltage_flag; 
}

/***********************************************************************************
 * @ingroup dev-vin-sccp-public-functions
 * @fn      Dev_Iso_Voltage_Is_UV
 * @param   void
 * @return  uint8_t
 * @brief   this function returns the undervoltage flag
 **********************************************************************************/
uint8_t Dev_Iso_Voltage_Is_UV(void) 
{
    return dev_iso_voltage_data.under_voltage_flag; 
}

/***********************************************************************************
 * Private Function Call Prototypes
 **********************************************************************************/

/***********************************************************************************
 * @ingroup dev-vin-sccp-private-functions
 * @fn      Dev_Iso_Voltage_Sens_Callback
 * @param   void
 * @return  void
 * @brief   this is the ISR for capture compare module
 * @details 
 *          This is the ISR for the capture compare module. The module is configured
 * to sample every edge. and the callback is called when four values have been sampled, i.e
 * two pulses. AS the optooupler on the primary side is confiugured in Open Collector,
 * this results in inverting of the waveform. therefore the off time of the pwm represents the
 * Duty cycle from the primary side. the difference of the 3rd sample and second sample
 * would return the Duty cycle value.
 * 
 * This also checks for overvoltage and undervoltage conditions against the thresholds 
 * defined in dev-vin-sccp-sys-spec-macros
 **********************************************************************************/
void Dev_Iso_Voltage_Sens_Callback() {
    uint8_t i = 0;
    static uint16_t vdd_sens_buff[CCP_INPUT_CAPTURE_BUFFER];
    
    if (InputCapture_bufferoverflow)
    {
        i = 0;
        while ((!InputCapture_BufferEmpty) || (i < CCP_INPUT_CAPTURE_BUFFER)) 
        {
            vdd_sens_buff[i++] = CCP_BUF_L;
        }
        dev_iso_voltage_data.voltage_raw = vdd_sens_buff[2] - vdd_sens_buff[1];
        dev_iso_voltage_data.voltage_buffer[dev_iso_voltage_data.voltage_buffer_idx++] = dev_iso_voltage_data.voltage_raw ;
        
        CCP_TIMER_HIGH_REG = 0;
        CCP_TIMER_LOW_REG = 0;
    }
    CCP_INTERRUPT_FLAG = 0;

    interrupt_counter++ ;
    
    if (dev_iso_voltage_data.voltage_buffer_idx >= (MAX_NUM_SAMPLES_ISO_BUFFER - 1))  {      ///< store last 128 values and then average
        dev_iso_voltage_data.voltage_buffer_idx = 0;
       dev_iso_voltage_data.buffer_full = 1;    ///< flag to monitor if full then do averaging.
    }
    
    if(FAULT_CheckMax(&Vin_Fault_Max, dev_iso_voltage_data.voltage_raw, NULL ))
    {
        dev_iso_voltage_data.over_voltage_flag = 1; //for overvoltage
    }
    else
    {
        dev_iso_voltage_data.over_voltage_flag = 0; //for overvoltage
    }

    if(FAULT_CheckMin(&Vin_Fault_Min, dev_iso_voltage_data.voltage_raw, NULL ))
    {
        dev_iso_voltage_data.under_voltage_flag = 1; //for overvoltage
    } 
    else
    {
        dev_iso_voltage_data.under_voltage_flag = 0; //for overvoltage
    }
}

/*********************************************************************************
 * @ingroup dev-temp-private-functions
 * @fn      Iso_Calculate_Average
 * @param   uint16_t
 * @brief   calculates average of the buffer
 * @return  uint16_t averaged buffer
 * @details 
 *          
 **********************************************************************************/

uint16_t Iso_Calculate_Average(uint16_t * buffer, uint16_t length) {
    uint16_t index = 0;
    uint32_t sum = 0;
    for (index = 0; index <length; index++)
        sum +=buffer[index];
    return (sum >> MAX_NUM_SAMPLES_ISO_BUFFER_AS_EXP_OF_TWO);
}
/*******************************************************************************
 * end of file
 *******************************************************************************/