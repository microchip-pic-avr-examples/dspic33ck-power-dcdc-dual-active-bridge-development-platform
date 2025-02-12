
/* 
 * @file    PBV_config.h   
 * @author  M70027
 * @ingroup    app-pbv-config
 * @brief   Power Board Visualizer application configuration 
 */
#ifndef APP_PBV_CONFIG_H_
#define	APP_PBV_CONFIG_H_

#include "PBV_interface.h"

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for Init
 * @name    PBV_Intf_Init_t
 * @param   PBV_Datatype_TX_t *
 * @param   PBV_Datatype_TX_t *
 * @param   PBV_Datatype_RX_t *  
 * @details function pointer typedef for init. in app_PBV_config.c will be linked to UART init or CAN Init
 *********************************************************************************/
typedef void (*PBV_Intf_Init_t)(PBV_Datatype_TX_t * boardToPBV, 
                                PBV_Datatype_TX_t * boardToPBVAscii, 
                                PBV_Datatype_RX_t *App_PBV_To_System_Ptr);

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for app pbv receiving function
 * @name    PBV_Intf_Receive_from_GUI_t 
 * @details function pointer typedef for app pbv receiving function. in app_PBV_config.c will be linked to UART RX or CAN RX
 *********************************************************************************/
typedef uint8_t (*PBV_Intf_Receive_from_GUI_t)(void);

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for app pbv transmit function
 * @name    PBV_Intf_Transmit_to_GUI_t 
 * @details function pointer typedef for app pbv transmit function. in app_PBV_config.c will be linked to UART TX or CAN TX
 *********************************************************************************/
typedef uint8_t (*PBV_Intf_Transmit_to_GUI_t)(void);

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for app pbv transmit ascii function
 * @name    PBV_Intf_Transmit_Ascii_to_GUI_t 
 * @details function pointer typedef for app pbv transmit function. in app_PBV_config.c will be linked to UART ascii TX or CAN ascii TX
 *********************************************************************************/
typedef uint8_t (*PBV_Intf_Transmit_Ascii_to_GUI_t)(void);

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for app pbv re init function
 * @name    PBV_Intf_Reinit_t 
 * @param   PBV_Datatype_TX_t * 
 * @details 
 *  function pointer typedef for app pbv reinit function. in app_PBV_config.c will be linked to UART re init or CAN re init.
 *  this function is used to re init can or uart objects 
 *********************************************************************************/
typedef void (*PBV_Intf_Reinit_t)(PBV_Datatype_TX_t * ptr);

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for app pbv link data TX function
 * @name    PBV_Intf_Reinit_t 
 * @param   PBV_Datatype_TX_t * 
 * @details 
 *  function pointer typedef for app pbv link data function. in app_PBV_config.c will be linked to UART link or CAN link.
 *  this function is used to link data and other parameters to can or uart objects from calling application
 *********************************************************************************/
typedef void (*PBV_Intf_Link_Data_TX_t)(PBV_Datatype_TX_t * ptr);

/*********************************************************************************
 * @ingroup app-pbv-config
 * @brief   function pointer typedef for app pbv link data RX function
 * @name    PBV_Intf_Reinit_t 
 * @param   PBV_Datatype_RX_t * 
 * @details 
 *  function pointer typedef for app pbv link data function. in app_PBV_config.c will be linked to UART link or CAN link.
 *  this function is used to link data and other parameters from can or uart objects to calling application
 *********************************************************************************/
typedef int (*PBV_Intf_Link_Data_RX_t)(PBV_Datatype_RX_t * ptr);

/***********************************************************************************
 * @ingroup app-pbv-config
 * @struct  APP_PBV_INTF_API_s
 * @brief   structure of API function pointers. 
 **********************************************************************************/
struct APP_PBV_INTF_API_s 
{    
    PBV_Intf_Init_t init;                   
    PBV_Intf_Receive_from_GUI_t rxGui;
    PBV_Intf_Transmit_to_GUI_t txGui;
    PBV_Intf_Transmit_Ascii_to_GUI_t txGuiAscii;
    PBV_Intf_Reinit_t reinit;
    PBV_Intf_Link_Data_TX_t linkDataTX;
    PBV_Intf_Link_Data_RX_t linkDataRX;
};

typedef struct APP_PBV_INTF_API_s APP_PBV_INTF_API_t;

#endif	/*  APP_PBV_INTERFACE_H */
