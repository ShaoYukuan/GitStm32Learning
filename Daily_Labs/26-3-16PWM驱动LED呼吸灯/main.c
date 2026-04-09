#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint8_t i;
int main(void)
{
	OLED_Init();
	OLED_ShowString(1,3,"ChengGong");
	PWM_Init();
	while(1)
	{
		for(i = 0;i <= 100; i++)
		{
			PWM_SetCompare1(i);//设置CCR的值
			Delay_ms(10);
		}
		for(i = 0;i <= 100; i++)
		{
			PWM_SetCompare1(100-i);
			Delay_ms(10);
		}
	}
}
