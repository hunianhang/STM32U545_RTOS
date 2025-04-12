/*
 * 002_threadx_semaphore_func_test.c
 *
 *  Created on: Apr 12, 2025
 *      Author: xiang
 */

#include "002_threadx_semaphore_func_test.h"
#include "debug.h"
#include "task_manager.h"
#include "tx_api.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SEMAPHORE_THREAD_STACK_SIZE 1024
#define SEMAPHORE_THREAD_PRIORITY   15

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TX_THREAD producer_thread;
static TX_THREAD consumer_thread;
static TX_SEMAPHORE my_semaphore;
static uint8_t producer_stack[SEMAPHORE_THREAD_STACK_SIZE];
static uint8_t consumer_stack[SEMAPHORE_THREAD_STACK_SIZE];
static uint32_t shared_data = 0;

/* Private function prototypes -----------------------------------------------*/
static void producer_thread_entry(ULONG thread_input);
static void consumer_thread_entry(ULONG thread_input);

/* Function Implementations --------------------------------------------------*/
void Semaphore_Func_Test(void)
{
    UINT status;
    
    /* Create a semaphore */
    status = tx_semaphore_create(&my_semaphore, "My Semaphore", 0);
    if (status != TX_SUCCESS)
    {
        Debug_Println("Semaphore creation failed!");
        return;
    }
    
    /* Create producer thread */
    status = tx_thread_create(&producer_thread, "Producer Thread",
                             producer_thread_entry, 0,
                             producer_stack, SEMAPHORE_THREAD_STACK_SIZE,
                             SEMAPHORE_THREAD_PRIORITY, SEMAPHORE_THREAD_PRIORITY,
                             TX_NO_TIME_SLICE, TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        Debug_Println("Producer thread creation failed!");
        return;
    }
    
    /* Create consumer thread */
    status = tx_thread_create(&consumer_thread, "Consumer Thread",
                             consumer_thread_entry, 0,
                             consumer_stack, SEMAPHORE_THREAD_STACK_SIZE,
                             SEMAPHORE_THREAD_PRIORITY, SEMAPHORE_THREAD_PRIORITY,
                             TX_NO_TIME_SLICE, TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        Debug_Println("Consumer thread creation failed!");
        return;
    }
    
    Debug_Println("Semaphore test started!");
}

/* Producer thread entry function */
static void producer_thread_entry(ULONG thread_input)
{
    while (1)
    {
        /* Increment shared data */
        shared_data++;
        
        /* Print producer message */
        Debug_Println("Producer: Data = %lu", shared_data);
        
        /* Release semaphore */
        tx_semaphore_put(&my_semaphore);
        
        /* Wait for 1 second */
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
    }
}

/* Consumer thread entry function */
static void consumer_thread_entry(ULONG thread_input)
{
    UINT status;
    
    while (1)
    {
        /* Wait for semaphore */
        status = tx_semaphore_get(&my_semaphore, TX_WAIT_FOREVER);
        
        if (status == TX_SUCCESS)
        {
            /* Print consumer message */
            Debug_Println("Consumer: Received data = %lu", shared_data);
        }
        else
        {
            Debug_Println("Consumer: Semaphore get failed!");
        }
    }
}











































