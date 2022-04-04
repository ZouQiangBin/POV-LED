/*************************************************************************************************/
/*文件：SPI协议										                                                               */
/*记录：By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/


#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"
#include "main.h"	
//=== IO模拟 设置IO输出
#define SPI_LE_H		{GPIOA->BSRR = GPIO_Pin_4;}
#define SPI_LE_L		{GPIOA->BRR  = GPIO_Pin_4;}
#define SPI_SCK_H		{GPIOA->BSRR = GPIO_Pin_5;}
#define SPI_SCK_L		{GPIOA->BRR  = GPIO_Pin_5;}
#define SPI_SDO_H		{GPIOA->BSRR = GPIO_Pin_6;}
#define SPI_SDO_L		{GPIOA->BRR  = GPIO_Pin_6;}
#define SPI_SDI_H		{GPIOA->BSRR = GPIO_Pin_7;}
#define SPI_SDI_L		{GPIOA->BRR  = GPIO_Pin_7;}
#define GCLK_H			{GPIOA->BSRR = GPIO_Pin_8;}
#define GCLK_L			{GPIOA->BRR  = GPIO_Pin_8;}

//=== IO模拟 设置IO读取
#define SPI_SDO_READ		(GPIOA->IDR & GPIO_Pin_6)

//=== IO控制源选择：GPIO or SPI
#define SPI_CLK_Out_PP	{GPIOA->CRL &=0xFF0FFFFF;;	GPIOA->CRL|=0x00300000;}
#define SPI_CLK_AF_PP		{GPIOA->CRL &=0xFF0FFFFF;;	GPIOA->CRL|=0x00B00000;}
#define SPI_SDO_Out_PP	{GPIOA->CRL &=0xF0FFFFFF;;	GPIOA->CRL|=0x04000000;}
#define SPI_SDO_AF_PP		{GPIOA->CRL &=0xF0FFFFFF;;	GPIOA->CRL|=0x0B000000;}
#define SPI_SDI_Out_PP	{GPIOA->CRL &=0x0FFFFFFF;;	GPIOA->CRL|=0x30000000;}
#define SPI_SDI_AF_PP		{GPIOA->CRL &=0x0FFFFFFF;;	GPIOA->CRL|=0xB0000000;}
						  	    													  
void SPI1_Init(void);			 						//== 初始化SPI1口  
void SPI1_WriteByte(u16 TxData);			//== SPI1总线读写2个字节
void SPI1_TX_START(Uint16 *Buf, Uint32 Len);
void SPI2_Init(void);			 						//== 初始化SPI2口  
void SPI2_WriteByte(u16 TxData);			//== SPI2总线读写2个字节

#endif

