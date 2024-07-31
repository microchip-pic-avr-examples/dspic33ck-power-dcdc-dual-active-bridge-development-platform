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


#ifndef _DRV_CONTROLLER_DAB_DCDC_H
#define _DRV_CONTROLLER_DAB_DCDC_H
#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include "smps_control.h"
//#include "../../../dcdt/icomp1hv/dcdt_generated_code/icomp1hv_dcdt.h"
#include "../../../dcdt/dab_psio/dcdt_generated_code/dab_psio_dcdt.h"
//#include "../../../dcdt/dab_psvo/dcdt_generated_code/dab_psvo_dcdt.h"
#include "../../../dcdt/dab_pspo/dcdt_generated_code/dab_pspo_dcdt.h"

//#include "../../../dcdt/dab_spio/dcdt_generated_code/dab_spio_dcdt.h"
#include "../../../dcdt/dab_spvo/dcdt_generated_code/dab_spvo_dcdt.h"
//#include "../../../dcdt/dab_sppo/dcdt_generated_code/dab_sppo_dcdt.h"

#include "../../../dcdt/dab_psio/dcdt_generated_code/dab_fm_dcdt.h"

#include "../power_controller/../application/canfd/app_can.h"

#define PRIMTOSEC_TARGET (882)

extern SMPS_2P2Z_T VMC_2p2z;//voltage loop compensator forward
extern SMPS_2P2Z_T IMC_2p2z;//current loop comp forward

extern SMPS_2P2Z_T VMC_SP_2p2z;//reverse, secondary towards primary
extern SMPS_2P2Z_T IMC_SP_2p2z;//reverse, secondary towards primary

extern SMPS_2P2Z_T FMC_2p2z;//future development



extern int16_t   VMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
extern int16_t   VMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
extern int16_t   VMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
extern int16_t   VMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
extern uint16_t  VMC_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
extern int16_t   IMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
extern int16_t   IMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
extern int16_t   IMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
extern int16_t   IMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
extern uint16_t  IMC_2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T IMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================
extern int16_t   FMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
extern int16_t   FMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
extern int16_t   FMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
extern int16_t   FMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
extern uint16_t  FMC_2p2zReferenceSet;

//------------------------------------------------------------------------------
// @brief power controller states
//------------------------------------------------------------------------------
typedef enum 
{
    DAB_DISABLED        = 0,
    DAB_CHARGER         = 1,
    DAB_DISCHARGER      = 2 
    //to be continued if needed        
}DAB_DIRECTION_e ;


typedef enum 
{
    PFC_ACTIVE_VOK        = 0,
    PFC_INACTIVE_VOK      = 1,
    BATT_UNDERDISCH   = 2, 
    BATT_OVERCHARGE    = 3, 
    DC400V_SYSTEM         = 4,
    DC800V_SYSTEM         = 5,
    DC_EXPERIMENT_SYSTEM  = 6,         
    //to be continued if needed        
}VDC_STATUS_FLAGS_e ;

typedef enum 
{
    DCS_INIT                 = 0,
    DCS_WAIT_FAULT_ACTIVE    = 1,
    DCS_STANDBY              = 2, 
    DCS_SOFT_START           = 3, 
    DCS_UP_AND_RUNNING       = 4
        
    //to be continued if needed        
}DAB_CONTROLLER_STATE_e ;





struct ANALOG_VALUES_s
{
  volatile int16_t  Raw;
  volatile int16_t  Rectified; //For DCDC context input and output current direction can be differentiated
  volatile uint16_t Offset;
  
  volatile uint16_t FilteredRaw; 
  volatile uint16_t FilteredRectified; 
  volatile uint16_t FilterBuff;
  volatile uint16_t FilterCounter;
  //volatile uint16_t PreviousValue;
};
typedef struct ANALOG_VALUES_s ANALOG_VALUES_t;



