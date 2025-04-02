#include "001_threadx_task_func_test.h"
#include "debug.h"
#include "task_manager.h"

/* Define constants */
#define THREAD1_STACK_SIZE     2048
#define THREAD2_STACK_SIZE     2048
#define THREAD1_PRIORITY       5
#define THREAD2_PRIORITY       6
#define THREAD1_SLEEP_TIME     100
#define THREAD2_SLEEP_TIME     500

/* Thread state enumeration */
typedef enum {
    THREAD_STATE_INIT = 0,
    THREAD_STATE_RUNNING,
    THREAD_STATE_SUSPENDED,
    THREAD_STATE_TERMINATED
} ThreadState_t;

/* Thread control structure */
typedef struct {
    TX_THREAD thread;
    uint8_t stack[THREAD1_STACK_SIZE];
    ThreadState_t state;
    const char* name;
} ThreadControl_t;

/* Static variable definitions */
static ThreadControl_t thread1_ctrl = {
    .state = THREAD_STATE_INIT,
    .name = "Thread1"
};

static ThreadControl_t thread2_ctrl = {
    .state = THREAD_STATE_INIT,
    .name = "Thread2"
};

/* Function declarations */
static void thread1_entry(ULONG entry_input);
static void thread2_entry(ULONG entry_input);
static void thread_entry_exit_notify(TX_THREAD *thread_ptr, UINT condition);
static UINT create_thread(ThreadControl_t* ctrl, void (*entry)(ULONG), UINT priority);
static void handle_thread_state_change(ThreadControl_t* ctrl, ThreadState_t new_state);

/* Thread creation function */
static UINT create_thread(ThreadControl_t* ctrl, void (*entry)(ULONG), UINT priority)
{
    UINT status;

    status = tx_thread_create(&ctrl->thread,
                            ctrl->name,
                            entry,
                            0,
                            ctrl->stack,
                            THREAD1_STACK_SIZE,
                            priority,
                            priority,
                            TX_NO_TIME_SLICE,
                            TX_AUTO_START);

    if (status == TX_SUCCESS)
    {
        ctrl->state = THREAD_STATE_RUNNING;
        Debug_Println("%s created successfully", ctrl->name);
    }
    else
    {
        Debug_Println("%s creation failed: %d", ctrl->name, status);
    }

    return status;
}

/* Thread state change handler */
static void handle_thread_state_change(ThreadControl_t* ctrl, ThreadState_t new_state)
{
    ctrl->state = new_state;
    Debug_Println("%s state changed to: %d", ctrl->name, new_state);
}

/* Thread1 entry function */
static void thread1_entry(ULONG entry_input)
{
    INT count = 0;

    while(1)
    {
        Debug_Println("1->my_thread1");

        if(count == 0)
        {
            Debug_Println("1->start now");
        }

        count++;

        if (tx_thread_sleep(THREAD1_SLEEP_TIME) != TX_SUCCESS)
        {
            Debug_Println("Thread1 sleep failed");
            break;
        }
    }
}

/* Thread2 entry function */
static void thread2_entry(ULONG entry_input)
{
    INT count = 0;

    while(1)
    {
        Debug_Println("2->I am thread2");

        switch(count)
        {
            case 3:
                Debug_Println("1->suspend");
                if (tx_thread_suspend(&thread1_ctrl.thread) == TX_SUCCESS)
                {
                    handle_thread_state_change(&thread1_ctrl, THREAD_STATE_SUSPENDED);
                }
                break;

            case 6:
                Debug_Println("1->resume");
                if (tx_thread_resume(&thread1_ctrl.thread) == TX_SUCCESS)
                {
                    handle_thread_state_change(&thread1_ctrl, THREAD_STATE_RUNNING);
                }
                break;

            case 9:
                Debug_Println("1->terminate");
                if (tx_thread_terminate(&thread1_ctrl.thread) == TX_SUCCESS)
                {
                    handle_thread_state_change(&thread1_ctrl, THREAD_STATE_TERMINATED);
                }
                break;

            case 12:
                Debug_Println("1->before reset");
                if (tx_thread_reset(&thread1_ctrl.thread) == TX_SUCCESS)
                {
                    if (tx_thread_resume(&thread1_ctrl.thread) == TX_SUCCESS)
                    {
                        handle_thread_state_change(&thread1_ctrl, THREAD_STATE_RUNNING);
                        Debug_Println("1->after reset");
                    }
                }
                break;

            case 14:
                Debug_Println("Terminating all threads...");
                if (tx_thread_terminate(&thread1_ctrl.thread) == TX_SUCCESS)
                {
                    handle_thread_state_change(&thread1_ctrl, THREAD_STATE_TERMINATED);
                }
                if (tx_thread_terminate(&thread2_ctrl.thread) == TX_SUCCESS)
                {
                    handle_thread_state_change(&thread2_ctrl, THREAD_STATE_TERMINATED);
                }
                break;
        }

        count++;
        if (tx_thread_sleep(THREAD2_SLEEP_TIME) != TX_SUCCESS)
        {
            Debug_Println("Thread2 sleep failed");
            break;
        }
    }
}

/* Thread entry/exit notification function */
static void thread_entry_exit_notify(TX_THREAD *thread_ptr, UINT condition)
{
    ThreadControl_t* ctrl = NULL;

    /* Determine which thread */
    if (thread_ptr == &thread1_ctrl.thread)
    {
        ctrl = &thread1_ctrl;
    }
    else if (thread_ptr == &thread2_ctrl.thread)
    {
        ctrl = &thread2_ctrl;
    }

    if (ctrl != NULL)
    {
        if (condition == TX_THREAD_ENTRY)
        {
            Debug_Println("%s entry", ctrl->name);
        }
        else if (condition == TX_THREAD_EXIT)
        {
            Debug_Println("%s exit", ctrl->name);
        }
    }
}

/* Initialization function */
UINT Threadx_Task_Func_Test_Init(void)
{
    UINT status;

    /* Create Thread1 */
    status = create_thread(&thread1_ctrl, thread1_entry, THREAD1_PRIORITY);
    if (status != TX_SUCCESS)
    {
        return status;
    }

    /* Create Thread2 */
    status = create_thread(&thread2_ctrl, thread2_entry, THREAD2_PRIORITY);
    if (status != TX_SUCCESS)
    {
        return status;
    }

    /* Set thread notifications */
    status = tx_thread_entry_exit_notify(&thread1_ctrl.thread, thread_entry_exit_notify);
    if (status != TX_SUCCESS)
    {
        Debug_Println("Thread1 notification setup failed");
        return status;
    }

    status = tx_thread_entry_exit_notify(&thread2_ctrl.thread, thread_entry_exit_notify);
    if (status != TX_SUCCESS)
    {
        Debug_Println("Thread2 notification setup failed");
        return status;
    }

    return TX_SUCCESS;
}
