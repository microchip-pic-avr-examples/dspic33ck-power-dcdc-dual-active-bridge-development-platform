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
///< Includes
#include "config/comms_config.h"
#include "PBV_interface.h"
#include "PBV_config.h"

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE
 * @brief   extern const struct of function pointers
 * @details 
 *      this structure has the function pointers that links to the interfaces from CAN 
 * or UART. depending upon the selection in config.h
 **********************************************************************************/
extern const APP_PBV_INTF_API_t appPbvFuncs;

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE
 * @brief   extern const struct of function pointers
 * @details 
 *      pointer to the PBV_Datatype_TX_t object that will be passed from the application. 
 * 
 **********************************************************************************/
static PBV_Datatype_TX_t * appSystemToPbvPtr;


/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE
 * @brief   
 * @details 
 *      pointer to the PBV_Datatype_RX_t object that will be passed from the application. 
 * 
 **********************************************************************************/
static PBV_Datatype_RX_t * appPbvToSystemPtr;

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE
 * @var     App_System_To_PBV_ASCII_Ptr
 * @brief   
 * @details 
 *      pointer to the PBV_Datatype_TX_t object that will be passed from the application. 
 **********************************************************************************/
static PBV_Datatype_TX_t * appSystemToPbvAsciiPtr;

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE  
 * @param   sixteenPtr - ptr to sixteen bit data
 * @param   eightPtr - ptr to eigtht bit data
 * @param   length - length of sixteen bit
 * @return  void
 * @details 
 *  This function changes 16 bit data to 8 bit data
 **********************************************************************************/
