/*
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


/**
 * @file pwrctrl_dcdt.h     
 * @ingroup pwrctrl-dcdt   
 * @brief Contains public functions and data types relevant for DCDT file interface.
 */

#ifndef _DRV_CONTROLLER_DAB_DCDC_H
#define _DRV_CONTROLLER_DAB_DCDC_H
#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include "smps_control.h"
//forward mode: primary to secondary
#include "dab_psio/dcdt_generated_code/dab_psio_dcdt.h"
#include "dab_pspo/dcdt_generated_code/dab_pspo_dcdt.h"
#include "dab_psvo/dcdt_generated_code/dab_psvo_dcdt.h"
//reverse mode: secondary to primary
#include "dab_spio/dcdt_generated_code/dab_spio_dcdt.h"
#include "dab_sppo/dcdt_generated_code/dab_sppo_dcdt.h"
#include "dab_spvo/dcdt_generated_code/dab_spvo_dcdt.h"

extern SMPS_2P2Z_T VMC_2p2z;
extern SMPS_2P2Z_T IMC_2p2z;
extern SMPS_2P2Z_T PMC_2p2z; 

extern SMPS_2P2Z_T VMC_2p2z_Rev;
extern SMPS_2P2Z_T IMC_2p2z_Rev;
extern SMPS_2P2Z_T PMC_2p2z_Rev; 

void PwrCtrl_IComp_Initialize(void);
void PwrCtrl_VComp_Initialize(void);
void PwrCtrl_PComp_Initialize(void);
void PwrCtrl_ResetControlLoopHistories(void);
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif	/* XC_HEADER_TEMPLATE_H */