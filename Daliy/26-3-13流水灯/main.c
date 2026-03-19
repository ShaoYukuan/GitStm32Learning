#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	//打开GPIOA时钟
	//操作STM32的GPIO总共需要三个步骤
	//1.使用RCC开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//2.使用GPIO_Init函数初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;//结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出,开漏输出低电平驱动
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//3.使用输出或者输入的函数控制GPIO口
	//GPIO_WriteBit(GPIOA, GPIO_Pin_0,Bit_SET);//控制点亮
	while(1)
	{
		
		GPIO_Write(GPIOA,~0x0001);//0000 0000 0000 0001
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0002);//0000 0000 0000 0010
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0004);//0000 0000 0000 0100
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0008);//0000 0000 0000 1000
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0010);//0000 0000 0001 0000
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0020);//0000 0000 0010 0000
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0040);//0000 0000 0100 0000
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0080);//0000 0000 1000 0000
		Delay_ms(500);
	}
}