void PBV_Change_from_Sixteen_to_Eight(uint16_t * sixteenPtr, uint8_t * eightPtr, uint16_t length)
{
    for (uint16_t i = 0, j = 0; i < (length * 2); i = i + 2, j++)
    {
        eightPtr[i] = (uint8_t) (sixteenPtr[j] >> 8);
        eightPtr[i + 1] = (uint8_t) (sixteenPtr[j] & 0x00FF);
    }
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE
 * @param   sixteenPtr - ptr to sixteen bit data
 * @param   eightPtr - ptr to eigtht bit data
 * @param   length - length of eight bit 
 * @details 
 *  This function changes 8 bit data to sixteen bit data
 **********************************************************************************/
void PBV_Change_from_Eight_to_Sixteen(uint8_t * eightPtr, uint16_t * sixteenPtr, uint16_t length)
{
    uint8_t length_odd = (uint8_t) (length & 0x01);
    uint16_t length_t;
    
    uint16_t i, j;
    
    if (length_odd)
    {
        length_t = (length & 0xFFFE);
    }
    else
    {
        length_t = length;
    }
    
    for (i = 0, j = 0; i < length_t; i = i + 2, j++)
    {
        sixteenPtr[j] = ((uint16_t) (eightPtr[i] << 8)) + eightPtr[i + 1];
    }    
    if (length_odd)
    {
        sixteenPtr[++j] = ((uint16_t) (eightPtr[length-1] << 8));
    }

}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE
 * @param   PBV_Datatype_TX_t *  - ptr to numerical data
 * @param   PBV_Datatype_TX_t *  - ptr to ascii data 
 * @param   PBV_Datatype_RX_t *  - ptr to data from pbv
 * @brief   
 * @return  void
 * @details 
 *  initializes the PBV init, by linking the application object pointers from the application to 
 * CAN or UART data types
 **********************************************************************************/
void App_PBV_Init(PBV_Datatype_TX_t * Board_To_PBV, PBV_Datatype_TX_t * Board_To_PBVAscii, PBV_Datatype_RX_t *PBV_To_Board)
{
    appSystemToPbvPtr = Board_To_PBV;
    appSystemToPbvAsciiPtr = Board_To_PBVAscii;
    appPbvToSystemPtr = PBV_To_Board;

    appPbvFuncs.init(appSystemToPbvPtr, appSystemToPbvAsciiPtr, PBV_To_Board);
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE 
 * @param   PBV_Datatype_TX_t * - ptr to numerical data
 * @brief   
 * @return  void
 * @details 
 *      reinitializes the objects if any parameters( ID, length, etc) are to be changed 
 * to the CAN or UART objects. 
 **********************************************************************************/
void App_PBV_Re_Init(PBV_Datatype_TX_t * ptr)
{
    // experiment with reassigning the protocol IDs in runtime to CAN objects.
    // or changing DLCs...
    appPbvFuncs.reinit(ptr);
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE 
 * @param   PBV_Datatype_RX_t * - ptr to numerical data
 * @brief   
 * @return  void
 * @details 
 *      this function changes the state of the application object. this acts as a trigger 
 * for the periodic task to receive data
 **********************************************************************************/
void App_Receive_From_PBV(PBV_Datatype_RX_t * ptr)
{
    if (ptr->PBV_Message_State != PBV_STATE_RECEIVING)
    {
        ptr->PBV_Message_State = PBV_MESSAGE_READY_TO_RECEIVE;
    }
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE 
 * @param   PBV_Datatype_TX_t * - ptr
 * @brief   
 * @return  void
 * @details 
 *      This function links the data from application object to the CAN or UART object and     
 * This function changes the state of the application object. This acts as a trigger 
 * for the periodic task to send data
 **********************************************************************************/
void App_Send_To_PBV(PBV_Datatype_TX_t * ptr)
{
    if (ptr->PBV_Message_State == PBV_MESSAGE_INIT
            ||
        ptr->PBV_Message_State == PBV_MESSAGE_TRANSMITTED   
            ||
        ptr->PBV_Message_State == PBV_MESSAGE_TX_ERROR      // TODO: for CAN. If the tx fails in arbitration. Introduce a RETRY mechanism.
            )
    {
        appPbvFuncs.linkDataTX(ptr);
        ptr->PBV_Message_State = PBV_MESSAGE_TRANSMIT_START;
    }
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE 
 * @param   PBV_Datatype_RX_t * - ptr
 * @brief   
 * @return  void
 * @details 
 *         This function links the data received by CAN or UART objects to the application
 * object
 **********************************************************************************/
int App_Read_Received_From_PBV(PBV_Datatype_RX_t * ptr)                             
{
    return appPbvFuncs.linkDataRX(ptr);
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE 
 * @param   none
 * @brief   
 * @return  void
 * @details 
 *      main application task. this looks at the states of the TX and RX pointers, and 
 * if the states are changed for RX/TX then executes TX and RX.
 * **********************************************************************************/
static void App_PBV_Task(void)
{
    if (appPbvToSystemPtr->PBV_Message_State == PBV_MESSAGE_READY_TO_RECEIVE 
            ||
        appPbvToSystemPtr->PBV_Message_State == PBV_STATE_RECEIVING
            )
    {
            appPbvToSystemPtr->PBV_Message_State = appPbvFuncs.rxGui();
    }

    if (appSystemToPbvPtr->PBV_Message_State == PBV_MESSAGE_TRANSMIT_START
            ||
        appSystemToPbvPtr->PBV_Message_State == PBV_MESSAGE_TRANSMITTING
            )
    {
        appSystemToPbvPtr->PBV_Message_State = appPbvFuncs.txGui();
    }

    if (appSystemToPbvAsciiPtr->PBV_Message_State == PBV_MESSAGE_TRANSMIT_START
            ||
        appSystemToPbvAsciiPtr->PBV_Message_State == PBV_MESSAGE_TRANSMITTING    
            )
    {
        appSystemToPbvAsciiPtr->PBV_Message_State = appPbvFuncs.txGuiAscii();
    }
}

/*********************************************************************************
 * @ingroup APP_PBV_INTERFACE 
 * @param   none
 * @brief   task to be executed every 10ms
 * @return  void
 * @details 
 *  task that is to be executed every 100 ms (CAN msg) can work on slower task as 
 * mcc implements a queuing buffer
 **********************************************************************************/
void App_PBV_Task_10ms()
{
#if PBV_CANFD
    App_PBV_Task();
#endif 
}

/*********************************************************************************
 * @ingroup  APP_PBV_INTERFACE
 * @param   
 * @brief   Task to be executed every 100 us
 * @return  void
 * @details 
 *  task that is to be executed every 100 us (UART) for executing UART State machine.
 * calling task has to ensure that the system is not overloaded 
 **********************************************************************************/
void App_PBV_Task_100us()
{
#if PBV_UART
    App_PBV_Task();
#endif 
}
