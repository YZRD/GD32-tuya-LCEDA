#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "gd32f10x.h"

void bsp_led_init(void);//led ��ʼ��
void bsp_led_on(void);//����Led
void bsp_led_off(void);//�ر�Led
void bsp_led_toggle(void);
void led_mode(void);


#endif

