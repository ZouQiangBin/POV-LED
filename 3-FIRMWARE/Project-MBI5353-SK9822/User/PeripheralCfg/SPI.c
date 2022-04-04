/*************************************************************************************************/
/*�ļ���SPIЭ��										                                                               */
/*��¼��By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "SPI.h"		  
					
/*************************************************************************************************/
/* ���ܣ�SPI1��������     																								 	 */
/* ���룺��                           																								 					 */
/* �������			                                                                                 */
/* ������1������SPI1  PA4--CS PA5--SCK  PA6--MISO  PA7--MOSI  ��������ģʽ                 	 		 */
/*			 2�����֧��30Mʱ��*/
/*			 3�����ûҶ�ʱ��*/
/*************************************************************************************************/
void SPI1_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_SPI1, ENABLE );	//== ʹ��PA�˿ں�SPI1ʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		//=== SPI��������(�ú�������)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;					//== PA5--SCK PA6--MISO PA7--MOSI
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//== �����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;																		//== LE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== �������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;											//== PA5--SCK PA6--MISO PA7--MOSI
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== �����������
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
//		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5|GPIO_Pin_7);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																		//== 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  											//== �������
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
//		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 	 								//== �����߷���ģʽ
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//== ����ģʽ
		
#if (DEVICE_SEL == 1)
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;													//== 16bit���ݸ�ʽ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== ʱ�ӿ��е͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
#else
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;														//== 8bit���ݸ�ʽ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== ʱ�ӿ��е͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
#endif
		
		
																	//== ���ݲ����ڵ�һ��ʱ����
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//== �������Ƭѡ
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;					//== ͨ������ 18M
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//== ��λ�ȴ�
		SPI_InitStructure.SPI_CRCPolynomial = 7;																		//== CRC7
		SPI_Init(SPI1, &SPI_InitStructure);  																				//== ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���																																		 
		
		DMA_DeInit(DMA1_Channel3); 																									//== �ָ�Ĭ��ֵ
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&SPI1->DR;               //== �����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                          //== �ڴ�->����
//		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)buf;                         //== �ڴ��ַ
//		DMA_InitStructure.DMA_BufferSize = 9;                                       //== �ڴ滺������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //== �����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //== �ڴ��ַ����
		
#if (DEVICE_SEL == 1)
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //== ����16bit
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //== �ڴ�16bit
#else
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 		//== ����8bit
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;         //== �ڴ�8bit
#endif
		
		
		DMA_InitStructure.DMA_Mode =   DMA_Mode_Normal;                             //== ���δ���
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //== �����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //== �ڴ浽�ڴ�ر�
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                //== ��ʼ��
		DMA_Cmd(DMA1_Channel3, DISABLE);																						//== ʧ��

		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
		SPI_Cmd(SPI1, ENABLE);
} 

/*************************************************************************************************/
/* ���ܣ�SPI2��������    																								 	 */
/* ���룺��                           																								 					 */
/* �������			                                                                                 */
/* ������PB12--NSS PB13--SCK  PB14--MISO  PB15--MOSI  ��������ģʽ                 	 			 */
/*************************************************************************************************/
void SPI2_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;


		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//== ʹ��SPI2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//== ʹ��PB�˿�ʱ��
																																										 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;																	//== PB12--EN(595_RCLK)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== �������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
		GPIO_Init(GPIOB, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
		GPIO_SetBits(GPIOB, GPIO_Pin_12);                                                   
																																										 
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 	 								//== �����߷���ģʽ
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//== ����ģʽ
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;													//== 16bit���ݸ�ʽ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== ʱ�ӿ��е͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//== ���ݲ����ڵ�һ��ʱ����
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//== �������Ƭѡ
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;					//== ͨ������ 9M
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;													//== ��λ�ȴ�
		SPI_InitStructure.SPI_CRCPolynomial = 7;																		//== CRC7
		SPI_Init(SPI2, &SPI_InitStructure);  																				//== ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
																																										 
		SPI_Cmd(SPI2, ENABLE); 																											//== ʹ��SPI����
																																								    
} 

/*************************************************************************************************/
/* ���ܣ�SPI1д2���ֽ�      																								 	 									 */
/* ���룺TxData--��д������                           																					 */
/* �������			                                                                                 */
/* ������16��LED״̬����(��ռ�ձ�)  ��ʱ�жϸ���   	 			 																			 */
/*************************************************************************************************/
void SPI1_WriteByte(Uint16 TxData)
{		
		u8 retry = 0;	

		SPI_I2S_SendData(SPI1, TxData); 				//== ͨ������SPIx����һ������		

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
			retry++;
			if(retry>200)	return;
		}
}

/*************************************************************************************************/
/* ���ܣ�SPI1 DMA����Len��Word																								 	 									 */
/* ���룺���ݻ��棬����                           																					 */
/* �������			                                                                                 */
/* ������DMA��������Ŀ������   	 			 																			 */
/*************************************************************************************************/
void SPI1_TX_START(Uint16 *Buf, Uint32 Len)
{
		DMA_Cmd(DMA1_Channel3, DISABLE);
		DMA1_Channel3->CMAR  = (Uint32)Buf;
    DMA1_Channel3->CNDTR = Len;  
    DMA_Cmd(DMA1_Channel3, ENABLE);
}

/*************************************************************************************************/
/* ���ܣ�SPI2д2���ֽ�      																								 	 									 */
/* ���룺TxData--��д������                           																					 */
/* �������			                                                                                 */
/* ������16��LED״̬����(��ռ�ձ�)  ��ʱ�жϸ���   	 			 																			 */
/*************************************************************************************************/
void SPI2_WriteByte(Uint16 TxData)
{		
		GPIO_SetBits(GPIOB, GPIO_Pin_12);  

		SPI_I2S_SendData(SPI2, TxData); 				//== ͨ������SPIx����һ������		
		
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);			//== S9S��ʾ												
}
