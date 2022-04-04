#include "main.h"
#include "GPIO.h"
#include "IS31FL3236.h"
#include "I2C.h"
#include "ADC.h"
#include "DELAY.h"
#include "USART.h"
#include "SPI.h"
#include "TIM.h"
#include "SNAKE.h"
#include "string.h"
#include "stdio.h"
#include "ASCII.h"



#define RUpdateFrq 	500				//=== ����LEDˢ��Ƶ��
#define LUpdateFrq 	2000
#define BLE_BAUD		9600

TYPE_LED_LED LED = {0};

int main(void)
{
    SysTick_Init(72);		 										//== ϵͳʱ��72M
		GPIO_User_Init();
		SPI1_Init();														//== 595����
		Adc_Init();															//== ��Դ��ѹ����
		USART2_DMA_Init(BLE_BAUD);							//== �������ڲ�����
//		USART2_BuadSelfCheck();									//== ����ģ��ͨ�Ų������Զ�ƥ��
		
		POV_LED_GPIO_Init();
		
		TIM1_Init(LUpdateFrq); 									//== ƽ��LED��������								
		TIM2_Init();														//== ת�ٲ��� ��ʼ��λ
//		TIM3_Init(1); 												//== ��Դ������			
		TIM4_Init(RUpdateFrq); 									//== 500Hz 2ms ĿǰI2C����144��LED��Ҫ1674us I2C�ٶ�1MHz		

//		for(Uint8 i = 0; i < 144; i ++)					//== �����ʾ
//		{
//				IS31_LED.DutyRatio1[i] = 0;
//		}
//		POV_LED_Update(&IS31_LED);
		for(Uint8 i = 0; i < RMAXW; i ++)				//== �߽���ʾ
		{
				LED.RBuf[RMAXL][i].R = 0x00;
				LED.RBuf[RMAXL][i].G = 0x00;
				LED.RBuf[RMAXL][i].B = 0x10;
		}
		LED.LBuf[LMAXL] = 0xFFFF;
		
		LED.RFrqDiv = 32;												//== ����LED���·�Ƶ��
		LED.RPointMax = RMAXL+1;								//== ����LED�������
		LED.RCntMax = RMAXW;										//== ����LED�������(��RGB����)
		LED.LFrqDiv = 0;
		LED.LPointMax = LMAXL;
		LED.LCntMax = LMAXW;
		
		LED.Flag.bit.LMode = 0;									//== ƽ��LED��ʾģʽ����ʾ��������
		LED.Flag.bit.RMode = 0;									//== ����LED��ʾģʽ������
		
		LED.Flag.bit.RUpdateCtrl = 0;
		
		LED.Color.R = 0x00;											//== ̰������ͷ��ɫ
		LED.Color.G = 0xFF;
		LED.Color.B = 0x03;
		LED.Flag.bit.SnakeRunDir	 = STOP;			//== ��ʼ�˶�״̬����ͣ
		LED.Flag.bit.SnakeRunDirOld = STOP;			//== ��ʼ�˶�״̬����ͣ
		SnakeInit(&LED.Color);									//== ̰���߳�ʼ��
		
    while(1)
    {
				//=== 0����ʵʱ��ȡ��ѹ������0-VDD 1-+5V
				Get_AdcValue(&LED.AdcRaw[0]);
				
				//=== 1����λ��Ϸ
				if(LED.Flag.bit.SnakeReStart)
				{
						LED.Flag.bit.SnakeRunDir	 = UP;				//== ��ʼ�˶�״̬����ͣ
						LED.Flag.bit.SnakeRunDirOld = UP;				//== ��ʼ�˶�״̬����ͣ
						LED.Flag.bit.SnakeReStart = 0;
						SnakeReset();
						SnakeInit(&LED.Color);
				}
				
				//=== 2��̰�����˶�
				if(LED.Flag.bit.SnakeRunDir <= 3) SnakeRun(LED.Flag.bit.SnakeRunDir);		
				
				//=== 3�����ݷ��� APP����ָ��
				if(BT05.flag.bit.status)
				{
						//=== �����ʵ���
						BT05_Config(&BT05);
				}
				else if(0 == LED.Flag.bit.DataTxSts)	
				{
						LED.Flag.bit.DataTxSts = 1;
//						sprintf((char *)Index, "Adc0=%4d,Adc1=%4d", LED.AdcRaw[0],LED.AdcRaw[1]);
//						USART2_TX_START(Index, 20);
						sprintf((char *)Index, "Adc0=%4d,Adc1=%4d,Speed=%6d", LED.AdcRaw[0],LED.AdcRaw[1],LED.SpeedPrd);
						USART2_TX_START(Index, 40);
				}
				
				//=== 4����ʵʱ��ʱ
				delayms(50);
    }
}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM1 �жϷ�����								    																						 */
/* ���룺��                          																					 						 			 */
/* �������			                                                                                 */
/* �������ṩϵͳ���Ȼ�׼																																				 */
/*			 �������� 5ms*/
/*************************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
		Uint16 DisData = 0;
		Uint8  DeltCnt = 2;

		if(TIM_GetITStatus(TIM1, TIM_IT_Update)) 
		{
				int16 T0 = TIM2->CNT;
				
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
					
				//=== 2��Ϊ����LED�ṩ������ʾ���Ȼ�׼(����I2Cͨ�����ʣ���ת�����޷���ʱ��������LED����������Ч���ӳ���ÿ�����ص㱻����)
				if(LED.Flag.bit.RUpdateCtrl)
				{
						LED.Flag.bit.RUpdate = !LED.Flag.bit.RUpdate;
						if(LED.Flag.bit.RUpdate)	GPIOB->BSRR = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
						else											GPIOB->BRR  = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
				}
				else
				{
						GPIOB->BSRR = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
				}
					
				//=== 1����Ƶ����
				if(++LED.LFrqDivCnt >= LED.LFrqDiv) LED.LFrqDivCnt = 0;				
				else 																return;	
					
				//=== 3��ˢ��ƽ��LED
				//=== ĿǰһȦ���ٸ��ַ���ȫ��ת�ٺ���ʱ����
				switch(LED.Flag.bit.LMode)
				{
								
						case 0:	//== ������ʾ 8*12 ��ʱ��
										DisData = (ASCII_Code[Index[LED.LPoint]-' '][LED.LCnt] << 8) | ASCII_Code[Index[LED.LPoint]-' '][LED.LCnt+1];
										break;
						case 1:	//== ������ʾ 8*12 ��ʱ��
										DisData = (Reverse16((ASCII_Code[Index[LED.LPoint]-' '][14-LED.LCnt] << 8) | ASCII_Code[Index[LED.LPoint]-' '][15-LED.LCnt]))>>6;
										DisData |= (BT05.flag.bit.BaudSet << 13);
										break;
						case 2: //== ������ʾ 8*12 ˳ʱ��
										DisData = (ASCII_Code[Index[19 - LED.LPoint]-' '][LED.LCnt] << 8) | ASCII_Code[Index[19 - LED.LPoint]-' '][LED.LCnt+1];
										break;
						case 3: //== ������ʾ 8*12 ˳ʱ��
										DisData = (Reverse16((ASCII_Code[Index[19 - LED.LPoint]-' '][14-LED.LCnt] << 8) | ASCII_Code[Index[19 - LED.LPoint]-' '][15-LED.LCnt]))>>6;
										break;
						case 4:	//== ������ʾ ���Ҿ���ͷ¿�
										DisData = (WJSTLM[LED.LPoint][LED.LCnt] << 8) | WJSTLM[LED.LPoint][LED.LCnt+1];
										break;
						case 5:
										DisData = Reverse16((WJSTLM[LED.LPoint][14-LED.LCnt] << 8) | WJSTLM[LED.LPoint][15-LED.LCnt])>>3;
										break;
						case 6:
										DisData = (WJSTLM[19 - LED.LPoint][LED.LCnt] << 8) | WJSTLM[19 - LED.LPoint][LED.LCnt+1];
										break;										
						case 7:
										DisData = Reverse16((WJSTLM[19 - LED.LPoint][14-LED.LCnt] << 8) | WJSTLM[19 - LED.LPoint][15-LED.LCnt])>>3;
										break;
						case 8: //== ̰����ģʽ
										DisData = Reverse16(LED.LBuf[LED.LPoint]);
										DeltCnt = 0;
										break;	
						default:
										LED.Flag.bit.LMode = 0;
										break;
				}

				SPI1_WriteByte(DisData);

				if(DeltCnt) LED.LCnt += DeltCnt;
				else				LED.LCnt = LED.LCntMax;
				if(LED.LCnt >= LED.LCntMax) 
				{					
						LED.LCnt = 0;

						if(++LED.LPoint >= LED.LPointMax)
						{
								LED.LPoint = 0;
						}
				}
				
				LED.RunTime = ((int16)TIM2->CNT - (int16)T0);
		}
		
}

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
		static Uint32 OvCnt = 0;

		//=== 1������ж�
		if(TIM_GetITStatus(TIM2, TIM_IT_Update)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				OvCnt++;	
		}
		
		//=== 2�������ж�(�½���)
		if(TIM_GetITStatus(TIM2, TIM_IT_CC1)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
				
				Uint16 CCR1 = TIM_GetCapture1(TIM2);
				
				//=== (1)��¼תһȦ��ʱ�䣬��λ��us
				LED.SpeedPrd = (OvCnt << 16) + CCR1;
				
				
				//=== (2)��ʼ����Ļ�߽�
				LED.LCnt  = 0;
				LED.LPoint = 0;
				LED.RPoint = 0;
				LED.Flag.bit.RSigStop = 0;
				LED.Flag.bit.LSigStop = 0;
				
				OvCnt = 0;
				TIM2->CNT = 0;
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
				
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		
				//=== 1����Ƶ����
				if(++LED.RFrqDivCnt >= LED.RFrqDiv) 	LED.RFrqDivCnt = 0;				
				else 																	return;
				
				
				//=== 2��ˢ������LED����
				#ifdef POV_LED_EN
	
//				if(LED.Flag.bit.RSigStop == 0)
				if(++LED.RPoint >= LED.RPointMax)// && LED.Flag.bit.RSigStop == 0) 
				{
						LED.RPoint = 0;	
						LED.Flag.bit.RSigStop = 1;
				}
		
				//=== 3��ģʽѡ�� 0-̰���� 1-����
				switch(LED.Flag.bit.RMode)
				{
						case 0:	//== ������ģʽ
										POV_LED_Show();
										POV_LED_Update(&IS31_LED);
										break;
						case 1:	//== ����ģʽ(���ɲ���)
										POV_LED_Update(&IS31_LED);
										break;
						case 2:	//== ̰����ģʽ
										POV_LED_Update1((Uint8 *)&LED.RBuf[LED.RPoint][0]);
										break;
						default:
										LED.Flag.bit.RMode = 0;
										break;
				}
							
				
				#endif
				LED.RunTime1 = ((int16)TIM2->CNT - (int16)T0);
		}
		
}




