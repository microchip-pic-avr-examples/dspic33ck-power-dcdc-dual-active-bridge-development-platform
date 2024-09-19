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
 * @file    mcc_custom_config.h     
 * @ingroup driver-layer 
 * @brief   Contain the MCC Custom peripheral configuration that is not supported
 *  by MCC Melody tool. 
 */
#include <xc.h>
#include "driver/mcc_extension/drv_mcc_extension.h"
#include "driver/mcc_extension/drv_custom_config_pwm.h"
#include "driver/mcc_extension/drv_custom_config_adc.h"
#include "device/pwrctrl/dev_pwrctrl.h"

/*******************************************************************************
 * @ingroup driver-layer 
 * @brief  Contains custom peripheral initialization 
 * @return void
 * 
 * @details This function initializes peripheral settings that are needed in the
 *  application but is not supported by the generated code through MCC Melody tool. 
 * 
 *********************************************************************************/

void MCC_Custom_User_Config (void)
{
    // Initialize PWM as re-triggerable for the cascaded PWM scheme
    DRV_Config_Custom_PWM();
    
    // Initialize ADC with filtering in Vprimary and VSecondary
    DRV_Config_Custom_ADC();
}
