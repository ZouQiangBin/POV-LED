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


#define RUpdateFrq 	2000				//== 球形LED刷新频率 Hz SK9822刷新一次只要42us
#define BLE_BAUD		115200

TYPE_LED_LED LED = {0};

int main(void)
{
		Uint16 TimeBase = 0, TimeCnt = 0;

		GPIO_User_Init();
		
#if (DEVICE_SEL == 1)
		MBI5353_Init();													//== MBI5353配置
#else
		SK9822_Init();													//== SK9822配置
#endif
		
		SPI2_Init();														//== 595配置
		Adc_Init();															//== 电源电压采样
		USART2_DMA_Init(BLE_BAUD);							//== 蓝牙串口波特率
		
		TIM1_Init(70); 													//== GCLK输出 PWM PA8 1MHz						
		TIM2_Init();														//== 转速测量 初始定位 1us
		TIM3_Init(5000); 												//== 无源蜂鸣器	初始频率5000Hz
		TIM4_Init(RUpdateFrq); 									//== 定时中断刷新LED

		for(Uint8 i = 0; i < RMAXY; i ++)				//== 边界显示
		{
				LED.RBuf[RMAXX][i].R = 0x00;
				LED.RBuf[RMAXX][i].G = 0x00;
				LED.RBuf[RMAXX][i].B = 0x10;
		}
		LED.LBuf[LMAXX] = 0xFFFF;
		
		LED.RFrqDiv = 0;												//== 球形LED更新分频数:0
		LED.RAxisMaxX = RMAXX+1;								//== 球形LED X坐标(列数) RGB为单位
		LED.RAxisMaxY = RMAXY;									//== 球形LED Y坐标(行数)
		LED.LFrqDiv = 0;												//== 平面LED更新分频数:0
		LED.LAxisMaxX = LMAXX;									//== 平面LED X坐标(列数) LED为单位
		LED.LAxisMaxY = LMAXY;									//== 平面LED Y坐标(行数)

		Menu.KeySts = ELSE;
		Menu.PicShiftBits = 3;									//== 图片亮度衰减移位数
		Menu.Rgb.R = 100;
		Menu.Rgb.G = 100;
		Menu.Rgb.B = 100;

    while(1)
    {
				if((int16)(TIM2->CNT - TimeBase) >= 8000)
				{
						TimeBase += 8000;
						TimeCnt++;
																					
									//== 8ms 菜单及功能
						
						if((TimeCnt & 0x1) == 0x0)		//== 16ms
						{
								(*MenuRun[Menu.Layer])();
						}
						
						if((TimeCnt & 0x7) == 0x7)		//== 64ms
						{
								//=== 0、音乐播放
								MusicRun(&Music);
						}
						
						if((TimeCnt & 0xFF) == 0xFF)	  //== 128*8ms
						{
								Uint8 Index[60];
								
								//=== 0、非实时获取电压采样：0-VDD 1-+5V
								Get_AdcValue(&LED.AdcRaw[0]);
						
								//=== 1、发送本机状态
								if(0 == Menu.Flag.bit.DataTxSts)	
								{
										Menu.Flag.bit.DataTxSts = 1;
										sprintf((char *)Index, "VDD:%fV\r\nVCC:%fV\r\nSPEED:%frpm\r\n", LED.AdcRaw[0]*6.6/4096,LED.AdcRaw[1]*6.6/4096,60000000.0f / LED.SpeedPrd);
										USART2_TX_START(Index, 60);
								}
								
								//=== 2、AT功能
								AT_CMD(&Menu.ATSel);
						}						
				}					
    }
}

/*************************************************************************************************/
/* 功能：定时器TIM1 中断服务函数								    																						 */
/* 输入：无                          																					 						 			 */
/* 输出：无			                                                                                 */
/* 描述：提供系统调度基准																																				 */
/*			 调度周期 5ms*/
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
/* 功能：定时器TIM2 中断服务函数								    																						 */
/* 输入：无                          																					 						 			 */
/* 输出：无			                                                                                 */
/* 描述：T法测速 																																								 */
/*			 1、溢出中断对溢出次数计数             	 			 																			 */
/*			 2、捕获中断来临后计算频率,同时清除中断溢出计数																					 */
/*************************************************************************************************/
void TIM2_IRQHandler(void)
{
		static Uint8  ScrollCnt = 0, ScrollX = 0;
		static Uint32 OvCnt = 0;

		//=== 1、溢出中断
		if(TIM_GetITStatus(TIM2, TIM_IT_Update)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				OvCnt++;	
		}
		
		//=== 2、捕获中断(下降沿) 因硬件设计 转速到了1500rpm后就会抓不到Z信号
		if(TIM_GetITStatus(TIM2, TIM_IT_CC1)) 
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
				
				Uint16 CCR1 = TIM_GetCapture1(TIM2);

				//== 1、记录转一圈的时间，单位：us
				LED.SpeedPrd = (OvCnt << 16) + CCR1;
				OvCnt = 0;
				TIM_Cmd(TIM2, DISABLE);
				TIM2->CNT = 0;					//== T法测速要重新计时
				TIM_Cmd(TIM2, ENABLE);
				
				TIM_Cmd(TIM4, DISABLE);	//== 本处清零定时器4，抖动问题会改善很多，但还是有随机抖动现象
				TIM4->CNT = 0;					//== 转速不稳定+光电不合适，在现有的硬件和机械条件下暂时没有更好的办法优化
				TIM_Cmd(TIM4, ENABLE);
				
				//== 2、初始化屏幕边界、带滚动显示
				LED.LAxisCntY = 0;
				if(1 == Menu.Flag.bit.ScrollMode)	//== 顺向滚动
				{
						if(++ScrollCnt >= LED.ScrollDiv)
						{
								ScrollCnt = 0;
								if(++ScrollX >= LED.RAxisMaxX) ScrollX = 0;
						}
				}
				else if(2 == Menu.Flag.bit.ScrollMode)	//== 逆向滚动
				{
						if(++ScrollCnt >= LED.ScrollDiv)
						{
								ScrollCnt = 0;
								if(--ScrollX >= LED.RAxisMaxX) ScrollX = LED.RAxisMaxX;
						}
				}
				else	//== 手动滑动
				{
						ScrollX = LED.ScrollDiv;
						ScrollCnt = 0;
				}
				LED.RAxisCntX = ScrollX;
				LED.LAxisCntX = ScrollX;
				
				//== 3、加载界面
				Menu.Init = 1;
				if(Menu.Init && Menu.Layer == 0)	DispInit();	

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
				
				Menu.Flag.bit.UpdataSts = 1;
				
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
				//=== 1、分频处理
				if(++LED.RFrqDivCnt >= LED.RFrqDiv) 	LED.RFrqDivCnt = 0;				
				else 																	return;
				if(Menu.Flag.bit.UpdataSts == 0)			return;
				
				//=== 2、显示更新
				DispUpdate();
				
				//=== 3、时间测量
				LED.RunTime = ((int16)TIM2->CNT - (int16)T0);
				
				Menu.Flag.bit.UpdataSts = 0;
		}
		
}




