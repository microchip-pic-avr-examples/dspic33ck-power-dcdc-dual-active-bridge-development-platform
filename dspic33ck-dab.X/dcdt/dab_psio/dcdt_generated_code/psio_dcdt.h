/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     psio_dcdt.h
  Project Name:  dab_psio
  Date:          10/27/2023
  Time:          13:56.41

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
#ifndef _PSIO_DCDT_DEFINES_H
#define _PSIO_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 2.0000e+01 Hz
  Pole 2                 1.5048e+04 Hz
  Zero 1                 3.5000e+02 Hz
  Gain(Kdc)              30.000
  Warp                   false
  PWM Frequency          6.5000e+04
  PWM Sampling Ratio     1
  Sampling Frequency     6.5000e+04
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    20000
  Kuc Gain               4.9591e+01
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    15.909
  Gain                  1.625e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  1.26e+02   9.45e+04(2.20e+03 + s)
  H(s) = 30.000 X -------- X ----------------------
                     s       2.20e+03(9.45e+04 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.158     1.000         32764    0x7FFC
  a2      -0.158    -0.136        -4467    0xEE8D
  b0      0.734     0.634         20771    0x5123
  b1      0.024     0.021         691      0x02B3
  b2      -0.710    -0.613        -20080   0xB190


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.734) + (0.024)z^(-1) + (-0.710)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.158)z^(-1) - (-0.158)z^(-2)

**/


// Compensator Coefficient Defines
#define PSIO_COMP_2P2Z_COEFF_A1      0x7FFC
#define PSIO_COMP_2P2Z_COEFF_A2      0xEE8D
#define PSIO_COMP_2P2Z_COEFF_B0      0x5123
#define PSIO_COMP_2P2Z_COEFF_B1      0x02B3
#define PSIO_COMP_2P2Z_COEFF_B2      0xB190
#define PSIO_COMP_2P2Z_POSTSCALER    0x4A1C
#define PSIO_COMP_2P2Z_POSTSHIFT     0xFFFF
#define PSIO_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define PSIO_COMP_2P2Z_MIN_CLAMP    0x0000
#define PSIO_COMP_2P2Z_MAX_CLAMP    0x4E20


#endif
