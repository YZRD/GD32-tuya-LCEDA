#include "gd32f10x.h"
#include "bsp_key.h"

uint8_t keyFlage;

/*******************************************
函数名：bsp_key_init
作  者：WZ
时  间：2021.2.22
功  能：按键 引脚初始化
*******************************************/
void bsp_key_init(void)
{
    /* enable the button clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    /* configure button pin as input */ 
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    
    
    /* enable and set key EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_5);

    /* configure key EXTI line */
    exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_5);
}

