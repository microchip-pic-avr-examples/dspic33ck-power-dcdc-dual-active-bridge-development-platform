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
struct FAULT_FLAGS_s
{
    union
    {
        struct
        {
            unsigned vPri_ov      : 1;  ///< bit  0
            unsigned vPri_uv      : 1;  ///< bit  1
            unsigned vSec_ov      : 1;  ///< bit  2
            unsigned vSec_uv      : 1;  ///< bit  3
            unsigned iPri_oc      : 1;  ///< bit  4
            unsigned iSec_oc      : 1;  ///< bit  5 
            unsigned i_sc         : 1;  ///< bit  6, primary or secondary short circuit fault
            unsigned notUsed      : 9;  ///< bits 7 to 15
        } __attribute__((packed)) bits; 
        uint16_t value;
    };
};
typedef struct FAULT_FLAGS_s FAULT_FLAGS_t;

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

struct FAULT_SETTINGS_s
{
    FAULT_t Object;                     ///< Fault Objects
//    FAULT_FLAGS_t Flags;                ///< Fault Flags
    FAULT_FLAGS_t FaultFlagsLatched;    ///< Latch faults in here so can be read by PBV GUI
};
typedef struct FAULT_SETTINGS_s FAULT_SETTINGS_t;


#endif	/* DEV_FAULT_TYPEDEF_H */

