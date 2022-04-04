#include "USART.h"
#include "IS31FL3236.h"
#include "string.h"
#include "stdio.h"

#define WATI_TIME	360000

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
/* ���ܣ�����2+DMA�շ���������(PA9--TX;PA10--RX)     																							*/
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
/* ���ܣ�����2���ݸ�ʽ��������(PA9--TX;PA10--RX)     																						 */
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
/* ���ܣ����ڲ������Լ�     																						 */
/* ���룺                           																								 */
/* �������			                                                                                 */
/* ����������������ģ��ƥ��Ĳ����ʲ�����                    	 */
/*			 				*/
/*************************************************************************************************/
void USART2_BuadSelfCheck(void)
{
		Uint8 	BaudSet = 2, SendCnt = 0;;
		Uint32	TimeCnt = 0;
		
		//=== ����AT����
		while(1)
		{
				//=== ���5ms��һ��
				if(++TimeCnt >= WATI_TIME && 0 == LED.Flag.bit.DataTxSts)
				{
						Uint8 Buf[USART_BUFFSIZE] = AT_TEST;
						
						TimeCnt = 0;

						USART2_Config(Baud[BaudSet++]);
						USART2_TX_START(Buf,strlen((char *)Buf));	
						
						//=== �����ʱ������޷��õ���ȷ��AT����Ӧ���˳�����ʾͨ�Ź���
						if(BaudSet > 8)
						{
								BaudSet = 8;
								if(++SendCnt > 10)
								{
										BT05.flag.bit.BaudSet = BaudSet;
										LED.ErrFlag.bit.BleErr = 1;
										break;
								}
						}
				}
				
				//=== �����ȷATָ���Ӧ������״̬
				if(2 == BT05.flag.bit.status)
				{
						BT05.flag.bit.BaudSet = (SendCnt) ? 8 : (BaudSet - 1);
						BT05.flag.bit.status = 0;
						break;
				}
		}
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
				
				LED.Flag.bit.DataTxSts = 0;
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
						case 	0x00:		//=== 0������LED���ƣ�������RGB
													if(Len == 4 && Buf[2] < 144)	IS31_LED.DutyRatio1[Buf[2]] = Buf[3];
													break;
													
						case 	0x01:  	//=== 1������RGB����
													if(Len == 6 && Buf[2] < 48)
													{
															IS31_LED.DutyRatio1[Buf[2]*3 + 0] = Buf[3];
															IS31_LED.DutyRatio1[Buf[2]*3 + 1] = Buf[4];
															IS31_LED.DutyRatio1[Buf[2]*3 + 2] = Buf[5];
													}
													break;
						case 	0x02:  	//=== 2������LED����
													if(Len == 4 && Buf[2] < 4)
													{
															for(Uint8 i = 0; i < 36; i++)
															{
																	IS31_LED.DutyRatio1[Buf[2]*36 + i] = Buf[3];
															}
													}
						case 	0x03:  	//=== 3������RGB����
													if(Len == 6 && Buf[2] < 4)
													{
															for(Uint8 i = 0; i < 36; i++)
															{
																	IS31_LED.DutyRatio1[Buf[2]*36 + i + 0] = Buf[3];
																	IS31_LED.DutyRatio1[Buf[2]*36 + i + 1] = Buf[4];
																	IS31_LED.DutyRatio1[Buf[2]*36 + i + 2] = Buf[5];
															}
													}
						case 	0x04:  	//=== 4������LED����
													if(Len == 3)
													{
															for(Uint8 i = 0; i < 36; i++)
															{
																	IS31_LED.DutyRatio1[i] = Buf[2];
																	IS31_LED.DutyRatio2[i] = Buf[2];
																	IS31_LED.DutyRatio3[i] = Buf[2];
																	IS31_LED.DutyRatio4[i] = Buf[2];
															}
													}
						case 	0x05:  	//=== 6������RGB����
													if(Len == 5)
													{
															for(Uint8 i = 0; i < 12; i++)
															{
																	IS31_LED.DutyRatio1[i*3 + 0] = Buf[2];
																	IS31_LED.DutyRatio1[i*3 + 1] = Buf[3];
																	IS31_LED.DutyRatio1[i*3 + 2] = Buf[4];
																	
																	IS31_LED.DutyRatio2[i*3 + 0] = Buf[2];
																	IS31_LED.DutyRatio2[i*3 + 1] = Buf[3];
																	IS31_LED.DutyRatio2[i*3 + 2] = Buf[4];
																	
																	IS31_LED.DutyRatio3[i*3 + 0] = Buf[2];
																	IS31_LED.DutyRatio3[i*3 + 1] = Buf[3];
																	IS31_LED.DutyRatio3[i*3 + 2] = Buf[4];
																	
																	IS31_LED.DutyRatio4[i*3 + 0] = Buf[2];
																	IS31_LED.DutyRatio4[i*3 + 1] = Buf[3];
																	IS31_LED.DutyRatio4[i*3 + 2] = Buf[4];
															}
													}
													break;
						
						case 	0x06:  	//=== 7������RGB����
													if(Buf[2])	GPIOB->BSRR = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
													else				GPIOB->BRR  = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
													break;

						case 	0x07:  	//=== 8����Ƶ���ơ�ģʽѡ��
													LED.LFrqDiv = Buf[2];
													LED.RFrqDiv = Buf[3];
													LED.Flag.bit.LMode = Buf[4];
													LED.Flag.bit.RMode = Buf[5];	
													
													break;
						case 	0x08:  	//=== 9�����˶�״̬��־ 0-��	1-�� 2-�� 3-�� �����߼�
													switch(LED.Flag.bit.SnakeRunDirOld)
													{
															case UP:			//== �����˶�(֮ǰ���������˶�)
																						if(Buf[2] != DOWN)	LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															case DOWN: 		//== �����˶�
																						if(Buf[2] != UP)		LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															case LEFT: 		//== �����˶� ��ʼ��������ͷ���
																						if(Buf[2] != RIGHT)	LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															case RIGHT:		//== �����˶�
																						if(Buf[2] != LEFT)	LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															default:			//== ���˶�
																						LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
													}
													break;
						case 	0x09:  	//=== 10��״̬��־
													LED.Flag.bit.SnakeReStart = Buf[3];	//== ��λ��Ϸ
													LED.Flag.bit.RUpdateCtrl = Buf[4];	//== ��ˢ�¿���
													break;
													
						case 0x10:		//=== ���Ĵ��ڲ�����(����ģ��Ͽ�APP����)
													BT05.flag.bit.BaudSet = Buf[2] & 0x0F;
													BT05.flag.bit.status = 1;
													break;

						default:			break;
				}
				
				//=== ���ؽӵ������ݣ���ʾ���ճɹ�
				USART2_TX_START(BT05.RX_Buf, USART_BUFFSIZE);
		}
		//=== 2���ж�AT����ָ���Ƿ�ظ���ȷ����ȷ�����
		else if(Buf[0] == 'O' && Buf[1] == 'K' && Buf[2] == '\r' && Buf[3] == '\n')
		{
				BT05.flag.bit.status = 2;
		}
		//=== 3���յ�����ģ�鲨�����޸���ɣ����ô��ڼĴ���������
		else if(Buf[0] == '+' && Buf[1] == 'B' && Buf[2] == 'A' && Buf[3] == 'U' && Buf[4] == 'D')
		{
				BT05.flag.bit.status = 3;
		}

}

