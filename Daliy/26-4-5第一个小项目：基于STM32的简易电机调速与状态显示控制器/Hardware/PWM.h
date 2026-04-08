#ifndef _PWM_H
#define _PWM_H

void PWM_Init(void);
void PWM_SetDutyByADC(uint16_t ADValue);
void PWM_SetCompare4(uint16_t CCR);


#endif
