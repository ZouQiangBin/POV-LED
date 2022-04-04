/*************************************************************************************************/
/*文件：IS31FL3236芯片底层驱动						                                                       */
/*描述：支持硬件和软件I2C通信，由宏定义决定																											 */
/*记录：By Zouqb@2021.4.5																																				 */
/*************************************************************************************************/
#include "IS31FL3236.h"
#include "I2C.h"

TYPE_IS31_LED   IS31_LED = IS31_LED_DEFS;


/*************************************************************************************************/
/* 功能：引脚配置																																	 	             */
/* 输入：无                                                                                      */
/* 输出：无                                                                                      */
/* 描述：1、配置引脚：SDB1--PB12,SDB2--PB13,SDB3--PB14,SDB4--PB15                                */
/*       2、IIC引脚配置：SCL--PB6,SDA--PB7                                                       */
/*       3、旋转LED初始化	                                                   */
/*************************************************************************************************/
void POV_LED_GPIO_Init(void)
{
		#ifdef POV_LED_EN					
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    //=== 1、配置片选信号：SDB1--PB12,SDB2--PB13,SDB3--PB14,SDB4--PB15，配置I2C引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     	
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      	
    GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);		//== 直接使能
  
    //=== 2、配置I2C
    IIC_Init();
		
		//=== 3、旋转LED初始化	
		POV_LED_Init(IS31FL3236_ADDRESS1, IS31_LED.DutyRatio1);							
		POV_LED_Init(IS31FL3236_ADDRESS2, IS31_LED.DutyRatio2);							//== 旋转LED初始化
		POV_LED_Init(IS31FL3236_ADDRESS3, IS31_LED.DutyRatio3);							//== 旋转LED初始化
		POV_LED_Init(IS31FL3236_ADDRESS4, IS31_LED.DutyRatio4);							//== 旋转LED初始化
		#endif
}

/*************************************************************************************************/
/* 功能：初始化指定IS31的状态																																	 	 */
/* 输入：Device--设备地址，Data--占空比数组首地址																								 */
/* 输出：无                                                                                      */
/* 描述：0、复位目标设备                                                                         */
/* 			 1、使能目标设备                                                                         */
/*			 2、设置初始占空比、开启LED通道、设置最大输出电流																				 */
/*************************************************************************************************/
void POV_LED_Init(Uint8 Device, Uint8* Data)
{
		Uint8 Index[1];
		
		//=== 0、复位目标设备
		Index[0] = RESET_IS31_ENABLE;
		IIC_Write(Device, RESET_IS31, 1, Index);
		
		//=== 1、启用目标设备(4个)
//		IIC_Write_One_Byte(Device, SHUTDOWN, SHUTDOWN_DISABLE);
		Index[0] = SHUTDOWN_DISABLE;
		IIC_Write(Device, SHUTDOWN, 1, Index);
  
		//=== 2、设置每个LED通道初始占空比(0~255)：50
		IIC_Write(Device, PWM_OUT(0x01), 36, Data);
  
		//=== 3、设置LED每个通道电流(Imax/4)并使能通道
		Index[0] = LED_IMAX_Div4;
		for(Uint8 i = 1; i <= 36; i++)
		{				
				IIC_Write(Device, LED_CTRL(i), 1, Index);
		}
//		IIC_Write(Device, LED_CTRL(0x01), 36, Data);
	 
		//=== 4、设置每个LED通道输出频率
		Index[0] = PWM_FRQ_22KHZ;
		IIC_Write(Device, PWM_FRQ, 1, Index);
	 
		//=== 5、更新以上LED相关设置 
//		IIC_Write_One_Byte(Device, LED_UPDATE, LED_UPDATE_ENABLE);
		Index[0] = LED_UPDATE_ENABLE;
		IIC_Write(Device, LED_UPDATE, 1, Index);
}

/*************************************************************************************************/
/* 功能：更新目标设备所有LED通道占空比																										 	 		 */
/* 输入：Device--设备地址，Data--占空比数组首地址																								 */
/* 描述：写入目标设备所有LED占空比寄存器                                                     		 */
/*************************************************************************************************/
void POV_LED_Set_Duty(Uint8 Device, Uint8* Data)
{
		//=== 写入每个通道的占空比
		IIC_Write(Device, PWM_OUT(0x01), 36, Data);
	
}

