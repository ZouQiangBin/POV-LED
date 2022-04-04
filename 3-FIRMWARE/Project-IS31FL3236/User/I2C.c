/*************************************************************************************************/
/*文件：IIC协议										                                                               */
/*记录：By Zouqb@2021.4.5																																				 */
/*************************************************************************************************/
#include "main.h"
#include "I2C.h"

/*************************************硬件&&模拟I2C通信*******************************************/

#if !defined I2C_USE_HARDWARE

Uint8 IIC_Start(void);								//== 发送IIC开始信号
void  IIC_Stop(void);	  						  //== 发送IIC停止信号
Uint8 IIC_Wait_Ack(void); 						//== IIC等待ACK信号
void  IIC_Ack(void);									//== IIC发送ACK信号
void  IIC_NAck(void);								  //== IIC不发送ACK信号
void  IIC_Send_Byte(Uint8 TxData);		//== IIC发送一个字节
Uint8 IIC_Read_Byte(Uint8 AckSel);    //== IIC读取一个字节

Uint8 I2C_ReadOneByte(Uint8 addr, Uint8 reg);	              //== IIC读取指定设备指定寄存器数据
void IIC_Write_One_Byte(Uint8 addr, Uint8 reg, Uint8 data); //== IIC向指定设备指定寄存器写入指定数据

/*************************************************************************************************/
/* 功能：初始化I2C引脚        																																 	 */
/* 输入：无                                     																								 */
/* 输出：SCL--PB6                                                                                */
/*       SDA--PB7                                                                                */
/* 描述：无                                     																								 */
/*************************************************************************************************/
void IIC_Init(void)
{			
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	  //== 使能PB端口时钟
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;	//== 选择PB6、PB7
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;      	  //== 推挽输出模式
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     	  //== 速度50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					      	  //== 根据设定参数初始化GPIOB

		GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);						  //== 拉高时钟线与信号线	
}


/*************************************************************************************************/
/* 功能：产生I2C起始信号       																																 	 */
/* 输入：无                                     																								 */
/* 输出：返回1则通信启动成功                                                                     */
/* 描述：无                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Start(void)
{
    IIC_SDA = 1;							//== SDA置高
    if(!READ_SDA) return 0;	  //== 若读取到SDA被拉低，则返回
    IIC_SCL = 1;							//== SCL置高
    delay_us(1);						  //== 延时1us
    IIC_SDA = 0;							//== SDA由高变低，I2C通信开始
    if(READ_SDA)  return 0;		//== 若读取到SDA被拉高，则返回
    delay_us(1);						  //== 延时1us
    IIC_SCL = 0;							//== 钳住I2C总线，准备发送或接收数据 
    return 1;								  //== 返回1，通信正常，可以传输数据
}

/*************************************************************************************************/
/* 功能：产生I2C停止信号       																																 	 */
/* 输入：无                                     																								 */
/* 输入：无                                     																								 */
/* 描述：控制时钟线与信号线以配对通信                                                            */
/*************************************************************************************************/
void IIC_Stop(void)
{
    IIC_SCL = 0;	            //== SCL置低，SDA无效，防止重复产生开始信号
    IIC_SDA = 0;	            //== SDA置低，准备产生停止信号
    delay_us(1);                   
    IIC_SCL = 1; 	            //== SCL置高，SDA有效
    IIC_SDA = 1;	            //== 发送I2C总线结束信号
    delay_us(1);							   	
}

/*************************************************************************************************/
/* 功能：等待从机应答          																																 	 */
/* 输入：无                                     																								 */
/* 输出：1--接收应答成功                                                                         */
/* 			 0--接收应答失败                                                                         */
/* 描述：应答超时则报错                                                                          */
/*************************************************************************************************/
Uint8 IIC_Wait_Ack(void)
{
    Uint8 ErrTime = 0;		    //== 超时时间
    
    IIC_SDA = 1;
    delay_us(1);	   
    IIC_SCL = 1;
    delay_us(1);	 
    while(READ_SDA)
    {
        if(++ErrTime > 50)    //===指定时间内未收到应答信号则返回0
        {   
            IIC_Stop();		    //===停止信号
            return 0;
        }
        delay_us(1);
    }
    IIC_SCL = 0;	   
    return 1;  
} 

