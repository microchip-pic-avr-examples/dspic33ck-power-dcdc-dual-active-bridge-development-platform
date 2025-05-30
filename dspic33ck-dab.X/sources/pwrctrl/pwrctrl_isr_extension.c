
/**
 * @file      pwrctrl_isr_extension.c
 * @ingroup   pwrctrl-isr   
 * @brief     Contains some of the functions used in the interrupt service routine
 *  of control loop.
 */

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// MCC header files
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "system/pins.h"

// DAB header files
#include "config/macros.h"
#include "config/config.h"
#include "pwrctrl.h"
#include "fault/fault.h"
#include "dcdt/pwrctrl_dcdt.h"

    
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of primary voltage averaging
 * 
 * @details The 'VprimAveraging' data object holds the averaging parameter of the 
 *  Primary Voltage.
 *******************************************************************************/
AVERAGING_t vPrimAveraging;
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of secondary voltage averaging
 * 
 * @details The 'VsecAveraging' data object holds the averaging parameter of the 
 *  Secondary Voltage.
 *******************************************************************************/
AVERAGING_t vSecAveraging;
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of secondary current averaging
 * 
 * @details The 'IsecAveraging' data object holds the averaging parameter of the 
 *  Secondary Current.
 *******************************************************************************/
AVERAGING_t iSecAveraging;

// STATIC VARIABLES and FUNCTIONS
static void PwrCtrl_AdaptiveGainUpdate(void);
static bool VLoopInterleaveExec = true;

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members with ADC raw values
 * @return void
 * 
 * @details This function is called every 100KHz and triggers the ADC module. This
 *  also handles the updating of DAB data members with its latest ADC raw values
 *  and collection of data for averaging.
 *********************************************************************************/
