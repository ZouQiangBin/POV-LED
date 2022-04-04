
#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define	DEVICE_SEL 			2								//== ICѡ��1-MBI5353 else-SK9822

#if (DEVICE_SEL == 1)
		#define DEVICES			3								//== ������MBI5353����
		#define RGB_NUM			DEVICES*16			//== ����MBI5353������16��RGB
		
		#define		RMAXX		(96)							//== ��תLED������󳤶� ��������״̬ 16��
#else
		#define DEVICES			40							//== ������SK9822����
		#define RGB_NUM			DEVICES*1				//== ����SK9822������1RGB��
		
		#define		RMAXX		(120)							//== ��תLED������󳤶� ��������״̬ 16��
#endif

//#define		RMAXX		(120)						//== ��תLED������󳤶� ��������״̬ 16��
#define		RMAXY		RGB_NUM					//== ��תLED��������� 48��RGB 144��LED RGBֵ
#define		LMAXX		(RMAXX)					//== ����LED�������̰����
#define		LMAXY		16							//== ����LED��������� 16bit

#define ALED_RST	0								//== ��ռ���ȼ�
#define ALED_RST1	0								//== �����ȼ�
						
#define LLED_IRQ	1
#define LLED_IRQ1	1

#define RLED_IRQ	1								//== ��תLEDˢ�����ȼ�
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

//=== ѭ����λ
#define CycL8(val,n)	(val >> (8 - n)  | (val << n))
#define CycR8(val,n)  (val << (8 - n)  | (val >> n))
#define CycL16(val,n)	(val >> (16 - n) | (val << n))
#define CycR16(val,n) (val << (16 - n) | (val >> n))
#define CycL32(val,n)	(val >> (32 - n) | (val << n))
#define CycR32(val,n) (val << (32 - n) | (val >> n))

//=== ��λ����8λ
#define Reverse8C(data)	(((data&0xf0)>>4) | ((data&0x0f)<<4))
#define Reverse8B(data)	(((data&0xCC)>>2) | ((data&0x33)<<2))
#define Reverse8A(data)	(((data&0xAA)>>1) | ((data&0x55)<<1))
#define Reverse8(data)	(Reverse8A(Reverse8B(Reverse8C(data))))

//=== ��λ����16λ
#define Reverse16D(data) (((data & 0xFF00) >> 8) | ((data & 0x00FF) << 8))
#define Reverse16C(data) (((data & 0xF0F0) >> 4) | ((data & 0x0F0F) << 4))
#define Reverse16B(data) (((data & 0xCCCC) >> 2) | ((data & 0x3333) << 2))
#define Reverse16A(data) (((data & 0xAAAA) >> 1) | ((data & 0x5555) << 1))
#define Reverse16(data)	 (Reverse16A(Reverse16B(Reverse16C(Reverse16D(data)))))

typedef enum {TURE = 1, FALSE = 0}STATUS;
typedef enum {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, OK = 4, STOP = 5, ELSE = 6}DIR;

//=== ̰���߽ṹ������
typedef struct{
		Uint8		Sts;			//== ��ǰ�ڵ������ռ��־
		Uint8 	X;
		Uint8		Y;
}TYPE_POS;

//=== ̰���߽ṹ��������ɫ
typedef struct{
		Uint8		R;
		Uint8		G;
		Uint8		B;
}TYPE_COLOR;

//=== ̰���߽ṹ��ڵ�
typedef struct snake{

		TYPE_POS				Pos;
		TYPE_COLOR			Color;
		struct snake* 	Next;

}Snake, *pSnake;

//=== ���ṹ��
typedef struct{
		
		int32					Speed;
		int32					SpeedPrd;
		Uint16				AdcRaw[2];
		Uint8  				ScrollDiv;		
	
		Uint8 				LFrqDiv;					//== ����LEDˢ�����ڷ�Ƶ
		Uint8					LFrqDivCnt;				//== ��Ƶ����
		Uint16 				RFrqDiv;					//== ˢ�����ڷ�Ƶ
		Uint16				RFrqDivCnt;				//== ��Ƶ����
				
		Uint8					LAxisCntX;				//== ��ʾ�����ַ����
		Uint8		 			LAxisMaxX;				//== ��ʾ�����ַ�������ֵ
		Uint8		 			LAxisCntY;				//== ��ʾ�����ַ������к�
		Uint8		 			LAxisMaxY;				//== ��ʾ�����ַ������к����ֵ
		
		Uint8					RAxisCntX;				//== ��תLED��ʾ�����ַ����
		Uint8		 			RAxisMaxX;				//== ��תLED��ʾ�������������
		Uint8		 			RAxisCntY;				//== ��תLED��ʾ�����ַ������к�
		Uint8		 			RAxisMaxY;				//== ��תLED��ʾ�����ַ������к����ֵ

		Uint16    		LBuf[LMAXX+1]; 				//== ����LED��ʾ�洢����
    TYPE_COLOR    RBuf[RMAXX+1][RMAXY]; //== ��תLED��ʾ�洢���� ��RGB
				
		Uint16				RunTime;
		
}TYPE_LED_LED;

extern TYPE_LED_LED LED;

extern const Uint8 Pic1[RMAXX][RMAXY*3];
extern const Uint8 Pic2[RMAXX][RMAXY*3];
extern const Uint8 Pic3[RMAXX][RMAXY*3];

extern void POV_LED_Show(void);

#endif
