#ifndef __UART_TASK_H
#define __UART_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tx_api.h"

/* 函数声明 */
void UART_Task_Init(void);
void UART_Task_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_TASK_H */ 