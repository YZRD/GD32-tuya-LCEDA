#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

extern unsigned char Timer1_Handler_Flag;		// Timer1�жϱ�־
extern unsigned char Timer1_Handler_Flag_Message;// Timer1�ж���Ϣ��־	
extern unsigned char Timer1_Handler_Flag_Once;	// Timer1�ж�һ�α�־
extern char g_delaynum;

void bsp_timer_init(void);//��ʱ�� ��ʼ��

unsigned char Timer_1_Add_Fun(unsigned long Time,void (*Fun)(void));
unsigned char Timer_1_Add_Fun_Once(unsigned long Time,void (*Fun)(void));
unsigned char Timer1_Add_MessageFun(unsigned char Message,void (*Fun)(void));
unsigned char Timer1_SendMessage(unsigned char Message);
void Timer1_SYS_APP_LOOP(void);
void Timer1_SYS_APP_LOOP_Message(void);
void Timer1_SYS_APP_LOOP_Once(void);

void bsp_timer3_init(void);
void delay_us(int delay_nUs);

#endif
