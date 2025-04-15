#ifndef __THREADX_SEMAPHORE_FUNC_TEST_H
#define __THREADX_SEMAPHORE_FUNC_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tx_api.h"

/* Exported functions prototypes ---------------------------------------------*/
void Semaphore_Func_Test(void);
void Semaphore_Func_Test_Buffer(void);
void Semaphore_Func_Test_MultThreads(void);
void ISR_Simulation_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* __THREADX_SEMAPHORE_FUNC_TEST_H */ 
