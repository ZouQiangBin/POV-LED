#include "main.h"
#include "GPIO.h"
#include "MBI5353.h"
#include "ADC.h"
#include "USART.h"
#include "SPI.h"
#include "TIM.h"
#include "SNAKE.h"
#include "ASCII.h"
#include "MENU.h"
#include "MUSIC.h"
#include "ATCMD.h"
#include "PAINT.h"
#include "SK9822.h"


#define RUpdateFrq 	2000				//== ����LEDˢ��Ƶ�� Hz SK9822ˢ��һ��ֻҪ42us
#define BLE_BAUD		115200

TYPE_LED_LED LED = {0};

int main(void)
{
		Uint16 TimeBase = 0, TimeCnt = 0;

		GPIO_User_Init();
		
#if (DEVICE_SEL == 1)
		MBI5353_Init();													//== MBI5353����
#else
		SK9822_Init();													//== SK9822����
#endif
		
		SPI2_Init();														//== 595����
		Adc_Init();															//== ��Դ��ѹ����
		USART2_DMA_Init(BLE_BAUD);							//== �������ڲ�����
		
		TIM1_Init(70); 													//== GCLK��� PWM PA8 1MHz						
		TIM2_Init();														//== ת�ٲ��� ��ʼ��λ 1us
		TIM3_Init(5000); 												//== ��Դ������	��ʼƵ��5000Hz
		TIM4_Init(RUpdateFrq); 									//== ��ʱ�ж�ˢ��LED

		for(Uint8 i = 0; i < RMAXY; i ++)				//== �߽���ʾ
		{
				LED.RBuf[RMAXX][i].R = 0x00;
				LED.RBuf[RMAXX][i].G = 0x00;
				LED.RBuf[RMAXX][i].B = 0x10;
		}
		LED.LBuf[LMAXX] = 0xFFFF;
		
		LED.RFrqDiv = 0;												//== ����LED���·�Ƶ��:0
		LED.RAxisMaxX = RMAXX+1;								//== ����LED X����(����) RGBΪ��λ
		LED.RAxisMaxY = RMAXY;									//== ����LED Y����(����)
		LED.LFrqDiv = 0;												//== ƽ��LED���·�Ƶ��:0
		LED.LAxisMaxX = LMAXX;									//== ƽ��LED X����(����) LEDΪ��λ
		LED.LAxisMaxY = LMAXY;									//== ƽ��LED Y����(����)

		Menu.KeySts = ELSE;
		Menu.PicShiftBits = 3;									//== ͼƬ����˥����λ��
		Menu.Rgb.R = 100;
		Menu.Rgb.G = 100;
		Menu.Rgb.B = 100;

    while(1)
    {
				if((int16)(TIM2->CNT - TimeBase) >= 8000)
				{
						TimeBase += 8000;
						TimeCnt++;
																					
									//== 8ms �˵�������
						
						if((TimeCnt & 0x1) == 0x0)		//== 16ms
						{
								(*MenuRun[Menu.Layer])();
						}
						
						if((TimeCnt & 0x7) == 0x7)		//== 64ms
						{
								//=== 0�����ֲ���
								MusicRun(&Music);
						}
						
						if((TimeCnt & 0xFF) == 0xFF)	  //== 128*8ms
						{
								Uint8 Index[60];
								
								//=== 0����ʵʱ��ȡ��ѹ������0-VDD 1-+5V
								Get_AdcValue(&LED.AdcRaw[0]);
						
								//=== 1�����ͱ���״̬
								if(0 == Menu.Flag.bit.DataTxSts)	
								{
										Menu.Flag.bit.DataTxSts = 1;
										sprintf((char *)Index, "VDD:%fV\r\nVCC:%fV\r\nSPEED:%frpm\r\n", LED.AdcRaw[0]*6.6/4096,LED.AdcRaw[1]*6.6/4096,60000000.0f / LED.SpeedPrd);
										USART2_TX_START(Index, 60);
								}
								
								//=== 2��AT����
								AT_CMD(&Menu.ATSel);
						}						
				}					
    }
}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM1 �жϷ�����								    																						 */
/* ���룺��                          																					 						 			 */
/* �������			                                                                                 */
/* �������ṩϵͳ���Ȼ�׼																																				 */
/*			 �������� 5ms*/
/*************************************************************************************************/
//void TIM1_UP_IRQHandler(void)
//{
//		if(TIM_GetITStatus(TIM1, TIM_IT_Update)) 
//		{
//				int16 T0 = TIM2->CNT;

