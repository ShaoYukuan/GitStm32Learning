#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;

int main(void)
{
	OLED_Init();
	AD_Init();
	
	//ADValue = AD_GetValue();
	
	OLED_ShowString(2,1,"ADVlaue:");
	//OLED_ShowHexNum(2,8,ADValue,6);
	OLED_ShowString(1,3,"ShaoYukuan");
	while(1)
	{
		ADValue = AD_GetValue();
		OLED_ShowNum(2,9,ADValue,4);

	}
}
