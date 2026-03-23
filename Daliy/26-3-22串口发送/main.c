#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"


int main(void)
{
	OLED_Init();
	Serial_Init();
	
	Serial_SendByte(0x41);
	
	uint8_t MyArray[] = {0x42,0x43,0x44,0x45};//uint8_t是每个数据的长度，数组长度是uint16_t
	Serial_SendArray(MyArray,4);
	Serial_SendString("Hello World!");
	Serial_SendNumber(12345,5);
	printf("Num=%d\r\n",666);
	
	char String[100];
	sprintf(String,"Num=%d\r\n",666);
	Serial_SendString(String);
	
	OLED_ShowString(1,3,"ShaoYukuan");//   \r\n两个转义字符才能实现换行
	while(1)
	{
		
	}
}
