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

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

// MCC header files

// other header files
#include "App_PBV_DAB_frame_map.h"

#include "system/pins.h"
#include "device/dev_fan.h"
#include "device/fault/dev_fault_temp.h"
#include "device/pwrctrl/dev_pwrctrl_api.h"
#include "device/fault/dev_fault_api.h"
#include "config/macros.h"
#include "config/version.h"

/*********************************************************************************
 * @ingroup pbv-protocol-ids
 * @
 * @brief   protocol ids defined. firmware id 0x1000 is fixed. the rest are arbitary
 **********************************************************************************/
#define FIRMWARE_PROTOCOL_ID            0x1000           ///< Firmware ID

#define PBV_RX_PROTOCOL_ID              0x202            ///< ID on which data is received by dsPIC33C
#define PBV_TX_PROTOCOL_ID              0x204            ///< ID on which data is transmitted by dsPIC33

#define PBV_GUI_BUTTONS                 0x202            ///< ID on which data is received Buttons.         
#define PBV_GUI_SLIDERS                 0x203            ///< ID on which data is received Sliders. 
#define PBV_SYSTEM_TO_GUI_ID            0x204            ///< ID on which data is sent
#define PBV_LOG_ID                      0x300            ///< ID on which log data is sent. 

// command IDs, first data word in received package
// use this to decide what action to take when data is received
#define PBV_CMD_ID_DAB_ON_OFF           0xAAAA           ///< turn DAB on or off
#define PBV_CMD_ID_DAB_RESET            0xAAAB           ///< DAB operation is resetted
#define PBV_CMD_ID_FREQ_CHANGE          0xBBBB           ///< change DAB switching frequency
#define PBV_CMD_ID_PWM_DTH              0xBBBC           ///< change DAB PWM DeadTime High
#define PBV_CMD_ID_PWM_DTL              0xBBBD           ///< change DAB PWM DeadTime Low
#define PBV_CMD_ID_FAN_SPEED            0xCCCC           ///< set fan speed 

#define PBV_CMD_ID_ILOOP_REF_SET        0xDDDD           ///< set current loop reference
#define PBV_CMD_ID_VLOOP_REF_SET        0xDDDE           ///< set voltage loop reference
#define PBV_CMD_ID_PLOOP_REF_SET        0xDDDF           ///< set voltage loop reference

#define PBV_CMD_ID_PHASE_CHANGE         0xEE01           ///< set control phase

#define PBV_CMD_ID_PRI_OVP_TEST         0xEE11           ///< set over voltage protection threshold
#define PBV_CMD_ID_SEC_OVP_TEST         0xEE10           ///< set over voltage protection threshold
#define PBV_CMD_ID_IPRI_TEST            0xEE20           ///< set primary current protection threshold
#define PBV_CMD_ID_ISEC_TEST            0xEE30           ///< set secondary current protection threshold


/** @} */ // end of pbv-protocol-ids

// static because these are private.

static PBV_Datatype_TX_t App_PBV_DAB_TX;          ///< Application TX object
static PBV_Datatype_RX_t App_PBV_DAB_RX;          ///< Application RX object
static PBV_Datatype_TX_t App_PBV_DAB_ASCII;       ///< Application TX object for ascii

static PBV_Datatype_TX_t * App_PBV_DAB_TX_Ptr = &App_PBV_DAB_TX;        ///< Application TX object pointer
static PBV_Datatype_RX_t * App_PBV_DAB_RX_Ptr = &App_PBV_DAB_RX;        ///< Application RX object pointer
static PBV_Datatype_TX_t * App_PBV_DAB_ASCII_Ptr = &App_PBV_DAB_ASCII;  ///< Application TX object ascii pointer

uint8_t buffer_eight_rx[64];
uint16_t buffer_sixteen_rx[32];

uint8_t buffer_eight_tx[64];
uint16_t buffer_sixteen_tx[32];

