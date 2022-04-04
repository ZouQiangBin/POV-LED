/*************************************************************************************************/
/*文件：RGB IC SK9822配置H文件			                                                             */
/*记录：By Zouqb@2022.3.18																																			 */
/*************************************************************************************************/
#ifndef __SK9822_H
#define	__SK9822_H

#include "main.h"

typedef struct
{
		Uint8			Iout:5;	//== 电流共32个等级：0-0mA 1-0.581mA ...... 31-18mA
		Uint8			rsvd:3;	//== 默认111

}SK9822_BITS;
typedef union
{
		Uint8			all;	//== 默认111
 SK9822_BITS	bit;	//== 电流共32个等级：0-0mA 1-0.581mA ...... 31-18mA

}SK9822_BYTE;
typedef struct
{
	SK9822_BYTE	H;	
		Uint8			B;	
		Uint8			G;
		Uint8			R;
		
}SK9822_LED;

typedef struct
{
		Uint32		Head;							//== 帧头：0x00000000
	SK9822_LED	LED[RGB_NUM];
		Uint32		Tail; 						//== 帧尾：0xFFFFFFFF
		Uint8			*pLED[RGB_NUM][3];

}TYPE_SK9822;

#define	SK9822_DEF {			\
/*	Head;	*/	0x00000000,	\
/*	LED;	*/	{0xFF,0xFF,0xFF,},				\
/*	Tail;	*/	0xFFFFFFFF,	\
/*	pLED;	*/	{0,},				\
}; 

extern TYPE_SK9822 SK9822;

extern void SK9822_Init(void);
extern void SK9822_Update_DMA(Uint8 *pRGB);

#endif
