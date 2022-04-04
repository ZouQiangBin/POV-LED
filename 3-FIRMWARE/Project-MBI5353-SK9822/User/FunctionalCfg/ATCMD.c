/*************************************************************************************************/
/*�ļ���AT����C�ļ�			                                                               */
/*��¼��By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#include "ATCMD.h"
#include "MENU.h"

#define WATI_TIME	360000

/*************************************************************************************************/
/* ���ܣ�AT����ʵ��								    																						 */
/* ���룺*Sel---AT����ѡ��																			 						 			 */
/* �������			                                                                                 */
/* ������ѡ�����õ�AT����(����ʹ��)																																			 */
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
				if(++TimeCnt >= WATI_TIME && 0 == Menu.Flag.bit.DataTxSts)
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
										Menu.Flag.bit.BleErr = 1;
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
