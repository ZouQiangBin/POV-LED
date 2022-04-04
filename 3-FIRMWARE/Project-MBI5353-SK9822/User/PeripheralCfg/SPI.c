/*************************************************************************************************/
/*文件：SPI协议										                                                               */
/*记录：By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "SPI.h"		  
					
/*************************************************************************************************/
/* 功能：SPI1功能配置     																								 	 */
/* 输入：无                           																								 					 */
/* 输出：无			                                                                                 */
/* 描述：1、配置SPI1  PA4--CS PA5--SCK  PA6--MISO  PA7--MOSI  主机发送模式                 	 		 */
/*			 2、最大支持30M时钟*/
/*			 3、配置灰度时钟*/
/*************************************************************************************************/
void SPI1_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_SPI1, ENABLE );	//== 使能PA端口和SPI1时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		//=== SPI控制引脚(用宏来处理)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;					//== PA5--SCK PA6--MISO PA7--MOSI
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//== 复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== 初始化PA端口
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;																		//== LE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== 推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== 初始化PA端口
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;											//== PA5--SCK PA6--MISO PA7--MOSI
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== 复用推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
//		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== 初始化PA端口
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5|GPIO_Pin_7);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																		//== 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  											//== 推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
//		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== 初始化PA端口
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 	 								//== 单向单线发送模式
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//== 主机模式
		
#if (DEVICE_SEL == 1)
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;													//== 16bit数据格式
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== 时钟空闲低电平
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
#else
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;														//== 8bit数据格式
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== 时钟空闲低电平
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
#endif
		
		
																	//== 数据捕获于第一个时钟沿
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//== 软件管理片选
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;					//== 通信速率 18M
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//== 高位先传
		SPI_InitStructure.SPI_CRCPolynomial = 7;																		//== CRC7
		SPI_Init(SPI1, &SPI_InitStructure);  																				//== 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器																																		 
		
		DMA_DeInit(DMA1_Channel3); 																									//== 恢复默认值
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&SPI1->DR;               //== 外设地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                          //== 内存->外设
//		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)buf;                         //== 内存地址
//		DMA_InitStructure.DMA_BufferSize = 9;                                       //== 内存缓冲区大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //== 外设地址不自增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //== 内存地址自增
		
#if (DEVICE_SEL == 1)
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //== 外设16bit
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //== 内存16bit
#else
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 		//== 外设8bit
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;         //== 内存8bit
#endif
		
		
		DMA_InitStructure.DMA_Mode =   DMA_Mode_Normal;                             //== 单次触发
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //== 中优先级
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //== 内存到内存关闭
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                //== 初始化
		DMA_Cmd(DMA1_Channel3, DISABLE);																						//== 失能

		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
		SPI_Cmd(SPI1, ENABLE);
} 

/*************************************************************************************************/
/* 功能：SPI2功能配置    																								 	 */
/* 输入：无                           																								 					 */
/* 输出：无			                                                                                 */
/* 描述：PB12--NSS PB13--SCK  PB14--MISO  PB15--MOSI  主机发送模式                 	 			 */
/*************************************************************************************************/
void SPI2_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;


		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//== 使能SPI2时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//== 使能PB端口时钟
																																										 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;																	//== PB12--EN(595_RCLK)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== 推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== 速度50M
		GPIO_Init(GPIOB, &GPIO_InitStructure);																			//== 初始化PA端口
		GPIO_SetBits(GPIOB, GPIO_Pin_12);                                                   
																																										 
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 	 								//== 单向单线发送模式
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//== 主机模式
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;													//== 16bit数据格式
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== 时钟空闲低电平
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//== 数据捕获于第一个时钟沿
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//== 软件管理片选
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;					//== 通信速率 9M
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;													//== 高位先传
		SPI_InitStructure.SPI_CRCPolynomial = 7;																		//== CRC7
		SPI_Init(SPI2, &SPI_InitStructure);  																				//== 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
																																										 
		SPI_Cmd(SPI2, ENABLE); 																											//== 使能SPI外设
																																								    
} 

/*************************************************************************************************/
/* 功能：SPI1写2个字节      																								 	 									 */
/* 输入：TxData--待写入数据                           																					 */
/* 输出：无			                                                                                 */
/* 描述：16个LED状态更新(非占空比)  定时中断更新   	 			 																			 */
/*************************************************************************************************/
void SPI1_WriteByte(Uint16 TxData)
{		
		u8 retry = 0;	

		SPI_I2S_SendData(SPI1, TxData); 				//== 通过外设SPIx发送一个数据		

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
			retry++;
			if(retry>200)	return;
		}
}

/*************************************************************************************************/
/* 功能：SPI1 DMA发送Len个Word																								 	 									 */
/* 输入：数据缓存，长度                           																					 */
/* 输出：无			                                                                                 */
/* 描述：DMA启动发送目标数据   	 			 																			 */
/*************************************************************************************************/
void SPI1_TX_START(Uint16 *Buf, Uint32 Len)
{
		DMA_Cmd(DMA1_Channel3, DISABLE);
		DMA1_Channel3->CMAR  = (Uint32)Buf;
    DMA1_Channel3->CNDTR = Len;  
    DMA_Cmd(DMA1_Channel3, ENABLE);
}

/*************************************************************************************************/
/* 功能：SPI2写2个字节      																								 	 									 */
/* 输入：TxData--待写入数据                           																					 */
/* 输出：无			                                                                                 */
/* 描述：16个LED状态更新(非占空比)  定时中断更新   	 			 																			 */
/*************************************************************************************************/
void SPI2_WriteByte(Uint16 TxData)
{		
		GPIO_SetBits(GPIOB, GPIO_Pin_12);  

		SPI_I2S_SendData(SPI2, TxData); 				//== 通过外设SPIx发送一个数据		
		
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);			//== S9S显示												
}
