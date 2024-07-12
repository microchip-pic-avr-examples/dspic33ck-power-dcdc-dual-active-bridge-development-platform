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

// includes
#include "app_PBV_CAN.h"
// MCC header files
#include "can/can1.h"

///< CAN Object data type
typedef struct CAN_MSG_OBJ CAN_MSG_OBJ_t;///< CAN Object data type

/***********************************************************************************
 * @ingroup PBV_CAN
 * @struct  PBV_CAN_Object_TX
 * @brief   CAN object for Numeric TX. Initialized by PBV_CAN_Init()
 **********************************************************************************/
CAN_MSG_OBJ_t PBV_CAN_Object_TX; //

/***********************************************************************************
 * @ingroup PBV_CAN
 * @struct  PBV_CAN_Object_RX
 * @brief   CAN object for numeric RX. Does not have to be initialized.
 **********************************************************************************/
CAN_MSG_OBJ_t PBV_CAN_Object_RX; //

/***********************************************************************************
 * @ingroup PBV_CAN
 * @struct  PBV_CAN_Object_ASCII
 * @brief   CAN object for ascii TX. Initialized by PBV_CAN_Init()
 **********************************************************************************/
CAN_MSG_OBJ_t PBV_CAN_Object_ASCII; //

/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Init(PBV_Datatype_TX_t * boardToPBV, PBV_Datatype_TX_t * boardToPBVAscii)
 * @param   PBV_Datatype_TX_t * - ptr to numerical data
 * @param   PBV_Datatype_TX_t * - ptr to ascii data
 * @param   PBV_Datatype_TX_t * - ptr to rx numerical data. here as a place holder, only needed for completeness
 * @brief   initializes CAN objects with PBV objects ( only CAN IDs for now)
 * @return  void
 * @details 
 **********************************************************************************/

void PBV_CAN_Init(PBV_Datatype_TX_t* Board_To_PBV, PBV_Datatype_TX_t* Board_To_PBVAscii, PBV_Datatype_RX_t *PBV_To_Board)
{
    PBV_CAN_Object_TX.msgId = Board_To_PBV->PBV_Protcol_ID;
    PBV_CAN_Object_TX.field.dlc = DLC_64;
    PBV_CAN_Object_TX.field.brs = CAN_BRS_MODE;
    PBV_CAN_Object_TX.field.formatType = CAN_FD_FORMAT;
    PBV_CAN_Object_TX.field.frameType = CAN_FRAME_DATA;
    PBV_CAN_Object_TX.field.idType = CAN_FRAME_STD;

    PBV_CAN_Object_ASCII.msgId = Board_To_PBVAscii->PBV_Protcol_ID;
    PBV_CAN_Object_ASCII.field.dlc = DLC_64;
    PBV_CAN_Object_ASCII.field.brs = CAN_BRS_MODE;
    PBV_CAN_Object_ASCII.field.formatType = CAN_FD_FORMAT;
    PBV_CAN_Object_ASCII.field.frameType = CAN_FRAME_DATA;
    PBV_CAN_Object_ASCII.field.idType = CAN_FRAME_EXT;
}

/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Receive_from_GUI
 * @param
 * @brief   
 * @return  int (0 on successful, 1 on unsuccessful)
 * @details checks the queue and if a message is received then links the data pointer to can object
 **********************************************************************************/

uint8_t PBV_CAN_Receive_from_GUI()
{
    if (CAN1_ReceivedMessageCountGet() > 0)
        {
        if (CAN1_Receive(&PBV_CAN_Object_RX) == true)
            return 0;
        }
    return 1;
}

/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Transmit_Ascii_to_GUI
 * @param   none
 * @brief   
 * @return  0 on successful transmission. 
 * @details transmits the Ascii msg to PBV 
 **********************************************************************************/

uint8_t PBV_CAN_Transmit_Ascii_to_GUI()
{
    if( CAN1_Transmit(CAN1_TXQ, &PBV_CAN_Object_ASCII ) != CAN_TX_MSG_REQUEST_SUCCESS) 
        return PBV_MESSAGE_TX_ERROR; 
    else
        return PBV_MESSAGE_TRANSMITTED;
}

/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Transmit_to_GUI
 * @param   none
 * @brief   transmits the numerical msg to PBV
 * @return  int
 * @details this function sends the message on CAN1 Transmit Queue. TODO: make this portable to other transmit queues. 
 **********************************************************************************/

