/* 
 * File:   dev_vin_isolated.h
 * Author: M15690
 *
 * Created on July 16, 2024, 4:13 PM
 */

#ifndef DEV_VIN_ISOLATED_H
#define	DEV_VIN_ISOLATED_H

/*********************************************************************************
 * @ingroup dev-vin-sccp-sys-spec-macros
 * @
 * @brief   defines the voltage limits, scaling factors of the system. 
 * @details 
 * SCCP capture counting at 100 Mhz. 
 * DC waveform being generated at 15.66 khz resulting in a period of 63.25ms.   
 * in 63.25ms, a counter running at 100MHz would count up to 63.25ms/0.01ms = 6325 
 * this would correspond to 100% DC.
 * 6325 corresponds to 1024 ADC value. that is multiplier of 6.325
 * now in the case of DAB input voltage = 0.002276 * Vin 
 * so RAW value divided by 6.325 gets us ADC sample. 
 * ADC sample * 4.096 / 1024 gets us voltage on pin.
 * so the multiplier converting RAW to Vin
 * Vin = (RAW/6.23) * 4.096 / (1024 * 0.002276)
 * Vin = 0.282 * RAW 
 **********************************************************************************/
#define OV_THRESHOLD_VOLT       900                                               ///< Application Specfic. TBD
#define OV_THRESHOLD_SCALING    0.282                                              ///< Application Specfic. TBD
#define OV_THRESHOLD_RAW        (OV_THRESHOLD_VOLT / OV_THRESHOLD_SCALING )             ///< example . Must be calcualted based on Scaling factor

#define OV_THRESHOLD_HYST 140               ///< example. Application Specific. Hystersis range. To be determined
#define OV_THRESHOLD_HYST_HALF (OV_THRESHOLD_HYST/2) ///<  

#define OV_UPPER_THRESHOLD_WITH_HYST  OV_THRESHOLD_RAW + OV_THRESHOLD_HYST_HALF
//#define OV_THRESHOLD_HYST 140    
#define OV_LOWER_THRESHOLD_WITH_HYST  OV_THRESHOLD_RAW - OV_THRESHOLD_HYST_HALF

#define UV_THRESHOLD_VOLT 500               ///< Application Specfic. TBD
#define UV_THRESHOLD_SCALING 0.282                ///< Application Specfic. TBD
#define UV_THRESHOLD_RAW  (UV_THRESHOLD_VOLT / UV_THRESHOLD_SCALING)              ///< example . Must be calcualted based on Scaling factor)

#define UV_THRESHOLD_HYST 140               ///< example. Application Specific. Hystersis range. To be determined
#define UV_THRESHOLD_HYST_HALF (OV_THRESHOLD_HYST/2)

#define UV_UPPER_THRESHOLD_WITH_HYST  UV_THRESHOLD_RAW + UV_THRESHOLD_HYST_HALF
//#define UV_THRESHOLD_HYST 140    
#define UV_LOWER_THRESHOLD_WITH_HYST  UV_THRESHOLD_RAW - UV_THRESHOLD_HYST_HALF

#define FAULT_PERSISTENCE_COUNT            4

extern void Dev_VinIsolated_Initialize(void);
extern void Dev_VinIsolated_Start(void);

#endif	/* DEV_VIN_ISOLATED_H */

