/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dev_pwrctrl.h"
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "config/macros.h"
#include "config/config.h"
#include "dev_pwrctrl_pwm.h"
#include "device/fault/dev_fault.h"
#include "dcdt/dev_pwrctrl_dcdt.h"
#include "system/pins.h"
    
/*******************************************************************************
 * @ingroup dev-pwrctrl-properties-public
 * @brief Data Object of primary voltage averaging
 * 
 * @details The 'VprimAveraging' data object holds the averaging parameter of the 
 *  Primary Voltage.
 *******************************************************************************/
AVERAGING_t VprimAveraging;
/*******************************************************************************
 * @ingroup dev-pwrctrl-properties-public
 * @brief Data Object of secondary voltage averaging
 * 
 * @details The 'VsecAveraging' data object holds the averaging parameter of the 
 *  Secondary Voltage.
 *******************************************************************************/
AVERAGING_t VsecAveraging;
/*******************************************************************************
 * @ingroup dev-pwrctrl-properties-public
 * @brief Data Object of secondary current averaging
 * 
 * @details The 'IsecAveraging' data object holds the averaging parameter of the 
 *  Secondary Current.
 *******************************************************************************/
AVERAGING_t IsecAveraging;

static void Dev_PwrCtrl_AdaptiveGainUpdate(void);

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief Executes the power converter control loop 
 * @return void
 * 
 * @details This function is called to execute the control loop of the power
 *  converter. It comprise of Voltage Loop control (VLoop), Power Loop control (PLoop) 
 *  and Current Loop Control (ILoop). Vloop and PLoop is ten times slower than
 *  the current loop with interleaved execution while Iloop is executed everytime 
 *  this function is called.  
 *********************************************************************************/
void Dev_PwrCtrl_ControlLoopExecute(void)
{
    static uint16_t cnt = 0;
    static bool VLoopExec = true;
    
    //Interleave the execution of VLoop and PLoop control for 10KHz execution
    if(++cnt == 5) 
    {
        // Averaging of Secondary Voltage
        VsecAveraging.AverageValue = (uint16_t)(__builtin_divud(VsecAveraging.Accumulator, VsecAveraging.Counter));
        VsecAveraging.Accumulator = 0;
        VsecAveraging.Counter = 0;
        
        //Condition for control loop execution controlling the loop enable bit
        if((dab.VLoop.Enable == false) && (VLoopExec == true)){
            dab.VLoop.Enable = true;
            dab.PLoop.Enable = false;
            
            // Averaging of Primary Voltage
            VprimAveraging.AverageValue = (uint16_t)(__builtin_divud(VprimAveraging.Accumulator, VprimAveraging.Counter));
            VprimAveraging.Accumulator = 0;
            VprimAveraging.Counter = 0;
            
            Dev_PwrCtrl_AdaptiveGainUpdate();
        }
        
        else if((dab.PLoop.Enable == false) && (VLoopExec == false)){
            dab.VLoop.Enable = false;
            dab.PLoop.Enable = true;
        
            // Averaging of Secondary Current
            IsecAveraging.AverageValue = (uint16_t)(__builtin_divud(IsecAveraging.Accumulator, IsecAveraging.Counter));
            IsecAveraging.Accumulator = 0;
            IsecAveraging.Counter = 0;
        }
        cnt = 0;
    }
    
    // Execute the Voltage Loop Control
    if((dab.VLoop.Enable == true) && (VLoopExec == true))
    {
        VLoopExec = false;
        
        if(dab.PowerDirection == PWR_CTRL_CHARGING)
        { 
            VMC_2p2z.KfactorCoeffsB = 0x7FFF;
            VMC_2p2z.maxOutput =  0x7FFF;

            // Bit-shift value used to perform input value normalization
            dab.VLoop.Feedback = VsecAveraging.AverageValue << 3;  
            dab.VLoop.Reference = dab.VLoop.Reference << 3;

            // Execute the Voltage Loop Control
            SMPS_Controller2P2ZUpdate(&VMC_2p2z, &dab.VLoop.Feedback,
                    dab.VLoop.Reference, &dab.VLoop.Output);
        
            // Reset the Vloop reference to its original scaling
            dab.VLoop.Reference = dab.VLoop.Reference >> 3;
        }
    }

    // Execute the Power Loop Control
    if((dab.PLoop.Enable == true) && (VLoopExec == false))
    {      
        VLoopExec = true;

        // Bit-shift value used to perform input value normalization
        // Scaled the feedback to Power (Watts in units)
        uint32_t buf = (uint32_t)IsecAveraging.AverageValue * 
                (uint32_t)VsecAveraging.AverageValue * POWER_RESOLUTION; 

        // scale back the 14 bit from the POWER_RESOLUTION calculation 
        // to get the Watts value for Power Loop
        buf >>=14;
        
        dab.Data.SecPower = buf;
         
        dab.PLoop.Feedback = dab.Data.SecPower;
        dab.PLoop.Reference = dab.PLoop.Reference;
        
        // Execute the Power Loop Control
        SMPS_Controller2P2ZUpdate(&PMC_2p2z, &dab.PLoop.Feedback,
                dab.PLoop.Reference, &dab.PLoop.Output);

    }

    // Execute the Current Loop Control
    if(dab.ILoop.Enable == true)
    {
        //Bit-shift value used to perform input value normalization
        dab.ILoop.Feedback = dab.Data.ISecAverageRectified << 3;
        //adaptive gain factor
        IMC_2p2z.KfactorCoeffsB = dab.ILoop.AgcFactor;
        //refresh limits
        IMC_2p2z.maxOutput =  0x7FFF;

        // Mixing stage from voltage loop 10KHz
        uint32_t RefBuf = (uint32_t)(dab.ILoop.Reference) * 
                (uint32_t)(dab.VLoop.Output & 0x7FFF);
        uint16_t ILoopReference = (uint16_t)(RefBuf >> 12); 

        // Mixing stage from power loop 10KHz
        RefBuf =  (uint32_t)ILoopReference * (uint32_t)(dab.PLoop.Output & 0x7FFF);  
        ILoopReference = (int16_t)(RefBuf >> 15);    
         
        XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback,   
                ILoopReference, &dab.ILoop.Output);    

        // Control loop output copied to control phase
        dab.Pwm.ControlPhase = (((uint32_t)(dab.Pwm.ControlDutyCycle) * 
                (uint32_t)dab.ILoop.Output) >> 15); //range 0..180
        
         dab.Pwm.ControlPhase += dab.Pwm.DeadTimeLow;
        
        if(dab.Pwm.ControlPhase > (dab.Pwm.ControlDutyCycle - MIN_PHASE_SHIFTED_PULSE))
            dab.Pwm.ControlPhase = dab.Pwm.ControlDutyCycle - MIN_PHASE_SHIFTED_PULSE;
        
        if(dab.Pwm.ControlPhase < dab.Pwm.DeadTimeLow) dab.Pwm.ControlPhase = dab.Pwm.DeadTimeLow;  
    }
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief  This function updates the DAB data members with ADC raw values
 * @return void
 * 
 * @details This function is called every 100KHz and triggers the ADC module. This
 *  also handles the updating of DAB data members with its latest ADC raw values
 *  and collection of data for averaging.
 *********************************************************************************/
