#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	//操作STM32的GPIO总共需要三个步骤
	//1.使用RCC开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//2.使用GPIO_Init函数初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;//结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//推挽输出,开漏输出低电平驱动
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//3.使用输出或者输入的函数控制GPIO口
	//GPIO_WriteBit(GPIOA, GPIO_Pin_0,Bit_SET);//控制点亮
	while(1)
	{
		//GPIO_WriteBit(GPIOA, GPIO_Pin_0,Bit_RESET);
		//延时函数
		 //Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0,Bit_SET);
		//Delay_ms(500);

	}
}
