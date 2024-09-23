
#include "os_reset.h"
#include "stdint.h"

uint16_t os_resetCause = 0;

/* OS_RESETTHINGY is not defined and hence a violation is reported */
/* LDRA_EXCLUDE 337 S */
#if OS_RESETTHINGY == 1
void OS_Reset_Init(void)
{
	
}

OS_RESET_RESETCAUSEBITS_t OS_Reset_GetResetCause(void);
#endif /* OS_RESETHINGY */
