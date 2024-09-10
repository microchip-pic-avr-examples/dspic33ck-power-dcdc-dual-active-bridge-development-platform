/* 
 * File:   drv_mcc_config_pwm.h
 * Author: M15690
 *
 * Created on 10 July 2024, 14:50
 */

#ifndef DRV_CONFIG_CUSTOM_PWM_H
#define	DRV_CONFIG_CUSTOM_PWM_H

#include "device/pwrctrl/dev_pwrctrl_typedef.h"
#include "drv_mcc_extension_pwm.h"
#include "config/macros.h"
#include "pwm_hs/pwm.h"

static __inline__ void DRV_Config_Custom_PWM(void)
{
    //PWMs needs to be re-triggerable for cascaded PWM scheme
    PWM_Trigger_Mode(PWM_GENERATOR_1, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_GENERATOR_2, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_GENERATOR_3, PWM_TRIG_MODE_RETRIGGERABLE);
    PWM_Trigger_Mode(PWM_GENERATOR_4, PWM_TRIG_MODE_RETRIGGERABLE);
    
    // PWM Swap output based on the hardware
    PWM_Swap_PWMxL_and_PWMxH(PWM_GENERATOR_3, true);
    PWM_Swap_PWMxL_and_PWMxH(PWM_GENERATOR_4, true);
    
//    // fault configuration not covered by Melody  
//    // PWM6 "phantom" PMM which is triggered when PWM1H changes state
//    // is used as LEB signal for PWM1, PWM2, PWM3 and PWM4
//    // the FAULT PCI input for these 4 PWMs needs to have "latched"
//    // as the acceptance criteria. If its "latched rising edge" and the
//    // edge occurs in the LEB period but the comparator stays high
//    // the fault is ignored (as no rising edge to latch it)
//    // want level and edge sensitive acceptance
//    PWM_PCI_Fault_AcceptanceCriteria(pcInstance->Pwm.Primary_1, PWM_PCI_ACCEPT_LATCHED);     
//    PWM_PCI_Fault_AcceptanceCriteria(pcInstance->Pwm.Secondary_1, PWM_PCI_ACCEPT_LATCHED);    
//    PWM_PCI_Fault_AcceptanceCriteria(pcInstance->Pwm.Primary_2, PWM_PCI_ACCEPT_LATCHED);     
//    PWM_PCI_Fault_AcceptanceCriteria(pcInstance->Pwm.Secondary_2, PWM_PCI_ACCEPT_LATCHED);     
//    
//    // configure for LEB - PCI source should be PWM6 (phantom PWM used for LEB)
//    // this signal is routed to the acceptance qualifier for the PCI fault input for
//    // these four PWMs
//    PWM_PCI_Source1(pcInstance->Pwm.Primary_1, PWM_PCI_SOURCE_PWM6);
//    PWM_PCI_Source1(pcInstance->Pwm.Primary_2, PWM_PCI_SOURCE_PWM6);
//    PWM_PCI_Source1(pcInstance->Pwm.Secondary_1, PWM_PCI_SOURCE_PWM6);
//    PWM_PCI_Source1(pcInstance->Pwm.Secondary_2, PWM_PCI_SOURCE_PWM6);
//    
//#ifndef I_SC_LEB_DISABLED    
//    // below is needed for LEB invert output of "LEB" signal from PWM6 as we want to accept faults outside the 
//    // LEB high time only
//    PWM_PCI_Fault_AcceptanceQualiferInvert(pcInstance->Pwm.Primary_1, true);
//    PWM_PCI_Fault_AcceptanceQualiferInvert(pcInstance->Pwm.Secondary_1, true);
//    PWM_PCI_Fault_AcceptanceQualiferInvert(pcInstance->Pwm.Primary_2, true);
//    PWM_PCI_Fault_AcceptanceQualiferInvert(pcInstance->Pwm.Secondary_2, true);
//        
//    // PWMPCI mux has been set to PWM_PCI_SOURCE_PWM6 above
//    PWM_PCI_Fault_AcceptanceQualifierSource(pcInstance->Pwm.Primary_1, PWM_PCI_ACCEPTANCE_QUALIFER_PCI_PWMPCI);
//    PWM_PCI_Fault_AcceptanceQualifierSource(pcInstance->Pwm.Secondary_1, PWM_PCI_ACCEPTANCE_QUALIFER_PCI_PWMPCI);
//    PWM_PCI_Fault_AcceptanceQualifierSource(pcInstance->Pwm.Primary_2, PWM_PCI_ACCEPTANCE_QUALIFER_PCI_PWMPCI);
//    PWM_PCI_Fault_AcceptanceQualifierSource(pcInstance->Pwm.Secondary_2, PWM_PCI_ACCEPTANCE_QUALIFER_PCI_PWMPCI);  
//#endif // #ifndef I_SC_LEB_DISABLED
//     
//    // stop PWMs straight away upon detection of a fault
//    PWM_PCI_Fault_TimetoTerminateAfterEvent(pcInstance->Pwm.Primary_1, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
//    PWM_PCI_Fault_TimetoTerminateAfterEvent(pcInstance->Pwm.Secondary_1, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
//    PWM_PCI_Fault_TimetoTerminateAfterEvent(pcInstance->Pwm.Primary_2, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
//    PWM_PCI_Fault_TimetoTerminateAfterEvent(pcInstance->Pwm.Secondary_2, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
//           
//    // set all PWMs to be in re-triggerable mode
//    PWM_Trigger_Mode(pcInstance->Pwm.Primary_1, PWM_TRIG_MODE_RETRIGGERABLE);
//    PWM_Trigger_Mode(pcInstance->Pwm.Secondary_1, PWM_TRIG_MODE_RETRIGGERABLE);
//    PWM_Trigger_Mode(pcInstance->Pwm.Secondary_2, PWM_TRIG_MODE_RETRIGGERABLE);
//    PWM_Trigger_Mode(pcInstance->Pwm.Primary_2, PWM_TRIG_MODE_RETRIGGERABLE);
//
//    //TODO: move this configuration else-where!!
//    //
//    // settings for PWM6 which is the "phantom PWM" used for LEB on all PWMs 
//    // on the rising and falling edges of PWM1H
//    //
//    // we want LEB on the rising and falling edges of PWM1H
//    // cannot use LEB feature in the dsPIC33C as it is, because LEB triggering
//    // is tied to that PWM only (for example, cannot route LEB signal from PWM1 to PWM2)
//    // one solution is to use a "phantom PWM" (PWM6 in our case)
//    // use duty of this PWM as the LEB signal
//    // this PWM can be routed as a Acceptance Qualifier to the PCI fault inputs of other PWMs
//    // trigger this PWM from either PWM1H rising or falling edge using the PCI sync input
//    // create this trigger by XORing PWM1H and PWM1L via CLC2
//    // PCI sync source select doesn't have CLC2 as an option (only CLC1 which we use for PCI fault already)
//    // use CLC2 to xor PWM1H and PWM1L
//    // tie CLC2 output to a virtual pin RP176
//    // tie RP176 to PCI input 9
//    // then use PCI input 9 as trigger for PWM6 (the phantom PWM)
//    // this means that PWM6 will run every time PWM1H changes state
//    // during the duty cycle of PWM6, comparator faults on all PWMs are ignored, as PWM6 is used as 
//    // the PCI fault acceptance qualifier for all PWMs
//    
//    // set LEB blanking time via PG6DC register
//    // when phantom PWM is on = comparators faults are blanked
//    PWM_DutyCycleSet(PWM_LEB, I_SC_LEB_TIME_PGxDC);
//    
//    // PG6 is "phantom PWM" used for LEB of PWM1H rising and falling edges
//    // re-triggerable mode is needed as SOC is coming from 
//    // CLC2
//    PWM_Trigger_Mode(PWM_LEB, PWM_TRIG_MODE_RETRIGGERABLE);
//    
//    // trigger for phantom PWM
//    // CLC to xor PWM1H and PWM1L: rising edge on CLC2 output whenever PWM1H changes state
//    // use this rising edge to trigger phantom PWM
//    // tie CLC2 to RP176 (virtual pin)
//    // use RP176 as PCI sync input to trigger phantom PWM start of cycle
//    RPnR_VirtualPin_Source(RPnR_VIRTUAL_PIN_RP176, RPnR_SOURCE_CLC2OUT);
//    
//    // map PCI input 9 to virtual pin RP176
//    PWM_PCI_INPUT_MaptoPin(PWM_PCI_INPUT9, RPx_INPUT_RP176); 
//    
//    // set PCI Sync source select of PWM6 to PCI input 9 - so CLC2 set to PCI sync source select
//    PWM_PCI_Sync_Source_Select(PWM_LEB, PWM_PCI_SOURCE_SELECT_PCI9R);
//    
//    // terminate immediately after event (don't wait until EOC)
//    PWM_PCI_Sync_TimetoTerminateAfterEvent(PWM_LEB, PWM_PCI_TERMTIME_AFTER_EVENT_IMMEDIATE);
//    
//    PWM_PCI_Sync_TerminationEventSelect(PWM_LEB, PWM_PCI_TERM_AUTO);
//    
//    // rising edge on CLC2 output triggers a start of cycle on phantom PWM
//    PWM_PCI_Sync_AcceptanceCriteria(PWM_LEB, PWM_PCI_ACCEPT_RISING_EDGE);
    
}

#endif	/* DRV_MCC_CONFIG_PWM_H */

