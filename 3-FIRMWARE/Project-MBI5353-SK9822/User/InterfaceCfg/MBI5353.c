#include "MBI5353.h"
#include "SPI.h"

#define SPI_DMA_ENABLE

TYPE_MBI5353 		MBI5353 = MBI5353_DEFS;

/*************************************************************************************************/
/* ���ܣ���ʼ������																																	 	             */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ������1����������                                */
/*       2������MBI5353ȫ������                                                       */
/*       3��֡��ʼ��(�Ȳ���)	                                                   */
/*       4��ָ���ʼ��(������)	                                                   */
/*************************************************************************************************/
void MBI5353_Init(void)
{
		//=== 1����ʼ�����ź�SPI����
		SPI1_Init();
		
		//=== 2����ʼ��
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;
		SPI_SDO_Out_PP;
		
		MBI5353.Global.R1.bit.GCLK_Mul_En = 1;
		MBI5353.Global.R1.bit.Line_Num = 0;//0x1F;	//== ɨ����1��
		MBI5353.Global.R1.bit.SPWM_Mode = 3;
		MBI5353.Global.R1.bit.Gray_Mode = 0;

		MBI5353.Global.R2.bit.Con_Gain = 1;		//== ȫ�ֵ�������47.6->15.8
//		MBI5353.Global.R2.bit.Adj_Gain = 0;
//		MBI5353.Global.R2.bit.Power_En = 1;	//== ��������Ϊ0ʱ����

//		MBI5353.Global.R3.bit.BlackPreChargeEn = 1;	//== ��ʼ�Ҷ�Ϊ0

		MBI5353_Write_Config();
		
//		MBI5353.RGB.bit.CurGain = 32;	//== R/G/B��������
//		MBI5353_Write_RGB_Config();

		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
		SPI_SDO_AF_PP;
		
		//=== 3��
//		MBI5353_Frame_Init();

		//=== 4��ָ���ʼ��
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
/* ���ܣ�֡(SRAM)��ʼ��																														 	             */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* �������ϵ�ʱԤ��ʾ����                                */
/*************************************************************************************************/
void MBI5353_Frame_Init(void)
{
		//=== 0����ʼ״̬
		SPI_LE_L;
		RCC_MCOConfig(RCC_MCO_NoClock); //== �رջҶ�ʱ��
		
		//=== 1����ʼ��ʾ����������(�����������ȷ��1��32ɨ����)
		MBI5353_Update();
		
		//=== 2������2��clk���� ֡ͬ������ ��ʾ����
		SPI_CLK_Out_PP;
		SPI_LE_H;
		for(Uint8 i = 0; i < 3; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		SPI_CLK_AF_PP;
		
		//=== 3����ʱ1.2us����
		for(Uint8 i = 0; i < 144; i++);
		
		RCC_MCOConfig(RCC_MCO_HSE); //== 8M
}

/*************************************************************************************************/
/* ���ܣ�д��ȫ�����üĴ���																												 	             */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ���������ݼ�����MBI5353����д��ȫ�����ã�ÿ��������3��16bitȫ�ּĴ���(R1/2/3)                 */
/* 			 GPIOģ����ƣ�д����������Ÿ�ΪSPI����                 */
/*************************************************************************************************/
void MBI5353_Write_Config(void)
{
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;

		//=== 0����ʼ״̬
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1������14��clk���� ȷ������
		SPI_LE_H;
		for(Uint8 i = 0; i < CMD_CONFIRM; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2������48bit�������� MSB����
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
						//=== ���4��clk������������д��
						if(i == (DEVICES-1) && j == 12) SPI_LE_H;
						
						if(DataR3 & 0x8000)	{SPI_SDI_H;}
						else								{SPI_SDI_L;}
						DataR3 = DataR3 << 1;
						SPI_SCK_H;
						SPI_SCK_L;
				}
		}
		
		//=== 3������д�����
		SPI_LE_L;
		
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
}

/*************************************************************************************************/
/* ���ܣ�д��ȫ����ɫ��������üĴ���																												 	   */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ���������ݼ�����MBI5353����д��ȫ�����ã�ÿ��������3��16bitȫ����ɫ�������üĴ���(R/G/B)      */
/* 			 GPIOģ����ƣ�д����������Ÿ�ΪSPI����                 */
/*************************************************************************************************/
void MBI5353_Write_RGB_Config(void)
{
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;

		for(Uint8 i = 1; i < 4; i++)
		{
				//=== 0����ʼ״̬
				SPI_LE_L;
				SPI_SCK_L;
				
				//=== 1������14��clk���� ȷ������
				SPI_LE_H;
				for(Uint8 i0 = 0; i0 < CMD_CONFIRM; i0++)
				{
						SPI_SCK_H;
						SPI_SCK_L;
				}
				SPI_LE_L;
				
				//=== 2����ʼ��2bitΪ��ɫ��ѡ�񣬷���48bit�������� MSB����
				MBI5353.RGB.bit.Sel = i;
				for(Uint8 i1 = 0; i1 < DEVICES;  i1++)
				{
						Uint16 DataR1 = MBI5353.RGB.all;	//== R/G/B���üĴ�������һ��
						Uint16 DataR2 = 0;								//== �������������ֲ��޶���
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
								//=== ���4��clk������������д��
								if(i1 == DEVICES && j == 12) SPI_LE_H;
								
								if(DataR3 & 0x8000)	{SPI_SDI_H;}
								else								{SPI_SDI_L;}
								DataR3 = DataR3 << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
				}
				
				//=== 3������д�����
				SPI_LE_L;
		}
		
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
}

/*************************************************************************************************/
/* ���ܣ�����ȫ�����üĴ���																												 	             */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ���������ݼ�����MBI5353��������ȫ�����ã�ÿ��������3��16bitȫ�ּĴ���(R1/2/3)                 */
/* 			 GPIOģ����ƣ���ȡ���������Ÿ�ΪSPI����                 */
/*************************************************************************************************/
void MBI5353_Read_Config(void)
{
		SPI_CLK_Out_PP;
		SPI_SDO_Out_PP;
		
		//=== 0����ʼ״̬
		SPI_LE_L;
		SPI_SCK_L;
		
		SPI_LE_H;
		//=== 1������5��clk����
		for(Uint8 i = 0; i < CMD_READ_GLOBAL_REG; i++)	//== ����ȷ������
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2����ȡ3��������SDO״̬
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
				
//				//=== ��������48BIT�յ�
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
/* ���ܣ�����ͨ��״̬(��·0����·1)																												 	     */
/* ���룺Type:0-��·ͨ����ȡ 1-��·ͨ����ȡ                                                      */
/* �������                                                                                      */
/* ���������ݼ�����MBI5353������������ͨ���Ĵ���״̬                 */
/* 			 GPIOģ����ƣ���ȡ���������Ÿ�ΪSPI����                 */
/*************************************************************************************************/
void MBI5353_Read_Err(Uint8 Type)
{
		Uint8 Cmd = Type ? CMD_SHOT_ERR_ENABLE : CMD_OPEN_ERR_ENABLE;
		
		SPI_CLK_Out_PP;
		SPI_SDO_Out_PP;
		
		//=== 0����ʼ״̬
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1������5��clk����
		SPI_LE_H;
		for(Uint8 i = 0; i < Cmd; i++)	//== ����ȷ������
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2����ʱ����700ns
		for(Uint8 i = 0; i < 14; i++);
		
		//=== 3��ֹͣ�������׼����ȡ����
		for(Uint8 i = 0; i < CMD_STOP_ERR_CHECK_REG; i++)	//== ����ȷ������
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		
		//=== 4����ȡ��־
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
				
				//=== ��������48BIT�յ�
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
/* ���ܣ������üĴ������������ݸ�λ																												 	     */
/* ���룺��																                                                       */
/* �������                                                                                      */
/* ��������λSRAM���ݣ����üĴ������ᱻ����                 */
/*************************************************************************************************/
void MBI5353_Reset(void)
{
		SPI_CLK_Out_PP;

		//=== 0����ʼ״̬
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1������5��clk����
		SPI_LE_H;
		for(Uint8 i = 0; i < CMD_SOFTWARE_RESET; i++)	//== ����ȷ������
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		SPI_CLK_AF_PP;
}

/*************************************************************************************************/
/* ���ܣ�������ͨ��ض�ͨ��																											 	     					 */
/* ���룺Type: 0-�ر���� 1-�������																	                           */
/* �������                                                                                      */
/* ����������ͨ����ռ�ձ������0ռ�ձȹر�                */
/*************************************************************************************************/
void MBI5353_Output_En(Uint8 Type)
{
		Uint8 Cmd = Type ? CMD_ALL_OUTPUT_ENABLE : CMD_ALL_OUTPUT_DISABLE;
		
		SPI_CLK_Out_PP;
		
		//=== 0����ʼ״̬
		SPI_LE_L;
		SPI_SCK_L;
		
		//=== 1������14��clk���� ȷ������
		SPI_LE_H;
		for(Uint8 i = 0; i < CMD_CONFIRM; i++)
		{
				SPI_SCK_H;
				SPI_SCK_L;
		}
		SPI_LE_L;
		
		//=== 2����������
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
/* ���ܣ�֡ͬ������(��ʾ����)																											 	     					 */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* �����������źų���2 or 3��ʱ��                */
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
/* ���ܣ�ͨ��ռ�ձ����ݸ���																										 	     					 	 */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ������48*DEVICES ͨ���� GPIOģ��              */
/*************************************************************************************************/
void MBI5353_Update(void)
{
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;

		//=== 0����ʼ״̬
		SPI_LE_L;
		SPI_SCK_L;

		//=== 1�����ݸ���
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
								//=== ���1��clk������������
								if(j == (DEVICES-1) && k == 15) {SPI_LE_H;SPI_LE_H;SPI_LE_H;}
								
								if(bt & 0x8000)	{SPI_SDI_H;}
								else						{SPI_SDI_L;}
								bt = bt << 1;
								SPI_SCK_H;
								SPI_SCK_L;
						}
				}
				//=== �������
				SPI_LE_L;
		}
		
		//=== 2��������ʾ
		MBI5353_FRAME_SYNC();
		
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
}

/*************************************************************************************************/
/* ���ܣ�ͨ��ռ�ձ����ݸ���																										 	     					 	 */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ������48*DEVICES ͨ���� DMA+GPIOģ�� ���ݲ��ԣ�3����������֧��2KHˢ��              */
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
		
				//== DMA��������ǰ8��HalfWord
				SPI1_TX_START(&MBI5353.LED[i].Pixel[0].R, (DEVICES*3 - 1));
				
				//== �ж�SPI�Ƿ�����ɣ�������ɣ�SPI���߲�æ
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
				{
						
				}
				while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET)
				{
						
				}
				//== �ֶ�����IOΪ�������ģʽ
				SPI_CLK_Out_PP;
				SPI_SDI_Out_PP;
				
				//== ��ȡ���һ������
				LastHalfWord = MBI5353.LED[i].Pixel[DEVICES - 1].B;
				for(Uint8 k = 0; k < 16; k++)
				{
						//=== ���1��clk������������
						if(k == 15) SPI_LE_H;
						
						if(LastHalfWord & 0x8000)	{SPI_SDI_H;}
						else											{SPI_SDI_L;}
						LastHalfWord = LastHalfWord << 1;
						
						SPI_SCK_H;
						SPI_SCK_L;
				}
				SPI_LE_L;
				
				//== ��������SPI�������IO
				SPI_CLK_AF_PP;
				SPI_SDI_AF_PP;
		}
		
		//== �ֶ�����IOΪ�������ģʽ
		SPI_CLK_Out_PP;
		SPI_SDI_Out_PP;
		
