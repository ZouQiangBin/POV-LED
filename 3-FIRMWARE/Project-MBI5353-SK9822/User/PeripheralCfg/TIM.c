/*************************************************************************************************/
/*???t��o?������?��???????t						                                                               */
/*????��oBy Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "TIM.h"

/*************************************************************************************************/
/* 1|?����o?������?��TIM1 ?D??????3?��??��     																								 	 				 */
/* ��?��?��o?D???��?��                          																					 						 */
/* ��?3?��o?T			                                                                                 */
/* ?����?��o?������?��TIM1 ?D??????3?��??��                	 			 																			 */
/*************************************************************************************************/
void TIM1_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		NVIC_InitTypeDef        NVIC_InitStructure;
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 				//== ��1?��?������?��TIM1��?����?��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//== ��1?��PA???������?��

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		TIM_TimeBaseStructure.TIM_Period = Frq + 2; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 0;									//== ?�衤??��0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM1
 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0; 			
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);

		TIM_CtrlPWMOutputs(TIM1,ENABLE);
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM1, ENABLE);
		TIM_Cmd(TIM1, ENABLE);

		TIM_SetCompare1(TIM1, (TIM_TimeBaseStructure.TIM_Period >> 1));
//		TIM_TimeBaseStructure.TIM_Period = 10000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== ?�衤??�̡�?72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???��??��y����??
//		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM1
		
//		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);								//== ??3y?D??����????
//		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//== ��1?��?D??
//		                                                                 
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??��??��??��?����2��??��??��??��??0~3��?��?��??��??0~3��?��y��???D?��??��??????
//		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;; 				//== ????TIM1?D??
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LLED_IRQ; 	//== ?��??��??��??1
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = LLED_IRQ1; 					//== ��?��??��??�̨�??1
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ��1?��???D???����?
//		NVIC_Init(&NVIC_InitStructure);															//== 3?��??��?D???????��
		                                                                 
//		TIM_Cmd(TIM1,ENABLE);																				//== ��1?��?������?��TIM1
}

/*************************************************************************************************/
/* 1|?����o?������?��TIM2 �����̨�1��?��?2????D??????3?��??��     																						 */
/* ��?��?��o?T                          																					 						 			 */
/* ��?3?��o?T			                                                                                 */
/* ?����?��oT����2a?��               	 			 																			 */
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

		TIM_TimeBaseStructure.TIM_Period = 65535; 									//== 65536us ��?3?��?��? ��?��?����?a2a?������???����? T����2a?��
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?�衤??�̡�?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???��??��y����??
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM2

		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//TIM_ICPolarity_Rising; 
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM2_ICInitStructure.TIM_ICFilter = 0x0F;										//== ��???��?D?��???��??a2???
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		//=== ��?3??D??��?2????D?? 2a��?��?��??��?��
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
		
		//=== ��??��??????
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== �ж����ȼ�����2����ռ���ȼ�0~3�������ȼ�0~3������ԽС���ȼ�Խ��
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ALED_RST;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = ALED_RST1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM2, ENABLE);
}

/*************************************************************************************************/
/* 1|?����o?������?��TIM3 ?D??????3?��??��     																								 	 				 */
/* ��?��?��o?D???��?��                          																					 						 */
/* ��?3?��o?T			                                                                                 */
/* ?����?��o?������?��TIM3 ?D??????3?��??��                	 			 																			 */
/*			 PB5---TIM3->CH2*/
/*************************************************************************************************/
void TIM3_Init(Uint16 Frq)
{
//		GPIO_InitTypeDef 				GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef  			TIM_OCInitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== ��1?��?������?��TIM3��?����?��
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
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== ?�衤??�̡�?72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???��??��y����??
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM3
//    
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//== ?????������?��?�꨺?:TIM??3??��?���̡�???�꨺?1(?����???��y?�꨺???��???��y?��D?��������???�̨���PWM��?3?�����̨���?3?��DD���???)
//																																	//==  							TIM??3??��?���̡�???�꨺?2(?����???��y?�꨺???��???��y?��D?��������???�̨���PWM��?3?�����̨���?3??TD���???)
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== ����??��?3?��1?��
//		TIM_OCInitStructure.TIM_Pulse = 250;                            //== ����??����???�̡�???��y?��??��y?�̡�?��?����???��o��??DD��???��-��a
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== ����??��DD���????a??��???	
//		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	
//		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
//		                                                                 
//		TIM_Cmd(TIM3,ENABLE);																				//== ��1?��?������?��TIM3
		
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== ��1?��?������?��TIM3��?����?��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
		 
		TIM_TimeBaseStructure.TIM_Period = 360000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 200-1;									//== ?�衤??�̡�?360k
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???��??��y����??
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM3
    
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//== ?????������?��?�꨺?:TIM??3??��?���̡�???�꨺?1(?����???��y?�꨺???��???��y?��D?��������???�̨���PWM��?3?�����̨���?3?��DD���???)
																																	//==  							TIM??3??��?���̡�???�꨺?2(?����???��y?�꨺???��???��y?��D?��������???�̨���PWM��?3?�����̨���?3??TD���???)
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== ����??��?3?��1?��
		TIM_OCInitStructure.TIM_Pulse = 0;                            //== ����??����???�̡�???��y?��??��y?�̡�?��?����???��o��??DD��???��-��a
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== ����??��DD���????a??��???	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
		                                                                 
		TIM_Cmd(TIM3,ENABLE);																				//== ��1?��?������?��TIM3

		TIM_SetCompare2(TIM3, TIM_TimeBaseStructure.TIM_Period >> 1);
}

/*************************************************************************************************/
/* 1|?����o?������?��TIM4 ?D??????3?��??��     																								 	 				 */
/* ��?��?��o?D???��?��                          																					 						 */
/* ��?3?��o?T			                                                                                 */
/* ?����?��o?������?��TIM4 ?D??????3?��??��                	 			 																			 */
/*************************************************************************************************/
void TIM4_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//== ��1?��?������?��TIM4��?����?��
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 1000000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?�衤??�̡�?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???��??��y����??
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM4
                                                                     
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);								//== ??3y?D??����????
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);									//== ��1?��?D??
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??��??��??��?����2��??��??��??��??0~3��?��?��??��??0~3��?��y��???D?��??��??????
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ????TIM4?D??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ?��??��??��??1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== ��?��??��??�̨�??2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ��1?��???D???����?
		NVIC_Init(&NVIC_InitStructure);															//== 3?��??��?D???????��
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== ��1?��?������?��TIM4
}

/*************************************************************************************************/
/* 1|?����o?������?��TIM5 ?D??????3?��??��     																								 	 				 */
/* ��?��?��o?D???��?��                          																					 						 */
/* ��?3?��o?T			                                                                                 */
/* ?����?��o?������?��TIM5 ?D??????3?��??��                	 			 																			 */
/*************************************************************************************************/
void TIM5_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 				//== ��1?��?������?��TIM5��?����?��
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 1000000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?�衤??�̡�?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ����?��2?��???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?����???��y?�꨺?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???��??��y����??
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 						//== ?��?Y��?��?????3?��??��TIM4
                                                                     
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);								//== ??3y?D??����????
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);									//== ��1?��?D??
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??��??��??��?����2��??��??��??��??0~3��?��?��??��??0~3��?��y��???D?��??��??????
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ????TIM4?D??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ?��??��??��??1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== ��?��??��??�̨�??2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ��1?��???D???����?
		NVIC_Init(&NVIC_InitStructure);															//== 3?��??��?D???????��
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== ��1?��?������?��TIM4
}
