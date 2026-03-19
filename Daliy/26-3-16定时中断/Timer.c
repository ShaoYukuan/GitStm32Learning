#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//1.RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//2.选择时基单元的时钟源，
	TIM_InternalClockConfig(TIM2);//内部时钟
	//3.配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义结构体
	TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数器模式
	TIM_TimeBaseStructure.TIM_Period = 10000-1;//周期，ARR自动重装器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;//PSC预分频器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数器的值（高级计数器采用） 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//4.配置输出中断控制，允许更新中断输出到NVIC
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//避免刚初始化完就进中断的问题
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//5.配置NVIC，在NVIC中打开定时器中断的通道，分配优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC优先级分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//赋能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init( &NVIC_InitStructure);
	//6.运行控制
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}

//中断函数
/*void TIM2_IRQHandler(void)
{
	//检查中断标志位
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		//清除标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}*/
