/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     dab_psio_dcdt.h
  Project Name:  dab_psio
  Date:          3/7/2024
  Time:          17:14.47

  Software License Agreement

  Copyright � 2024 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
#ifndef _DAB_PSIO_DCDT_DEFINES_H
#define _DAB_PSIO_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 5.0000e+03 Hz
  Pole 2                 5.0000e+03 Hz
  Zero 1                 9.0000e+03 Hz
  Gain(Kdc)              2.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    20000
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

                  3.14e+04   3.14e+04(5.65e+04 + s)
  H(s) = 2.000 X -------- X ----------------------
                     s       5.65e+04(3.14e+04 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.728     1.000         32764    0x7FFC
  a2      -0.728    -0.421        -13809   0xCA0F
  b0      0.193     0.112         3667     0x0E53
  b1      0.085     0.049         1616     0x0650
  b2      -0.108    -0.063        -2050    0xF7FE


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.193) + (0.085)z^(-1) + (-0.108)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.728)z^(-1) - (-0.728)z^(-2)

**/


// Compensator Coefficient Defines
#define DAB_PSIO_COMP_2P2Z_COEFF_A1      0x7FFC
#define DAB_PSIO_COMP_2P2Z_COEFF_A2      0xCA0F
#define DAB_PSIO_COMP_2P2Z_COEFF_B0      0x0E53
#define DAB_PSIO_COMP_2P2Z_COEFF_B1      0x0650
#define DAB_PSIO_COMP_2P2Z_COEFF_B2      0xF7FE
#define DAB_PSIO_COMP_2P2Z_POSTSCALER    0x6EA2
#define DAB_PSIO_COMP_2P2Z_POSTSHIFT     0xFFFF
#define DAB_PSIO_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define DAB_PSIO_COMP_2P2Z_MIN_CLAMP    0x0000
#define DAB_PSIO_COMP_2P2Z_MAX_CLAMP    0x4E20


#endif
