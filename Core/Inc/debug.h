#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* 函数声明 */
void Debug_Print(const char *format, ...);
void Debug_Println(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H */ 