//Future development:s
//DAB DCDC system and converter related  structure. Application specific details collection. Not general purpose
struct DAB_CONTROLLER_s
{
    ANALOG_VALUES_t NTCadcVal;// = 4000;
    ANALOG_VALUES_t FB_5V_val;// = 0; 
    ANALOG_VALUES_t FB_VDD_INT_val;// = 0;
    ANALOG_VALUES_t FB_VDD_Batt_val;

    ANALOG_VALUES_t AVG_CurrentSec;// = 0;//1845;  //Aceinna
    ANALOG_VALUES_t AVG_PowerSec;
    
    DAB_DIRECTION_e PowerDirection;
    DAB_DIRECTION_e PowerDirectionBkp;//detect transitions
    uint16_t OBC_status;//none, charge finished, discharge finished
    uint16_t RefRampInPorgressFlag;
    uint16_t FaultBits;
    uint16_t FaultPinsState;
   
   //can be used bidirectional
    uint16_t VoltageGain;// and gain factor ,  used to steer control logic, buck boos, unity cases may need separate optimization steps 
   
    uint16_t PrimaryVoltage;//plug here allready pre-processed values. Use ANALOG_VALUES_t for pre-adjusting steps of data, based on usecase
    uint16_t PrimaryCurrent;//not implemented in hw
    uint32_t PrimaryPower;//plug here a Power Analyzer module. Can run in background loops
    uint16_t PrimaryCurrentAvg;//
    uint16_t PrimaryVoltageAvg;
    
    uint16_t SecondaryVoltage;//plug here allready pre-processed values. Use ANALOG_VALUES_t for pre-adjusting steps of data, based on usecase
    uint16_t SecondaryCurrent;
    uint16_t SecondaryPower;
    uint16_t SecondaryCurrentAvg;
    uint16_t SecondaryVoltageAvg;
   
//   //review and cleanup
    uint16_t SecondaryPowerDemo;
    uint16_t SecondaryPowerBattPercent;
    uint16_t SecondaryAmpereAvg;
   
   
   //trafo phases related
   uint16_t Prim_PWM_Ph_Value;
   uint16_t Prim_PWM_Ph_Degrees;//normalized to 360
   uint16_t Prim_PWM_Ph_DegreesX10;//normalized to 360x10
   
   uint16_t Sec_PWM_Ph_Value;
   uint16_t Sec_PWM_Ph_Degrees; //normalized to 360
   uint16_t Sec_PWM_Ph_DegreesX10; //normalized to 360 X10
   
   uint16_t PrimToSec_PWM_Ph_Value;
   uint16_t PrimToSec_PWM_Ph_Degrees; //normalized to 360
   uint16_t PrimToSec_PWM_Ph_DegreesX10; //normalized to 360 X10
   
   //PWM freq modulation related
   uint16_t PWM_Period;//common to prim and sec bridges 360 degree point
   uint16_t PWM_DutyCycle;//common to prim and sec bridges 180 degree point @ 50%
   uint16_t PWM_DutyCycleHalf;//common to prim and sec bridges.  90 degree shift
   
   //refference target values. decouple to achieve ramp control from actual values plugged into compensators IMreference  VMreference  VMreferenceTP
   
   uint16_t IoutRef;//battery side  
   uint16_t VoutRef;//battery side or intermediate Voltage.
   uint16_t PoutRef;//battery power, defined by design
//   uint16_t VoutRef_int; //
   
   
   
   //compnesators related contexts
   //I mode compensator linking
   volatile uint16_t IMControllerInputRegister; //hooks to 2p2z compensator pipeline. may need decoupling to insert other processing. FF calculation etc.
   int16_t IMreference;
   int16_t IMreference2p2z;
   volatile uint16_t IMControllerOutputRegister;
//   volatile uint16_t IMControllerOutputRegisterPrev;
   
   //voltage mode compensator linking
   volatile uint16_t VMControllerInputRegister;
   int16_t VMreference;
   int16_t VMreference2p2z;
   volatile uint16_t VMControllerOutputRegister;
   