void Dev_PwrCtrl_UpdateADConverterData (void)
{
    ADC1_SoftwareTriggerEnable();
    
    dab.Data.ISecAverage = ADC1_ConversionResultGet(ISEC_AVG); 
    dab.Data.ISecAverageRectified = abs(dab.Data.ISecAverage - dab.Data.ISecSensorOffset);
    IsecAveraging.Accumulator += dab.Data.ISecAverageRectified;
    IsecAveraging.Counter = IsecAveraging.Counter + 1;
    
    dab.Data.VSecVoltage = ADC1_ConversionResultGet(VSEC);
    VsecAveraging.Accumulator += dab.Data.VSecVoltage;
    VsecAveraging.Counter = VsecAveraging.Counter + 1; 
    
    dab.Data.ISenseSecondary = ADC1_ConversionResultGet(ISEC_CT); 
    dab.Data.ISensePrimary = ADC1_ConversionResultGet(IPRI_CT);   

    dab.Data.VPriVoltage = ADC1_ConversionResultGet(VPRI);
    VprimAveraging.Accumulator += dab.Data.VPriVoltage;
    VprimAveraging.Counter = VprimAveraging.Counter + 1;         
            
    dab.Data.VRail_5V = ADC1_ConversionResultGet(VRAIL_5V);
    dab.Data.Temperature = ADC1_ConversionResultGet(TEMP);
    
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief Updates the Adaptive gain for the power converter control loop 
 * @return void
 * 
 * @details tbd
 *********************************************************************************/
static void Dev_PwrCtrl_AdaptiveGainUpdate(void)
{
    uint16_t DAB_PrimaryVoltage; // Convert to actual voltage 

    // calculate the primary voltage in terms of Volts
    DAB_PrimaryVoltage = __builtin_divud((VprimAveraging.AverageValue * VPRI_SCALER), VPRI_FACTOR);     
    
    if(dab.PowerDirection == PWR_CTRL_CHARGING)
    { 
        if(DAB_PrimaryVoltage > AGC_MINIMUM_VIN_THRESHOLD)
            dab.ILoop.AgcFactor = (int16_t) (0x7FFF & 
                    __builtin_divud(AGC_DAB_FACTOR, DAB_PrimaryVoltage));
        
        else
            dab.ILoop.AgcFactor = 0x7FFF;
    }
    
    // Reserved for future Development
    if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
    { 
    }
}


/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief  This function updates the DAB data members with phase values normalized in degree
 * @return void
 * 
 * @details 
 *********************************************************************************/
void Dev_PwrCtrl_PrimToSecPHDegree(void)//normalize phase to 0..90.  feed this value to steer period modulation
{
    static unsigned long buff;
    static unsigned int buf;
    
    //instead of floating point operations:
    buff = ((unsigned long)dab.Pwm.ControlPhase)<<10;//*1024
    buf = __builtin_divud( buff ,dab.Pwm.ControlDutyCycle);
    buff = (unsigned long)buf*90;
    buf = __builtin_divud( buff ,102);//divide by 102.4. Accept  the small rounding error
    dab.Pwm.ControlPhase_P2S_Degreex10 = buf;
    if(dab.Pwm.ControlPhase_P2S_Degreex10 >= 900)
    { 
       dab.Pwm.ControlPhase_P2S_Degreex10 = 899;//rounding errors hard clip   roughly 10 bit resolution
    }
}
/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief  This function updates the DAB data members dead time based on load. experimental values.
 * @return void
 * 
 * @details 
 *********************************************************************************/
void Dev_PwrCtrl_DeadTimeAdjust(void)
{
        uint16_t NewDT=0;   
        // Deadtime values adjusted on rough phase range. experimentally measured values.
        if  (dab.Pwm.ControlPhase_P2S_Degreex10 < 290)                                           
            {NewDT = 500*4;}
        if ((dab.Pwm.ControlPhase_P2S_Degreex10 > 330)&&(dab.Pwm.ControlPhase_P2S_Degreex10 < 800)) 
            {NewDT = 500*3;}
        if ((dab.Pwm.ControlPhase_P2S_Degreex10 > 805)&&(dab.Pwm.ControlPhase_P2S_Degreex10 < 875)) 
            {NewDT = 700;}
        if  (dab.Pwm.ControlPhase_P2S_Degreex10 > 880)                                           
            {NewDT = 600;} // Minimum deadtime

        if(NewDT)
        {
            if (NewDT < MINIMUM_DEADTIME) 
                NewDT = MINIMUM_DEADTIME; //minimum DT for safe operation.
            
            dab.Pwm.DeadTimeLow = NewDT;
            dab.Pwm.DeadTimeHigh = NewDT;
            
            NewDT=0;// update once
        }
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-private
 * @brief  This function updates the DAB data members and modulates PWM period
 * @return void
 * 
 * @details 
 *********************************************************************************/
void  Dev_PwrCtrl_PeriodModulator(void)
{
    static uint16_t decimPM;
    decimPM++;
    
    if(dab.Pwm.ControlPhase_P2S_Target > PRI_TO_SEC_PHASE_TARGET) 
        dab.Pwm.ControlPhase_P2S_Target = PRI_TO_SEC_PHASE_TARGET;//clamp cutoff while modulating period
    if(dab.Pwm.ControlPhase_P2S_Target == 0) 
        dab.Pwm.ControlPhase_P2S_Target = PRI_TO_SEC_PHASE_TARGET;
        
    if(decimPM>=8) 
    {
        if (dab.Pwm.ControlPhase_P2S_Degreex10 < dab.Pwm.ControlPhase_P2S_Target-5)
        {
            if((dab.Pwm.ControlPeriod > MIN_PWM_PERIOD) && (dab.Pwm.LowPowerSlowMode == 0))
            {  
                dab.Pwm.ControlPeriod-= PERIODSTEP;
            }
            else
            {
                // When phase shift between primary to secondary is 68 degrees,
                // DAB runs in low power mode
                if (dab.Pwm.ControlPhase_P2S_Degreex10 < 680)
                {
                   dab.Pwm.LowPowerSlowMode =1;
                }
            }
        }

        if (dab.Pwm.ControlPhase_P2S_Degreex10 > dab.Pwm.ControlPhase_P2S_Target+5)
        {  
            if((dab.Pwm.ControlPeriod < MAX_PWM_PERIOD) && (dab.Pwm.ControlPhase_P2S_Degreex10  > 20))
            {    
                dab.Pwm.ControlPeriod+= PERIODSTEP;
            }
        }

        if(dab.Pwm.LowPowerSlowMode == 1)
        {    
            if (dab.Pwm.ControlPhase_P2S_Degreex10 > 600)
            {    
                dab.Pwm.LowPowerSlowMode = 0;
            }
            else
            {    
                if(dab.Pwm.ControlPeriod < MIN_PWM_PERIOD)
                {  
                    dab.Pwm.ControlPeriod += PERIODSTEP;
                }
                else
                if (dab.Pwm.ControlPhase_P2S_Degreex10 > 330 )//snap out
                {
                    dab.Pwm.LowPowerSlowMode = 0;
                }
            }
        }
        decimPM=0;
    }
}

