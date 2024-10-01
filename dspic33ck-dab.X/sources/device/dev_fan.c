


/**
 * @file      dev_fan.c
 * @ingroup   dev-fan   
 * @brief     Contains fan initialization and execution functions.
 */

#include "dev_fan.h"
#include "dev_temp.h"

/*******************************************************************************
 * @ingroup dev-fan
 * @brief Data Object of fan 
 * 
 * @details The 'FAN_DATA_t' data object holds the access to all the fan control
 *   and monitoring values.
 *******************************************************************************/
FAN_DATA_t devFanData;
FAN_DATA_t* devFanDataPtr = &devFanData;

enum FAN_SETTINGS_e {
    NO_CHANGE     = 0,
    CHANGE_SPEED  = 1,
};
typedef enum FAN_SETTINGS_e FAN_SETTINGS_t;

static FAN_SETTINGS_t changeSpeed = CHANGE_SPEED;

//Private Function Call Prototypes
static uint16_t Convert_From_Percentage (uint8_t percentage);
static uint8_t Convert_From_Raw(uint16_t raw);
static void Override_Speed(void);
static void Calculate_Speed(void);
static void Update_Speed(void);


/***********************************************************************************
 * @ingroup dev-fan
 * @brief This function initializes the fan object
 * @return  void
 * 
 * @details This function initializes fan data objects including the initial
 *  fan speed, the maximum allowable speed, and the increment/decrement step 
 *  of the fan speed.
 **********************************************************************************/
void Dev_Fan_Initialize(void)
{
    devFanData.CurrentSpeedPercent = 0 ;
    devFanData.TargetSpeedPercent = INIT_SPEED_PERCENT;
    devFanData.MaxSpeedPercent = MAX_SPEED_PERCENT;
    devFanData.Tick = 5 ;
    devFanData.StepSizePercent = STEP_SIZE ;
    devFanData.TargetSpeedRaw = 0 ;
    devFanData.CurrentSpeedRaw = 0 ;
    devFanData.MaxSpeedRaw = Convert_From_Percentage(MAX_SPEED_PERCENT);
    //dev_fan_data.hystersis_raw = Convert_From_Percentage(HYSTERESIS);
    devFanData.OverrideFlag = 0 ;
}

/***********************************************************************************
 * @ingroup dev-fan
 * @brief this function needs to be called every 100ms it contains the code to 
 *  update the status of the fan
 * @return void
 * 
 * @details This function needs to be called every 100ms. This modulates the 
 * speed of fan depending on the initialized parameters in the fan object. 
 *
 * @note
 *    This is called in a rather slow 100ms object, as the speed modulation 
 * is not so critical.
 **********************************************************************************/
void Dev_Fan_Task_100ms(void) 
{
    static uint8_t currentTickCount;
    
    if((devTempData.OverTemperatureFlag == 1)){
        Dev_Fan_Set_Speed(50);  // set the fan speed to 50% when Over temperature fault trips
    }
    else{
        Dev_Fan_Set_Speed(20);  // set the fan speed to 10% when Over temperature fault does not trip
    }
    
    if (devFanData.OverrideFlag == 1) 
    {
        changeSpeed = CHANGE_SPEED;
        Override_Speed();
        Calculate_Speed();
        Update_Speed();     
    }
    else if (++currentTickCount >= devFanData.Tick ) 
    {
        Calculate_Speed();
        Update_Speed();
        currentTickCount = 0;
    }
}

/*******************************************************************************
 * @ingroup dev-fan
 * @brief  Sets the fan override bit and the fan speed
 * @return void
 * 
 * @details This function sets the fan override flag bit to true and sets the 
 *  fan target speed to maximum speed.
 * 
 *********************************************************************************/
void Dev_Fan_Set_Override(void) 
{
    devFanData.OverrideFlag = 1;
    Override_Speed();
}

/*******************************************************************************
 * @ingroup dev-fan
 * @brief  Sets the fan speed
 * @return void
 * 
 * @details This function sets the fan speed when the target speed does not 
 *  exceed the maximum fan speed.
 *********************************************************************************/
void Dev_Fan_Set_Speed(uint8_t target_speed_percent) 
{
    if (target_speed_percent <= MAX_SPEED_PERCENT)
    {
        devFanData.TargetSpeedPercent = target_speed_percent;
    }
}

/*******************************************************************************
 * @ingroup dev-fan
 * @brief  Converts the percentage value to number of ticks
 * @return void
 * 
 * @details This function converts the percent value to the duty cycle value as 
 *  per MULTIPLIER.
 *********************************************************************************/
static uint16_t Convert_From_Percentage(uint8_t percentage_value) 
{
    return (percentage_value * MULTIPLIER) ;
}

/*******************************************************************************
 * @ingroup dev-fan
 * @brief  Override the fan speed with maximum speed
 * @return void
 * 
 * @details This function changes the target speed value to maximum.
 *********************************************************************************/
static void Override_Speed(void) 
{
    devFanData.TargetSpeedPercent = MAX_SPEED_PERCENT;
    devFanData.Tick = MIN_TICK;
}

/*********************************************************************************
 * @ingroup dev-fan-private-functions
 * @fn      Calculate_Speed
 * @param   void
 * @brief   changes the target value
 * @return  nothing
 * @details 
 *  changes the target as per step size
 **********************************************************************************/
/*******************************************************************************
 * @ingroup dev-fan
 * @brief  Calculates the fan speed
 * @return void
 * 
 * @details This function calculates the fan speed.
 *********************************************************************************/
static void Calculate_Speed(void) 
{
    devFanData.TargetSpeedPercent -= (devFanData.TargetSpeedPercent % devFanData.StepSizePercent);
    
    devFanData.TargetSpeedRaw = Convert_From_Percentage(devFanData.TargetSpeedPercent);
    
    if (devFanData.CurrentSpeedRaw > (devFanData.TargetSpeedRaw )) // + dev_fan_data.hystersis_raw) )
    {
        devFanData.CurrentSpeedRaw -= Convert_From_Percentage(devFanData.StepSizePercent); 
        changeSpeed = CHANGE_SPEED;
    }
        
    else if (devFanData.CurrentSpeedRaw < (devFanData.TargetSpeedRaw )) // + dev_fan_data.hystersis_raw) )
    {
        devFanData.CurrentSpeedRaw += Convert_From_Percentage(devFanData.StepSizePercent) ;
        changeSpeed = CHANGE_SPEED;
    } 
    else 
    {
        changeSpeed = NO_CHANGE;
    }
} 

/*******************************************************************************
 * @ingroup dev-fan
 * @brief  Update the fan speed
 * @return void
 * 
 * @details This functions updates the fan speed.
 *********************************************************************************/
static void Update_Speed(void) 
{
    if (changeSpeed != NO_CHANGE)
    {
        if (devFanData.CurrentSpeedRaw > devFanData.MaxSpeedRaw)
        {
            devFanData.CurrentSpeedRaw = devFanData.MaxSpeedRaw;
        }
        FAN_PWM_DutyCycleSet(devFanData.CurrentSpeedRaw);
        devFanData.CurrentSpeedPercent = Convert_From_Raw(devFanData.CurrentSpeedRaw);
    }
}

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
static uint8_t Convert_From_Raw(uint16_t raw) 
{
    return ( raw * 0.02 );  //TODO: recalculate math
}

/*******************************************************************************
 * end of file
 *******************************************************************************/