static uint32_t tick_counter = 0;
static uint8_t transmit_firmware_id = 1;

// temporary variables
static int16_t temperature = 0;

/***********************************************************************************
 * Private Functions Prototypes
 **********************************************************************************/

void App_PBV_DAB_Build_Frame(void);
void App_PBV_DAB_Process_Rx_Data(uint16_t * data);
void App_PBV_DAB_Frame_Parser(uint16_t protocol_ID, uint16_t length, uint8_t * data);
/***********************************************************************************
 * Public Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @param   void
 * @return  nothing
 * @brief   this function initializes the local pbv objects. these objects are then
 *          passed on to the app_PBV_init function to initialize the CAN/UART objects
 * @details
 *          RX object just needs state change. 
 **********************************************************************************/
void App_PBV_DAB_Init()
{
    App_PBV_DAB_TX_Ptr->PBV_Protcol_ID        = PBV_TX_PROTOCOL_ID;
    App_PBV_DAB_TX_Ptr->PBV_Signal_Ascii      = PBV_SIGNAL_MODE;
    App_PBV_DAB_TX_Ptr->PBV_Message_State     = PBV_MESSAGE_INIT;
    App_PBV_DAB_TX_Ptr->Length                = 64;

    App_PBV_DAB_RX_Ptr->PBV_Message_State     = PBV_MESSAGE_READY_TO_RECEIVE;

    App_PBV_DAB_ASCII_Ptr->PBV_Protcol_ID     = FIRMWARE_PROTOCOL_ID;
    App_PBV_DAB_ASCII_Ptr->PBV_Signal_Ascii   = PBV_ASCII_MODE;
    App_PBV_DAB_ASCII_Ptr->PBV_Message_State  = PBV_MESSAGE_INIT;
    App_PBV_DAB_ASCII_Ptr->Length             = 64;

    App_PBV_Init(App_PBV_DAB_TX_Ptr, App_PBV_DAB_ASCII_Ptr, App_PBV_DAB_RX_Ptr);
}

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @param   void
 * @return  nothing
 * @brief   this is high frequency task to simulate sending of high frequency numeric data.
 *          Also to check if there are any messages received, must be done at a high enough
 *          rate to ensure that the messages are not dropped.
 * @details
 *   
 **********************************************************************************/

void App_PBV_DAB_Task_10ms(void)
{
    // RX handler
    if (App_PBV_DAB_RX_Ptr->PBV_Message_State == PBV_MESSAGE_RECEIVED)
    {        
        App_Read_Received_From_PBV(App_PBV_DAB_RX_Ptr);       
        App_PBV_DAB_Frame_Parser(App_PBV_DAB_RX_Ptr->PBV_Protcol_ID, App_PBV_DAB_RX_Ptr->Length, App_PBV_DAB_RX_Ptr->Data_Buffer);
        
        // msg read. Read another
        App_Receive_From_PBV(App_PBV_DAB_RX_Ptr); 
    } 
    ///< 110ms sending 
    if (++tick_counter > 11)
    {
        App_PBV_DAB_Build_Frame();
        App_Send_To_PBV(App_PBV_DAB_TX_Ptr);  
        tick_counter = 0;
    }    
}

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @param   void
 * @return  nothing
 * @brief   this is a slow task simulating the low speed sending of ascii data
 *           it sends the firmware id, and the switches to the log id.
 * @details
 *   
 **********************************************************************************/
