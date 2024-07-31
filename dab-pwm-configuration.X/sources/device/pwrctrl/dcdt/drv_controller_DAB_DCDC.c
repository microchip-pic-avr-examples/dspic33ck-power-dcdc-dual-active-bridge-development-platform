/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    dev_TPBLPFC_typedef.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CH512MP506S1
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v6.00
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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

/**
  Section: Included Files
 */

#include <xc.h>

#include "drv_controller_DAB_DCDC.h"
#include "../../../mcc_generated_files/peripheral/pwm.h"

//dab_sp   sec to prim, discharger
//dab_ps   prim to sec, charger
//
//io  output current   fs 100kHz
//vo  output voltage   fs  10kHz
//po  output power     fs   1kHz
//fo  frequency modulation for opearting point soft switch improvememnt

SMPS_2P2Z_T VMC_2p2z;//Charger
SMPS_2P2Z_T IMC_2p2z;
SMPS_2P2Z_T FMC_2p2z;//nrnd
SMPS_2P2Z_T PMC_2p2z; 


SMPS_2P2Z_T VMD_2p2z;//Voltage mode Discharger
SMPS_2P2Z_T IMD_2p2z;
SMPS_2P2Z_T FMD_2p2z;//nrnd
SMPS_2P2Z_T PMD_2p2z; 
////======================================================================================================================
////@brief SMPS_2P2Z_T VMC_2p2z //typedef from smps_control.h to var
////======================================================================================================================
//
//int16_t   VMCTP_2p2zACoefficients[2]__attribute__((space(xmemory)));
//int16_t   VMCTP_2p2zBCoefficients[3] __attribute__((space(xmemory)));
//int16_t   VMCTP_2p2zControlHistory[2] __attribute__((space(ymemory), far));
//int16_t   VMCTP_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
//uint16_t  VMCTP_2p2zReferenceSet;



//charger
//======================================================================================================================
//@brief SMPS_2P2Z_T VMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t   VMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   VMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   VMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   VMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  VMC_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   IMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   IMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   IMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   IMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  IMC_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   FMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   FMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   FMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   FMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  FMC_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   PMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   PMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   PMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   PMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  PMC_2p2zReferenceSet;


//Discharger
//======================================================================================================================
//@brief SMPS_2P2Z_T VMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t   VMD_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   VMD_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   VMD_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   VMD_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  VMD_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   IMD_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   IMD_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   IMD_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   IMD_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  IMD_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   FMD_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   FMD_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   FMD_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   FMD_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  FMD_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   PMD_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   PMD_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   PMD_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   PMD_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
uint16_t  PMD_2p2zReferenceSet;


DAB_CONTROLLER_t DAB;
CAN_x603_FRAME_s CAN_x603_FRAME;//DAB board specific GUI
CAN_x603_FRAME_s *pCAN_x603_FRAME = &CAN_x603_FRAME;


inline void VoltageCtrl_Compensator_Handler(void)
{
    VMC_2p2z.KfactorCoeffsB = 0x7FFF;//DAB.Adaptive_Gain_Factor; 
    VMC_2p2z.maxOutput =  0x7FFF;//DAB.PWM_DutyCycle;
    
    if(DAB.PowerDirection == DAB_CHARGER)
    { 
        VMC_2p2z.KfactorCoeffsB = 0x7FFF;//DAB.Adaptive_Gain_Factor; 
        VMC_2p2z.maxOutput =  0x7FFF;//DAB.PWM_DutyCycle;
        XFT_SMPS_Controller2P2ZUpdate(&VMC_2p2z, 
                                  &DAB.VMControllerInputRegister, 
                                   DAB.VMreference2p2z,
                                  &DAB.VMControllerOutputRegister);
    }
    
    if(DAB.PowerDirection == DAB_DISCHARGER)
    { 
        VMD_2p2z.KfactorCoeffsB = 0x7FFF;//DAB.Adaptive_Gain_Factor; 
        VMD_2p2z.maxOutput =  0x7FFF;//DAB.PWM_DutyCycle;
        XFT_SMPS_Controller2P2ZUpdate(&VMD_2p2z, 
                                  &DAB.VMControllerInputRegister, 
                                   DAB.VMreference2p2z,
                                  &DAB.VMControllerOutputRegister);
    }
}

