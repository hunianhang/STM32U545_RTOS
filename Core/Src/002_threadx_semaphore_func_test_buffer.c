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

#define SEMAPHORE_THREAD_STACK_SIZE 1024
#define SEMAPHORE_THREAD_PRIORITY 	15
#define BUFFER_SIZE 				5

static TX_THREAD producer_thread;
static TX_THREAD consumer_thread;

static uint8_t producer_stack[SEMAPHORE_THREAD_STACK_SIZE];
static uint8_t consumer_stack[SEMAPHORE_THREAD_STACK_SIZE];

static TX_SEMAPHORE sem_empty;
static TX_SEMAPHORE sem_full;
static TX_MUTEX 	buffer_mutex;

static uint32_t buffer[BUFFER_SIZE];

static uint32_t in = 0;
static uint32_t out = 0;
static uint32_t data_counter = 0;

static void producer_thread_entry(ULONG thread_input);
static void consumer_thread_entry(ULONG thread_input);


void Semaphore_Func_Test_Buffer(void)
{
	UINT status;

	tx_semaphore_create(&sem_empty,"Empty Slots", BUFFER_SIZE);
	tx_semaphore_create(&sem_full, "Full Slots",0);
	tx_mutex_create(&buffer_mutex, "Buffer Mutex",TX_NO_INHERIT);

	status = tx_thread_create(&producer_thread, "Producer Thread",
			producer_thread_entry, 0,
			producer_stack, SEMAPHORE_THREAD_STACK_SIZE,
			SEMAPHORE_THREAD_PRIORITY, SEMAPHORE_THREAD_PRIORITY,
			TX_NO_TIME_SLICE, TX_AUTO_START);

	if(status != SUCCESS)
	{
		Debug_Println("Producer thread creation failed!");
		return ;
	}

	status = tx_thread_create(&consumer_thread, "Consumer Thread",
			consumer_thread_entry, 0,
			consumer_stack, SEMAPHORE_THREAD_STACK_SIZE,
			SEMAPHORE_THREAD_PRIORITY, SEMAPHORE_THREAD_PRIORITY,
			TX_NO_TIME_SLICE, TX_AUTO_START);

	if(status != TX_SUCCESS)
	{
		Debug_Println("Consumer thread creation failed!");
		return;
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

		Debug_Println("Produced--->: Buffer[%lu] = %lu", in, buffer[in]);
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
		Debug_Println("Consumed: buffer[%lu] = %lu", out, value);

		out = (out + 1) % BUFFER_SIZE;
		tx_mutex_put(&buffer_mutex);
		tx_semaphore_put(&sem_empty);
		tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 2);
	}
}






























