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
//includes
#include <stdint.h>
#include <stdbool.h>

#include "app_PBV_UART.h"
// MCC header files

#include "uart/uart1.h"
#include "crc/crc.h"

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     PBV_CRC_POLYNOM
 * @brief   Standard PBV CRC Polynomial. For documentation, as 
 * it is not used in calculation in this file
 **********************************************************************************/
#define PBV_CRC_POLYNOM             0x8005

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     PBV_CRC_POLYNOM_REV
 * @brief   Reversed polynomial. needed for odd byte in the data stream
 **********************************************************************************/
#define PBV_CRC_POLYNOM_REV         0xA001

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     PBV_ReadyToSend
 * @brief   linking functions with UART mcc driver functions
 **********************************************************************************/
#define PBV_ReadyToSend             UART1_IsTxReady

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     PBV_Write
 * @brief   linking functions with UART mcc driver functions
 **********************************************************************************/
#define PBV_Write                   UART1_Write

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     PBV_IsRxReady
 * @brief   linking functions with UART mcc driver functions
 **********************************************************************************/
#define PBV_IsRxReady               UART1_IsRxReady

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     PBV_Read
 * @brief   linking functions with UART mcc driver functions
 **********************************************************************************/
#define PBV_Read                    UART1_Read

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     Start of frame
 * @brief   Standard Start of frame for PBV UART frames 
 **********************************************************************************/
#define PBV_START_OF_FRAME          0x55

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     End of frame
 * @brief   Standard End of frame for PBV UART frames 
 **********************************************************************************/
#define PBV_END_OF_FRAME            0x0d

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     RCV_<STATES>
 * @brief   Internal STATES for RX State machine for PBV UART Frames
 **********************************************************************************/
///< STATES for RX State machine for PBV UART Frames
#define RCV_WAIT_FOR_STARTBYTE		0
#define RCV_READ_ID_HIGHBYTE		1
#define RCV_READ_ID_LOWBYTE			2
#define RCV_READ_LENGTH_HIGHBYTE	3
#define RCV_READ_LENGTH_LOWBYTE		4
#define RCV_READ_DATA				5
#define RCV_READ_CRC_HIGHBYTE		6
#define RCV_READ_CRC_LOWBYTE		7
#define RCV_READ_EOF				8
#define RCV_MESSAGE_RECEIVED        9

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     Receive Data lenght excluding header (5 bytes) and footer (3 bytes, but not needed in memory allocation for UART arrays.)
 * @brief   Maximum UART buffer. The max data that could be received is 64 bytes
 **********************************************************************************/
#define PBV_RCV_DATABUFFER_SIZE     64      // MAX.. 

/*********************************************************************************
 * @ingroup PBV_UART
 * @def     Receive Data lenght excluding header (5 bytes) and footer (3 bytes, but not needed in memory allocation for UART arrays.)
 * @brief   Maximum UART buffer. The max data that could be received is 64 bytes
 **********************************************************************************/
#define PBV_HEADER_SIZE             5       // in bytes 0x55 + protocol ID (2 bytes) + length (2 bytes)?

/***********************************************************************************
 * @ingroup PBV_UART
 * @struct  UART_MSG_RX_OBJ
 * @brief   UART msg receive object. 
 **********************************************************************************/
typedef struct UART_MSG_RX_OBJ
{
    uint8_t UART_Frame_State;               ///< UART Receive Frame states 
    uint8_t Length_in_Bytes;                ///< UART bytes Received   
    uint16_t Protocol_ID;                   ///< Received Frame ID
    uint16_t offset;                        ///< Offset needed to store data in _data_  
    uint16_t CRC;                           ///< received CRC
    uint8_t PBV_Header[PBV_HEADER_SIZE];    ///< recived Header. 5bytes ( SOF + ID + Length)
    uint8_t data[PBV_RCV_DATABUFFER_SIZE];  ///< data buffer. linked by the calling application. STATIC(64) bytes for now.
} UART_MSG_RX_OBJ_t; ///< UART RX Object data type

