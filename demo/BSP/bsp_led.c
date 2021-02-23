#include "bsp_led.h"
#include "gd32f10x.h"
#include "bsp_usart.h"
#include "bsp_system.h"

/*******************************************
��������bsp_led_init
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ�led���ų�ʼ��
*******************************************/
void bsp_led_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure led GPIO port */ 
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
}

/*******************************************
��������bsp_led_on
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ�����led
*******************************************/
void bsp_led_on(void)
{
    gpio_bit_reset(GPIOB,GPIO_PIN_12);
}

/*******************************************
��������bsp_led_off
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ��ر�led
*******************************************/
void bsp_led_off(void)
{
    gpio_bit_set(GPIOB,GPIO_PIN_12);
}


/*******************************************
��������bsp_led_off
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ��ر�led
*******************************************/
void bsp_led_toggle(void)
{
    if(gpio_output_bit_get(GPIOB,GPIO_PIN_12)==SET)
    {
        bsp_led_on();
    }
    else
    {
        bsp_led_off();
    }
}
/*******************************************
��������led_mode
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ�led���ų�ʼ��
*******************************************/
void led_mode(void)
{
    switch(s_para.WifiStatus)
    {
        case 0:
            bsp_led_on();
            break;
        case 1:
            bsp_led_off();
            break;
        case 2:
            bsp_led_toggle();
            break;
        default:
            break;
    }
}

