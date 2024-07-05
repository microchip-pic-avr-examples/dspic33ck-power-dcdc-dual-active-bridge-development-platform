
#include "../project_settings.h"
#if OS_FEATURE_WATCHDOG_ENABLED == 1

#if OS_USE_MCC_WATCHDOG == 1
#include "xc.h"     //workaround, mcc watchdog driver does not include that!
#include "../../mcc_generated_files/watchdog.h"
#else

//-------------------------------------------------------------------------------------------------------
// FWDT
//#pragma config RWDTPS = PS2048		// ==> 1,95 sec
//#pragma config RCLKSEL = LPRC       // Watchdog Timer Clock Select bits (Always use LPRC)
//#pragma config WINDIS = ON          // Watchdog Timer Window Enable bit (Watchdog Timer operates in Non-Window mode)
//#pragma config WDTWIN = WIN25       // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
//#pragma config SWDTPS = PS1048576   // Sleep Mode Watchdog Timer Post Scaler select bits (1:1048576)
//#pragma config FWDTEN = ON       // Watchdog Timer Enable bit (WDT enabled in hardware)

#endif

/* The functions in this file write to registers that are related to the 
 WatchDog Timer, and hence a return is not required */
/* LDRA_EXCLUDE 65 D */
void OS_Watchdog_Init(void)
{
#if OS_USE_MCC_WATCHDOG == 1
	WATCHDOG_TimerClear();
	WATCHDOG_TimerSoftwareEnable();
#endif
}

/* 61 D reports that the function needs to be declared static because as per 
 76 D the function is not called in the code being analyzed */
/* LDRA_EXCLUDE 61 D */
/* LDRA_EXCLUDE 76 D */
/* LDRA_EXCLUDE 65 D */
void OS_Watchdog_Enable(void)
{
#if OS_USE_MCC_WATCHDOG == 1
	WATCHDOG_TimerSoftwareEnable();
#endif
}

/* LDRA_EXCLUDE 61 D */
/* LDRA_EXCLUDE 76 D */
/* LDRA_EXCLUDE 65 D */
void OS_Watchdog_Disable(void)
{
#if OS_USE_MCC_WATCHDOG == 1
	WATCHDOG_TimerSoftwareDisable();
#endif
}

/* LDRA_EXCLUDE 65 D */
void OS_Watchdog_KeepAlivePing(void)
{
#if OS_USE_MCC_WATCHDOG == 1
	WATCHDOG_TimerClear();
#endif
}

#endif
