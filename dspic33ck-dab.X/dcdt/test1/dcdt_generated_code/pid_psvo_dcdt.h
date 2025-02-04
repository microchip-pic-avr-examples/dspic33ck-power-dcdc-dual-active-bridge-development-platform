/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     pid_psvo_dcdt.h
  Project Name:  test1
  Date:          2/4/2025
  Time:          16:26.04

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
#ifndef _PID_PSVO_DCDT_DEFINES_H
#define _PID_PSVO_DCDT_DEFINES_H

/**
  Compensator Type:  PID
      Entry                Value  
    ---------            ---------
  Kp                     1.0000e+01
  Ki                     0.0000e+00
  Kd                     2.0000e-02
  Kd                     2.0000e-02
  Gain(Kdc)              1
  PWM Frequency          1.0000e+04
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+04
  PWM Max Resolution     1.0600e-09
  Computational Delay    1.1000e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    32767
  Kuc Gain               3.0432e+02
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    16.526
  Gain                  1.060e-05


  s-domain transfer function

               (             Ki )
  H(s) = Kdc X (Kp + Kd(s) + ---)
               (              s )

                 (                         0.00e+00 )
  H(s) = 1.000 X (1.00e+01 + 2.00e-02(s) + ---------)
              (                                s    )



  Digital Compensator Coefficients

  Name    Value       Normalized    Q15      Hex
  ----    -----       ----------    ---      ---
  Ka      210.000     0.512         16781    0x418D
  Kb      -410.000    -1.000        -32764   0x8004
  Kc      200.000     0.488         15982    0x3E6E


  z-domain transfer function

         u(z)  KA + KBz^(-1) + KCz^(-2)
  H(z) = --- = ------------------------
         e(z)       1 - z^(-1)

          (210.000) + (-410.000)z^(-1) + (200.000)z^(-2)
  H(z) = ---------------------------------------------
                        1 - z^(-1)

**/


// Compensator Coefficient Defines
#define PID_PSVO_COMP_PID_COEFF_KA      0x418D
#define PID_PSVO_COMP_PID_COEFF_KB      0x8004
#define PID_PSVO_COMP_PID_COEFF_KC      0x3E6E
#define PID_PSVO_COMP_PID_POSTSCALER    0x6682
#define PID_PSVO_COMP_PID_POSTSHIFT     0xFFF7
#define PID_PSVO_COMP_PID_PRESHIFT      0x0000


// Compensator Clamp Limits
#define PID_PSVO_COMP_PID_MIN_CLAMP    0x0000
#define PID_PSVO_COMP_PID_MAX_CLAMP    0x7FFF


#endif
