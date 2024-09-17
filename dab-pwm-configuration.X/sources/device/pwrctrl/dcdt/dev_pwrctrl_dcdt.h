/**
  @Company
    Microchip Technology Inc.

  @File Name
    dev_TPBLPFC_typedef.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides Controller settings for average current mode and voltage mode controller.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
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
 * @file dev_pwrctrl_dcdt.h     
 * @ingroup dev-pwrctrl-dcdt   
 * @brief Contains public functions and data types relevant for DCDT file interface.    
 */

#ifndef _DRV_CONTROLLER_DAB_DCDC_H
#define _DRV_CONTROLLER_DAB_DCDC_H
#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include "smps_control.h"

#include "dab_psio/dcdt_generated_code/dab_psio_dcdt.h"
#include "dab_pspo/dcdt_generated_code/dab_pspo_dcdt.h"
#include "dab_spvo/dcdt_generated_code/dab_spvo_dcdt.h"

extern SMPS_2P2Z_T VMC_2p2z;
extern SMPS_2P2Z_T IMC_2p2z;
extern SMPS_2P2Z_T PMC_2p2z; 

void Dev_PwrCtrl_IComp_Initialize(void);
void Dev_PwrCtrl_VComp_Initialize(void);
void Dev_PwrCtrl_PComp_Initialize(void);
void Dev_PwrCtrl_ResetControlLoopHistories(void);
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif	/* XC_HEADER_TEMPLATE_H */