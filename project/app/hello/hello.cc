#include "hello.hpp"

#include <stdio.h>
#include "usart.h"

hello::hello()
{
}

hello::~hello()
{
}

void hello::print_hello()
{
    printf("hello class!!!\r\n");
}



/*  */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 重定向printf函数到串口，重定义后可将printf写入串口 */
#if defined   ( __GNUC__ )

    /*** 
     * @description: 
     * @param {char} data
     * @return {*}
     */
    void serial_send(char data)
    {
        /* USART_SR_TC */

        /* while ((USART1->SR & 0x40) == 0);
        USART1->DR = (data & (uint16_t)0x01FF);
        USART1->SR = (0 << 6); */
        
        USART1->SR;
        USART1->DR = (data & (uint16_t)0x01FF);
        while ((USART1->SR & 0x40) == 0);
    }
 
    /*** 
     * @description: 重定向printf函数
     * @param {int} _write
     * @param {char} *ptr
     * @param {int} len
     * @return {*}
     */
    __attribute__((used)) int _write(int fd, char *ptr, int len)
    {
        int i;
        for (i = 0; i < len; i++)
        {
            serial_send(ptr[i]);
        }
        fflush(stdout);
        return len;
    }

#elif defined ( __CC_ARM )
    //加入以下代码,支持printf函数,而不需要选择use MicroLIB	
    #pragma import(__use_no_semihosting)
    //标准库需要的支持函数
    struct __FILE 
    {
        int handle; 
    };
    
    FILE __stdout;
    //定义_sys_exit()以避免使用半主机模式  
    void _sys_exit(int x) 
    {
        x = x; 
    }
    //解决HAL库使用时,某些情况可能报错的bug
    void _ttywrch(int ch)
    {
        ch = ch;
    }
    //以上的代码为关闭半主机模式
    
    
    int fputc(int ch, FILE *f)
    {
        while ((USART1->SR & 0x40) == 0);
        USART1->DR = (ch & (uint16_t)0x01FF);
        // USART1->SR = (0 << 6);
        return (ch);
    }

#endif  /* __CC_ARM  __GNUC__ */


 
#ifdef __cplusplus 
}
#endif /* __cplusplus */

