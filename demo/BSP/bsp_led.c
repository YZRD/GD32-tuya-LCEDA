#include "bsp_led.h"
#include "gd32f10x.h"
#include "bsp_usart.h"
#include "bsp_system.h"

/*******************************************
函数名：bsp_led_init
作  者：WZ
时  间：2021.2.22
功  能：led引脚初始化
*******************************************/
void bsp_led_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure led GPIO port */ 
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
}

/*******************************************
函数名：bsp_led_on
作  者：WZ
时  间：2021.2.22
功  能：点亮led
*******************************************/
void bsp_led_on(void)
{
    gpio_bit_reset(GPIOB,GPIO_PIN_12);
}

/*******************************************
函数名：bsp_led_off
作  者：WZ
时  间：2021.2.22
功  能：关闭led
*******************************************/
void bsp_led_off(void)
{
    gpio_bit_set(GPIOB,GPIO_PIN_12);
}


/*******************************************
函数名：bsp_led_off
作  者：WZ
时  间：2021.2.22
功  能：关闭led
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
函数名：led_mode
作  者：WZ
时  间：2021.2.22
功  能：led引脚初始化
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

