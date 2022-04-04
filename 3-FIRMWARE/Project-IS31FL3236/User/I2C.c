/*************************************************************************************************/
/*�ļ���IICЭ��										                                                               */
/*��¼��By Zouqb@2021.4.5																																				 */
/*************************************************************************************************/
#include "main.h"
#include "I2C.h"

/*************************************Ӳ��&&ģ��I2Cͨ��*******************************************/

#if !defined I2C_USE_HARDWARE

Uint8 IIC_Start(void);								//== ����IIC��ʼ�ź�
void  IIC_Stop(void);	  						  //== ����IICֹͣ�ź�
Uint8 IIC_Wait_Ack(void); 						//== IIC�ȴ�ACK�ź�
void  IIC_Ack(void);									//== IIC����ACK�ź�
void  IIC_NAck(void);								  //== IIC������ACK�ź�
void  IIC_Send_Byte(Uint8 TxData);		//== IIC����һ���ֽ�
Uint8 IIC_Read_Byte(Uint8 AckSel);    //== IIC��ȡһ���ֽ�

Uint8 I2C_ReadOneByte(Uint8 addr, Uint8 reg);	              //== IIC��ȡָ���豸ָ���Ĵ�������
void IIC_Write_One_Byte(Uint8 addr, Uint8 reg, Uint8 data); //== IIC��ָ���豸ָ���Ĵ���д��ָ������

/*************************************************************************************************/
/* ���ܣ���ʼ��I2C����        																																 	 */
/* ���룺��                                     																								 */
/* �����SCL--PB6                                                                                */
/*       SDA--PB7                                                                                */
/* ��������                                     																								 */
/*************************************************************************************************/
void IIC_Init(void)
{			
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	  //== ʹ��PB�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;	//== ѡ��PB6��PB7
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;      	  //== �������ģʽ
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     	  //== �ٶ�50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					      	  //== �����趨������ʼ��GPIOB

		GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);						  //== ����ʱ�������ź���	
}


/*************************************************************************************************/
/* ���ܣ�����I2C��ʼ�ź�       																																 	 */
/* ���룺��                                     																								 */
/* ���������1��ͨ�������ɹ�                                                                     */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Start(void)
{
    IIC_SDA = 1;							//== SDA�ø�
    if(!READ_SDA) return 0;	  //== ����ȡ��SDA�����ͣ��򷵻�
    IIC_SCL = 1;							//== SCL�ø�
    delay_us(1);						  //== ��ʱ1us
    IIC_SDA = 0;							//== SDA�ɸ߱�ͣ�I2Cͨ�ſ�ʼ
    if(READ_SDA)  return 0;		//== ����ȡ��SDA�����ߣ��򷵻�
    delay_us(1);						  //== ��ʱ1us
    IIC_SCL = 0;							//== ǯסI2C���ߣ�׼�����ͻ�������� 
    return 1;								  //== ����1��ͨ�����������Դ�������
}

/*************************************************************************************************/
/* ���ܣ�����I2Cֹͣ�ź�       																																 	 */
/* ���룺��                                     																								 */
/* ���룺��                                     																								 */
/* ����������ʱ�������ź��������ͨ��                                                            */
/*************************************************************************************************/
void IIC_Stop(void)
{
    IIC_SCL = 0;	            //== SCL�õͣ�SDA��Ч����ֹ�ظ�������ʼ�ź�
    IIC_SDA = 0;	            //== SDA�õͣ�׼������ֹͣ�ź�
    delay_us(1);                   
    IIC_SCL = 1; 	            //== SCL�øߣ�SDA��Ч
    IIC_SDA = 1;	            //== ����I2C���߽����ź�
    delay_us(1);							   	
}

