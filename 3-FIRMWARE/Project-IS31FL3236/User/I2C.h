#ifndef __I2C_H
#define __I2C_H

#include "main.h"
#include "stm32f10x.h"
#include "DELAY.h"

/********************IO口操作宏定义*****************************/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

/*********************IO口地址映射******************************/
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //== 输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //== 输入 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //== 输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //== 输入 


/**********************IO操作函数*******************************/
#define IIC_SCL    PBout(6)           //== SCL
#define IIC_SDA    PBout(7)           //== SDA	 
#define READ_SDA   PBin(7)            //== 输入SDA 

/*********************IIC所有操作函数***************************/
void  IIC_Init(void);         				//== 初始化IIC的IO口	
Uint8 IIC_Read (Uint8 addr, Uint8 reg, Uint8 len, Uint8 *buf);
Uint8 IIC_Write(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *data);

extern void IIC_Write_One_Byte(Uint8 addr, Uint8 reg, Uint8 data); //== IIC向指定设备指定寄存器写入指定数据

#endif

//------------------End of File----------------------------
