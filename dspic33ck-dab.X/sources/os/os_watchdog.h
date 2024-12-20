
#ifndef _OS_WATCHDOG_H
#define _OS_WATCHDOG_H

void OS_Watchdog_Init(void);
void OS_Watchdog_Enable(void);
void OS_Watchdog_Disable(void);
void OS_Watchdog_KeepAlivePing(void);

#endif /* _OS_WATCHDOG_H */
