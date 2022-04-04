/*************************************************************************************************/
/*文件：AT命令C文件			                                                               */
/*记录：By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#include "ATCMD.h"
#include "MENU.h"

#define WATI_TIME	360000

/*************************************************************************************************/
/* 功能：AT命令实现								    																						 */
/* 输入：*Sel---AT功能选择																			 						 			 */
/* 输出：无			                                                                                 */
/* 描述：选择内置的AT功能(测试使用)																																			 */
/*************************************************************************************************/
void AT_CMD(Uint8 *Sel)
{
		if(*Sel == 1)
		{
				Uint8 Buf[USART_BUFFSIZE] = "AT+NAME?\r\n";
				*Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 2)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+NAME=CYY\r\n";
				*Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 3)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+PASSWORD?\r\n";
				*Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 4)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+PASSWORD=CYY\r\n";
				*Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 5)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+PASSWORDC\r\n";
				Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 6)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+ROLE?\r\n";
				*Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 7)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+ROLEMODE?\r\n";
				Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
		else if(*Sel == 8)
		{
			
				Uint8 Buf[USART_BUFFSIZE] = "AT+ROLE=2\r\n";
				*Sel = 0;
				USART2_TX_START(Buf,strlen((char *)Buf));
		}
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
				if(++TimeCnt >= WATI_TIME && 0 == Menu.Flag.bit.DataTxSts)
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
										Menu.Flag.bit.BleErr = 1;
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
