#include "gd32f10x.h"
#include "bsp_usart.h"
#include <stdio.h>


/*******************************************
��������bsp_usart_init
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ����� ��ʼ��
*******************************************/
void bsp_usart_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 9600U);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    
    /* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 1);
    
    /* enable USART TBE interrupt */  
    //usart_interrupt_enable(USART1, USART_INT_TBE);
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    
    usart_enable(USART1);
    
    
    
    
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}
/*******************************************
��������bsp_usart_printf
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ��������ݴ�ӡ
*******************************************/
void bsp_usart_printf(char *str)
{
    printf("%s",str);
}
/*******************************************
��������Uart_PutChar
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ����ڷ���������MCU����
*******************************************/
void Uart_PutChar(unsigned char value)
{
    usart_data_transmit(USART1,value);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TC));
}