/*************************************************************************************************/
/* 功能：更新目标设备LED输出																									 									 */
/* 输入：Device--设备地址																																				 */
/* 描述：设置输出寄存器																				 																	 */
/*************************************************************************************************/
void POV_LED_Set_Output(Uint8 Device)
{
		Uint8 Index[1] ;
	
		//=== 输出
//		IIC_Write_One_Byte(Device, LED_UPDATE, LED_UPDATE_ENABLE);
		Index[0] = LED_UPDATE_ENABLE;
		IIC_Write(Device, LED_UPDATE, 1, Index);
			
}

/*************************************************************************************************/
/* 功能：更新所有设备输出通道																									 	 		 */
/* 输入：无																								 */
/* 描述：1、写入所有设备所有LED占空比寄存器                                                     		 */
/* 			 2、使能所有设备通道输出*/
/*************************************************************************************************/
void POV_LED_Update(TYPE_IS31_LED *p)
{
		Uint8 Index[1] ;
		
		Index[0] = LED_UPDATE_ENABLE;
	
		//=== 1、写入每个通道的占空比
		IIC_Write(IS31FL3236_ADDRESS1, PWM_OUT(0x01), 36, p->DutyRatio1);
		IIC_Write(IS31FL3236_ADDRESS2, PWM_OUT(0x01), 36, p->DutyRatio2);
		IIC_Write(IS31FL3236_ADDRESS3, PWM_OUT(0x01), 36, p->DutyRatio3);
		IIC_Write(IS31FL3236_ADDRESS4, PWM_OUT(0x01), 36, p->DutyRatio4);
		
		//=== 2、输出
		IIC_Write(IS31FL3236_ADDRESS1, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS2, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS3, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS4, LED_UPDATE, 1, Index);
	
}

/*************************************************************************************************/
/* 功能：更新所有设备输出通道																									 	 		 */
/* 输入：无																								 */
/* 描述：1、写入所有设备所有LED占空比寄存器                                                     		 */
/* 			 2、使能所有设备通道输出*/
/*************************************************************************************************/
void POV_LED_Update1(Uint8 *Buf)
{
		Uint8 Index[1] ;
	
		//=== 1、写入每个通道的占空比
		IIC_Write(IS31FL3236_ADDRESS1, PWM_OUT(0x01), 36, Buf);
		IIC_Write(IS31FL3236_ADDRESS2, PWM_OUT(0x01), 36, Buf+36);
		IIC_Write(IS31FL3236_ADDRESS3, PWM_OUT(0x01), 36, Buf+72);
		IIC_Write(IS31FL3236_ADDRESS4, PWM_OUT(0x01), 36, Buf+108);
//		for(Uint8 i = 0; i < 12; i++)
//		{
//				Uint8 Temp = i*3;
//				IIC_Write(IS31FL3236_ADDRESS1, PWM_OUT(0x01+Temp), 1, Buf+Temp);
//				IIC_Write(IS31FL3236_ADDRESS2, PWM_OUT(0x01+Temp), 1, Buf+Temp+36);
//				IIC_Write(IS31FL3236_ADDRESS3, PWM_OUT(0x01+Temp), 1, Buf+Temp+72);
//				IIC_Write(IS31FL3236_ADDRESS4, PWM_OUT(0x01+Temp), 1, Buf+Temp+108);
//		}
		
		//=== 2、输出
		Index[0] = LED_UPDATE_ENABLE;
		IIC_Write(IS31FL3236_ADDRESS1, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS2, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS3, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS4, LED_UPDATE, 1, Index);
	
}

