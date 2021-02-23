#include "gd32f10x.h"
#include "bsp_system.h"
#include "wifi.h"
#include "bsp_key.h"
#include "bsp_usart.h"

system_para s_para;//ϵͳ����

/*******************************************
��������sys_func
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ�ϵͳ״̬
*******************************************/
void sys_func(void)
{
    static int Sys_pre_status=0;   
    s_para.Sys_status=mcu_get_wifi_work_state();
    if(keyFlage)
    {
        keyFlage=0;
        s_para.Sys_status=WIFI_SATE_UNKNOW;
        s_para.sys_time=0;
    }
    if(Sys_pre_status!=s_para.Sys_status)
    {
        Sys_pre_status=s_para.Sys_status;
        s_para.sys_time=0;
    }
    s_para.sys_time++;
    switch(s_para.Sys_status)
    {
        case SMART_CONFIG_STATE:
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=NO_WIFI;
                bsp_usart_printf("0-SMART_CONFIG_STATE\r\n"); 
            }
            break;
        case AP_STATE:
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=NO_WIFI;
                bsp_usart_printf("1-AP_STATE\r\n"); 
            }
            break;
        case WIFI_NOT_CONNECTED:
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=NO_WIFI;
                bsp_usart_printf("2-WIFI_NOT_CONNECTED\r\n"); 
            }
            break;
        case WIFI_CONNECTED:
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=LINK_WIFI;
                bsp_usart_printf("3-WIFI_CONNECTED\r\n"); 
            }
            break;
        case WIFI_CONN_CLOUD:
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=LINK_WIFI_CLOUD;  
                bsp_usart_printf("4-WIFI_CONN_CLOUD\r\n"); 
            }
            all_data_update(); 
            break;
        case WIFI_LOW_POWER:
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=NO_WIFI;
                bsp_usart_printf("5-WIFI_LOW_POWER\r\n");
            }                
            break;
        case SMART_AND_AP_STATE:
            if(s_para.sys_time==1)
            {
                bsp_usart_printf("6-SMART_AND_AP_STATE\r\n"); 
            }
            break;
        case WIFI_SATE_UNKNOW:           
            if(s_para.sys_time==1)
            {
                s_para.WifiStatus=NO_WIFI;
                bsp_usart_printf("7-WIFI_SATE_UNKNOW\r\n");
                mcu_reset_wifi(); //ģ�鴦��������״̬ʱ������ͨ������WiFiʹ�豸���ڴ�����״̬������WiFi��Ĭ�Ͻ���Smartconfig����״̬��               
                //mcu_set_wifi_mode(SMART_CONFIG);
            }          
            break;
    }
}




