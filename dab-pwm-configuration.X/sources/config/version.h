/* 
 * File:   version.h
 * Author: M15690
 *
 * Created on July 24, 2024, 10:38 AM
 */

#ifndef VERSION_H
#define	VERSION_H

/*********************************************************************************
 * @ingroup apps-layer-properties-public-defines
 * @brief   32-bit wide, Integer encoded firmware version number
 * @details
 *  This macro returns the most recent firmware version as integer encoded 
 *  value of format 
 * 
 *      [MAJOR][MINOR][REVISION]
 * 
 *  with 
 *    - MAJOR    being one to three digits wide (without leading zeros)
 *               Major/Fundamental/Architectural Changes
 * 
 *    - MINOR    being two digits wide (with leading zeros)
 *               Feature Adds/Extensions
 * 
 *    - REVISION being three digits wide (with leading zeros)
 *               Bug fixes and improvements
 * 
 *  Version 1.2.23 will be encoded as 1.02.023 and shown as integer value of 102023.
 *  The largest firmware version number supported is 999.99.999.
 * 
 *  Firmware modules can use this integer encoded firmware version to conditional
 *  select code sections depending on the availability of firmware version-specific
 *  features or limitations.
 **********************************************************************************/
#define FIRMWARE_VERSION (uint32_t) 001000u ///< from right to left: digits 0-2: Revision, digits 3-4: Minor, digits 5-n: Major

/*********************************************************************************
 * @ingroup apps-layer-properties-public-defines
 * @brief   Property screen string showing firmware version
 * @details
 *  This macro returns the most recent firmware version as clear-text string of
 *  fixed length of 10 characters of which the first three are reserved for MAJOR.
 *  followed by a separating dot, followed by two characters for MINOR, followed 
 *  by a separating dot and finally three characters reserved for the revision 
 *  number REV.
 *  This string may be used to be displayed in terminal windows or LC displays.
 **********************************************************************************/
#define FIRMWARE_VERSION_STRING "000.01.000" ///< Clear text version number

/*********************************************************************************
 * @ingroup apps-layer-properties-public-defines
 * @brief   Start screen string of second line showing firmware name
 * @details
 *  This macro returns the most recent firmware name as clear-text string of
 *  fixed length of 16 characters. This string is displayed on the LC display
 *  at power-up or after a CPU reset (splash-screen).
 **********************************************************************************/
#define FIRMWARE_NAME  "    DAB    "
#endif	/* VERSION_H */

