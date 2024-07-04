/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */
#ifndef DRV_MCC_EXTENSION_PWM_H
#define	DRV_MCC_EXTENSION_PWM_H

#include <xc.h>

#if defined (PWM_HS_TYPES_H)

#include "pwm_hs/pwm_hs_types.h"
#define PWM_COUNT  (PWM_MAX_GENERATOR - 1)

typedef enum PWM_GENERATOR PWM_GENERATOR_INSTANCE_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     PWM_PCI_FaultAcceptanceCriteria(uint16_t pwmIndex, PWM_ACP_t acpSetting)
 * @brief   set PCI fault acceptance criteria
 * @details
 **********************************************************************************/
enum PWM_PCI_ACCEPTANCE_CRITERIA_e
{
    PWM_PCI_ACCEPT_LEVEL_SENSITIVE = 0,
    PWM_PCI_ACCEPT_RISING_EDGE = 1,
    PWM_PCI_ACCEPT_ANY_EDGE = 2,
    PWM_PCI_ACCEPT_LATCHED = 3,
    PWM_PCI_ACCEPT_LATCHED_RISING_EDGE = 4,
    PWM_PCI_ACCEPT_LATCHED_ANY_EDGE = 5
};

typedef enum PWM_PCI_ACCEPTANCE_CRITERIA_e PWM_PCI_ACCEPTANCE_CRITERIA_t;


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     PWM_PCI_FaultAcceptanceCriteria(uint16_t pwmIndex, PWM_ACP_t acpSetting)
 * @brief   set PCI fault acceptance criteria
 * @details
 **********************************************************************************/
