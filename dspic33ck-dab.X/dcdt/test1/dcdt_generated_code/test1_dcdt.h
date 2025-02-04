/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     test1_dcdt.h
  Project Name:  test1
  Date:          2/4/2025
  Time:          12:21.45

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
#ifndef _TEST1_DCDT_DEFINES_H
#define _TEST1_DCDT_DEFINES_H

/**
  Compensator Type:  PID
      Entry                Value  
    ---------            ---------
  Kp                     9.0000e-01
  Ki                     2.0000e+01
  Kd                     1.0000e-04
  Kd                     1.0000e-04
  Gain(Kdc)              1
  PWM Frequency          3.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     3.0000e+05
  PWM Max Resolution     1.0600e-09
  Computational Delay    1.1000e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    32767
  Kuc Gain               1.0144e+01
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    11.619
  Gain                  3.181e-04


  s-domain transfer function

               (             Ki )
  H(s) = Kdc X (Kp + Kd(s) + ---)
               (              s )

                 (                         2.00e+01 )
  H(s) = 1.000 X (9.00e-01 + 1.00e-04(s) + ---------)
              (                                s    )



  Digital Compensator Coefficients

  Name    Value      Normalized    Q15      Hex
  ----    -----      ----------    ---      ---
  Ka      30.900     0.507         16624    0x40F0
  Kb      -60.900    -1.000        -32764   0x8004
  Kc      30.000     0.493         16140    0x3F0C


  z-domain transfer function

         u(z)  KA + KBz^(-1) + KCz^(-2)
  H(z) = --- = ------------------------
         e(z)       1 - z^(-1)

          (30.900) + (-60.900)z^(-1) + (30.000)z^(-2)
  H(z) = ---------------------------------------------
                        1 - z^(-1)

**/


// Compensator Coefficient Defines
#define TEST1_COMP_PID_COEFF_KA      0x40F0
#define TEST1_COMP_PID_COEFF_KB      0x8004
#define TEST1_COMP_PID_COEFF_KC      0x3F0C
#define TEST1_COMP_PID_POSTSCALER    0x79CF
#define TEST1_COMP_PID_POSTSHIFT     0xFFFA
#define TEST1_COMP_PID_PRESHIFT      0x0000


// Compensator Clamp Limits
#define TEST1_COMP_PID_MIN_CLAMP    0x0000
#define TEST1_COMP_PID_MAX_CLAMP    0x7FFF


#endif
