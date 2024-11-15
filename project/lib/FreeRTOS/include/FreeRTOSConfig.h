/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stdint.h"

/***************************************************************************************************************/
/*                                FreeRTOS 对 newlib 的支持                                               */
/***************************************************************************************************************/
#define configUSE_NEWLIB_REENTRANT 0


// #define configENABLE_FPU                            1

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* 是否启用抢占式调度。1 表示启用，0 表示禁用。抢占式调度允许任务在执行时被更高优先级的任务抢占。 */
#define configUSE_PREEMPTION		1

/* 是否启用空闲任务挂钩函数。1 表示启用，0 表示禁用。启用后，空闲任务将调用 vApplicationIdleHook 函数。 */
#define configUSE_IDLE_HOOK			0

/* 是否启用时钟滴答挂钩函数。1 表示启用，0 表示禁用。启用后，系统每次时钟滴答都将调用 vApplicationTickHook 函数。 */
#define configUSE_TICK_HOOK			0

/* CPU 的时钟频率，单位 Hz。这个值应该根据实际硬件配置进行设置。 */
// #define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )
extern uint32_t SystemCoreClock;
#define configCPU_CLOCK_HZ          SystemCoreClock

/* FreeRTOS 的时钟节拍率，即每秒中断次数。 */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
// #define configTICK_RATE_HZ			( 1000 )

/* 系统中支持的最大任务优先级数。 */
#define configMAX_PRIORITIES		( 8 )

/* 任务的最小堆栈大小，以字节为单位。 */
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )

/* 系统的总堆大小，以字节为单位。 */
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 128 * 1024 ) )

// #define configIDLE_TASK_STACK_SIZE   (256)


/* 系统中任务名称的最大长度。 */
#define configMAX_TASK_NAME_LEN		( 16 )

/* 系统中是否使用 16 位的时钟节拍计数器。 */
#define configUSE_16_BIT_TICKS		0

/* 空闲任务是否应该主动让步（yield）。1 表示是，0 表示否。 */
#define configIDLE_SHOULD_YIELD		1

/* 启用 FreeRTOS 内建的追踪功能。这将允许你使用 xTaskGetSystemState() 函数来获取系统状态。 */
#define configUSE_TRACE_FACILITY	1
/* 是否启用任务统计数据格式化函数 */
#define configUSE_STATS_FORMATTING_FUNCTIONS    1


#define configSUPPORT_STATIC_ALLOCATION 0

/* 是否启用互斥量 */
#define configUSE_MUTEXES    0
#define INCLUDE_xSemaphoreGetMutexHolder    0



/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		    1
#define INCLUDE_uxTaskPriorityGet		    1
#define INCLUDE_vTaskDelete				    1
#define INCLUDE_vTaskCleanUpResources	    0
#define INCLUDE_vTaskSuspend			    1
#define INCLUDE_vTaskDelayUntil			    1
#define INCLUDE_vTaskDelay				    1

#define INCLUDE_xTaskGetCurrentTaskHandle   1

// #define xPortPendSVHandler PendSV_Handler
// #define xPortSysTickHandler SysTick_Handler
// #define vPortSVCHandler SVC_Handler

void vPortSVCHandler( void );
void xPortPendSVHandler( void );
void xPortSysTickHandler( void );
#define INCLUDE_xTaskGetSchedulerState      1

// #define configUSE_TIMERS    1
// #define configSUPPORT_STATIC_ALLOCATION     1


/***************************************************************************************************************/
/*                                FreeRTOS中断配置选项                                               */
/***************************************************************************************************************/
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS             __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS             4
#endif




/*** 
 * 在往寄存器里面写数值配置的时候，是按照8bit来写的，所以真正写的时候需要经过转换 
 * SysTick的优先级配置为最低,这样可以提高系统的实时响应能力，即其他的外部中断可以及时的得到响应。
 * 
 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY                 15          //设置最低优先级宏
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )        //设置内核(滴答定时器)中断优先级


/*** 
 * 用于配置STM32的特殊寄存器basepri寄存器的值，用于屏蔽中断，当大于basepri值的优先级的中断将被全部屏蔽。basepri只有4bit有效
 * 在FreeRTOS中，关中断是通过配置basepri寄存器来实现的，关掉的中断由配置的basepri的值决定，小于basepri值的中断FreeRTOS是关不掉的
 * 这样做的好处是可以系统设计者可以人为的控制那些非常重要的中断不能被关闭，在紧要的关头必须被响应
 * 
 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY            5           //系统可管理的最高中断优先级
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )   //管理最高中断优先级
        
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#endif /* FREERTOS_CONFIG_H */

