/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     dab_spio_dcdt.h
  Project Name:  dab_spio
  Date:          1/16/2025
  Time:          20:45.53

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
#ifndef _DAB_SPIO_DCDT_DEFINES_H
#define _DAB_SPIO_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 5.0000e+03 Hz
  Pole 2                 5.0000e+03 Hz
  Zero 1                 9.0000e+03 Hz
  Gain(Kdc)              4.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    32767
  Kuc Gain               5.3546e+01
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
  H(s) = 4.000 X -------- X ----------------------
                     s       5.65e+04(3.14e+04 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.728     1.000         32764    0x7FFC
  a2      -0.728    -0.421        -13809   0xCA0F
  b0      0.387     0.224         7335     0x1CA7
  b1      0.171     0.099         3233     0x0CA1
  b2      -0.216    -0.125        -4101    0xEFFB


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.387) + (0.171)z^(-1) + (-0.216)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.728)z^(-1) - (-0.728)z^(-2)

**/


// Compensator Coefficient Defines
#define DAB_SPIO_COMP_2P2Z_COEFF_A1      0x7FFC
#define DAB_SPIO_COMP_2P2Z_COEFF_A2      0xCA0F
#define DAB_SPIO_COMP_2P2Z_COEFF_B0      0x1CA7
#define DAB_SPIO_COMP_2P2Z_COEFF_B1      0x0CA1
#define DAB_SPIO_COMP_2P2Z_COEFF_B2      0xEFFB
#define DAB_SPIO_COMP_2P2Z_POSTSCALER    0x6EA2
#define DAB_SPIO_COMP_2P2Z_POSTSHIFT     0xFFFF
#define DAB_SPIO_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define DAB_SPIO_COMP_2P2Z_MIN_CLAMP    0x0000
#define DAB_SPIO_COMP_2P2Z_MAX_CLAMP    0x7FFF


#endif
