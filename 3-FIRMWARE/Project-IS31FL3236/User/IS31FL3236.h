#ifndef __IS31FL3236_H
#define	__IS31FL3236_H

#include "main.h"

//=== IS31FL3236设备地址：仅支持4个设备
#define		IS31FL3236_ADDRESS1	0x78							//=== AD connected to GND, AD = 00;
#define		IS31FL3236_ADDRESS2	0x7A							//=== AD connected to SCL, AD = 01;
#define		IS31FL3236_ADDRESS3	0x7C              //=== AD connected to SDA, AD = 10;
#define		IS31FL3236_ADDRESS4	0x7E              //=== AD connected to VCC, AD = 11;

//=== IS31FL3236功能寄存器地址
#define 	SHUTDOWN 						0x00  						//=== 00h 		Set software shutdown mode
#define 	PWM_OUT(Address)		(Address)					//=== 01h~24h PWM Register 36 channels PWM duty cycle data register
#define 	LED_CTRL(Address)		(Address+0x25)		//=== 26h~49h LED Control Register Channel 1 to 36 enable bit and current setting
#define 	LED_UPDATE					0x25							//=== 25h			PWM Update Register,Load PWM Register(01h~24h) and LED Control Register(26h~49h)
#define 	LED_GLOBAL					0x4A							//=== 4Ah 		Global Control Register Set all channels enable
#define 	PWM_FRQ							0x4B							//=== 4Bh			Set all channels operating frequency
#define 	RESET_IS31					0x4F							//===	4Fh		 	Reset Register Reset all registers into default value

//=== IS31FL3236功能寄存器数据
#define 	SHUTDOWN_ENABLE 		0x00  						//=== 00h 		Close Chip
#define 	SHUTDOWN_DISABLE 		0x01  						//=== 00h 		Enable Chip

#define 	LED_UPDATE_ENABLE		0x00							//=== 25h			Update Duty and OutCurrent data

#define 	LED_IMAX_Div1				0x01							//=== 26h~49h LED Out Current Imax/1 ENABLE
#define 	LED_IMAX_Div2				0x03							//=== 26h~49h LED Out Current Imax/2 ENABLE
#define 	LED_IMAX_Div3				0x05							//=== 26h~49h LED Out Current Imax/3 ENABLE
#define 	LED_IMAX_Div4				0x07							//=== 26h~49h LED Out Current Imax/4 ENABLE

#define 	LED_GLOBAL_ENABLE		0x00							//=== 4Ah 		All LED ENABLE
#define 	LED_GLOBAL_DISABLE	0x01							//=== 4Ah 		All LED DISABLE

#define 	PWM_FRQ_3KHZ				0x00							//=== 4Bh			Set all channels operating frequency 3kHz(默认)
#define 	PWM_FRQ_22KHZ				0x01							//=== 4Bh			Set all channels operating frequency 22kHz

#define 	RESET_IS31_ENABLE		0x00							//===	4Fh		 	写0就是复位芯片

//=== IS31结构体
typedef struct{   
    Uint8  SDB1: 1;
    Uint8  SDB2: 1;
    Uint8  SDB3: 1;
    Uint8  SDB4: 1;
    Uint8  Rsvd: 4;
}IS31_BITS;

typedef union{
    Uint8     all;
    IS31_BITS bit;
}IS31_WORD;

typedef struct{
    Uint8       Mode;
    Uint8       DutyRatio1[36];           //== 第一个占空比数组
    Uint8       DutyRatio2[36];
    Uint8       DutyRatio3[36];
    Uint8       DutyRatio4[36];
    IS31_WORD   Flag;                         //== 控制标志位
}TYPE_IS31_LED;

#define DutyRatio   {0x0f, 0x0f, 0x0f,    0x00, 0x00, 0x00,     0x00, 0x00, 0x00,    0x00, 0x00, 0x00,  \
                         0x00, 0x00, 0x00,    0x07, 0x00, 0x07,     0x00, 0x00, 0x00,    0x00, 0x00, 0x00,  \
                         0x00, 0x00, 0x00,    0x00, 0x00, 0x00,     0x00, 0x00, 0x0f,    0x0f, 0x0f, 0x0f}

//#define DutyRatio   {0xff, 0xff, 0xff,    0xff, 0xff, 0xff,     0xff, 0xff, 0xff,    0xff, 0xff, 0xff,  \
                         0xff, 0xff, 0xff,    0xff, 0xff, 0xff,     0xff, 0xff, 0xff,    0xff, 0xff, 0xff,  \
                         0xff, 0xff, 0xff,    0xff, 0xff, 0xff,     0xff, 0xff, 0xff,    0xff, 0xff, 0xff}
												 
//#define DutyRatio   {0x10, 0x00, 0x00,    0x10, 0x08, 0x00,     0x10, 0x10, 0x00,    0x00, 0x10, 0x00,  \
//                         0x00, 0x10, 0x10,    0x00, 0x00, 0x10,     0x09, 0x00, 0x10,    0x01, 0x01, 0x01,  \
//                         0x03, 0x03, 0x03,    0x06, 0x06, 0x06,     0x09, 0x09, 0x09,    0x12, 0x12, 0x12}

//#define DutyRatio		{0xff,}

#define IS31_LED_DEFS    {                   \
/*  Mode                */   0,              \
/*  DutyRatio1[36]  */   DutyRatio,  \
/*  DutyRatio2[36]  */   DutyRatio,  \
/*  DutyRatio3[36]  */   DutyRatio,  \
/*  DutyRatio4[36]  */   DutyRatio,  \
/*  Flag                */   {0x0F},         \
};

//=== 结构体声明
extern TYPE_IS31_LED   IS31_LED;

//=== 功能函数声明
void POV_LED_GPIO_Init(void);
void POV_LED_Init(Uint8 Device, Uint8* Data);
void POV_LED_Set_Duty(Uint8 Device, Uint8* Data);
void POV_LED_Set_Output(Uint8 Device);
void POV_LED_Update(TYPE_IS31_LED *p);
void POV_LED_Update1(Uint8 *Buf);

#endif
