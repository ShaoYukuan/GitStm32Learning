#include "stm32f10x.h"                  // Device header


//输入捕获初始化
void IC_Init(void)
{
	//1.RCC开启GPIO TIM 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//选择TIM3是因为还要用TIM2输出PWM，选择不同TIM和通道记得改变GPIO引脚
	//2.GPIO初始化，GPIO配置成输入模式，一般选择上拉输入或浮空输入
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//3.配置时基单元，让CNT计数器在内部时钟驱动下自增运行
	TIM_InternalClockConfig(TIM3);//选择内部时钟源
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
	TIM_TimBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//选择计数器模式，这里选择自增
	TIM_TimBaseStructure.TIM_Period = 65536-1;//ARR的值，防止溢出给最大值
	TIM_TimBaseStructure.TIM_Prescaler = 72-1;//PSC的值，决定Fc,增大这个值可以扩大测量频率下
	TIM_TimBaseStructure.TIM_RepetitionCounter = 0 ;//重复计数器的值（高级计数器采用） 
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseStructure);
	//4.配置PWMI输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选择定时器通道参数
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//用来选择输入捕获滤波器,作用滤除高频噪声，使信号更平滑，不改变信号频率
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//极性选择(上升沿/下降沿)
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器，1，2，4，5每隔n-1次有效一次
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//选择触发信号从哪个引脚输入
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);//这个函数可以将TIM3的另一个通道初始化为相反的配置
	/*TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//选择定时器通道参数
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//用来选择输入捕获滤波器,作用滤除高频噪声，使信号更平滑，不改变信号频率
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//极性选择(上升沿/下降沿)
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器，1，2，4，5每隔n-1次有效一次
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;//选择触发信号从哪个引脚输入
	TIM_ICInit(TIM3, &TIM_ICInitStructure);*/
	//5.选择(TRGI)从模式触发源，这里选择TI1FP1
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	//6.选择触发之后的执行操作，这里选择RESET操作
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	//7.调用TIM_Cmd函数，开启定时器
	TIM_Cmd(TIM3,ENABLE);
}

//查看频率，返回最新周期的频率值，单位是Hz
uint32_t IC_GetFreq(void)
{
	return 1000000 / TIM_GetCapture1(TIM3);      // 1M/CCR的值
}
//获取占空比函数
uint32_t IC_GetDuty(void)
{
	return TIM_GetCapture2(TIM3) * 100/ TIM_GetCapture1(TIM3);//整个周期计数值存在CCR1中，高电平存在CCR2中
}

