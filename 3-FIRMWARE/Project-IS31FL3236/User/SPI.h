/*************************************************************************************************/
/*�ļ���SPIЭ��										                                                               */
/*��¼��By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/


#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

						  	    													  
void SPI1_Init(void);			 						//===��ʼ��SPI��  
void SPI1_WriteByte(u16 TxData);			//===SPI���߶�д2���ֽ�
		 
#endif