uint8_t PBV_CAN_Transmit_to_GUI()
{
    if( CAN1_Transmit(CAN1_TXQ, &PBV_CAN_Object_TX ) != CAN_TX_MSG_REQUEST_SUCCESS) 
        return PBV_MESSAGE_TX_ERROR; 
    else
        return PBV_MESSAGE_TRANSMITTED;
}
/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Reniit
 * @param   PBV_Datatype_RX_t *
 * @brief   reinitializes the can object with new can id. 
 * @return  
 * @details This function is needed in CAN TX to reinit the CAN TX object with new parameters.
 **********************************************************************************/

void PBV_CAN_Reinit(PBV_Datatype_TX_t * ptr){
    if (ptr->PBV_Signal_Ascii == PBV_SIGNAL_MODE)
    {
        PBV_CAN_Object_TX.msgId = ptr->PBV_Protcol_ID;
        switch (ptr->Length)
        {
            case 0:
                PBV_CAN_Object_TX.field.dlc = DLC_0;    /**< Data length count 0 */
                break;
            case 1:
                PBV_CAN_Object_TX.field.dlc = DLC_1;    /**< Data length count 1 */  
                break;
            case 2:
                PBV_CAN_Object_TX.field.dlc = DLC_2;    /**< Data length count 2 */ 
                break;
            case 3:
                PBV_CAN_Object_TX.field.dlc = DLC_3;    /**< Data length count 3 */ 
                break;
            case 4:
                PBV_CAN_Object_TX.field.dlc = DLC_4;    /**< Data length count 4 */ 
                break;
            case 5:
                PBV_CAN_Object_TX.field.dlc = DLC_5;    /**< Data length count 5 */  
                break;     
            case 6:
                PBV_CAN_Object_TX.field.dlc = DLC_6;    /**< Data length count 6 */  
                break;   
            case 7:
                PBV_CAN_Object_TX.field.dlc = DLC_7;    /**< Data length count 7 */  
                break;
            case 8:
                PBV_CAN_Object_TX.field.dlc = DLC_8;    /**< Data length count 8 */  
                break; 
            case 9 ... 12:
                PBV_CAN_Object_TX.field.dlc = DLC_12;    /**< Data length count 12 */  
                break;    
           case 13 ... 16:
                PBV_CAN_Object_TX.field.dlc = DLC_16;    /**< Data length count 16 */  
                break;   
           case 17 ... 20:
                PBV_CAN_Object_TX.field.dlc = DLC_20;    /**< Data length count 20 */  
                break;   
           case 21 ... 24:
                PBV_CAN_Object_TX.field.dlc = DLC_24;    /**< Data length count 24 */  
                break;       
           case 25 ... 32:
                PBV_CAN_Object_TX.field.dlc = DLC_32;    /**< Data length count 32 */  
                break; 
           case 33 ... 48:
                PBV_CAN_Object_TX.field.dlc = DLC_48;    /**< Data length count 48 */  
                break;  
           case 49 ... 64:
                PBV_CAN_Object_TX.field.dlc = DLC_64;    /**< Data length count 64 */  
                break;                
            default:
                PBV_CAN_Object_TX.field.dlc = DLC_64;    /**< Data length count 64 */  
                break;
        }
    }
    else 
    {    
        PBV_CAN_Object_ASCII.msgId = ptr->PBV_Protcol_ID;
        switch (ptr->Length)
        {
            case 0:
                PBV_CAN_Object_ASCII.field.dlc = DLC_0;    /**< Data length count 0 */
                break;
            case 1:
                PBV_CAN_Object_ASCII.field.dlc = DLC_1;    /**< Data length count 1 */  
                break;
            case 2:
                PBV_CAN_Object_ASCII.field.dlc = DLC_2;    /**< Data length count 2 */ 
                break;
            case 3:
                PBV_CAN_Object_ASCII.field.dlc = DLC_3;    /**< Data length count 3 */ 
                break;
            case 4:
                PBV_CAN_Object_ASCII.field.dlc = DLC_4;    /**< Data length count 4 */ 
                break;
            case 5:
                PBV_CAN_Object_ASCII.field.dlc = DLC_5;    /**< Data length count 5 */  
                break;     
            case 6:
                PBV_CAN_Object_ASCII.field.dlc = DLC_6;    /**< Data length count 6 */  
                break;   
            case 7:
                PBV_CAN_Object_ASCII.field.dlc = DLC_7;    /**< Data length count 7 */  
                break;
            case 8:
                PBV_CAN_Object_ASCII.field.dlc = DLC_8;    /**< Data length count 8 */  
                break; 
            case 9 ... 12:
                PBV_CAN_Object_ASCII.field.dlc = DLC_12;    /**< Data length count 12 */  
                break;    
           case 13 ... 16:
                PBV_CAN_Object_ASCII.field.dlc = DLC_16;    /**< Data length count 16 */  
                break;   
           case 17 ... 20:
                PBV_CAN_Object_ASCII.field.dlc = DLC_20;    /**< Data length count 20 */  
                break;   
           case 21 ... 24:
                PBV_CAN_Object_ASCII.field.dlc = DLC_24;    /**< Data length count 24 */  
                break;       
           case 25 ... 32:
                PBV_CAN_Object_ASCII.field.dlc = DLC_32;    /**< Data length count 32 */  
                break; 
           case 33 ... 48:
                PBV_CAN_Object_ASCII.field.dlc = DLC_48;    /**< Data length count 48 */  
                break;  
           case 49 ... 64:
                PBV_CAN_Object_ASCII.field.dlc = DLC_64;    /**< Data length count 64 */  
                break;                
            default:
                PBV_CAN_Object_ASCII.field.dlc = DLC_64;    /**< Data length count 64 */  
                break;
        }
    }
}