/*************************************************************************************************/
/* ���ܣ��ȴ��ӻ�Ӧ��          																																 	 */
/* ���룺��                                     																								 */
/* �����1--����Ӧ��ɹ�                                                                         */
/* 			 0--����Ӧ��ʧ��                                                                         */
/* ������Ӧ��ʱ�򱨴�                                                                          */
/*************************************************************************************************/
Uint8 IIC_Wait_Ack(void)
{
    Uint8 ErrTime = 0;		    //== ��ʱʱ��
    
    IIC_SDA = 1;
    delay_us(1);	   
    IIC_SCL = 1;
    delay_us(1);	 
    while(READ_SDA)
    {
        if(++ErrTime > 50)    //===ָ��ʱ����δ�յ�Ӧ���ź��򷵻�0
        {   
            IIC_Stop();		    //===ֹͣ�ź�
            return 0;
        }
        delay_us(1);
    }
    IIC_SCL = 0;	   
    return 1;  
} 

/*************************************************************************************************/
/* ���ܣ�����Ӧ���ź�          																																 	 */
/* ���룺��                                     																								 */
/* �������                                                                                      */
/* ����������Ӧ��ӻ�����ʾ�����յ�����                                                          */
/*************************************************************************************************/
void IIC_Ack(void)
{
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);
    IIC_SCL = 0;
}
	
/*************************************************************************************************/
/* ���ܣ�����Ӧ���ź�          																																 	 */
/* ���룺��                                     																								 */
/* �������                                                                                      */
/* ��������9��ʱ������SDA���ָߵ�ƽ���򱻶���Ϊ��Ӧ���źš�                                      */
/*			 ����Ҫô����Stop�źţ�Ҫô����Start�ź����¿�ʼ��                                       */
/*************************************************************************************************/
void IIC_NAck(void)
{
    IIC_SCL = 0;
    IIC_SDA = 1;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);
    IIC_SCL = 0;
}


/*************************************************************************************************/
/* ���ܣ�����һ���ֽ�����       																																 */
/* ���룺��Ҫ���͵��ֽ����ݣ�8bit��                                     												 */
/* �������                                                                                      */
/* ��������9��ʱ������SDA���ָߵ�ƽ���򱻶���Ϊ��Ӧ���źš�                                      */
/*			 ����Ҫô����Stop�źţ�Ҫô����Start�ź����¿�ʼ��                                       */
/*************************************************************************************************/
void IIC_Send_Byte(Uint8 TxData)
{                        
    Uint8 i;
  
    IIC_SCL = 0;						  //== ����ʱ�ӿ�ʼ���ݴ��䡢��λ�ȴ���
    for(i = 0; i < 8; i++)
    {              
        IIC_SDA = (TxData & 0x80) >> 7;
        TxData <<= 1; 	  
        delay_us(1);   				//== ��Ҫ��ʱ
        IIC_SCL = 1;					//== SCL�øߣ���������
        delay_us(1); 						   
        IIC_SCL = 0;					//== SCL�õͣ��������
        delay_us(1);
    }	 
} 	 

/*************************************************************************************************/
/* ���ܣ���һ���ֽ�����          																																 */
/* ���룺AckSel--Ӧ��ѡ��                                                												 */
/* ���������������                                                                              */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Read_Byte(Uint8 AckSel)
{
    Uint8 i, Receive = 0;
  
    for(i = 0; i < 8; i++)    //== ���ζ�ȡ8��SDA��״̬��������Receive���� 
    {
        IIC_SCL = 0; 
        delay_us(2);
        IIC_SCL = 1;					//== SCL�ɵͱ�ߣ�����һ����Ч��ʱ���ź�
        Receive <<= 1;               
        if(READ_SDA)  Receive++;  
        delay_us(2);                 
    }					                     
    if(AckSel)  IIC_Ack(); 		//== ����ACK 
    else        IIC_NAck();		//== ����nACK  
    return Receive;						//== ���ض���������
}

