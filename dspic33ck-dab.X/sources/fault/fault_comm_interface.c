
/**
 * @file      fault_comm_interface.c
 * @ingroup   fault 
 * @brief     Contains API functions for fault protection.
 */

#include "fault_common.h"
#include "pwrctrl/pwrctrl.h"

enum FAULTLOG_FLAGS_e {
    FLT_VPRI_OVP, //0
    FLT_VPRI_UVP, //1
    FLT_VSEC_OVP, //2
    FLT_VSEC_UVP, //3
    FLT_IPRI_OCP, //4
    FLT_ISEC_OCP, //5
    FLT_ISNS_SCP, //6
    FLT_DAB_TEMP, //7
    FLT_RAIL_5V,  //8
    FLT_LOAD_DISCONNECT, //9
    RESET //10
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