void App_PBV_DAB_Task_1s(void)
{
    if (App_PBV_DAB_ASCII_Ptr->PBV_Protcol_ID == FIRMWARE_PROTOCOL_ID)
    {
        App_PBV_DAB_ASCII_Ptr->Data_Buffer = (uint8_t *)FIRMWARE_VERSION_STRING;
        App_Send_To_PBV(App_PBV_DAB_ASCII_Ptr);
        App_PBV_DAB_ASCII_Ptr->PBV_Protcol_ID = PBV_LOG_ID;
        transmit_firmware_id = 1;
        return;
    }
    if (App_PBV_DAB_ASCII_Ptr->PBV_Protcol_ID == PBV_LOG_ID)
    {
        if (transmit_firmware_id) App_PBV_Re_Init(App_PBV_DAB_ASCII_Ptr);     ///< reinit to new id
        transmit_firmware_id = 0; 
    }
    //ToDo: Check with Cormac
    App_PBV_DAB_ASCII_Ptr->Data_Buffer = (uint8_t *)"                 Log Message From Protocol ID 0x300";
    App_Send_To_PBV(App_PBV_DAB_ASCII_Ptr);
   
    temperature = Dev_Temp_Get_Temperature_Celcius();
}

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @param   void
 * @return  PBV_Datatype_TX_t *
 * @brief   this function can be used to send log messages from other files.
 * @details
 *   
 **********************************************************************************/
PBV_Datatype_TX_t * App_PB_DAB_Get_TX_ASCII_ptr(void)
{
    if (App_PBV_DAB_ASCII_Ptr ->PBV_Message_State == PBV_MESSAGE_TRANSMITTING)
    {
        return 0;
    }
    return App_PBV_DAB_ASCII_Ptr;
}

/***********************************************************************************
 * Private Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup pbv-private-function
 * @param   void
 * @return  nothing
 * @brief   this builds frame
 * @details
 * @note
 **********************************************************************************/

void App_PBV_DAB_Build_Frame()
{        
    // Power Board Visualizer can only mask single bits
    // so create maskable word that can be parsed by GUI by sending the
    // "2 ^ (state)" instead of "state"
    buffer_sixteen_tx[0] = 1<<(Dev_PwrCtrl_Get_State());
    
    // send back one "flag word" which combines fault and status and enable control flag
    uint16_t enabled = Dev_PwrCtrl_Get_EnableFlag();
    uint16_t fault_flags = Dev_Fault_GetFlags();
    uint16_t status_flags = Dev_PwrCtrl_Get_Status();
    uint16_t current_sensor_cal_flag = Dev_CurrentSensor_Get_CalibrationStatus();
    uint16_t flag_word = enabled + ((status_flags & 0x0003)<<1) + (fault_flags<<3);
    
    buffer_sixteen_tx[1] = flag_word;
    buffer_sixteen_tx[2] = Dev_PwrCtrl_GetAdc_Vpri();
    buffer_sixteen_tx[3] = Dev_PwrCtrl_GetAdc_Vsec();
    buffer_sixteen_tx[4] = Dev_PwrCtrl_GetAdc_Ipri_ct();
    buffer_sixteen_tx[5] = Dev_PwrCtrl_GetAdc_Isec_ct();
    buffer_sixteen_tx[6] = Dev_PwrCtrl_GetAdc_Isec_avg();
    buffer_sixteen_tx[7] = Dev_PwrCtrl_GetAdc_Temperature();
    buffer_sixteen_tx[8] = Dev_PwrCtrl_GetAdc_Vrail_5V();    
    buffer_sixteen_tx[9] =  devFanDataPtr->CurrentSpeedRaw;
    buffer_sixteen_tx[10] = devFanDataPtr->CurrentSpeedPercent;
    buffer_sixteen_tx[11] = temperature;
    buffer_sixteen_tx[12] = Dev_PwrCtrl_Get_Period();
    buffer_sixteen_tx[13] = Dev_PwrCtrl_Get_PwmprdTarget();
    buffer_sixteen_tx[14] = Dev_PwrCtrl_Get_IReference();
    buffer_sixteen_tx[15] = Dev_PwrCtrl_Get_IloopReference();
    buffer_sixteen_tx[16] = Dev_PwrCtrl_Get_IloopOutput();
    buffer_sixteen_tx[17] = Dev_PwrCtrl_Get_IloopFeedback(); 
    buffer_sixteen_tx[18] = Dev_PwrCtrl_Get_SecPower(); 
    
    PBV_Change_from_Sixteen_to_Eight(buffer_sixteen_tx, buffer_eight_tx, 18);
    
    App_PBV_DAB_TX_Ptr->Data_Buffer = buffer_eight_tx;
    App_PBV_DAB_TX_Ptr->Length = 18 * 2 ;
}


