#include "stm32f10x.h"                  // Device header

//计数模块
uint16_t CountSensor_Count;


//初始化函数
void CountSensor_Init(void)

{
	//配置外部中断
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//配置GPIO，选择端口为输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// 配置AFIO，选择所用的这一路GPIO，连接后面的EXTI
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	//配置EXTI，选择边沿触发方式，选择触发响应方式（中断响应，事件响应）
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init( &EXTI_InitStructure);
	//配置NVIC，选择优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init( &NVIC_InitStructure);
	
}
//返回计数值
uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

//中断函数
void EXTI15_10_IRQHandler(void)
{
	if  (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count ++;
		//清除终端标志位
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
