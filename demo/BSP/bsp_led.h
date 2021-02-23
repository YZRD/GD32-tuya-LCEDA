#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "gd32f10x.h"

void bsp_led_init(void);//led 初始化
void bsp_led_on(void);//点亮Led
void bsp_led_off(void);//关闭Led
void bsp_led_toggle(void);
void led_mode(void);


#endif