inline static void PWM_PCI_Fault_AcceptanceCriteria(PWM_GENERATOR_INSTANCE_t pwmIndex, 
        PWM_PCI_ACCEPTANCE_CRITERIA_t acpSetting)
{
   
    switch (pwmIndex)
    {
        case 1:
            PG1FPCIHbits.ACP = acpSetting;
            break;
        case 2:
            PG2FPCIHbits.ACP = acpSetting;
            break;  
        case 3:
            PG3FPCIHbits.ACP = acpSetting;
            break;          
        case 4:
            PG4FPCIHbits.ACP = acpSetting;
            break;    
    #if (PWM_COUNT == 8)
        case 5:
            PG5FPCIHbits.ACP = acpSetting;
            break;          
        case 6:
            PG6FPCIHbits.ACP = acpSetting;
            break;         
        case 7:
            PG7FPCIHbits.ACP = acpSetting;
            break;  
        case 8:
            PG8FPCIHbits.ACP = acpSetting;
            break;  
    #endif         
        default:
            break;      
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     PWM_PCI_Fault_AcceptanceQualiferInvert(uint16_t pwmIndex, bool invert)
 * @brief   function to set polarity of PCI fault acceptance input
 * @details
 **********************************************************************************/
inline static void PWM_PCI_Fault_AcceptanceQualiferInvert(PWM_GENERATOR_INSTANCE_t pwmIndex, bool invert)
{
    switch (pwmIndex)
    {
        case 1:
            PG1FPCILbits.AQPS = (uint16_t)invert;
            break;
        case 2:
            PG2FPCILbits.AQPS = (uint16_t)invert;
            break;
        case 3:
            PG3FPCILbits.AQPS = (uint16_t)invert;
            break;
        case 4:
            PG4FPCILbits.AQPS = (uint16_t)invert;
            break;
    #if (PWM_COUNT == 8)
        case 5:
            PG5FPCILbits.AQPS = (uint16_t)invert;
            break;
        case 6:
            PG6FPCILbits.AQPS = (uint16_t)invert;
            break;
        case 7:
            PG7FPCILbits.AQPS = (uint16_t)invert;
            break;
        case 8:
            PG8FPCILbits.AQPS = (uint16_t)invert;
            break;     
    #endif        
        default:
            break;
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def     PWM_PCI_Sync_AcceptanceCriteria(uint16_t pwmIndex, PWM_PCI_ACCEPTANCE_CRITERIA_t acpSetting)
 * @brief   Set acceptance criteria for SYNC PWM
 * @details
 **********************************************************************************/
inline static void PWM_PCI_Sync_AcceptanceCriteria(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_ACCEPTANCE_CRITERIA_t acpSetting)
{
    switch (pwmIndex)
    {
        case 1:
            PG1SPCIHbits.ACP = acpSetting;
            break;
        case 2:
            PG2SPCIHbits.ACP = acpSetting;
            break;  
        case 3:
            PG3SPCIHbits.ACP = acpSetting;
            break;          
        case 4:
            PG4SPCIHbits.ACP = acpSetting;
            break;    
    #if (PWM_COUNT == 8)        
        case 5:
            PG5SPCIHbits.ACP = acpSetting;
            break;    
        case 6:
            PG6SPCIHbits.ACP = acpSetting;
            break;         
        case 7:
            PG7SPCIHbits.ACP = acpSetting;
            break;  
        case 8:
            PG8SPCIHbits.ACP = acpSetting;
            break;  
    #endif        
        default:
            break;
            
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_TERMTIME_AFTER_EVENT_e 
 * @brief   settings for TSYNCDIS bit
 * @details
 *  
 **********************************************************************************/
enum PWM_PCI_TERMTIME_AFTER_EVENT_e
{
    PWM_PCI_TERMTIME_AFTER_EVENT_AT_EOC = 0,
    PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE = 1
};
typedef enum PWM_PCI_TERMTIME_AFTER_EVENT_e PWM_PCI_TERMTIME_AFTER_EVENT_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Fault_TerminateAfterEvent 
 * @brief   set TSYNCDIS bit to determine when PWM should stop after a fault occurs
 * @details
 *  
 **********************************************************************************/
inline static void PWM_PCI_Fault_TimetoTerminateAfterEvent(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_TERMTIME_AFTER_EVENT_t faultTerm)
{
    switch (pwmIndex)
    {
        case 1:
            PG1FPCILbits.TSYNCDIS = faultTerm;
            break;
        case 2:
            PG2FPCILbits.TSYNCDIS = faultTerm;
            break;  
        case 3:
            PG3FPCILbits.TSYNCDIS = faultTerm;
            break;          
        case 4:
            PG4FPCILbits.TSYNCDIS = faultTerm;
            break;    
    #if (PWM_COUNT == 8)    
        case 5:
            PG5FPCILbits.TSYNCDIS = faultTerm;
            break;    
        case 6:
            PG6FPCILbits.TSYNCDIS = faultTerm;
            break;         
        case 7:
            PG7FPCILbits.TSYNCDIS = faultTerm;
            break;  
        case 8:
            PG8FPCILbits.TSYNCDIS = faultTerm;
            break;  
    #endif        
        default:
            break;
            
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Sync_TerminateAfterEvent 
 * @brief   set TSYNCDIS bit to determine when PWM should stop after a sync event occurs
 * @details
 *  
 **********************************************************************************/
inline static void PWM_PCI_Sync_TimetoTerminateAfterEvent(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_TERMTIME_AFTER_EVENT_t faultTerm)
{
    switch (pwmIndex)
    {
        case 1:
            PG1SPCILbits.TSYNCDIS = faultTerm;
            break;
        case 2:
            PG2SPCILbits.TSYNCDIS = faultTerm;
            break;  
        case 3:
            PG3SPCILbits.TSYNCDIS = faultTerm;
            break;          
        case 4:
            PG4SPCILbits.TSYNCDIS = faultTerm;
            break;    
    #if (PWM_COUNT == 8)          
        case 5:
            PG5SPCILbits.TSYNCDIS = faultTerm;
            break;    
        case 6:
            PG6SPCILbits.TSYNCDIS = faultTerm;
            break;         
        case 7:
            PG7SPCILbits.TSYNCDIS = faultTerm;
            break;  
        case 8:
            PG8SPCILbits.TSYNCDIS = faultTerm;
            break;  
    #endif        
        default:
            break;
            
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_SOCS_e 
 * @brief   settings for SOCS field
 * @details
 *  
 **********************************************************************************/
enum PWM_SOCS_e
{
    PWM_SOCS_SELF_TRIGGER = 0,
    PWM_SOCS_PG1_OR_PG5 = 1,
    PWM_SOCS_PG2_OR_PG6 = 2,
    PWM_SOCS_PG3_OR_PG7 = 3,
    PWM_SOCS_PG4_OR_PG8 = 4,
    PWM_SOCS_TRIG_OR_PCI_SYNC = 15
};
typedef enum PWM_SOCS_e PWM_SOCS_t;


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_StartOfCycleTrigger 
 * @brief   set SOCS field to determine start of cycle trigger
 * @details
 *  
 **********************************************************************************/
inline static void PWM_StartOfCycleTrigger(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_SOCS_t triggerSource)
{
    switch (pwmIndex)
    {
        case 1:
            PG1CONHbits.SOCS = triggerSource;
            break;
        case 2:
            PG2CONHbits.SOCS = triggerSource;
            break;
        case 3:
            PG3CONHbits.SOCS = triggerSource;
            break;
        case 4:
            PG4CONHbits.SOCS = triggerSource;
            break;
    #if (PWM_COUNT == 8)          
        case 5:
            PG5CONHbits.SOCS = triggerSource;
            break;
        case 6:
            PG6CONHbits.SOCS = triggerSource;
            break;
        case 7:
            PG7CONHbits.SOCS = triggerSource;
            break;
        case 8:
            PG8CONHbits.SOCS = triggerSource;
            break;
    #endif        
        default:
            break;
    }
}       

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_StartOfCycleTrigger 
 * @brief  Enable broadcasting of UPDREQ bit to other PWMs 
 * @details
 * this is used for register update synchronization between PWMs
 * Setting the MSTEN bit means that the UPDREQ flag is broadcast to other PWMs
 * if the UPDMOD field in the receiving PWM is set to "Client SOC" or 
 * "client immediate" then this will
 * take effect and data update between different PWM modules can be implemented
 **********************************************************************************/
inline static void PWM_UPDREQ_Broadcast_Enable(PWM_GENERATOR_INSTANCE_t pwmIndex, bool enable)
{
    uint16_t enable_ = (uint16_t)enable;
    PG1CONHbits.MSTEN = enable_;
    
    switch (pwmIndex)
    {
        case 1:
            PG1CONHbits.MSTEN = enable_;
            break;
        case 2:
            PG2CONHbits.MSTEN = enable_;
            break;
        case 3:
            PG3CONHbits.MSTEN = enable_;
            break;
        case 4:
            PG4CONHbits.MSTEN = enable_;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5CONHbits.MSTEN = enable_;
            break;
        case 6:
            PG6CONHbits.MSTEN = enable_;
            break;
        case 7:
            PG7CONHbits.MSTEN = enable_;
            break;
        case 8:
            PG8CONHbits.MSTEN = enable_;
            break;
    #endif        
        default:
            break;
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_UPDMOD_e 
 * @brief  settings for UPDMOD field
 * @details
 *  
 **********************************************************************************/
enum PWM_UPDMOD_e
{
    PWM_UPDMOD_SOC = 0,
    PWM_UPDMOD_IMMEDIATE = 1,
    PWM_UPDMOD_CLIENT_SOC = 2,
    PWM_UPDMOD_CLIENT_IMMEDIATE = 3
};
typedef enum PWM_UPDMOD_e PWM_UPDMOD_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_Data_Update_Mode(uint16_t pwmIndex, PWM_UPDMOD_t updateMode) 
 * @brief  Set PWM Register update mode
 * @details
 * this is used for register update synchronization between PWMs
 * Setting the MSTEN bit means that the UPDREQ flag is broadcast to other PWMs
 * if the UPDMOD field in the receiving PWM is set to "Client SOC" or 
 * "client immediate" then this will
 * take effect and data update between different PWM modules can be implemented
 **********************************************************************************/
inline static void PWM_Data_Update_Mode(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_UPDMOD_t updateMode)
{
    switch (pwmIndex)
    {
        case 1:
            PG1CONHbits.UPDMOD = updateMode;
            break;
        case 2:
            PG2CONHbits.UPDMOD = updateMode;
            break;
        case 3:
            PG3CONHbits.UPDMOD = updateMode;
            break;
        case 4:
            PG4CONHbits.UPDMOD = updateMode;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5CONHbits.UPDMOD = updateMode;
            break;
        case 6:
            PG6CONHbits.UPDMOD = updateMode;
            break;
        case 7:
            PG7CONHbits.UPDMOD = updateMode;
            break;
        case 8:
            PG8CONHbits.UPDMOD = updateMode;
            break;
    #endif        
        default:
            break;            
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_SOURCE_e 
 * @brief  settings for PWMPCI field in PGxLEBH register
 * @details
 *  
 **********************************************************************************/
enum PWM_PCI_SOURCE_e
{
    PWM_PCI_SOURCE_PWM1 = 0,
    PWM_PCI_SOURCE_PWM2 = 1,
    PWM_PCI_SOURCE_PWM3 = 2,
    PWM_PCI_SOURCE_PWM4 = 3,
    #if (PWM_COUNT == 8) 
    PWM_PCI_SOURCE_PWM5 = 4,
    PWM_PCI_SOURCE_PWM6 = 5,
    PWM_PCI_SOURCE_PWM7 = 6,
    PWM_PCI_SOURCE_PWM8 = 7    
    #endif
};
typedef enum PWM_PCI_SOURCE_e PWM_PCI_SOURCE_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Source1(uint16_t pwmIndex, PWM_PCI_SOURCE_t pciSource)
 * @brief  Set PWM source for PCI selection bits (for PCI source 1)
 * @details
 * The selected PWM Generator source does not affect the LEB counter. This source can be optionally 
 * used as a PCI input, PCI qualifier, PCI terminator or PCI terminator qualifier
 **********************************************************************************/
inline static void PWM_PCI_Source1(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_SOURCE_t pciSource)
{
    switch (pwmIndex)
    {
        case 1:
            PG1LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
        case 2:
            PG2LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
        case 3:
            PG3LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
        case 4:
            PG4LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
    #if (PWM_COUNT == 8) 
        case 5:
            PG5LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
        case 6:
            PG6LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
        case 7:
            PG7LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;
        case 8:
            PG8LEBHbits.PWMPCI = (uint16_t)pciSource;
            break;      
    #endif        
        default:
            break;
    }
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    RPnR_SOURCE_e 
 * @brief  peripheral output for re-mappable pins
 * @details
 *  
 **********************************************************************************/
enum RPnR_SOURCE_e
{
    RPnR_SOURCE_DefaultPORT = 0, ///> RPn tied to Default Pin
    RPnR_SOURCE_U1TX 		= 1, ///> RPn tied to UART1 Transmit
    RPnR_SOURCE_U1RTS 		= 2, ///> RPn tied to UART1 Request-to-Send
    RPnR_SOURCE_U2TX 		= 3, ///> RPn tied to UART2 Transmit
    RPnR_SOURCE_U2RTS 		= 4, ///> RPn tied to UART2 Request-to-Send
    RPnR_SOURCE_SDO1 		= 5, ///> RPn tied to SPI1 Data Output
    RPnR_SOURCE_SCK1 		= 6, ///> RPn tied to SPI1 Clock Output
    RPnR_SOURCE_SS1 		= 7, ///> RPn tied to SPI1 Client Select
    RPnR_SOURCE_SDO2 		= 8, ///> RPn tied to SPI2 Data Output
    RPnR_SOURCE_SCK2 		= 9, ///> RPn tied to SPI2 Clock Output
    RPnR_SOURCE_SS2 		= 10, ///> RPn tied to SPI2 Client Select
    RPnR_SOURCE_SDO3 		= 11, ///> RPn tied to SPI3 Data Output
    RPnR_SOURCE_SCK3 		= 12, ///> RPn tied to SPI3 Clock Output
    RPnR_SOURCE_SS3 		= 13, ///> RPn tied to SPI3 Client Select
    RPnR_SOURCE_REFCLKO 	= 14, ///> RPn tied to Reference Clock Output
    RPnR_SOURCE_OCM1 		= 15, ///> RPn tied to SCCP1 Output 
    RPnR_SOURCE_OCM2 		= 16, ///> RPn tied to SCCP2 Output 
    RPnR_SOURCE_OCM3 		= 17, ///> RPn tied to SCCP3 Output 
    RPnR_SOURCE_OCM4 		= 18, ///> RPn tied to SCCP4 Output 
    RPnR_SOURCE_OCM5 		= 19, ///> RPn tied to SCCP5 Output 
    RPnR_SOURCE_OCM6 		= 20, ///> RPn tied to SCCP6 Output 
    RPnR_SOURCE_CAN1TX 		= 21, ///> RPn tied to CAN1 Transmit 
    RPnR_SOURCE_CMP1 		= 23, ///> RPn tied to Comparator 1 Output 
    RPnR_SOURCE_CMP2 		= 24, ///> RPn tied to Comparator 2 Output 
    RPnR_SOURCE_CMP3 		= 25, ///> RPn tied to Comparator 3 Output 
    RPnR_SOURCE_U3TX 		= 27, ///> RPn tied to UART3 Transmit
    RPnR_SOURCE_U3RTS 		= 28, ///> RPn tied to UART3 Request-to-Send
    RPnR_SOURCE_PWM4H 		= 34, ///> RPn tied to PWM4H Output 
    RPnR_SOURCE_PWM4L 		= 35, ///> RPn tied to PWM4L Output 
    RPnR_SOURCE_PWMEA 		= 36, ///> RPn tied to PWM Event A Output 
    RPnR_SOURCE_PWMEB 		= 37, ///> RPn tied to PWM Event B Output 
    RPnR_SOURCE_QEICMP1 	= 38, ///> RPn tied to QEI1 Comparator Output
    RPnR_SOURCE_QEICMP2 	= 39, ///> RPn tied to QEI2 Comparator Output
    RPnR_SOURCE_CLC1OUT 	= 40, ///> RPn tied to CLC1 Output 
    RPnR_SOURCE_CLC2OUT 	= 41, ///> RPn tied to CLC2 Output 
    RPnR_SOURCE_OCM7 		= 42, ///> RPn tied to SCCP7 Output 
    RPnR_SOURCE_OCM8 		= 43, ///> RPn tied to SCCP8 Output 
    RPnR_SOURCE_PWMEC 		= 44, ///> RPn tied to PWM Event C Output 
    RPnR_SOURCE_PWMED 		= 45, ///> RPn tied to PWM Event D Output 
    RPnR_SOURCE_PTGTRG24 	= 46, ///> PTG Trigger Output 24
    RPnR_SOURCE_PTGTRG25 	= 47, ///> PTG Trigger Output 25
    RPnR_SOURCE_SENT1OUT 	= 48, ///> RPn tied to SENT1 Output 
    RPnR_SOURCE_SENT2OUT	= 49, ///> RPn tied to SENT2 Output
    RPnR_SOURCE_MCCP9A 		= 50, ///> RPn tied to MCCP9 Output A
    RPnR_SOURCE_MCCP9B 		= 51, ///> RPn tied to MCCP9 Output B
    RPnR_SOURCE_MCCP9C 		= 52, ///> RPn tied to MCCP9 Output C
    RPnR_SOURCE_MCCP9D 		= 53, ///> RPn tied to MCCP9 Output D
    RPnR_SOURCE_MCCP9E 		= 54, ///> RPn tied to MCCP9 Output E
    RPnR_SOURCE_MCCP9F 		= 55, ///> RPn tied to MCCP9 Output F
    RPnR_SOURCE_CLC3OUT 	= 59, ///> RPn tied to CLC4 Output
    RPnR_SOURCE_CLC4OUT 	= 60, ///> RPn tied to CLC4 Output
    RPnR_SOURCE_U1DTR 		= 61, ///> RPn tied to UART1 DTR
    RPnR_SOURCE_U2DTR 		= 62, ///> RPn tied to UART2 DTR
    RPnR_SOURCE_U3DTR 		= 63 ///> RPn tied to UART3 DTR        
};
typedef enum RPnR_SOURCE_e RPnR_SOURCE_t;


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    RPnR_VIRTUAL_PIN_e 
 * @brief  list of re-mappable pins for dsPIC33CK256MP508
 * @details
 *  
 **********************************************************************************/
enum RPnR_VIRTUAL_PIN_e
{
    RPnR_VIRTUAL_PIN_RP176 = 0,
    RPnR_VIRTUAL_PIN_RP177 = 1,
    RPnR_VIRTUAL_PIN_RP178 = 2,
    RPnR_VIRTUAL_PIN_RP179 = 3,
    RPnR_VIRTUAL_PIN_RP180 = 4,
    RPnR_VIRTUAL_PIN_RP181 = 5
};
typedef enum RPnR_VIRTUAL_PIN_e RPnR_VIRTUAL_PIN_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    RPnR_VirtualPin_Source(RPnR_VIRTUAL_PIN_t pin, RPnR_SOURCE_t peripheral)
 * @brief  Set source for a virtual pin
 * @details
 * These pins provide a simple way for inter-peripheral
 * connection without utilizing a physical pin. For
 * example, the output of the analog comparator can be
 * connected to RP176 and the PWM Fault input can be
 * configured for RP176 as well. This configuration allows
 * the analog comparator to trigger PWM Faults without
 * the use of an actual physical pin on the device
 **********************************************************************************/
inline static void RPnR_VirtualPin_Source(RPnR_VIRTUAL_PIN_t virtualPin, RPnR_SOURCE_t peripheral)
{
    
    __builtin_write_RPCON(0x0000); // unlock PPS
    
    switch (virtualPin)
    {
        case RPnR_VIRTUAL_PIN_RP176:
            RPOR24bits.RP176R = peripheral;
            break;
           
        case RPnR_VIRTUAL_PIN_RP177:
            RPOR24bits.RP177R = peripheral;
            break;
        
        case RPnR_VIRTUAL_PIN_RP178:
            RPOR25bits.RP178R = peripheral;
            break;
            
        case RPnR_VIRTUAL_PIN_RP179:
            RPOR25bits.RP179R = peripheral;
            break;    
            
        case RPnR_VIRTUAL_PIN_RP180:
            RPOR26bits.RP180R = peripheral;
            break;    

        case RPnR_VIRTUAL_PIN_RP181:
            RPOR26bits.RP181R = peripheral;
            break;                   
        
        default:
            break;
    };
    
    __builtin_write_RPCON(0x0800); // lock PPS
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    RPx_INPUT_e 
 * @brief  list of output selection for re-mappable pins (taken from dsPIC33CK256MP508 datasheet)
 * @details
 *  
 **********************************************************************************/
enum RPx_INPUT_e
{
    RPx_INPUT_VSS					= 0, ///> Internal
    RPx_INPUT_CMP1 					= 1, ///> Internal 
    RPx_INPUT_CMP2 					= 2, ///> Internal 
    RPx_INPUT_CMP3 					= 3, ///> Internal 
    RPx_INPUT_PTG_TRIG_26 			= 6, ///> Internal 
    RPx_INPUT_PTG_TRIG_27 			= 7, ///> Internal 
    RPx_INPUT_PWM_EVENT_C 			= 11, ///> Internal 
    RPx_INPUT_PWM_EVENT_D 			= 12, ///> Internal 
    RPx_INPUT_PWM_EVENT_E 			= 13, ///> Internal 
    RPx_INPUT_RP32 					= 32, ///> Port Pin RB0
    RPx_INPUT_RP33 					= 33, ///> Port Pin RB1
    RPx_INPUT_RP34 					= 34, ///> Port Pin RB2
    RPx_INPUT_RP35 					= 35, ///> Port Pin RB3
    RPx_INPUT_RP36 					= 36, ///> Port Pin RB4
    RPx_INPUT_RP37 					= 37, ///> Port Pin RB5
    RPx_INPUT_RP38 					= 38, ///> Port Pin RB6
    RPx_INPUT_RP39 					= 39, ///> Port Pin RB7
    RPx_INPUT_RP40 					= 40, ///> Port Pin RB8
    RPx_INPUT_RP41 					= 41, ///> Port Pin RB9
    RPx_INPUT_RP42 					= 42, ///> Port Pin RB10
    RPx_INPUT_RP43 					= 43, ///> Port Pin RB11
    RPx_INPUT_RP44 					= 44, ///> Port Pin RB12
    RPx_INPUT_RP45 					= 45, ///> Port Pin RB13
    RPx_INPUT_RP46 					= 46, ///> Port Pin RB14
    RPx_INPUT_RP47 					= 47, ///> Port Pin RB15
    RPx_INPUT_RP48 					= 48, ///> Port Pin RC0
    RPx_INPUT_RP49 					= 49, ///> Port Pin RC1
    RPx_INPUT_RP50 					= 50, ///> Port Pin RC2
    RPx_INPUT_RP51 					= 51, ///> Port Pin RC3
    RPx_INPUT_RP52 					= 52, ///> Port Pin RC4
    RPx_INPUT_RP53 					= 53, ///> Port Pin RC5
    RPx_INPUT_RP54 					= 54, ///> Port Pin RC6
    RPx_INPUT_RP55 					= 55, ///> Port Pin RC7
    RPx_INPUT_RP56 					= 56, ///> Port Pin RC8
    RPx_INPUT_RP57 					= 57, ///> Port Pin RC9
    RPx_INPUT_RP58 					= 58, ///> Port Pin RC10
    RPx_INPUT_RP59 					= 59, ///> Port Pin RC11
    RPx_INPUT_RP60 					= 60, ///> Port Pin RC12
    RPx_INPUT_RP61 					= 61, ///> Port Pin RC13
    RPx_INPUT_RP62 					= 62, ///> Port Pin RC14
    RPx_INPUT_RP63 					= 63, ///> Port Pin RC15
    RPx_INPUT_RP64 					= 64, ///> Port Pin RD0
    RPx_INPUT_RP65 					= 65, ///> Port Pin RD1
    RPx_INPUT_RP66 					= 66, ///> Port Pin RD2
    RPx_INPUT_RP67 					= 67, ///> Port Pin RD3
    RPx_INPUT_RP68 					= 68, ///> Port Pin RD4
    RPx_INPUT_RP69 					= 69, ///> Port Pin RD5
    RPx_INPUT_RP70 					= 70, ///> Port Pin RD6
    RPx_INPUT_RP71 					= 71, ///> Port Pin RD7
    RPx_INPUT_RP72 					= 72, ///> Port Pin RD8
    RPx_INPUT_RP73 					= 73, ///> Port Pin RD9
    RPx_INPUT_RP74 					= 74, ///> Port Pin RD10
    RPx_INPUT_RP75 					= 75, ///> Port Pin RD11
    RPx_INPUT_RP76 					= 76, ///> Port Pin RD12
    RPx_INPUT_RP77 					= 77, ///> Port Pin RD13
    RPx_INPUT_RP78 					= 78, ///> Port Pin RD14
    RPx_INPUT_RP79 					= 79, ///> Port Pin RD15
    RPx_INPUT_DAC3_pwm_req_on 		= 164, ///> Internal
    RPx_INPUT_DAC3_pwm_req_off 		= 165, ///> Internal
    RPx_INPUT_DAC2_pwm_req_on 		= 166, ///> Internal
    RPx_INPUT_DAC2_pwm_req_off 		= 167, ///> Internal
    RPx_INPUT_DAC1_pwm_req_on 		= 168, ///> Internal
    RPx_INPUT_DAC1_pwm_req_off 		= 169, ///> Internal
    RPx_INPUT_RP176 				= 176, ///> Virtual RPV0
    RPx_INPUT_RP177 				= 177, ///> Virtual RPV1
    RPx_INPUT_RP178 				= 178, ///> Virtual RPV2
    RPx_INPUT_RP179 				= 179, ///> Virtual RPV3
    RPx_INPUT_RP180 				= 180, ///> Virtual RPV4
    RPx_INPUT_RP181 				= 181  ///> Virtual RPV5
};
typedef enum RPx_INPUT_e RPx_INPUT_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_INPUT_e 
 * @brief  list of PWM PCI inputs
 * @details
 *  
 **********************************************************************************/
enum PWM_PCI_INPUT_e
{
    PWM_PCI_INPUT8 = 0,
    PWM_PCI_INPUT9 = 1,
    PWM_PCI_INPUT10 = 2,
    PWM_PCI_INPUT11 = 3
};
typedef enum PWM_PCI_INPUT_e PWM_PCI_INPUT_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_MaptoPin(PWM_PCI_INPUT_t pciIndex, RPx_INPUT_t pin)
 * @brief  Map PWM PCI input to a pin
 * @details
 * details taken from dsPIC33CK256MP508 datasheet
 **********************************************************************************/
inline static void PWM_PCI_INPUT_MaptoPin(PWM_PCI_INPUT_t pciIndex, RPx_INPUT_t pin)
{
    __builtin_write_RPCON(0x0000); // unlock PPS
    switch (pciIndex)
    {
        case PWM_PCI_INPUT8:
            RPINR12bits.PCI8R = pin;
            break;
            
        case PWM_PCI_INPUT9:
            RPINR12bits.PCI9R = pin;
            break;

        case PWM_PCI_INPUT10:
            RPINR13bits.PCI10R = pin;
            break;

        case PWM_PCI_INPUT11:
            RPINR13bits.PCI11R = pin;
            break;            
            
        default:
            break;            
    };
    __builtin_write_RPCON(0x0800); // lock PPS
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_INPUT_e 
 * @brief  list of PWM PCI inputs
 * @details
 *  
 **********************************************************************************/
enum PWM_PCI_SOURCE_SELECT_e
{
    PWM_PCI_SOURCE_SELECT_CLC1 		   = 31,
    // 30 is reserved           
    PWM_PCI_SOURCE_SELECT_CMP3         = 29,
    PWM_PCI_SOURCE_SELECT_CMP2         = 28,
    PWM_PCI_SOURCE_SELECT_CMP1         = 27,
    PWM_PCI_SOURCE_SELECT_PWM_EVENT_D  = 26,
    PWM_PCI_SOURCE_SELECT_PWM_EVENT_C  = 25,
    PWM_PCI_SOURCE_SELECT_PWM_EVENT_B  = 24,
    PWM_PCI_SOURCE_SELECT_PWM_EVENT_A  = 23,
    PWM_PCI_SOURCE_SELECT_PCI22		   = 22,	///> device pin
    PWM_PCI_SOURCE_SELECT_PCI21		   = 21,	///> device pin
    PWM_PCI_SOURCE_SELECT_PCI20		   = 20,	///> device pin
    PWM_PCI_SOURCE_SELECT_PCI19		   = 19,	///> device pin
    PWM_PCI_SOURCE_SELECT_PCI18R	   = 18,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI17R	   = 17,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI16R	   = 16,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI15R 	   = 15,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI14R 	   = 14,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI13R       = 13,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI12R       = 12,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI11R       = 11,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI10R	   = 10,	///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI9R        = 9,	    ///> RPn input
    PWM_PCI_SOURCE_SELECT_PCI8R        = 8,
    // 7 is reserved,           
    // 6 is reserved,           
    // 5 is reserved,           
    // 4 is reserved,           
    PWM_PCI_SOURCE_SELECT_COMBO_TRIG_B = 3,
    PWM_PCI_SOURCE_SELECT_COMBO_TRIG_A = 2,
    PWM_PCI_SOURCE_SELECT_PWMPCI_MUX   = 1,
    PWM_PCI_SOURCE_SELECT_TIED_TO_0    = 0
};

typedef enum PWM_PCI_SOURCE_SELECT_e PWM_PCI_SOURCE_SELECT_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Sync_Source_Select(uint16_t pwmIndex, PWM_PCI_SOURCE_SELECT_t source)
 * @brief  set PCI source (PSS field) for PCI sync
 * @details
 **********************************************************************************/
inline static void PWM_PCI_Sync_Source_Select(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_SOURCE_SELECT_t pciSource)
{
    switch (pwmIndex)
    {
        case 1:
            PG1SPCILbits.PSS = pciSource;
            break;
        case 2:
            PG2SPCILbits.PSS = pciSource;
            break;
        case 3:
            PG3SPCILbits.PSS = pciSource;
            break;
        case 4:
            PG4SPCILbits.PSS = pciSource;
            break;
    #if (PWM_COUNT == 8) 
        case 5:
            PG5SPCILbits.PSS = pciSource;
            break;
        case 6:
            PG6SPCILbits.PSS = pciSource;
            break;
        case 7:
            PG7SPCILbits.PSS = pciSource;
            break;      
        case 8:
            PG8SPCILbits.PSS = pciSource;
            break;  
    #endif        
        default:
            break;
    };
}


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_ACCEPTANCE_QUALIFER_e 
 * @brief  list of sources of PWM acceptance qualifers
 * @details
 *  
 **********************************************************************************/
enum PWM_PCI_ACCEPTANCE_QUALIFER_e
{
    PWM_PCI_ACCEPTANCE_QUALIFER_NONE = 0, ///> No acceptance qualifier is used (qualifier forced to ?1?)
    PWM_PCI_ACCEPTANCE_QUALIFER_DUTY = 1, ///> Duty cycle is active (base PWM Generator signal)
    PWM_PCI_ACCEPTANCE_QUALIFER_LEB = 2, ///> LEB is active
    PWM_PCI_ACCEPTANCE_QUALIFER_PWM = 3, ///> PWM Generator is triggered
    PWM_PCI_ACCEPTANCE_QUALIFER_PCI_PWMPCI = 4, ///> Selects PCI Source #1 (PWM Generator output selected by the PWMPCI[2:0] bits)
    PWM_PCI_ACCEPTANCE_QUALIFER_PCI_SOURCE8 = 5, ///> Selects PCI Source #8
    PWM_PCI_ACCEPTANCE_QUALIFER_PCI_SOURCE9 = 6, ///> Selects PCI Source #9
    PWM_PCI_ACCEPTANCE_QUALIFER_SWPCI = 7 ///> SWPCI control bit only (qualifier forced to ?0?)
};
typedef enum PWM_PCI_ACCEPTANCE_QUALIFER_e PWM_PCI_ACCEPTANCE_QUALIFER_t;


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Fault_AcceptanceQualifierSource(uint16_t pwmIndex, PWM_PCI_ACCEPTANCE_QUALIFER_e source)
 * @brief  set source of fault PCI acceptance qualifier
 * @details
 **********************************************************************************/
inline static void PWM_PCI_Fault_AcceptanceQualifierSource(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_ACCEPTANCE_QUALIFER_t source)
{
    switch (pwmIndex)
    {
        case 1:
            PG1FPCILbits.AQSS = source;
            break;
        case 2:
            PG2FPCILbits.AQSS = source;
            break;
        case 3:
            PG3FPCILbits.AQSS = source;
            break;
        case 4:
            PG4FPCILbits.AQSS = source;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5FPCILbits.AQSS = source;
            break;
        case 6:
            PG6FPCILbits.AQSS = source;
            break;
        case 7:
            PG7FPCILbits.AQSS = source;
            break;
        case 8:
            PG8FPCILbits.AQSS = source;
            break;      
    #endif        
        default:
            break;
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_TRIG_MODE_e 
 * @brief  set PWM trigger mode
 * @details
 * TRGNOD field
 **********************************************************************************/
enum PWM_TRIG_MODE_e
{
    PWM_TRIG_MODE_SINGLE = 0,
    PWM_TRIG_MODE_RETRIGGERABLE = 1
};
typedef enum PWM_TRIG_MODE_e PWM_TRIG_MODE_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Fault_AcceptanceQualifierSource(uint16_t pwmIndex, PWM_PCI_ACCEPTANCE_QUALIFER_e source)
 * @brief  set source of fault PCI acceptance qualifier
 * @details
 **********************************************************************************/
inline static void PWM_Trigger_Mode(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_TRIG_MODE_t trigMode)
{
    switch (pwmIndex)
    {
        case 1:
            PG1CONHbits.TRGMOD = trigMode;
            break;
        case 2:
            PG2CONHbits.TRGMOD = trigMode;
            break;
        case 3:
            PG3CONHbits.TRGMOD = trigMode;
            break;
        case 4:
            PG4CONHbits.TRGMOD = trigMode;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5CONHbits.TRGMOD = trigMode;
            break;
        case 6:
            PG6CONHbits.TRGMOD = trigMode;
            break;
        case 7:
            PG7CONHbits.TRGMOD = trigMode;
            break;
        case 8:
            PG8CONHbits.TRGMOD = trigMode;
            break;   
    #endif        
        default:
            break;
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_TERM_e 
 * @brief  termination event selection 
 * @details
 **********************************************************************************/
enum PWM_PCI_TERM_e
{
    PWM_PCI_TERM_MANUAL		= 0,	//> Manual Terminate: Terminate on a write of ?1? to the SWTERM bit location
    PWM_PCI_TERM_AUTO 		= 1,    //> Auto-Terminate: Terminate when PCI source transitions from active to inactive
    PWM_PCI_TERM_PGxTRIGA	= 2,    //> PGxTRIGA trigger event
    PWM_PCI_TERM_PGxTRIGB   = 3,    //> PGxTRIGB trigger event
    PWM_PCI_TERM_PGxTRIGC   = 4,    //> PGxTRIGC trigger event
    PWM_PCI_TERM_PCI_SOURCE1 = 5,   //> Selects PCI Source #1 (PWM Generator output selected by the PWMPCI[2:0] bits)
    PWM_PCI_TERM_PCI_SOURCE8 = 6,   //> Selects PCI Source #8
    PWM_PCI_TERM_PCI_SOURCE9 = 7    //> Selects PCI Source #9
};
typedef enum PWM_PCI_TERM_e PWM_PCI_TERM_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_PCI_Sync_TerminationEventSelect(uint16_t pwmIndex, PWM_PCI_TERM_t trigMode)
 * @brief  select termination event for SYNC PCI
 * @details
 **********************************************************************************/
inline static void PWM_PCI_Sync_TerminationEventSelect(PWM_GENERATOR_INSTANCE_t pwmIndex, PWM_PCI_TERM_t termEvent)
{
    switch (pwmIndex)
    {
        case 1:
            PG1SPCILbits.TERM = termEvent;
            break;
        case 2:
            PG2SPCILbits.TERM = termEvent;
            break;
        case 3:
            PG3SPCILbits.TERM = termEvent;
            break;
        case 4:
            PG4SPCILbits.TERM = termEvent;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5SPCILbits.TERM = termEvent;
            break;
        case 6:
            PG6SPCILbits.TERM = termEvent;
            break;
        case 7:
            PG7SPCILbits.TERM = termEvent;
            break;
        case 8:
            PG8SPCILbits.TERM = termEvent;
            break;       
    #endif        
        default:
            break;
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_Swap_PWMxL_and_PWMxH(uint16_t pwmIndex, bool swap)
 * @brief  set swapPWMH_PWML to true to swap PWML and PWMH
 * @details
 **********************************************************************************/
inline static void PWM_Swap_PWMxL_and_PWMxH(PWM_GENERATOR_INSTANCE_t pwmIndex, bool swapPWMH_PWML)
{
    switch (pwmIndex)
    {
        case 1:
            PG1IOCONLbits.SWAP = swapPWMH_PWML;
            break;
        case 2:
            PG2IOCONLbits.SWAP = swapPWMH_PWML;
            break;
        case 3:
            PG3IOCONLbits.SWAP = swapPWMH_PWML;
            break;
        case 4:
            PG4IOCONLbits.SWAP = swapPWMH_PWML;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5IOCONLbits.SWAP = swapPWMH_PWML;
            break;
        case 6:
            PG6IOCONLbits.SWAP = swapPWMH_PWML;
            break;
        case 7:
            PG7IOCONLbits.SWAP = swapPWMH_PWML;
            break;
        case 8:
            PG8IOCONLbits.SWAP = swapPWMH_PWML;
            break;     
    #endif        
        default:
            break;
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_EVENT_SOURCE_e 
 * @brief  select event source for PWMEVTx register
 * @details
 **********************************************************************************/
enum PWM_EVENT_SOURCE_e
{
    PWM_EVENT_SOURCE_PGTRGSEL = 0,
    PWM_EVENT_SOURCE_PWM_GEN_OUTPUT = 1,
    PWM_EVENT_SOURCE_PCI_SYNC_ACTIVE = 2,
    PWM_EVENT_SOURCE_PCI_FF_ACTIVE = 3,
    PWM_EVENT_SOURCE_PCI_FF_CL_ACTIVE = 4,
    PWM_EVENT_SOURCE_PCI_FAULT_ACTIVE = 5,
    PWM_EVENT_SOURCE_CAHALF = 6,
    PWM_EVENT_SOURCE_STEER = 7,
    PWM_EVENT_SOURCE_ADC_TRIG1 = 8,
    PWM_EVENT_SOURCE_ADC_TRIG2 = 9,
    PWM_EVENT_SOURCE_HR_ERROR_EVENT = 15
};
typedef enum PWM_EVENT_SOURCE_e PWM_EVENT_SOURCE_t;


/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_EVENTA_Configure
 * @brief  Configure Event A register
 * @details
 **********************************************************************************/
inline static void PWM_EVENTA_Configure(  PWM_GENERATOR_INSTANCE_t pwmSource, 
                                          PWM_EVENT_SOURCE_t eventSource,
                                          bool invert,
                                          bool outputEnable,
                                          bool stretchDisable,
                                          bool outputSync)
{
    {        
        PWMEVTAbits.EVTAPGS = (pwmSource-1);
        PWMEVTAbits.EVTAOEN = outputEnable;
        PWMEVTAbits.EVTAPOL = invert;
        PWMEVTAbits.EVTASTRD = stretchDisable;
        PWMEVTAbits.EVTASYNC = outputSync;
        PWMEVTAbits.EVTASEL = eventSource;        
    };
}

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_UPD_TRIG_e 
 * @brief  settings for UPDTRG field
 * @details
 *  
 **********************************************************************************/
enum PWM_UPDTRG_e
{
    PWM_UPDTRG_MANUAL = 0,      ///> user must manual set UPDREQ bit to update
    PWM_UPDTRG_PGxDC = 1,       ///> a write to PGxDC will update
    PWM_UPDTRG_PGxPHASE = 2,    ///> a write to PGxPHASE will update
    PWM_UPDTRG_PGxTRIGA = 3     ///> a write to PGxTRIGA will update
};
typedef enum PWM_UPDTRG_e PWM_UPDTRG_t;

/*********************************************************************************
 * @ingroup GROUP_NAME
 * @def    PWM_Data_Update_Trigger(uint16_t pwmIndex, PWM_UPDTRG_t updateTrigger) 
 * @brief  Set PWM Register update trigger
 * @details
 **********************************************************************************/
inline static void PWM_Data_Update_Trigger(uint16_t pwmIndex, PWM_UPDTRG_t updateTrigger)
{
    switch (pwmIndex)
    {
        case 1:
            PG1EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;
        case 2:
            PG2EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;
        case 3:
            PG3EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;
        case 4:
            PG4EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;
    #if (PWM_COUNT == 8)         
        case 5:
            PG5EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;
        case 6:
            PG6EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;
        case 7:
            PG7EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;       
        case 8:
            PG8EVTLbits.UPDTRG = (uint16_t)updateTrigger;
            break;   
    #endif        
        default:
            break;
    }
}
#endif
#endif	/* DRV_MCC_EXTENSION_PWM_H */

