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

#include <xc.h>
#include "input_capture/sccp2.h"
#include "clc/clc2.h"
#include "dma/dma.h"


#include "pwrctrl/dev_pwrctrl_typedef.h"


/*********************************************************************************
 * @ingroup dev-temp-private-variables
 * @var     FAULT_OBJ_T Temp_Fault_Min
 * @brief   fault object for checking over temperature condition
 * @details min as negative temperature coefficient
 **********************************************************************************/
FAULT_OBJ_T vinFault;

extern POWER_CONTROL_t dab;

void Dev_VinIsolated_Initialize(void)
{
    DMA_SourceAddressSet(DMA_CHANNEL_2, (uint16_t)&CCP2BUFL);
    DMA_DestinationAddressSet(DMA_CHANNEL_2, (uint16_t)&dab.Adc.VInputVoltage);
    
//    FAULT_Init(
//            &vinFault,                                  ///< fault object
//            MAX_TEMPERATURE_THRESHOLD_RAW,               ///< threshold against which raw values will be compared
//            OVER_TEMP_UPPER_THRESHOLD_WITH_HYST,         ///< threshold plus hystersis
//            FAULT_PERSISTENCE_COUNT_TEMP,                    ///< number of ISR counts for which the fault should clear
//            FAULT_PERSISTENCE_COUNT_TEMP                     ///< number of ISR counts for which the fault should clear
//            ) ; 
    
}

void Dev_VinIsolated_Start(void)
{
    SCCP2_InputCapture_Start();
}