inline void CurrentCtrl_Compensator_Handler(void)
{
    uint32_t RefBuf;//used for some intermediate steps only. to be redefined later
    
    //DAB_PSIO
    // 16000 = 0.5 fract; 3276 = 0.1 fract 
    // Gain should be within 0.1 and 0.5
    // compensator calculated with Gain of 10
    //IMC_2p2z.KfactorCoeffsB = 0x7FFF;//DAB.Adaptive_Gain_Factor; //Adaptive_Gain_Factor = 3276;  //factor = 0.1
    
    
    
    //adaptive gain parameter refresh
    IMC_2p2z.KfactorCoeffsB = DAB.Adaptive_Gain_Factor;
    //refresh limits
    IMC_2p2z.maxOutput =  0x7FFF;//DAB.PWM_DutyCycle; 
    
    
    //mixing stage from voltage loop 10khz
    RefBuf = (uint32_t)DAB.IMreference * (uint32_t)(DAB.VMControllerOutputRegister & 0x7FFF);
    DAB.IMreference2p2z = (uint16_t)(RefBuf>>12) ; //15-3
            
    //DAB.IMreference2p2z = DAB.IMreference<<3;
    
    //mixing stage from power loop 10khz. Future reserved!
    RefBuf =  (uint32_t)DAB.IMreference2p2z * (uint32_t)(DAB.PMControllerOutputRegister & 0x7FFF);  
    DAB.IMreference2p2z = (int16_t)(RefBuf>>15 );        
            
    
    //hard clipping tests DEMO only
    //redundant Voltage Clipping if other params go out of range during tests. Avoid Caps exploding
    //Voltage 2p2z loop parameters may not be final
//    if(DAB.PowerDirection == DAB_DISCHARGER)
//    {    
//      if(DAB.PrimaryVoltage >= 50) DAB.IMreference2p2z = 2;
//    }
//    if(DAB.PowerDirection == DAB_CHARGER)
//    {    
//      if(DAB.SecondaryVoltage >= 52) DAB.IMreference2p2z = 1;
//    }
    
    
     //update compensator        
    XFT_SMPS_Controller2P2ZUpdate(&IMC_2p2z, //same compensator for charge/discharge directions
                              &DAB.IMControllerInputRegister, 
                               DAB.IMreference2p2z,
                              &DAB.IMControllerOutputRegister);
    
}


inline void FreqCtrl_Compensator_Handler(void)//not used
{
//    FMC_2p2z.minOutput = 0;
    //FMC_2p2z.maxOutput = 0xEF00 - 0x2800; //C700
    
    SMPS_Controller2P2ZUpdate(&FMC_2p2z, 
                             &DAB.FMControllerInputRegister,
                              DAB.FMreference,
                             &DAB.FMControllerOutputRegister);

}

inline void PowCtrl_Compensator_Handler(void)
{
//    FMC_2p2z.minOutput = 0;
    //FMC_2p2z.maxOutput = 0xEF00 - 0x2800; //C700
    if(DAB.PowerDirection == DAB_CHARGER)
    { 
        SMPS_Controller2P2ZUpdate(&PMC_2p2z, //charger
                                  &DAB.PMControllerInputRegister,
                                   DAB.PMreference2p2z,
                                  &DAB.PMControllerOutputRegister);
    }
    if(DAB.PowerDirection == DAB_DISCHARGER)
    { 
        SMPS_Controller2P2ZUpdate(&PMD_2p2z,  //discharger 
                                  &DAB.PMControllerInputRegister,
                                   DAB.PMreference2p2z,
                                  &DAB.PMControllerOutputRegister);
    }
}



