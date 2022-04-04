#ifndef __MBI5353_H
#define	__MBI5353_H

#include "main.h"

#define CMD_STOP_ERR_CHECK_REG	0x01
#define CMD_DATA_LATCH					0x01
#define CMD_DATA_DISPLAY				0x02
#define CMD_WRIT_GLOBAL_REG			0x04		//== ��Ҫ���CMD_CONFIRM
#define CMD_READ_GLOBAL_REG			0x05
#define CMD_READ_RED_REG				0x06
#define CMD_READ_GREEN_REG			0x07
#define CMD_READ_BLUE_REG				0x08
#define CMD_OPEN_ERR_ENABLE			0x09		//== ��Ҫ���CMD_STOP_ERR_CHECK_REG
#define CMD_SOFTWARE_RESET			0x0A
#define CMD_ALL_OUTPUT_ENABLE		0x0B		//== ��Ҫ���CMD_CONFIRM
#define CMD_ALL_OUTPUT_DISABLE	0x0C		//== ��Ҫ���CMD_CONFIRM
#define CMD_SHOT_ERR_ENABLE			0x0D		//== ��Ҫ���CMD_STOP_ERR_CHECK_REG
#define CMD_CONFIRM							0x0E


typedef struct{
//		Uint16		bit0:				1;		//== bit0
//		Uint16		bit1:				1;		//== bit1
//		Uint16		bit2:				1;		//== bit2
//		Uint16		bit3:				1;		//== bit3
//		Uint16		bit4:				1;		//== bit4
//		Uint16		bit5:				1;		//== bit5
//		Uint16		bit6:				1;		//== bit6
//		Uint16		bit7:				1;		//== bit7
//		Uint16		bit8:				1;		//== bit8
//		Uint16		bit9:				1;		//== bit9
//		Uint16		bit10:			1;		//== bit10
//		Uint16		bit11:			1;		//== bit11
//		Uint16		bit12:			1;		//== bit12
//		Uint16		bit13:			1;		//== bit13
//		Uint16		bit14:			1;		//== bit14
//		Uint16		bit15:			1;		//== bit15
		Uint8 ByteL;
		Uint8 ByteH;
}TYPE_WORD;

//=== ���ص�
typedef struct{
		Uint16		R;
		Uint16		G;
		Uint16		B;
}TYPE_PIXEL16;

typedef union{
		TYPE_PIXEL16	Pixel[DEVICES];				//== IC1.Pixel[ch]/IC2.Pixel[ch]/IC3.Pixel[ch],ch=0~15
		TYPE_WORD			word[DEVICES][3];
}TYPE_GROUP;

//=== Global Config Reg1
typedef struct{										//==					Default		Discription
		Uint16		Rsvd1:				4;		//== bit0~3:	0000			����
		Uint16		GCLK_Mul_En:	1;    //== bit4~4:	0					GCLK���������أ�0-�ر�
		Uint16		Line_Num:			5;    //== bit5~9:	00011			��Ҫɨ���������00011-4��
		Uint16		SPWM_Mode:		2;    //== bitA~B:	00        SPWMģʽ��00-64�� each has 1024 GCLKs
		Uint16		Gray_Mode:		2;    //== bitC~D:	00        �Ҷ�ģʽ��00-16bit���ݸ�ʽ
    Uint16  	Rsvd2: 				2;    //== bitE~F:	00        ���üĴ���ѡ��00-ȫ��	01-Red 01-Green 01-Blue
}TYPE_GLOBAL_REG1;
typedef union{
    Uint16    				all;
    TYPE_GLOBAL_REG1 	bit;
//		TYPE_WORD					word;
}GLOBAL_REG1_WORD;

