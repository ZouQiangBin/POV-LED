
#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h"

#define POV_LED_EN							//=== ����LED���뿪��
#define	I2C_USE_HARDWARE				//=== I2CӲ��ͨ�ź궨��
#define I2C_SPEED	1000000				//== 800K Ӳ��I2Cͨ���ٶ�

#define ALED_RST	0							//=== ��ռ���ȼ�
#define ALED_RST1	0							//=== �����ȼ�
						
#define LLED_IRQ	1
#define LLED_IRQ1	1

#define RLED_IRQ	1							//=== ��תLEDˢ�����ȼ�
#define RLED_IRQ1	1	

#define RLED_SWT	0
#define RLED_SWT1	0

#define USART2_IRQ0 2
#define USART2_IRQ1 1

#define		SMAXL		(20*8)				//=== ����LED������󳤶� 20���ַ�
#define		LMAXL		(8*8)					//=== ����LED�������̰����
#define		LMAXW		16						//=== ����LED��������� 16bit
#define		RMAXL		(16*2)				//=== ��תLED������󳤶� ��������״̬ 16��
#define		RMAXW		48						//=== ��תLED��������� 48��RGB 144��LED RGBֵ

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
typedef enum {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, STOP = 4}DIR;

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
		Uint32		LMode:4;
		Uint32		RMode:4;
		Uint32		RUpdate:1;					//== ��תLED����ICʹ�ܿ���
		Uint32		RUpdateCtrl:1;			//== ��תLED����ICʹ�ܿ���
		Uint32		SnakeSts:1;					//== ��Ϸ״̬ 0-���� 1-����
		Uint32		SnakeReStart:1;			//== ��Ϸ���³�ʼ�� 0-����ʼ�� 1-��ʼ��
		Uint32		SnakeRunDir:4;			//== ̰�����˶�����
		Uint32		SnakeRunDirOld:4;		//== ̰�����˶�ֹͣǰ���˶�����
		Uint32		LSigStop:1;					//== ��Ȧ���Ʊ���
		Uint32		RSigStop:1;					//== ��Ȧ���Ʊ���
		Uint32		DataTxSts:1;				//== ���ڷ���״̬
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
		
		
		Uint8 				LFrqDiv;						//== ����LEDˢ�����ڷ�Ƶ
		Uint8					LFrqDivCnt;					//== ��Ƶ����
		Uint8					Dltime;							//== ��ʱ
				
		Uint8					LPoint;							//== ��ʾ�����ַ����
		Uint8		 			LPointMax;					//== ��ʾ�����ַ�������ֵ
		Uint8		 			LCnt;								//== ��ʾ�����ַ������к�
		Uint8		 			LCntMax;						//== ��ʾ�����ַ������к����ֵ
		Uint8					RPoint;							//== ��תLED��ʾ�����ַ����
		Uint8		 			RPointMax;					//== ��תLED��ʾ�����ַ�������ֵ
		Uint8		 			RCnt;								//== ��תLED��ʾ�����ַ������к�
		Uint8		 			RCntMax;						//== ��תLED��ʾ�����ַ������к����ֵ
		Uint8					RLine;							//== ��תLED ��
		Uint8					RList;							//== ��תLED ��
				
		Uint16    		LBuf[SMAXL+1]; 				//== ����LED��ʾ�洢����
    TYPE_COLOR    RBuf[RMAXL+1][RMAXW]; //== ��תLED��ʾ�洢���� ��RGB
		
		Uint8 				RFrqDiv;						//== ˢ�����ڷ�Ƶ
		Uint8					RFrqDivCnt;					//== ��Ƶ����
				
		Uint16				RunTime;
		Uint16				RunTime1;
		
		TYPE_COLOR		Color;						 	//== ����������ɫ
		
    LED_WORD  		Flag;               //== LED��ʾ���Ʊ�־
		ERR_WORD			ErrFlag;
		
}TYPE_LED_LED;

extern TYPE_LED_LED LED;

extern void POV_LED_Show(void);

#endif