void VoltageCtrl_DAB_Init(void)
{
    
  //charger direction  
   VMC_2p2z.aCoefficients =  &VMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   VMC_2p2z.bCoefficients =  &VMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   VMC_2p2z.controlHistory = &VMC_2p2zControlHistory[0]; // Set up pointer to controller history
   VMC_2p2z.errorHistory =   &VMC_2p2zErrorHistory[0]; // Set up pointer to error history

   VMC_2p2z.preShift =   DAB_SPVO_COMP_2P2Z_PRESHIFT;  //to be fixed later into SPVO version. historical evolving reasons 
   VMC_2p2z.postScaler = DAB_SPVO_COMP_2P2Z_POSTSCALER;
   VMC_2p2z.postShift =  DAB_SPVO_COMP_2P2Z_POSTSHIFT;
   VMC_2p2z.minOutput =  DAB_SPVO_COMP_2P2Z_MIN_CLAMP;
   VMC_2p2z.maxOutput =  DAB_SPVO_COMP_2P2Z_MAX_CLAMP; 
   
   VMC_2p2zACoefficients[0] = DAB_SPVO_COMP_2P2Z_COEFF_A1;
   VMC_2p2zACoefficients[1] = DAB_SPVO_COMP_2P2Z_COEFF_A2;
   VMC_2p2zBCoefficients[0] = DAB_SPVO_COMP_2P2Z_COEFF_B0;
   VMC_2p2zBCoefficients[1] = DAB_SPVO_COMP_2P2Z_COEFF_B1;
   VMC_2p2zBCoefficients[2] = DAB_SPVO_COMP_2P2Z_COEFF_B2;
  
   //please fix here
//   VMC_2p2z.preShift =   DAB_PSVO_COMP_2P2Z_PRESHIFT;
//   VMC_2p2z.postScaler = DAB_PSVO_COMP_2P2Z_POSTSCALER;
//   VMC_2p2z.postShift =  DAB_PSVO_COMP_2P2Z_POSTSHIFT;
//   VMC_2p2z.minOutput =  DAB_PSVO_COMP_2P2Z_MIN_CLAMP;
//   VMC_2p2z.maxOutput =  DAB_PSVO_COMP_2P2Z_MAX_CLAMP; 
//   
//   VMC_2p2zACoefficients[0] = DAB_PSVO_COMP_2P2Z_COEFF_A1;
//   VMC_2p2zACoefficients[1] = DAB_PSVO_COMP_2P2Z_COEFF_A2;
//   VMC_2p2zBCoefficients[0] = DAB_PSVO_COMP_2P2Z_COEFF_B0;
//   VMC_2p2zBCoefficients[1] = DAB_PSVO_COMP_2P2Z_COEFF_B1;
//   VMC_2p2zBCoefficients[2] = DAB_PSVO_COMP_2P2Z_COEFF_B2; 
  
  
   //explicit clear to 0
  VMC_2p2zControlHistory[0] =0;
  VMC_2p2zControlHistory[1] =0;
  VMC_2p2zErrorHistory[0] =0;
  VMC_2p2zErrorHistory[1] =0;
  VMC_2p2zErrorHistory[2] =0;
  
  VMC_2p2zReferenceSet = 0;
   
  SMPS_Controller2P2ZInitialize(&VMC_2p2z);  
   
   
   
   //discharger direction
   VMD_2p2z.aCoefficients =  &VMD_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   VMD_2p2z.bCoefficients =  &VMD_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   VMD_2p2z.controlHistory = &VMD_2p2zControlHistory[0]; // Set up pointer to controller history
   VMD_2p2z.errorHistory =   &VMD_2p2zErrorHistory[0]; // Set up pointer to error history

   VMD_2p2z.preShift =   DAB_SPVO_COMP_2P2Z_PRESHIFT;
   VMD_2p2z.postScaler = DAB_SPVO_COMP_2P2Z_POSTSCALER;
   VMD_2p2z.postShift =  DAB_SPVO_COMP_2P2Z_POSTSHIFT;
   VMD_2p2z.minOutput =  DAB_SPVO_COMP_2P2Z_MIN_CLAMP;
   VMD_2p2z.maxOutput =  DAB_SPVO_COMP_2P2Z_MAX_CLAMP; 
   
   VMD_2p2zACoefficients[0] = DAB_SPVO_COMP_2P2Z_COEFF_A1;
   VMD_2p2zACoefficients[1] = DAB_SPVO_COMP_2P2Z_COEFF_A2;
   VMD_2p2zBCoefficients[0] = DAB_SPVO_COMP_2P2Z_COEFF_B0;
   VMD_2p2zBCoefficients[1] = DAB_SPVO_COMP_2P2Z_COEFF_B1;
   VMD_2p2zBCoefficients[2] = DAB_SPVO_COMP_2P2Z_COEFF_B2;
  
   //explicit clear to 0
   VMD_2p2zControlHistory[0] =0;
   VMD_2p2zControlHistory[1] =0;
   VMD_2p2zErrorHistory[0] =0;
   VMD_2p2zErrorHistory[1] =0;
   VMD_2p2zErrorHistory[2] =0;
  
   VMD_2p2zReferenceSet = 0;
   
   SMPS_Controller2P2ZInitialize(&VMD_2p2z);  
   
   
   
}



