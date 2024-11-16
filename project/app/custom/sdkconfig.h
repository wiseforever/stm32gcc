#ifndef __SDKCONFIG_H__
#define __SDKCONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/* 定义用于存储64位整数的类型 */
typedef unsigned long long ull_t;
typedef signed long long ll_t;
typedef unsigned int uint_t;

/* 
0,不支持os
1,支持os 
*/
#define SYSTEM_SUPPORT_OS     1

#ifndef SYSTEM_SUPPORT_OS
    #define SYSTEM_SUPPORT_OS     0
#endif

#if (SYSTEM_SUPPORT_OS == 0)

    #if __has_include("app_main.h")
        #include "app_main.h"
    #endif

#elif (SYSTEM_SUPPORT_OS == 1)

    #if __has_include("FreeRTOS.h")
        #include "FreeRTOS.h"
        #define MAX_TASK_PRIORITIES configMAX_PRIORITIES
    #endif


    #define APP_MAIN_TASK_STACK_SIZE    ( 1024 )
    #define APP_MAIN_TASK_PRIORITY      ( MAX_TASK_PRIORITIES / 2 )

#endif


#ifdef __cplusplus
}
#endif

#endif /* !__SDKCONFIG_H__ */

