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
 * @file      fault_comm_interface.c
 * @ingroup   fault 
 * @brief     Contains API functions for fault protection.
 */

#include "fault_common.h"
#include "pwrctrl/pwrctrl.h"

enum FAULTLOG_FLAGS_e {
    FLT_VPRI_OVP,
    FLT_VPRI_UVP,
    FLT_VSEC_OVP,
    FLT_VSEC_UVP,
    FLT_IPRI_OCP,
    FLT_ISEC_OCP,
    FLT_ISNS_SCP,
    FLT_DAB_TEMP,
    FLT_RAIL_5V,
    FLT_LOAD_DISCONNECT
};
typedef enum FAULTLOG_FLAGS_e FAULTLOG_FLAGS_t;

extern bool loadDisconnect;

uint16_t Fault_GetFlags(void)
{
    uint16_t fault = 0;
    
    // Concatenate the Fault status in one for PBV 
    fault = (dab.Fault.Object.IPrimaryOCP.FaultLatch << FLT_IPRI_OCP) +
            (dab.Fault.Object.ISecondaryOCP.FaultLatch << FLT_ISEC_OCP) +
            (dab.Fault.Object.ISenseSCP.FaultLatch << FLT_ISNS_SCP) +
            (dab.Fault.Object.VPrimaryOVP.FaultLatch << FLT_VPRI_OVP) +
            (dab.Fault.Object.VSecondaryOVP.FaultLatch << FLT_VSEC_OVP) +
            (dab.Fault.Object.PowerSupplyOTP.FaultLatch << FLT_DAB_TEMP) +
            (dab.Fault.Object.VRail_5V.FaultLatch << FLT_RAIL_5V) +
            (loadDisconnect << FLT_LOAD_DISCONNECT);
            
    return(fault);        
}
