#ifndef __LED_H__
#define __LED_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#define LED0 PBout(0)   // DS0

void led_on(void);

void led_off(void);

#ifdef __cplusplus
}
#endif


#endif