void CurrentCtrl_DAB_Init(void)
{
   //charger
   IMC_2p2z.aCoefficients =  &IMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   IMC_2p2z.bCoefficients =  &IMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   IMC_2p2z.controlHistory = &IMC_2p2zControlHistory[0]; // Set up pointer to controller history
   IMC_2p2z.errorHistory =   &IMC_2p2zErrorHistory[0]; // Set up pointer to error history

   IMC_2p2z.preShift =   DAB_PSIO_COMP_2P2Z_PRESHIFT;
   IMC_2p2z.postScaler = DAB_PSIO_COMP_2P2Z_POSTSCALER;
   IMC_2p2z.postShift =  DAB_PSIO_COMP_2P2Z_POSTSHIFT;
   IMC_2p2z.minOutput =  DAB_PSIO_COMP_2P2Z_MIN_CLAMP;
   IMC_2p2z.maxOutput =  DAB_PSIO_COMP_2P2Z_MAX_CLAMP;
   
   
   IMC_2p2zACoefficients[0] = DAB_PSIO_COMP_2P2Z_COEFF_A1;
   IMC_2p2zACoefficients[1] = DAB_PSIO_COMP_2P2Z_COEFF_A2;
   IMC_2p2zBCoefficients[0] = DAB_PSIO_COMP_2P2Z_COEFF_B0;
   IMC_2p2zBCoefficients[1] = DAB_PSIO_COMP_2P2Z_COEFF_B1;
   IMC_2p2zBCoefficients[2] = DAB_PSIO_COMP_2P2Z_COEFF_B2;
  
  //explicit clear to 0
   IMC_2p2zControlHistory[0] =0;//1830;
   IMC_2p2zControlHistory[1] =0;//1830;
   IMC_2p2zErrorHistory[0] =0;
   IMC_2p2zErrorHistory[1] =0;
   IMC_2p2zErrorHistory[2] =0;
  
   IMC_2p2zReferenceSet = 0;    
   DAB.IMreference = 0;
   
   SMPS_Controller2P2ZInitialize(&IMC_2p2z);
   
//   //discharger   .  DCDT project must generate h files. 
//   IMD_2p2z.aCoefficients =  &IMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
//   IMD_2p2z.bCoefficients =  &IMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
//   IMD_2p2z.controlHistory = &IMC_2p2zControlHistory[0]; // Set up pointer to controller history
//   IMD_2p2z.errorHistory =   &IMC_2p2zErrorHistory[0]; // Set up pointer to error history
//
//   IMD_2p2z.preShift =   DAB_SPIO_COMP_2P2Z_PRESHIFT;
//   IMD_2p2z.postScaler = DAB_SPIO_COMP_2P2Z_POSTSCALER;
//   IMD_2p2z.postShift =  DAB_SPIO_COMP_2P2Z_POSTSHIFT;
//   IMD_2p2z.minOutput =  DAB_SPIO_COMP_2P2Z_MIN_CLAMP;
//   IMD_2p2z.maxOutput =  DAB_SPIO_COMP_2P2Z_MAX_CLAMP;
//   
//   
//   IMD_2p2zACoefficients[0] = DAB_SPIO_COMP_2P2Z_COEFF_A1;
//   IMD_2p2zACoefficients[1] = DAB_SPIO_COMP_2P2Z_COEFF_A2;
//   IMD_2p2zBCoefficients[0] = DAB_SPIO_COMP_2P2Z_COEFF_B0;
//   IMD_2p2zBCoefficients[1] = DAB_SPIO_COMP_2P2Z_COEFF_B1;
//   IMD_2p2zBCoefficients[2] = DAB_SPIO_COMP_2P2Z_COEFF_B2;
//  
//  //explicit clear to 0
//   IMD_2p2zControlHistory[0] = 0;
//   IMD_2p2zControlHistory[1] = 0;
//   IMD_2p2zErrorHistory[0] = 0;
//   IMD_2p2zErrorHistory[1] = 0;
//   IMD_2p2zErrorHistory[2] = 0;
//  
//   IMD_2p2zReferenceSet = 0;    
//   DAB.IMreference = 0;
//   
//   SMPS_Controller2P2ZInitialize(&IMD_2p2z);
   
   
}