//				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//				
//				LED.RunTime = ((int16)TIM2->CNT - (int16)T0);
//		}
//		
//}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM2 �жϷ�����								    																						 */
/* ���룺��                          																					 						 			 */
/* �������			                                                                                 */
/* ������T������ 																																								 */
/*			 1������ж϶������������             	 			 																			 */
/*			 2�������ж����ٺ����Ƶ��,ͬʱ����ж��������																					 */
/*************************************************************************************************/
void TIM2_IRQHandler(void)
{
		static Uint8  ScrollCnt = 0, ScrollX = 0;
		static Uint32 OvCnt = 0;

		//=== 1������ж�
		if(TIM_GetITStatus(TIM2, TIM_IT_Update)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				OvCnt++;	
		}
		
		//=== 2�������ж�(�½���) ��Ӳ����� ת�ٵ���1500rpm��ͻ�ץ����Z�ź�
		if(TIM_GetITStatus(TIM2, TIM_IT_CC1)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
				
				Uint16 CCR1 = TIM_GetCapture1(TIM2);

				//== 1����¼תһȦ��ʱ�䣬��λ��us
				LED.SpeedPrd = (OvCnt << 16) + CCR1;
				OvCnt = 0;
				TIM_Cmd(TIM2, DISABLE);
				TIM2->CNT = 0;					//== T������Ҫ���¼�ʱ
				TIM_Cmd(TIM2, ENABLE);
				
				TIM_Cmd(TIM4, DISABLE);	//== �������㶨ʱ��4�������������ƺܶ࣬�������������������
				TIM4->CNT = 0;					//== ת�ٲ��ȶ�+��粻���ʣ������е�Ӳ���ͻ�е��������ʱû�и��õİ취�Ż�
				TIM_Cmd(TIM4, ENABLE);
				
				//== 2����ʼ����Ļ�߽硢��������ʾ
				LED.LAxisCntY = 0;
				if(1 == Menu.Flag.bit.ScrollMode)	//== ˳�����
				{
						if(++ScrollCnt >= LED.ScrollDiv)
						{
								ScrollCnt = 0;
								if(++ScrollX >= LED.RAxisMaxX) ScrollX = 0;
						}
				}
				else if(2 == Menu.Flag.bit.ScrollMode)	//== �������
				{
						if(++ScrollCnt >= LED.ScrollDiv)
						{
								ScrollCnt = 0;
								if(--ScrollX >= LED.RAxisMaxX) ScrollX = LED.RAxisMaxX;
						}
				}
				else	//== �ֶ�����
				{
						ScrollX = LED.ScrollDiv;
						ScrollCnt = 0;
				}
				LED.RAxisCntX = ScrollX;
				LED.LAxisCntX = ScrollX;
				
				//== 3�����ؽ���
				Menu.Init = 1;
				if(Menu.Init && Menu.Layer == 0)	DispInit();	

		}
}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM4 �жϷ�����								    																						 */
/* ���룺��                          																					 						 			 */
/* �������			                                                                                 */
/* ������ˢ������LED																																						 */
/*************************************************************************************************/
void TIM4_IRQHandler(void)
{
		//=== ����ж�
		if(TIM_GetITStatus(TIM4, TIM_IT_Update)) 
		{
				int16 T0 = TIM2->CNT;
				
				Menu.Flag.bit.UpdataSts = 1;
				
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
				//=== 1����Ƶ����
				if(++LED.RFrqDivCnt >= LED.RFrqDiv) 	LED.RFrqDivCnt = 0;				
				else 																	return;
				if(Menu.Flag.bit.UpdataSts == 0)			return;
				
				//=== 2����ʾ����
				DispUpdate();
				
				//=== 3��ʱ�����
				LED.RunTime = ((int16)TIM2->CNT - (int16)T0);
				
				Menu.Flag.bit.UpdataSts = 0;
		}
		
}




