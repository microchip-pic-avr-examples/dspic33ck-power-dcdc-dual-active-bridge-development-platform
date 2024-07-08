/*
Copyright (c) [2012-2020] Microchip Technology Inc.  
    All rights reserved.
    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
*/

/**
 * This file collects all X2Cscope external application interfaces that can be used.
 */
#ifndef X2CSCOPE_H
#define X2CSCOPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
/* External X2Cscope API functions that must be used in the applications  */
void X2CScope_Init(void); // Init X2Cscope and hook communication interfaces.
/* Below functions are implemented in X2Cscope library */
void X2CScope_Communicate(void); // Communicate with X2Cscope GUI on PC side (Call in idle loop)
void X2CScope_Update(void); // Sample point of scope functionality (Call with fixed periods)

#ifdef __cplusplus
}
#endif

#endif
