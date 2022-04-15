/*************************************************************************************************/
/*???t￡o?¨ê±?÷???????t						                                                               */
/*????￡oBy Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "TIM.h"

/*************************************************************************************************/
/* 1|?ü￡o?¨ê±?÷TIM1 ?D??????3?ê??ˉ     																								 	 				 */
/* ê?è?￡o?D???μ?ê                          																					 						 */
/* ê?3?￡o?T			                                                                                 */
/* ?èê?￡o?¨ê±?÷TIM1 ?D??????3?ê??ˉ                	 			 																			 */
/*************************************************************************************************/
void TIM1_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		NVIC_InitTypeDef        NVIC_InitStructure;
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 				//== ê1?ü?¨ê±?÷TIM1μ?ê±?ó
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//== ê1?üPA???úê±?ó

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		TIM_TimeBaseStructure.TIM_Period = Frq + 2; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 0;									//== ?¤·??μ0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM1
 
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
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== ?¤·??μ￡?72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???′??êyéè??
//		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM1
		
//		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);								//== ??3y?D??±ê????
//		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//== ê1?ü?D??
//		                                                                 
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??ó??è??·?×é2￡??à??ó??è??0~3￡?′?ó??è??0~3￡?êy×???D?ó??è??????
//		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;; 				//== ????TIM1?D??
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LLED_IRQ; 	//== ?à??ó??è??1
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = LLED_IRQ1; 					//== ′?ó??è??μè??1
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ê1?ü???D???üá?
//		NVIC_Init(&NVIC_InitStructure);															//== 3?ê??ˉ?D???????÷
		                                                                 
//		TIM_Cmd(TIM1,ENABLE);																				//== ê1?ü?¨ê±?÷TIM1
}

/*************************************************************************************************/
/* 1|?ü￡o?¨ê±?÷TIM2 í¨μà1ê?è?2????D??????3?ê??ˉ     																						 */
/* ê?è?￡o?T                          																					 						 			 */
/* ê?3?￡o?T			                                                                                 */
/* ?èê?￡oT·¨2a?ù               	 			 																			 */
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

		TIM_TimeBaseStructure.TIM_Period = 65535; 									//== 65536us ò?3?ò?′? ó?ò?×÷?a2a?ùê±???ù×? T·¨2a?ù
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?¤·??μ￡?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???′??êyéè??
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM2

		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//TIM_ICPolarity_Rising; 
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM2_ICInitStructure.TIM_ICFilter = 0x0F;										//== á???á?D?μ???è??a2???
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		//=== ò?3??D??ó?2????D?? 2aá?ê?è??μ?ê
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
		
		//=== ó??è??????
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== 中断优先级分组2，抢占优先级0~3，次优先级0~3，数字越小优先级越高
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ALED_RST;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = ALED_RST1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM2, ENABLE);
}

