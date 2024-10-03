
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
    FLT_LOAD_DISCONNECT,
    RESET
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
            (dab.Fault.Object.VPrimaryUVP.FaultLatch << FLT_VPRI_UVP) +
            (dab.Fault.Object.VSecondaryOVP.FaultLatch << FLT_VSEC_OVP) +
            (dab.Fault.Object.VSecondaryUVP.FaultLatch << FLT_VSEC_UVP) +
            (dab.Fault.Object.PowerSupplyOTP.FaultLatch << FLT_DAB_TEMP) +
            (dab.Fault.Object.VRail_5V.FaultLatch << FLT_RAIL_5V) +
            (loadDisconnect << FLT_LOAD_DISCONNECT) + 
            (dab.Fault.Object.ISenseSCP.FaultLatch << RESET);
            
    return(fault);        
}