/***********************************************************************************
 * @ingroup PBV_UART
 * @struct  UART_MSG_TX_OBJ
 * @brief   UART msg Transmit object. 
 **********************************************************************************/
typedef struct UART_MSG_TX_OBJ
{
    uint8_t UART_Frame_State;               ///< UART Transmit Frame states 
    uint8_t Length_in_Bytes;                ///< UART Bytes to be Transmitted
    uint16_t Protocol_ID;                   ///< Frame ID to be appended
    uint16_t offset;                        ///< Offset needed for data transmission
    uint16_t CRC;                           ///< calculated CRC
    uint8_t PBV_Header[PBV_HEADER_SIZE];    ///< sent Header. 5bytes ( SOF + ID + Length)
    uint8_t * data;                         ///< data buffer. linked by the calling application. STATIC(64) bytes for now.
} UART_MSG_TX_OBJ_t; ///< UART TX Object data type

uint32_t PBV_calculated_CRC = 0;            ///< memory location for return CRC value from function

static bool uartActiveTx;                   ///< UART TX lock. High when numeric frame is being sent
static bool uartActiveTxAscii;              ///< UART TX lock. High when ASCII frame is being sent

// private function declaration
uint16_t PBV_Calculate_CRC(uint8_t *message, uint16_t length);

/***********************************************************************************
 * @ingroup PBV_UART
 * @struct  PBV_UART_Object_TX
 * @brief   UART object for numeric tx
 **********************************************************************************/
UART_MSG_TX_OBJ_t PBV_UART_Object_TX;

/***********************************************************************************
 * @ingroup PBV_UART
 * @struct  PBV_UART_Object_RX
 * @brief   UART object for numeric rx
 **********************************************************************************/
UART_MSG_RX_OBJ_t PBV_UART_Object_RX;

/***********************************************************************************
 * @ingroup PBV_UART
 * @struct  PBV_UART_Object_ASCII
 * @brief   UART object for ascii tx
 **********************************************************************************/
UART_MSG_TX_OBJ_t PBV_UART_Object_ASCII; //

/*********************************************************************************
 * @ingroup PBV_UART 
 * @fn      PBV_UART_Init(PBV_Datatype_TX_t * Board_To_PBV, PBV_Datatype_TX_t * Board_To_PBVAscii, PBV_Datatype_RX_t *Pbv_To_Board)
 * @param   PBV_Datatype_TX_t * - ptr to numerical data
 * @param   PBV_Datatype_TX_t * - ptr to ascii data
 * @param   PBV_Datatype_TX_t * - ptr to received data ( for completeness, not used now)
 * @brief   
 * @return  void
 * @details initializes UART objects with application objects. Also initializes uartActiveTx and uartActiveTxAscii
 **********************************************************************************/

void PBV_UART_Init(PBV_Datatype_TX_t * Board_To_PBV, PBV_Datatype_TX_t * Board_To_PBVAscii, PBV_Datatype_RX_t * Pbv_To_Board)
{
    PBV_UART_Object_TX.Protocol_ID = Board_To_PBV->PBV_Protcol_ID;
    PBV_UART_Object_TX.offset = 0;
    PBV_UART_Object_TX.Length_in_Bytes = Board_To_PBV->Length;
    PBV_UART_Object_TX.data = Board_To_PBV->Data_Buffer;

    PBV_UART_Object_RX.offset = 0;

    PBV_UART_Object_ASCII.Protocol_ID = Board_To_PBVAscii->PBV_Protcol_ID;
    PBV_UART_Object_ASCII.offset = 0;
    PBV_UART_Object_ASCII.Length_in_Bytes = Board_To_PBVAscii->Length;
    PBV_UART_Object_ASCII.data = Board_To_PBVAscii->Data_Buffer;

    uartActiveTx = false;
    uartActiveTxAscii = false;
}

