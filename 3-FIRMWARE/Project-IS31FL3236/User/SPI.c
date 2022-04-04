/*************************************************************************************************/
/*文件：SPI协议										                                                               */
/*记录：By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "SPI.h"
#include "main.h"	
#include "DELAY.h"			  
					
/*************************************************************************************************/
/* 功能：SPI1功能配置（PA9--TX;PA10--RX）      																								 	 */
/* 输入：无                           																								 					 */
/* 输出：无			                                                                                 */
/* 描述：配置SPI1  PA5--SCK  PA6--EN(595_RCLK)  PA7--MOSI  主机发送模式                 	 			 */
/*************************************************************************************************/
void SPI1_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	//== 使能PA端口和SPI1时钟
																																										 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;											//== PA5--SCK PA7--MOSI
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//== 复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== 初始化PA端口
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																		//== PA6--EN(595_RCLK)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== 推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== 初始化PA端口
		GPIO_SetBits(GPIOA, GPIO_Pin_6);                                                   
																																										 
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 	 								//== 单向单线发送模式
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//== 主机模式
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;													//== 16bit数据格式
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== 时钟空闲低电平
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//== 数据捕获于第一个时钟沿
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//== 软件管理片选
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;					//== 通信速率 9M
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;													//== 高位先传
		SPI_InitStructure.SPI_CRCPolynomial = 7;																		//== CRC7
		SPI_Init(SPI1, &SPI_InitStructure);  																				//== 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
																																										 
		SPI_Cmd(SPI1, ENABLE); 																											//== 使能SPI外设
																																								    
}  


/*************************************************************************************************/
/* 功能：SPI1写2个字节      																								 	 									 */
/* 输入：TxData--待写入数据                           																					 */
/* 输出：无			                                                                                 */
/* 描述：16个LED状态更新(非占空比)  定时中断更新   	 			 																			 */
/*************************************************************************************************/
void SPI1_WriteByte(Uint16 TxData)
{		
		GPIO_SetBits(GPIOA, GPIO_Pin_6);  

		SPI_I2S_SendData(SPI1, TxData); 				//== 通过外设SPIx发送一个数据		
		
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);			//== S9S显示												
}


