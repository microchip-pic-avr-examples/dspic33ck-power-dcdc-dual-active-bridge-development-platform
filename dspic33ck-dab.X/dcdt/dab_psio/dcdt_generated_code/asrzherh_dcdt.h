/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     asrzherh_dcdt.h
  Project Name:  dab_psio
  Date:          10/24/2023
  Time:          16:55.08

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
#ifndef _ASRZHERH_DCDT_DEFINES_H
#define _ASRZHERH_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 3.5630e+03 Hz
  Pole 2                 4.1842e+04 Hz
  Zero 1                 2.5000e+02 Hz
  Gain(Kdc)              120.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
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
  Bits of Resolution    15.288
  Gain                  2.500e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  2.24e+04   2.63e+05(1.57e+03 + s)
  H(s) = 120.000 X -------- X ----------------------
                     s       1.57e+03(2.63e+05 + s)



  Digital Compensator Coefficients

  Name    Value       Normalized    Q15      Hex
  ----    -----       ----------    ---      ---
  a1      0.864       0.001         28       0x001C
  a2      0.136       0.000         4        0x0004
  b0      978.946     1.000         32764    0x7FFC
  b1      15.257      0.016         510      0x01FE
  b2      -963.689    -0.984        -32254   0x8202


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (978.946) + (15.257)z^(-1) + (-963.689)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.864)z^(-1) - (0.136)z^(-2)

**/


// Compensator Coefficient Defines
#define ASRZHERH_COMP_2P2Z_COEFF_A1      0x001C
#define ASRZHERH_COMP_2P2Z_COEFF_A2      0x0004
#define ASRZHERH_COMP_2P2Z_COEFF_B0      0x7FFC
#define ASRZHERH_COMP_2P2Z_COEFF_B1      0x01FE
#define ASRZHERH_COMP_2P2Z_COEFF_B2      0x8202
#define ASRZHERH_COMP_2P2Z_POSTSCALER    0x7A61
#define ASRZHERH_COMP_2P2Z_POSTSHIFT     0xFFF6
#define ASRZHERH_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define ASRZHERH_COMP_2P2Z_MIN_CLAMP    0x0000
#define ASRZHERH_COMP_2P2Z_MAX_CLAMP    0x7D00


#endif
