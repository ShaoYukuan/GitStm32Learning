#ifndef _MOTOR_H
#define _MOTOR_H

typedef enum
{
	MOTOR_Stop = 0,
	MOTOR_Forward = 1,
	MOTOR_Backward = 2
}Motor_Mode_t;

void Motor_Init(void);
void Motor_Forward(uint16_t Duty);
void Motor_Backward(uint16_t Duty);
void Motor_Stop(void);
Motor_Mode_t Motor_GetMode(void);

//void Motor_SetSpeed(int8_t Speed);


#endif
