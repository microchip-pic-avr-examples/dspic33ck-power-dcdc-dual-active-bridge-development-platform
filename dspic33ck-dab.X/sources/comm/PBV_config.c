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

/* 
 * @file    PBV_config.c   
 * @author  M70027
 * @file    app-pbv-config
 * @brief   Power Board Visualizer application configuration 
 */

#include "config/comms_config.h"

#include "PBV_config.h"

#if defined (PBV_UART) && (PBV_UART == 1)
#include "app_PBV_UART.h"
#endif

#if defined (PBV_CANFD) && (PBV_CANFD == 1)
#include "PBV_CAN.h"
#endif

/***********************************************************************************
 * @ingroup app-pbv-config
 * @brief   structure of API function pointers. 
 **********************************************************************************/
const APP_PBV_INTF_API_t appPbvFuncs = {

#if defined (PBV_UART) && (PBV_UART == 1)
    PBV_UART_Init,
    PBV_UART_Receive_from_GUI,
    PBV_UART_Transmit_to_GUI,
    PBV_UART_Transmit_Ascii_to_GUI,
    PBV_UART_Reinit,
    PBV_UART_Link_Data_TX,
    PBV_UART_Link_Data_RX
#endif

#if defined (PBV_CANFD) && (PBV_CANFD == 1)
    PBV_CAN_Init,
    PBV_CAN_Receive_from_GUI,
    PBV_CAN_Transmit_to_GUI,
    PBV_CAN_Transmit_Ascii_to_GUI,
    PBV_CAN_Reinit,
    PBV_CAN_Link_Data_TX,
    PBV_CAN_Link_Data_RX
#endif
};
