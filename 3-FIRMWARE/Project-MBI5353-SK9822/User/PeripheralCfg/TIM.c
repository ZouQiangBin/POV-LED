/*************************************************************************************************/
/*???t㏒o?“那㊣?‾???????t						                                                               */
/*????㏒oBy Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "TIM.h"

/*************************************************************************************************/
/* 1|?邦㏒o?“那㊣?‾TIM1 ?D??????3?那??‘     																								 	 				 */
/* 那?豕?㏒o?D???米?那                          																					 						 */
/* 那?3?㏒o?T			                                                                                 */
/* ?豕那?㏒o?“那㊣?‾TIM1 ?D??????3?那??‘                	 			 																			 */
/*************************************************************************************************/
void TIM1_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		NVIC_InitTypeDef        NVIC_InitStructure;
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 				//== 那1?邦?“那㊣?‾TIM1米?那㊣?車
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				//== 那1?邦PA???迆那㊣?車

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		TIM_TimeBaseStructure.TIM_Period = Frq + 2; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 0;									//== ?∟﹞??米0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM1
 
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
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== ?∟﹞??米㏒?72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???∩??那y谷豕??
//		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM1
		
//		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);								//== ??3y?D??㊣那????
//		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//== 那1?邦?D??
//		                                                                 
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??車??豕??﹞?℅谷2㏒??角??車??豕??0~3㏒?∩?車??豕??0~3㏒?那y℅???D?車??豕??????
//		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;; 				//== ????TIM1?D??
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LLED_IRQ; 	//== ?角??車??豕??1
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = LLED_IRQ1; 					//== ∩?車??豕??米豕??1
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== 那1?邦???D???邦芍?
//		NVIC_Init(&NVIC_InitStructure);															//== 3?那??‘?D???????‾
		                                                                 
//		TIM_Cmd(TIM1,ENABLE);																				//== 那1?邦?“那㊣?‾TIM1
}

/*************************************************************************************************/
/* 1|?邦㏒o?“那㊣?‾TIM2 赤“米角1那?豕?2????D??????3?那??‘     																						 */
/* 那?豕?㏒o?T                          																					 						 			 */
/* 那?3?㏒o?T			                                                                                 */
/* ?豕那?㏒oT﹞“2a?迄               	 			 																			 */
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

		TIM_TimeBaseStructure.TIM_Period = 65535; 									//== 65536us 辰?3?辰?∩? 車?辰?℅‾?a2a?迄那㊣???迄℅? T﹞“2a?迄
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?∟﹞??米㏒?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???∩??那y谷豕??
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM2

		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//TIM_ICPolarity_Rising; 
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM2_ICInitStructure.TIM_ICFilter = 0x0F;										//== 芍???芍?D?米???豕??a2???
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		//=== 辰?3??D??車?2????D?? 2a芍?那?豕??米?那
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
		
		//=== 車??豕??????
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== 笢剿蚥珂撰煦郪2ㄛШ梩蚥珂撰0~3ㄛ棒蚥珂撰0~3ㄛ杅趼埣苤蚥珂撰埣詢
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ALED_RST;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = ALED_RST1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM2, ENABLE);
}

