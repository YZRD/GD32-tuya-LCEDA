#include "bsp_timer.h"
#include "gd32f10x.h"

#define	Timer_1_List_Count	10	// ��Ϣ���еĴ�С

struct 
{
	unsigned char Flag[Timer_1_List_Count];				// ��־
	void (*Fun_Point_List[Timer_1_List_Count])(void);	// ����ָ��
	unsigned long Counter[Timer_1_List_Count];			// ��ǰ����
	unsigned long Timer[Timer_1_List_Count];			// ʱ��
}Timer1_Struct;			// Timer1ѭ��ִ�еĺ���ͳ��

struct 
{
	unsigned char Flag[Timer_1_List_Count];				// ��־
	void (*Fun_Point_List[Timer_1_List_Count])(void);	// ����ָ��
	unsigned long Counter[Timer_1_List_Count];			// ��ǰ����
	unsigned long Timer[Timer_1_List_Count];			// ʱ��
}Timer1_Struct_Once;	// Timer1ִ��һ�εĺ���ͳ��

struct
{
	unsigned char MessageQueue[Timer_1_List_Count];				// ��ǰ����Ϣ
	unsigned char MessageList[Timer_1_List_Count];				// ���õ���Ϣ
	void (*MessageFun_Point_List[Timer_1_List_Count])(void);	// ����ָ��
	
}Timer1_Message_Struct;	// Timer1ִ����Ϣӳ��ĺ���ͳ��

unsigned char Timer1_Handler_Flag=0;		// Timer1�жϱ�־
unsigned char Timer1_Handler_Flag_Message=0;// Timer1�ж���Ϣ��־	
unsigned char Timer1_Handler_Flag_Once=0;	// Timer1�ж�һ�α�־

