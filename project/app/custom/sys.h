
#ifndef __SYS_H__
#define __SYS_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "log.h"
#include "sdkconfig.h"
// #include "EZAccessMicro.h"


#define SystemFrequency SystemCoreClock

#define _boundary(in, a, b) ((in <= a) ? (a) : ((in > b) ? (b) : (in)))
#define _inrange(x, min, max) ((x >= min) && (x <= max))

/* 定义系统tick类型 */
typedef uint32_t sys_tick_t;

typedef enum
{
    OS_0MS,
    OS_1MS = 1,
    OS_5MS,
    OS_10MS,
    OS_50MS,
    OS_100MS,
    OS_500MS,
    OS_1000MS
}OSPeriod;


void inc_tick(void);
sys_tick_t custom_tick_get(void);
sys_tick_t sys_tick_get(void);


#if (SYSTEM_SUPPORT_OS == 1)

    #if defined (INC_FREERTOS_H)
        #include "task.h"
        #include "queue.h"
        #include "semphr.h"

        #if __has_include("app_main.h")
            #include "app_main.h"
            extern TaskHandle_t app_main_thread_handle;
            void app_main_thread(void *param);
        #endif

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
    #endif /* INC_FREERTOS_H */

    sys_tick_t os_tick_get();
    void os_suspend( void * task_to_suspend );
    void os_resume( void * task_to_resume );
    void os_suspend_all();
    void os_resume_all();

#endif /* SYSTEM_SUPPORT_OS == 1 */





///////////////////////////////////////////////////////////////////////////////////
//定义一些常用的数据类型短关键字 
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;
 
typedef volatile uint32_t   vu32;
typedef volatile uint16_t   vu16;
typedef volatile uint8_t    vu8; 

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入
////////////////////////////////////////////////////////////////////////////////// 
//Ex_NVIC_Config专用定义
#define GPIO_A              0
#define GPIO_B              1
#define GPIO_C              2
#define GPIO_D              3
#define GPIO_E              4
#define GPIO_F              5
#define GPIO_G              6 
#define GPIO_H              7 
#define GPIO_I              8 

#define FTIR                1       //下降沿触发
#define RTIR                2       //上升沿触发

//GPIO设置专用宏定义
#define GPIO_MODE_IN        0       //普通输入模式
#define GPIO_MODE_OUT       1       //普通输出模式
#define GPIO_MODE_AF        2       //AF功能模式
#define GPIO_MODE_AIN       3       //模拟输入模式

#define GPIO_SPEED_2M       0       //GPIO速度2Mhz
#define GPIO_SPEED_25M      1       //GPIO速度25Mhz
#define GPIO_SPEED_50M      2       //GPIO速度50Mhz
#define GPIO_SPEED_100M     3       //GPIO速度100Mhz

#define GPIO_PUPD_NONE      0       //不带上下拉
#define GPIO_PUPD_PU        1       //上拉
#define GPIO_PUPD_PD        2       //下拉
#define GPIO_PUPD_RES       3       //保留 

#define GPIO_OTYPE_PP       0       //推挽输出
#define GPIO_OTYPE_OD       1       //开漏输出 

//GPIO引脚编号定义
#define PIN0                1<<0
#define PIN1                1<<1
#define PIN2                1<<2
#define PIN3                1<<3
#define PIN4                1<<4
#define PIN5                1<<5
#define PIN6                1<<6
#define PIN7                1<<7
#define PIN8                1<<8
#define PIN9                1<<9
#define PIN10               1<<10
#define PIN11               1<<11
#define PIN12               1<<12
#define PIN13               1<<13
#define PIN14               1<<14
#define PIN15               1<<15 
////////////////////////////////////////////////////////////////////////////////// 
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq);		//系统时钟设置
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq); //时钟初始化  
void Sys_Soft_Reset(void);      							//系统软复位
void Sys_Standby(void);         							//待机模式 	
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);	//设置偏移地址
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);			//设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//设置中断
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);				//外部中断配置函数(只对GPIOA~I)
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);		//GPIO复用功能设置
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);//GPIO设置函数  
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 


#ifdef __cplusplus
}
#endif



#endif  // __SYS_H__

