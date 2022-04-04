#ifndef __DELAY_H
#define	__DELAY_H

#include <stm32f10x.h>

void SysTick_Init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delayms(uint16_t ms);

#endif
