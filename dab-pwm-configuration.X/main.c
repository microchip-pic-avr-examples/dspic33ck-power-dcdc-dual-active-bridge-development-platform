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
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types

#include "system/system.h"

#include "x2cScope/X2CScope.h"
#include "app/app_PBV_DAB_frame_map.h"
#include "driver/mcc_extension/mcc_custom_config.h"
#include "os/os.h"

/*
    Main application
*/


/*******************************************************************************
 * @ingroup 
 * @brief  Application main function executed after device comes out of RESET
 * @details 
 * This function is the starting point of the firmware. It is called after the
 * device is coming out of RESET, starting to execute code. 
 * 
 *********************************************************************************/

/**
 * TP45_H/47_L - PWM1
 * TP42_H/40_L - PWM2 
 * TP37_L/41_H - PWM3 (output swap)
 * TP43_L/44_H - PWM4 (output swap)
 */
int main(void)
{
    SYSTEM_Initialize();
    
    MCC_Custom_User_Config();
    
    // X2CScope will be initialized when X2CDEBUG_ENABLED is enabled
    #if (X2CDEBUG_ENABLED == 1)
    X2CScope_Init();
    #endif
    
    
    OS_Init(); 
    
    App_PBV_DAB_Init();

    OS_Scheduler_RunForever();
    
    while(1)
    {
        
    }    
}