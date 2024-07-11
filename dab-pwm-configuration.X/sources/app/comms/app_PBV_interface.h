/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * @file    app_PBV_interface.h   
 * @author  M70027
 * @brief   Power Board Visualizer application interface 
 */

#ifndef APP_PBV_INTERFACE_H
#define	APP_PBV_INTERFACE_H

///< includes
#include <xc.h> // include processor files - each processor file is guarded.  
#include "config/comms_config.h"

/***********************************************************************************
 * @ingroup APP_PBV
 * @brief   Power Board Visualizer TX type
 * @details This type defines if the TX Object is transmitting numbers or ASCII
 **********************************************************************************/
enum PBV_SIGNAL_ASCII_MODE 
{   
    PBV_SIGNAL_MODE    = 0,    ///< Object will transmit/receive signals 
    PBV_ASCII_MODE     = 1     ///< Object will transmit Ascii data
} ;

/***********************************************************************************
 * @ingroup APP_PBV
 * @brief   Power Board Visualizer TX Object transmission states
 * @details 
 *      These states are used to keep track of the transmission of message
 **********************************************************************************/
enum PBV_MESSAGE_TX_STATE
{    
    PBV_MESSAGE_TRANSMITTED     = 0,    ///< Message transmitted. successful transmit
    PBV_MESSAGE_INIT            = 1,    ///< Init State. Only Send Messages in this State    
    PBV_MESSAGE_TRANSMIT_START  = 2,    ///< Start Transmitting message
    PBV_MESSAGE_TRANSMITTING    = 3,    ///< Transmitting Message
    PBV_MESSAGE_TX_ERROR        = 4    ///< if some error happens. unsuccessful transmit
};
typedef enum PBV_MESSAGE_TX_STATE PBV_MESSAGE_TX_STATE_t;

/***********************************************************************************
 * @ingroup APP_PBV
 * @brief   Power Board Visualizer RX Object Receiving states
 * @details 
 *      These states are used to keep track of the receiving of messages
 **********************************************************************************/
enum PBV_MESSAGE_RX_STATE
{
    PBV_MESSAGE_RECEIVED = 0,           ///< Message Received
    PBV_MESSAGE_READY_TO_RECEIVE = 1,   ///< Message Reception triggered
    PBV_STATE_RECEIVING = 2,            ///< Message beinf received
    PBV_MESSAGE_RX_ERROR = 3           ///< if some error happens (CRC ?)
};
typedef enum PBV_MESSAGE_RX_STATE PBV_MESSAGE_RX_STATE_t;

/*********************************************************************************
 * @ingroup APP_PBV
 * @var     func_p
 * @brief   function pointer typedef for callbacks
 * @param   length - length of passed eight bit buffer
 * @param   ptr    - pointer to eight bit data
 * @details function pointer typedef for callback after receiving data. 
 *********************************************************************************/
typedef void (*func_p)(uint16_t, uint8_t *);    

/*********************************************************************************
 * @ingroup APP_PBV
 * @struct PBV_Datatype_TX_t
 * @brief   
 * @details pbv App TX data type definition structure   
 *********************************************************************************/

typedef struct { 
    uint16_t Length;                                ///< Length of transmitted message in Bytes
    uint32_t PBV_Protcol_ID;                        ///< Protocol ID
    enum PBV_SIGNAL_ASCII_MODE  PBV_Signal_Ascii;   ///< Object will transmit Signals or ASCII
    PBV_MESSAGE_TX_STATE_t  PBV_Message_State;      ///< State of the TX object
    uint8_t * Data_Buffer;                          ///< Pointer to the data to be transmitted
}PBV_Datatype_TX_t;

/*********************************************************************************
 * @ingroup 
 * @struct     PBV_Datatype_RX_t
 * @brief   
 * @details pbv App RX data type definition structure 
 *********************************************************************************/
typedef struct { 
    uint16_t Length;                                ///< Length of received message in Bytes
    uint32_t PBV_Protcol_ID;                        ///< Protocol ID of the received message
    func_p Callback_Function ;                      ///< Callback function 
    PBV_MESSAGE_RX_STATE_t  PBV_Message_State;      ///< State of the received message
    uint8_t * Data_Buffer;                          ///< Received Data pointer
}PBV_Datatype_RX_t;

///< functions
void App_PBV_Init(PBV_Datatype_TX_t * system_to_PBV, PBV_Datatype_TX_t * system_To_PBV_Ascii, PBV_Datatype_RX_t *App_PBV_To_System_Ptr);
void App_Send_To_PBV (PBV_Datatype_TX_t * ptr);
void App_Receive_From_PBV(PBV_Datatype_RX_t * ptr);
int App_Read_Received_From_PBV(PBV_Datatype_RX_t * ptr);

void PBV_Change_from_Sixteen_to_Eight(uint16_t * sixteen_ptr, uint8_t * eight_ptr, uint16_t length);
void PBV_Change_from_Eight_to_Sixteen(uint8_t * eight_ptr, uint16_t * sixteen_ptr, uint16_t length);

void App_PBV_Task_10ms(void);
void App_PBV_Task_100us(void);

void App_PBV_Re_Init(PBV_Datatype_TX_t * ptr);

#endif	/*  APP_PBV_INTERFACE_H */
