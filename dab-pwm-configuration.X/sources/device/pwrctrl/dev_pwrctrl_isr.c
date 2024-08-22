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
#include "timer/sccp1.h"
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "config/macros.h"
#include "dev_pwrctrl_pwm.h"
#include "device/fault/dev_fault.h"
#include "dcdt/dev_pwrctrl_dcdt.h"
#include "system/pins.h"
#include "dev_pwrctrl_utils.h"

// PRIVATE FUNCTIONS
static void Dev_PwrCtrl_UpdateConverterData(void);
static void Dev_PwrCtrl_ControlLoopExecute(void);

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

/*******************************************************************************
 * @ingroup dev-pwrctrl-isr-methods-public
 * @brief  Executes the power conveter's control loop
 * @return void
 * 
 * @details This interrupt function is a timing interrupt executed every 100KHz
 *   and calls an ADC function that updates the DAB data member with ADC raw values. 
 *   The fault check is also called in this function to detect any fault events.
 *   It is followed by the control loop execution for Voltage, Current and Power
 *   Loop Controllers. The end result of the control loop is handed over to the 
 *   PWM distribution. 
 *********************************************************************************/

void ControlLoop_Interrupt_CallBack(void)
{      
    GPIO_1_SetHigh();
            
    // Update the ADC data member
    Dev_PwrCtrl_UpdateConverterData();
    
    // Execute the fault detection
    Dev_Fault_Execute();
    
    #if(OPEN_LOOP_PBV == false)
    // Execute Power Converter Control Loop
    Dev_PwrCtrl_ControlLoopExecute();
    #endif

    #if(DAC_DEBUG == true)
    // This is for DAC debugging purposes
//    CMP2_DACDataWrite(dab.VLoop.Reference);
//    CMP2_DACDataWrite(dab.ILoop.Reference << 2);
    CMP2_DACDataWrite(dab.PLoop.Reference >> 2);
    #endif

    #if (true == DPDB_TEST_RUN)

    // Connect TP11 -> Pot1 & TP12 -> Pot2 in digital Power Development Board
    uint16_t Pot1 = dab.Data.ISecAverage; 
    uint16_t Pot2 = dab.Data.ISenseSecondary; 
    
    // Calculate the Frequency based on the Potentiometer 1 voltage
    dab.Pwm.ControlPeriod = (uint16_t)(MIN_PWM_PERIOD + (Pot1 * ADC_PERIOD_RANGE)); 
    
    // Change the control Phase with Potentiometer 2
    dab.Pwm.ControlPhase = (uint16_t)(Pot2 * ADC_SCALER * (dab.Pwm.ControlPeriod >> 1));
    
    #endif

    // Update PWM Properties
    Dev_PwrCtrl_PWM_Update(&dab);
    
    GPIO_1_SetLow();
    
}
    
/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-public
 * @brief  This function updates the DAB data members with ADC raw values
 * @return void
 * 
 * @details This function is called every 100KHz and triggers the ADC module. This
 *  also handles the updating of DAB data members with its latest ADC raw values
 *  and collection of data for averaging.
 *********************************************************************************/
static void Dev_PwrCtrl_UpdateConverterData (void)
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
    
    dab.Data.VRail_5V = ADC1_ConversionResultGet(VRAIL_5V);
    dab.Data.Temperature = ADC1_ConversionResultGet(TEMP);
    
}

/*******************************************************************************
 * @ingroup dev-pwrctrl-methods-public
 * @brief Executes the power converter control loop 
 * @return void
 * 
 * @details This function is called to execute the control loop of the power
 *  converter. It comprise of Voltage Loop control (VLoop), Power Loop control (PLoop) 
 *  and Current Loop Control (ILoop). Vloop and PLoop is ten times slower than
 *  the current loop with interleaved execution while Iloop is executed everytime 
 *  this function is called.  
 *********************************************************************************/
static void Dev_PwrCtrl_ControlLoopExecute(void)
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
            dab.VLoop.Feedback = VsecAveraging.AverageValue << 4;  
            dab.VLoop.Reference = dab.VLoop.Reference << 4;

            // Execute the Voltage Loop Control
            SMPS_Controller2P2ZUpdate(&VMC_2p2z, &dab.VLoop.Feedback,
                    dab.VLoop.Reference, &dab.VLoop.Output);
        
            // Reset the Vloop reference to its original scaling
            dab.VLoop.Reference = dab.VLoop.Reference >> 4;
        }
    }

    // Execute the Power Loop Control
    if((dab.PLoop.Enable == true) && (VLoopExec == false))
    {      
        VLoopExec = true;

        //Bit-shift value used to perform input value normalization
        uint32_t buf = (uint32_t)IsecAveraging.AverageValue * 
                (uint32_t)VsecAveraging.AverageValue  * 131; //131; 131;  //0.0079*10000 ==  131 / 16384
        buf >>=14;
        
        dab.Data.SecPower = buf;
         
        dab.PLoop.Feedback = dab.Data.PowerOffset + (dab.Data.SecPower);
        dab.PLoop.Reference = dab.Data.PowerOffset + dab.PLoop.Reference;
        
        // Execute the Power Loop Control
        SMPS_Controller2P2ZUpdate(&PMC_2p2z, &dab.PLoop.Feedback,
                dab.PLoop.Reference, &dab.PLoop.Output);
        
        // Reset the Ploop reference to its original scaling and offset
        dab.PLoop.Reference = dab.PLoop.Reference - dab.Data.PowerOffset;
    }

    // Execute the Current Loop Control
    if(dab.ILoop.Enable == true)
    {
        //Bit-shift value used to perform input value normalization
        dab.ILoop.Feedback = dab.Data.ISecAverageRectified << 3;
        //adaptive gain factor
        IMC_2p2z.KfactorCoeffsB = 0x7FFF;//dab.ILoop.AgcFactor;
        //refresh limits
        IMC_2p2z.maxOutput =  0x7FFF;

        // Mixing stage from voltage loop 10KHz
        uint32_t RefBuf = (uint32_t)(dab.ILoop.Reference) * 
                (uint32_t)(dab.VLoop.Output & 0x7FFF);
        uint16_t ILoopReference = (uint16_t)(RefBuf>>12) ; //15-3

        // Mixing stage from power loop 10KHz
        RefBuf =  (uint32_t)ILoopReference * (uint32_t)(dab.PLoop.Output & 0x7FFF);  
        ILoopReference = (int16_t)(RefBuf >> 15);    
         
        XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback,   
                ILoopReference, &dab.ILoop.Output);    

        // Control loop output copied to control phase
        dab.Pwm.ControlPhase = (((uint32_t)(dab.Pwm.ControlDutyCycle) * 
                (uint32_t)dab.ILoop.Output) >> 15); //range 0..180
    }
}