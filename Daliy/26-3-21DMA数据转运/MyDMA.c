#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_size;

void MyDMA_Init(uint32_t ADDrA,uint32_t ADDrB,uint16_t Size)
{
	//定义结构体初始化，外设和储存器站点的起始地址，数据宽度，地址是否自增
	//方向，传输计数器，是否需要自动重装，选择触发源
	//通道优先级
	
	//DMA是AHB总线设备，第一个参数取决于是否为互联型设备，如STM32F105/107
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	MyDMA_size = Size;
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = Size;//缓存区大小（传输计数器）
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//选择硬件触发还是软件触发
	DMA_InitStructure.DMA_MemoryBaseAddr = ADDrB;//存储器的起始地址。
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//是否自增
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//传输模式（是否重装）
	//对于SRAM数组，它的地址是编译器分配的，并不是固定的，所以在写外设地址不写绝对地址，通过数组名来获取地址。把地址提取成初始化函数的参数,这样在初始化时，想转运哪个数组，就传哪个数组地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADDrA;//外设站点的起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//是否自增
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	//DMA_Cmd(DMA1_Channel1,DISABLE);
}

//DMA传输函数，调用一次在启动一次DMA转运
void MyDMA_Transfer(void)
{
	//重新给传输计数器赋值，前提是给DMA失能
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//等待转运完成
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//没完成就一直循环
	//清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);

}
