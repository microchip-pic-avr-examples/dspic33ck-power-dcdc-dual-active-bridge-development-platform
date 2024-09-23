/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     fm_dcdt.h
  Project Name:  dab_psio
  Date:          10/26/2023
  Time:          10:40.58

  Software License Agreement

  Copyright © 2023 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
  OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
  PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
  OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
  BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
  DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
  INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
  CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
  OR OTHER SIMILAR COSTS.
 **/
#ifndef _FM_DCDT_DEFINES_H
#define _FM_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 1.0000e+00 Hz
  Pole 2                 2.5000e+03 Hz
  Zero 1                 2.5000e+01 Hz
  Gain(Kdc)              1000.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     10
  Sampling Frequency     1.0000e+04
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    32000
  Kuc Gain               1.9550e+02
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    18.610
  Gain                  2.500e-06


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  6.28e+00   1.57e+04(1.57e+02 + s)
  H(s) = 1000.000 X -------- X ----------------------
                     s       1.57e+02(1.57e+04 + s)



  Digital Compensator Coefficients

  Name    Value      Normalized    Q15      Hex
  ----    -----      ----------    ---      ---
  a1      1.120      0.063         2069     0x0815
  a2      -0.120     -0.007        -222     0xFF22
  b0      17.734     1.000         32764    0x7FFC
  b1      0.276      0.016         510      0x01FE
  b2      -17.458    -0.984        -32254   0x8202


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (17.734) + (0.276)z^(-1) + (-17.458)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.120)z^(-1) - (-0.120)z^(-2)

**/


//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1     0x4135//0x1B84//0x3A96//0x452F//0x77F5//0x7FFC//0x0815
//#define DAB_FM_COMP_2P2Z_COEFF_A2     0x0CDF//0x0542//0x02EA//0x0371//0xF321//0xF245//0xFF22
//#define DAB_FM_COMP_2P2Z_COEFF_B0     0x7FFC//0x7FFC//0x7FFC//0x7FFC//0x7FFC//0x510C//0x7FFC
//#define DAB_FM_COMP_2P2Z_COEFF_B1     0x1C93//0x0263//0x04BC//0x0263//0x04BC//0x0143//0x01FE
//#define DAB_FM_COMP_2P2Z_COEFF_B2     0x9C97//0x8268//0x84C0//0x8268//0x84C0//0xB038//0x8202
//#define DAB_FM_COMP_2P2Z_POSTSCALER   0x68E9//0x7CF6//0x4298//0x70C9//0x4C7F//0x47B3//0x46F1
//#define DAB_FM_COMP_2P2Z_POSTSHIFT    0xFFFF//0xFFFE//0xFFFE//0xFFFF//0xFFFF//0xFFFF//0xFFFB
//#define DAB_FM_COMP_2P2Z_PRESHIFT     0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000

//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1     0x7FFC//
//#define DAB_FM_COMP_2P2Z_COEFF_A2     0xC33A//
//#define DAB_FM_COMP_2P2Z_COEFF_B0     0x0872//
//#define DAB_FM_COMP_2P2Z_COEFF_B1     0x00CE//
//#define DAB_FM_COMP_2P2Z_COEFF_B2     0xF85D//
//#define DAB_FM_COMP_2P2Z_POSTSCALER   0x79E2//
//#define DAB_FM_COMP_2P2Z_POSTSHIFT    0xFFFF//
//#define DAB_FM_COMP_2P2Z_PRESHIFT     0x0000//
//
//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1     0x23C1//0x11A6//0x7FFC//0x7FFC//0x7FFC//0x1F84//0x7FFC//0x1B84//0x3A96//0x452F//0x77F5//0x7FFC//0x0815
//#define DAB_FM_COMP_2P2Z_COEFF_A2     0xEECC//0xF770//0xC3C8//0xC19E//0xC33A//0xFD0A//0xE8CA//0x0542//0x02EA//0x0371//0xF321//0xF245//0xFF22
//#define DAB_FM_COMP_2P2Z_COEFF_B0     0x7FFC//0x7FFC//0x23DF//0x0926//0x0872//0x7FFC//0x541E//0x7FFC//0x7FFC//0x7FFC//0x7FFC//0x510C//0x7FFC
//#define DAB_FM_COMP_2P2Z_COEFF_B1     0x094C//0x006E//0x03CA//0x0072//0x00CE//0x0004//0x051F//0x0263//0x04BC//0x0263//0x04BC//0x0143//0x01FE
//#define DAB_FM_COMP_2P2Z_COEFF_B2     0x8950//0x8073//0xDFEB//0xF74D//0xF85D//0x8008//0xB101//0x8268//0x84C0//0x8268//0x84C0//0xB038//0x8202
//#define DAB_FM_COMP_2P2Z_POSTSCALER   0x6E64//0x70B7//0x78E3//0x7CDF//0x79E2//0x47B7//0x4E30//0x7CF6//0x4298//0x70C9//0x4C7F//0x47B3//0x46F1
//#define DAB_FM_COMP_2P2Z_POSTSHIFT    0xFFFD//0xFFFC//0xFFFF//0xFFFF//0xFFFF//0xFFFD//0xFFFF//0xFFFE//0xFFFE//0xFFFF//0xFFFF//0xFFFF//0xFFFB
//#define DAB_FM_COMP_2P2Z_PRESHIFT     0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000


