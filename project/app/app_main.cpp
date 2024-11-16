#include "app_main.h"


#include "main.h"
#include "delay.h"
#include "usart.h"



TaskHandle_t test_task_handle;
void test_task(void *param)
{
    while (1)
    {
        log_printf("test task run\r\n");
        delay_ms(500);
    }
}


static void send_byte(const unsigned char c)
{
    USART1->SR;
    USART1->DR = c & (uint16_t)0x01FF;
    while ((USART1->SR & 0x40) == 0);
}

static int send_data(const unsigned char *buf, int len)
{
    /* Sends over a buffer of specified length */
    for (int i = 0; i < len; i++)
    {
        send_byte(buf[i]);
    }
    
    return len;
}


int app_main()
{
    log_param_t log_uart = {
        LOG_LEVEL_ALL, 
        0, 
        20, 80, 6,
        send_data
    };
    log_init(log_uart);

    log_printf("Hardware_init is OK!\r\n");

    xTaskCreate(test_task, "test_task", 512, NULL, 2, &test_task_handle);

    while (1)
    {
        log_printf("Hello World! log_printf!!!\n");
        float temperature = 25.0;
        log_printf("Temperature: %.2f\r\n", temperature);
        delay_ms(1000);
    }
    
    return 0;
}

