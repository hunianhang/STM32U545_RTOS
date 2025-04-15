/*
 * 003_ISR_Semphore_Test.c
 *
 *  Created on: Apr 12, 2025
 *      Author: xiang
 */


#include "tx_api.h"
#include "debug.h"

#define STACK_SIZE 1024
#define THREAD_PRIO 10

TX_THREAD isr_trigger_thread;
TX_THREAD consumer_thread;
TX_SEMAPHORE isr_semaphore;

uint8_t isr_thread_stack[STACK_SIZE];
uint8_t consumer_thread_stack[STACK_SIZE];

void Simulate_ISR(void)
{
	UINT old_posture;
	old_posture = tx_interrupt_control(TX_INT_DISABLE);
	tx_semaphore_put(&isr_semaphore);
	tx_interrupt_control(old_posture);
	Debug_Println("ISR:Semaphore released.");
}

void isr_trigger_thread_entry(ULONG thread_input)
{
	while(1)
	{
		tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 3);
		Simulate_ISR();
	}
}

void consumer_thread_entry(ULONG thread_input)
{
	UINT status;
	while(1)
	{
		status = tx_semaphore_get(&isr_semaphore, TX_WAIT_FOREVER);

		if(status == TX_SUCCESS)
		{
			Debug_Println("Consumer: Woken up by ISR.");
			/* Simulator handle the task... */
		}
		else
		{
			Debug_Println("Consumer: Failed to get semaphore!");
		}
	}
}

void ISR_Simulation_Test(void)
{
	UINT status;

	status = tx_semaphore_create(&isr_semaphore, "ISR Semaphore", 0);
	if(status != TX_SUCCESS)
	{
		Debug_Println("Semaphore Creation Failed!");
		return;
	}

	status = tx_thread_create(&consumer_thread, "Consumer Thread",
			consumer_thread_entry,0,
			consumer_thread_stack, STACK_SIZE,
			THREAD_PRIO, THREAD_PRIO,
			TX_NO_TIME_SLICE,TX_AUTO_START
		);
	if(status != TX_SUCCESS)
	{
		Debug_Println("Consumer thread creation failed!");
		return;
	}

	status = tx_thread_create(&isr_trigger_thread,"ISR Trigger Thread",
			isr_trigger_thread_entry, 0,
			isr_thread_stack, STACK_SIZE,
			THREAD_PRIO + 1, THREAD_PRIO + 1,
			TX_NO_TIME_SLICE, TX_AUTO_START
		);
	if(status != TX_SUCCESS)
	{
		Debug_Println("ISR Trigger thread creation failed!");
		return;
	}

	Debug_Println("ISR simulation test started.");
}

