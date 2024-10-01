
//=======================================================================================================
// @file os_sys_time.h
//
// @brief contains the function for the system time
//
// @note    counting up the time can/should be done in the interrupt routine of the scheduler
//          reading the time can also be done outside of the interrupt without any conflicts
//
// @author M52409
//
// @date 2019-08-09
//=======================================================================================================

#ifndef OS_SYS_TIME_H
#define	OS_SYS_TIME_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct
{
    uint16_t    millisecond;
    uint8_t     second;
    uint8_t     minute;
    uint8_t     hour;
} OS_SYSTIME_t;


//=======================================================================================================
// @brief   call this function every millisecond from the interrupt in your scheduler
//=======================================================================================================
void OS_SysTime_IncrementTime_1ms(void);

//=======================================================================================================
// @brief   function to reset the time to zero
// @ note   use this function at boot up time or in the init function of your scheduler
//=======================================================================================================
void OS_SysTime_ResetTime(void);

//=======================================================================================================
// @brief   function to reset the time to zero
// @ note   use this function at boot up time or in the init function of your scheduler
//=======================================================================================================
void OS_SysTime_GetTime(OS_SYSTIME_t* retVal);






#ifdef	__cplusplus
}
#endif  // __cplusplus
#endif	// OS_SYS_TIME_H

