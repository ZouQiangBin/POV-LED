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
/* 功能：串口1功能配置（PA9--TX;PA10--RX）      																								 */
/* 输入：bound--波特率                           																								 */
/* 输出：无			                                                                                 */
/* 描述：配置USART1                     	 */
/*************************************************************************************************/
void USART1_DMA_Init(Uint32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 					   
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);		//== 使能USART1和GPIOA端口时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);														//== 使能复用功能时钟
		                                                                                     
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;																				//== PA9--TX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	//== 引脚工作设置为复用推挽输出模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 															//== 速度50M
		GPIO_Init(GPIOA, &GPIO_InitStructure);    																			//== 根据以上配置初始化PA9
		                                                                                     
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																			//== PA10-RX
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;														//== 浮空输入模式
		GPIO_Init(GPIOA, &GPIO_InitStructure);																					//== 初始化PA10
		                                                                                     
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);																	//== 中断向量分组2
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   												//== 指定中断源USART1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;												//== 抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;       												//== 指定响应优先级别0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	          											//== 打开中断
		NVIC_Init(&NVIC_InitStructure);							       															//== 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器  
		                                                                                     
		USART_InitStructure.USART_BaudRate = bound;																			//== 设置波特率bound
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//== 8个数据位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;													//== 1个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No ;														//== 无奇偶校验
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //== 硬件流控制失能
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//== 收发模式
		USART_Init(USART1, &USART_InitStructure); 																			//== 根据以上配置初始化USART1
		USART_Cmd(USART1, ENABLE);																											//== 使能USART1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 																	//== 使能指定的USART1接收中断	    
}

/*************************************************************************************************/
/* 功能：串口2+DMA收发功能配置(PA9--TX;PA10--RX)     																							*/
/* 输入：bound--波特率                           																								 */
/* 输出：无			                                                                                 */
/* 描述：配置USART2 DMA方式 空闲中断+发送中断+DMA收发                   	 */
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

		//=== DMA1 通道6 USART2_RX
		DMA_DeInit(DMA1_Channel6);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&USART2->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)BT05.RX_Buf;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;												//== 外设->内存开启
		DMA_InitStructure.DMA_BufferSize = USART_BUFFSIZE;												//== 最大缓存区大小(接收区)
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//== 数据寄存器宽度(8bit)
		DMA_InitStructure.DMA_PeripheralInc 		 = DMA_PeripheralInc_Disable;		
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_MemoryInc 		 = DMA_MemoryInc_Enable;	
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;															//== 内存->内存关闭
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_Init(DMA1_Channel6, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel6, ENABLE);
		
		//=== DMA1 通道7 USART2_TX
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
/* 功能：串口2数据格式配置配置(PA9--TX;PA10--RX)     																						 */
/* 输入：bound--波特率                           																								 */
/* 输出：无			                                                                                 */
/* 描述：配置USART2 配置 1起始+8数据+1停止+无奇偶校验                    	 */
/*			 开启空闲中断、发送完成中断*/
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

		USART_ITConfig(USART2, USART_IT_TC,   ENABLE);  //== 发送完成中断使能
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	//== 空闲中断
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

		USART_Cmd(USART2, ENABLE);
}


