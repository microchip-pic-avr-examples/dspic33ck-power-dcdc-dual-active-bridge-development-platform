
//=======================================================================================================
// @file project_setting.h
//
// @brief contains some project specific settings regarding the operating system parts and application
//
// @version v1.0
// @date 2019-08-22
// @author M52409
//
//=======================================================================================================

#ifndef _PROJECT_SETTINGS_H_
#define	_PROJECT_SETTINGS_H_
#include <stdbool.h>

//  OS Settings for the OS_Scheduler
//
//  Decide which timing precision you need for your project and choose the right scheduler
//-------------------------------------------------------------------------------------------------------
#define OS_USE_SCHEDULER_100us true
#define OS_USE_SCHEDULER_1ms   false
//=======================================================================================================


//=======================================================================================================
//  OS Settings for the OS_Scheduler
//
//  Decide if you want to use OS_Systime in your project
//-------------------------------------------------------------------------------------------------------
    #define OS_USE_SYSTIME false
//=======================================================================================================

//=======================================================================================================
//  OS Settings for OS_Timer
//-------------------------------------------------------------------------------------------------------
    #define OS_TIMER_NUMBER_OF_TIMERS       false
//-------------------------------------------------------------------------------------------------------
//  Application specific Timers:
//    #define OS_TIMER_COUNTDOWN_MY_STATUS_LED   0
//=======================================================================================================

//=======================================================================================================
//  OS Settings for the watchdog
//-------------------------------------------------------------------------------------------------------
#define OS_FEATURE_WATCHDOG_ENABLED true
//=======================================================================================================

//=======================================================================================================
// USE MCC driver layer if available
//-------------------------------------------------------------------------------------------------------
#define OS_USE_MCC_TIMER1 true
#define OS_USE_MCC_WATCHDOG true
//#define OS_USE_MCC_RESET true
//=======================================================================================================


#endif	//_PROJECT_SETTINGS_H_

