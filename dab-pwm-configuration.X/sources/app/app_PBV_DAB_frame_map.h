/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
/**
 * @file    app_PBV_cllc_frame_map.h
 * @brief   app PBV cllc Frame map file Example
 * @author  M70027
 * @date    15/02/2024
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APP_PBV_DAB_FRAME_MAP_H
#define	APP_PBV_DAB_FRAME_MAP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "app_PBV_interface.h"


/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/
void App_PBV_DAB_Init(void);
void App_PBV_DAB_Task_10ms(void);
void App_PBV_DAB_Task_1s(void);
PBV_Datatype_TX_t * App_PBV_DAB_Get_TX_Log_ptr(void);

#endif	/* APP_PBV_DAB_FRAME_MAP_H */

