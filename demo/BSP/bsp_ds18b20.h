#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H
#include "gd32f10x.h"

extern int16_t ds18b20_reg;

void bsp_ds18b20_init(void);


void ds18b20_readTemp(void);

#endif
