#include "app_main.h"

/* Standard library header file */
#include <stdint.h>

#include "sys.h"

#include "delay.h"

#include "hello.hpp"




static void Hardware_init(void)
{
   hello h;
   h.print_hello();
}

int app_main(void)
{
    unsigned int app_main_loop_tms = 0;
    int inc = 0;
    
#if (SYSTEM_SUPPORT_OS == 0)

    Hardware_init();

    while (1)
    {
        #if __has_include("iwdg.h")
            #include "iwdg.h"
            HAL_IWDG_Refresh(&hiwdg); // IWDG喂狗
        #endif
        // custom_none_os_task();

        if (sys_tick_get() - app_main_loop_tms > 500)
        {
            /* if (inc++ % 2 == 0)
            {
                led_on();
            }
            else
            {
                led_off();
            } */

            // log_printf("SystemCoreClock: %u Hz, %u MHz\r\n", (unsigned int)SystemCoreClock, (unsigned int)SystemCoreClock / 1000000);
            app_main_loop_tms = sys_tick_get();
        }
    }

#elif (SYSTEM_SUPPORT_OS == 1)

    #if __has_include("iwdg.h")
        xTaskCreate(iwdg_task, "iwdg_task", 512, NULL, 3, &iwdg_task_handle);
    #endif

    /* uint32_t main_task_loop_tms = 0;
    int inc = 0; */

    Hardware_init();
    
    /* float Rldr = 0.88;
    log_printf("Rldr: %.1f\r\n", Rldr); */

   /*  comm_demo(); */
    
    /* xTaskCreate(lcd_task, "lcd_task", 512, NULL, 2, &lcd_task_handle); */

    while (1)
    {
        // // 处理eeprom的写入
        // if(gEep_write) {
        //     eep.WriteBytes((uint8_t *)&para, 0, sizeof(para));
        //     gEep_write = false;
        // }

        // DS1302_ReadTime();                                                      // 读取实时时钟

        // if(!OLED_Status) {
        //     OLED_Init();
        // }

        // sprintf(oled_str, "%0.1f", Rldr);
        // OLED_ShowString(4, 8, oled_str);

        if(sys_tick_get() - app_main_loop_tms > 1000) {
            // log_printf("%d-%02d-%02d  %02d:%02d:%02d\r\n",ds1302.year + 2000,ds1302.month,ds1302.day,
            //                                                 ds1302.hour,ds1302.min,ds1302.sec);
            // log_printf("main_task!\r\n");
            /* if (inc++ % 2 == 0) {
                led_on();
            } else {
                led_off();
            } */

            app_main_loop_tms = sys_tick_get();
        }

        delay_ms(10);
    }

#endif

    return 0;
}