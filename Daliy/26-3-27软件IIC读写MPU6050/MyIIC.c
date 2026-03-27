#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//为了后续的代码修改方便，直接对操作端口的库函数进行封装
void MyIIC_W_SCL(uint8_t BitValue)//_W_代表写的意思
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
    Delay_us(10);
}
void MyIIC_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
    Delay_us(10);
}

uint8_t MyIIC_R_SDA(void)//_R_代表读的意思
{
	uint8_t BitVlaue;
	BitVlaue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
    Delay_us(10);
	return BitVlaue;//返回读取SDA的电平 
}


void MyIIC_Init (void)
{
	//本节使用软件IIC所以不用库函数，只需要用GPIO读写函数
	//软件IIC初始化分两步
	//1.把SCL和SDA都初始化为开漏输出的模式
	//2.把SCL和SDA都置为高电平
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏模式不仅可以输出还可以输入，输入时先输出1，再直接读取输入寄存器就可以了
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}
//产生起始条件
//1.先拉低SDA再拉低SCL
void MyIIC_Start (void)
{
	//因为在指定地址读中起始条件还要兼容重复起始条件
	//Sr最开始SCL是低电平，SDA不确定，所以最好先将SDA变成高电平，这样可以完美兼容Sr
	MyIIC_W_SDA(1);
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(0);
}
//终止条件
void MyIIC_Stop(void)
{
	//为了确保之后释放SDA能产生上升沿，我们要在时序单元刚开始时，先拉低SDA
	MyIIC_W_SDA(0);
	MyIIC_W_SCL(1);
	MyIIC_W_SDA(1);
}
//发送一个字节
void MyIIC_SenByte(uint8_t Byte)
{
	//除了终止条件，SCL以高电平结束，所有单元我们都会保障SCL以低电平结束
	//SCL低电平，把Byte最高位放在SDA线上，（高位先行）
	//循环8次才能发送一个字节，所以定义一个变量，使用循环的方式
	uint8_t i;
	for(i = 0;i < 8;i ++)
	{	
		MyIIC_W_SDA(Byte & (0x80>>i));//使用按位与的方式取出数据的某一位或几位 
		MyIIC_W_SCL(1);
		MyIIC_W_SCL(0);
	}
}
//接收一个字节
uint8_t MyIIC_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	MyIIC_W_SDA(1);
	for (i = 0;i<8;i++)
	{
		
		MyIIC_W_SCL(1);
		if (MyIIC_R_SDA() == 1)
		{
			Byte |= (0x80>>i);
		}
		MyIIC_W_SCL(0);
	}
	return Byte;
}
//发送应答和接收应答
void MyIIC_SendAck(uint8_t AckBit)
{	
		MyIIC_W_SDA(AckBit);//使用按位与的方式取出数据的某一位或几位 
		MyIIC_W_SCL(1);
		MyIIC_W_SCL(0);
	
}
//接收一个字节
uint8_t MyIIC_ReceiveAck(void)
{
	uint8_t AckBit;
	MyIIC_W_SDA(1);
	MyIIC_W_SCL(1);
	AckBit = MyIIC_R_SDA() ;
	MyIIC_W_SCL(0);
	
	return AckBit;
}
