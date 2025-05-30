
/**
 * @file    app_PBV_dab_frame_map.h
 * @brief   app PBV dab Frame map file Example
 * @author  M70027
 * @ingroup app-pbv-dab-map
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types

#include <stdio.h>
#include <string.h>
//#include <stdlib.h>

#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

// MCC header files
#include "system/pins.h"
#include "device/device.h"

// other header files
#include "PBV_interface.h"
#include "pwrctrl/pwrctrl_typedef.h"
#include "pwrctrl/pwrctrl_comm_interface.h"
#include "pwrctrl/pwrctrl.h"
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
#define PBV_CMD_ID_DAB_ON_REV           0x5502           ///< turn DAB on - REVERSED discharge mode
#define PBV_CMD_ID_DAB_ON               0x5501           ///< turn DAB on
#define PBV_CMD_ID_DAB_OFF              0x5500           ///< turn DAB off
#define PBV_CMD_ID_FREQ_CHANGE          0xBBBB           ///< change DAB switching frequency
#define PBV_CMD_ID_FAN_SPEED            0xCCCC           ///< set fan speed 

#define PBV_CMD_ID_VLOOPREV_REF_SET     0xDDDC           ///< set voltage loop reference reverse mode
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

static uint8_t transmitFirmwareId = 1;



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
 * @brief   This function initializes the local pbv objects. these objects are then
 *          passed on to the app_PBV_init function to initialize the CAN/UART objects
 * @details
 *          This function initializes the local pbv objects. these objects are then
 *          passed on to the app_PBV_init function to initialize the CAN/UART objects.
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
    static uint32_t tickCounter = 0;
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
 * @brief   1 second PBV task to be execution
 * @details This is a slow task simulating the low speed sending of ascii data
 *           it sends the firmware id, and the switches to the log id.
 *   
 **********************************************************************************/
