
//=======================================================================================================
// @file os_scheduler_1ms.h
//
// @brief contains the main scheduler that calls all the tasks that need to be called regularly
//
// @note put your application specific tasks that need to be called regularly into main_tasks.c
// @note call the Init function in your main.c
// @note call the OS_Scheduler_Run in your main loop or call the RunForever-Function in your main.c
//
// @version v1.0
// @date 2019-08-02
// @author M52409
//
//=======================================================================================================


#ifndef _OS_SCHEDULER_H_
#define _OS_SCHEDULER_H_


#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif


//=======================================================================================================
//  @brief  Initializes Scheduler
//  @note   call this function in your main routine before calling the RunForever function
//=======================================================================================================
extern void OS_Scheduler_Init(void);

//=======================================================================================================
//  @brief  Scheduler function for calling all the Tasks regularly ( 1ms, 10ms, 100ms, 1s )
//  @note   call this function in your main loop in main.c after calling the Init-function
//          if you have nothing else to call in your main loop then you can call the function
//          Main_Scheduler_RunForever() instead.
//          please consider that the timing of the calls are dependent on the duration of the last call
//          the resulting jitter therefore depends on the timing of the calls before
//=======================================================================================================
extern void OS_Scheduler_RunOnce(void);

//=======================================================================================================
//  @brief  Scheduler function for calling all the Tasks regularly ( 1ms, 10ms, 100ms, 1s )
//  @note   call this function as last function in main.c after calling the Init-function
//          please consider that the timing of the calls are dependent on the duration of the last call
//          the resulting jitter therefore depends on the timing of the calls before
//=======================================================================================================
extern void OS_Scheduler_RunForever(void);

//=======================================================================================================
//  The following extern Tasks_... functions should be placed in the file main/main_tasks.c
//  call OS_Scheduler_Run() in the main loop or call OS_Scheduler_RunForever() in main.c
//=======================================================================================================
extern void Tasks_100us(void);
extern void Tasks_Realtime_100us(void);
extern void Tasks_Realtime_1ms(void);
extern void Tasks_1ms(void);
extern void Tasks_10ms(void);
extern void Tasks_100ms(void);
extern void Tasks_1s(void);
extern void Tasks_Background(void);
extern void TMR1_CallBack(void);
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif              // _OS_SCHEDULER_H_