/*************************************************************************************************/
/* 功能：串口波特率自检     																						 */
/* 输入：                           																								 */
/* 输出：无			                                                                                 */
/* 描述：查找与蓝牙模块匹配的波特率并设置                    	 */
/*			 				*/
/*************************************************************************************************/
void USART2_BuadSelfCheck(void)
{
		Uint8 	BaudSet = 2, SendCnt = 0;;
		Uint32	TimeCnt = 0;
		
		//=== 启动AT测试
		while(1)
		{
				//=== 间隔5ms发一次
				if(++TimeCnt >= WATI_TIME && 0 == LED.Flag.bit.DataTxSts)
				{
						Uint8 Buf[USART_BUFFSIZE] = AT_TEST;
						
						TimeCnt = 0;

						USART2_Config(Baud[BaudSet++]);
						USART2_TX_START(Buf,strlen((char *)Buf));	
						
						//=== 波特率遍历后无法得到正确的AT测试应答，退出并提示通信故障
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
				
				//=== 获得正确AT指令回应：清零状态
				if(2 == BT05.flag.bit.status)
				{
						BT05.flag.bit.BaudSet = (SendCnt) ? 8 : (BaudSet - 1);
						BT05.flag.bit.status = 0;
						break;
				}
		}
}

/*************************************************************************************************/
/* 功能：串口2中断					     																								 */
/* 输入：无                           																								 */
/* 输出：无			                                                                                 */
/* 描述：空闲中断(IDLE)+发送完成中断(TC)                    	 */
/*************************************************************************************************/
void USART2_IRQHandler(void)
{
		//=== 发送完成中断
		if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
		{
				USART_ClearFlag(USART2, USART_FLAG_TC);
				
				LED.Flag.bit.DataTxSts = 0;
		}
	
		//=== 接收到一帧数据
		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
		{
				Uint8 ClearFlag,Len;
				
				//=== 先关闭通道，避免处理接收缓冲区时又来新数据
				DMA_Cmd(DMA1_Channel6, DISABLE);
			
				//=== 清除空闲中断标志
				ClearFlag = USART2->SR;
				ClearFlag = USART2->DR;
			
				//=== 分析数据包	
				Len = USART_BUFFSIZE - DMA_GetCurrDataCounter(DMA1_Channel6);
				USART2_RX_CMD(BT05.RX_Buf, Len);
			
				//=== 重新启动DMA接收(必须重新设置计数器)
				DMA_SetCurrDataCounter(DMA1_Channel6, USART_BUFFSIZE);
				DMA_Cmd(DMA1_Channel6, ENABLE);
		}
}

/*************************************************************************************************/
/* 功能：串口2数据发送启动					     																								 */
/* 输入：Buf-发送缓存区; Len--帧长度                           																								 */
/* 输出：无			                                                                                 */
/* 描述：USART2 DMA TX                    	 */
/*************************************************************************************************/
void USART2_TX_START(Uint8 *Buf,Uint16 Len)
{
		//=== DMA发送要先关，再设置长度，再使能通道
    DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA1_Channel7->CMAR  = (Uint32)Buf;
    DMA1_Channel7->CNDTR = Len;  
    DMA_Cmd(DMA1_Channel7, ENABLE);
}


/*************************************************************************************************/
/* 功能：串口2接收命令分析					     																								 */
/* 输入：Buf-接收缓存区; Len--帧长度                           																								 */
/* 输出：无			                                                                                 */
/* 描述：根据命令帧实现相关操作                    	 */
/*************************************************************************************************/
void USART2_RX_CMD(Uint8 *Buf, Uint16 Len)
{
		//=== 1、判断帧首字节是否等于:'B'
		if(Buf[0] == 0x42)
		{
				//=== 自定义命令帧
				switch(Buf[1])
				{
						case 	0x00:		//=== 0、单个LED控制，不区分RGB
													if(Len == 4 && Buf[2] < 144)	IS31_LED.DutyRatio1[Buf[2]] = Buf[3];
													break;
													
						case 	0x01:  	//=== 1、单个RGB控制
													if(Len == 6 && Buf[2] < 48)
													{
															IS31_LED.DutyRatio1[Buf[2]*3 + 0] = Buf[3];
															IS31_LED.DutyRatio1[Buf[2]*3 + 1] = Buf[4];
															IS31_LED.DutyRatio1[Buf[2]*3 + 2] = Buf[5];
													}
													break;
						case 	0x02:  	//=== 2、单组LED控制
													if(Len == 4 && Buf[2] < 4)
													{
															for(Uint8 i = 0; i < 36; i++)
															{
																	IS31_LED.DutyRatio1[Buf[2]*36 + i] = Buf[3];
															}
													}
						case 	0x03:  	//=== 3、单组RGB控制
													if(Len == 6 && Buf[2] < 4)
													{
															for(Uint8 i = 0; i < 36; i++)
															{
																	IS31_LED.DutyRatio1[Buf[2]*36 + i + 0] = Buf[3];
																	IS31_LED.DutyRatio1[Buf[2]*36 + i + 1] = Buf[4];
																	IS31_LED.DutyRatio1[Buf[2]*36 + i + 2] = Buf[5];
															}
													}
						case 	0x04:  	//=== 4、所有LED控制
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
						case 	0x05:  	//=== 6、所有RGB控制
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
						
						case 	0x06:  	//=== 7、所有RGB控制
													if(Buf[2])	GPIOB->BSRR = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
													else				GPIOB->BRR  = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
													break;

						case 	0x07:  	//=== 8、分频控制、模式选择
													LED.LFrqDiv = Buf[2];
													LED.RFrqDiv = Buf[3];
													LED.Flag.bit.LMode = Buf[4];
													LED.Flag.bit.RMode = Buf[5];	
													
													break;
						case 	0x08:  	//=== 9、蛇运动状态标志 0-上	1-下 2-左 3-右 互斥逻辑
													switch(LED.Flag.bit.SnakeRunDirOld)
													{
															case UP:			//== 向上运动(之前不是向下运动)
																						if(Buf[2] != DOWN)	LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															case DOWN: 		//== 向下运动
																						if(Buf[2] != UP)		LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															case LEFT: 		//== 向左运动 初始蛇身在蛇头左侧
																						if(Buf[2] != RIGHT)	LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															case RIGHT:		//== 向右运动
																						if(Buf[2] != LEFT)	LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
															default:			//== 不运动
																						LED.Flag.bit.SnakeRunDir = Buf[2];
																						break;
													}
													break;
						case 	0x09:  	//=== 10、状态标志
													LED.Flag.bit.SnakeReStart = Buf[3];	//== 复位游戏
													LED.Flag.bit.RUpdateCtrl = Buf[4];	//== 点刷新控制
													break;
													
						case 0x10:		//=== 更改串口波特率(必须模块断开APP链接)
													BT05.flag.bit.BaudSet = Buf[2] & 0x0F;
													BT05.flag.bit.status = 1;
													break;

						default:			break;
				}
				
				//=== 返回接到的数据，表示接收成功
				USART2_TX_START(BT05.RX_Buf, USART_BUFFSIZE);
		}
		//=== 2、判断AT测试指令是否回复正确，正确后进入
		else if(Buf[0] == 'O' && Buf[1] == 'K' && Buf[2] == '\r' && Buf[3] == '\n')
		{
				BT05.flag.bit.status = 2;
		}
		//=== 3、收到蓝牙模块波特率修改完成，配置串口寄存器波特率
		else if(Buf[0] == '+' && Buf[1] == 'B' && Buf[2] == 'A' && Buf[3] == 'U' && Buf[4] == 'D')
		{
				BT05.flag.bit.status = 3;
		}

}

/*************************************************************************************************/
/* 功能：蓝牙模块BT05 AT指令实现					     																								 */
/* 输入：蓝牙模块结构体                           																								 */
/* 输出：无			                                                                                 */
/* 描述：主循环调度	根据蓝牙接收实现特殊AT指令 任何一个步骤必须延时                  	 */
/*************************************************************************************************/
void BT05_Config(TYPE_BLUETOOTH *p)
{
		static Uint32 TimeCnt = 0;

		//=== 1、先发送AT测试命令，确认蓝牙串口模块可以接受AT指令 
		if(1 == p->flag.bit.status && ++TimeCnt > 10)
		{
				Uint8 Buf[USART_BUFFSIZE] = AT_TEST;
				USART2_TX_START(Buf,strlen((char *)Buf));
				TimeCnt = 0;
		}
		//=== 2、测试成功，发送AT命令
		else if(2 == p->flag.bit.status && ++TimeCnt > 10)
		{
				sprintf(BT05.AT_Buf, "AT+BAUD%d\r\n",BT05.flag.bit.BaudSet);
				USART2_TX_START((Uint8 *)p->AT_Buf,strlen(p->AT_Buf));
				TimeCnt = 0;
		}
		//=== 3、软件重置，修改串口波特率
		else if(3 == p->flag.bit.status && ++TimeCnt > 10)
		{
//				Uint8 Buf[USART_BUFFSIZE] = AT_RESET;
//				USART2_TX_START(Buf,strlen((char *)Buf));
//		
				USART2_Config(Baud[BT05.flag.bit.BaudSet]);									//== 蓝牙串口波特率
				p->flag.bit.status = 0;
				TimeCnt = 0;
		}
		//=== 4、手动改蓝牙模块波特率
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
