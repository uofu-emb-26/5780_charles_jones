#ifndef TIM_SETUP_H
#define TIM_SETUP_H

#include "stm32f0xx.h"

void TIM2_Init_4Hz(void);
void TIM3_Init_PWM_800Hz(void);
void TIM3_Start(void);
void TIM3_PWM_GPIO_PC6_PC7_Init(void);

#endif