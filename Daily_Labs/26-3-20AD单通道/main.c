#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;//存放返回值

int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1,1,"ADValue:");
	while(1)
	{
		ADValue = AD_GetValue();
		OLED_ShowNum(1,9,ADValue,4);
	}
}
