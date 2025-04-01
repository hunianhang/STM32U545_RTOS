/*
 * print_task.c
 *
 *  Created on: Mar 26, 2025
 *      Author: xiang
 */
/* Private Definitions */
#include "uart_task.h"
#include "usart.h"
#include "debug.h"
#include "task_manager.h"

#define thread1_STACK_SIZE 1024
#define thread2_STACK_SIZE 1024

/* Private Variables */

static TX_THREAD my_thread_1;
static TX_THREAD my_thread_2;
static uint8_t thread1_stack[thread1_STACK_SIZE];
static uint8_t thread2_stack[thread2_STACK_SIZE];

//static uint8_t pData1[] = "I am thread1";
//static uint8_t pData2[] = "I am thread2";

static void thread1_entry(ULONG entry_input);
static void thread2_entry(ULONG entry_input);
/* Function Implementations */
void Print_Task_Init(void)
{
    /* Create first task */
	tx_thread_create(
			&my_thread_1,
			"my_thread1",
			thread1_entry,
			0,
			thread1_stack,
			thread1_STACK_SIZE,
			3,
			3,
			TX_NO_TIME_SLICE,
			TX_AUTO_START
			);


	tx_thread_create(
			&my_thread_2,
			"my_thrread2",
			thread2_entry,
			0,
			thread2_stack,
			thread2_STACK_SIZE,
			1,
			1,
			TX_NO_TIME_SLICE,
			TX_AUTO_START
			);
}

static void thread1_entry(ULONG entry_input)
{
	while(1)
	{
		Debug_Println("Hello thread 1");
//		HAL_UART_Transmit(&huart1, pData1, sizeof(pData1), HAL_MAX_DELAY);
		tx_thread_sleep(1000);
	}

}

static void thread2_entry(ULONG entry_input)
{
	while(1)
	{
		Debug_Println("Hello thread 2");
//		HAL_UART_Transmit(&huart1, pData2, sizeof(pData2), HAL_MAX_DELAY);
		tx_thread_sleep(500);
	}
}

void Print_Task_DeInit(void)
{
    /* Delete Thread */
	tx_thread_delete(&my_thread_1);
    tx_thread_delete(&my_thread_2);
}