/*************************************************************************************************/
/* ���ܣ�����ģ��BT05 ATָ��ʵ��					     																								 */
/* ���룺����ģ��ṹ��                           																								 */
/* �������			                                                                                 */
/* ��������ѭ������	������������ʵ������ATָ�� �κ�һ�����������ʱ                  	 */
/*************************************************************************************************/
void BT05_Config(TYPE_BLUETOOTH *p)
{
		static Uint32 TimeCnt = 0;

		//=== 1���ȷ���AT�������ȷ����������ģ����Խ���ATָ�� 
		if(1 == p->flag.bit.status && ++TimeCnt > 10)
		{
				Uint8 Buf[USART_BUFFSIZE] = AT_TEST;
				USART2_TX_START(Buf,strlen((char *)Buf));
				TimeCnt = 0;
		}
		//=== 2�����Գɹ�������AT����
		else if(2 == p->flag.bit.status && ++TimeCnt > 10)
		{
				sprintf(BT05.AT_Buf, "AT+BAUD%d\r\n",BT05.flag.bit.BaudSet);
				USART2_TX_START((Uint8 *)p->AT_Buf,strlen(p->AT_Buf));
				TimeCnt = 0;
		}
		//=== 3��������ã��޸Ĵ��ڲ�����
		else if(3 == p->flag.bit.status && ++TimeCnt > 10)
		{
//				Uint8 Buf[USART_BUFFSIZE] = AT_RESET;
//				USART2_TX_START(Buf,strlen((char *)Buf));
//		
				USART2_Config(Baud[BT05.flag.bit.BaudSet]);									//== �������ڲ�����
				p->flag.bit.status = 0;
				TimeCnt = 0;
		}
		//=== 4���ֶ�������ģ�鲨����
		else if(4 == p->flag.bit.status)
		{
				sprintf(BT05.AT_Buf, "AT+BAUD%d\r\n",p->flag.bit.BaudSet);
				USART2_TX_START((Uint8 *)p->AT_Buf,strlen(p->AT_Buf));
				p->flag.bit.status = 0;
				
		}
		else if(5 == p->flag.bit.status)
		{
				Uint8 Buf[USART_BUFFSIZE] = AT_DEFAULT;
				USART2_TX_START(Buf,strlen((char *)Buf));

				p->flag.bit.status = 0;
		}
}
