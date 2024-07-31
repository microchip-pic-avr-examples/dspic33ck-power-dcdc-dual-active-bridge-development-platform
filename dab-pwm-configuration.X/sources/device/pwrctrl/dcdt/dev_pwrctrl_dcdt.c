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

#include "dev_pwrctrl_dcdt.h"
#include "device/pwrctrl/dev_pwrctrl.h"

//dab_sp   sec to prim, discharger
//dab_ps   prim to sec, charger
//
//io  output current   fs 100kHz
//vo  output voltage   fs  10kHz
//po  output power     fs   1kHz
//fo  frequency modulation for opearting point soft switch improvememnt

SMPS_2P2Z_T VMC_2p2z;//Charger
SMPS_2P2Z_T IMC_2p2z;
SMPS_2P2Z_T FMC_2p2z;
SMPS_2P2Z_T PMC_2p2z; 

//charger
//======================================================================================================================
//@brief SMPS_2P2Z_T VMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t   VMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   VMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   VMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   VMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   IMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   IMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   IMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   IMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   FMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   FMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   FMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   FMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
int16_t   PMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   PMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
int16_t   PMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   PMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));

/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_VComp_Initialize(void)
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
  
   //explicit clear to 0
  VMC_2p2zControlHistory[0] = 0;
  VMC_2p2zControlHistory[1] = 0;
  VMC_2p2zErrorHistory[0] = 0;
  VMC_2p2zErrorHistory[1] = 0;
  VMC_2p2zErrorHistory[2] = 0;
   
  SMPS_Controller2P2ZInitialize(&VMC_2p2z);  
  
}


/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_IComp_Initialize(void)
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
   IMC_2p2zControlHistory[0] =0;
   IMC_2p2zControlHistory[1] =0;
   IMC_2p2zErrorHistory[0] =0;
   IMC_2p2zErrorHistory[1] =0;
   IMC_2p2zErrorHistory[2] =0;
   
   SMPS_Controller2P2ZInitialize(&IMC_2p2z);
}


/*******************************************************************************
 * @ingroup 
 * @brief  
 * @return 
 * 
 * @details 
 * 
 *********************************************************************************/
void Dev_PwrCtrl_PComp_Initialize(void)  //only battery has power sense option. should be bidirectional
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
   
   SMPS_Controller2P2ZInitialize(&PMC_2p2z);  
}

void Dev_PwrCtrl_FComp_Initialize(void)//not used
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
   FMC_2p2zControlHistory[0] = 1860;
   FMC_2p2zControlHistory[1] = 1860;
   FMC_2p2zErrorHistory[0] =0;
   FMC_2p2zErrorHistory[1] =0;
   FMC_2p2zErrorHistory[2] =0;
    
   SMPS_Controller2P2ZInitialize(&FMC_2p2z);  
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

