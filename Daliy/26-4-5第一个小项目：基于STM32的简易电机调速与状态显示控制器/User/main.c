#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "Serial.h"
#include "PWM.h"
#include "KEY.h"

uint16_t ADValue;
uint8_t i;

int main(void)
{
	OLED_Init();
	AD_Init();
	Serial_Init();
	PWM_Init();

	OLED_ShowString(2,1,"ADVlaue:");
	OLED_ShowString(1,3,"ShaoYukuan");
	//printf("Num=%d\r\n",666);
	
	while(1)
	{
		
		ADValue = AD_GetValue();
		OLED_ShowNum(2,9,ADValue,4);
		Delay(i);
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET)  // 按下（接地）
		{
			Delay_ms(20);  // 简单消抖，可先不加
			printf("Key Pressed\r\n");
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET); // 等待松手
		}

	}
}
