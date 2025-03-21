#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tx_api.h"

/* Task Priority Definitions */
#define TASK_PRIORITY_HIGH     10
#define TASK_PRIORITY_NORMAL   15
#define TASK_PRIORITY_LOW      20

/* Task Stack Size Definitions */
#define TASK_STACK_SIZE_SMALL  512
#define TASK_STACK_SIZE_NORMAL 1024
#define TASK_STACK_SIZE_LARGE  2048

/* Function Declarations */
void Task_Manager_Init(void);
void Task_Manager_Start(void);

#ifdef __cplusplus
}
#endif

#endif /* __TASK_MANAGER_H */ 