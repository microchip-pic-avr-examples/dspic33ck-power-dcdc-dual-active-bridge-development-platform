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
/**
 * @file    app_PBV_dab_frame_map.h
 * @brief   app PBV dab Frame map file Example
 * @author  M70027
 * @ingroup app-pbv-dab-map
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

// MCC header files
#include "system/pins.h"
#include "device/device.h"

// other header files
#include "PBV_interface.h"
#include "pwrctrl/pwrctrl_comm_interface.h"
#include "fault/fault_comm_interface.h"
#include "config/macros.h"
#include "config/version.h"

/*********************************************************************************
 * @ingroup app-pbv-dab-map
 * @{
 **********************************************************************************/
#define FIRMWARE_PROTOCOL_ID            0x1000           ///< Firmware ID
#define PBV_TX_PROTOCOL_ID              0x404            ///< ID on which data is transmitted by dsPIC33
#define PBV_LOG_ID                      0x300            ///< ID on which log data is sent. 

// command IDs, first data word in received package
// use this to decide what action to take when data is received
#define PBV_CMD_ID_DAB_ON               0x5501           ///< turn DAB on
#define PBV_CMD_ID_DAB_OFF              0x5500           ///< turn DAB off
#define PBV_CMD_ID_FREQ_CHANGE          0xBBBB           ///< change DAB switching frequency
#define PBV_CMD_ID_FAN_SPEED            0xCCCC           ///< set fan speed 

#define PBV_CMD_ID_ILOOP_REF_SET        0xDDDD           ///< set current loop reference
#define PBV_CMD_ID_VLOOP_REF_SET        0xDDDE           ///< set voltage loop reference
#define PBV_CMD_ID_PLOOP_REF_SET        0xDDDF           ///< set voltage loop reference

#define PBV_CMD_ID_PHASE_CHANGE         0xEE01           ///< set control phase
#define PBV_CMD_ID_P2S_PHASE_TARGET     0xEE02           ///< set control phase

/** @} */ // end of pbv-protocol-ids

// static because these are private.

static PBV_Datatype_TX_t appPbvDabTx;          ///< Application TX object
static PBV_Datatype_RX_t appPbvDabRx;          ///< Application RX object
static PBV_Datatype_TX_t appPbvDabAscii;       ///< Application TX object for ascii

static PBV_Datatype_TX_t * appPbvDabTxPtr = &appPbvDabTx;        ///< Application TX object pointer
static PBV_Datatype_RX_t * appPbvDabRxPtr = &appPbvDabRx;        ///< Application RX object pointer
static PBV_Datatype_TX_t * appPbvDabAsciiPtr = &appPbvDabAscii;  ///< Application TX object ascii pointer

uint8_t bufferEightRx[64];
uint16_t bufferSixteenRx[32];

uint8_t bufferEightTx[64];
uint16_t bufferSixteenTx[32];

static uint32_t tickCounter = 0;
static uint8_t transmitFirmwareId = 1;

// temporary variables
static int16_t temperature = 0;

/***********************************************************************************
 * Private Functions Prototypes
 **********************************************************************************/

void App_PBV_DAB_Build_Frame(void);
void App_PBV_DAB_Process_Rx_Data(uint16_t * data);
void protocolID(uint16_t protocol_ID, uint16_t length, uint8_t * data);
/***********************************************************************************
 * Public Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @return  void
 * @brief   this function initializes the local pbv objects. these objects are then
 *          passed on to the app_PBV_init function to initialize the CAN/UART objects
 * @details
 *          RX object just needs state change. 
 **********************************************************************************/
void App_PBV_DAB_Init()
{
    appPbvDabTxPtr->PBV_Protcol_ID        = PBV_TX_PROTOCOL_ID;
    appPbvDabTxPtr->PBV_Signal_Ascii      = PBV_SIGNAL_MODE;
    appPbvDabTxPtr->PBV_Message_State     = PBV_MESSAGE_INIT;
    appPbvDabTxPtr->Length                = 64;

    appPbvDabRxPtr->PBV_Message_State     = PBV_MESSAGE_READY_TO_RECEIVE;

    appPbvDabAsciiPtr->PBV_Protcol_ID     = FIRMWARE_PROTOCOL_ID;
    appPbvDabAsciiPtr->PBV_Signal_Ascii   = PBV_ASCII_MODE;
    appPbvDabAsciiPtr->PBV_Message_State  = PBV_MESSAGE_INIT;
    appPbvDabAsciiPtr->Length             = 64;

    App_PBV_Init(appPbvDabTxPtr, appPbvDabAsciiPtr, appPbvDabRxPtr);
}