/*************************************************************************************************/
/* 功能：产生应答信号          																																 	 */
/* 输入：无                                     																								 */
/* 输出：无                                                                                      */
/* 描述：主机应答从机，表示主机收到数据                                                          */
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
/* 功能：产生应答信号          																																 	 */
/* 输入：无                                     																								 */
/* 输出：无                                                                                      */
/* 描述：第9个时钟脉冲SDA保持高电平，则被定义为非应答信号。                                      */
/*			 主机要么产生Stop信号，要么发出Start信号重新开始。                                       */
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
/* 功能：发送一个字节数据       																																 */
/* 输入：需要发送的字节数据（8bit）                                     												 */
/* 输出：无                                                                                      */
/* 描述：第9个时钟脉冲SDA保持高电平，则被定义为非应答信号。                                      */
/*			 主机要么产生Stop信号，要么发出Start信号重新开始。                                       */
/*************************************************************************************************/
void IIC_Send_Byte(Uint8 TxData)
{                        
    Uint8 i;
  
    IIC_SCL = 0;						  //== 拉低时钟开始数据传输、高位先传输
    for(i = 0; i < 8; i++)
    {              
        IIC_SDA = (TxData & 0x80) >> 7;
        TxData <<= 1; 	  
        delay_us(1);   				//== 必要延时
        IIC_SCL = 1;					//== SCL置高，传输数据
        delay_us(1); 						   
        IIC_SCL = 0;					//== SCL置低，传输完成
        delay_us(1);
    }	 
} 	 

/*************************************************************************************************/
/* 功能：读一个字节数据          																																 */
/* 输入：AckSel--应答选择                                                												 */
/* 输出：读到的数据                                                                              */
/* 描述：无                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Read_Byte(Uint8 AckSel)
{
    Uint8 i, Receive = 0;
  
    for(i = 0; i < 8; i++)    //== 依次读取8次SDA的状态，并存入Receive变量 
    {
        IIC_SCL = 0; 
        delay_us(2);
        IIC_SCL = 1;					//== SCL由低变高，产生一个有效的时钟信号
        Receive <<= 1;               
        if(READ_SDA)  Receive++;  
        delay_us(2);                 
    }					                     
    if(AckSel)  IIC_Ack(); 		//== 发送ACK 
    else        IIC_NAck();		//== 发送nACK  
    return Receive;						//== 返回读到的数据
}

/*************************************************************************************************/
/* 功能：I2C读指定设备、指定寄存器数据																				                   */
/* 输入：addr--地址，reg--要读取的寄存器                                                         */
/* 输出：读取的数据                                                                              */
/* 描述：无                                                                                      */
/*************************************************************************************************/
Uint8 I2C_ReadOneByte(Uint8 addr, Uint8 reg)
{
    Uint8 res = 0;
    
    IIC_Start();							//== 开始
    IIC_Send_Byte(addr);	    //== 发送写命令
    res++;                         
    IIC_Wait_Ack();                
    IIC_Send_Byte(reg);				//== 发送地址 
    res++;  									     
    IIC_Wait_Ack();	               
    //IIC_Stop();							//== 产生一个停止条件	
    IIC_Start();                   
    IIC_Send_Byte(addr + 1); 	//== 进入接收模式			 
    res++;                         
    IIC_Wait_Ack();                
    res = IIC_Read_Byte(0);		//== 接收数据	   
    IIC_Stop();								//== 产生一个停止条件
                                   
    return res;								//== 返回接收值
}

/*************************************************************************************************/
/* 功能：I2C写指定设备、指定寄存器数据																				                   */
/* 输入：addr--地址，reg--要读取的寄存器，data--要写的数据                                       */
/* 输出：无                                                                                      */
/* 描述：无                                                                                      */
/*************************************************************************************************/
void IIC_Write_One_Byte(Uint8 addr, Uint8 reg, Uint8 data)
{
    IIC_Start();
    IIC_Send_Byte(addr);	    //== 发送写命令
    IIC_Wait_Ack();                
    IIC_Send_Byte(reg);       //== 发送地址
    IIC_Wait_Ack();	               
    IIC_Send_Byte(data);           
    IIC_Wait_Ack();                
    IIC_Stop();						    //== 产生一个停止条件
}