void FreqCtrl_DAB_Init(void)//not used
{
   FMC_2p2z.aCoefficients =  &FMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   FMC_2p2z.bCoefficients =  &FMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   FMC_2p2z.controlHistory = &FMC_2p2zControlHistory[0]; // Set up pointer to controller history
   FMC_2p2z.errorHistory =   &FMC_2p2zErrorHistory[0]; // Set up pointer to error history

   FMC_2p2z.preShift =   DAB_FM_COMP_2P2Z_PRESHIFT;
   FMC_2p2z.postScaler = DAB_FM_COMP_2P2Z_POSTSCALER;
   FMC_2p2z.postShift =  DAB_FM_COMP_2P2Z_POSTSHIFT;
   FMC_2p2z.minOutput =  DAB_FM_COMP_2P2Z_MIN_CLAMP;
   FMC_2p2z.maxOutput =  DAB_FM_COMP_2P2Z_MAX_CLAMP; 
   
   FMC_2p2zACoefficients[0] = DAB_FM_COMP_2P2Z_COEFF_A1;
   FMC_2p2zACoefficients[1] = DAB_FM_COMP_2P2Z_COEFF_A2;
   FMC_2p2zBCoefficients[0] = DAB_FM_COMP_2P2Z_COEFF_B0;
   FMC_2p2zBCoefficients[1] = DAB_FM_COMP_2P2Z_COEFF_B1;
   FMC_2p2zBCoefficients[2] = DAB_FM_COMP_2P2Z_COEFF_B2;
  
    //explicit clear to 0
   FMC_2p2zControlHistory[0] =1860;
   FMC_2p2zControlHistory[1] =1860;
   FMC_2p2zErrorHistory[0] =0;
   FMC_2p2zErrorHistory[1] =0;
   FMC_2p2zErrorHistory[2] =0;
   
   FMC_2p2zReferenceSet = 0;
    
   SMPS_Controller2P2ZInitialize(&FMC_2p2z);  
}