/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @return  void
 * @brief   10ms PBV task to be execution
 * @details This is high frequency task to simulate sending of high frequency numeric data.
 *          Also to check if there are any messages received, must be done at a high enough
 *          rate to ensure that the messages are not dropped.
 *   
 **********************************************************************************/
void App_PBV_DAB_Task_10ms(void)
{
    // RX handler
    if (appPbvDabRxPtr->PBV_Message_State == PBV_MESSAGE_RECEIVED)
    {        
        App_Read_Received_From_PBV(appPbvDabRxPtr);       
        protocolID(appPbvDabRxPtr->PBV_Protcol_ID, appPbvDabRxPtr->Length, appPbvDabRxPtr->Data_Buffer);
        
        // msg read. Read another
        App_Receive_From_PBV(appPbvDabRxPtr); 
    } 
    ///< 110ms sending 
    if (++tickCounter > 11)
    {
        App_PBV_DAB_Build_Frame();
        App_Send_To_PBV(appPbvDabTxPtr);  
        tickCounter = 0;
    }    
}

/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @param   void
 * @return  nothing
 * @brief   1 second PBV task to be execution
 * @details This is a slow task simulating the low speed sending of ascii data
 *           it sends the firmware id, and the switches to the log id.
 *   
 **********************************************************************************/
void App_PBV_DAB_Task_1s(void)
{
    if (appPbvDabAsciiPtr->PBV_Protcol_ID == FIRMWARE_PROTOCOL_ID)
    {
        appPbvDabAsciiPtr->Data_Buffer = (uint8_t *)FIRMWARE_VERSION_STRING;
        App_Send_To_PBV(appPbvDabAsciiPtr);
        appPbvDabAsciiPtr->PBV_Protcol_ID = PBV_LOG_ID;
        transmitFirmwareId = 1;
        return;
    }
    if (appPbvDabAsciiPtr->PBV_Protcol_ID == PBV_LOG_ID)
    {
        if (transmitFirmwareId) App_PBV_Re_Init(appPbvDabAsciiPtr);     ///< reinit to new id
        transmitFirmwareId = 0; 
    }

    appPbvDabAsciiPtr->Data_Buffer = (uint8_t *)"                 Log Message From Protocol ID 0x300";
    App_Send_To_PBV(appPbvDabAsciiPtr);
   
    temperature = (int16_t)Dev_Temp_Get_Temperature_Celcius();
}

/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @return  PBV_Datatype_TX_t *
 * @brief   This function can be used to send log messages from other files.
 * @details This function can be used to send log messages from other files.
 *   
 **********************************************************************************/
PBV_Datatype_TX_t * App_PB_DAB_Get_TX_ASCII_ptr(void)
{
    if (appPbvDabAsciiPtr ->PBV_Message_State == PBV_MESSAGE_TRANSMITTING)
    {
        return 0;
    }
    return appPbvDabAsciiPtr;
}

/***********************************************************************************
 * Private Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup pbv-private-function
 * @return  void
 * @brief   this builds frame
 * @details
 **********************************************************************************/

