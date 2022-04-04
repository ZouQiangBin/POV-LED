
#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define	DEVICE_SEL 			2								//== IC选择：1-MBI5353 else-SK9822

#if (DEVICE_SEL == 1)
		#define DEVICES			3								//== 级联的MBI5353个数
		#define RGB_NUM			DEVICES*16			//== 单个MBI5353可驱动16个RGB
		
		#define		RMAXX		(96)							//== 旋转LED画面最大长度 最清晰的状态 16列
#else
		#define DEVICES			40							//== 级联的SK9822个数
		#define RGB_NUM			DEVICES*1				//== 单个SK9822可驱动1RGB个
		
		#define		RMAXX		(120)							//== 旋转LED画面最大长度 最清晰的状态 16列
#endif

//#define		RMAXX		(120)						//== 旋转LED画面最大长度 最清晰的状态 16列
#define		RMAXY		RGB_NUM					//== 旋转LED画面最大宽度 48个RGB 144个LED RGB值
#define		LMAXX		(RMAXX)					//== 线性LED画面配合贪吃蛇
#define		LMAXY		16							//== 线性LED画面最大宽度 16bit

#define ALED_RST	0								//== 抢占优先级
#define ALED_RST1	0								//== 从优先级
						
#define LLED_IRQ	1
#define LLED_IRQ1	1

#define RLED_IRQ	1								//== 旋转LED刷新优先级
#define RLED_IRQ1	1	

#define USART2_IRQ0 2
#define USART2_IRQ1 1

typedef int64_t 	int64;
typedef int32_t 	int32;
typedef int16_t 	int16;
typedef int8_t  	int8;

typedef uint64_t 	Uint64;
typedef uint32_t 	Uint32;
typedef uint16_t 	Uint16;
typedef uint8_t  	Uint8;

//=== 循环移位
#define CycL8(val,n)	(val >> (8 - n)  | (val << n))
#define CycR8(val,n)  (val << (8 - n)  | (val >> n))
#define CycL16(val,n)	(val >> (16 - n) | (val << n))
#define CycR16(val,n) (val << (16 - n) | (val >> n))
#define CycL32(val,n)	(val >> (32 - n) | (val << n))
#define CycR32(val,n) (val << (32 - n) | (val >> n))

//=== 码位倒叙8位
#define Reverse8C(data)	(((data&0xf0)>>4) | ((data&0x0f)<<4))
#define Reverse8B(data)	(((data&0xCC)>>2) | ((data&0x33)<<2))
#define Reverse8A(data)	(((data&0xAA)>>1) | ((data&0x55)<<1))
#define Reverse8(data)	(Reverse8A(Reverse8B(Reverse8C(data))))

//=== 码位倒叙16位
#define Reverse16D(data) (((data & 0xFF00) >> 8) | ((data & 0x00FF) << 8))
#define Reverse16C(data) (((data & 0xF0F0) >> 4) | ((data & 0x0F0F) << 4))
#define Reverse16B(data) (((data & 0xCCCC) >> 2) | ((data & 0x3333) << 2))
#define Reverse16A(data) (((data & 0xAAAA) >> 1) | ((data & 0x5555) << 1))
#define Reverse16(data)	 (Reverse16A(Reverse16B(Reverse16C(Reverse16D(data)))))

typedef enum {TURE = 1, FALSE = 0}STATUS;
typedef enum {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, OK = 4, STOP = 5, ELSE = 6}DIR;

//=== 贪吃蛇结构体坐标
typedef struct{
		Uint8		Sts;			//== 当前节点所处空间标志
		Uint8 	X;
		Uint8		Y;
}TYPE_POS;

//=== 贪吃蛇结构体坐标颜色
typedef struct{
		Uint8		R;
		Uint8		G;
		Uint8		B;
}TYPE_COLOR;

//=== 贪吃蛇结构体节点
typedef struct snake{

		TYPE_POS				Pos;
		TYPE_COLOR			Color;
		struct snake* 	Next;

}Snake, *pSnake;

//=== 主结构体
typedef struct{
		
		int32					Speed;
		int32					SpeedPrd;
		Uint16				AdcRaw[2];
		Uint8  				ScrollDiv;		
	
		Uint8 				LFrqDiv;					//== 线性LED刷新周期分频
		Uint8					LFrqDivCnt;				//== 分频计数
		Uint16 				RFrqDiv;					//== 刷新周期分频
		Uint16				RFrqDivCnt;				//== 分频计数
				
		Uint8					LAxisCntX;				//== 显示缓存字符序号
		Uint8		 			LAxisMaxX;				//== 显示缓存字符序号最大值
		Uint8		 			LAxisCntY;				//== 显示缓存字符字内列号
		Uint8		 			LAxisMaxY;				//== 显示缓存字符字内列号最大值
		
		Uint8					RAxisCntX;				//== 旋转LED显示缓存字符序号
		Uint8		 			RAxisMaxX;				//== 旋转LED显示横坐标最大限制
		Uint8		 			RAxisCntY;				//== 旋转LED显示缓存字符字内列号
		Uint8		 			RAxisMaxY;				//== 旋转LED显示缓存字符字内列号最大值

		Uint16    		LBuf[LMAXX+1]; 				//== 线性LED显示存储数组
    TYPE_COLOR    RBuf[RMAXX+1][RMAXY]; //== 旋转LED显示存储数组 带RGB
				
		Uint16				RunTime;
		
}TYPE_LED_LED;

extern TYPE_LED_LED LED;

extern const Uint8 Pic1[RMAXX][RMAXY*3];
extern const Uint8 Pic2[RMAXX][RMAXY*3];
extern const Uint8 Pic3[RMAXX][RMAXY*3];

extern void POV_LED_Show(void);

#endif
