#include "MBI5353.h"
#include "SPI.h"

#define SPI_DMA_ENABLE

TYPE_MBI5353 		MBI5353 = MBI5353_DEFS;

/*************************************************************************************************/
/* 功能：初始化配置																																	 	             */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：1、配置引脚                                */
/*       2、配置MBI5353全局设置                                                       */
/*       3、帧初始化(先不加)	                                                   */
/*       4、指针初始化(重排列)	                                                   */
/*************************************************************************************************/
void MBI5353_Init(void)
{
		//=== 1、初始化引脚和SPI外设
		SPI1_Init();
		
		//=== 2、初始化
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;
		SPI_SDO_Out_PP;
		
		MBI5353.Global.R1.bit.GCLK_Mul_En = 1;
		MBI5353.Global.R1.bit.Line_Num = 0;//0x1F;	//== 扫描行1行
		MBI5353.Global.R1.bit.SPWM_Mode = 3;
		MBI5353.Global.R1.bit.Gray_Mode = 0;

		MBI5353.Global.R2.bit.Con_Gain = 1;		//== 全局电流增益47.6->15.8
//		MBI5353.Global.R2.bit.Adj_Gain = 0;
//		MBI5353.Global.R2.bit.Power_En = 1;	//== 所有数据为0时黑屏

//		MBI5353.Global.R3.bit.BlackPreChargeEn = 1;	//== 初始灰度为0

		MBI5353_Write_Config();
		
//		MBI5353.RGB.bit.CurGain = 32;	//== R/G/B电流控制
//		MBI5353_Write_RGB_Config();

		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
		SPI_SDO_AF_PP;
		
		//=== 3、
//		MBI5353_Frame_Init();

		//=== 4、指针初始化
		for(Uint8 i = 0; i < DEVICES; i++)
		{
				for(Uint8 j = 0; j < 16; j++)
				{
//						MBI5353.pLED[i][3*j+0] = &MBI5353.LED[j].Pixel[i].R;
//						MBI5353.pLED[i][3*j+1] = &MBI5353.LED[j].Pixel[i].G;
//						MBI5353.pLED[i][3*j+2] = &MBI5353.LED[j].Pixel[i].B;
//						MBI5353.pLED[i][3*j+0] = (Uint8 *)&MBI5353.LED[j].Pixel[i].R+1;
//						MBI5353.pLED[i][3*j+1] = (Uint8 *)&MBI5353.LED[j].Pixel[i].G+1;
//						MBI5353.pLED[i][3*j+2] = (Uint8 *)&MBI5353.LED[j].Pixel[i].B+1;
						
						MBI5353.pLED[i][3*j+0] = (Uint8 *)&MBI5353.LED[j].word[DEVICES-1-i][0].ByteH;
						MBI5353.pLED[i][3*j+1] = (Uint8 *)&MBI5353.LED[j].word[DEVICES-1-i][1].ByteH;
						MBI5353.pLED[i][3*j+2] = (Uint8 *)&MBI5353.LED[j].word[DEVICES-1-i][2].ByteH;
				}
		}
}