/*******************************************
��������bsp_timer_init
��  �ߣ�WZ
ʱ  �䣺2021.2.22
��  �ܣ�timer1��ʱ����ʼ������
*******************************************/
void bsp_timer_init(void)
{
    unsigned char i=0;
    timer_parameter_struct timer_initpara;
    /* enable the timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
    
    timer_deinit(TIMER1);
    
    timer_initpara.prescaler=107;//1000KHz 
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.period=1000;//1ms
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
    
    
    
	
	for(i=0;i<Timer_1_List_Count;i++)
	{
		Timer1_Struct.Flag[i] = 0;
		Timer1_Struct.Counter[i] = 0;
		
		Timer1_Struct_Once.Flag[i] = 0;
		Timer1_Struct_Once.Counter[i] = 0;
		
		Timer1_Message_Struct.MessageQueue[i] = 0;
		Timer1_Message_Struct.MessageList[i] = 0;
	}
    
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    timer_interrupt_enable(TIMER1,TIMER_INT_UP);
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    nvic_irq_enable(TIMER1_IRQn, 1, 1);
    timer_enable(TIMER1);
    
}

//���һ����������ʱ����
unsigned char Timer_1_Add_Fun(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(Timer1_Struct.Flag[i] == 0)
		{
			Timer1_Struct.Flag[i] = 1;		// ��־��1
			Timer1_Struct.Counter[i] = 0;	// ������0
			Timer1_Struct.Fun_Point_List[i] = Fun;	// ����ָ��
			Timer1_Struct.Timer[i] = Time-1;		// ʱ��
			return 1;
		}
	}
	return 0;
}

//���һ��һ����ִ�к�������ʱ����
unsigned char Timer_1_Add_Fun_Once(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(Timer1_Struct_Once.Flag[i] == 0)
		{
			Timer1_Struct_Once.Flag[i] = 1;		// ��־��1
			Timer1_Struct_Once.Counter[i] = 0;	// ������0
			Timer1_Struct_Once.Fun_Point_List[i] = Fun;	// ����ָ��
			Timer1_Struct_Once.Timer[i] = Time-1;		// ʱ��
			return 1;
		}
	}
	return 0;
}


//���һ����Ϣ�ͺ�����ӳ���ϵ
unsigned char Timer1_Add_MessageFun(unsigned char Message,void (*Fun)(void))
{
	unsigned char i;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(!Timer1_Message_Struct.MessageList[i]) // ����ϢΪ0
		{
			Timer1_Message_Struct.MessageList[i] = Message;			// �����Ϣ
			Timer1_Message_Struct.MessageFun_Point_List[i] = Fun;	// ��Ӻ���ָ��
			return 1;
		}
	}
	return 0;
}
//��ϵͳ����һ���ź�
unsigned char Timer1_SendMessage(unsigned char Message)
{
	unsigned char i;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(!Timer1_Message_Struct.MessageQueue[i])
		{
			Timer1_Message_Struct.MessageQueue[i] = Message;
			return 1;
		}
	}
	return 0;
}

//ϵͳ��ʱ���ж�����ִ�еĺ���
void Timer1_SYS_APP_LOOP(void)
{
	unsigned char i=0;
	if(Timer1_Handler_Flag == 0)	// ����Timer0��û���ж�
		return;
	Timer1_Handler_Flag = 0;
	for(i=0;i<Timer_1_List_Count;i++) 	// ѭ����ʱ�ĺ���
	{
		if(Timer1_Struct.Flag[i])		// ����ִ�б�־
		{
			Timer1_Struct.Counter[i] ++ ;
			if(Timer1_Struct.Counter[i] > Timer1_Struct.Timer[i])	// ʱ�䵽��ִ��һ�κ���ָ��ָ��ĺ���
			{
				Timer1_Struct.Counter[i] = 0;	// �������
				Timer1_Struct.Fun_Point_List[i]();
			}
		}
	}
	
	
}


//ϵͳѭ��ִ��-���䴦��
void Timer1_SYS_APP_LOOP_Message(void)
{
	unsigned char i=0,j=0;
	if(Timer1_Handler_Flag_Message == 0)	// ����Timer0��û���ж�
		return;
	Timer1_Handler_Flag_Message = 0;
	
	for(i=0;i<Timer_1_List_Count;i++) //������Ϣ�����еĺ���
	{
		if(Timer1_Message_Struct.MessageQueue[i])	// ��ǰ����Ϣ
		{
			for(j=0;j<Timer_1_List_Count;j++)
			{
				if(Timer1_Message_Struct.MessageQueue[i] == Timer1_Message_Struct.MessageList[j])	// ��ǰ��Ϣ == ���õ���Ϣ
				{
					//DebugSendByte(Timer1_Message_Struct.MessageQueue[i]);
					Timer1_Message_Struct.MessageFun_Point_List[j]();	// ִ����Ӧ����
					j = Timer_1_List_Count + 10;	// Ϊ���˳�ѭ��
				}
			}
			Timer1_Message_Struct.MessageQueue[i] = 0;	// ��յ�ǰ��Ϣ
		}
	}
}
//ϵͳѭ��������ִ�к�������
void Timer1_SYS_APP_LOOP_Once(void)
{
	unsigned char i=0;
	if(Timer1_Handler_Flag_Once == 0)
		return;
	Timer1_Handler_Flag_Once = 0;
	
	for(i=0;i<Timer_1_List_Count;i++) 	//һ���Զ�ʱ����
	{
		if(Timer1_Struct_Once.Flag[i])	// ����ִ�б�־
		{
			Timer1_Struct_Once.Counter[i] ++ ;	
			if(Timer1_Struct_Once.Counter[i] > Timer1_Struct_Once.Timer[i])	// ʱ�䵽��ִ��һ�κ���ָ��ָ��ĺ���
			{
				Timer1_Struct_Once.Counter[i] = 0;	// ��������
				Timer1_Struct_Once.Fun_Point_List[i]();	// ִ�ж�Ӧ����
				Timer1_Struct_Once.Flag[i] = 0;// �����־����			
			}
		}
	}
	
}



void bsp_timer3_init(void)
{
    timer_parameter_struct timer_initpara;
    /* enable the timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER3);
    
    timer_deinit(TIMER3);
    
    timer_initpara.prescaler=107;//1000KHz 
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.period=10;//10us
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3,&timer_initpara);
    
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);
    timer_interrupt_enable(TIMER3,TIMER_INT_UP);
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
    nvic_irq_enable(TIMER3_IRQn, 1, 1);
    //timer_enable(TIMER3);
    timer_disable(TIMER3);
    
}

char g_delaynum;
//delay_nUs �����10�ı���
void delay_us(int delay_nUs)
{
    g_delaynum=0;
    timer_enable(TIMER3);
    while(g_delaynum<delay_nUs/10){};
    timer_disable(TIMER3);    
}



