/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     dab_psvo_dcdt.h
  Project Name:  dab_psvo
  Date:          2/4/2025
  Time:          17:17.08

  Software License Agreement

  Copyright © 2025 Microchip Technology Inc.  All rights reserved.
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
#ifndef _DAB_PSVO_DCDT_DEFINES_H
#define _DAB_PSVO_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 8.0000e+01 Hz
  Pole 2                 4.0000e+03 Hz
  Zero 1                 1.2000e+01 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          1.0000e+04
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+04
  PWM Max Resolution     1.0000e-09
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    32767
  Kuc Gain               5.3585e+02
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    16.610
  Gain                  1.000e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  5.03e+02   2.51e+04(7.54e+01 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       7.54e+01(2.51e+04 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      0.886     0.238         7792     0x1E70
  a2      0.114     0.031         999      0x03E7
  b0      3.726     1.000         32764    0x7FFC
  b1      0.028     0.008         246      0x00F6
  b2      -3.698    -0.992        -32518   0x80FA


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (3.726) + (0.028)z^(-1) + (-3.698)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.886)z^(-1) - (0.114)z^(-2)

**/


// Compensator Coefficient Defines
#define DAB_PSVO_COMP_2P2Z_COEFF_A1      0x1E70
#define DAB_PSVO_COMP_2P2Z_COEFF_A2      0x03E7
#define DAB_PSVO_COMP_2P2Z_COEFF_B0      0x7FFC
#define DAB_PSVO_COMP_2P2Z_COEFF_B1      0x00F6
#define DAB_PSVO_COMP_2P2Z_COEFF_B2      0x80FA
#define DAB_PSVO_COMP_2P2Z_POSTSCALER    0x7741
#define DAB_PSVO_COMP_2P2Z_POSTSHIFT     0xFFFE
#define DAB_PSVO_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define DAB_PSVO_COMP_2P2Z_MIN_CLAMP    0x0000
#define DAB_PSVO_COMP_2P2Z_MAX_CLAMP    0x7FFF


#endif
