/*************************************************************************************************/
/*文件：定时器配置文件						                                                               */
/*记录：By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#ifndef __TIM_H
#define __TIM_H	

#include "stm32f10x.h"
#include "main.h"	
 
void TIM1_Init(Uint16 Frq);
void TIM2_Init(void);
void TIM3_Init(Uint16 Frq);
void TIM4_Init(Uint16 Frq);

#endif 

