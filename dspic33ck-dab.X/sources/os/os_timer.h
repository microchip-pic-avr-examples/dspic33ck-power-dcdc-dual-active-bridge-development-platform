
//=======================================================================================================
// @file os_timer.h
//
// @brief contains the functions for using software timers
//
// @note    in this file you will find functions for using timers and countdowns for your application
//          These software timers are updated by the hardware timer interrupt of the os_scheduler
//          The number of timers need to be defined in main/project_settings.h :
//              #define OS_TIMER_NUMBER_OF_TIMERS 3
//          You should also put your your individual Timer defines with the corresponding index there
//              #define OS_TIMER_COUNTDOWN_EXAMPLE_SWITCHOFFTHELIGHT        0
//              #define OS_TIMER_COUNTDOWN_EXAMPLE_SWITCHOFFCOFFEEMACHINE   1
//              #define OS_TIMER_COUNTDOWN_EXAMPLE_IAMGOINGHOMENOW          2
//
//
// @version v1.0
// @date 2019-09-03
// @author M52409
//
//=======================================================================================================

#ifndef OS_TIMER_H
#define	OS_TIMER_H

#include <stdbool.h>
#include "os_config.h"

#ifndef OS_TIMER_NUMBER_OF_TIMERS
    #error OS_TIMER_NUMBER_OF_TIMERS needs to be defined in the file project_settings.h
#endif  //OS_TIMER_NUMBER_OF_TIMERS

#ifdef	__cplusplus
extern "C" {
#endif // __cplusplus

#ifdef	__cplusplus
}
#endif  // __cplusplus

//=======================================================================================================
// @brief   OS_Timer_Init initializes the Software Timers
// @note    call this function at boot up to initialize the Software Timers
//=======================================================================================================
void OS_Timer_Init(void);


//=======================================================================================================
// @brief   OS_Timer_StartCountdown_100us/1ms starts a new Countdown
// @note    call this function if you want to use a new countdown
// @param   timer_index - index of the timer that should be used for that countdown
// @param   numOfTicks - depending on the scheduler timing, the number of 100�s or 1ms ticks to count
//=======================================================================================================
#if OS_USE_SCHEDULER_100us == 1
void OS_Timer_StartCountdown_100us(uint8_t timer_index, uint16_t numOfTicks);
#elif OS_USE_SCHEDULER_1ms == 1
void OS_Timer_StartCountdown_1ms(uint8_t timer_index, uint16_t numOfTicks);
#else
#error OS_USE_SCHEDULER_100us or OS_USE_SCHEDULER_1ms needs to be definied as 1 to get the right timer functions
#endif


//=======================================================================================================
// @brief   OS_Timer_IsCountdownExpired returns the if the countdown is already expired
// @returns true, if the countdown is expired
//=======================================================================================================
bool OS_Timer_IsCountdownExpired(uint8_t timer_index);

//=======================================================================================================
// @brief   OS_Timer_Tick counts the Timer ticks
// @note    Call this function as a countdown timer 
//=======================================================================================================
void OS_Timer_Tick(void);

#endif  // OS_TIMER_H