//		RCC_MCOConfig(RCC_MCO_NoClock); //== �رջҶ�ʱ��
		
		//== ������ʾ
		MBI5353_FRAME_SYNC();
		
		//== ��һ������SPI+DMA�������IO
		SPI_CLK_AF_PP;
		SPI_SDI_AF_PP;
		
//		for(Uint8 i = 0; i < 1; i++);	//== ��ʱ1200ns
//		RCC_MCOConfig(RCC_MCO_HSE); //== 8M
}

/*************************************************************************************************/
/* ���ܣ�MBI5353ȫ����ѡ��																										 	     					 	 */
/* ���룺Sel---ѡ��                                                                              */
/* �������                                                                                      */
/* ������0-�޲���             			 																														 */
/* 			 1-д��ȫ������              																														 */
/* 			 2-����ȫ������              																														 */
/* 			 3-��ͨ����·״̬            																														 */
/* 			 4-��ͨ����·״̬            																														 */
/* 			 5-��λ�������ݼĴ���(�����üĴ���)           																					 */
/* 			 6-�ر�����ͨ��              																														 */
/* 			 7-��������ͨ��(��ռ�ձ����)             																							 */
/* 			 8-дR��G��B��������üĴ���              																							 */
/* 			 9-��������ͨ������(IOģ��)               																							 */
/* 			 A-��������ͨ������(DMA+IOģ��)           																							 */
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
