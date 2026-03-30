#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint8_t  MID;
uint16_t DID;

uint8_t ArrayWrite[4]={0x11,0x22,0x33,0x44};
uint8_t ArrayRead[4];

int main(void)
{
	OLED_Init();
	W25Q64_Init();
	
	OLED_ShowString(1,1,"MID:");
	OLED_ShowString(1,8,"DID:");
	OLED_ShowString(2,1,"W:" );
	OLED_ShowString(3,1,"R:");
	
	W25Q64_Read_ID(&MID,&DID);
	
	OLED_ShowHexNum(1,5,MID,2);
	OLED_ShowHexNum(1,12,DID,4);
	//写之前先擦除
	W25Q64_SectorErase(0x000000);
	
	W25Q64_PageProgram(0x0000000,ArrayWrite,4);
	
	W25Q64_ReadData(0x000000,ArrayRead,4);
	
	
	
	OLED_ShowHexNum(2,3,ArrayWrite[0],2);
	OLED_ShowHexNum(2,6,ArrayWrite[1],2);
	OLED_ShowHexNum(2,9,ArrayWrite[2],2);
	OLED_ShowHexNum(2,12,ArrayWrite[3],2);
	
	OLED_ShowHexNum(3,3,ArrayRead[0],2);
	OLED_ShowHexNum(3,6,ArrayRead[1],2);
	OLED_ShowHexNum(3,9,ArrayRead[2],2);
	OLED_ShowHexNum(3,12,ArrayRead[3],2);
	while(1)
	{
		
	}
}