//=== Global Config Reg2
typedef struct{										//==					Default		Discription
		Uint16		Power_En:			1;		//== bit0~0:	0					�͹��Ŀ��أ�0-�ر�
		Uint16		Rsvd1:				2;    //== bit1~2:	0					
		Uint16		Delay:				2;    //== bit3~4:	00				�洢����ʱ��00-����ʱ
		Uint16		Rsvd2:				1;    //== bit5~5:	0				
		Uint16		Adj_Gain:			3;    //== bit6~8:	000       ȫ�ֿɵ��������棺1 step=14.3% (100%~200%)
		Uint16		Con_Gain:			2;    //== bit9~A:	11        ȫ�̶ֹ��������棺11-47.6 01-15.8
		Uint16		Rext_Check_En:1;		//== bitB~B:	1					ȫ�����������·��⿪�أ�1-��
    Uint16  	Rsvd3: 				4;    //== bitC~F:	0000        
}TYPE_GLOBAL_REG2;
typedef union{
    Uint16    				all;
    TYPE_GLOBAL_REG2 	bit;
//		TYPE_WORD					word;
}GLOBAL_REG2_WORD;

//=== Global Config Reg3
typedef struct{												//==					Default		Discription
		Uint16		Rsvd1:						10;   //== bit0~9:	0	
		Uint16		BlackPreChargeEn:	1;		//== bitA~A:	0					����Ԥ��翪�أ�0-�ر�
		Uint16		Rsvd2:						5;   	//== bitB~F:	0	
}TYPE_GLOBAL_REG3;
typedef union{
    Uint16    				all;
    TYPE_GLOBAL_REG3 	bit;
//		TYPE_WORD					word;
}GLOBAL_REG3_WORD;

//=== ȫ�����üĴ����ṹ��
typedef struct{
		GLOBAL_REG1_WORD		R1;				//== ȫ�����üĴ���1
		GLOBAL_REG2_WORD		R2;				//== ȫ�����üĴ���2
		GLOBAL_REG3_WORD		R3;				//== ȫ�����üĴ���3
}TYPE_GLOBAL_REG;		

//=== R/G/B Config Reg1/2/3 (��������Ҫ���ú���������ĵ������棬Ĭ��100%)
typedef struct{										//==					Default		Discription
		Uint16		Rsvd1:				2;    //== bit0~1:	11	
		Uint16		CurGain:			7;		//== bit2~8:	1111111	  �������棺1111111-100%
		Uint16		Rsvd2:				1;   	//== bit9~9:	1	
		Uint16		Rsvd3:				4;   	//== bitA~D:	0000	
		Uint16  	Sel: 					2;    //== bitE~F:	00        ���üĴ���ѡ��00-ȫ��	01-Red 01-Green 01-Blue
}TYPE_RGB_REG1;
typedef union{
    Uint16    				all;
    TYPE_RGB_REG1	 		bit;
}TYPE_RGB_REG1_WORD;

//=== ������
typedef union{
    Uint16    all[DEVICES];
}TYPE_ERR_WORD;

typedef struct{
		TYPE_GROUP					LED[16];						//== MBI5353��λ�Ĵ���Ϊ48bit�����������ݴ�����48*DEVICES bits
		TYPE_GLOBAL_REG			Global;//[DEVICES];	//== ȫ�����üĴ��� IC1/2/3
		TYPE_RGB_REG1_WORD	RGB;
		TYPE_ERR_WORD				Flag[DEVICES];	  	//== ȫ�ִ����־����·/��· IC1/2/3
		Uint8								*pLED[DEVICES][48];	//== ָ�����飬�ض������е�ַ
}TYPE_MBI5353;

#define MBI5353_DEFS	{                 		\
/*  LED[16]         */   {0xF000,0x000,0x000,0x000,0xF000,},            			\
/*  Global[3]  			*/   {0x0060,0x0E00,0x0000},  \
/*  RGB		  				*/   0x03FF,            		  \
/*  Flag[3]  				*/   {0},            		  \
/*	*pLED						*/	 {0,},\
};

extern TYPE_MBI5353 MBI5353;

void MBI5353_Init(void);
void MBI5353_Frame_Init(void);
void MBI5353_Write_Config(void);
void MBI5353_Read_Config(void);
void MBI5353_Write_RGB_Config(void);
void MBI5353_Read_Err(Uint8 Type);
void MBI5353_Reset(void);
void MBI5353_Output_En(Uint8 Type);
void MBI5353_Update(void);
void MBI5353_Update_DMA(Uint8 *p);
void MBI5353_Func(Uint8 Sel);
#endif