/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Link_Data_TX
 * @param   PBV_Datatype_RX_t *
 * @brief   Links the data from TX PBV datatype to CAN TX object. 
 * @return  
 * @details This function links the data (uint_8 *) from the passing struct to the CAN object data (uint_8 *)
 **********************************************************************************/

void PBV_CAN_Link_Data_TX(PBV_Datatype_TX_t * ptr){
    if (ptr->PBV_Signal_Ascii == PBV_SIGNAL_MODE)
        PBV_CAN_Object_TX.data = ptr->Data_Buffer;
    else 
        PBV_CAN_Object_ASCII.data = ptr->Data_Buffer;
}

/*********************************************************************************
 * @ingroup PBV_CAN 
 * @fn      PBV_CAN_Link_Data_RX
 * @param   PBV_Datatype_RX_t *
 * @brief   links the received data to the application data structure
 * @return  void
 * @details This links the received data (uint8_t *) to the application data structure data (uint8_t *)
 **********************************************************************************/

void PBV_CAN_Link_Data_RX(PBV_Datatype_RX_t * ptr){
    if (ptr->PBV_Message_State == 0){
        ptr->Data_Buffer = PBV_CAN_Object_RX.data;
        ptr->PBV_Protcol_ID = PBV_CAN_Object_RX.msgId ;
        switch (PBV_CAN_Object_RX.field.dlc)
        {
            case DLC_0:
                ptr->Length = 0;    /**< Data length count 0 */
                break;
            case DLC_1:
                ptr->Length = 1;    /**< Data length count 1 */  
                break;
            case DLC_2:
                ptr->Length = 2;    /**< Data length count 2 */ 
                break;
            case DLC_3:
                ptr->Length = 3;    /**< Data length count 3 */ 
                break;
            case DLC_4:
                ptr->Length = 4;    /**< Data length count 4 */ 
                break;
            case DLC_5:
                ptr->Length = 5;    /**< Data length count 5 */  
                break;     
            case DLC_6:
                ptr->Length = 6;    /**< Data length count 6 */  
                break;   
            case DLC_7:
                ptr->Length = 7;    /**< Data length count 7 */  
                break;
            case DLC_8:
                ptr->Length = 8;    /**< Data length count 8 */  
                break;     
            case DLC_12:
                ptr->Length = 12;    /**< Data length count 12 */  
                break;    
           case DLC_16:
                ptr->Length = 16;    /**< Data length count 16 */  
                break;   
           case DLC_20:
                ptr->Length = 20;    /**< Data length count 20 */  
                break;   
           case DLC_24:
                ptr->Length = 24;    /**< Data length count 24 */  
                break;       
           case DLC_32:
                ptr->Length = 32;    /**< Data length count 32 */  
                break; 
           case DLC_48:
                ptr->Length = 48;    /**< Data length count 48 */  
                break;  
           case DLC_64:
                ptr->Length = 64;    /**< Data length count 64 */  
                break;                
            default:
                ptr->Length = 64;    /**< Data length count 64 */  
                break;
        }
    }
}