void App_PBV_DAB_Build_Frame()
{        
    // Power Board Visualizer can only mask single bits
    // so create maskable word that can be parsed by GUI by sending the
    // "2 ^ (state)" instead of "state"
    bufferSixteenTx[0] = 1<<(Dev_PwrCtrl_Get_State());
    
    // send back one "flag word" which combines fault and status and enable control flag
    uint16_t enabled = Dev_PwrCtrl_Get_EnableFlag();
    uint16_t fault_flags = Fault_GetFlags();
    uint16_t status_flags = Dev_PwrCtrl_Get_Status();
    uint16_t flag_word = enabled + ((status_flags & 0x0003)<<1) + (fault_flags<<3);
    
    bufferSixteenTx[1] = flag_word;
    bufferSixteenTx[2] = Dev_PwrCtrl_GetAveraging_Vprim(); 
    bufferSixteenTx[3] = Dev_PwrCtrl_GetAveraging_Vsec();
    bufferSixteenTx[4] = PwrCtrl_GetAdc_Ipri_ct();
    bufferSixteenTx[5] = PwrCtrl_GetAdc_Isec_ct();
    bufferSixteenTx[6] = Dev_PwrCtrl_GetAveraging_Isec();
    bufferSixteenTx[7] = temperature + 40;
    bufferSixteenTx[8] = PwrCtrl_GetAdc_Vrail_5V();    
    bufferSixteenTx[9] =  PwrCtrl_GetPhase_P2SDegree();
    bufferSixteenTx[10] = devFanDataPtr->CurrentSpeedPercent;
    bufferSixteenTx[11] = temperature;
    bufferSixteenTx[12] = Dev_PwrCtrl_Get_Period();
    bufferSixteenTx[13] = Dev_PwrCtrl_Get_PwmprdTarget();
    bufferSixteenTx[14] = Dev_PwrCtrl_Get_SecPower(); 
    
    PBV_Change_from_Sixteen_to_Eight(bufferSixteenTx, bufferEightTx, 18);
    
    appPbvDabTxPtr->Data_Buffer = bufferEightTx;
    appPbvDabTxPtr->Length = 18 * 2 ;
}


/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @param   pointer to received data
 * @return  nothing
 * @brief   process received data
 * @details
 * @note
 **********************************************************************************/
void App_PBV_DAB_Process_Rx_Data(uint16_t * data) 
{
    uint16_t cmd_id = data[0];
    uint16_t control_word = data[1];
    switch (cmd_id)
    {
        case PBV_CMD_ID_DAB_ON: {
            PwrCtrl_SetEnable(true);
            break;
        }
        case PBV_CMD_ID_DAB_OFF: {
            PwrCtrl_SetEnable(false);
            Dev_PwrCtrl_SetState(0);
            break;
        } 
        case PBV_CMD_ID_FREQ_CHANGE: {
            // change target frequency
            if ((control_word <= MAX_PWM_PERIOD) && (control_word >= MIN_PWM_PERIOD)) {
                PwrCtrl_SetPeriodTarget(control_word);
                // when Frequency is changed, control phase will be zero
                uint16_t controlPhase = 0;
                PwrCtrl_SetPhaseTarget(controlPhase);
            }            
            break; 
        }
        case PBV_CMD_ID_ILOOP_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                PwrCtrl_SetIReference(control_word);
            }
            break; 
        }        
        case PBV_CMD_ID_VLOOP_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                PwrCtrl_SetVSecReference(control_word);
            }
            break; 
        }  
        
        case PBV_CMD_ID_PLOOP_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                PwrCtrl_SetPwrReference(control_word);
            }
            break; 
        }  
       
        case  PBV_CMD_ID_P2S_PHASE_TARGET: {
            // change target phase
            #if(OPEN_LOOP_PBV == false)
            uint16_t controlPhase = (uint16_t)(control_word);
            PwrCtrl_SetP2SPhaseTarget(controlPhase);
            #endif
            break; 
        }
                
        case PBV_CMD_ID_PHASE_CHANGE: {
            // change target phase
            #if(OPEN_LOOP_PBV == true)
            uint16_t controlPhase = (uint16_t)((control_word)* PHASE_180_SCALER * (Dev_PwrCtrl_Get_DutyCycle()));
            PwrCtrl_SetPhaseTarget(controlPhase); 
            #endif
            break; 
        }
        case PBV_CMD_ID_FAN_SPEED: {            
            Dev_Fan_Set_Speed(control_word);
            break; 
        }
                 
        default:
            break;
    }
}

/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @param   void
 * @return  nothing
 * @brief   default callback
 * @details
 * @note
 **********************************************************************************/
void protocolID(uint16_t protocol_ID, uint16_t length, uint8_t * data)
{
    PBV_Change_from_Eight_to_Sixteen(data, bufferSixteenRx, length);
    App_PBV_DAB_Process_Rx_Data(bufferSixteenRx);
}


/*******************************************************************************
 * end of file
 *******************************************************************************/