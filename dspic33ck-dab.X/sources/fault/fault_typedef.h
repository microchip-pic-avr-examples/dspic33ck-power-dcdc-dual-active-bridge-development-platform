
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

