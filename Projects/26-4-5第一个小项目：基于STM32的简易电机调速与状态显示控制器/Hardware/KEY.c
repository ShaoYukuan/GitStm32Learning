#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStrecture;
	GPIO_InitStrecture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrecture.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStrecture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrecture);
}

uint8_t Key_GetState(void)
{
	uint8_t KeyState = 0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == RESET)
	{
		Delay_ms(20);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == RESET)
		{
			KeyState = 1;
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET); // 等待松手
			Delay_ms(20);
		}
	}
	return KeyState;
}