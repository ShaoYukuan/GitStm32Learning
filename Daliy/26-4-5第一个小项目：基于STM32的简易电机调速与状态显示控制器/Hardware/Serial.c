#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


void Serial_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	USART_InitTypeDef USART_InitStrecture;
	USART_InitStrecture.USART_BaudRate = 9600;//波特率
	USART_InitStrecture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制，不需要
	USART_InitStrecture.USART_Mode = USART_Mode_Tx;//模式，发送模式
	USART_InitStrecture.USART_Parity = USART_Parity_No;//奇偶校验，不需要
	USART_InitStrecture.USART_StopBits = USART_StopBits_1;//停止位，选择1
	USART_InitStrecture.USART_WordLength = USART_WordLength_8b;//字长，选择8位
	USART_Init(USART1,&USART_InitStrecture);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//将PA9引脚初始化为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_Cmd (USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	//TXE(发送数据寄存器空)这个标志位=1时表示：TDR 中的数据已经被转移到发送移位寄存器，可以写入下一个字节了。
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Serial_SendString(char*String)
{
	uint8_t i;
	for (i =0;String[i] != '\0';i++)
	{
		Serial_SendByte(String[i]);
	}
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}