void PwrCtrl_UpdateADConverterData (void)
{        
    // Enable the ADC sampling
    ADC1_SoftwareTriggerEnable();
    
    if(ADC1_IsConversionComplete(ISEC_CT))
        dab.Data.ISenseSecondary = ADC1_ConversionResultGet(ISEC_CT); 
    
    if(ADC1_IsConversionComplete(IPRI_CT))
        dab.Data.ISensePrimary = ADC1_ConversionResultGet(IPRI_CT);   
    
    if(ADC1_IsConversionComplete(VRAIL_5V))
        dab.Data.VRail_5V = ADC1_ConversionResultGet(VRAIL_5V);
    
    if(ADC1_IsConversionComplete(TEMP))
        dab.Data.Temperature = ADC1_ConversionResultGet(TEMP);
    
    if(ADC1_IsConversionComplete(VPRI)){
        dab.Data.VPriVoltage = ADC1_ConversionResultGet(VPRI);
        vPrimAveraging.Accumulator += dab.Data.VPriVoltage;
        vPrimAveraging.Counter = vPrimAveraging.Counter + 1;   
    }
    
    if(ADC1_IsConversionComplete(VSEC)){
        dab.Data.VSecVoltage = ADC1_ConversionResultGet(VSEC);
        vSecAveraging.Accumulator += dab.Data.VSecVoltage;
        vSecAveraging.Counter = vSecAveraging.Counter + 1; 
    }
    
    if(ADC1_IsConversionComplete(ISEC_AVG)){
        dab.Data.ISecAverage = ADC1_ConversionResultGet(ISEC_AVG); 
        dab.Data.ISecAverageRectified = abs(dab.Data.ISecAverage - dab.Data.ISecSensorOffset);
        iSecAveraging.Accumulator += dab.Data.ISecAverageRectified;
        iSecAveraging.Counter = iSecAveraging.Counter + 1;
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function prepares the data for control loop and selects which 
 *  control loop will be executed. 
 * @return void
 * 
 * @details This function prepares the data for Voltage loop control and Power
 *  loop control. The enable bit of these control loops are manage in this function 
 *  in which the VLoop and PLoop are enabled at 10KHz with 180 degrees phase. 
 *********************************************************************************/
void PwrCtrl_10KHzVPLoopPrepareData(void)
{
    static uint16_t cnt = 0;
    
    //Interleave the execution of VLoop and PLoop control for 10KHz execution
    if(++cnt == (VPLOOP_ILOOP_EXE_RATIO)) 
    {
        // Averaging of Secondary Voltage
        vSecAveraging.AverageValue = (uint16_t)(__builtin_divud(vSecAveraging.Accumulator, vSecAveraging.Counter));
        vSecAveraging.Accumulator = 0;
        vSecAveraging.Counter = 0;  
        
        // Averaging of Primary Voltage
        vPrimAveraging.AverageValue = (uint16_t)(__builtin_divud(vPrimAveraging.Accumulator, vPrimAveraging.Counter));
        vPrimAveraging.Accumulator = 0;
        vPrimAveraging.Counter = 0;
            
        #if(OPEN_LOOP_PBV == false)
        //Condition for control loop execution controlling the loop enable bit
        if((dab.VLoop.Enable == false) && (VLoopInterleaveExec == true) && (dab.Status.bits.Running == 1))
        #endif  
        {
            // Enable Vloop control
            dab.VLoop.Enable = true;
            dab.PLoop.Enable = false;
            // Compute the Adaptive Gain 
            PwrCtrl_AdaptiveGainUpdate();
        }
 
        #if(OPEN_LOOP_PBV == false)
        else if((dab.PLoop.Enable == false) && (VLoopInterleaveExec == false) && (dab.Status.bits.Running == 1))
        #endif
        {
            // Enable PLoop control
            dab.VLoop.Enable = false;
            dab.PLoop.Enable = true;
        
            // Averaging of Secondary Current
            iSecAveraging.AverageValue = (uint16_t)(__builtin_divud(iSecAveraging.Accumulator, iSecAveraging.Counter));
            iSecAveraging.Accumulator = 0;
            iSecAveraging.Counter = 0;
            
            // Bit-shift value used to perform input value normalization
            // Scaled the feedback to Power (Watts in units)
            uint32_t buf = (uint32_t)iSecAveraging.AverageValue * 
                    (uint32_t)vSecAveraging.AverageValue * POWER_FACTOR; 

            // scale back the 14 bit from the POWER_RESOLUTION calculation 
            // to get the Watts value for Power Loop
            buf >>= POWER_SCALER;

            // Transfer to SecPower data member the power computation in [Watts] 
            dab.Data.SecPower = buf;
        
        }
        
        // If Power supply is not yet running, the averaging of
        // primary voltage and secondary voltage continues to display in PBV
        if(!dab.Status.bits.Running){

            // Averaging of Secondary Current
            iSecAveraging.AverageValue = (uint16_t)(__builtin_divud(iSecAveraging.Accumulator, iSecAveraging.Counter));
            iSecAveraging.Accumulator = 0;
            iSecAveraging.Counter = 0;
            
            // Bit-shift value used to perform input value normalization
            // Scaled the feedback to Power (Watts in units)
            uint32_t buf = (uint32_t)iSecAveraging.AverageValue * 
                    (uint32_t)vSecAveraging.AverageValue * POWER_FACTOR; 

            // scale back the 14 bit from the POWER_RESOLUTION calculation 
            // to get the Watts value for Power Loop
            buf >>= POWER_SCALER;

            // Transfer to SecPower data member the power computation in [Watts] 
            dab.Data.SecPower = buf;
        }
        cnt = 0;
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Executes the power converter control loop 
 * @return void
 * 
 * @details This function is called to execute the control loop of the power
 *  converter. It comprise of Voltage Loop control (VLoop), Power Loop control (PLoop) 
 *  and Current Loop Control (ILoop). Vloop and PLoop is ten times slower than
 *  the current loop with interleaved execution while Iloop is executed every time 
 *  this function is called.  
 *********************************************************************************/


/*******************************************************************************
 *    In reverse mode of power flow, voltage sensor is not working reliably below 100V.  
 *    In order to avoid possible voltage overshoot, checking needs to be done against a minimum voltage threshold 
 *    When under voltage detected, makes it mandatory to ramp up reverse mode voltage very very slowly, without sensing
 *    Until voltage readings become reliable, P and I references are forced to minimum safe values.
 *    This achieves very slow ramp up of voltage across output capacitors.
 *********************************************************************************/
#define MIN_REFERENCE_THRESHOLD_IV (10)//roughly 300mA
#define MIN_REFERENCE_THRESHOLD_P  (15)//
#define PIRM_MIN_VOLTAGE_55V       (250)
#define PRIMARY_VOLTAGE_MODE_SENSOR_SAFE_CHECK() do { \
    if (dab.Properties.IReference > MIN_REFERENCE_THRESHOLD_IV) \
        if (dab.Properties.VPriReference > MIN_REFERENCE_THRESHOLD_IV) \
            if (dab.Properties.PwrReference > MIN_REFERENCE_THRESHOLD_P) \
                if (vPrimAveraging.AverageValue < PIRM_MIN_VOLTAGE_55V) { \
                    dab.Properties.IReference = MIN_REFERENCE_THRESHOLD_IV; \
                    dab.ILoop.Reference = MIN_REFERENCE_THRESHOLD_IV; \
                    dab.Properties.PwrReference = MIN_REFERENCE_THRESHOLD_P; \
                    dab.PLoop.Reference = MIN_REFERENCE_THRESHOLD_P; \
                } \
} while (0)

void PwrCtrl_ControlLoopExecute(void)
{   
    
    // Execute the Voltage Loop Control
    if((dab.VLoop.Enable == true) && (VLoopInterleaveExec == true))
    {     
        VLoopInterleaveExec = false;
        
        if(dab.PowerDirection == PWR_CTRL_CHARGING)
        { 
            VMC_2p2z.KfactorCoeffsB = 0x7FFF;
            VMC_2p2z.maxOutput =  0x7FFF;

            // Bit-shift value used to perform input value normalization
            dab.VLoop.Feedback = vSecAveraging.AverageValue << 3;  
            dab.VLoop.Reference = (dab.VLoop.Reference << 3);

            // Execute the Voltage Loop Control 
            XFT_SMPS_Controller2P2ZUpdate(&VMC_2p2z, &dab.VLoop.Feedback,dab.VLoop.Reference, &dab.VLoop.Output);

            // Reset the Vloop reference to its original scaling
            dab.VLoop.Reference = (dab.VLoop.Reference >> 3); 
        }
        
        if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
        { 
            VMC_2p2z_Rev.KfactorCoeffsB = 0x7FFF;
            VMC_2p2z_Rev.maxOutput =  0x7FFF;

            // Bit-shift value used to perform input value normalization
            dab.VLoop.Feedback = vPrimAveraging.AverageValue << 3;  
            dab.VLoop.Reference = (dab.VLoop.Reference << 3);//should be limited range or even fixed for GTI input

            // Execute the Voltage Loop Control
            XFT_SMPS_Controller2P2ZUpdate(&VMC_2p2z_Rev, &dab.VLoop.Feedback,
                    dab.VLoop.Reference, &dab.VLoop.Output);

            // Reset the Vloop reference to its original scaling
            dab.VLoop.Reference = (dab.VLoop.Reference >> 3);
        }
    }


    // Execute the Power Loop Control
    if((dab.PLoop.Enable == true) && (VLoopInterleaveExec == false))
    {      
        if(dab.PowerDirection == PWR_CTRL_CHARGING)
        { 
            VLoopInterleaveExec = true;

            dab.PLoop.Feedback = dab.Data.SecPower;
            dab.PLoop.Reference = dab.PLoop.Reference;

            // Execute the Power Loop Control
            SMPS_Controller2P2ZUpdate(&PMC_2p2z, &dab.PLoop.Feedback,
                    dab.PLoop.Reference, &dab.PLoop.Output);
        }

        if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
        {
            VLoopInterleaveExec = true;

            dab.PLoop.Feedback = dab.Data.SecPower;
            dab.PLoop.Reference = dab.PLoop.Reference;

            // Execute the Power Loop Control
            SMPS_Controller2P2ZUpdate(&PMC_2p2z_Rev, &dab.PLoop.Feedback,
                    dab.PLoop.Reference, &dab.PLoop.Output);
        }
    }

    // Execute the Current Loop Control
    if(dab.ILoop.Enable == true)
    {
        DPD_TP31_SetHigh();//Test/dbg purpose only
        //Bit-shift value used to perform input value normalization
        dab.ILoop.Feedback = dab.Data.ISecAverageRectified << 3;
        //adaptive gain factor
        IMC_2p2z.KfactorCoeffsB = dab.ILoop.AgcFactor;
        IMC_2p2z_Rev.KfactorCoeffsB = dab.ILoop.AgcFactor;
        //refresh limits
        IMC_2p2z.maxOutput =  0x7FFF;

        if(dab.PowerDirection == PWR_CTRL_CHARGING)
        { 
            // Mixing stage from voltage loop 10KHz
            uint32_t RefBuf = (uint32_t)(dab.ILoop.Reference) * (uint32_t)(dab.VLoop.Output & 0x7FFF);
            uint16_t ILoopReference = (uint16_t)(RefBuf >> 12); 

            // Mixing stage from power loop 10KHz
            RefBuf =  (uint32_t)ILoopReference * (uint32_t)(dab.PLoop.Output & 0x7FFF);  
            ILoopReference = (int16_t)(RefBuf >> 15);       

            // Basic clamping in rising direction, in case of  Iloop or Vloop overshoot during large load step. 
            if( dab.Data.ISecAverageRectified >  (dab.ILoop.Reference + ISEC_LOAD_STEP_CLAMP)) 
            {
                 XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback, 0, &dab.ILoop.Output); //force I ref to 0
            }
            else
                if(vSecAveraging.AverageValue > (dab.VLoop.Reference + VSEC_LOAD_STEP_CLAMP))
                {    
                    XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback, 0, &dab.ILoop.Output);//force I ref to 0
                }
                else
                {
                    XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback, ILoopReference, &dab.ILoop.Output);  
                }    
        }
        
        if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
        {
            PRIMARY_VOLTAGE_MODE_SENSOR_SAFE_CHECK();
            
            // Mixing stage from voltage loop 10KHz
            uint32_t RefBuf = (uint32_t)(dab.ILoop.Reference) * (uint32_t)(dab.VLoop.Output & 0x7FFF);
            uint16_t ILoopReference = (uint16_t)(RefBuf >> 12); 

            // Mixing stage from power loop 10KHz
            RefBuf =  (uint32_t)ILoopReference * (uint32_t)(dab.PLoop.Output & 0x7FFF);  
            ILoopReference = (int16_t)(RefBuf >> 15);       

            // Basic clamping in rising direction, in case of  Iloop or Vloop overshoot during large load step. 
            if( dab.Data.ISecAverageRectified >  (dab.ILoop.Reference + ISEC_LOAD_STEP_CLAMP)) 
            {
                 XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z_Rev, &dab.ILoop.Feedback,0, &dab.ILoop.Output); //force I ref to 0
            }
            else
                if(vPrimAveraging.AverageValue > (dab.VLoop.Reference +  VPRIM_LOAD_STEP_CLAMP  ))
                {    
                    XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z_Rev, &dab.ILoop.Feedback, 0, &dab.ILoop.Output);//force I ref to 0, clipping V
                }
                else
                {
                    XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z_Rev, &dab.ILoop.Feedback, ILoopReference, &dab.ILoop.Output);   
                }   
        }
        
        // Control loop output mapped to control phase. Normalized to Duty Cycle value
        dab.Pwm.ControlPhase = (((uint32_t)(dab.Pwm.ControlDutyCycle) * 
                (uint32_t)dab.ILoop.Output) >> 15); //range 0..180
        dab.Pwm.ControlPhase += MIN_PHASE_SHIFTED_PULSE;
        
        // Clamping value of maximum control phase
        if(dab.Pwm.ControlPhase > (dab.Pwm.ControlDutyCycle - MAX_PWM_DEAD_TIME))//MIN_PHASE_SHIFTED_PULSE))
            dab.Pwm.ControlPhase = dab.Pwm.ControlDutyCycle - MAX_PWM_DEAD_TIME;//MIN_PHASE_SHIFTED_PULSE;
        
        // Clamping value of minimum control phase 
         else if(dab.Pwm.ControlPhase < MIN_PHASE_SHIFTED_PULSE) 
            dab.Pwm.ControlPhase = MIN_PHASE_SHIFTED_PULSE;  

        DPD_TP31_SetLow();//Test/dbg purpose only
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Updates the Adaptive gain for the power converter control loop 
 * @return void
 * 
 * @details AGC is a specific method for tuning the overall feedback loop gain 
 * during runtime. This function handles the calculation of the gain at a particular 
 * input voltage. AGC is active when it is above the minimum AGC input voltage.
 *********************************************************************************/
