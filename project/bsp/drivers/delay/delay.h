#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "sys.h"

#if (SYSTEM_SUPPORT_OS == 0)

    void delay_xus(volatile unsigned int xus);

    void delay_xms(volatile unsigned int xms);

    void Delay_us(uint32_t us);

    void Delay_ms(uint32_t ms);

    void Delay_s(uint32_t s);

#elif (SYSTEM_SUPPORT_OS == 1)


#endif


void delay_ms(uint32_t ms);

void delay_us(uint32_t us);


#ifdef __cplusplus
}
#endif

#endif

