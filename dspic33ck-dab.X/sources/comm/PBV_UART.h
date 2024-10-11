
/* 
 * @file    PBV_UART.h
 * @author  M70027
 * @ingroup PBV_UART
 * @brief   Power Board Visualizer UART interface 
 */

// This is a guard condition so that contents of this file are not included
// more than once. 
#ifndef APP_PBV_UART_INTERFACE_H
#define	APP_PBV_UART_INTERFACE_H

#include "PBV_interface.h"

void PBV_UART_Init(PBV_Datatype_TX_t * boardToPBV, PBV_Datatype_TX_t * boardToPBVAscii, PBV_Datatype_RX_t *pbvToBoard);
uint8_t PBV_UART_Receive_from_GUI();
uint8_t PBV_UART_Transmit_to_GUI();
uint8_t PBV_UART_Transmit_Ascii_to_GUI();
void PBV_UART_Reinit(PBV_Datatype_TX_t * ptr);
void PBV_UART_Link_Data_TX(PBV_Datatype_TX_t * ptr);
void PBV_UART_Link_Data_RX(PBV_Datatype_RX_t * ptr);

#endif