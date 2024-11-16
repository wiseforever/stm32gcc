#include "delay.h"


// #define SysTick_CLKSource_HCLK_Div8    ((sys_tick_t)0xFFFFFFFB)
// #define SysTick_CLKSource_HCLK         ((sys_tick_t)0x00000004)
// #define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SysTick_CLKSource_HCLK) || \
//                                        ((SOURCE) == SysTick_CLKSource_HCLK_Div8))


/**
 * @brief  以下为中断的方式进行延时
 */
// #define SystemFrequency 72000000UL

#define SystemFrequency SystemCoreClock

volatile sys_tick_t time_delay; // 延时时间，注意定义为全局变量

//延时xus
void delay_xus(volatile sys_tick_t xus)
{

    // //SYSTICK分频--1us的系统时钟中断
    // while (SysTick_Config(SystemFrequency / 1000000)) {}

    // time_delay = xus;//读取定时时间
    // while(time_delay) {}
    // // while (SysTick_Config(SystemFrequency / 1000)) {}
    // SysTick->CTRL = 0x00; //关闭计数器
    // SysTick->VAL = 0x00; //清空计数器


    sys_tick_t ticks = 0;
    sys_tick_t told = 0;
    sys_tick_t tnow = 0;
    sys_tick_t reload = 0;
    sys_tick_t tcnt = 0;
    if((0x0001&(SysTick->CTRL)) == 0)    //定时器未工作

    reload = SysTick->LOAD;                     //获取重装载寄存器值
    ticks = xus * (SystemCoreClock / 1000000);  //计数时间值(得出滴答定时器计数次数)
    
    // os_suspend_all();//阻止OS调度，防止打断us延时
    told = SysTick->VAL;  //获取当前数值寄存器值（开始时数值
    while(1)
    {
        tnow = SysTick->VAL; //获取当前数值寄存器值
        if(tnow != told)  //当前值不等于开始值说明已在计数
        {         
            if(tnow < told)  //当前值小于开始数值，说明未计到0
                tcnt += told-tnow; //计数值=开始值-当前值

            else     //当前值大于开始数值，说明已计到0并重新计数
                tcnt += reload-tnow+told;   //计数值=重装载值-当前值+开始值  （
                                            //已从开始值计到0

            told = tnow;   //更新开始值
            if(tcnt >= ticks)break;  //时间超过/等于要延迟的时间,则退出.
        }
    }
    // os_resume_all();    //恢复OS调度
}
 
//延时xms
void delay_xms(volatile sys_tick_t xms)
{
    // //SYSTICK分频--1ms的系统时钟中断
    // while (SysTick_Config(SystemFrequency / 1000)) {}

    // time_delay=xms;//读取定时时间
    // while(time_delay) {}
    // SysTick->CTRL = 0x00; //关闭计数器
    // SysTick->VAL = 0x00; //清空计数器

    while(xms--)
    {
        delay_xus(1000);
    }
}



#if (SYSTEM_SUPPORT_OS == 0)

    // 以下为读取计时器的方式
    /**
     * @brief  微秒级延时
     * @param  xus 延时时长，范围：0~4294967295
     * @retval 无
     */
    void Delay_us(sys_tick_t xus)
    {
        SysTick->LOAD = 72 * xus;				//设置定时器重装值
        SysTick->VAL = 0x00;					//清空当前计数值
        SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
        while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
        SysTick->CTRL = 0x00000004;				//关闭定时器
    }

    /**
     * @brief  毫秒级延时
     * @param  xms 延时时长，范围：0~4294967295
     * @retval 无
     */
    void Delay_ms(sys_tick_t xms)
    {
        while(xms--)
        {
            Delay_us(1000);
        }
    }
    
    /**
     * @brief  秒级延时
     * @param  xs 延时时长，范围：0~4294967295
     * @retval 无
     */
    void Delay_s(sys_tick_t xs)
    {
        while(xs--)
        {
            Delay_ms(1000);
        }
    }