/*************************************************************************************************/
/* 1|?ü￡o?¨ê±?÷TIM3 ?D??????3?ê??ˉ     																								 	 				 */
/* ê?è?￡o?D???μ?ê                          																					 						 */
/* ê?3?￡o?T			                                                                                 */
/* ?èê?￡o?¨ê±?÷TIM3 ?D??????3?ê??ˉ                	 			 																			 */
/*			 PB5---TIM3->CH2*/
/*************************************************************************************************/
void TIM3_Init(Uint16 Frq)
{
//		GPIO_InitTypeDef 				GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef  			TIM_OCInitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== ê1?ü?¨ê±?÷TIM3μ?ê±?ó
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
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== ?¤·??μ￡?72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???′??êyéè??
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM3
//    
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//== ?????¨ê±?÷?￡ê?:TIM??3??í?èμ÷???￡ê?1(?òé???êy?￡ê???￡???êy?μD?óú±è???μê±PWMê?3?í¨μàê?3?óDD§μ???)
//																																	//==  							TIM??3??í?èμ÷???￡ê?2(?òé???êy?￡ê???￡???êy?μD?óú±è???μê±PWMê?3?í¨μàê?3??TD§μ???)
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== ±è??ê?3?ê1?ü
//		TIM_OCInitStructure.TIM_Pulse = 250;                            //== éè??±è???μ￡???êy?÷??êy?μ′?μ?±è???μoó??DDμ???·-×a
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== éè??óDD§μ????a??μ???	
//		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	
//		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
//		                                                                 
//		TIM_Cmd(TIM3,ENABLE);																				//== ê1?ü?¨ê±?÷TIM3
		
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== ê1?ü?¨ê±?÷TIM3μ?ê±?ó
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
		 
		TIM_TimeBaseStructure.TIM_Period = 360000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 200-1;									//== ?¤·??μ￡?360k
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???′??êyéè??
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM3
    
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//== ?????¨ê±?÷?￡ê?:TIM??3??í?èμ÷???￡ê?1(?òé???êy?￡ê???￡???êy?μD?óú±è???μê±PWMê?3?í¨μàê?3?óDD§μ???)
																																	//==  							TIM??3??í?èμ÷???￡ê?2(?òé???êy?￡ê???￡???êy?μD?óú±è???μê±PWMê?3?í¨μàê?3??TD§μ???)
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== ±è??ê?3?ê1?ü
		TIM_OCInitStructure.TIM_Pulse = 0;                            //== éè??±è???μ￡???êy?÷??êy?μ′?μ?±è???μoó??DDμ???·-×a
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== éè??óDD§μ????a??μ???	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
		                                                                 
		TIM_Cmd(TIM3,ENABLE);																				//== ê1?ü?¨ê±?÷TIM3

		TIM_SetCompare2(TIM3, TIM_TimeBaseStructure.TIM_Period >> 1);
}

/*************************************************************************************************/
/* 1|?ü￡o?¨ê±?÷TIM4 ?D??????3?ê??ˉ     																								 	 				 */
/* ê?è?￡o?D???μ?ê                          																					 						 */
/* ê?3?￡o?T			                                                                                 */
/* ?èê?￡o?¨ê±?÷TIM4 ?D??????3?ê??ˉ                	 			 																			 */
/*************************************************************************************************/
void TIM4_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//== ê1?ü?¨ê±?÷TIM4μ?ê±?ó
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 1000000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?¤·??μ￡?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???′??êyéè??
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM4
                                                                     
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);								//== ??3y?D??±ê????
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);									//== ê1?ü?D??
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??ó??è??·?×é2￡??à??ó??è??0~3￡?′?ó??è??0~3￡?êy×???D?ó??è??????
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ????TIM4?D??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ?à??ó??è??1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== ′?ó??è??μè??2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ê1?ü???D???üá?
		NVIC_Init(&NVIC_InitStructure);															//== 3?ê??ˉ?D???????÷
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== ê1?ü?¨ê±?÷TIM4
}

/*************************************************************************************************/
/* 1|?ü￡o?¨ê±?÷TIM5 ?D??????3?ê??ˉ     																								 	 				 */
/* ê?è?￡o?D???μ?ê                          																					 						 */
/* ê?3?￡o?T			                                                                                 */
/* ?èê?￡o?¨ê±?÷TIM5 ?D??????3?ê??ˉ                	 			 																			 */
/*************************************************************************************************/
void TIM5_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 				//== ê1?ü?¨ê±?÷TIM5μ?ê±?ó
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 1000000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?¤·??μ￡?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== ê±?ó2?·???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?òé???êy?￡ê?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???′??êyéè??
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 						//== ?ù?Yò?é?????3?ê??ˉTIM4
                                                                     
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);								//== ??3y?D??±ê????
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);									//== ê1?ü?D??
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??ó??è??·?×é2￡??à??ó??è??0~3￡?′?ó??è??0~3￡?êy×???D?ó??è??????
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ????TIM4?D??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ?à??ó??è??1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== ′?ó??è??μè??2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== ê1?ü???D???üá?
		NVIC_Init(&NVIC_InitStructure);															//== 3?ê??ˉ?D???????÷
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== ê1?ü?¨ê±?÷TIM4
}
