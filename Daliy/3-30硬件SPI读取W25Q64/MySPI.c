#include "stm32f10x.h"                  // Device header


void MySPI_W_SS(uint8_t BitValue)
{
	//GPIO_WriteBit(端口, 引脚号, 输出电平);标准库自带的单个 GPIO 引脚电平控制函数
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//输出引脚配置为推挽输出，输入引脚配置为浮空或者上拉输入
	//对于主机来说，时钟，主机输出，片选都是输出引脚
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//计算频率，APB2是72Mhz，APB1是36Mhz
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//时钟相位，第几个边沿开始采样
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	MySPI_W_SS(1);//默认不选中从机
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
	//等待标志位TXE为1
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
	//软件写入数据至SPI_DR,也就是TDR，TDR是要发送的数据所以参数为ByteSend
	SPI_I2S_SendData(SPI1,ByteSend);
	//发送和接收是同时的，接收完成也就代表发送完成，而接收完成会置标志位RXNE
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
	//读取DR，从RDR中将交换接收到的数据读取出来
	return SPI_I2S_ReceiveData(SPI1);
}










