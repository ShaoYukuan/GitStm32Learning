#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//RCC开启时钟，TIM和GPIO外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//2.选择时基单元的时钟源，
	TIM_InternalClockConfig(TIM2);//内部时钟
	//3.配置时基单元,GPIO
	GPIO_InitTypeDef GPIO_InitStrusture;
	GPIO_InitStrusture.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStrusture.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStrusture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrusture);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义结构体
	TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数器模式
	TIM_TimeBaseStructure.TIM_Period = 100-1;//周期，ARR自动重装器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;//PSC预分频器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数器的值（高级计数器采用） 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//给结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//设置输出比较极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;//设置CCR
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);//初始化输出比较单元，选择通道
	
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}

void PWM_SetCompare3(uint16_t Compare)//
{
	TIM_SetCompare3(TIM2,Compare);//实时改变CCR
}
