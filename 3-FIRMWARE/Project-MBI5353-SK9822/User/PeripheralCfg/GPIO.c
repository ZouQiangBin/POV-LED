#include "GPIO.h"


/*************************************************************************************************/
/* 功能：串口1功能配置（PA9--TX;PA10--RX）      																								 */
/* 输入：bound--波特率                           																								 */
/* 输出：无			                                                                                 */
/* 描述：配置USART1                     	 */
/*************************************************************************************************/
void GPIO_User_Init(void)
{
//		GPIO_InitTypeDef GPIO_InitStructure;

//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//== 使能PA端口和SPI1时钟
//																																										 
//		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;											//== PA5 BUZZ
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;  							//== 推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//== 速度50M
//		GPIO_Init(GPIOB, &GPIO_InitStructure);													//== 初始化PA端口
//		GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

