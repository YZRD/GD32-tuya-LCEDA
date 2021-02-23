#include "bsp_timer.h"
#include "gd32f10x.h"

#define	Timer_1_List_Count	10	// 消息队列的大小

struct 
{
	unsigned char Flag[Timer_1_List_Count];				// 标志
	void (*Fun_Point_List[Timer_1_List_Count])(void);	// 函数指针
	unsigned long Counter[Timer_1_List_Count];			// 当前计数
	unsigned long Timer[Timer_1_List_Count];			// 时间
}Timer1_Struct;			// Timer1循环执行的函数统计

struct 
{
	unsigned char Flag[Timer_1_List_Count];				// 标志
	void (*Fun_Point_List[Timer_1_List_Count])(void);	// 函数指针
	unsigned long Counter[Timer_1_List_Count];			// 当前计数
	unsigned long Timer[Timer_1_List_Count];			// 时间
}Timer1_Struct_Once;	// Timer1执行一次的函数统计

struct
{
	unsigned char MessageQueue[Timer_1_List_Count];				// 当前的消息
	unsigned char MessageList[Timer_1_List_Count];				// 设置的消息
	void (*MessageFun_Point_List[Timer_1_List_Count])(void);	// 函数指针
	
}Timer1_Message_Struct;	// Timer1执行消息映射的函数统计

unsigned char Timer1_Handler_Flag=0;		// Timer1中断标志
unsigned char Timer1_Handler_Flag_Message=0;// Timer1中断消息标志	
unsigned char Timer1_Handler_Flag_Once=0;	// Timer1中断一次标志

/*******************************************
函数名：bsp_timer_init
作  者：WZ
时  间：2021.2.22
功  能：timer1定时器初始化配置
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

//添加一个函数到定时器中
unsigned char Timer_1_Add_Fun(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(Timer1_Struct.Flag[i] == 0)
		{
			Timer1_Struct.Flag[i] = 1;		// 标志置1
			Timer1_Struct.Counter[i] = 0;	// 计数清0
			Timer1_Struct.Fun_Point_List[i] = Fun;	// 函数指针
			Timer1_Struct.Timer[i] = Time-1;		// 时间
			return 1;
		}
	}
	return 0;
}

//添加一个一次性执行函数到定时器中
unsigned char Timer_1_Add_Fun_Once(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(Timer1_Struct_Once.Flag[i] == 0)
		{
			Timer1_Struct_Once.Flag[i] = 1;		// 标志置1
			Timer1_Struct_Once.Counter[i] = 0;	// 计数清0
			Timer1_Struct_Once.Fun_Point_List[i] = Fun;	// 函数指针
			Timer1_Struct_Once.Timer[i] = Time-1;		// 时间
			return 1;
		}
	}
	return 0;
}


//添加一个消息和函数的映射关系
unsigned char Timer1_Add_MessageFun(unsigned char Message,void (*Fun)(void))
{
	unsigned char i;
	for(i=0;i<Timer_1_List_Count;i++)
	{
		if(!Timer1_Message_Struct.MessageList[i]) // 若消息为0
		{
			Timer1_Message_Struct.MessageList[i] = Message;			// 添加消息
			Timer1_Message_Struct.MessageFun_Point_List[i] = Fun;	// 添加函数指针
			return 1;
		}
	}
	return 0;
}
//给系统发射一个信号
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

//系统定时器中断里面执行的函数
void Timer1_SYS_APP_LOOP(void)
{
	unsigned char i=0;
	if(Timer1_Handler_Flag == 0)	// 若定Timer0还没有中断
		return;
	Timer1_Handler_Flag = 0;
	for(i=0;i<Timer_1_List_Count;i++) 	// 循环定时的函数
	{
		if(Timer1_Struct.Flag[i])		// 允许执行标志
		{
			Timer1_Struct.Counter[i] ++ ;
			if(Timer1_Struct.Counter[i] > Timer1_Struct.Timer[i])	// 时间到，执行一次函数指针指向的函数
			{
				Timer1_Struct.Counter[i] = 0;	// 清零计数
				Timer1_Struct.Fun_Point_List[i]();
			}
		}
	}
	
	
}


//系统循环执行-邮箱处理
void Timer1_SYS_APP_LOOP_Message(void)
{
	unsigned char i=0,j=0;
	if(Timer1_Handler_Flag_Message == 0)	// 若定Timer0还没有中断
		return;
	Timer1_Handler_Flag_Message = 0;
	
	for(i=0;i<Timer_1_List_Count;i++) //调用消息队列中的函数
	{
		if(Timer1_Message_Struct.MessageQueue[i])	// 当前有消息
		{
			for(j=0;j<Timer_1_List_Count;j++)
			{
				if(Timer1_Message_Struct.MessageQueue[i] == Timer1_Message_Struct.MessageList[j])	// 当前消息 == 设置的消息
				{
					//DebugSendByte(Timer1_Message_Struct.MessageQueue[i]);
					Timer1_Message_Struct.MessageFun_Point_List[j]();	// 执行相应函数
					j = Timer_1_List_Count + 10;	// 为了退出循环
				}
			}
			Timer1_Message_Struct.MessageQueue[i] = 0;	// 清空当前消息
		}
	}
}
//系统循环，单次执行函数处理
void Timer1_SYS_APP_LOOP_Once(void)
{
	unsigned char i=0;
	if(Timer1_Handler_Flag_Once == 0)
		return;
	Timer1_Handler_Flag_Once = 0;
	
	for(i=0;i<Timer_1_List_Count;i++) 	//一次性定时函数
	{
		if(Timer1_Struct_Once.Flag[i])	// 允许执行标志
		{
			Timer1_Struct_Once.Counter[i] ++ ;	
			if(Timer1_Struct_Once.Counter[i] > Timer1_Struct_Once.Timer[i])	// 时间到，执行一次函数指针指向的函数
			{
				Timer1_Struct_Once.Counter[i] = 0;	// 计数清零
				Timer1_Struct_Once.Fun_Point_List[i]();	// 执行对应函数
				Timer1_Struct_Once.Flag[i] = 0;// 允许标志清零			
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
//delay_nUs 最好是10的倍数
void delay_us(int delay_nUs)
{
    g_delaynum=0;
    timer_enable(TIMER3);
    while(g_delaynum<delay_nUs/10){};
    timer_disable(TIMER3);    
}



