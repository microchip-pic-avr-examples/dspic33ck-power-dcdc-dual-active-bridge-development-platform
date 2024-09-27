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
 * @file      fault_typedef.h
 * @ingroup   fault-common
 * @brief     Contains fault data types.
 */

#ifndef DEV_FAULT_TYPEDEF_H
#define	DEV_FAULT_TYPEDEF_H

#include "fault_common.h"


/*******************************************************************************
 * @ingroup fault-common
 * @brief Collection of faults to be monitored
 * @details This data structure in a collection of fault data objects that needs
 *  to be monitored by the fault tasks. 
 *******************************************************************************/
struct FAULT_s
{
    FAULT_OBJ_T IPrimaryOCP;        ///< Fault object for Primary over current protection
    FAULT_OBJ_T ISecondaryOCP;      ///< Fault object for Secondary over current protection
    FAULT_OBJ_T VPrimaryOVP;        ///< Fault object for Primary over voltage protection
    FAULT_OBJ_T VPrimaryUVP;        ///< Fault object for Primary under voltage protection
    FAULT_OBJ_T VSecondaryOVP;      ///< Fault object for Secondary over voltage protection
    FAULT_OBJ_T VSecondaryUVP;      ///< Fault object for Secondary under voltage protection
    FAULT_OBJ_T ISenseSCP;          ///< Fault object for Short circuit protection
    FAULT_OBJ_T PowerSupplyOTP;     ///< Fault object for Power Supply Temperature
    FAULT_OBJ_T VRail_5V;            ///< Fault object for 5V rail 
};
typedef struct FAULT_s FAULT_t;

/*******************************************************************************
 * @ingroup fault_common
 * @brief Contains Fault settings 
 * @details This data structure is utilized to to set-up the fault settings. 
 *******************************************************************************/
struct FAULT_SETTINGS_s
{
    uint16_t FaultDetected;     ///< Fault indication for fault occurrence
    FAULT_t Object;             ///< Fault Objects
};
typedef struct FAULT_SETTINGS_s FAULT_SETTINGS_t;


#endif	/* DEV_FAULT_TYPEDEF_H */

