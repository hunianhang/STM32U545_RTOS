#include "debug.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* 私有变量定义 */
static char debug_buffer[256];

/* 函数实现 */
void Debug_Print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(debug_buffer, sizeof(debug_buffer), format, args);
    va_end(args);
    
    HAL_UART_Transmit(&huart1, (uint8_t*)debug_buffer, strlen(debug_buffer), HAL_MAX_DELAY);
}

void Debug_Println(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(debug_buffer, sizeof(debug_buffer), format, args);
    va_end(args);
    
    strcat(debug_buffer, "\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)debug_buffer, strlen(debug_buffer), HAL_MAX_DELAY);
} 