static void PwrCtrl_AdaptiveGainUpdate(void)
{
   
    if(dab.PowerDirection == PWR_CTRL_CHARGING)
    { 
        // Calculate the primary voltage in terms of Volts
        uint16_t DAB_PrimaryVoltage = __builtin_divud((vPrimAveraging.AverageValue * VPRI_SCALER), VPRI_FACTOR);
        
        // Apply AGC when primary voltage is greater than the minimum VIN AGC threshold 
        if(DAB_PrimaryVoltage > AGC_MINIMUM_VIN_THRESHOLD)
            dab.ILoop.AgcFactor = (int16_t) (0x7FFF & 
                    __builtin_divud(AGC_VOLTAGE_FACTOR, DAB_PrimaryVoltage));
        else // AGC is not active
            dab.ILoop.AgcFactor = 0x7FFF;
    }
    
    
    if(dab.PowerDirection == PWR_CTRL_DISCHARGING)
    { 
        // Calculate the secondary voltage in terms of Volts
        uint16_t DAB_SecondaryVoltage = __builtin_divud((vSecAveraging.AverageValue * VSEC_SCALER), VSEC_FACTOR);
        
        // Apply AGC when secondary voltage is greater than the minimum VIN AGC threshold 
        if(DAB_SecondaryVoltage > AGC_MINIMUM_VIN_THRESHOLD_SEC)
            dab.ILoop.AgcFactor = (int16_t) (0x7FFF & 
                    __builtin_divud(AGC_VOLTAGE_FACTOR_SEC, DAB_SecondaryVoltage));
        else // AGC is not active
            dab.ILoop.AgcFactor = 0x7FFF;
    }
}