/*************************************************************************************************/
/* 功能：帧(SRAM)初始化																														 	             */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：上电时预显示内容                                */
/*************************************************************************************************/
void MBI5353_Frame_Init(void)
{
		//=== 0、初始状态
		SPI_LE_L;
		RCC_MCOConfig(RCC_MCO_NoClock); //== 关闭灰度时钟
		
		//=== 1、初始显示数据先置入(必须根据需求确定1：32扫描数)
		MBI5353_Update();
		
		//=== 2、发送2个clk边沿 帧同步命令 显示数据
		SPI_CLK_Out_PP;
		SPI_LE_H;
		for(Uint8 i = 0; i < 3; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		SPI_CLK_AF_PP;
		
		//=== 3、延时1.2us至少
		for(Uint8 i = 0; i < 144; i++);
		
		RCC_MCOConfig(RCC_MCO_HSE); //== 8M
}

/*************************************************************************************************/
/* 功能：写入全局配置寄存器																												 	             */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：根据级联的MBI5353个数写入全局配置，每个器件有3个16bit全局寄存器(R1/2/3)                 */
/* 			 GPIO模拟控制，写入结束后将引脚改为SPI功能                 */
/*************************************************************************************************/
void MBI5353_Write_Config(void)
{
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;

		//=== 0、初始状态
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1、发送14个clk边沿 确认命令
		SPI_LE_H;
		for(Uint8 i = 0; i < CMD_CONFIRM; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2、发送48bit配置数据 MSB先行
		for(Uint8 i = 0; i < DEVICES;  i++)
		{
				Uint16 DataR1 = MBI5353.Global.R1.all;
				Uint16 DataR2 = MBI5353.Global.R2.all;
				Uint16 DataR3 = MBI5353.Global.R3.all;
				
				for(Uint8 j = 0; j < 16; j++)
				{
						if(DataR1 & 0x8000)	{SPI_SDI_H;}
						else								{SPI_SDI_L;}
						DataR1 = DataR1 << 1;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				for(Uint8 j = 0; j < 16; j++)
				{
						if(DataR2 & 0x8000)	{SPI_SDI_H;}
						else								{SPI_SDI_L;}
						DataR2 = DataR2 << 1;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				for(Uint8 j = 0; j < 16; j++)
				{
						//=== 最后4个clk边沿启动配置写入
						if(i == (DEVICES-1) && j == 12) SPI_LE_H;
						
						if(DataR3 & 0x8000)	{SPI_SDI_H;}
						else								{SPI_SDI_L;}
						DataR3 = DataR3 << 1;
						SPI_SCK_H;
						SPI_SCK_L;
				}
		}
		
		//=== 3、配置写入结束
		SPI_LE_L;
		
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
}

/*************************************************************************************************/
/* 功能：写入全局颜色组独立配置寄存器																												 	   */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：根据级联的MBI5353个数写入全局配置，每个器件有3个16bit全局颜色独立配置寄存器(R/G/B)      */
/* 			 GPIO模拟控制，写入结束后将引脚改为SPI功能                 */
/*************************************************************************************************/
void MBI5353_Write_RGB_Config(void)
{
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;

		for(Uint8 i = 1; i < 4; i++)
		{
				//=== 0、初始状态
				SPI_LE_L;
				SPI_SCK_L;
				
				//=== 1、发送14个clk边沿 确认命令
				SPI_LE_H;
				for(Uint8 i0 = 0; i0 < CMD_CONFIRM; i0++)
				{
						SPI_SCK_H;
						SPI_SCK_L;
				}
				SPI_LE_L;
				
				//=== 2、开始的2bit为颜色组选择，发送48bit配置数据 MSB先行
				MBI5353.RGB.bit.Sel = i;
				for(Uint8 i1 = 0; i1 < DEVICES;  i1++)
				{
						Uint16 DataR1 = MBI5353.RGB.all;	//== R/G/B配置寄存器功能一致
						Uint16 DataR2 = 0;								//== 后面两个数据手册无定义
						Uint16 DataR3 = 0;
						
						for(Uint8 j = 0; j < 16; j++)
						{
								if(DataR1 & 0x8000)	{SPI_SDI_H;}
								else								{SPI_SDI_L;}
								DataR1 = DataR1 << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
						for(Uint8 j = 0; j < 16; j++)
						{
								if(DataR2 & 0x8000)	{SPI_SDI_H;}
								else								{SPI_SDI_L;}
								DataR2 = DataR2 << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
						for(Uint8 j = 0; j < 16; j++)
						{
								//=== 最后4个clk边沿启动配置写入
								if(i1 == DEVICES && j == 12) SPI_LE_H;
								
								if(DataR3 & 0x8000)	{SPI_SDI_H;}
								else								{SPI_SDI_L;}
								DataR3 = DataR3 << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
				}
				
				//=== 3、配置写入结束
				SPI_LE_L;
		}
		
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
}

/*************************************************************************************************/
/* 功能：读出全局配置寄存器																												 	             */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：根据级联的MBI5353个数读出全局配置，每个器件有3个16bit全局寄存器(R1/2/3)                 */
/* 			 GPIO模拟控制，读取结束后将引脚改为SPI功能                 */
/*************************************************************************************************/
void MBI5353_Read_Config(void)
{
		SPI_CLK_Out_PP;
		SPI_SDO_Out_PP;
		
		//=== 0、初始状态
		SPI_LE_L;
		SPI_SCK_L;
		
		SPI_LE_H;
		//=== 1、发送5个clk边沿
		for(Uint8 i = 0; i < CMD_READ_GLOBAL_REG; i++)	//== 发送确认命令
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2、读取3个器件的SDO状态
		for(Uint8 i = 0; i < DEVICES;  i++)
		{
				Uint16 DataR1 = 0;
				Uint16 DataR2 = 0;
				Uint16 DataR3 = 0;
				
				for(Uint8 j = 0; j < 16; j++)
				{
						DataR1 = DataR1 << 1;
						if(SPI_SDO_READ) DataR1 |= 0x01;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				for(Uint8 j = 0; j < 16; j++)
				{
						DataR2 = DataR2 << 1;
						if(SPI_SDO_READ) DataR2 |= 0x01;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				for(Uint8 j = 0; j < 16; j++)
				{
						DataR3 = DataR3 << 1;
						if(SPI_SDO_READ) DataR3 |= 0x01;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				
//				//=== 串级会有48BIT空挡
//				for(Uint8 j = 0; j < 48; j++)
//				{
//						SPI_SCK_H;
//						SPI_SCK_L;
//				}
				
				MBI5353.Global.R1.all = DataR1;
				MBI5353.Global.R2.all = DataR2;
				MBI5353.Global.R3.all = DataR3;
		}	
		
		SPI_CLK_AF_PP;
		SPI_SDO_AF_PP;
}

/*************************************************************************************************/
/* 功能：读出通道状态(开路0、短路1)																												 	     */
/* 输入：Type:0-开路通道读取 1-短路通道读取                                                      */
/* 输出：无                                                                                      */
/* 描述：根据级联的MBI5353个数读出所有通道的错误状态                 */
/* 			 GPIO模拟控制，读取结束后将引脚改为SPI功能                 */
/*************************************************************************************************/
void MBI5353_Read_Err(Uint8 Type)
{
		Uint8 Cmd = Type ? CMD_SHOT_ERR_ENABLE : CMD_OPEN_ERR_ENABLE;
		
		SPI_CLK_Out_PP;
		SPI_SDO_Out_PP;
		
		//=== 0、初始状态
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1、发送5个clk边沿
		SPI_LE_H;
		for(Uint8 i = 0; i < Cmd; i++)	//== 发送确认命令
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2、延时至少700ns
		for(Uint8 i = 0; i < 14; i++);
		
		//=== 3、停止错误命令，准备获取数据
		for(Uint8 i = 0; i < CMD_STOP_ERR_CHECK_REG; i++)	//== 发送确认命令
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		
		//=== 4、读取标志
		for(Uint8 i = 0; i < DEVICES;  i++)
		{
				Uint16 DataR1 = 0;
				Uint16 DataR2 = 0;
				Uint16 DataR3 = 0;
				
				for(Uint8 j = 0; j < 16; j++)
				{
						DataR1 = DataR1 << 1;
						if(SPI_SDO_READ) DataR1 |= 0x01;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				for(Uint8 j = 0; j < 16; j++)
				{
						DataR2 = DataR2 << 1;
						if(SPI_SDO_READ) DataR2 |= 0x01;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				for(Uint8 j = 0; j < 16; j++)
				{
						DataR3 = DataR3 << 1;
						if(SPI_SDO_READ) DataR3 |= 0x01;
						SPI_SCK_H;
						SPI_SCK_L;
				}
				
				//=== 串级会有48BIT空挡
//				for(Uint8 j = 0; j < 48; j++)
//				{
//						SPI_SCK_H;
//						SPI_SCK_L;
//				}
				
				MBI5353.Flag[i].all[0] = DataR1;
				MBI5353.Flag[i].all[1] = DataR2;
				MBI5353.Flag[i].all[2] = DataR3;
		}
		
		SPI_CLK_AF_PP;
		SPI_SDO_AF_PP;		
}

/*************************************************************************************************/
/* 功能：除配置寄存器外所有数据复位																												 	     */
/* 输入：无																                                                       */
/* 输出：无                                                                                      */
/* 描述：复位SRAM数据，配置寄存器不会被操作                 */
/*************************************************************************************************/
void MBI5353_Reset(void)
{
		SPI_CLK_Out_PP;

		//=== 0、初始状态
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1、发送5个clk边沿
		SPI_LE_H;
		for(Uint8 i = 0; i < CMD_SOFTWARE_RESET; i++)	//== 发送确认命令
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		SPI_CLK_AF_PP;
}

/*************************************************************************************************/
/* 功能：主动开通或关断通道																											 	     					 */
/* 输入：Type: 0-关闭输出 1-开启输出																	                           */
/* 输出：无                                                                                      */
/* 描述：所有通道满占空比输出或0占空比关闭                */
/*************************************************************************************************/
void MBI5353_Output_En(Uint8 Type)
{
		Uint8 Cmd = Type ? CMD_ALL_OUTPUT_ENABLE : CMD_ALL_OUTPUT_DISABLE;
		
		SPI_CLK_Out_PP;
		
		//=== 0、初始状态
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1、发送14个clk边沿 确认命令
		SPI_LE_H;
		for(Uint8 i = 0; i < CMD_CONFIRM; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2、发送命令
		SPI_LE_H;
		for(Uint8 i = 0; i < Cmd; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		SPI_CLK_AF_PP;
}

/*************************************************************************************************/
/* 功能：帧同步命令(显示更新)																											 	     					 */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：所存信号持续2 or 3个时钟                */
/*************************************************************************************************/
void MBI5353_FRAME_SYNC(void)
{
		SPI_LE_H;
		for(Uint8 i = 0; i < 2; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
}

/*************************************************************************************************/
/* 功能：通道占空比数据更新																										 	     					 	 */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：48*DEVICES 通道数 GPIO模拟              */
/*************************************************************************************************/
void MBI5353_Update(void)
{
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;

		//=== 0、初始状态
		SPI_LE_L;
		SPI_SCK_L;

		//=== 1、数据更新
		for(Uint8 i = 0; i < 16; i++)
		{
				for(Uint8 j = 0; j < DEVICES;  j++)
				{
						Uint16 rt = MBI5353.LED[i].Pixel[j].R;
						Uint16 gt = MBI5353.LED[i].Pixel[j].G;
						Uint16 bt = MBI5353.LED[i].Pixel[j].B;
						
						for(Uint8 k = 0; k < 16; k++)		//== R
						{
								if(rt & 0x8000)	{SPI_SDI_H;}
								else						{SPI_SDI_L;}
								rt = rt << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
						for(Uint8 k = 0; k < 16; k++)		//== G
						{
								if(gt & 0x8000)	{SPI_SDI_H;}
								else						{SPI_SDI_L;}
								gt = gt << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
						for(Uint8 k = 0; k < 16; k++)		//== B
						{
								//=== 最后1个clk边沿启动所存
								if(j == (DEVICES-1) && k == 15) {SPI_LE_H;SPI_LE_H;SPI_LE_H;}
								
								if(bt & 0x8000)	{SPI_SDI_H;}
								else						{SPI_SDI_L;}
								bt = bt << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
				}
				//=== 锁存结束
				SPI_LE_L;
		}
		
		//=== 2、启动显示
		MBI5353_FRAME_SYNC();
		
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
}

/*************************************************************************************************/
/* 功能：通道占空比数据更新																										 	     					 	 */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：48*DEVICES 通道数 DMA+GPIO模拟 根据测试，3个级联最大可支持2KH刷新              */
/*************************************************************************************************/
void MBI5353_Update_DMA(Uint8 *p)
{
		for(Uint8 cnt = 0; cnt < RGB_NUM*3; cnt++)
		{
				*MBI5353.pLED[0][cnt] = *p++;
		}

		for(Uint16 i = 0; i < 16; i++)
		{
				Uint16 LastHalfWord = 0x00;
		
				//== DMA启动发送前8个HalfWord
				SPI1_TX_START(&MBI5353.LED[i].Pixel[0].R, (DEVICES*3 - 1));
				
				//== 判断SPI是否发送完成：发送完成，SPI总线不忙
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
				{
						
				}
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET)
				{
						
				}
				//== 手动控制IO为推挽输出模式
				SPI_CLK_Out_PP;
				SPI_SDI_Out_PP;
				
				//== 获取最后一个数据
				LastHalfWord = MBI5353.LED[i].Pixel[DEVICES - 1].B;
				for(Uint8 k = 0; k < 16; k++)
				{
						//=== 最后1个clk边沿启动锁存
						if(k == 15) SPI_LE_H;
						
						if(LastHalfWord & 0x8000)	{SPI_SDI_H;}
						else											{SPI_SDI_L;}
						LastHalfWord = LastHalfWord << 1;
						
						SPI_SCK_H;
						SPI_SCK_L;
				}
				SPI_LE_L;
				
				//== 接下来由SPI外设控制IO
				SPI_CLK_AF_PP;
				SPI_SDI_AF_PP;
		}
		
		//== 手动控制IO为推挽输出模式
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;
		
//		RCC_MCOConfig(RCC_MCO_NoClock); //== 关闭灰度时钟
		
		//== 启动显示
		MBI5353_FRAME_SYNC();
		
		//== 下一次先由SPI+DMA外设控制IO
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
		
//		for(Uint8 i = 0; i < 1; i++);	//== 延时1200ns
//		RCC_MCOConfig(RCC_MCO_HSE); //== 8M
}

/*************************************************************************************************/
/* 功能：MBI5353全功能选择																										 	     					 	 */
/* 输入：Sel---选择                                                                              */
/* 输出：无                                                                                      */
/* 描述：0-无操作             			 																														 */
/* 			 1-写入全局配置              																														 */
/* 			 2-读出全局配置              																														 */
/* 			 3-读通道开路状态            																														 */
/* 			 4-读通道短路状态            																														 */
/* 			 5-复位所有数据寄存器(除配置寄存器)           																					 */
/* 			 6-关闭所有通道              																														 */
/* 			 7-开启所有通道(满占空比输出)             																							 */
/* 			 8-写R、G、B组电流配置寄存器              																							 */
/* 			 9-更新所有通道数据(IO模拟)               																							 */
/* 			 A-更新所有通道数据(DMA+IO模拟)           																							 */
/*************************************************************************************************/
void MBI5353_Func(Uint8 Sel)
{
		Uint8 *p = &LED.RBuf[0][0].R;
		
		switch(Sel)
		{
				case 0:																	break;
				case 1:		MBI5353_Write_Config();				break;
				case 2:		MBI5353_Read_Config();				break;
				case 3:		MBI5353_Read_Err(0);					break;
				case 4:		MBI5353_Read_Err(1);					break;
				case 5:		MBI5353_Reset();							break;
				case 6:		MBI5353_Output_En(0);					break;
				case 7:		MBI5353_Output_En(1);					break;
				case 8:		MBI5353_Write_RGB_Config();		break;
				case 9:  	MBI5353_Update();							break;
				default:  MBI5353_Update_DMA(p);				break;		
		}
}
