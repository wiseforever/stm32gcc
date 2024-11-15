#ifndef __SDKCONFIG_H__
#define __SDKCONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


/* #include "EZAccessMicro.h" */

/* 定义用于存储64位整数的类型 */
typedef unsigned long long ull_t;
typedef signed long long ll_t;

typedef unsigned int uint_t;


/* 
0,不支持os
1,支持os 
*/
#define SYSTEM_SUPPORT_OS     0

#if (SYSTEM_SUPPORT_OS == 0)

    #if __has_include("app_main.h")
        #include "app_main.h"
    #endif

#elif (SYSTEM_SUPPORT_OS == 1)

    #include "FreeRTOS.h"

#endif

#if defined (INC_FREERTOS_H)
    #include "task.h"
    #include "queue.h"
    #include "semphr.h"

    #if __has_include("app_main.h")
        #include "app_main.h"
        #define APP_MAIN_TASK_STACK_SIZE    ( 1024 )
        #define APP_MAIN_TASK_PRIORITY      ( configMAX_PRIORITIES / 2 )
        extern TaskHandle_t app_main_task_handle;
        void app_main_task(void *param);
    #endif

    #define os_tick_get()                   xTaskGetTickCount()
    #define os_suspend( xTaskToSuspend )    vTaskSuspend( xTaskToSuspend ) 
    #define os_resume( xTaskToResume )      vTaskResume( xTaskToResume )
    #define os_suspend_all()                vTaskSuspendAll()
    #define os_resume_all()                 xTaskResumeAll()

    /*** 
     * @brief: 下面注释一些freertos中的API
     *  taskENTER_CRITICAL_FROM_ISR()
     *  
     */
    // 前两个是任务级的临界段代码保护，后两个是中断级的临界段代码保护。
    // taskENTER_CRITICAL()
    // taskEXIT_CRITICAL()
    // 而且这个中断的优先级一定要低于 configMAX_SYSCALL_INTERRUPT_PRIORITY
    // taskENTER_CRITICAL_FROM_ISR()
    // taskEXIT_CRITICAL_FROM_ISR()。
#endif


#ifdef __cplusplus
}
#endif

#endif /* !__SDKCONFIG_H__ */

