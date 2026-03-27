#ifndef _MYIIC_H
#define _MYIIC_H

void MyIIC_Init (void);
void MyIIC_Start (void);
void MyIIC_Stop(void);
void MyIIC_SenByte(uint8_t Byte);
uint8_t MyIIC_ReceiveByte(void);
void MyIIC_SendAck(uint8_t AckBit);
uint8_t MyIIC_ReceiveAck(void);


#endif