/*********************************************************************************
 * @ingroup PBV_UART
 * @fn      PBV_UART_Receive_from_GUI
 * @param   none
 * @brief   implements the state machine for UART Frame receiving from PBV.
 * @return  int (1: ready to receive, 2: Receiving, 0: Received successfully, -1: CRC error)
 * @details implements the state machine for message RX. maintains internal state machine states as defined in RCV_<STATES>
 **********************************************************************************/

uint8_t PBV_UART_Receive_from_GUI()
{ 
    static uint16_t rcv_data_index = 0;
    static uint16_t rcv_CRC = 0;
    static uint16_t rcv_timeout = 0;
    static  uint8_t rcv_copy_for_CRC[PBV_RCV_DATABUFFER_SIZE + PBV_HEADER_SIZE];     ///< local copy created to pass to CRC function. TODO: could use the UART RX object, i.e. data, and header as they are stored regardless. but the CRC implementation would then require some work.
    uint8_t data;

    while (1)
    {
        if (PBV_IsRxReady() == false)
        {
            if (++rcv_timeout >= 10000)
            {
                rcv_timeout = 0;
                PBV_UART_Object_RX.UART_Frame_State = RCV_WAIT_FOR_STARTBYTE;
            }
            return PBV_MESSAGE_READY_TO_RECEIVE;
        }
        // ok, we have some data coming in, lets read and process
        rcv_timeout = 0;
        data = PBV_Read();
        switch (PBV_UART_Object_RX.UART_Frame_State)
        {
        case RCV_WAIT_FOR_STARTBYTE:
            if (data == PBV_START_OF_FRAME)
            {
                PBV_UART_Object_RX.UART_Frame_State = RCV_READ_ID_HIGHBYTE;
                PBV_UART_Object_RX.offset = 0;
                PBV_UART_Object_RX.PBV_Header[PBV_UART_Object_RX.offset] = data; 
                rcv_copy_for_CRC[rcv_data_index] = data;                        // for CRC
                rcv_data_index++;                                               // for CRC
                PBV_UART_Object_RX.offset++;
            }
            return PBV_STATE_RECEIVING;

        case RCV_READ_ID_HIGHBYTE:
            PBV_UART_Object_RX.Protocol_ID = data << 8;
            PBV_UART_Object_RX.UART_Frame_State = RCV_READ_ID_LOWBYTE;
            PBV_UART_Object_RX.PBV_Header[PBV_UART_Object_RX.offset] = data; 
            rcv_copy_for_CRC[rcv_data_index] = data;                        // for CRC
            rcv_data_index++;                                               // for CRC
            PBV_UART_Object_RX.offset++;
            return PBV_STATE_RECEIVING;

        case RCV_READ_ID_LOWBYTE:
            PBV_UART_Object_RX.Protocol_ID |= data;
            PBV_UART_Object_RX.UART_Frame_State = RCV_READ_LENGTH_HIGHBYTE;
            PBV_UART_Object_RX.PBV_Header[PBV_UART_Object_RX.offset] = data; 
            rcv_copy_for_CRC[rcv_data_index] = data;                        // for CRC
            rcv_data_index++;                                               // for CRC
            PBV_UART_Object_RX.offset++;
            return PBV_STATE_RECEIVING;

        case RCV_READ_LENGTH_HIGHBYTE:
            // clear CRC flag, for now, as the firmware doesn't support it
            // With PBV version 2.0.0, dsPIC cannot receive
            // messages from the GUI if this flag is not cleared
            PBV_UART_Object_RX.PBV_Header[PBV_UART_Object_RX.offset] = data;
            rcv_copy_for_CRC[rcv_data_index] = data;                        // for CRC
            rcv_data_index++;                                               // for CRC
            data = data & 0x7F; // clear bit 7 (CRC flag) 
            PBV_UART_Object_RX.Length_in_Bytes = data << 8;
            PBV_UART_Object_RX.UART_Frame_State = RCV_READ_LENGTH_LOWBYTE;
            PBV_UART_Object_RX.offset++;
            return PBV_STATE_RECEIVING;

        case RCV_READ_LENGTH_LOWBYTE:
            PBV_UART_Object_RX.PBV_Header[PBV_UART_Object_RX.offset] = data;
            PBV_UART_Object_RX.Length_in_Bytes |= data;
            rcv_copy_for_CRC[rcv_data_index] = data;                        // for CRC
            rcv_data_index++;                                               // for CRC
            PBV_UART_Object_RX.UART_Frame_State = RCV_READ_DATA;
            PBV_UART_Object_RX.offset = 0;
            return PBV_STATE_RECEIVING;

        case RCV_READ_DATA:
            if (PBV_UART_Object_RX.offset < PBV_RCV_DATABUFFER_SIZE + PBV_HEADER_SIZE ) // MAX
            {
                PBV_UART_Object_RX.data[PBV_UART_Object_RX.offset] = data;
                PBV_UART_Object_RX.offset++;
                
                rcv_copy_for_CRC[rcv_data_index] = data;                        // for CRC
                rcv_data_index++;                                               // for CRC
            }
            if (PBV_UART_Object_RX.offset >= PBV_UART_Object_RX.Length_in_Bytes) //are we finished receiving data???
            {
                PBV_UART_Object_RX.UART_Frame_State = RCV_READ_CRC_HIGHBYTE;
            }
            return PBV_STATE_RECEIVING;

        case RCV_READ_CRC_HIGHBYTE:
            rcv_CRC = data << 8;
            PBV_UART_Object_RX.UART_Frame_State = RCV_READ_CRC_LOWBYTE;
            return PBV_STATE_RECEIVING;

        case RCV_READ_CRC_LOWBYTE:
            rcv_CRC |= data;
            PBV_UART_Object_RX.UART_Frame_State = RCV_READ_EOF;
            return PBV_STATE_RECEIVING;

        case RCV_READ_EOF:
            if (data == 0x0d)
            {
                ///< calculating CRC. data for CRC  [header][data]
                PBV_calculated_CRC = PBV_Calculate_CRC(rcv_copy_for_CRC, PBV_UART_Object_RX.Length_in_Bytes + PBV_HEADER_SIZE);
                if (PBV_calculated_CRC == rcv_CRC)
                {
                    rcv_data_index = 0;
                    rcv_CRC = 0;
                    PBV_calculated_CRC = 0;
                    PBV_UART_Object_RX.UART_Frame_State = RCV_MESSAGE_RECEIVED;
                    return PBV_MESSAGE_RECEIVED;
                }
                else {
                    rcv_data_index = 0;         //for CRC
                    return PBV_MESSAGE_RX_ERROR;
                }
            }
        }
    }
}

