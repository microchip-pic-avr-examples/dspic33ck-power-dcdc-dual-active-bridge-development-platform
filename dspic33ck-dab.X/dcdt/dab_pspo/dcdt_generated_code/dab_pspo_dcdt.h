/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     dab_pspo_dcdt.h
  Project Name:  dab_pspo
  Date:          11/26/2024
  Time:          12:28.22

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
#ifndef _DAB_PSPO_DCDT_DEFINES_H
#define _DAB_PSPO_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 1.5000e+02 Hz
  Pole 2                 1.5500e+03 Hz
  Zero 1                 8.0000e+01 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          1.0000e+04
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+04
  PWM Max Resolution     4.0000e-08
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-09
  Control Output Min.    0
  Control Output Max.    32767
  Kuc Gain               3.3466e+00
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    11.288
  Gain                  4.002e-04


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  9.42e+02   9.74e+03(5.03e+02 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       5.03e+02(9.74e+03 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.345     1.000         32764    0x7FFC
  a2      -0.345    -0.257        -8405    0xDF2B
  b0      0.629     0.468         15333    0x3BE5
  b1      0.031     0.023         751      0x02EF
  b2      -0.599    -0.445        -14581   0xC70B


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.629) + (0.031)z^(-1) + (-0.599)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.345)z^(-1) - (-0.345)z^(-2)

**/


// Compensator Coefficient Defines
#define DAB_PSPO_COMP_2P2Z_COEFF_A1      0x7FFC
#define DAB_PSPO_COMP_2P2Z_COEFF_A2      0xDF2B
#define DAB_PSPO_COMP_2P2Z_COEFF_B0      0x3BE5
#define DAB_PSPO_COMP_2P2Z_COEFF_B1      0x02EF
#define DAB_PSPO_COMP_2P2Z_COEFF_B2      0xC70B
#define DAB_PSPO_COMP_2P2Z_POSTSCALER    0x5617
#define DAB_PSPO_COMP_2P2Z_POSTSHIFT     0xFFFF
#define DAB_PSPO_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define DAB_PSPO_COMP_2P2Z_MIN_CLAMP    0x0000
#define DAB_PSPO_COMP_2P2Z_MAX_CLAMP    0x7FFF


#endif
