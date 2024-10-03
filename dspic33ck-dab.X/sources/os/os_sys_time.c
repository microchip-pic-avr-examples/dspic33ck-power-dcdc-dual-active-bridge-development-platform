
//=======================================================================================================
// @file os_sys_time.c
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

#include <string.h>
#include "os/os_sys_time.h"
#include "os_config.h"

#if OS_USE_SYSTIME == 1

static volatile OS_SYSTIME_t os_systime;

//=======================================================================================================
// @brief   call this function every millisecond from the interrupt in your scheduler
//=======================================================================================================
void OS_SysTime_IncrementTime_1ms(void)
{
    if (os_systime.millisecond < 999U)
    {
        os_systime.millisecond++;
    }
    else
    {
        os_systime.millisecond = 0U;
        if (os_systime.second < 59U)
        { 
            os_systime.second++;
        }
        else
        {
            os_systime.second = 0U;
            if (os_systime.minute < 59U)
            {
                os_systime.minute++;
            }
            else
            {
                os_systime.minute = 0U;
                if (os_systime.hour < 23U)
                {
                    os_systime.hour++;
                }
                else
                {
                    os_systime.hour = 0U;
                }
            }
        }
    }
}


//=======================================================================================================
// @brief   function to reset the time to zero
// @ note   use this function at boot up time or in the init function of your scheduler or
//          whenever you want to set the system time to zero
//=======================================================================================================
void OS_SysTime_ResetTime(void)
{
    os_systime.millisecond = 0U; //starting with milliseconds is important for consistency
    os_systime.second = 0U;
    os_systime.minute = 0U;
    os_systime.hour = 0U;
}


//=======================================================================================================
// @brief   get the time now
// @ note   use this function to get a copy of the system time
//=======================================================================================================
void OS_SysTime_GetTime(OS_SYSTIME_t* retVal)
{
    /* LDRA_EXCLUDE 45 D */
    /* The void* returned by memcpy is not required in this function */
    /* LDRA_EXCLUDE 382 S */
    memcpy (retVal, (void*) &os_systime, sizeof(OS_SYSTIME_t));
    // if the 1ms interrupt changed the system time during this copy operation, the copy is not valid
    // so we have to check for that with a simple while loop and copy again in such a case
    /* We are comparing the right sizes between the structure pointers and the 
     return from memcmp is an int, which is being rightly checked*/
    /* LDRA_EXCLUDE 618 S */
    /* LDRA_EXCLUDE 114 S */
    while (memcmp(retVal, (void*) &os_systime, sizeof(OS_SYSTIME_t) != 0U))
    {
        /* LDRA_EXCLUDE 382 S */
        memcpy (retVal, (void*) &os_systime, sizeof(OS_SYSTIME_t));
    }
}

#endif  //OS_USE_SYSTIME
