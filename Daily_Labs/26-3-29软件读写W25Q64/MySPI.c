#include "stm32f10x.h"                  // Device header

//封装置引脚高低电平的函数封装换个名字
//从机选择，输出引脚
void MySPI_W_SS(uint8_t BitValue)
{
	//GPIO_WriteBit(端口, 引脚号, 输出电平);标准库自带的单个 GPIO 引脚电平控制函数
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}
//输入引脚
uint8_t MySPI_R_MISO(void)
{
	//单个 GPIO 输入电平读取函数,GPIO_ReadInputDataBit(端口, 引脚号);
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

void MySPI_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//输出引脚配置为推挽输出，输入引脚配置为浮空或者上拉输入
	//对于主机来说，时钟，主机输出，片选都是输出引脚
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//置初始化之后的引脚默认电平
	MySPI_W_SS(1);//默认不选中从机
	MySPI_W_SCK(0);//计划使用SPI模式0，默认低电平

}

//起始信号
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}
//终止信号
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}
//交换一个字节
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	for(i = 0;i<8;i++)
	{
	MySPI_W_MOSI(ByteSend & (0x80 >> i));
	MySPI_W_SCK(1);
	if (MySPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}
	MySPI_W_SCK(0);
	}
	return ByteReceive;
}