/*************************************************************************************************/
/* ���ܣ�I2C��ָ���豸��ָ���Ĵ�������																				                   */
/* ���룺addr--��ַ��reg--Ҫ��ȡ�ļĴ���                                                         */
/* �������ȡ������                                                                              */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 I2C_ReadOneByte(Uint8 addr, Uint8 reg)
{
    Uint8 res = 0;
    
    IIC_Start();							//== ��ʼ
    IIC_Send_Byte(addr);	    //== ����д����
    res++;                         
    IIC_Wait_Ack();                
    IIC_Send_Byte(reg);				//== ���͵�ַ 
    res++;  									     
    IIC_Wait_Ack();	               
    //IIC_Stop();							//== ����һ��ֹͣ����	
    IIC_Start();                   
    IIC_Send_Byte(addr + 1); 	//== �������ģʽ			 
    res++;                         
    IIC_Wait_Ack();                
    res = IIC_Read_Byte(0);		//== ��������	   
    IIC_Stop();								//== ����һ��ֹͣ����
                                   
    return res;								//== ���ؽ���ֵ
}

/*************************************************************************************************/
/* ���ܣ�I2Cдָ���豸��ָ���Ĵ�������																				                   */
/* ���룺addr--��ַ��reg--Ҫ��ȡ�ļĴ�����data--Ҫд������                                       */
/* �������                                                                                      */
/* ��������                                                                                      */
/*************************************************************************************************/
void IIC_Write_One_Byte(Uint8 addr, Uint8 reg, Uint8 data)
{
    IIC_Start();
    IIC_Send_Byte(addr);	    //== ����д����
    IIC_Wait_Ack();                
    IIC_Send_Byte(reg);       //== ���͵�ַ
    IIC_Wait_Ack();	               
    IIC_Send_Byte(data);           
    IIC_Wait_Ack();                
    IIC_Stop();						    //== ����һ��ֹͣ����
}

/*************************************************************************************************/
/* ���ܣ�I2C��ָ���豸��ָ���Ĵ�����ȡָ�����ȵ�ָ������																				 */
/* ���룺addr--��ַ��reg--Ҫ��ȡ�ļĴ�����len--�ֽ�����*buf--����ָ��                            */
/* �����1��ʾ��ȡ�ɹ���0��ʾ��ȡʧ��                                                            */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Read(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *buf)
{
    if(!IIC_Start())  return 0;   //== ������ʼ�źż�⵽����æ���˳�
    IIC_Send_Byte(addr);			    //== д��ַ
    if(!IIC_Wait_Ack())						//== �ȴ�Ӧ��
		{
        IIC_Stop();
        return 0;
    }
    IIC_Send_Byte(reg);						//== д�Ĵ���
    IIC_Wait_Ack();								//== �ȴ�Ӧ��
    IIC_Start();									//== ���²�����ʼ�źţ�׼����ȡ����
    IIC_Send_Byte(addr + 1);	    //== д��ַ+1
    IIC_Wait_Ack();								//== �ȴ�Ӧ��
    while(len) 										//== ���ݳ��ȶ�ȡ����
		{
        if(len == 1)  *buf = IIC_Read_Byte(0);
        else          *buf = IIC_Read_Byte(1);
        buf++;
        len--;
    }
    IIC_Stop();
		
    return 1;
}

