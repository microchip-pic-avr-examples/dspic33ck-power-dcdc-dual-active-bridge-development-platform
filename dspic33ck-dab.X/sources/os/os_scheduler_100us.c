
//=======================================================================================================
// @file os_scheduler_100us.c
// @ingroup sched-layer
// @brief contains the 100�s scheduler that calls all the tasks that need to be called regularly
//        two different timing priorities are available:
//          1. 100�s and 1ms Tasks called from the scheduler interrupt
//              the jitter that you will have in the 100�s realtime tasks called by the interrupt depends
//              on other interrupts that have a higher interrupt priority
//              the jitter that you will have in the 1ms realtime tasks called by the interrupt depends
//              on other interrupts that have a higher interrupt priority amd by the duration of the
//              100�s realtime task
//          2. 100�s, 1ms, 10ms, 100ms, 1s Tasks called from the main loop
//              these tasks are for soft realtime and not for hard realtime
//              so in average they are called with the required timing but the jitter can be very huge,
//              depending on the calls before.
//              use this for your non-timing critical application state machines
//
//  @note   put your application specific code in main/main_scheduler.c
//
// @version v1.0
// @date 2019-08-29
// @author M52409
//
//=======================================================================================================

#include <stdint.h>
#include <xc.h>
#include "os_config.h"
#include "os/os_sys_time.h"
#if OS_FEATURE_WATCHDOG_ENABLED == 1
#include "os_watchdog.h"
#endif

#if OS_USE_SCHEDULER_100us == 1

#ifndef OS_TIMER_NUMBER_OF_TIMERS
#warning OS_TIMER_NUMBER_OF_TIMERS needs to be defined in main/project_setting.h
#endif
#ifndef OS_USE_SYSTIME
#warning OS_USE_SYSTIME needs to be defined in main/project_setting.h
#endif
#include "os/os_scheduler.h"
#include "timer/tmr1.h"

//=======================================================================================================
//  The following extern Tasks_... functions should be placed in the file main/main_tasks.c
//  call OS_Scheduler_Run() in the main loop or call OS_Scheduler_RunForever() in main.c
//=======================================================================================================
//void Tasks_Realtime_100us(void);
//void Tasks_Realtime_1ms(void);
//void Tasks_100us(void);
//void Tasks_1ms(void);
//void Tasks_10ms(void);
//void Tasks_100ms(void);
//void Tasks_1s(void);
//void Tasks_Background(void);

void TMR1_CallBack(void);

//=======================================================================================================
//
//  put your application specific code in the file main/main_scheduler.c in the following functions:
//                              choose wisely between real-time and non-realtime!
//
//  Interrupt Realtime Functions:
//  Tasks_Realtime_100us:   is called by the 100�s interrupt    - for time critical low jitter stuff
//  Tasks_Realtime_1ms  :   is called by the interrupt every ms - for time critical low jitter stuff
//
//
//  Mainloop Non-Realtime Functions:
//  Tasks_100us         :   function is called by the main loop in average every 100�s
//  Tasks_1ms           :   function is called by the main loop in average every 1ms
//  Tasks_10ms          :   function is called by the main loop in average every 10ms
//  Tasks_100ms         :   function is called by the main loop in average every 100ms
//  Tasks_1s            :   function is called by the main loop in average every second
//
//  @note there could be some jitter here because it is not called directly by a timer interrupt
//        the timing in average is exact (keep in mind: in average), the jitter depends on the
//        called functions before
//=======================================================================================================

#if OS_TIMER_NUMBER_OF_TIMERS > 0
    void OS_Timer_Tick(void);
#endif

static volatile uint16_t   scheduler_interrupt_leader_100us = 0;
static volatile uint16_t   scheduler_interrupt_follower_100us = 0;
static volatile uint8_t    scheduler_interrupt_realtime_counter_1ms = 0;

#if OS_USE_MCC_TIMER1 == 0
static inline void OS_Scheduler_Init_Timer1_100us(void)
{
    //Switch off Timer 1
    T1CONbits.TON = 0;      // Disable Timer1
    IEC0bits.T1IE = 0;      // Disable Timer1 interrupt

    //Configure Timer 1
    T1CONbits.TSIDL = 0;    // Timer1 Stop in Idle Mode: Continues module operation in Idle mode
    T1CONbits.TMWDIS = 0;   // Asynchronous Timer1 Write Disable: Back-to-back writes are enabled in Asynchronous mode
    T1CONbits.TMWIP = 0;    // Asynchronous Timer1 Write in Progress: Write to the timer in Asynchronous mode is complete
    T1CONbits.PRWIP = 0;    // Asynchronous Period Write in Progress: Write to the Period register in Asynchronous mode is complete
    T1CONbits.TECS = 0b11;  // Timer1 Extended Clock Select: FRC clock
    T1CONbits.TGATE = 0;    // Timer1 Gated Time Accumulation Enable: Gated time accumulation is disabled when TCS = 0
    T1CONbits.TCKPS = 0;    // Timer1 Input Clock Prescale Select: 1:1
    T1CONbits.TCKPS = 0; // Timer1 Input Clock Prescale Select: 1:1
    T1CONbits.TSYNC = 0;    // Timer1 External Clock Input Synchronization Select: Does not synchronize the External Clock input
    T1CONbits.TCS = 0;      // Timer1 Clock Source Select: Internal peripheral clock

    TMR1 = 0x00;            // Reset Timer Counter Register TMR to Zero; 
    PR1 = 9999;             // Period = 0.0001 s; Frequency = 100000000 Hz; PR 9999

    IPC0bits.T1IP = 1;      // Set interrupt priority to one (cpu is running on ip zero)
    IFS0bits.T1IF = 0;      // Reset interrupt flag bit

    //Switch on Timer 1
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    T1CONbits.TON = 1;      // Enable Timer1
    
}
#endif

