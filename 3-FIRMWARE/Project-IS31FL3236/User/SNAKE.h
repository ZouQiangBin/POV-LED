/*************************************************************************************************/
/*ÎÄ¼þ£ºÌ°³ÔÉß						                                                               				*/
/*¼ÇÂ¼£ºBy Zouqb@2021.7.7																																			 */
/*************************************************************************************************/

#ifndef __SNAKE_H
#define __SNAKE_H	

#include "stm32f10x.h"
#include "main.h"	

extern pSnake S0;
extern pSnake SF;
extern void SnakeInit(TYPE_COLOR *p);
extern void SnakeFoodGen(void);
extern void SnakeRun(Uint8 Dir);
extern void SnakeReset(void);

#endif 

