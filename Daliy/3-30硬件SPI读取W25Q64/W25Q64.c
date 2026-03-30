#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init()
{
	MySPI_Init();
}

//参数（8位的厂商ID，16位的设备ID）
void W25Q64_Read_ID(uint8_t *MID,uint16_t *DID)
{
	MySPI_Start();
	//发送9F，代表读ID号的指令
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	//发送一个无意义的数据，目的是来接收对面有意义的数据
	//因为上一步开始了读取ID号指令，这一步接收ID号，接收返回值
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	//设备ID分为高八位和第八位，高八位是设备类型，低八位是设备容量
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

//读状态寄存器1，判断是否处于忙状态，写一个等待函数
//最低位Busy，1忙0不忙
void W25Q64_WaitBusy(void)
{
	uint32_t TimeOut;
	TimeOut = 10000;
	MySPI_Start();
	//读状态寄存器1
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	//接收数据
	//利用掩码获得取出数据的最低位，判断是否为1，套while循环，利用连续读出状态寄存器，实现等待BUSY功能
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		if(TimeOut == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}
//页编程
//先发送开始指令，连发三个字节，就是24位地址
//参数（24位地址，储存数据，表示一次写多少）
void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	W25Q64_WriteEnable();
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	//交换发送三个字节的地址，地址是高位字节先发送
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	//发送写入数据
	for(i =0;i<Count;i++)
	{
		MySPI_SwapByte(DataArray[i]);
		
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy();
	
}

//扇区擦除
//先发送指令20，再发送3个字节的地址
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();

	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();

}
//读取数据
//交换发送指令03，发送3个字节地址,随后转入接收，就可以一次接收数据
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	//交换发送三个字节的地址，地址是高位字节先发送
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	//开始读
	for(i = 0;i<Count;i++)
	{
		DataArray[i]=MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}