/*************************************************************************************************/
/* ���ܣ�I2C��ָ���豸��ָ���Ĵ���д��ָ�����ȵ�ָ������																				 */
/* ���룺addr--��ַ��reg--Ҫд��ļĴ�����len--�ֽ�����*data--����ָ��                           */
/* �����1��ʾд��ɹ���0��ʾд��ʧ��                                                            */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Write(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *data)
{
		Uint8 i;
  
    if(!IIC_Start())  return 0;   //== ������ʼ�źż�⵽����æ���˳�
    IIC_Send_Byte(addr);		      //== д��ַ
    if(!IIC_Wait_Ack()) 			    //== �ȴ�Ӧ����Ӧ�����˳�������ֹͣ�ź�
		{			
       IIC_Stop();
       return 0;
    }
    IIC_Send_Byte(reg);					  //== д�Ĵ���
    if(!IIC_Wait_Ack()) 			    //== �ȴ�Ӧ����Ӧ�����˳�������ֹͣ�ź�
		{			
       IIC_Stop();
       return 0;
    }
		for(i = 0; i < len; i++) 		  //== д����
		{
        IIC_Send_Byte(data[i]);
        if(!IIC_Wait_Ack())			  //== �ȴ�Ӧ��
        {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();									  //== ����
		
    return 1;
}

#else

/*************************************************************************************************/
/* ���ܣ���ʼ��I2C����        																																 	 */
/* ���룺��                                     																								 */
/* �����SCL--PB6                                                                                */
/*       SDA--PB7                                                                                */
/* ��������                                     																								 */
/*************************************************************************************************/
void IIC_Init(void)
{			
		GPIO_InitTypeDef 	GPIO_InitStructure;
		I2C_InitTypeDef 	I2C_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//== Ӳ��I2C����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_OD;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		I2C_DeInit(I2C1);
		I2C_InitStructure.I2C_Mode								= I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle 					= I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 				= 0x77;					//== ������ַ
		I2C_InitStructure.I2C_Ack 								= I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed 					= I2C_SPEED;				//== ����100kHz
		I2C_Init(I2C1, &I2C_InitStructure);
		I2C_Cmd(I2C1, ENABLE);	
}

/*************************************************************************************************/
/* ���ܣ�I2C��ָ���豸��ָ���Ĵ�����ȡָ�����ȵ�ָ������																				 */
/* ���룺addr--��ַ��reg--Ҫ��ȡ�ļĴ�����len--�ֽ�����*buf--����ָ��                            */
/* �����1��ʾ��ȡ�ɹ���0��ʾ��ȡʧ��                                                            */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Read(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *buf)
{
		Uint8 Temp;
	
		do
		{
				I2C_GenerateSTART(I2C1, ENABLE);
				Temp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
				I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
			
		}while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

		I2C_ClearFlag(I2C1, I2C_FLAG_AF);
		
		I2C_GenerateSTART(I2C1, ENABLE);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
		if(len == 1)
		{
				I2C_AcknowledgeConfig(I2C1, DISABLE);
		}

		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		I2C_Cmd(I2C1, ENABLE);

		I2C_SendData(I2C1, reg);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		I2C_GenerateSTART(I2C1, ENABLE);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

		while(len)
		{
				if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
				{
						if(len == 2)	I2C_AcknowledgeConfig(I2C1, DISABLE);
						if(len == 1)	I2C_GenerateSTOP(I2C1, ENABLE);

						*buf = I2C_ReceiveData(I2C1);

						buf++;
						len--;
				}
		}

		I2C_AcknowledgeConfig(I2C1, ENABLE);
		
		return 1;
}

/*************************************************************************************************/
/* ���ܣ�I2C��ָ���豸��ָ���Ĵ���д��ָ�����ȵ�ָ������																				 */
/* ���룺addr--��ַ��reg--Ҫд��ļĴ�����len--�ֽ�����*data--����ָ��                           */
/* �����1��ʾд��ɹ���0��ʾд��ʧ��                                                            */
/* ��������                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Write(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *data)
{
		Uint8 Index = 0;
		if(len == 0) return 1;
		
		//=== 1����ʼ�������ȴ���Ӧ(EV5)
		I2C_GenerateSTART(I2C1, ENABLE);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
		//=== 2��������ַ���ȴ���Ӧ(EV6)
		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		//=== 3���Ĵ�����ַ���ȴ���Ӧ(EV8)
		I2C_SendData(I2C1, reg);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
		//=== 4������д���ݣ��ȴ���Ӧ(EV8)
		while(len)
		{
				I2C_SendData(I2C1, data[Index]);
				while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
				Index++;
				len--;
		}
		//=== 5��������ɡ�����ֹͣ����
		I2C_GenerateSTOP(I2C1, ENABLE);
		
		return 1;
}
#endif
