#include "bsp_ds18b20.h"
#include "bsp_timer.h"
#include "bsp_usart.h"
#include <string.h>

#define DQ_0()		gpio_bit_reset(GPIOA,GPIO_PIN_4);
#define DQ_1()		gpio_bit_set(GPIOA,GPIO_PIN_4);

/* �ж�DQ�����Ƿ�Ϊ�� */
#define DQ_IS_LOW()	(gpio_input_bit_get(GPIOA, GPIO_PIN_4) == RESET)

/*******************************************
��������bsp_ds18b20_init
��  �ߣ�WZ
ʱ  �䣺2021.2.23
��  �ܣ�ds18b20���ų�ʼ��
*******************************************/
void bsp_ds18b20_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    DQ_1();
    /* configure led GPIO port */ 
    gpio_init(GPIOA, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_Reset
*	����˵��: ��λDS18B20�� ����DQΪ�ͣ���������480us��Ȼ��ȴ�
*	��    ��: ��
*	�� �� ֵ: 0 ʧ�ܣ� 1 ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t DS18B20_Reset(void)
{
	/*
		��λʱ��, ��DS18B20 page 15

		������������DQ���������� 480us
		Ȼ���ͷ�DQ���ȴ�DQ�������������ߣ�Լ 15-60us
		DS18B20 ������DQΪ�� 60-240us�� ����źŽ� presence pulse  (��λ����,��ʾDS18B20׼������ ���Խ�������)
		���������⵽�����Ӧ���źţ���ʾDS18B20��λ�ɹ�
	*/

	uint8_t i;
	uint16_t k;

	/* ��λ�����ʧ���򷵻�0 */
	for (i = 0; i < 1; i++)
	{
		DQ_0();				/* ����DQ */
		delay_us(520);	/* �ӳ� 520uS�� Ҫ������ӳٴ��� 480us */
		DQ_1();				/* �ͷ�DQ */

		delay_us(20);	/* �ȴ�15us */

		/* ���DQ��ƽ�Ƿ�Ϊ�� */
		for (k = 0; k < 10; k++)
		{
			if (DQ_IS_LOW())
			{
				break;
			}
			delay_us(10);	/* �ȴ�65us */
		}
		if (k >= 10)
		{
			continue;		/* ʧ�� */
		}

		/* �ȴ�DS18B20�ͷ�DQ */
		for (k = 0; k < 30; k++)
		{
			if (!DQ_IS_LOW())
			{
				break;
			}
			delay_us(10);	/* �ȴ�65us */
		}
		if (k >= 30)
		{
			continue;		/* ʧ�� */
		}

		break;
	}

	delay_us(10);

	if (i >= 1)
	{
		return 0;
	}

	return 1;
}
/*
*********************************************************************************************************
*	�� �� ��: DS18B20_WriteByte
*	����˵��: ��DS18B20д��1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DS18B20_WriteByte(uint8_t _val)
{
	/*
		д����ʱ��, ��DS18B20 page 16
	*/
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		DQ_0();
		delay_us(10);

		if (_val & 0x01)
		{
			DQ_1();
		}
		else
		{
			DQ_0();
		}
		delay_us(60);
		DQ_1();
		delay_us(10);
		_val >>= 1;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadByte
*	����˵��: ��DS18B20��ȡ1�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t DS18B20_ReadByte(void)
{
	/*
		д����ʱ��, ��DS18B20 page 16
	*/
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		read >>= 1;

		DQ_0();
		delay_us(10);
		DQ_1();
		delay_us(10);

		if (DQ_IS_LOW())
		{
			;
		}
		else
		{
			read |= 0x80;
		}
		delay_us(60);
	}

	return read;
}
/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadTempReg
*	����˵��: ���¶ȼĴ�����ֵ��ԭʼ���ݣ�
*	��    ��: ��
*	�� �� ֵ: �¶ȼĴ������� ������16�õ� 1���϶ȵ�λ, Ҳ����С����ǰ�������)
*********************************************************************************************************
*/
int16_t DS18B20_ReadTempReg(void)
{
	uint8_t temp1, temp2;

	/* ���߸�λ */
	if (DS18B20_Reset() == 0)
	{
		return 0;
	}		

	DS18B20_WriteByte(0xcc);	/* ������ */
	DS18B20_WriteByte(0x44);	/* ��ת������ */

	DS18B20_Reset();		/* ���߸�λ */

	DS18B20_WriteByte(0xcc);	/* ������ */
	DS18B20_WriteByte(0xbe);

	temp1 = DS18B20_ReadByte();	/* ���¶�ֵ���ֽ� */
	temp2 = DS18B20_ReadByte();	/* ���¶�ֵ���ֽ� */

	return ((temp2 << 8) | temp1);	/* ����16λ�Ĵ���ֵ */
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadID
*	����˵��: ��DS18B20��ROM ID�� �����ϱ���ֻ��1��оƬ
*	��    ��: _id �洢ID
*	�� �� ֵ: 0 ��ʾʧ�ܣ� 1��ʾ��⵽��ȷID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID(uint8_t *_id)
{
	uint8_t i;

	/* ���߸�λ */
	if (DS18B20_Reset() == 0)
	{
		return 0;
	}

	DS18B20_WriteByte(0x33);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_ReadByte();
	}

	DS18B20_Reset();		/* ���߸�λ */
	
	return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: DS18B20_ReadTempByID
*	����˵��: ��ָ��ID���¶ȼĴ�����ֵ��ԭʼ���ݣ�
*	��    ��: ��
*	�� �� ֵ: �¶ȼĴ������� ������16�õ� 1���϶ȵ�λ, Ҳ����С����ǰ�������)
*********************************************************************************************************
*/
int16_t DS18B20_ReadTempByID(uint8_t *_id)
{
	uint8_t temp1, temp2;
	uint8_t i;

	DS18B20_Reset();		/* ���߸�λ */

	DS18B20_WriteByte(0x55);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteByte(_id[i]);
	}
	DS18B20_WriteByte(0x44);	/* ��ת������ */

	DS18B20_Reset();		/* ���߸�λ */

	DS18B20_WriteByte(0x55);	/* ������ */
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteByte(_id[i]);
	}	
	DS18B20_WriteByte(0xbe);

	temp1 = DS18B20_ReadByte();	/* ���¶�ֵ���ֽ� */
	temp2 = DS18B20_ReadByte();	/* ���¶�ֵ���ֽ� */

	return ((temp2 << 8) | temp1);	/* ����16λ�Ĵ���ֵ */
}

