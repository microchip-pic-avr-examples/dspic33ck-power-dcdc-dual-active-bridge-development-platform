/**
 * @file    PBV_DAB_frame_map.h
 * @brief   app PBV DAB Frame map file Example
 * @author  M70027
 * @ingroup app-pbv-dab-map
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef APP_PBV_DAB_FRAME_MAP_H
#define	APP_PBV_DAB_FRAME_MAP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "PBV_interface.h"


/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/
void App_PBV_DAB_Init(void);
void App_PBV_DAB_Task_10ms(void);
void App_PBV_DAB_Task_1s(void);
PBV_Datatype_TX_t * App_PBV_DAB_Get_TX_Log_ptr(void);

#endif	/* APP_PBV_DAB_FRAME_MAP_H */

