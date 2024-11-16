#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "sys.h"

#if (SYSTEM_SUPPORT_OS == 0)

    void delay_xus(volatile sys_tick_t xus);

    void delay_xms(volatile sys_tick_t xms);

    void Delay_us(sys_tick_t us);

    void Delay_ms(sys_tick_t ms);

    void Delay_s(sys_tick_t s);

#elif (SYSTEM_SUPPORT_OS == 1)


#endif


void delay_ms(uint32_t ms);

void delay_us(uint32_t us);


#ifdef __cplusplus
}
#endif

#endif

