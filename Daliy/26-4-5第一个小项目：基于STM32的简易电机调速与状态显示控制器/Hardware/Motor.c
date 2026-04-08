#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "AD.h"

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

void Motor_Speed(uint16_t Duty)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	TIM_SetCompare4(TIM2,Duty);
}
//void Motor_SetSpeed(int8_t Speed)
//{
//	if (Speed >= 0)							//如果设置正转的速度值
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_4);	//PA4置高电平
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5);	//PA5置低电平，设置方向为正转
//		PWM_SetCompare4(Speed);				//PWM设置为速度值
//	}
//	else									//否则，即设置反转的速度值
//	{
//		GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//PA4置低电平
//		GPIO_SetBits(GPIOA, GPIO_Pin_5);	//PA5置高电平，设置方向为反转
//		PWM_SetCompare4(-Speed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
//	}
//}