/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members with phase values normalized 
 *  in degree
 * @return void
 * 
 * @details This function updates the DAB data members with phase values normalized 
 * in degree. The calculated phase is scaled by 10x to have better phase resolution.
 *********************************************************************************/
void PwrCtrl_PrimToSecPHDegree(void)
{
    // Calculation of Primary and Secondary degrees phase
    // Normalize phase to 0..90.  
    uint32_t buff = ((unsigned long)dab.Pwm.ControlPhase) << DEGREES_PHASE_SCALER;
    uint16_t buf = __builtin_divud( buff ,dab.Pwm.ControlDutyCycle);
    buff = __builtin_muluu(buf, PRI_TO_SEC_PHASE_DEGREES_LIMIT);
    buf = __builtin_divud( buff ,DEGREES_PHASE_SCALING_10);

    // Calculation result for phase value scaled by 10x 
    dab.Pwm.ControlPhase_P2S_Degreex10 = buf;
    
    // Clamping value for degrees phase when it exceeds the 90.0 degrees  
    if(dab.Pwm.ControlPhase_P2S_Degreex10 >= 900)
    { 
        // Override the calculated control phase value
        dab.Pwm.ControlPhase_P2S_Degreex10 = 899;
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members dead time based on load. 
 * @return void
 * 
 * @details This function updates the DAB data members dead time based on load. 
 * The phase and dead-time in this function is based on the actual board test.
 *********************************************************************************/
void PwrCtrl_DeadTimeAdjust(void)
{
        uint16_t NewDT = 0;   
        
        // Deadtime values adjusted on rough phase range. experimentally measured values.
        // If the control phase is less than 29.0 degrees phase
        if  (dab.Pwm.ControlPhase_P2S_Degreex10 < 290)                                          
            {NewDT = 2000;} // Equivalent to 500ns DeadTime
        // If the control phase is within the range of 33.0 and 80.0 degrees phase
        else if ((dab.Pwm.ControlPhase_P2S_Degreex10 > 330)&&(dab.Pwm.ControlPhase_P2S_Degreex10 < 800)) 
            {NewDT = 1500;} // Equivalent to 375ns DeadTime
        // If the control phase is within the range of 80.5 and 87.5 degrees phase
        else if ((dab.Pwm.ControlPhase_P2S_Degreex10 > 805)&&(dab.Pwm.ControlPhase_P2S_Degreex10 < 875)) 
            {NewDT = 700;} // Equivalent to 175ns DeadTime
        // If the control phase is greater than 88.0 degrees phase
        else if  (dab.Pwm.ControlPhase_P2S_Degreex10 > 880)                                           
            {NewDT = 600;} // Equivalent to 150ns DeadTime

        // When new dead-time did not satisfy any condition in degrees phase
        //retain the previous dead-time value
        if(NewDT)
        {
            // Minimum clamping value of dead-time
            if (NewDT < MINIMUM_DEADTIME) 
                NewDT = MINIMUM_DEADTIME; 
            
            // Write the new dead-time to the PWM
            dab.Pwm.DeadTimeLow = NewDT;
            dab.Pwm.DeadTimeHigh = NewDT;

            // Clear NewDT variable
            NewDT=0;
        }
}

#if defined (PERIOD_MODULATION_DEMO) && (PERIOD_MODULATION_DEMO == true)
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members and modulates PWM period
 * @return void
 * 
 * @details Modulates the DAB period when the maximum phase has been reached.
 *********************************************************************************/
void  PwrCtrl_PeriodModulator(void)
{
    static uint16_t decimPM;
    decimPM++;
    
    if(dab.Pwm.ControlPhase_P2S_Target > PRI_TO_SEC_PHASE_TARGET) 
        dab.Pwm.ControlPhase_P2S_Target = PRI_TO_SEC_PHASE_TARGET;//clamp cutoff while modulating period
    if(dab.Pwm.ControlPhase_P2S_Target == 0) 
        dab.Pwm.ControlPhase_P2S_Target = PRI_TO_SEC_PHASE_TARGET;
        
    if(decimPM>=8) 
    {
        if (dab.Pwm.ControlPhase_P2S_Degreex10 < (dab.Pwm.ControlPhase_P2S_Target-5))
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

        if (dab.Pwm.ControlPhase_P2S_Degreex10 > (dab.Pwm.ControlPhase_P2S_Target + 5))
        {  
            if((dab.Pwm.ControlPeriod < MAX_PWM_PERIOD) && (dab.Pwm.ControlPhase_P2S_Degreex10  > 20))
            {    
                dab.Pwm.ControlPeriod+= PERIODSTEP;
            }
        }

        if(dab.Pwm.LowPowerSlowMode == 1)
        {    
            if (dab.Pwm.ControlPhase_P2S_Degreex10 > 660)
            {    
                dab.Pwm.LowPowerSlowMode = 0;
            }
            else
            {    
                if(dab.Pwm.ControlPeriod < MAX_PWM_PERIOD)
                {  
                    dab.Pwm.ControlPeriod += PERIODSTEP;
                }
                else
                if (dab.Pwm.ControlPhase_P2S_Degreex10 > 440 )//snap out
                {
                    dab.Pwm.LowPowerSlowMode = 0;
                }
            }
        }
        decimPM=0;
    }
}

#endif