#elif (SYSTEM_SUPPORT_OS == 1)

    #ifdef INC_FREERTOS_H
        #include "timers.h"

        void vPortSetupTimerInterrupt( void );

        // void vPortSetupTimerInterrupt(void)
        // {
        // 	/* Configure SysTick to generate interrupts at the desired tick rate */
        // 	SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);

        // 	/* Set SysTick interrupt priority to the lowest level (numerically highest) */
        // 	NVIC_SetPriority(SysTick_IRQn, 15);
        // }


        /* 操作步骤：
        a.根据延时时间和定时器所选时钟频率，计算出定时器要计数的时间数值；
        b.获取当前数值寄存器的数值；
        c.以当前数值为基准开始计数；
        d.当所计数值等于（大于）需要延时的时间数值时退出。
        注：计数时间值的计算，我们以延时10us，时钟频率为72MHZ的STM32F103C8T6来计算，
                计数值 = 延时时间/1S × 时钟频率 = 0.000 01/1 *72 000 000 =  720 */
        void os_delay_xus(sys_tick_t xus)
        { 
            sys_tick_t ticks;
            sys_tick_t told, tnow, reload, tcnt = 0;
            if((0x0001&(SysTick->CTRL)) == 0)    //定时器未工作
                    vPortSetupTimerInterrupt();  //初始化定时器

            reload = SysTick->LOAD;                     //获取重装载寄存器值
            ticks = xus * (SystemCoreClock / 1000000);  //计数时间值(得出滴答定时器计数次数)
            
            os_suspend_all();//阻止OS调度，防止打断us延时
            told = SysTick->VAL;  //获取当前数值寄存器值（开始时数值
            while(1)
            {
                tnow = SysTick->VAL; //获取当前数值寄存器值
                if(tnow != told)  //当前值不等于开始值说明已在计数
                {         
                    if(tnow < told)  //当前值小于开始数值，说明未计到0
                        tcnt += told-tnow; //计数值=开始值-当前值

                    else     //当前值大于开始数值，说明已计到0并重新计数
                        tcnt += reload-tnow+told;   //计数值=重装载值-当前值+开始值  （
                                                    //已从开始值计到0

                    told = tnow;   //更新开始值
                    if(tcnt >= ticks)break;  //时间超过/等于要延迟的时间,则退出.
                }
            }
            os_resume_all();    //恢复OS调度
        }


        // // 定义软件定时器句柄
        // TimerHandle_t xMicrosecondTimer = NULL;

        // // 软件定时器回调函数
        // void vMicrosecondTimerCallback(TimerHandle_t xTimer)
        // {
        //     // 将任务置为就绪状态
        //     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        //     vTaskNotifyGiveFromISR(NULL, &xHigherPriorityTaskWoken);
        //     portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        // }

        // // 微秒级别延时函数
        // void os_delay_xus(sys_tick_t xus)
        // {
        //     // 创建或重置软件定时器
        //     if (xMicrosecondTimer == NULL)
        //     {
        //         xMicrosecondTimer = xTimerCreate("MicrosecondTimer", pdMS_TO_TICKS(1), pdFALSE, NULL, vMicrosecondTimerCallback);
        //     }

        //     // 计算延时时间
        //     const sys_tick_t usDelayTicks = xus / portTICK_PERIOD_MS;
            
        //     // 启动软件定时器
        //     xTimerChangePeriod(xMicrosecondTimer, usDelayTicks, 0);

        //     // 等待任务被软件定时器唤醒
        //     ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        // }

    #endif


#endif


void delay_ms(sys_tick_t ms) {
    #if (SYSTEM_SUPPORT_OS == 0)

        delay_xms(ms);

    #elif (SYSTEM_SUPPORT_OS == 1)
        #ifdef INC_FREERTOS_H

            if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
            {
                vTaskDelay(ms / portTICK_PERIOD_MS);
            }
            else
            {
                delay_xms(ms);
            }
            
        // #elif defined 

        #endif
    #endif
}

void delay_us(sys_tick_t us) {
    #if (SYSTEM_SUPPORT_OS == 0)
        delay_xus(us);
    #elif (SYSTEM_SUPPORT_OS == 1)
        #ifdef INC_FREERTOS_H
            os_delay_xus(us);
        #endif
    #endif
}