/***********************************************************************************
 * @ingroup pbv-private-function
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
        case PBV_CMD_ID_DAB_ON_OFF: {
            // turn DAB on or off
            if (control_word < 2) {// should be 0 or 1
            
                bool enable = (bool)control_word;
                Dev_PwrCtrl_SetEnable(enable);
            }
            break; 
        }    
        case PBV_CMD_ID_DAB_RESET: {
            Dev_PwrCtrl_SetState(control_word);
            break; 
        }
        case PBV_CMD_ID_FREQ_CHANGE: {
            // change target frequency
            if ((control_word <= MAX_PWM_PERIOD) && (control_word >= MIN_PWM_PERIOD)) {
                Dev_PwrCtrl_SetPeriodTarget(control_word);
                // when Frequency is changed, control phase will be zero
                uint16_t controlPhase = 0;
                Dev_PwrCtrl_SetPhaseTarget(controlPhase);
            }            
            break; 
        }
        case PBV_CMD_ID_ILOOP_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                Dev_PwrCtrl_SetIReference(control_word);
            }
            break; 
        }        
        case PBV_CMD_ID_VLOOP_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                Dev_PwrCtrl_SetVSecReference(control_word);
            }
            break; 
        }  
        
        case PBV_CMD_ID_PLOOP_REF_SET: {
            if (control_word < 32767) //TODO: put in proper check here!
            {
                Dev_PwrCtrl_SetPwrReference(control_word);
            }
            break; 
        }  
        
        case PBV_CMD_ID_PHASE_CHANGE: {
            // change target phase
            uint16_t controlPhase = (uint16_t)((control_word)* PHASE_180_SCALER * (Dev_PwrCtrl_Get_DutyCycle()));
            Dev_PwrCtrl_SetPhaseTarget(controlPhase);      
            break; 
        }
        case PBV_CMD_ID_FAN_SPEED: {            
            Dev_Fan_Set_Speed(control_word);
            break; 
        }
        case PBV_CMD_ID_PRI_OVP_TEST: {
            Dev_Fault_SetPriOVPThreshold(control_word);
            break;
        }
        case PBV_CMD_ID_SEC_OVP_TEST: {
            Dev_Fault_SetSecOVPThreshold(control_word); 
            break; 
        }
        case PBV_CMD_ID_IPRI_TEST: {
            Dev_Fault_SetIPrimaryThreshold(control_word);
             break; 
        }
        case PBV_CMD_ID_ISEC_TEST:{
            Dev_Fault_SetISecondaryThreshold(control_word);    
            break;
        }
        case PBV_CMD_ID_PWM_DTH:{
            Nop();
            Nop();
            Nop();
//            Dev_PwrCtrl_SetDeadTimeHigh(control_word);
            break; 
        }
        case PBV_CMD_ID_PWM_DTL:{
//            Dev_PwrCtrl_SetDeadTimeLow(control_word);
            break; 
        }
                 
        default:
            break;
    }
}

/***********************************************************************************
 * @ingroup pbv-private-function
 * @param   void
 * @return  nothing
 * @brief   default callback
 * @details
 * @note
 **********************************************************************************/
void App_PBV_DAB_Frame_Parser(uint16_t protocol_ID, uint16_t length, uint8_t * data)
{
    PBV_Change_from_Eight_to_Sixteen(data, buffer_sixteen_rx, length);
    App_PBV_DAB_Process_Rx_Data(buffer_sixteen_rx);
}


/*******************************************************************************
 * end of file
 *******************************************************************************/