void App_PBV_DAB_Task_1s(void)
{
    static uint16_t OneSecCounter;//debug log print demo purpose. Sporadic Resets can be detected by checking this number
    static uint8_t PBVBuffer[64<<1];//PBV msg buffer. Take care of 64B length boundary when creating messages. 

    for(uint16_t i=0; i<(64); i++) PBVBuffer[i]=0;//clear to 0 all 64 bytes
    
    if (appPbvDabAsciiPtr->PBV_Protcol_ID == FIRMWARE_PROTOCOL_ID)
    {
        strcpy((char *)&PBVBuffer[0], (char *)FIRMWARE_VERSION_STRING);
        strcpy((char *)&PBVBuffer[10], (char *)FIRMWARE_NAME);
        appPbvDabAsciiPtr->Data_Buffer = &PBVBuffer[0];
                
        App_Send_To_PBV(appPbvDabAsciiPtr);
        appPbvDabAsciiPtr->PBV_Protcol_ID = PBV_LOG_ID;
        transmitFirmwareId = 1;
        return;
    }
    

    if (appPbvDabAsciiPtr->PBV_Protcol_ID == PBV_LOG_ID)
    {
        if (transmitFirmwareId) App_PBV_Re_Init(appPbvDabAsciiPtr);     ///< reinit to new id
        transmitFirmwareId = 0; 
        
        if(OneSecCounter)
        {
            if(!(OneSecCounter%20))
            {
 //                                    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";   
                sprintf((char *)&PBVBuffer[0], "\rDAB board heat sink Temperature is %d degree Celsius  ", Dev_Temp_Get_Temperature_Celcius() );
            }
            else
            sprintf((char *)&PBVBuffer[0], "\r Dual Active Bridge. 64B fixed length Log. MsgNo %d ", OneSecCounter);
        }
        else
        {   
            sprintf((char *)&PBVBuffer[0], "\r Dual Active Bridge. AFTER RESET SYSTEM STARTUP  " ); 
        }

        appPbvDabAsciiPtr->Data_Buffer =&PBVBuffer[0];
        App_Send_To_PBV(appPbvDabAsciiPtr);//64B fixed frame
        
        OneSecCounter++;
    }              
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
 * @brief   This builds application frame
 * @details This function builds the frame of the Power board visualizer for
 *  the dual active bridge application. 
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
    uint16_t PowDir_flags = Dev_PwrCtrl_Get_PowerDir();
    uint16_t flag_word = enabled + ((status_flags & 0x0003)<<1) + (fault_flags<<3) +  (PowDir_flags<<14) ;
    bufferSixteenTx[1] = flag_word;
    bufferSixteenTx[2] = Dev_PwrCtrl_GetAveraging_Vprim(); 
    bufferSixteenTx[3] = Dev_PwrCtrl_GetAveraging_Vsec();
    bufferSixteenTx[4] = PwrCtrl_GetAdc_Ipri_ct();
    bufferSixteenTx[5] = PwrCtrl_GetAdc_Isec_ct();
    bufferSixteenTx[6] = Dev_PwrCtrl_GetAveraging_Isec();
    bufferSixteenTx[7] = (uint16_t)(TEMPERATURE_PBV_OFFSET_CELSIUS + (int16_t)Dev_Temp_Get_Temperature_Celcius());
    bufferSixteenTx[8] = PwrCtrl_GetAdc_Vrail_5V();    
    bufferSixteenTx[9] =  PwrCtrl_GetPhase_P2SDegree();
    bufferSixteenTx[10] = 0;//devFanDataPtr->CurrentSpeedPercent;
    bufferSixteenTx[11] = 0;//(uint16_t)(TEMPERATURE_PBV_OFFSET_CELSIUS + (int16_t)Dev_Temp_Get_Temperature_Celcius());
    bufferSixteenTx[12] = Dev_PwrCtrl_Get_Period();
    bufferSixteenTx[13] = Dev_PwrCtrl_Get_PwmprdTarget();
    bufferSixteenTx[14] = Dev_PwrCtrl_Get_SecPower(); 
    
    PBV_Change_from_Sixteen_to_Eight(bufferSixteenTx, bufferEightTx, 18);
    
    appPbvDabTxPtr->Data_Buffer = bufferEightTx;
    appPbvDabTxPtr->Length = 18 * 2 ;
}


/***********************************************************************************
 * @ingroup app-pbv-dab-map
 * @param   data pointer to received data
 * @brief   process received data
 * @details This function transfer the data coming from the Power Board Visualizer 
 *  to the MCU to process the action taken by the user in the Dual active Bridge Board. 
 **********************************************************************************/
void App_PBV_DAB_Process_Rx_Data(uint16_t * data) 
{
    uint16_t cmd_id = data[0];
    uint16_t control_word = data[1];
    switch (cmd_id)
    {
        case PBV_CMD_ID_DAB_ON: {
            //Dev_PwrCtrl_SetState(0);
            PwrCtrl_SetEnable(PWR_CTRL_CHARGING);
            break;
        }
        case PBV_CMD_ID_DAB_ON_REV: {
            //Dev_PwrCtrl_SetState(0);
            PwrCtrl_SetEnable(PWR_CTRL_DISCHARGING);
            break;
        }
        case PBV_CMD_ID_DAB_OFF: {
            PwrCtrl_SetEnable(PWR_CTRL_STOP);
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
        case PBV_CMD_ID_VLOOPREV_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                PwrCtrl_SetVPriReference(control_word);
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
 * @param   protocal_ID indicates the protocol ID for the PBV
 * @param   length  length of the data
 * @param   data    pointer to the data
 * @brief   default callback
 * @details This function defines the PBV protocol ID for specific data received. 
 * @note    This function defines the PBV protocol ID for specific data received. 
 **********************************************************************************/
void protocolID(uint16_t protocol_ID, uint16_t length, uint8_t * data)
{
    PBV_Change_from_Eight_to_Sixteen(data, bufferSixteenRx, length);
    App_PBV_DAB_Process_Rx_Data(bufferSixteenRx);
}


/*******************************************************************************
 * end of file
 *******************************************************************************/