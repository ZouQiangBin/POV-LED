#include <Motor_PWM.h>

/*******************************************************
*�������ܣ�С������������������
*��		 ������ʱ��TIM2��·PWM�������	PA0 PA1 PA2 PA3
*��ڲ�������
*�� �� ֵ����
*******************************************************/
void Motor_PWMConfig(uint16_t Frequence,uint16_t Duty_Cycle)
{	 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);												//=====ʹ���ܹ�����APB1����ͨ��ʱ��TIM2��ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  											//=====ʹ��TIM2�ĸ�PWMͨ��ӳ���GPIOA����ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//=====TIM4_CH1 CH2 CH3 CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//=====����ģʽ����Ϊ�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//=====�ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);																			//=====GPIOB��ʼ����������
	
	TIM_TimeBaseStructure.TIM_Period = Duty_Cycle-1; 									  				//=====���ü�������ֵ(ռ�ձ����ֵDuty_Cycle)��PWMƵ��Frequence
	TIM_TimeBaseStructure.TIM_Prescaler = 72000000/(Duty_Cycle*Frequence)-1; 		//=====����72Mʱ�ӷ�Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 																//=====����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 								//=====TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 														//=====����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//=====ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1(���ϼ���ģʽ�£�����ֵС�ڱȽ�ֵʱPWM���ͨ�������Ч��ƽ)
																																//===== 							TIM�����ȵ���ģʽ2(���ϼ���ģʽ�£�����ֵС�ڱȽ�ֵʱPWM���ͨ�������Ч��ƽ)
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //=====�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //=====���ñȽ�ֵ������������ֵ�ﵽ�Ƚ�ֵ����е�ƽ��ת
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//=====������Ч��ƽΪ�ߵ�ƽ
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  										//=====����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 						//=====TIM2��PWM���ͨ��Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);	 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);	 	
	
	TIM_Cmd(TIM2, ENABLE);  																			//=====������������ʹ��TIM2 
}
