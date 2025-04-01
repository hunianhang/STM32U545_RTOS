#include "001_threadx_task_func_test.h"
//#include "usart.h"
#include "debug.h"
#include "task_manager.h"

#define thread1_STACK_SIZE 1024
#define thread2_STACK_SIZE 1024

static TX_THREAD my_thread_1;
static TX_THREAD my_thread_2;
static uint8_t thread1_stack[thread1_STACK_SIZE];
static uint8_t thread2_stack[thread2_STACK_SIZE];

static uint8_t pData[] = "-------Threadx--------\n";

static uint8_t pData1[] = "I am thread1";
static uint8_t pData2[] = "I am thread2";

static void  thread1_entry(ULONG entry_input);
static void thread2_entry(ULONG entry_input);
static void my_entry_exit_notify(TX_THREAD *thread_ptr, UINT condition);

void Threadx_Task_Func_Test_Init(void)
{
	tx_thread_create(&my_thread_1,
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

	tx_thread_create(&my_thread_2,
			"my_thread2",
			thread2_entry,
			0,
			thread2_stack,
			thread2_STACK_SIZE,
			1,
			1,
			TX_NO_TIME_SLICE,
			TX_AUTO_START
	);

	tx_thread_entry_exit_notify(&my_thread_1, my_entry_exit_notify);
}


static void thread1_entry(ULONG entry_input)
{
	INT count = 0;

	while(1)
	{
		Debug_Println("my_thread1");

		if(count == 0)
		{
			Debug_Println("start now");
		}
		count++;
		tx_thread_sleep(1000);
	}
}


static void thread2_entry(ULONG entry_input)
{
	INT count = 0;
	while(1)
	{
		Debug_Println("I am thread2");

		if(count == 3)
		{
			tx_thread_suspend(&my_thread_1);
		}
		else if(count == 6)
		{
			tx_thread_resume(&my_thread_1);
		}
		else if(count == 9)
		{
			tx_thread_terminate(&my_thread_1);
		}
		else if(count == 12)
		{
			tx_thread_reset(&my_thread_1);
			tx_thread_resume(&my_thread_1);
		}
		else if(count == 13)
		{
			tx_thread_terminate(&my_thread_1);
			tx_thread_terminate(&my_thread_2);
		}
		else
		{
			;
		}
		count++;
		tx_thread_sleep(1000);
	}
}


static void my_entry_exit_notify(TX_THREAD *thread_ptr, UINT condition)
{
	uint8_t entry_data[] = "thread1_entry";
	uint8_t exit_data[] = "thread2_entry";

	if(condition == TX_THREAD_ENTRY)
	{
		Debug_Println("thread1_entry");
	}

	if(condition == TX_THREAD_EXIT)
	{
		Debug_Println("thread2_entry");
	}
}