/*********************************************************************************
 * @ingroup  
 * @fn      PBV_UART_Transmit_Ascii_to_GUI
 * @param
 * @brief   
 * @return  int
 * @details implements the state machine for ascii  TX 
 **********************************************************************************/

uint8_t PBV_UART_Transmit_Ascii_to_GUI()
{
    uint8_t retval = PBV_MESSAGE_TX_ERROR;
    uint8_t temp;
    if (uartActiveTx == true)
    {
        retval = PBV_MESSAGE_TX_ERROR;
    }
    else
    {
        switch (PBV_UART_Object_ASCII.UART_Frame_State)
        {
            case 0:
                PBV_UART_Object_ASCII.PBV_Header[0] = PBV_START_OF_FRAME;
                PBV_UART_Object_ASCII.PBV_Header[1] = PBV_UART_Object_ASCII.Protocol_ID >> 8;
                PBV_UART_Object_ASCII.PBV_Header[2] = PBV_UART_Object_ASCII.Protocol_ID & 0xff;
                PBV_UART_Object_ASCII.PBV_Header[3] = PBV_UART_Object_ASCII.Length_in_Bytes >> 8;
                PBV_UART_Object_ASCII.PBV_Header[4] = PBV_UART_Object_ASCII.Length_in_Bytes & 0xff;
                PBV_UART_Object_ASCII.offset = 0;
                PBV_UART_Object_ASCII.UART_Frame_State = 1;
                uartActiveTxAscii = true;
                retval =  PBV_MESSAGE_TRANSMITTING;
            break;

            case 1: //transfer header
                if (PBV_ReadyToSend())//    && (MsCounter_++ > 1)) //@ftx
                {
                    temp = PBV_UART_Object_ASCII.PBV_Header[PBV_UART_Object_ASCII.offset];
                    PBV_Write(temp);
                    PBV_UART_Object_ASCII.offset++;
                    if (PBV_UART_Object_ASCII.offset > 4)
                    {
                        PBV_UART_Object_ASCII.offset = 0;
                        PBV_UART_Object_ASCII.UART_Frame_State = 2;
                        retval = PBV_MESSAGE_TRANSMITTING; // Why is this here?
                    }
                }
                else
                {
                    retval =  PBV_MESSAGE_TRANSMITTING;   
                }
                break;

            case 2: //transfer data 
                if (PBV_ReadyToSend())// && (MsCounter_++ > 1)) //@ftx
                {
                    PBV_Write(PBV_UART_Object_ASCII.data[PBV_UART_Object_ASCII.offset]);
                    PBV_UART_Object_ASCII.offset++;
                    if (PBV_UART_Object_ASCII.offset >= PBV_UART_Object_ASCII.Length_in_Bytes)
                    {
                        PBV_UART_Object_ASCII.UART_Frame_State = 3;
                        PBV_UART_Object_ASCII.offset = 0;
                        retval = PBV_MESSAGE_TRANSMITTING;
                    }
                }
                else
                {
                    retval =  PBV_MESSAGE_TRANSMITTING;
                }
                break;
                
            case 3: //calculate CRC make end header
                //IMPORTANT calculate crc before reusing PBV_HEADER
                PBV_UART_Object_ASCII.PBV_Header[0] = 0; //
                PBV_UART_Object_ASCII.PBV_Header[1] = 0; //add crc
                PBV_UART_Object_ASCII.PBV_Header[2] = PBV_END_OF_FRAME;
                PBV_UART_Object_ASCII.UART_Frame_State = 4;
                retval = PBV_MESSAGE_TRANSMITTING;
                break;

            case 4: //transfer header
                if (PBV_ReadyToSend())
                {
                    PBV_Write(PBV_UART_Object_ASCII.PBV_Header[PBV_UART_Object_ASCII.offset]);
                    PBV_UART_Object_ASCII.offset++;
                    if (PBV_UART_Object_ASCII.offset >= 3)
                    {
                        PBV_UART_Object_ASCII.UART_Frame_State = 0;
                        PBV_UART_Object_ASCII.offset = 0;
                        uartActiveTxAscii = false;
                        retval =  PBV_MESSAGE_TRANSMITTED;
                    }
                }
                else
                {
                    retval = PBV_MESSAGE_TRANSMITTING;
                }
                break;
            
            default:
                retval = PBV_MESSAGE_TRANSMITTING;
                break;
        }
    }
    return retval;
}

