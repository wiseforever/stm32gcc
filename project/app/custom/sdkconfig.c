#include "sdkconfig.h"



#ifdef INC_FREERTOS_H

#if __has_include("iwdg.h")
#include "iwdg.h"
#endif

TaskHandle_t app_main_task_handle;

#if __has_include("iwdg.h")
TaskHandle_t iwdg_task_handle;
void iwdg_task(void *param)
{
    static uint32_t iwdg_ms = 0;

    while(1)
    {
        if(xTaskGetTickCount() - iwdg_ms > 1000) {
            iwdg_ms = xTaskGetTickCount();
            HAL_IWDG_Refresh(&hiwdg);
        }
        delay_ms(10);
    }
}
#endif

void app_main_task(void *param) {
    #if __has_include("iwdg.h")
        xTaskCreate(iwdg_task, "iwdg_task", 512, NULL, 3, &iwdg_task_handle);
    #endif

    app_main();

    vTaskDelete(NULL);
}

#endif

