#include "gd32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "bsp_system.h"
#include "wifi.h"
#include "bsp_ds18b20.h"

//void uart_receive_input(unsigned char value)
//void all_data_update(void)
//void uart_transmit_output(unsigned char value)
//void uart_receive_buff_input(unsigned char value[], unsigned short data_len)
//void wifi_test_result(unsigned char result,unsigned char rssi)
//void wifi_uart_service(void)
//void wifi_protocol_init(void)


char info[]="ver=1.0\r\n";
char info_0[]="GD32 T&M \r\n";
char info_1[]="GD32 usart0 printf!\r\n";


void sys_para_init(void)
{
    s_para.WriteFlag=1;
    s_para.Sys_status=0;
    s_para.sys_time=0;
    s_para.WifiStatus=LINK_WIFI;
}

uint32_t i;
int main(void)
{
    sys_para_init();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);//�����ж����ȼ���   
    bsp_led_init();//ledָʾ�Ƴ�ʼ��
    bsp_key_init();//������ʼ��-����Ϊ�ж�ģʽ
    bsp_timer_init();//��ʱ����ʼ��
    bsp_timer3_init();//�ӳٶ�ʱ����ʼ��
    bsp_usart_init();//���ڳ�ʼ��
    bsp_ds18b20_init();
    bsp_usart_printf(info);//���ڴ�ӡ��Ϣ
    bsp_usart_printf(info_0);//���ڴ�ӡ��Ϣ
    bsp_usart_printf(info_1);//���ڴ�ӡ��Ϣ
    wifi_protocol_init();
    Timer_1_Add_Fun(500,led_mode);
    Timer_1_Add_Fun(1000,wifi_uart_service);
    Timer_1_Add_Fun(1000,sys_func);
    
    Timer_1_Add_Fun(1500,ds18b20_readTemp);
    while(1)
    {
        Timer1_SYS_APP_LOOP();
    }  
}
