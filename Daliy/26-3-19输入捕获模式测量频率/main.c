#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "IC.h"


int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1,1,"Freq:00000Hz");
	PWM_SetPrescaler(720-1);          //频率Freq = 72MHz/(PSC+1)/(ARR+1)
	PWM_SetCompare1(50);            //占空比Duty = CCR / 100
	while(1)
	{
		OLED_ShowNum(1,6,IC_GetFreq(),5);
	}
}
