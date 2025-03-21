#include "uart_task.h"
#include "usart.h"
#include "debug.h"
#include "task_manager.h"

/* Private Definitions */
#define UART_RX_BUFFER_SIZE 64
#define UART_TX_BUFFER_SIZE 64

/* Private Variables */
static uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
static uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
static TX_THREAD uart_thread;
static TX_SEMAPHORE uart_rx_semaphore;

/* Private Function Declarations */
static void UART_Thread_Entry(ULONG thread_input);

/* Function Implementations */
void UART_Task_Init(void)
{
    /* Create UART Receive Semaphore */
    tx_semaphore_create(&uart_rx_semaphore, "UART_RX_SEM", 0);

    /* Create UART Thread */
    tx_thread_create(&uart_thread, "UART_THREAD", UART_Thread_Entry, 0,
                    uart_tx_buffer, TASK_STACK_SIZE_NORMAL,
                    TASK_PRIORITY_NORMAL, TASK_PRIORITY_NORMAL, TX_NO_TIME_SLICE, TX_AUTO_START);

    /* Start UART Receive Interrupt */
    HAL_UART_Receive_IT(&huart1, uart_rx_buffer, 1);
}

void UART_Task_DeInit(void)
{
    /* Stop UART Receive Interrupt */
    HAL_UART_AbortReceive_IT(&huart1);
    
    /* Delete UART Thread */
    tx_thread_delete(&uart_thread);
    
    /* Delete UART Receive Semaphore */
    tx_semaphore_delete(&uart_rx_semaphore);
}

/* UART Receive Interrupt Callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* Send Semaphore to Notify Receive Thread */
        tx_semaphore_put(&uart_rx_semaphore);
        /* Restart Receive Interrupt */
        HAL_UART_Receive_IT(&huart1, uart_rx_buffer, 1);
    }
}

/* UART Thread Entry Function */
static void UART_Thread_Entry(ULONG thread_input)
{
    uint8_t rx_data;
    uint32_t status;

    while (1)
    {
        /* Wait for Received Data */
        status = tx_semaphore_get(&uart_rx_semaphore, TX_WAIT_FOREVER);
        if (status == TX_SUCCESS)
        {
            /* Get Received Data */
            rx_data = uart_rx_buffer[0];
            
            /* Echo Received Data */
            Debug_Println("Received: %c", rx_data);
            
            /* Send Data */
            HAL_UART_Transmit(&huart1, &rx_data, 1, HAL_MAX_DELAY);
        }
    }
} 