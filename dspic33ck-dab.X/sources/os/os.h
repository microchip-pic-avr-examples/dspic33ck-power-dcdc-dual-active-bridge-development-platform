
//=======================================================================================================
// @file os.h
//
// @brief includes all the other header files that are necessary for the project
//
// @note adjust the options of the operating system according your needs in main/project_settings.h
// @note call the Init function in your main.c
// @note call the OS_Scheduler_Run in your main loop or call the RunForever-Function in your main.c
//
// @version v1.0
// @date 2019-08-02
// @author M52409
//
//=======================================================================================================
#ifndef OS_H
#define OS_H

#include "os_config.h" 
#include "os_scheduler.h"
        
//=======================================================================================================
//  @brief  Initializes all Operating System Components
//  @note   call this function in your main routine before calling the RunForever function
//=======================================================================================================
extern void OS_Init(void);


#endif              // OS_H
