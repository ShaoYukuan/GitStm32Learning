#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//时钟源选择
	TIM_InternalClockConfig(TIM2);
	
	GPIO_InitTypeDef GPIO_InitStrecture;
	GPIO_InitStrecture.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStrecture.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStrecture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrecture);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择始终分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式
	TIM_TimeBaseStructure.TIM_Period = 100-1;//周期，ARR自动重装器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;//PSC预分频器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数器的值（高级计数器采用）
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//配置时基单元
	
	TIM_OCInitTypeDef TIM_OCInitStrecture;
	TIM_OCStructInit(&TIM_OCInitStrecture);
	TIM_OCInitStrecture.TIM_OCMode= TIM_OCMode_PWM1;//输出比较模式
	TIM_OCInitStrecture.TIM_OCPolarity = TIM_OCPolarity_High;//极性
	TIM_OCInitStrecture.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStrecture.TIM_Pulse =0;//CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStrecture);
	
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_SetCompare1(uint16_t COmpare)
{
	TIM_SetCompare1(TIM2,COmpare);//实时改变CCR
}

void Delay(uint8_t i)
{
	for(i = 0;i <= 100; i++)
		{
			PWM_SetCompare1(i);//设置CCR的值
			Delay_ms(10);
		}
		for(i = 0;i <= 100; i++)
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
		}
}