//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1     0x21F2//0x23C1//0x11A6//0x7FFC//0x7FFC//0x7FFC//0x1F84//0x7FFC//0x1B84//0x3A96//0x452F//0x77F5//0x7FFC//0x0815
//#define DAB_FM_COMP_2P2Z_COEFF_A2     0xEF59//0xEECC//0xF770//0xC3C8//0xC19E//0xC33A//0xFD0A//0xE8CA//0x0542//0x02EA//0x0371//0xF321//0xF245//0xFF22
//#define DAB_FM_COMP_2P2Z_COEFF_B0     0x7FFC//0x7FFC//0x7FFC//0x23DF//0x0926//0x0872//0x7FFC//0x541E//0x7FFC//0x7FFC//0x7FFC//0x7FFC//0x510C//0x7FFC
//#define DAB_FM_COMP_2P2Z_COEFF_B1     0x0006//0x094C//0x006E//0x03CA//0x0072//0x00CE//0x0004//0x051F//0x0263//0x04BC//0x0263//0x04BC//0x0143//0x01FE
//#define DAB_FM_COMP_2P2Z_COEFF_B2     0x800A//0x8950//0x8073//0xDFEB//0xF74D//0xF85D//0x8008//0xB101//0x8268//0x84C0//0x8268//0x84C0//0xB038//0x8202
//#define DAB_FM_COMP_2P2Z_POSTSCALER   0x766D//0x6E64//0x70B7//0x78E3//0x7CDF//0x79E2//0x47B7//0x4E30//0x7CF6//0x4298//0x70C9//0x4C7F//0x47B3//0x46F1
//#define DAB_FM_COMP_2P2Z_POSTSHIFT    0xFFFD//0xFFFD//0xFFFC//0xFFFF//0xFFFF//0xFFFF//0xFFFD//0xFFFF//0xFFFE//0xFFFE//0xFFFF//0xFFFF//0xFFFF//0xFFFB
//#define DAB_FM_COMP_2P2Z_PRESHIFT     0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000//0x0000

//#define DAB_FM_COMP_2P2Z_COEFF_A1    0x0B11 
//#define DAB_FM_COMP_2P2Z_COEFF_A2    0xFE4B 
//#define DAB_FM_COMP_2P2Z_COEFF_B0    0x7FFC 
//#define DAB_FM_COMP_2P2Z_COEFF_B1    0x00F6 
//#define DAB_FM_COMP_2P2Z_COEFF_B2    0x80FA 
//#define DAB_FM_COMP_2P2Z_POSTSCALER  0x6D67 
//#define DAB_FM_COMP_2P2Z_POSTSHIFT   0xFFFC 
//#define DAB_FM_COMP_2P2Z_PRESHIFT    0x0000 

//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1   0x0FD0
//#define DAB_FM_COMP_2P2Z_COEFF_A2   0xFD8F
//#define DAB_FM_COMP_2P2Z_COEFF_B0   0x7FFC
//#define DAB_FM_COMP_2P2Z_COEFF_B1   0x00F6
//#define DAB_FM_COMP_2P2Z_COEFF_B2   0x80FA
//#define DAB_FM_COMP_2P2Z_POSTSCALER 0x4C95
//#define DAB_FM_COMP_2P2Z_POSTSHIFT  0xFFFC
//#define DAB_FM_COMP_2P2Z_PRESHIFT   0x0000











//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1   0x7FFC
//#define DAB_FM_COMP_2P2Z_COEFF_A2   0xEE8D
//#define DAB_FM_COMP_2P2Z_COEFF_B0   0x5123
//#define DAB_FM_COMP_2P2Z_COEFF_B1   0x02B3
//#define DAB_FM_COMP_2P2Z_COEFF_B2   0xB190
//#define DAB_FM_COMP_2P2Z_POSTSCALER 0x4A1C
//#define DAB_FM_COMP_2P2Z_POSTSHIFT  0xFFFF
//#define DAB_FM_COMP_2P2Z_PRESHIFT   0x0000


#define DAB_FM_COMP_2P2Z_COEFF_A1   0x4133
#define DAB_FM_COMP_2P2Z_COEFF_A2   0xF5EE
#define DAB_FM_COMP_2P2Z_COEFF_B0   0x7FFC
#define DAB_FM_COMP_2P2Z_COEFF_B1   0x05A9
#define DAB_FM_COMP_2P2Z_COEFF_B2   0x85AD
#define DAB_FM_COMP_2P2Z_POSTSCALER 0x4A4C
#define DAB_FM_COMP_2P2Z_POSTSHIFT  0xFFFE
#define DAB_FM_COMP_2P2Z_PRESHIFT   0x0000



//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1    0x3758 
//#define DAB_FM_COMP_2P2Z_COEFF_A2    0xF775 
//#define DAB_FM_COMP_2P2Z_COEFF_B0    0x7FFC 
//#define DAB_FM_COMP_2P2Z_COEFF_B1    0x00F6 
//#define DAB_FM_COMP_2P2Z_COEFF_B2    0x80FA 
//#define DAB_FM_COMP_2P2Z_POSTSCALER  0x5786 
//#define DAB_FM_COMP_2P2Z_POSTSHIFT   0xFFFE 
//#define DAB_FM_COMP_2P2Z_PRESHIFT    0x0000 




//// Compensator Coefficient Defines
//#define DAB_FM_COMP_2P2Z_COEFF_A1      0x0815
//#define DAB_FM_COMP_2P2Z_COEFF_A2      0xFF22
//#define DAB_FM_COMP_2P2Z_COEFF_B0      0x7FFC
//#define DAB_FM_COMP_2P2Z_COEFF_B1      0x01FE
//#define DAB_FM_COMP_2P2Z_COEFF_B2      0x8202
//#define DAB_FM_COMP_2P2Z_POSTSCALER    0x46F1
//#define DAB_FM_COMP_2P2Z_POSTSHIFT     0xFFFB
//#define DAB_FM_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define DAB_FM_COMP_2P2Z_MIN_CLAMP    0x0000
#define DAB_FM_COMP_2P2Z_MAX_CLAMP    0x7FFF//0xC700//0x7FFF //0xFFFB


#endif
