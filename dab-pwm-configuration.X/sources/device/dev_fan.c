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

#include "dev_fan.h"

/*********************************************************************************
 * @ingroup dev-fan-private-variables
 * @var     dev_fan_data
 * @brief   fan interface object
 * @details 'dev_fan_data'. is the object providing access to all the fan control
 *          and monitoring values.
 **********************************************************************************/
FAN_DATA_t devFanData;
FAN_DATA_t* devFanDataPtr = &devFanData;
/*********************************************************************************
 * @ingroup dev-fan-private-variables
 * @var     current_tick_count
 * @brief   ticking away
 * @details 
 *          Ticking away detailed description
 **********************************************************************************/

static uint8_t CurrentTickCount;

enum FAN_SETTINGS_e {
    NO_CHANGE     = 0,
    CHANGE_SPEED  = 1,
};
typedef enum FAN_SETTINGS_e FAN_SETTINGS_t;


static FAN_SETTINGS_t ChangeSpeed = CHANGE_SPEED;

/***********************************************************************************
 * Private Function Call Prototypes
 **********************************************************************************/
uint16_t Convert_From_Percentage (uint8_t percentage);
uint8_t Convert_From_Raw(uint16_t raw);

void Override_Speed(void);
void Calculate_Speed(void);
void Update_Speed(void);


/***********************************************************************************
 * @ingroup dev-fan-public-functions
 * @fn     Dev_Fan_Init
 * @param  void
 * @return nothing
 * @brief this function initializes to the fan object
 * @details
 *   This function initalizes the LED to normal blink interval
 **********************************************************************************/
void Dev_Fan_Initialize(void)
{
    devFanData.CurrentSpeedPercent = 0 ;
    devFanData.TargetSpeedPercent = INIT_SPEED_PERCENT ;
    devFanData.MaxSpeedPercent = MAX_SPEED_PERCENT ;
    devFanData.Tick = 5 ;
    devFanData.StepSizePercent = STEP_SIZE ;
    devFanData.TargetSpeedRaw = 0 ;
    devFanData.CurrentSpeedRaw = 0 ;
    devFanData.MaxSpeedRaw = Convert_From_Percentage(MAX_SPEED_PERCENT);
    //dev_fan_data.hystersis_raw = Convert_From_Percentage(HYSTERESIS);
    devFanData.OverrideFlag = 0 ;
}

/***********************************************************************************
 * @ingroup dev-fan-public-functions
 * @fn     Dev_Fan_Task_100ms
 * @param  void
 * @return nothing
 * @brief this function needs to be called every 100ms
 *        it contains the code to update the status of the fan
 * @details
 *   this function needs to be called every 100ms. This modulates the speed of fan as per the 
 *   parameters in the fan object. also checks for the override flag.
 * @note
 *    this is called in a rather slow 100ms object, as the speed modulation is not so critical.
 **********************************************************************************/

void Dev_Fan_Task_100ms(void) 
{
    if (devFanData.OverrideFlag == 1) 
    {
        ChangeSpeed = CHANGE_SPEED;
        Override_Speed();
        Calculate_Speed();
        Update_Speed();     
    }
    else if (++CurrentTickCount >= devFanData.Tick ) 
    {
        Calculate_Speed();
        Update_Speed();
        CurrentTickCount = 0;
    }
}

// not to be inlined, but this should be part of api.
void Dev_Fan_Set_Override(void) 
{
    devFanData.OverrideFlag = 1;
    Override_Speed();
}

/*********************************************************************************
 * @ingroup dev-fan-private-functions
 * @fn      Convert_From_Percentage
 * @param   void
 * @brief   converts the desired target
 * @return  nothing
 * @details 
 *  This function converts the percent value to the duty cycle value as per MULTIPLIER
 **********************************************************************************/
uint16_t Convert_From_Percentage(uint8_t percentage_value) 
{
    return (percentage_value * MULTIPLIER) ;
}

/*********************************************************************************
 * @ingroup dev-fan-private-functions
 * @fn      Override_Speed
 * @param   void
 * @brief   changes the values
 * @return  nothing
 * @details 
 *  Changes the target value to max, and the update frequency to max update speed
 **********************************************************************************/
void Override_Speed(void) 
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
void Calculate_Speed(void) 
{
    devFanData.TargetSpeedPercent -= (devFanData.TargetSpeedPercent % devFanData.StepSizePercent);
    
    devFanData.TargetSpeedRaw = Convert_From_Percentage(devFanData.TargetSpeedPercent);
    
    if (devFanData.CurrentSpeedRaw > (devFanData.TargetSpeedRaw )) // + dev_fan_data.hystersis_raw) )
    {
        devFanData.CurrentSpeedRaw -= Convert_From_Percentage(devFanData.StepSizePercent); 
        ChangeSpeed = CHANGE_SPEED;
    }
        
    else if (devFanData.CurrentSpeedRaw < (devFanData.TargetSpeedRaw )) // + dev_fan_data.hystersis_raw) )
    {
        devFanData.CurrentSpeedRaw += Convert_From_Percentage(devFanData.StepSizePercent) ;
        ChangeSpeed = CHANGE_SPEED;
    } 
    else 
    {
        ChangeSpeed = NO_CHANGE;
    }
} 

/*********************************************************************************
 * @ingroup dev-fan-private-functions
 * @fn      Update_Speed
 * @param   void
 * @brief   changes the target value
 * @return  nothing
 * @details 
 *  changes the target as per step size
 **********************************************************************************/
void Update_Speed(void) 
{
    if (ChangeSpeed != NO_CHANGE)
    {
        if (devFanData.CurrentSpeedRaw > devFanData.MaxSpeedRaw)
        {
            devFanData.CurrentSpeedRaw = devFanData.MaxSpeedRaw;
        }
        FAN_PWM_DutyCycleSet(devFanData.CurrentSpeedRaw);
        devFanData.CurrentSpeedPercent = Convert_From_Raw(devFanData.CurrentSpeedRaw);
    }
}

/*********************************************************************************
 * @ingroup dev-fan-private-functions
 * @fn      Convert_From_Raw
 * @param   raw value
 * @brief   raw to percentage
 * @return  percentage
 * @details 
 *  raw to percentage
 **********************************************************************************/

uint8_t Convert_From_Raw(uint16_t raw) 
{
    return ( raw * 0.02 );  //TODO: recalculate math
}

/*********************************************************************************
 * @fn      Dev_Fan_Set_Speed
 * @param   target speed (as a percent of max)
 * @brief   set fan speed
 * @return  none
 * @details 
 * set fan speed
 **********************************************************************************/
void Dev_Fan_Set_Speed(uint8_t target_speed_percent) 
{
    if (target_speed_percent <= MAX_SPEED_PERCENT)
    {
        devFanData.TargetSpeedPercent = target_speed_percent;
    }
}

/*******************************************************************************
 * end of file
 *******************************************************************************/