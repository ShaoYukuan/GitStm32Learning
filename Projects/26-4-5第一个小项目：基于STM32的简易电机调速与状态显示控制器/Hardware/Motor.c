#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "AD.h"

//电机运转模式枚举

typedef enum
{
	MOTOR_Stop = 0,
	MOTOR_Forward = 1,
	MOTOR_Backward = 2
}Motor_Mode_t;

static Motor_Mode_t Motor_CurrentMode = MOTOR_Stop;

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	PWM_Init();

}

void Motor_Forward(uint16_t Duty)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	TIM_SetCompare4(TIM2,Duty);
	Motor_CurrentMode = MOTOR_Forward;
}

void Motor_Backward(uint16_t Duty)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);    // PA6 = 0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);      // PA7 = 1
	TIM_SetCompare4(TIM2, Duty);
	Motor_CurrentMode = MOTOR_Backward;
}

void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	TIM_SetCompare4(TIM2,0);
	Motor_CurrentMode = MOTOR_Stop;
}
// 获取当前电机模式
Motor_Mode_t Motor_GetMode(void)
{
	return Motor_CurrentMode;
}
