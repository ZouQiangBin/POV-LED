#include "GPIO.h"


/*************************************************************************************************/
/* ���ܣ�����1�������ã�PA9--TX;PA10--RX��      																								 */
/* ���룺bound--������                           																								 */
/* �������			                                                                                 */
/* ����������USART1                     	 */
/*************************************************************************************************/
void GPIO_User_Init(void)
{
//		GPIO_InitTypeDef GPIO_InitStructure;

//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);						//== ʹ��PA�˿ں�SPI1ʱ��
//																																										 
//		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;											//== PA5 BUZZ
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;  							//== �������
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//== �ٶ�50M
//		GPIO_Init(GPIOB, &GPIO_InitStructure);													//== ��ʼ��PA�˿�
//		GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