/*********************************************************************************
 * @ingroup  
 * @fn      PBV_UART_Transmit_to_GUI
 * @param
 * @brief   
 * @return  int
 * @details implements the state machine for numerical TX 
 **********************************************************************************/

uint8_t PBV_UART_Transmit_to_GUI()
{
    uint16_t temp;
    uint8_t retval = PBV_MESSAGE_TX_ERROR;
    if (uartActiveTxAscii == true)
    {
        retval =  PBV_MESSAGE_TX_ERROR;
    }
    else
    {
        switch (PBV_UART_Object_TX.UART_Frame_State)
        {
            case 0:
                PBV_UART_Object_TX.PBV_Header[0] = PBV_START_OF_FRAME;
                PBV_UART_Object_TX.PBV_Header[1] = PBV_UART_Object_TX.Protocol_ID >> 8;
                PBV_UART_Object_TX.PBV_Header[2] = PBV_UART_Object_TX.Protocol_ID & 0xff;
                PBV_UART_Object_TX.PBV_Header[3] = PBV_UART_Object_TX.Length_in_Bytes >> 8;
                PBV_UART_Object_TX.PBV_Header[4] = PBV_UART_Object_TX.Length_in_Bytes & 0xff;
                PBV_UART_Object_TX.offset = 0;
                PBV_UART_Object_TX.UART_Frame_State = 1;
                uartActiveTx = true;
                retval = PBV_MESSAGE_TRANSMITTING;
                break;

            case 1: //transfer header
                if (PBV_ReadyToSend())//    && (MsCounter_++ > 1)) //@ftx
                {
                    temp = PBV_UART_Object_TX.PBV_Header[PBV_UART_Object_TX.offset];
                    PBV_Write(temp);
                    PBV_UART_Object_TX.offset++;
                    if (PBV_UART_Object_TX.offset > 4)
                    {
                        PBV_UART_Object_TX.offset = 0;
                        PBV_UART_Object_TX.UART_Frame_State = 2;
                        retval = PBV_MESSAGE_TRANSMITTING;
                    }
                }
                else
                {
                    retval =  PBV_MESSAGE_TRANSMITTING;
                }
                break;

            case 2: //transfer data 
                if (PBV_ReadyToSend())// && (MsCounter_++ > 1)) //@ftx
                {
                    PBV_Write(PBV_UART_Object_TX.data[PBV_UART_Object_TX.offset]);
                    PBV_UART_Object_TX.offset++;
                    if (PBV_UART_Object_TX.offset >= PBV_UART_Object_TX.Length_in_Bytes)
                    {
                        PBV_UART_Object_TX.UART_Frame_State = 3;
                        PBV_UART_Object_TX.offset = 0;
                        retval = PBV_MESSAGE_TRANSMITTING;
                    }
                }
                else
                {
                    retval = PBV_MESSAGE_TRANSMITTING;
                }
                break;

            case 3: //calculate CRC make end header
                //IMPORTANT calculate crc before reusing PBV_HEADER
                PBV_UART_Object_TX.PBV_Header[0] = 0; //
                PBV_UART_Object_TX.PBV_Header[1] = 0; //add crc
                PBV_UART_Object_TX.PBV_Header[2] = PBV_END_OF_FRAME;
                PBV_UART_Object_TX.UART_Frame_State = 4;
                retval = PBV_MESSAGE_TRANSMITTING;
                break;

            case 4: //transfer header
                if (PBV_ReadyToSend())
                {
                    PBV_Write(PBV_UART_Object_TX.PBV_Header[PBV_UART_Object_TX.offset]);
                    PBV_UART_Object_TX.offset++;
                    if (PBV_UART_Object_TX.offset >= 3)
                    {
                        PBV_UART_Object_TX.UART_Frame_State = 0;
                        PBV_UART_Object_TX.offset = 0;
                        uartActiveTx = false;
                        retval = PBV_MESSAGE_TRANSMITTED;
                    }
                }
                else
                {
                    retval = PBV_MESSAGE_TRANSMITTING;
                }
            
            default:
                retval = PBV_MESSAGE_TRANSMITTING;
                break;
        }
    }
    return (retval);
}

