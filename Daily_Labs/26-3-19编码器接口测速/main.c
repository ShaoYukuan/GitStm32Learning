#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed;

int main(void)
{
	Timer_Init();
	OLED_Init();
	Encoder_Init();
	
	OLED_ShowString(2,1,"Speed:");
	while(1)
	{
		OLED_ShowSignedNum(2,7,Encoder_Get(),5);//循环刷新Num值
	}
}
//中断函数
void TIM2_IRQHandler(void)
{
	//检查中断标志位
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		Speed = Encoder_Get();
		//清除标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
