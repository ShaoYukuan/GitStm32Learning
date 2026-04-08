#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "Serial.h"
#include "PWM.h"
#include "KEY.h"
#include "Motor.h"

uint16_t ADValue;
uint16_t Duty;

int main(void)
{
	OLED_Init();
	AD_Init();
	Serial_Init();
	Motor_Init();

	OLED_ShowString(2,1,"ADVlaue:");
	OLED_ShowString(3,1,"Duty:  %");
	OLED_ShowString(1,3,"ShaoYukuan");
	
	//printf("Num=%d\r\n",666);
	
	while(1)
	{
		
		ADValue = AD_GetValue();
		OLED_ShowNum(2,9,ADValue,4);
		OLED_ShowNum(3,6,Duty,2);
		
		Duty = (uint32_t)ADValue * 100 / 4096;
		PWM_SetCompare4(Duty);
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET)  // 按下（接地）
		{
			Delay_ms(20);  // 简单消抖
			printf("Key Pressed\r\n");
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == RESET); // 等待松手
		}
		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET)
		{
			Delay_ms(800);
			printf("AD_Value=%d\r\n",ADValue);
		}

	}
}
