/* 
 * File:   dev_fault_typedef.h
 * Author: M15690
 *
 * Created on 25 July 2024, 14:39
 */

#ifndef DEV_FAULT_TYPEDEF_H
#define	DEV_FAULT_TYPEDEF_H

#include "dev_fault_common.h"


/***********************************************************************************
 * @ingroup dev-fault-type-definitions-data-types
 * @brief 
 * @details
 **********************************************************************************/
struct FAULT_s
{
    FAULT_OBJ_T IPrimaryOCP;        ///< Fault object for Primary over current protection
    FAULT_OBJ_T ISecondaryOCP;      ///< Fault object for Secondary over current protection
    FAULT_OBJ_T VPrimaryOVP;        ///< Fault object for Primary over voltage protection
    FAULT_OBJ_T VSecondaryOVP;      ///< Fault object for Secondary over voltage protection
    FAULT_OBJ_T ISenseSCP;          ///< Fault object for Short circuit protection
};
typedef struct FAULT_s FAULT_t;

/***********************************************************************************
 * @ingroup dev-fault-type-definitions-data-types
 * @brief 
 * @details
 **********************************************************************************/
struct FAULT_SETTINGS_s
{
    uint16_t FaultDetected; 
    FAULT_t Object;                     ///< Fault Objects
};
typedef struct FAULT_SETTINGS_s FAULT_SETTINGS_t;


#endif	/* DEV_FAULT_TYPEDEF_H */

