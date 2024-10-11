
//=======================================================================================================
// @file os/os.c
//
// @brief contains the init function of the operating system and calls all the other Init functions
//
// @version v1.0
// @date 2019-23-12
// @author M52409
//
//=======================================================================================================

#include "os/os_scheduler.h"
#include "os/os.h"
#include "stdint.h"

#if OS_FEATURE_WATCHDOG_ENABLED == true
#include "os_watchdog.h"
#endif     

#ifndef OS_TIMER_NUMBER_OF_TIMERS
#warning OS_TIMER_NUMBER_OF_TIMERS needs to be defined in main/project_setting.h
#endif
#ifndef OS_USE_SYSTIME
#warning OS_USE_SYSTIME needs to be defined in main/project_setting.h
#endif

#if OS_TIMER_NUMBER_OF_TIMERS > 0
#include"os/os_timer.h"
#endif


//=======================================================================================================
//  @brief  Initializes Scheduler
//  @note   call this function in your main routine before calling the RunForever function
//=======================================================================================================
void OS_Init(void)
{
/* LDRA_EXCLUDE 337 S */    
#if OS_RESETTHINGY == 1
	if (RCONbits.WDTO)
		os_resetCause |= 1 << OS_RESETCAUSE_WATCHDOG;
	RCON = 0;
#endif
    
#if OS_FEATURE_WATCHDOG_ENABLED == 1
    OS_Watchdog_Init();
#endif

    OS_Scheduler_Init();
#if OS_TIMER_NUMBER_OF_TIMERS > 0
	OS_Timer_Init();
#endif
}

