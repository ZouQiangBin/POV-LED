/*************************************************************************************************/
/*�ļ�����ʱ�������ļ�						                                                               */
/*��¼��By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "TIM.h"

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM1 �ж����ó�ʼ��     																								 	 				 */
/* ���룺�ж�Ƶ��                          																					 						 */
/* �������			                                                                                 */
/* ��������ʱ��TIM1 �ж����ó�ʼ��                	 			 																			 */
/*************************************************************************************************/
void TIM1_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 				//== ʹ�ܶ�ʱ��TIM1��ʱ��
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 10000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== Ԥ��Ƶ��72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ʱ�Ӳ��ָ�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== �ظ���������
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== �����������ó�ʼ��TIM1
                                                                     
		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);								//== ����жϱ�־λ
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//== ʹ���ж�
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== �ж����ȼ�����2����ռ���ȼ�0~3�������ȼ�0~3������ԽС���ȼ�Խ��
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;; 				//== ѡ��TIM1�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LLED_IRQ; 	//== ��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = LLED_IRQ1; 					//== �����ȼ��ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ʹ�ܸ��ж�����
		NVIC_Init(&NVIC_InitStructure);															//== ��ʼ���жϿ�����
		                                                                 
		TIM_Cmd(TIM1,ENABLE);																				//== ʹ�ܶ�ʱ��TIM1
}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM2 ͨ��1���벶���ж����ó�ʼ��     																						 */
/* ���룺��                          																					 						 			 */
/* �������			                                                                                 */
/* ������T������               	 			 																			 */
/*************************************************************************************************/
void TIM2_Init(void)
{
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef 			TIM2_ICInitStructure;
		NVIC_InitTypeDef 				NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		TIM_TimeBaseStructure.TIM_Period = 65535; 									//== 65536us ���һ�� ������Ϊ����ʱ���׼ T������
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== Ԥ��Ƶ��72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ʱ�Ӳ��ָ�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== �ظ���������
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//== �����������ó�ʼ��TIM2

		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//TIM_ICPolarity_Rising; 
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM2_ICInitStructure.TIM_ICFilter = 0x0F;										//== ����������ƽ��Ϊ����
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		//=== ����ж��벶���ж� ��������Ƶ��
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
		
		//=== ���ȼ��ϸ�
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ALED_RST;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = ALED_RST1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM2, ENABLE);
}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM3 �ж����ó�ʼ��     																								 	 				 */
/* ���룺�ж�Ƶ��                          																					 						 */
/* �������			                                                                                 */
/* ��������ʱ��TIM3 �ж����ó�ʼ��                	 			 																			 */
/*			 PB5---TIM3->CH2*/
/*************************************************************************************************/
void TIM3_Init(Uint16 Frq)
{
//		GPIO_InitTypeDef 				GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef  			TIM_OCInitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== ʹ�ܶ�ʱ��TIM3��ʱ��
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//		
//		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		 
//		TIM_TimeBaseStructure.TIM_Period = 10000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== Ԥ��Ƶ��72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ʱ�Ӳ��ָ�
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ���ϼ���ģʽ
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== �ظ���������
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== �����������ó�ʼ��TIM3
//    
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//== ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1(���ϼ���ģʽ�£�����ֵС�ڱȽ�ֵʱPWM���ͨ�������Ч��ƽ)
//																																	//==  							TIM�����ȵ���ģʽ2(���ϼ���ģʽ�£�����ֵС�ڱȽ�ֵʱPWM���ͨ�������Ч��ƽ)
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== �Ƚ����ʹ��
//		TIM_OCInitStructure.TIM_Pulse = 250;                            //== ���ñȽ�ֵ������������ֵ�ﵽ�Ƚ�ֵ����е�ƽ��ת
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== ������Ч��ƽΪ�ߵ�ƽ	
//		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	
//		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
//		                                                                 
//		TIM_Cmd(TIM3,ENABLE);																				//== ʹ�ܶ�ʱ��TIM3
		
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== ʹ�ܶ�ʱ��TIM3��ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
		 
		TIM_TimeBaseStructure.TIM_Period = 100000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 720-1;									//== Ԥ��Ƶ��72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ʱ�Ӳ��ָ�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== �ظ���������
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== �����������ó�ʼ��TIM3
    
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//== ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1(���ϼ���ģʽ�£�����ֵС�ڱȽ�ֵʱPWM���ͨ�������Ч��ƽ)
																																	//==  							TIM�����ȵ���ģʽ2(���ϼ���ģʽ�£�����ֵС�ڱȽ�ֵʱPWM���ͨ�������Ч��ƽ)
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== �Ƚ����ʹ��
		TIM_OCInitStructure.TIM_Pulse = 0;                            //== ���ñȽ�ֵ������������ֵ�ﵽ�Ƚ�ֵ����е�ƽ��ת
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== ������Ч��ƽΪ�ߵ�ƽ	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
		                                                                 
		TIM_Cmd(TIM3,ENABLE);																				//== ʹ�ܶ�ʱ��TIM3

		TIM_SetCompare2(TIM3, (TIM_TimeBaseStructure.TIM_Period >> 1));
}

/*************************************************************************************************/
/* ���ܣ���ʱ��TIM4 �ж����ó�ʼ��     																								 	 				 */
/* ���룺�ж�Ƶ��                          																					 						 */
/* �������			                                                                                 */
/* ��������ʱ��TIM4 �ж����ó�ʼ��                	 			 																			 */
/*************************************************************************************************/
void TIM4_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//== ʹ�ܶ�ʱ��TIM4��ʱ��
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 10000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== Ԥ��Ƶ��72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ʱ�Ӳ��ָ�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== �ظ���������
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//== �����������ó�ʼ��TIM4
                                                                     
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);								//== ����жϱ�־λ
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);									//== ʹ���ж�
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== �ж����ȼ�����2����ռ���ȼ�0~3�������ȼ�0~3������ԽС���ȼ�Խ��
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ѡ��TIM4�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== �����ȼ��ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ʹ�ܸ��ж�����
		NVIC_Init(&NVIC_InitStructure);															//== ��ʼ���жϿ�����
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== ʹ�ܶ�ʱ��TIM4
}
