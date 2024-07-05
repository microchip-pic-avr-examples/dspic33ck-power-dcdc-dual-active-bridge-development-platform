

#ifndef OS_RESET_H
#define	OS_RESET_H
#include "stdbool.h"

typedef struct
{
    bool reset_by_watchdog :1;	// Bit  0: 1, if the last reset was caused by the watchdog
    unsigned : 5;                              // Bit <3:7> (reserved)
} OS_RESET_RESETCAUSEBITS_t;

/* OS_RESETTHINGY is not defined and hence a violation is reported */
/* LDRA_EXCLUDE 337 S */
#if OS_RESETTHINGY == 1
void OS_Reset_Init(void);
OS_RESET_RESETCAUSEBITS_t OS_Reset_GetResetCause(void);
#endif /* OS_RESETTHINGY */

#endif /* OS_RESET_H */
