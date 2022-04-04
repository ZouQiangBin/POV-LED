/*************************************************************************************************/
/*�ļ���SPIЭ��										                                                               */
/*��¼��By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "SPI.h"
#include "main.h"	
#include "DELAY.h"			  
					
/*************************************************************************************************/
/* ���ܣ�SPI1�������ã�PA9--TX;PA10--RX��      																								 	 */
/* ���룺��                           																								 					 */
/* �������			                                                                                 */
/* ����������SPI1  PA5--SCK  PA6--EN(595_RCLK)  PA7--MOSI  ��������ģʽ                 	 			 */
/*************************************************************************************************/
void SPI1_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	//== ʹ��PA�˿ں�SPI1ʱ��
																																										 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;											//== PA5--SCK PA7--MOSI
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//== �����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																		//== PA6--EN(595_RCLK)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  													//== �������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//== �ٶ�50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);																			//== ��ʼ��PA�˿�
		GPIO_SetBits(GPIOA, GPIO_Pin_6);                                                   
																																										 
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 	 								//== �����߷���ģʽ
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//== ����ģʽ
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;													//== 16bit���ݸ�ʽ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//== ʱ�ӿ��е͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//== ���ݲ����ڵ�һ��ʱ����
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//== �������Ƭѡ
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;					//== ͨ������ 9M
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;													//== ��λ�ȴ�
		SPI_InitStructure.SPI_CRCPolynomial = 7;																		//== CRC7
		SPI_Init(SPI1, &SPI_InitStructure);  																				//== ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
																																										 
		SPI_Cmd(SPI1, ENABLE); 																											//== ʹ��SPI����
																																								    
}  


/*************************************************************************************************/
/* ���ܣ�SPI1д2���ֽ�      																								 	 									 */
/* ���룺TxData--��д������                           																					 */
/* �������			                                                                                 */
/* ������16��LED״̬����(��ռ�ձ�)  ��ʱ�жϸ���   	 			 																			 */
/*************************************************************************************************/
void SPI1_WriteByte(Uint16 TxData)
{		
		GPIO_SetBits(GPIOA, GPIO_Pin_6);  

		SPI_I2S_SendData(SPI1, TxData); 				//== ͨ������SPIx����һ������		
		
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);			//== S9S��ʾ												
}