   //power mode compensator linking
   volatile uint16_t PMControllerInputRegister;
   int16_t PMreference;
   int16_t PMreference2p2z;
   volatile uint16_t PMControllerOutputRegister;
   
   
   //voltage mode compensator linking
   volatile uint16_t VMControllerTPInputRegister;//Voltage mode controller at Totem Pole system side (PFC or Inverter)
   int16_t VMreferenceTP;
   int16_t VMreferenceTP2p2z;
   volatile uint16_t VMTPControllerTPOutputRegister;
   
      //FUTURE reserved:
   //Freq modulator compensator linking
   volatile uint16_t FMControllerInputRegister;
   int16_t FMreference;
   int16_t FMreference2p2z;
   volatile uint16_t FMControllerOutputRegister;
   //FUTURE reserved  end.

   
   int16_t SystemMonitoringFlags;//take out some relevant bits in PowerBoard visualizer or other debug purpose
//   VDC_STATUS_FLAGS_e VdcStatusFlags;
//   DAB_CONTROLLER_STATE_e DABControllerState;
   int16_t VdcStatusFlags;
   int16_t DABControllerState;
//   //experimental:
//   int16_t IAvgCntr;
//   int16_t VAvgCntr;
//   uint32_t BuffI; 
//   uint32_t BuffV; 
//   uint32_t BuffV_D;
   
   int16_t Adaptive_Gain_Factor;
   uint32_t Vin_k;  //power stage gain compensation charger direction
   uint32_t Vin_k_rev; //power stage gain compensation discharger direction
   uint16_t RegulatedValue;
   uint16_t TargetPrimToSecPhase;
   uint16_t LowPowerSlowMode;
   
//   uint8_t UARTRXBuff[5];//some experiments
   
};
typedef struct DAB_CONTROLLER_s DAB_CONTROLLER_t;
extern DAB_CONTROLLER_t DAB;  

//bit fiddling 
#define SET_BITn(var,nbit)  ((var) |=  (1<<(nbit)))
#define CLR_BITn(var,nbit)  ((var) &= ~(1<<(nbit)))
#define TOG_BITn(var,nbit)  ((var) ^=  (1<<(nbit)))
#define TST_BITn(var,nbit)  ((var) &   (1<<(nbit)))

#define SET_BITs(var,msk)  ((var) |=  (msk))
#define CLR_BITs(var,msk)  ((var) &= ~(msk))
#define TOG_BITs(var,msk)  ((var) ^=  (msk))
#define TST_BITs(var,msk)  ((var) &   (msk))





// PowerBoardVisualizer Interface app module
typedef struct
{ 
  uint16_t DAB_OV_Threshold              ;//NotDefinedYet00;//word 0    
  uint16_t DAB_PimToSecPh_TargetThreshold;//NotDefinedYet01;//word 1    
  uint16_t DAB_OC_CT_PS_Threshold        ;//NotDefinedYet02;//word 2  

  uint16_t DAB_REF_OutpRegV ;
  uint16_t DAB_REF_OutpRegI ;
  uint16_t DAB_REF_OutpRegP ;
} CAN_x603_FRAME_s;  //DAB DCDC GUI tests interfacing specifics. WIP. TBD
extern CAN_x603_FRAME_s CAN_x603_FRAME;
extern CAN_x603_FRAME_s *pCAN_x603_FRAME;

void VoltageCtrl_Compensator_Handler(void);
void CurrentCtrl_Compensator_Handler(void);
void FreqCtrl_Compensator_Handler(void);
void PowCtrl_Compensator_Handler(void);
void CurrentCtrl_DAB_Init(void);
void VoltageCtrl_DAB_Init(void);
void FreqCtrl_DAB_Init(void);
void PowCtrl_DAB_Init(void);
void Init_pwr_ctrl (void);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif	/* XC_HEADER_TEMPLATE_H */