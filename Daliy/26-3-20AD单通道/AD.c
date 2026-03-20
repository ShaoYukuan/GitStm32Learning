#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//1.RCC开启ADC和GPIO时钟，ADCCLK分频器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//2.配置GPIO，配置成模拟输入
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//使GPIO口无效，防止GPIO输入输出对模拟电压产生影响
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//PA0被初始化成模拟输入引脚
	
	//3.选择规则组的输入通道       指定通道     规则组序列器的通道    指定采样时间(时间越小越快，越大越稳定)
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	//4.配置ADC转换器
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//选择独立模式或者双ADC模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//触发控制的触发源选择
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//选择连续转换或者单次转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//选择扫描模式或者非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;//通道数目(在扫描模式下)
	ADC_Init(ADC1, &ADC_InitStructure);
	//5.开关控制，调用ADC_Cmd开启ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//函数作用为返回复位校准状态，
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);//等待校准是否完成
}
//启动转换，获取结果
uint16_t  AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换
//                         规则组完成标志位
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//RESET表示未完成
	return ADC_GetConversionValue(ADC1);//获取转换值，取结果
}
