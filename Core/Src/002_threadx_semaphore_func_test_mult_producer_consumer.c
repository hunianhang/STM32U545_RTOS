/*
 * 002_threadx_semaphore_func_test_buffer.c
 *
 *  Created on: Apr 12, 2025
 *      Author: xiang
 */
/*
 * 你想先扩展哪一部分？比如：

加 buffer 实现真实的生产者消费者

多线程竞争模型

模拟 ISR 唤醒任务

自定义场景（比如 LED 控制、按键唤醒）

你说个方向，我来帮你写好起点～
 */

//The producer produces one data per second, located in the plane;
//
//The consumer consumes one data every two seconds;
//
//If the arc is full, the producer will be blocked;
//
//If the coordinate is empty, the consumer will be blocked;
//
//The console can see the reading process of the buffer.

#include "002_threadx_semaphore_func_test.h"
#include "debug.h"
#include "task_manager.h"
#include "tx_api.h"
#include <stdio.h>

#define SEMAPHORE_THREAD_STACK_SIZE 1024
#define SEMAPHORE_THREAD_PRIORITY 	15
#define BUFFER_SIZE 				5

#define PRODUCER_COUNT 3

static TX_THREAD producer_threads[PRODUCER_COUNT];
static uint8_t producer_stacks[PRODUCER_COUNT][SEMAPHORE_THREAD_STACK_SIZE];

#define CONSUMER_COUNT 2

static TX_THREAD consumer_threads[CONSUMER_COUNT];
static uint8_t consumer_stacks[CONSUMER_COUNT][SEMAPHORE_THREAD_STACK_SIZE];



static TX_SEMAPHORE sem_empty;
static TX_SEMAPHORE sem_full;
static TX_MUTEX 	buffer_mutex;

static uint32_t buffer[BUFFER_SIZE];

static uint32_t in = 0;
static uint32_t out = 0;
static uint32_t data_counter = 0;

static void producer_thread_entry(ULONG thread_input);
static void consumer_thread_entry(ULONG thread_input);


void Semaphore_Func_Test_MultThreads(void)
{
	UINT status;

	tx_semaphore_create(&sem_empty,"Empty Slots", BUFFER_SIZE);
	tx_semaphore_create(&sem_full, "Full Slots",0);
	tx_mutex_create(&buffer_mutex, "Buffer Mutex",TX_NO_INHERIT);

	for (int i = 0; i < PRODUCER_COUNT; i++) {
	    char name[32];
	    sprintf(name, "Producer %d", i);
	    status = tx_thread_create(&producer_threads[i], name,
	        producer_thread_entry, i,  // 用 i 作为 thread_input 传进去
	        producer_stacks[i], SEMAPHORE_THREAD_STACK_SIZE,
	        SEMAPHORE_THREAD_PRIORITY, SEMAPHORE_THREAD_PRIORITY,
	        TX_NO_TIME_SLICE, TX_AUTO_START);

		if(status != SUCCESS)
		{
			Debug_Println("Producer thread creation failed!");
			return ;
		}
	}

	if(status != SUCCESS)
	{
		Debug_Println("Producer thread creation failed!");
		return ;
	}

	for (int i = 0; i < CONSUMER_COUNT; i++) {
	    char name[32];
	    sprintf(name, "Consumer %d", i);
	    status = tx_thread_create(&consumer_threads[i], name,
	        consumer_thread_entry, i,
	        consumer_stacks[i], SEMAPHORE_THREAD_STACK_SIZE,
	        SEMAPHORE_THREAD_PRIORITY, SEMAPHORE_THREAD_PRIORITY,
	        TX_NO_TIME_SLICE, TX_AUTO_START);

		if(status != TX_SUCCESS)
		{
			Debug_Println("Consumer thread creation failed!");
			return;
		}
	}



	Debug_Println("Producer-Consumer buffer test start!");
}

static void producer_thread_entry(ULONG thread_input)
{
	while(1)
	{
		tx_semaphore_get(&sem_empty, TX_WAIT_FOREVER);
		tx_mutex_get(&buffer_mutex, TX_WAIT_FOREVER);
		buffer[in] = data_counter++;

		Debug_Println("P->:Buf[%lu]=%lu,Id: %d", out, buffer[in],thread_input);
		in = (in + 1) % BUFFER_SIZE;

		tx_mutex_put(&buffer_mutex);
		tx_semaphore_put(&sem_full);

		tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
	}
}

static void consumer_thread_entry(ULONG thread_input)
{
	while(1)
	{
		tx_semaphore_get(&sem_full,TX_WAIT_FOREVER);
		tx_mutex_get(&buffer_mutex, TX_WAIT_FOREVER);

		uint32_t value = buffer[out];
		Debug_Println("C:buf[%lu]=%lu,Id: %d", out, value,thread_input);

		out = (out + 1) % BUFFER_SIZE;
		tx_mutex_put(&buffer_mutex);
		tx_semaphore_put(&sem_empty);
		tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 2);
	}
}






























