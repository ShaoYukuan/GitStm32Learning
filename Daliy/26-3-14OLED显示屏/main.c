#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	//初始化OLED
	OLED_Init();
	//显示一个字符，先写行列坐标
	//OLED_ShowChar(2,8,'N');
	

	while(1)
	{
		OLED_Clear();
	Delay_ms(500);
	OLED_ShowString(1,2,"ShaoYukuan");
	Delay_ms(500);
	OLED_ShowString(2,5,"love");
	Delay_ms(500);
	OLED_ShowString(3,2,"GuoShuang");
	Delay_ms(500);
	OLED_Clear();
	}
}
