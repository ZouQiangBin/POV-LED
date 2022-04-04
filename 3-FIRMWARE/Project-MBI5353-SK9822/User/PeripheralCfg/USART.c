#include "USART.h"
#include "MBI5353.h"
#include "MENU.h"

Uint32 Baud[]={600,1200,2400,4800,9600,19200,38400,57600,115200};

TYPE_BLUETOOTH BT05 = {0};

void USART2_TX_START(Uint8 *Buf,Uint16 Len);
void USART2_RX_CMD(Uint8 *Buf, Uint16 Len);

/*************************************************************************************************/
/* ���ܣ�����1�������ã�PA9--TX;PA10--RX��      																								 */
/* ���룺bound--������                           																								 */
/* �������			                                                                                 */
/* ����������USART1                     	 */
/*************************************************************************************************/
void USART1_DMA_Init(Uint32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 					   
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);		//== ʹ��USART1��GPIOA�˿�ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);														//== ʹ�ܸ��ù���ʱ��
		                                                                                     
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;																				//== PA9--TX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	//== ���Ź�������Ϊ�����������ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 															//== �ٶ�50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);    																			//== �����������ó�ʼ��PA9
		                                                                                     
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																			//== PA10-RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;														//== ��������ģʽ
		GPIO_Init(GPIOA, &GPIO_InitStructure);																					//== ��ʼ��PA10
		                                                                                     
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);																	//== �ж���������2
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   												//== ָ���ж�ԴUSART1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;												//== ��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;       												//== ָ����Ӧ���ȼ���0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	          											//== ���ж�
		NVIC_Init(&NVIC_InitStructure);							       															//== ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���  
		                                                                                     
		USART_InitStructure.USART_BaudRate = bound;																			//== ���ò�����bound
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//== 8������λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;													//== 1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No ;														//== ����żУ��
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //== Ӳ��������ʧ��
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//== �շ�ģʽ
		USART_Init(USART1, &USART_InitStructure); 																			//== �����������ó�ʼ��USART1
		USART_Cmd(USART1, ENABLE);																											//== ʹ��USART1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 																	//== ʹ��ָ����USART1�����ж�	    
}

/*************************************************************************************************/
/* ���ܣ�����2+DMA�շ���������(PA2--TX;PA3--RX)     																							*/
/* ���룺bound--������                           																								 */
/* �������			                                                                                 */
/* ����������USART2 DMA��ʽ �����ж�+�����ж�+DMA�շ�                   	 */
/*************************************************************************************************/
void USART2_DMA_Init(Uint32 bound)
{
		GPIO_InitTypeDef 	GPIO_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
		DMA_InitTypeDef 	DMA_InitStructure;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART2_Config(bound);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_IRQ0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_IRQ1;
    NVIC_Init(&NVIC_InitStructure);

		//=== DMA1 ͨ��6 USART2_RX
		DMA_DeInit(DMA1_Channel6);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&USART2->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)BT05.RX_Buf;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;												//== ����->�ڴ濪��
		DMA_InitStructure.DMA_BufferSize = USART_BUFFSIZE;												//== ��󻺴�����С(������)
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//== ���ݼĴ������(8bit)
		DMA_InitStructure.DMA_PeripheralInc 		 = DMA_PeripheralInc_Disable;		
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_MemoryInc 		 = DMA_MemoryInc_Enable;	
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;															//== �ڴ�->�ڴ�ر�
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_Init(DMA1_Channel6, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel6, ENABLE);
		
		//=== DMA1 ͨ��7 USART2_TX
		DMA_DeInit(DMA1_Channel7);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&USART2->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)BT05.TX_Buf;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = 0;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_PeripheralInc 		 = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_MemoryInc 		 = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel7, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL7);
			
	
}

/*************************************************************************************************/
/* ���ܣ�����2���ݸ�ʽ��������     																						 */
/* ���룺bound--������                           																								 */
/* �������			                                                                                 */
/* ����������USART2 ���� 1��ʼ+8����+1ֹͣ+����żУ��                    	 */
/*			 ���������жϡ���������ж�*/
/*************************************************************************************************/
void USART2_Config(Uint32 Buad)
{
		USART_InitTypeDef USART_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		USART_Cmd(USART2, DISABLE);
		USART_InitStructure.USART_BaudRate = Buad;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);

		USART_ITConfig(USART2, USART_IT_TC,   ENABLE);  //== ��������ж�ʹ��
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	//== �����ж�
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

		USART_Cmd(USART2, ENABLE);
}


