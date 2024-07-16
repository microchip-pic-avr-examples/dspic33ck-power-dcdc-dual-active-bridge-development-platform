/**
 * MAIN Generated Driver Header File
 * 
 * @file      system.c
 *            
 * @ingroup   systemdriver
 *            
 * @brief     This is the generated driver header file for the System driver
 *            
 * @version   Driver Version 1.0.1
 *            
 * @skipline  Device : dsPIC33CK256MP506
*/
/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "../system.h"
#include "../system_types.h"
#include "../clock.h"
#include "../pins.h"
#include "../../adc/adc1.h"
#include "../../can/can1.h"
#include "../../clc/clc2.h"
#include "../../cmp/cmp1.h"
#include "../../cmp/cmp2.h"
#include "../../cmp/cmp3.h"
#include "../../crc/crc.h"
#include "../../dma/dma.h"
#include "../dmt.h"
#include "../../pwm_hs/pwm.h"
#include "../../timer/sccp1.h"
#include "../../input_capture/sccp2.h"
#include "../../pwm/sccp3.h"
#include "../../timer/tmr1.h"
#include "../../uart/uart1.h"
#include "../interrupt.h"


void SYSTEM_Initialize(void)
{
    CLOCK_Initialize();
    PINS_Initialize();
    ADC1_Initialize();
    CAN1_Initialize();
    CLC2_Initialize();
    CMP1_Initialize();
    CMP2_Initialize();
    CMP3_Initialize();
    CRC_Initialize();
    DMA_Initialize();
    DMT_Initialize();
    PWM_Initialize();
    SCCP1_Timer_Initialize();
    SCCP2_InputCapture_Initialize();
    SCCP3_PWM_Initialize();
    TMR1_Initialize();
    UART1_Initialize();
    INTERRUPT_GlobalEnable();
    INTERRUPT_Initialize();
}

/**
 End of File
*/