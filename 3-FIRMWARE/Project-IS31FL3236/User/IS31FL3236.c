/*************************************************************************************************/
/*�ļ���IS31FL3236оƬ�ײ�����						                                                       */
/*������֧��Ӳ�������I2Cͨ�ţ��ɺ궨�����																											 */
/*��¼��By Zouqb@2021.4.5																																				 */
/*************************************************************************************************/
#include "IS31FL3236.h"
#include "I2C.h"

TYPE_IS31_LED   IS31_LED = IS31_LED_DEFS;


/*************************************************************************************************/
/* ���ܣ���������																																	 	             */
/* ���룺��                                                                                      */
/* �������                                                                                      */
/* ������1���������ţ�SDB1--PB12,SDB2--PB13,SDB3--PB14,SDB4--PB15                                */
/*       2��IIC�������ã�SCL--PB6,SDA--PB7                                                       */
/*       3����תLED��ʼ��	                                                   */
/*************************************************************************************************/
void POV_LED_GPIO_Init(void)
{
		#ifdef POV_LED_EN					
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    //=== 1������Ƭѡ�źţ�SDB1--PB12,SDB2--PB13,SDB3--PB14,SDB4--PB15������I2C����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     	
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      	
    GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);		//== ֱ��ʹ��
  
    //=== 2������I2C
    IIC_Init();
		
		//=== 3����תLED��ʼ��	
		POV_LED_Init(IS31FL3236_ADDRESS1, IS31_LED.DutyRatio1);							
		POV_LED_Init(IS31FL3236_ADDRESS2, IS31_LED.DutyRatio2);							//== ��תLED��ʼ��
		POV_LED_Init(IS31FL3236_ADDRESS3, IS31_LED.DutyRatio3);							//== ��תLED��ʼ��
		POV_LED_Init(IS31FL3236_ADDRESS4, IS31_LED.DutyRatio4);							//== ��תLED��ʼ��
		#endif
}

/*************************************************************************************************/
/* ���ܣ���ʼ��ָ��IS31��״̬																																	 	 */
/* ���룺Device--�豸��ַ��Data--ռ�ձ������׵�ַ																								 */
/* �������                                                                                      */
/* ������0����λĿ���豸                                                                         */
/* 			 1��ʹ��Ŀ���豸                                                                         */
/*			 2�����ó�ʼռ�ձȡ�����LEDͨ������������������																				 */
/*************************************************************************************************/
void POV_LED_Init(Uint8 Device, Uint8* Data)
{
		Uint8 Index[1];
		
		//=== 0����λĿ���豸
		Index[0] = RESET_IS31_ENABLE;
		IIC_Write(Device, RESET_IS31, 1, Index);
		
		//=== 1������Ŀ���豸(4��)
//		IIC_Write_One_Byte(Device, SHUTDOWN, SHUTDOWN_DISABLE);
		Index[0] = SHUTDOWN_DISABLE;
		IIC_Write(Device, SHUTDOWN, 1, Index);
  
		//=== 2������ÿ��LEDͨ����ʼռ�ձ�(0~255)��50
		IIC_Write(Device, PWM_OUT(0x01), 36, Data);
  
		//=== 3������LEDÿ��ͨ������(Imax/4)��ʹ��ͨ��
		Index[0] = LED_IMAX_Div4;
		for(Uint8 i = 1; i <= 36; i++)
		{				
				IIC_Write(Device, LED_CTRL(i), 1, Index);
		}
//		IIC_Write(Device, LED_CTRL(0x01), 36, Data);
	 
		//=== 4������ÿ��LEDͨ�����Ƶ��
		Index[0] = PWM_FRQ_22KHZ;
		IIC_Write(Device, PWM_FRQ, 1, Index);
	 
		//=== 5����������LED������� 
//		IIC_Write_One_Byte(Device, LED_UPDATE, LED_UPDATE_ENABLE);
		Index[0] = LED_UPDATE_ENABLE;
		IIC_Write(Device, LED_UPDATE, 1, Index);
}

/*************************************************************************************************/
/* ���ܣ�����Ŀ���豸����LEDͨ��ռ�ձ�																										 	 		 */
/* ���룺Device--�豸��ַ��Data--ռ�ձ������׵�ַ																								 */
/* ������д��Ŀ���豸����LEDռ�ձȼĴ���                                                     		 */
/*************************************************************************************************/
void POV_LED_Set_Duty(Uint8 Device, Uint8* Data)
{
		//=== д��ÿ��ͨ����ռ�ձ�
		IIC_Write(Device, PWM_OUT(0x01), 36, Data);
	
}

/*************************************************************************************************/
/* ���ܣ�����Ŀ���豸LED���																									 									 */
/* ���룺Device--�豸��ַ																																				 */
/* ��������������Ĵ���																				 																	 */
/*************************************************************************************************/
void POV_LED_Set_Output(Uint8 Device)
{
		Uint8 Index[1] ;
	
		//=== ���
//		IIC_Write_One_Byte(Device, LED_UPDATE, LED_UPDATE_ENABLE);
		Index[0] = LED_UPDATE_ENABLE;
		IIC_Write(Device, LED_UPDATE, 1, Index);
			
}

/*************************************************************************************************/
/* ���ܣ����������豸���ͨ��																									 	 		 */
/* ���룺��																								 */
/* ������1��д�������豸����LEDռ�ձȼĴ���                                                     		 */
/* 			 2��ʹ�������豸ͨ�����*/
/*************************************************************************************************/
void POV_LED_Update(TYPE_IS31_LED *p)
{
		Uint8 Index[1] ;
		
		Index[0] = LED_UPDATE_ENABLE;
	
		//=== 1��д��ÿ��ͨ����ռ�ձ�
		IIC_Write(IS31FL3236_ADDRESS1, PWM_OUT(0x01), 36, p->DutyRatio1);
		IIC_Write(IS31FL3236_ADDRESS2, PWM_OUT(0x01), 36, p->DutyRatio2);
		IIC_Write(IS31FL3236_ADDRESS3, PWM_OUT(0x01), 36, p->DutyRatio3);
		IIC_Write(IS31FL3236_ADDRESS4, PWM_OUT(0x01), 36, p->DutyRatio4);
		
		//=== 2�����
		IIC_Write(IS31FL3236_ADDRESS1, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS2, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS3, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS4, LED_UPDATE, 1, Index);
	
}

/*************************************************************************************************/
/* ���ܣ����������豸���ͨ��																									 	 		 */
/* ���룺��																								 */
/* ������1��д�������豸����LEDռ�ձȼĴ���                                                     		 */
/* 			 2��ʹ�������豸ͨ�����*/
/*************************************************************************************************/
void POV_LED_Update1(Uint8 *Buf)
{
		Uint8 Index[1] ;
	
		//=== 1��д��ÿ��ͨ����ռ�ձ�
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
		
		//=== 2�����
		Index[0] = LED_UPDATE_ENABLE;
		IIC_Write(IS31FL3236_ADDRESS1, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS2, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS3, LED_UPDATE, 1, Index);
		IIC_Write(IS31FL3236_ADDRESS4, LED_UPDATE, 1, Index);
	
}

/*************************************************************************************************/
/* ���ܣ�������ʾ	500Hzˢ��																								 	 		 */
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
/* ���ܣ�������ʾ	500Hzˢ��																								 	 		 */
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
/* ���ܣ�������ʾ	500Hzˢ��																								 	 		 */
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
/* ���ܣ�������ʾ	500Hzˢ��																								 	 		 */
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
/* ���ܣ�������ʾ	500Hzˢ��																								 	 		 */
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
