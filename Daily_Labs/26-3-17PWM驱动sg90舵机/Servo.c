#include "stm32f10x.h"                  // Device header
#include "PWM.h"//继承PWM功能

void Servo_Init(void)
{
	PWM_Init();
}

//角度        CCR
//0           500
//180         2500

void Servo_SetAngle(float Angle)//设置舵机角度
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}