void PowCtrl_DAB_Init(void)  //onla battery has power sense option. should be bidirectional
{
    //charger
   PMC_2p2z.aCoefficients =  &PMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   PMC_2p2z.bCoefficients =  &PMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   PMC_2p2z.controlHistory = &PMC_2p2zControlHistory[0]; // Set up pointer to controller history
   PMC_2p2z.errorHistory =   &PMC_2p2zErrorHistory[0]; // Set up pointer to error history

   PMC_2p2z.preShift =   DAB_PSPO_COMP_2P2Z_PRESHIFT;
   PMC_2p2z.postScaler = DAB_PSPO_COMP_2P2Z_POSTSCALER;
   PMC_2p2z.postShift =  DAB_PSPO_COMP_2P2Z_POSTSHIFT;
   PMC_2p2z.minOutput =  DAB_PSPO_COMP_2P2Z_MIN_CLAMP;
   PMC_2p2z.maxOutput =  DAB_PSPO_COMP_2P2Z_MAX_CLAMP; 
   
   PMC_2p2zACoefficients[0] = DAB_PSPO_COMP_2P2Z_COEFF_A1;
   PMC_2p2zACoefficients[1] = DAB_PSPO_COMP_2P2Z_COEFF_A2;
   PMC_2p2zBCoefficients[0] = DAB_PSPO_COMP_2P2Z_COEFF_B0;
   PMC_2p2zBCoefficients[1] = DAB_PSPO_COMP_2P2Z_COEFF_B1;
   PMC_2p2zBCoefficients[2] = DAB_PSPO_COMP_2P2Z_COEFF_B2;
  
    //explicit clear to 0
   PMC_2p2zControlHistory[0] =0;
   PMC_2p2zControlHistory[1] =0;
   PMC_2p2zErrorHistory[0] =0;
   PMC_2p2zErrorHistory[1] =0;
   PMC_2p2zErrorHistory[2] =0;
   
   PMC_2p2zReferenceSet = 0;
    
   SMPS_Controller2P2ZInitialize(&PMC_2p2z);  
   
   //discharger
   PMD_2p2z.aCoefficients =  &PMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   PMD_2p2z.bCoefficients =  &PMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   PMD_2p2z.controlHistory = &PMC_2p2zControlHistory[0]; // Set up pointer to controller history
   PMD_2p2z.errorHistory =   &PMC_2p2zErrorHistory[0]; // Set up pointer to error history

   PMD_2p2z.preShift =   DAB_PSPO_COMP_2P2Z_PRESHIFT;//later may become SPPO
   PMD_2p2z.postScaler = DAB_PSPO_COMP_2P2Z_POSTSCALER;
   PMD_2p2z.postShift =  DAB_PSPO_COMP_2P2Z_POSTSHIFT;
   PMD_2p2z.minOutput =  DAB_PSPO_COMP_2P2Z_MIN_CLAMP;
   PMD_2p2z.maxOutput =  DAB_PSPO_COMP_2P2Z_MAX_CLAMP; 
   
   PMD_2p2zACoefficients[0] = DAB_PSPO_COMP_2P2Z_COEFF_A1;
   PMD_2p2zACoefficients[1] = DAB_PSPO_COMP_2P2Z_COEFF_A2;
   PMD_2p2zBCoefficients[0] = DAB_PSPO_COMP_2P2Z_COEFF_B0;
   PMD_2p2zBCoefficients[1] = DAB_PSPO_COMP_2P2Z_COEFF_B1;
   PMD_2p2zBCoefficients[2] = DAB_PSPO_COMP_2P2Z_COEFF_B2;
  
    //explicit clear to 0
   PMD_2p2zControlHistory[0] =0;
   PMD_2p2zControlHistory[1] =0;
   PMD_2p2zErrorHistory[0] =0;
   PMD_2p2zErrorHistory[1] =0;
   PMD_2p2zErrorHistory[2] =0;
   
   PMD_2p2zReferenceSet = 0;
    
   SMPS_Controller2P2ZInitialize(&PMD_2p2z);  
   
   
}


void Init_pwr_ctrl (void)
{
    DAB.Prim_PWM_Ph_DegreesX10=0;
    DAB.Prim_PWM_Ph_Value=0;

    DAB.PrimToSec_PWM_Ph_DegreesX10=0;
    DAB.PrimToSec_PWM_Ph_Value=0;

    DAB.Sec_PWM_Ph_DegreesX10=0;
    DAB.Sec_PWM_Ph_Value=0;


     //(  Range: 0 max Power .. 20000 min power) 
    DAB.TargetPrimToSecPhase=PRIMTOSEC_TARGET;

    //PERIODMIN (0x4000)
    DAB.PWM_Period =0x4000; //0x9C38;

    DAB.PWM_DutyCycle = 0x4E1C;
    DAB.PWM_DutyCycleHalf = 0x4E1C>>1;          
    DAB.RegulatedValue=DAB.PWM_DutyCycle - 128;

    DAB.IMControllerInputRegister = 0;//1800;
    DAB.IMControllerOutputRegister = 0;//1900;

    DAB.VMControllerInputRegister = 0;//300;
    DAB.VMControllerOutputRegister = 0;
    DAB.VMreference = 0;//300; 
    DAB.IMreference = 0;//1830;
    DAB.FMreference = 0;//1860;     

    MPER = DAB.PWM_Period;
    MDC = DAB.PWM_DutyCycle;

    DAB.Prim_PWM_Ph_Value = DAB.PWM_DutyCycle-128;//min 20000 ..max 0
    DAB.Sec_PWM_Ph_Value = DAB.PWM_DutyCycle-128;//min 20000 ..max 0
    DAB.PrimToSec_PWM_Ph_Value = 0;//0°=360°
    DAB.RegulatedValue = DAB.PWM_DutyCycle - 128;

    PG1TRIGC = DAB.Prim_PWM_Ph_Value;
    PG2TRIGA = DAB.Sec_PWM_Ph_Value; 
    PG1TRIGA = DAB.PrimToSec_PWM_Ph_Value;      
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