/*************************************************************************************************/
/* 功能：呼吸演示	500Hz刷新																								 	 		 */
/*************************************************************************************************/
void POV_LED_Show0(Uint8 *p)
{
		//===
		IS31_LED.DutyRatio1[LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[LED.RList + 2] = 0;
		LED.RList += 3;
		if(LED.RList >= 144)
		{
				LED.RList = 0;
				(*p)++;
		}
		IS31_LED.DutyRatio1[LED.RList + 0] = LED.RList;
		IS31_LED.DutyRatio1[LED.RList + 1] = LED.RList;
		IS31_LED.DutyRatio1[LED.RList + 2] = LED.RList;
}

/*************************************************************************************************/
/* 功能：呼吸演示	500Hz刷新																								 	 		 */
/*************************************************************************************************/
void POV_LED_Show1(Uint8 *p)
{
		//===
		IS31_LED.DutyRatio1[LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[LED.RList + 2] = 0;
		IS31_LED.DutyRatio1[141 - LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[141 - LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[141 - LED.RList + 2] = 0;
		LED.RList += 3;
		if(LED.RList >= 72)
		{
				LED.RList = 0;
				(*p)++;
		}
		IS31_LED.DutyRatio1[LED.RList + 0] = LED.RList;
		IS31_LED.DutyRatio1[LED.RList + 1] = LED.RList;
		IS31_LED.DutyRatio1[LED.RList + 2] = LED.RList;
		IS31_LED.DutyRatio1[141 - LED.RList + 0] = LED.RList;
		IS31_LED.DutyRatio1[141 - LED.RList + 1] = LED.RList;
		IS31_LED.DutyRatio1[141 - LED.RList + 2] = LED.RList;
}

/*************************************************************************************************/
/* 功能：呼吸演示	500Hz刷新																								 	 		 */
/*************************************************************************************************/
void POV_LED_Show2(Uint8 *p)
{
		//===
		IS31_LED.DutyRatio1[00  + LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[00  + LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[00  + LED.RList + 2] = 0;
		IS31_LED.DutyRatio1[71  - LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[71  - LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[71  - LED.RList + 2] = 0;
		IS31_LED.DutyRatio1[72  + LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[72  + LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[72  + LED.RList + 2] = 0;
		IS31_LED.DutyRatio1[141 - LED.RList + 0] = 0;
		IS31_LED.DutyRatio1[141 - LED.RList + 1] = 0;
		IS31_LED.DutyRatio1[141 - LED.RList + 2] = 0;
		LED.RList += 3;
		if(LED.RList >= 36)
		{
				LED.RList = 0;
				(*p)++;
		}
		IS31_LED.DutyRatio1[00  + LED.RList + 0] = LED.RList * 4;
		IS31_LED.DutyRatio1[00  + LED.RList + 1] = LED.RList * 4;
		IS31_LED.DutyRatio1[00  + LED.RList + 2] = LED.RList * 4;
		IS31_LED.DutyRatio1[71  - LED.RList + 0] = LED.RList * 4;
		IS31_LED.DutyRatio1[71  - LED.RList + 1] = LED.RList * 4;
		IS31_LED.DutyRatio1[71  - LED.RList + 2] = LED.RList * 4;
		IS31_LED.DutyRatio1[72  + LED.RList + 0] = LED.RList * 4;
		IS31_LED.DutyRatio1[72  + LED.RList + 1] = LED.RList * 4;
		IS31_LED.DutyRatio1[72  + LED.RList + 2] = LED.RList * 4;
		IS31_LED.DutyRatio1[141 - LED.RList + 0] = LED.RList * 4;
		IS31_LED.DutyRatio1[141 - LED.RList + 1] = LED.RList * 4;
		IS31_LED.DutyRatio1[141 - LED.RList + 2] = LED.RList * 4;
}

/*************************************************************************************************/
/* 功能：呼吸演示	500Hz刷新																								 	 		 */
/*************************************************************************************************/
void POV_LED_Show3(void)
{
		static Uint8  Dir = 0;
		static Uint32 rgb = 0xFF;
		static TYPE_COLOR 		RGB,RGB1 = {0};
		
		if(!Dir)
		{
				if(++LED.RList >= 32)	
				{
						Dir = !Dir;
						
						rgb += 0x80;
						//rgb = CycL32(rgb, 1);
						if(rgb >= 0xFFFFFF)	rgb = 0x08;
						
						RGB1.R = (rgb & 0xFF);
						RGB1.G = ((rgb >> 8) & 0xFF);
						RGB1.B = ((rgb >> 16) & 0xFF);
				}
		}
		else
		{
				if(--LED.RList <= 01)	
				{
						Dir = !Dir;
				}
		}
		
		RGB.R = ((Uint16)RGB1.R*LED.RList) >> 5;
		RGB.G = ((Uint16)RGB1.G*LED.RList) >> 5;
		RGB.B = ((Uint16)RGB1.B*LED.RList) >> 5;
		
		for(Uint8 i = 0;i < 144; i+=3)
		{
				IS31_LED.DutyRatio1[i + 0] = RGB.R;//LED.RList;
				IS31_LED.DutyRatio1[i + 1] = RGB.G;//LED.RList;
				IS31_LED.DutyRatio1[i + 2] = RGB.B;//LED.RList;
		}
}

/*************************************************************************************************/
/* 功能：呼吸演示	500Hz刷新																								 	 		 */
/*************************************************************************************************/
void POV_LED_Show(void)
{
		static Uint8 Step = 0;
		switch(Step)
		{
				case 0: 
									POV_LED_Show0(&Step);
									break;
				case 1: 
									POV_LED_Show1(&Step);
									break;
				case 2: 
									POV_LED_Show2(&Step);
									break;
				case 3: 
									POV_LED_Show3();
									break;
				default:
									Step = 0;
									break;
		}
		
}
