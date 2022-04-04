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



#define RUpdateFrq 	500				//=== 球形LED刷新频率
#define LUpdateFrq 	2000
#define BLE_BAUD		9600

TYPE_LED_LED LED = {0};

int main(void)
{
    SysTick_Init(72);		 										//== 系统时钟72M
		GPIO_User_Init();
		SPI1_Init();														//== 595配置
		Adc_Init();															//== 电源电压采样
		USART2_DMA_Init(BLE_BAUD);							//== 蓝牙串口波特率
//		USART2_BuadSelfCheck();									//== 蓝牙模块通信波特率自动匹配
		
		POV_LED_GPIO_Init();
		
		TIM1_Init(LUpdateFrq); 									//== 平面LED更新速率								
		TIM2_Init();														//== 转速测量 初始定位
//		TIM3_Init(1); 												//== 无源蜂鸣器			
		TIM4_Init(RUpdateFrq); 									//== 500Hz 2ms 目前I2C更新144个LED需要1674us I2C速度1MHz		

//		for(Uint8 i = 0; i < 144; i ++)					//== 清除显示
//		{
//				IS31_LED.DutyRatio1[i] = 0;
//		}
//		POV_LED_Update(&IS31_LED);
		for(Uint8 i = 0; i < RMAXW; i ++)				//== 边界显示
		{
				LED.RBuf[RMAXL][i].R = 0x00;
				LED.RBuf[RMAXL][i].G = 0x00;
				LED.RBuf[RMAXL][i].B = 0x10;
		}
		LED.LBuf[LMAXL] = 0xFFFF;
		
		LED.RFrqDiv = 32;												//== 球形LED更新分频数
		LED.RPointMax = RMAXL+1;								//== 球形LED最多列数
		LED.RCntMax = RMAXW;										//== 球形LED最多行数(即RGB个数)
		LED.LFrqDiv = 0;
		LED.LPointMax = LMAXL;
		LED.LCntMax = LMAXW;
		
		LED.Flag.bit.LMode = 0;									//== 平面LED显示模式：显示采样数据
		LED.Flag.bit.RMode = 0;									//== 球形LED显示模式：加载
		
		LED.Flag.bit.RUpdateCtrl = 0;
		
		LED.Color.R = 0x00;											//== 贪吃蛇蛇头颜色
		LED.Color.G = 0xFF;
		LED.Color.B = 0x03;
		LED.Flag.bit.SnakeRunDir	 = STOP;			//== 初始运动状态：暂停
		LED.Flag.bit.SnakeRunDirOld = STOP;			//== 初始运动状态：暂停
		SnakeInit(&LED.Color);									//== 贪吃蛇初始化
		
    while(1)
    {
				//=== 0、非实时获取电压采样：0-VDD 1-+5V
				Get_AdcValue(&LED.AdcRaw[0]);
				
				//=== 1、复位游戏
				if(LED.Flag.bit.SnakeReStart)
				{
						LED.Flag.bit.SnakeRunDir	 = UP;				//== 初始运动状态：暂停
						LED.Flag.bit.SnakeRunDirOld = UP;				//== 初始运动状态：暂停
						LED.Flag.bit.SnakeReStart = 0;
						SnakeReset();
						SnakeInit(&LED.Color);
				}
				
				//=== 2、贪吃蛇运动
				if(LED.Flag.bit.SnakeRunDir <= 3) SnakeRun(LED.Flag.bit.SnakeRunDir);		
				
				//=== 3、数据发送 APP接收指令
				if(BT05.flag.bit.status)
				{
						//=== 波特率调整
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
				
				//=== 4、非实时延时
				delayms(50);
    }
}

/*************************************************************************************************/
/* 功能：定时器TIM1 中断服务函数								    																						 */
/* 输入：无                          																					 						 			 */
/* 输出：无			                                                                                 */
/* 描述：提供系统调度基准																																				 */
/*			 调度周期 5ms*/
/*************************************************************************************************/
void TIM1_UP_IRQHandler(void)
{
		Uint16 DisData = 0;
		Uint8  DeltCnt = 2;

		if(TIM_GetITStatus(TIM1, TIM_IT_Update)) 
		{
				int16 T0 = TIM2->CNT;
				
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
					
				//=== 2、为球形LED提供球形显示长度基准(由于I2C通信速率，高转速下无法及时更新所有LED，导致余晖效果加长，每个像素点被拉长)
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
					
				//=== 1、分频处理
				if(++LED.LFrqDivCnt >= LED.LFrqDiv) LED.LFrqDivCnt = 0;				
				else 																return;	
					
				//=== 3、刷新平面LED
				//=== 目前一圈多少个字符完全由转速和延时控制
				switch(LED.Flag.bit.LMode)
				{
								
						case 0:	//== 向内显示 8*12 逆时针
										DisData = (ASCII_Code[Index[LED.LPoint]-' '][LED.LCnt] << 8) | ASCII_Code[Index[LED.LPoint]-' '][LED.LCnt+1];
										break;
						case 1:	//== 向外显示 8*12 逆时针
										DisData = (Reverse16((ASCII_Code[Index[LED.LPoint]-' '][14-LED.LCnt] << 8) | ASCII_Code[Index[LED.LPoint]-' '][15-LED.LCnt]))>>6;
										DisData |= (BT05.flag.bit.BaudSet << 13);
										break;
						case 2: //== 向内显示 8*12 顺时针
										DisData = (ASCII_Code[Index[19 - LED.LPoint]-' '][LED.LCnt] << 8) | ASCII_Code[Index[19 - LED.LPoint]-' '][LED.LCnt+1];
										break;
						case 3: //== 向外显示 8*12 顺时针
										DisData = (Reverse16((ASCII_Code[Index[19 - LED.LPoint]-' '][14-LED.LCnt] << 8) | ASCII_Code[Index[19 - LED.LPoint]-' '][15-LED.LCnt]))>>6;
										break;
						case 4:	//== 向内显示 “我就是头驴嘛”
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
						case 8: //== 贪吃蛇模式
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
/* 功能：定时器TIM2 中断服务函数								    																						 */
/* 输入：无                          																					 						 			 */
/* 输出：无			                                                                                 */
/* 描述：T法测速 																																								 */
/*			 1、溢出中断对溢出次数计数             	 			 																			 */
/*			 2、捕获中断来临后计算频率,同时清除中断溢出计数																					 */
/*************************************************************************************************/
void TIM2_IRQHandler(void)
{
		static Uint32 OvCnt = 0;

		//=== 1、溢出中断
		if(TIM_GetITStatus(TIM2, TIM_IT_Update)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				OvCnt++;	
		}
		
		//=== 2、捕获中断(下降沿)
		if(TIM_GetITStatus(TIM2, TIM_IT_CC1)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
				
				Uint16 CCR1 = TIM_GetCapture1(TIM2);
				
				//=== (1)记录转一圈的时间，单位：us
				LED.SpeedPrd = (OvCnt << 16) + CCR1;
				
				
				//=== (2)初始化屏幕边界
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
/* 功能：定时器TIM4 中断服务函数								    																						 */
/* 输入：无                          																					 						 			 */
/* 输出：无			                                                                                 */
/* 描述：刷新球形LED																																						 */
/*************************************************************************************************/
void TIM4_IRQHandler(void)
{
		//=== 溢出中断
		if(TIM_GetITStatus(TIM4, TIM_IT_Update)) 
		{
				int16 T0 = TIM2->CNT;
				
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		
				//=== 1、分频处理
				if(++LED.RFrqDivCnt >= LED.RFrqDiv) 	LED.RFrqDivCnt = 0;				
				else 																	return;
				
				
				//=== 2、刷新球形LED数据
				#ifdef POV_LED_EN
	
//				if(LED.Flag.bit.RSigStop == 0)
				if(++LED.RPoint >= LED.RPointMax)// && LED.Flag.bit.RSigStop == 0) 
				{
						LED.RPoint = 0;	
						LED.Flag.bit.RSigStop = 1;
				}
		
				//=== 3、模式选择： 0-贪吃蛇 1-正常
				switch(LED.Flag.bit.RMode)
				{
						case 0:	//== 呼吸灯模式
										POV_LED_Show();
										POV_LED_Update(&IS31_LED);
										break;
						case 1:	//== 正常模式(自由操作)
										POV_LED_Update(&IS31_LED);
										break;
						case 2:	//== 贪吃蛇模式
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




