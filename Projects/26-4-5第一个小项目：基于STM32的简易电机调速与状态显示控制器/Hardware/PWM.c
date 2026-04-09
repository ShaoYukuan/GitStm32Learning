#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"

uint16_t CCR;

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//时钟源选择
	TIM_InternalClockConfig(TIM2);
	
	GPIO_InitTypeDef GPIO_InitStrecture;
	GPIO_InitStrecture.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStrecture.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStrecture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrecture);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择始终分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式
	TIM_TimeBaseStructure.TIM_Period = 100-1;//周期，ARR自动重装器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;//PSC预分频器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数器的值（高级计数器采用）
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//配置时基单元
	
	TIM_OCInitTypeDef TIM_OCInitStrecture;
	TIM_OCStructInit(&TIM_OCInitStrecture);
	TIM_OCInitStrecture.TIM_OCMode= TIM_OCMode_PWM1;//输出比较模式
	TIM_OCInitStrecture.TIM_OCPolarity = TIM_OCPolarity_High;//极性
	TIM_OCInitStrecture.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStrecture.TIM_Pulse =0;//CCR
	TIM_OC4Init(TIM2,&TIM_OCInitStrecture);
	
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_SetDutyByADC(uint16_t ADValue)
{
    CCR = (uint32_t)ADValue * 1000 / 4096;  // 假设 ARR=999
}


void PWM_SetCompare4(uint16_t CCR)
{
	TIM_SetCompare4(TIM2,CCR);//实时改变占空比
	uint16_t duty_percent = (CCR * 100) / 1000;
	printf("Duty = %d%%\r\n", duty_percent);
}