/*********************************************************************************
 * @ingroup PBV_UART
 * @fn      PBV_UART_Reniit
 * @param   
 * @brief   
 * @return  
 * @details reinitializes the UART object with new can id. 
 **********************************************************************************/

void PBV_UART_Reinit(PBV_Datatype_TX_t * ptr)
{
    if (ptr->PBV_Signal_Ascii == PBV_SIGNAL_MODE)
        PBV_UART_Object_TX.Protocol_ID = ptr->PBV_Protcol_ID;
    else
        PBV_UART_Object_ASCII.Protocol_ID = ptr->PBV_Protcol_ID;
}

/*********************************************************************************
 * @ingroup PBV_UART 
 * @fn      PBV_UART_Link_Data_TX
 * @param   PBV_Datatype_TX_t *
 * @brief   links the data from the calling application to the UART TX object
 * @return  void
 * @details links the data from the calling application to the UART TX object. 
 **********************************************************************************/

void PBV_UART_Link_Data_TX(PBV_Datatype_TX_t * ptr)
{
    if (ptr->PBV_Signal_Ascii == PBV_SIGNAL_MODE)
    {
        PBV_UART_Object_TX.data             =    ptr->Data_Buffer;
        PBV_UART_Object_TX.Length_in_Bytes  =    ptr->Length;
    }
    else
    {
        PBV_UART_Object_ASCII.data          =   ptr->Data_Buffer;
        PBV_UART_Object_TX.Length_in_Bytes  =   ptr->Length;
    }
}

