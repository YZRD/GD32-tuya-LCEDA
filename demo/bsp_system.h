#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

enum WIFISTATUS
{
    NO_WIFI,
    LINK_WIFI,
    LINK_WIFI_CLOUD    
};


typedef struct
{
    char WriteFlag;
    enum WIFISTATUS WifiStatus;
    char Sys_status;
    int sys_time;
}system_para;

extern system_para s_para;

void sys_func(void);

#endif
