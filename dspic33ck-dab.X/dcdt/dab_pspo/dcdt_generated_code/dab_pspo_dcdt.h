/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     dab_pspo_dcdt.h
  Project Name:  dab_pspo
  Date:          1/28/2025
  Time:          16:19.06

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
#ifndef _DAB_PSPO_DCDT_DEFINES_H
#define _DAB_PSPO_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 4.0000e+01 Hz
  Pole 2                 4.9000e+03 Hz
  Zero 1                 4.0000e+01 Hz
  Gain(Kdc)              10.000
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

                  2.51e+02   3.08e+04(2.51e+02 + s)
  H(s) = 10.000 X -------- X ----------------------
                     s       2.51e+02(3.08e+04 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      0.788     0.128         4204     0x106C
  a2      0.212     0.035         1133     0x046D
  b0      6.138     1.000         32764    0x7FFC
  b1      0.152     0.025         813      0x032D
  b2      -5.986    -0.975        -31951   0x8331


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (6.138) + (0.152)z^(-1) + (-5.986)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.788)z^(-1) - (0.212)z^(-2)

**/


// Compensator Coefficient Defines
#define DAB_PSPO_COMP_2P2Z_COEFF_A1      0x106C
#define DAB_PSPO_COMP_2P2Z_COEFF_A2      0x046D
#define DAB_PSPO_COMP_2P2Z_COEFF_B0      0x7FFC
#define DAB_PSPO_COMP_2P2Z_COEFF_B1      0x032D
#define DAB_PSPO_COMP_2P2Z_COEFF_B2      0x8331
#define DAB_PSPO_COMP_2P2Z_POSTSCALER    0x6238
#define DAB_PSPO_COMP_2P2Z_POSTSHIFT     0xFFFD
#define DAB_PSPO_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define DAB_PSPO_COMP_2P2Z_MIN_CLAMP    0x0000
#define DAB_PSPO_COMP_2P2Z_MAX_CLAMP    0x7FFF


#endif
