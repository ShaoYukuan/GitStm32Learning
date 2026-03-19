#include "stm32f10x.h"                  // Device header

//编码器初始化
void Encoder_Init(void)
{
	//1.RCC开启GPIO和定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//2.配置GPIO，配置PA6，7为输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//3.配置时基单元，一般选择不分频

	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
	TIM_TimBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数器模式，这里选择自增
	TIM_TimBaseStructure.TIM_Period = 65536-1;//ARR的值，防止溢出给最大值
	TIM_TimBaseStructure.TIM_Prescaler = 1-1;//预分频器PSC给0，就是不分频，编码器时钟直接驱动计数器
	TIM_TimBaseStructure.TIM_RepetitionCounter = 0 ;//重复计数器的值（高级计数器采用） 
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseStructure);	
	//4.配置输入捕获
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);//结构体初始化，由于结构体配置不完整，调用这个函数来避免出现一些问题
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选择定时器通道参数
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//用来选择输入捕获滤波器,作用滤除高频噪声，使信号更平滑，不改变信号频率
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//选择定时器通道参数
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//用来选择输入捕获滤波器,作用滤除高频噪声，使信号更平滑，不改变信号频率
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	
	//5.配置编码器接口模式
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//6.调用TIM_CMD，启动定时器
	TIM_Cmd(TIM3,ENABLE);
}
//
int16_t Encoder_Get(void)
{
	//实现测试，在固定的闸门时间内读一次CNT后CNT清零
	int16_t Temp;
	Temp =  TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}
