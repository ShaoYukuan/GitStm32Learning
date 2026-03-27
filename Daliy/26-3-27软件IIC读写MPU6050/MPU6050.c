//这个模块建立在IIC模块上
#include "stm32f10x.h"                  // Device header
#include "MyIIC.h"
#include "MPU6050_Reg.h"

//为了方便修改参数和突出它是从机地址，使用一下宏定义
#define MPU6050_ADDRESS     0xD0

//封装指定地址写
//指定地址写寄存器     寄存器地址          数据 
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyIIC_Start();
	MyIIC_SenByte(MPU6050_ADDRESS);//从机地址加读写位
	//接收应答
	MyIIC_ReceiveAck();//这个函数会返回应答位
	MyIIC_SenByte(RegAddress);//指定寄存器地址，这个字节会存在MPU6050当前地址指针里
	MyIIC_ReceiveAck();
	MyIIC_SenByte(Data);//指定我要写入指定寄存器地址下的数据
	MyIIC_ReceiveAck();
	MyIIC_Stop();
}
//封装指定地址读时序
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;//用来存储接收到的数据
	MyIIC_Start();
	MyIIC_SenByte(MPU6050_ADDRESS);//从机地址加读写位
	//接收应答
	MyIIC_ReceiveAck();//这个函数会返回应答位
	MyIIC_SenByte(RegAddress);//指定寄存器地址，这个字节会存在MPU6050当前地址指针里
	MyIIC_ReceiveAck();
	//上面的函数是设置地址，设置完地址之后重新指定读写位，转入读时序
	MyIIC_Start();
	MyIIC_SenByte(MPU6050_ADDRESS | 0x01);
	MyIIC_ReceiveAck();
	
	Data = MyIIC_ReceiveByte();
	MyIIC_SendAck(1);//1就是不给从机应答表示就要这些数据0则反之
	MyIIC_Stop();
	
	return Data;//返回读出值
}
void MPU6050_Init(void)
{
	//初始化时先把底层初始化
	MyIIC_Init();
	//再写入一些寄存器，对MPU6050硬件电路进行初始化配置
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
}
//获取数据寄存器函数
//根据任务需求，这个函数需要返回6个int16_t的数据，但是c中函数返回值只能有一个，所以要设计多返回值函数的设计

/**
  * 函    数：MPU6050获取数据
  * 参    数：AccX AccY AccZ 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 参    数：GyroX GyroY GyroZ 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 返 回 值：无
  */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX =(DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}





