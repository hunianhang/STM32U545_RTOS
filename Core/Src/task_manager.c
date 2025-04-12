#include "task_manager.h"
#include "uart_task.h"
#include "debug.h"
#include "print_task.h"
#include "002_threadx_semaphore_func_test.h"

/* Private Variables */
static TX_THREAD task_manager_thread;
static uint8_t task_manager_stack[TASK_STACK_SIZE_NORMAL];

/* Private Function Declarations */
static void Task_Manager_Thread_Entry(ULONG thread_input);

/* Function Implementations */
void Task_Manager_Init(void)
{
    /* Create Task Manager Thread */
    tx_thread_create(&task_manager_thread, "TASK_MANAGER", Task_Manager_Thread_Entry, 0,
                    task_manager_stack, TASK_STACK_SIZE_NORMAL,
                    TASK_PRIORITY_HIGH, TASK_PRIORITY_HIGH, TX_NO_TIME_SLICE, TX_AUTO_START);
}

void Task_Manager_Start(void)
{
    /* Initialize Function Tasks */
//    UART_Task_Init();
    
//    Print_Task_Init();
//	Threadx_Task_Func_Test_Init();
	Semaphore_Func_Test();
    /* Add more task initializations here */
}

/* Task Manager Thread Entry Function */
static void Task_Manager_Thread_Entry(ULONG thread_input)
{
    /* Start All Tasks */
    Task_Manager_Start();
    
    /* Task Manager Thread can add other management functions here */
    while (1)
    {
        /* Add task monitoring, status checking, etc. here */
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
    }
} 