/*************************************************************************************************/
/* 功能：I2C向指定设备、指定寄存器读取指定长度的指定数据																				 */
/* 输入：addr--地址，reg--要读取的寄存器，len--字节数，*buf--数据指针                            */
/* 输出：1表示读取成功；0表示读取失败                                                            */
/* 描述：无                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Read(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *buf)
{
    if(!IIC_Start())  return 0;   //== 产生起始信号检测到总线忙则退出
    IIC_Send_Byte(addr);			    //== 写地址
    if(!IIC_Wait_Ack())						//== 等待应答
		{
        IIC_Stop();
        return 0;
    }
    IIC_Send_Byte(reg);						//== 写寄存器
    IIC_Wait_Ack();								//== 等待应答
    IIC_Start();									//== 重新产生开始信号，准备读取数据
    IIC_Send_Byte(addr + 1);	    //== 写地址+1
    IIC_Wait_Ack();								//== 等待应答
    while(len) 										//== 根据长度读取数据
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
/* 功能：I2C向指定设备、指定寄存器写入指定长度的指定数据																				 */
/* 输入：addr--地址，reg--要写入的寄存器，len--字节数，*data--数据指针                           */
/* 输出：1表示写入成功；0表示写入失败                                                            */
/* 描述：无                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Write(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *data)
{
		Uint8 i;
  
    if(!IIC_Start())  return 0;   //== 产生起始信号检测到总线忙则退出
    IIC_Send_Byte(addr);		      //== 写地址
    if(!IIC_Wait_Ack()) 			    //== 等待应答，无应答则退出并产生停止信号
		{			
       IIC_Stop();
       return 0;
    }
    IIC_Send_Byte(reg);					  //== 写寄存器
    if(!IIC_Wait_Ack()) 			    //== 等待应答，无应答则退出并产生停止信号
		{			
       IIC_Stop();
       return 0;
    }
		for(i = 0; i < len; i++) 		  //== 写数据
		{
        IIC_Send_Byte(data[i]);
        if(!IIC_Wait_Ack())			  //== 等待应答
        {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();									  //== 结束
		
    return 1;
}

#else

/*************************************************************************************************/
/* 功能：初始化I2C引脚        																																 	 */
/* 输入：无                                     																								 */
/* 输出：SCL--PB6                                                                                */
/*       SDA--PB7                                                                                */
/* 描述：无                                     																								 */
/*************************************************************************************************/
void IIC_Init(void)
{			
		GPIO_InitTypeDef 	GPIO_InitStructure;
		I2C_InitTypeDef 	I2C_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//== 硬件I2C配置
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_OD;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		I2C_DeInit(I2C1);
		I2C_InitStructure.I2C_Mode								= I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle 					= I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 				= 0x77;					//== 主机地址
		I2C_InitStructure.I2C_Ack 								= I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed 					= I2C_SPEED;				//== 速率100kHz
		I2C_Init(I2C1, &I2C_InitStructure);
		I2C_Cmd(I2C1, ENABLE);	
}

/*************************************************************************************************/
/* 功能：I2C向指定设备、指定寄存器读取指定长度的指定数据																				 */
/* 输入：addr--地址，reg--要读取的寄存器，len--字节数，*buf--数据指针                            */
/* 输出：1表示读取成功；0表示读取失败                                                            */
/* 描述：无                                                                                      */
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
/* 功能：I2C向指定设备、指定寄存器写入指定长度的指定数据																				 */
/* 输入：addr--地址，reg--要写入的寄存器，len--字节数，*data--数据指针                           */
/* 输出：1表示写入成功；0表示写入失败                                                            */
/* 描述：无                                                                                      */
/*************************************************************************************************/
Uint8 IIC_Write(Uint8 addr, Uint8 reg, Uint8 len, Uint8 *data)
{
		Uint8 Index = 0;
		if(len == 0) return 1;
		
		//=== 1、起始条件，等待响应(EV5)
		I2C_GenerateSTART(I2C1, ENABLE);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
		//=== 2、器件地址，等待响应(EV6)
		I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		//=== 3、寄存器地址，等待响应(EV8)
		I2C_SendData(I2C1, reg);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
		//=== 4、连续写数据，等待响应(EV8)
		while(len)
		{
				I2C_SendData(I2C1, data[Index]);
				while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
				Index++;
				len--;
		}
		//=== 5、传输完成、发送停止条件
		I2C_GenerateSTOP(I2C1, ENABLE);
		
		return 1;
}
#endif
