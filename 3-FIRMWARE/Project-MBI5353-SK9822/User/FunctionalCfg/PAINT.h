/*************************************************************************************************/
/*文件：画图C文件			                                                               						 */
/*记录：By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/

#ifndef __PAINT_H
#define	__PAINT_H

#include "main.h"

extern void DrawClear(void);
extern void DrawPoint(Uint8 x, Uint8 y, Uint8 en, TYPE_COLOR *p);
extern void DrawArea(Uint8 x1, Uint8 y1, Uint8 x2, Uint8 y2, Uint8 en, TYPE_COLOR *p);
extern void DrawLine(Uint8 x1, Uint8 y1, Uint8 x2, Uint8 y2, Uint8 en, TYPE_COLOR *p);
extern void DrawOneChar(Uint8 x, Uint8 y, Uint8 en, char Code, TYPE_COLOR *p);
extern void DrawChars(Uint8 x, Uint8 y, Uint8 en, char *Code, TYPE_COLOR *p);

#endif
