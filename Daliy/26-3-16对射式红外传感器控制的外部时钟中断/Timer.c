#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//1.RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//第10，11，12行代码作用，将PA15，PB3，PB4将所选调试端口引脚重映射
	/*RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);//引脚重映射配置
	*/GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//用AFIO将JTAG复用解除
	
	GPIO_InitTypeDef GPIO_InitStrusture;
	GPIO_InitStrusture.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrusture.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStrusture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrusture);
	//2.选择时基单元的时钟源，
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00);
	//3.配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义结构体
	TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数器模式
	TIM_TimeBaseStructure.TIM_Period = 10-1;//周期，ARR自动重装器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 1-1;//PSC预分频器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数器的值（高级计数器采用） 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//4.配置输出中断控制，允许更新中断输出到NVIC
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//避免刚初始化完就进中断的问题
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//5.配置NVIC，在NVIC中打开定时器中断的通道，分配优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC优先级分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//赋能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init( &NVIC_InitStructure);
	//6.运行控制
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}

//实时显示CNT计数器的值
uint16_t Timer_COunter(void)
{
	return TIM_GetCounter(TIM2);
}

//中断函数
/*void TIM2_IRQHandler(void)
{
	//检查中断标志位
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		//清除标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}*/
