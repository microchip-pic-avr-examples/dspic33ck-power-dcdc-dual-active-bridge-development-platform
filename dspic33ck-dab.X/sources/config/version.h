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
 *    - MAJOR    being one to two digits wide (without leading zeros)
 *               Major/Fundamental/Architectural Changes.  version 01.00.0000 official launch
 * 
 *    - MINOR    being two digits wide (with leading zeros)
 *               Feature Adds/Extensions
 * 
 *    - REVISION being four digits wide (with leading zeros)
 *               Bug fixes and improvements. many small iterations during dev. cycles.
 * 
 *  Version 1.2.23 will be encoded as 1.02.0023 and shown as integer value of 102023.
 *  The largest firmware version number supported is 99.99.9999.
 * 
 *  Firmware modules can use this integer encoded firmware version to conditional
 *  select code sections depending on the availability of firmware version-specific
 *  features or limitations.
 **********************************************************************************/
//#define FIRMWARE_VERSION (uint32_t) 0x00010002 ///< from right to left:  digits 5-n: Major,  digits 3-4: Minor,  digits 0-2: Revision, 
#define FIRMWARE_VERSION_MAJOR    (0x00)
#define FIRMWARE_VERSION_MINOR    (0x01)
#define FIRMWARE_VERSION_REVISION (0x0009)
#define FIRMWARE_VERSION (uint32_t)(FIRMWARE_VERSION_MAJOR << 24)+ (uint32_t)(FIRMWARE_VERSION_MINOR<<16) + (uint32_t)(FIRMWARE_VERSION_REVISION)
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
//#define FIRMWARE_VERSION_STRING "00.01.00" ///< Clear text version number.   //  manually TRACK a quick sheet of release/dev history in comments
//#define FIRMWARE_VERSION_STRING "00.01.0001" ///< 13.08.2024 ILoop, Vloop, closed
//#define FIRMWARE_VERSION_STRING "00.01.0002" ///< 14.08.2024 ILoop, Vloop, Ploop closed
//#define FIRMWARE_VERSION_STRING "00.01.0003" ///< independent Ramping IVP Ref values
//#define FIRMWARE_VERSION_STRING "00.01.0004" ///< 04.09.2024 deadtime adjust. Test LEB alternative implementation from Kristine
//#define FIRMWARE_VERSION_STRING "00.01.0005" ///< 04.09.2024 period modulation of PWM. Allows regulation with minimizing RMS current amplitude in DAB transformer
//#define FIRMWARE_VERSION_STRING "00.01.0006" ///< 05.09.2024 adaptive gain for I loop. Average V primary
//#define FIRMWARE_VERSION_STRING "00.01.0007" ///< 06.09.2024 offset calib I sense at each power control start. Hig Voltage Start Stop. Minimum pulse width adjustment
//#define FIRMWARE_VERSION_STRING "00.01.0008" ///< 11.09.2024 code cleanup, stop button go to Initialize state, separate ISR functions in isr_extension 
#define FIRMWARE_VERSION_STRING "00.01.0009" ///< 26.09.2024 folder structure readjusted. Testing a basic I V clamping on rising edge.
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