/*********************************************************************************
 * @ingroup PBV_UART 
 * @fn      PBV_UART_Link_Data_RX
 * @param   PBV_Datatype_RX_t * 
 * @brief   Links the data from received frame to the calling application    
 * @return  void
 * @details Links the data from received frame to the calling application    
 **********************************************************************************/

void PBV_UART_Link_Data_RX(PBV_Datatype_RX_t * ptr)
{
    if (ptr->PBV_Message_State == PBV_MESSAGE_RECEIVED){
        ptr->PBV_Protcol_ID = PBV_UART_Object_RX.Protocol_ID;
        ptr->Data_Buffer = PBV_UART_Object_RX.data;
        ptr->Length = PBV_UART_Object_RX.Length_in_Bytes;
    } 
}

/*********************************************************************************
 * @ingroup PBV_UART_
 * @fn      PBV_Calculate_CRC
 * @param   uint8_t * pointer to data. 
 * @param   uint16_t length of data in bytes
 * @brief   Calculates the CRC on 16 bit stream of data
 * @return  calculated CRC
 * @details calculates the 16 bit CRC on an incoming stream of bytes. the data 
 * for CRC is [header][data]. odd byte processed seperately. 
 * MAnually cleared shift registers in the end. Ticket has been raised to fix this.
 * uses the CRC peripheral 
 **********************************************************************************/

uint16_t PBV_Calculate_CRC(uint8_t *message, uint16_t length)
{
    //CRC_Initialize();
    uint8_t message_odd = (uint8_t) (length & 0x01);
    uint16_t length_t = 0;
    uint16_t i, j;
    //uint16_t * message16bit;
    uint32_t trycount = 0;
    uint16_t resultCRC = 0;
    uint16_t message16bit[PBV_RCV_DATABUFFER_SIZE]; /// TODO: FIX THIS

    //calCRCTEMP = 0; //

    if (message_odd)
        length_t = (length & 0xFFFE);
    else
        length_t = length;

    for (i = 0, j = 0; i < length_t; i += 2, j++)
    {
        //*(message16bit + j) = (message[i + 1 ] << 8 ) | message[i] ; 
        message16bit[j] = (message[i + 1 ] << 8) | message[i];
    }

    CRC_CalculateBufferStart(message16bit, length_t);

    while (!CRC_CalculationIsDone() && trycount < 2000)
    {
        CRC_Tasks();
        trycount++;
    }

    resultCRC = CRC_CalculationResultReverseGet();

    if (message_odd)
    {
        resultCRC ^= message[length - 1];
        for (i = 0; i < 8; ++i)
        {
            if (resultCRC & 0x0001)
            {
                resultCRC = (resultCRC >> 1)^ PBV_CRC_POLYNOM_REV;
            }
            else
            {
                resultCRC = (resultCRC >> 1);
            }
        }
    }
    // clearing shift registers.
    CRCWDATL = 0x0;
    CRCWDATH = 0x0;
    
    return resultCRC;
}