//=======================================================================================================
/**  @ingroup sched-layer
//  @brief  Initializes Scheduler
//  @note   call this function in your main routine before calling the RunForever function
*/
//=======================================================================================================
void OS_Scheduler_Init(void)
{
    #if OS_USE_MCC_TIMER1 == 0
    OS_Scheduler_Init_Timer1_100us();
    #endif
#if OS_USE_SYSTIME == 1
    OS_SysTime_ResetTime();
#endif  //OS_USE_SYSTIME
    scheduler_interrupt_leader_100us = 0U;   // reset directly before calling the Scheduler Loop
    scheduler_interrupt_follower_100us = 0U; // reset directly before calling the Scheduler Loop
    
    TMR1_TimeoutCallbackRegister (TMR1_CallBack);       // melody breaks the naming convention, therefore manually registering the name.
}


//=======================================================================================================
/**  @ingroup sched-layer
//  @brief  Timer 1 interrupt routine for generating the 100�s timing for the scheduler
//  @note   with this simple implementation we do not lose any tick from the timer, even when the tasks
//          in the main loop take longer than 100�s
*/
//=======================================================================================================
#if defined (OS_USE_MCC_TIMER1) && (OS_USE_MCC_TIMER1 == true)
/* TMR1_CallBack is a weak linked function in the tmr1.c */
/* LDRA_EXCLUDE 34 D */
void TMR1_CallBack(void)
#else
void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
#endif
{
    scheduler_interrupt_leader_100us++; //increment our counter for the scheduler, no tick gets lost
    _T1IF = 0;                          //clear Timer1 interrupt flag
#if OS_TIMER_NUMBER_OF_TIMERS > 0
    OS_Timer_Tick();
#endif
    Tasks_Realtime_100us();
    scheduler_interrupt_realtime_counter_1ms += 1U;
    if (scheduler_interrupt_realtime_counter_1ms >= 10U)
    {
#if OS_USE_SYSTIME == 1
        OS_SysTime_IncrementTime_1ms();
#endif  //OS_USE_SYSTIME
        Tasks_Realtime_1ms();
        scheduler_interrupt_realtime_counter_1ms = 0U;
    }
}


//=======================================================================================================
/**  @ingroup sched-layer
//  @brief  Scheduler function for calling all the Tasks regularly ( 100us, 1ms, 10ms, 100ms, 1s )
//  @note   call this function as last function in main.c after calling the Init-function
//          please consider that the timing of the calls are dependent on the duration of the last call
//          the resulting jitter therefore depends on the timing of the calls before
*/
//=======================================================================================================
/* OS_Scheduler_RunOnce() is not called in this application, but is 
 available to do so */
/* LDRA_EXCLUDE 61 D */ 
void OS_Scheduler_RunOnce(void)
{
    volatile static uint16_t scheduler_1ms_timer = 0U;       // local counter for 1ms tasks
    volatile static uint16_t scheduler_10ms_timer = 0U;      // local counter for 10ms tasks
    volatile static uint16_t scheduler_100ms_timer = 0U;     // local counter for 100ms tasks
    volatile static uint16_t scheduler_1s_timer = 0U;        // local counter for 1s tasks

    //TODO: should we implement a Watchdog that gets triggered in one of the Task-Routines?

    if (scheduler_interrupt_follower_100us != scheduler_interrupt_leader_100us)
    {
        scheduler_interrupt_follower_100us += 1U;
        Tasks_100us();                              //call 100�s tasks
        scheduler_1ms_timer += 1U;
        if (scheduler_1ms_timer >= 10U)
        {
            scheduler_1ms_timer = 0U;                //reset 1 ms timer
            Tasks_1ms();                            //call 1 ms tasks
            scheduler_10ms_timer += 1U;
            if (scheduler_10ms_timer >= 10U)
            {
                scheduler_10ms_timer = 0U;           //reset 10 ms timer
                Tasks_10ms();                       //call 10 ms tasks
                scheduler_100ms_timer += 1U;
                if (scheduler_100ms_timer >= 10U)
                {
                    scheduler_100ms_timer = 0U;      //reset 100 ms timer
                    Tasks_100ms();                  //call 100 ms tasks
                    scheduler_1s_timer += 1U;
                    if (scheduler_1s_timer >= 10U)
                    {
                        scheduler_1s_timer = 0U;
                        #if OS_FEATURE_WATCHDOG_ENABLED == 1
							OS_Watchdog_KeepAlivePing();
                        #endif	
                        Tasks_1s();                  //call 1 s tasks
                    }
                }
            }
        }
    }
    else
    {
        Tasks_Background();                         // run the background tasks with their own timing
    }
}


//=======================================================================================================
/**  @ingroup sched-layer
//  @brief  Scheduler function for calling all the Tasks regularly ( 100us, 1ms, 10ms, 100ms, 1s )
//  @note   call this function as last function in main.c after calling the Init-function
//          please consider that the timing of the calls are dependent on the duration of the last call
//          the resulting jitter therefore depends on the timing of the calls before
*/
void OS_Scheduler_RunForever(void)
{
    // do some initialization
    scheduler_interrupt_leader_100us = 0U;   // reset directly before calling the Scheduler Loop
    scheduler_interrupt_follower_100us = 0U; // reset directly before calling the Scheduler Loop

    /*The while loop below is intended to run forever therefore 28 D is 
     excluded */
    /* LDRA_EXCLUDE 28 D */
    while (1)                       // run that loop forever
    {       
        OS_Scheduler_RunOnce();        
    }
}

#endif  //OS_USE_SCHEDULER_100us