/*************************************************************************************************/
/* 1|?邦㏒o?“那㊣?‾TIM3 ?D??????3?那??‘     																								 	 				 */
/* 那?豕?㏒o?D???米?那                          																					 						 */
/* 那?3?㏒o?T			                                                                                 */
/* ?豕那?㏒o?“那㊣?‾TIM3 ?D??????3?那??‘                	 			 																			 */
/*			 PB5---TIM3->CH2*/
/*************************************************************************************************/
void TIM3_Init(Uint16 Frq)
{
//		GPIO_InitTypeDef 				GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef  			TIM_OCInitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== 那1?邦?“那㊣?‾TIM3米?那㊣?車
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
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== ?∟﹞??米㏒?72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???∩??那y谷豕??
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM3
//    
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//== ?????“那㊣?‾?㏒那?:TIM??3??赤?豕米‾???㏒那?1(?辰谷???那y?㏒那???㏒???那y?米D?車迆㊣豕???米那㊣PWM那?3?赤“米角那?3?車DD∫米???)
//																																	//==  							TIM??3??赤?豕米‾???㏒那?2(?辰谷???那y?㏒那???㏒???那y?米D?車迆㊣豕???米那㊣PWM那?3?赤“米角那?3??TD∫米???)
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== ㊣豕??那?3?那1?邦
//		TIM_OCInitStructure.TIM_Pulse = 250;                            //== 谷豕??㊣豕???米㏒???那y?‾??那y?米∩?米?㊣豕???米o車??DD米???﹞-℅a
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== 谷豕??車DD∫米????a??米???	
//		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	
//		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
//		                                                                 
//		TIM_Cmd(TIM3,ENABLE);																				//== 那1?邦?“那㊣?‾TIM3
		
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== 那1?邦?“那㊣?‾TIM3米?那㊣?車
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
		 
		TIM_TimeBaseStructure.TIM_Period = 360000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 200-1;									//== ?∟﹞??米㏒?360k
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???∩??那y谷豕??
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM3
    
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//== ?????“那㊣?‾?㏒那?:TIM??3??赤?豕米‾???㏒那?1(?辰谷???那y?㏒那???㏒???那y?米D?車迆㊣豕???米那㊣PWM那?3?赤“米角那?3?車DD∫米???)
																																	//==  							TIM??3??赤?豕米‾???㏒那?2(?辰谷???那y?㏒那???㏒???那y?米D?車迆㊣豕???米那㊣PWM那?3?赤“米角那?3??TD∫米???)
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== ㊣豕??那?3?那1?邦
		TIM_OCInitStructure.TIM_Pulse = 0;                            //== 谷豕??㊣豕???米㏒???那y?‾??那y?米∩?米?㊣豕???米o車??DD米???﹞-℅a
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== 谷豕??車DD∫米????a??米???	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
		                                                                 
		TIM_Cmd(TIM3,ENABLE);																				//== 那1?邦?“那㊣?‾TIM3

		TIM_SetCompare2(TIM3, TIM_TimeBaseStructure.TIM_Period >> 1);
}

/*************************************************************************************************/
/* 1|?邦㏒o?“那㊣?‾TIM4 ?D??????3?那??‘     																								 	 				 */
/* 那?豕?㏒o?D???米?那                          																					 						 */
/* 那?3?㏒o?T			                                                                                 */
/* ?豕那?㏒o?“那㊣?‾TIM4 ?D??????3?那??‘                	 			 																			 */
/*************************************************************************************************/
void TIM4_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//== 那1?邦?“那㊣?‾TIM4米?那㊣?車
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 1000000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?∟﹞??米㏒?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???∩??那y谷豕??
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM4
                                                                     
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);								//== ??3y?D??㊣那????
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);									//== 那1?邦?D??
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??車??豕??﹞?℅谷2㏒??角??車??豕??0~3㏒?∩?車??豕??0~3㏒?那y℅???D?車??豕??????
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ????TIM4?D??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ?角??車??豕??1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== ∩?車??豕??米豕??2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== 那1?邦???D???邦芍?
		NVIC_Init(&NVIC_InitStructure);															//== 3?那??‘?D???????‾
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== 那1?邦?“那㊣?‾TIM4
}

/*************************************************************************************************/
/* 1|?邦㏒o?“那㊣?‾TIM5 ?D??????3?那??‘     																								 	 				 */
/* 那?豕?㏒o?D???米?那                          																					 						 */
/* 那?3?㏒o?T			                                                                                 */
/* ?豕那?㏒o?“那㊣?‾TIM5 ?D??????3?那??‘                	 			 																			 */
/*************************************************************************************************/
void TIM5_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 				//== 那1?邦?“那㊣?‾TIM5米?那㊣?車
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 1000000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== ?∟﹞??米㏒?72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 那㊣?車2?﹞???
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== ?辰谷???那y?㏒那?
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== ???∩??那y谷豕??
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 						//== ?迄?Y辰?谷?????3?那??‘TIM4
                                                                     
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);								//== ??3y?D??㊣那????
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);									//== 那1?邦?D??
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== ?D??車??豕??﹞?℅谷2㏒??角??車??豕??0~3㏒?∩?車??豕??0~3㏒?那y℅???D?車??豕??????
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== ????TIM4?D??
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== ?角??車??豕??1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== ∩?車??豕??米豕??2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== 那1?邦???D???邦芍?
		NVIC_Init(&NVIC_InitStructure);															//== 3?那??‘?D???????‾
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== 那1?邦?“那㊣?‾TIM4
}
