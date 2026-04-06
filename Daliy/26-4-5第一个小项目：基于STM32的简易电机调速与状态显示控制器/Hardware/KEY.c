#include "stm32f10x.h"                  // Device header

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStrecture;
	GPIO_InitStrecture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrecture.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStrecture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrecture);
	
}
