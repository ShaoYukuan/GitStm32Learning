#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>//封装sprintf函数

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//GPIO初始化，配置TX为复用输出，RX为输入
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//TX引脚是USART外设控制的输出脚，所以选用复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	//配置USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;//USART_Mode_Tx | USART_Mode_Rx这样可以实现同时接收和发送
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位长度
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
	//使用接收功能，可能需要配置中断，在开启USART之前加上ITConfig和NVIC的代码
}
//发送数据的函数，调用这个函数就可以从TX引脚发送一个字节数据

void Serial_SendByte(uint8_t Byte)
{
	//调用串口的SendData函数
	USART_SendData(USART1, Byte);//Byte写入到TDR
//   等待标志位              发送数据寄存器空标志位，
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待TXE置1
	
}

void Serial_SendArray(uint8_t *Array,uint16_t Length)//这是一个uint8_t的指针类型，指向待发射数组的首地址,由于数组无法判断是否结束，所以再传递Length
{
	uint16_t i;
	for (i = 0; i<Length; i ++)//对Array进行遍历
	{
		Serial_SendByte(Array[i]);//依次取出数组Array中每一项
	}
}

void Serial_SendString(char*String)//char = uint8_t，字符串自带结束标志位，所以不需要传递长度参数
{
	uint8_t i;
	for (i = 0;String[i]!='\0';i++)//循环条件利用结束标准位判断
	{
		Serial_SendByte(String[i]);
	}
}

//次方函数
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}




void Serial_SendNumber(uint32_t Number,uint8_t Length)//char = uint8_t，字符串自带结束标志位，所以不需要传递长度参数
{
	//需要将Number的个位，十位，百位等以十进制拆分开，转换成字符数字对应的数据依次发送出去
	uint8_t i;
	for (i = 0;i<Length;i++)//循环条件利用结束标准位判断
	{
		Serial_SendByte(Number / Serial_Pow(10,Length-i-1)%10+'0');
	}

}

int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

//               （接受格式化字符串,接受后面的可变参数列表）
void Serial_Printf(char *format, ...)
{
	//定义输出的字符串
	
	
}



