#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Num;//实现定时器每秒Num+1

int main(void)
{
	Timer_Init();//初始化定时器
	
	OLED_Init();
	OLED_ShowString(1,3,"Num:");
	OLED_ShowString(2,3,"CNT:");

	while(1)
	{
		OLED_ShowNum(1,7,Num,5);//循环刷新Num值
		OLED_ShowNum(2,7,Timer_COunter(),5);
		//OLED_ShowNum(3,5,TIM_GetCounter(TIM2),5);//实时显示 TIM2 定时器当前的计数值
	}
}
//中断函数
void TIM2_IRQHandler(void)
{
	//检查中断标志位
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		Num ++;
		//清除标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
