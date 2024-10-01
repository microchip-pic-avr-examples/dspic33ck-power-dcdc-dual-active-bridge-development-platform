
/* 
 * @file    PBV_CAN.h   
 * @author  M70027
 * @ingroup PBV_CAN
 * @brief   Power Board Visualizer CAN interface 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APP_PBV_CAN_INTERFACE_H
#define	APP_PBV_CAN_INTERFACE_H

#include "PBV_interface.h"

// Public Functions
void PBV_CAN_Init(PBV_Datatype_TX_t * boardToPBV, PBV_Datatype_TX_t * boardToPBVAscii, PBV_Datatype_RX_t *pbvToBoard);
uint8_t PBV_CAN_Receive_from_GUI();
uint8_t PBV_CAN_Transmit_to_GUI();
uint8_t PBV_CAN_Transmit_Ascii_to_GUI();
void PBV_CAN_Reinit(PBV_Datatype_TX_t * ptr);
void PBV_CAN_Link_Data_TX(PBV_Datatype_TX_t * ptr);
void PBV_CAN_Link_Data_RX(PBV_Datatype_RX_t * ptr);

#endif  /* APP_PBV_CAN_INTERFACE_H */