uint8_t g_rom_id[8] = {0};
void ds18b20_test(void)
{
    bsp_ds18b20_init();	/* ����GPIO ���ڷ���DS18B20 */

	/* �� ROM ID */
	{
		uint8_t id[8];
		uint8_t ret;
		uint8_t i;

		ret = DS18B20_ReadID(id);

		if (ret == 0)
		{
			bsp_usart_printf("δ��⵽ DS18B20 \r\n");

			memset(g_rom_id, 0, 8);
		}
		else
		{
			bsp_usart_printf("DS18B20 Ok, id = ");

			for (i = 0; i < 8; i++)
			{
				printf("%02X",id[i]);
			}
			bsp_usart_printf("\r\n");

			memcpy(g_rom_id, id, 8);
		}
	}

	/*
		�����¶�ת������ (���� DS18B20.pdf page = 4 �ı���֤��ֵת��)
	*/
	{
		uint16_t usReg[10] = {0x07D0, 0x0550, 0x0191, 0x00A2, 0x0008,
							  0x0000, 0xFFF8, 0xFF5E, 0xFE6F, 0xFC90};
		uint8_t m;
		int16_t reg;

		for (m = 0; m < 10; m++)
		{
			reg = usReg[m];
			printf("reg = 0x%04X %6d --> %-4.04f��\r\n", (uint16_t)reg, reg, (float)reg / 16);
		}

	}
}
int16_t ds18b20_reg;
void ds18b20_readTemp(void)
{
    

    ds18b20_reg = DS18B20_ReadTempReg();
    printf("reg = 0x%04X %6d --> %-4.04f��\r\n", (uint16_t)ds18b20_reg, ds18b20_reg, (float)ds18b20_reg / 16);
}

