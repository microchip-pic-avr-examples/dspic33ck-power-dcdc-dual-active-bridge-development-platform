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

#include "dev_pwrctrl.h"
#include "timer/sccp1.h"
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "config/macros.h"
#include "dev_pwrctrl_pwm.h"
#include "device/fault/dev_fault.h"
#include "dcdt/dev_pwrctrl_dcdt.h"
#include "system/pins.h"

void Dev_PwrCtrl_UpdateConverterData(void);
void Dev_PwrCtrl_ControlLoopExecute(void);

/*********************************************************************************
 * @ingroup 
 * @fn      void SCCP1_TimeoutCallback(void)
 * @brief   ISR in which the control loop is executed. 
 * @param   none
 * @return  none
 * @details
 * execute control loop
 * manage shared ADC core
 * ADC fault protection
 **********************************************************************************/
void ControlLoop_Interrupt_CallBack(void)
{      
    GPIO_1_SetHigh();
            
    // Update the ADC data member
    Dev_PwrCtrl_UpdateConverterData();
    
    // Execute the fault detection
    Dev_Fault_Execute();
    
    #if(OPEN_LOOP_PBV == false)
    Dev_PwrCtrl_ControlLoopExecute();
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

    #if (DAC_DEBUG== true)
//    CMP2_DACDataWrite(dab.VLoop.Reference);
//    CMP2_DACDataWrite(dab.PLoop.Reference >> 2);
    CMP2_DACDataWrite(dab.ILoop.Reference);
    #endif  
    // Update PWM Properties
    Dev_PwrCtrl_PWM_Update(&dab);
    GPIO_1_SetLow();
    
}
    
/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_UpdateConverterData (void)
{
    ADC1_SoftwareTriggerEnable();
    
    dab.Data.ISecAverage = ADC1_ConversionResultGet(ISEC_AVG); 
    dab.Data.ISenseSecondary = ADC1_ConversionResultGet(ISEC_CT); 
    
    dab.Data.VSecVoltage = ADC1_ConversionResultGet(VSEC);
    dab.Data.ISensePrimary = ADC1_ConversionResultGet(IPRI_CT);   

    dab.Data.VPriVoltage = ADC1_ConversionResultGet(VPRI);
    
    dab.Data.VRail_5V = ADC1_ConversionResultGet(VRAIL_5V);
    dab.Data.Temperature = ADC1_ConversionResultGet(TEMP);
    
}

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_ControlLoopExecute(void)
{
    static uint16_t cnt = 0;
    static bool VLoopExec = true;

    //Interleave the execution of the control loop
    if(++cnt == 5)
    {
        if((dab.VLoop.Enable == false) && (VLoopExec == true)){
            dab.VLoop.Enable = true;
            dab.PLoop.Enable = false;
            
        }
        else if((dab.PLoop.Enable == false) && (VLoopExec == false)){
            dab.VLoop.Enable = false;
            dab.PLoop.Enable = true;
        }
        cnt = 0;
    }
    
    if((dab.VLoop.Enable == true) && (VLoopExec == true))
    {
        
        VLoopExec = false;
        
        // Execute the Voltage Loop Control
        if(dab.PowerDirection == PWR_CTRL_CHARGING)
        { 
            VMC_2p2z.KfactorCoeffsB = 0x7FFF;
            VMC_2p2z.maxOutput =  0x7FFF;

            //Bit-shift value used to perform input value normalization
            //ToDo: check with Lorant if tested with at 700V, if yes, 4 shift bit 
            //exceeds the int value of 35535.
            dab.VLoop.Feedback = dab.Data.VSecVoltage << 4;  
            dab.VLoop.Reference = dab.VLoop.Reference << 4;

            SMPS_Controller2P2ZUpdate(&VMC_2p2z, &dab.VLoop.Feedback,
                    dab.VLoop.Reference, &dab.VLoop.Output);
        
            // reset the Vloop reference to its original scaling
            dab.VLoop.Reference = dab.VLoop.Reference >> 4;
        }
    }

    if((dab.PLoop.Enable == true) && (VLoopExec == false))
    {      
        VLoopExec = true;

        uint32_t isec = (dab.Data.ISecAverage - dab.Data.ISecSensorOffset) >> 2;
        uint32_t vsec = (dab.Data.VSecVoltage * 3) >> 8;

        dab.Data.SecPower = isec * vsec;
            
        // Execute the Power Loop Control
        //ToDo: check with Lorant the meaning of 100 in this code
        //100 is a power offset
        dab.PLoop.Feedback = 100 + (dab.Data.SecPower);
        dab.PLoop.Reference = 100 + dab.PLoop.Reference;
        
        SMPS_Controller2P2ZUpdate(&PMC_2p2z, &dab.PLoop.Feedback,
                dab.PLoop.Reference, &dab.PLoop.Output);
        
        // reset the Vloop reference to its original scaling and offset
        dab.PLoop.Reference = dab.PLoop.Reference - 100;
    }
    
    if(dab.ILoop.Enable == true)
    {
        // Execute the Current Loop Control
        //Bit-shift value used to perform input value normalization
        dab.ILoop.Feedback = dab.Data.ISenseSecondary << 3;
        //adaptive gain factor
        IMC_2p2z.KfactorCoeffsB = dab.ILoop.AgcFactor;
        //refresh limits
        IMC_2p2z.maxOutput =  0x7FFF;

        //mixing stage from voltage loop 10KHz
        uint32_t RefBuf = (uint32_t)dab.ILoop.Reference * (uint32_t)(dab.VLoop.Output & 0x7FFF);
        uint16_t ILoopReference = (uint16_t)(RefBuf>>12) ; //15-3

        //mixing stage from power loop 10KHz
        RefBuf =  (uint32_t)ILoopReference * (uint32_t)(dab.PLoop.Output & 0x7FFF);  
        ILoopReference = (int16_t)(RefBuf >> 15 );    

        XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, &dab.ILoop.Feedback, 
                ILoopReference, &dab.ILoop.Output);    

        Nop();
        Nop();
        Nop();
        
        //control loop output copied to control phase
        dab.Pwm.ControlPhase = (((uint32_t)(dab.Pwm.ControlDutyCycle >> 1) * (uint32_t)dab.ILoop.Output) >> 15);
        
        
    }
}