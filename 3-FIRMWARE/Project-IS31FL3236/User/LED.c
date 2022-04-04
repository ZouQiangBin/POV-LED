#include <LED.h>

/*******************************************************
*�������ܣ�С����LED������������
*��		 ������ʱ��TIM4��·PWM�������	PB6 PB7 PB8 PB9
*��ڲ�������
*�� �� ֵ����
*******************************************************/
void LED_Config(uint16_t Frequence,uint16_t Duty_Cycle)
{	 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);												//=====ʹ���ܹ�����APB1����ͨ��ʱ��TIM4��ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  											//=====ʹ��TIM4�ĸ�PWMͨ��ӳ���GPIOB����ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//=====TIM4_CH1 CH2 CH3 CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//=====����ģʽ����Ϊ�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//=====�ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);																			//=====GPIOB��ʼ����������
	
	TIM_TimeBaseStructure.TIM_Period = Duty_Cycle-1; 														//=====��������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72000000/(Duty_Cycle*Frequence)-1; 		//=====72Mʱ�ӷ�Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 																//=====����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 								//=====TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 														//=====����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//=====ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //=====�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //=====���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //=====�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  										//=====����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 						//=====TIM4��PWM���ͨ��Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	 	
	
	TIM_Cmd(TIM4, ENABLE);  																			//=====������������ʹ��TIM4 
}