/*************************************************************************************************/
/* ���ܣ�����2�ж�					     																								 */
/* ���룺��                           																								 */
/* �������			                                                                                 */
/* �����������ж�(IDLE)+��������ж�(TC)                    	 */
/*************************************************************************************************/
void USART2_IRQHandler(void)
{
		//=== ��������ж�
		if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
		{
				USART_ClearFlag(USART2, USART_FLAG_TC);
				
				Menu.Flag.bit.DataTxSts = 0;
		}
	
		//=== ���յ�һ֡����
		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
		{
				Uint8 ClearFlag,Len;
				
				//=== �ȹر�ͨ�������⴦����ջ�����ʱ����������
				DMA_Cmd(DMA1_Channel6, DISABLE);
			
				//=== ��������жϱ�־
				ClearFlag = USART2->SR;
				ClearFlag = USART2->DR;
			
				//=== �������ݰ�	
				Len = USART_BUFFSIZE - DMA_GetCurrDataCounter(DMA1_Channel6);
				USART2_RX_CMD(BT05.RX_Buf, Len);
			
				//=== ��������DMA����(�����������ü�����)
				DMA_SetCurrDataCounter(DMA1_Channel6, USART_BUFFSIZE);
				DMA_Cmd(DMA1_Channel6, ENABLE);
		}
}

/*************************************************************************************************/
/* ���ܣ�����2���ݷ�������					     																								 */
/* ���룺Buf-���ͻ�����; Len--֡����                           																								 */
/* �������			                                                                                 */
/* ������USART2 DMA TX                    	 */
/*************************************************************************************************/
void USART2_TX_START(Uint8 *Buf,Uint16 Len)
{
		//=== DMA����Ҫ�ȹأ������ó��ȣ���ʹ��ͨ��
    DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA1_Channel7->CMAR  = (Uint32)Buf;
    DMA1_Channel7->CNDTR = Len;  
    DMA_Cmd(DMA1_Channel7, ENABLE);
}


/*************************************************************************************************/
/* ���ܣ�����2�����������					     																								 */
/* ���룺Buf-���ջ�����; Len--֡����                           																								 */
/* �������			                                                                                 */
/* ��������������֡ʵ����ز���                    	 */
/*************************************************************************************************/
void USART2_RX_CMD(Uint8 *Buf, Uint16 Len)
{
		//=== 1���ж�֡���ֽ��Ƿ����:'B'
		if(Buf[0] == 0x42)
		{
				//=== �Զ�������֡
				switch(Buf[1])
				{
						case 	0x00:		Menu.Rgb.R = Buf[2];	//== 0��RGB����
													Menu.Rgb.G = Buf[3];
													Menu.Rgb.B = Buf[4];
													break;
						case 	0x01:  	Menu.KeySts = Buf[2];	//== 1����������
													break;
						case 	0x02:  	Menu.SnakeRunDiv = Buf[2] >> 4;		//== 2����Ϸ�Ѷ�
													Menu.PicShiftBits = Buf[2] >> 5;	//== ͼƬ����
													break;
						case 	0x03:  	Menu.X1 = Buf[2];	//== 3������/����X1/Y1/X2/Y2
													Menu.Y1 = Buf[3];
													Menu.X2 = Buf[4];
													Menu.Y2 = Buf[5];
													if(!Menu.DrawLineSel) Menu.DrawLineSel = Buf[6];
													break;
						case 	0x04:  	LED.RFrqDiv = Buf[2];	//== 4����Ƶ���ơ�ģʽѡ��
													break;
						case 	0x05:  	Menu.Flag.bit.ScrollMode = Buf[2];	//== 5������ģʽ
													if(Menu.Flag.bit.ScrollMode)	LED.ScrollDiv = (Uint16)Buf[3] * 10 / 120; //���10��Ƶ
													else													LED.ScrollDiv = Buf[3];
													break;
						case 	0x06:  	//=== 6��״̬��־
													Menu.Flag.bit.SnakeReStart = Buf[3];	//== ��λ��Ϸ
													break;
													
						case  0x10:		//=== ���Ĵ��ڲ�����(����ģ��Ͽ�APP����)
													BT05.flag.bit.BaudSet = Buf[2] & 0x0F;
													BT05.flag.bit.status = 1;
													break;

						default:			break;
				}
				
				//=== ���ؽӵ������ݣ���ʾ���ճɹ�
//				USART2_TX_START(BT05.RX_Buf, USART_BUFFSIZE);
		}
		//=== 2���ж�AT����ָ���Ƿ�ظ���ȷ����ȷ�����
		else if(Buf[0] == 'O' && Buf[1] == 'K' && Buf[2] == '\r' && Buf[3] == '\n')
		{
//				BT05.flag.bit.status = 2;
		}
		//=== 3���յ�����ģ�鲨�����޸���ɣ����ô��ڼĴ���������
		else if(Buf[0] == '+' && Buf[1] == 'B' && Buf[2] == 'A' && Buf[3] == 'U' && Buf[4] == 'D')
		{
//				BT05.flag.bit.status = 3;
		}

}
