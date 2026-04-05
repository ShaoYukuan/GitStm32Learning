#include "stm32f10x.h"                  // Device header


//AD初始化
void AD_Init(void)
{
	//开启时钟
	//ADC引脚PA1-7，PB0-1
	//ADC都是APB2上的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//设置ADC时钟,对时钟进行6分频（72/6），ADC最高频率为14Mhz，为保证稳定性和最快转换速率，选择6分频最合适
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	//GPIO_Mode_AIN为ADC专属模式，该模式下GPIO口断开，防止GPIO输入输出对模拟电压造成影响
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//规则组通道配置( ,根据引脚定义表找到gpio口对应的通道，规则组序列器次序, )
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//连续转换，失能，每转换一次规则组序列后停止
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐，选择右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发，使用软件触发，不需要外部触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//模式，选择独立模式，即单独使用ADC1
	ADC_InitStructure.ADC_NbrOfChannel = 1;//通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描模式，失能，只转换规则组的序列1这一个位置
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//ADC使能
	ADC_Cmd(ADC1,ENABLE);
	
	/*ADC校准*/
	ADC_ResetCalibration(ADC1);								//固定流程，内部有电路会自动执行校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

//获取ADC转换的值
//返回值：ADC转换的值，范围0-4095
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发AD转换一次
	while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//等待EOC标志位，即等待AD转换结束
	return ADC_GetConversionValue(ADC1);//读数据寄存器，得到AD转换的结果
}


