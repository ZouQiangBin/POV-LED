
#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h"

#define POV_LED_EN							//=== 球形LED编译开关
#define	I2C_USE_HARDWARE				//=== I2C硬件通信宏定义
#define I2C_SPEED	1000000				//== 800K 硬件I2C通信速度

#define ALED_RST	0							//=== 抢占优先级
#define ALED_RST1	0							//=== 从优先级
						
#define LLED_IRQ	1
#define LLED_IRQ1	1

#define RLED_IRQ	1							//=== 旋转LED刷新优先级
#define RLED_IRQ1	1	

#define RLED_SWT	0
#define RLED_SWT1	0

#define USART2_IRQ0 2
#define USART2_IRQ1 1

#define		SMAXL		(20*8)				//=== 线性LED画面最大长度 20个字符
#define		LMAXL		(8*8)					//=== 线性LED画面配合贪吃蛇
#define		LMAXW		16						//=== 线性LED画面最大宽度 16bit
#define		RMAXL		(16*2)				//=== 旋转LED画面最大长度 最清晰的状态 16列
#define		RMAXW		48						//=== 旋转LED画面最大宽度 48个RGB 144个LED RGB值

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
typedef enum {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, STOP = 4}DIR;

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
		Uint32		LMode:4;
		Uint32		RMode:4;
		Uint32		RUpdate:1;					//== 旋转LED驱动IC使能控制
		Uint32		RUpdateCtrl:1;			//== 旋转LED驱动IC使能控制
		Uint32		SnakeSts:1;					//== 游戏状态 0-正常 1-结束
		Uint32		SnakeReStart:1;			//== 游戏重新初始化 0-不初始化 1-初始化
		Uint32		SnakeRunDir:4;			//== 贪吃蛇运动方向
		Uint32		SnakeRunDirOld:4;		//== 贪吃蛇运动停止前的运动方向
		Uint32		LSigStop:1;					//== 单圈控制变量
		Uint32		RSigStop:1;					//== 单圈控制变量
		Uint32		DataTxSts:1;				//== 串口发送状态
    Uint32  	Rsvd: 6;
}LED_BITS;

typedef union{
    Uint32    all;
    LED_BITS 	bit;
}LED_WORD;

typedef struct{   
		Uint16		BleErr:1;
    Uint16  	Rsvd: 15;
}ERR_BITS;

typedef union{
    Uint16    all;
    ERR_BITS 	bit;
}ERR_WORD;

typedef struct{

		int32					SpeedPrd;
		Uint16				AdcRaw[2];
		
		
		Uint8 				LFrqDiv;						//== 线性LED刷新周期分频
		Uint8					LFrqDivCnt;					//== 分频计数
		Uint8					Dltime;							//== 延时
				
		Uint8					LPoint;							//== 显示缓存字符序号
		Uint8		 			LPointMax;					//== 显示缓存字符序号最大值
		Uint8		 			LCnt;								//== 显示缓存字符字内列号
		Uint8		 			LCntMax;						//== 显示缓存字符字内列号最大值
		Uint8					RPoint;							//== 旋转LED显示缓存字符序号
		Uint8		 			RPointMax;					//== 旋转LED显示缓存字符序号最大值
		Uint8		 			RCnt;								//== 旋转LED显示缓存字符字内列号
		Uint8		 			RCntMax;						//== 旋转LED显示缓存字符字内列号最大值
		Uint8					RLine;							//== 旋转LED 行
		Uint8					RList;							//== 旋转LED 列
				
		Uint16    		LBuf[SMAXL+1]; 				//== 线性LED显示存储数组
    TYPE_COLOR    RBuf[RMAXL+1][RMAXW]; //== 旋转LED显示存储数组 带RGB
		
		Uint8 				RFrqDiv;						//== 刷新周期分频
		Uint8					RFrqDivCnt;					//== 分频计数
				
		Uint16				RunTime;
		Uint16				RunTime1;
		
		TYPE_COLOR		Color;						 	//== 单点坐标颜色
		
    LED_WORD  		Flag;               //== LED显示控制标志
		ERR_WORD			ErrFlag;
		
}TYPE_LED_LED;

extern TYPE_LED_LED LED;

extern void POV_LED_Show(void);

#endif
