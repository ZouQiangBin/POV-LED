/*************************************************************************************************/
/*文件：定时器配置文件						                                                               */
/*记录：By Zouqb@2021.6.14																																			 */
/*************************************************************************************************/

#include "TIM.h"

/*************************************************************************************************/
/* 功能：定时器TIM1 中断配置初始化     																								 	 				 */
/* 输入：中断频率                          																					 						 */
/* 输出：无			                                                                                 */
/* 描述：定时器TIM1 中断配置初始化                	 			 																			 */
/*************************************************************************************************/
void TIM1_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 				//== 使能定时器TIM1的时钟
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 10000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== 预分频，72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 时钟不分割
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== 重复计数设置
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 						//== 根据以上配置初始化TIM1
                                                                     
		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);								//== 清除中断标志位
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);									//== 使能中断
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== 中断优先级分组2，抢占优先级0~3，次优先级0~3，数字越小优先级越高
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;; 				//== 选择TIM1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LLED_IRQ; 	//== 抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = LLED_IRQ1; 					//== 次优先级等级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== 使能该中断命令
		NVIC_Init(&NVIC_InitStructure);															//== 初始化中断控制器
		                                                                 
		TIM_Cmd(TIM1,ENABLE);																				//== 使能定时器TIM1
}

/*************************************************************************************************/
/* 功能：定时器TIM2 通道1输入捕获中断配置初始化     																						 */
/* 输入：无                          																					 						 			 */
/* 输出：无			                                                                                 */
/* 描述：T法测速               	 			 																			 */
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

		TIM_TimeBaseStructure.TIM_Period = 65535; 									//== 65536us 溢出一次 用以作为测速时间基准 T法测速
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;									//== 预分频，72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 时钟不分割
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== 重复计数设置
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//== 根据以上配置初始化TIM2

		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//TIM_ICPolarity_Rising; 
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM2_ICInitStructure.TIM_ICFilter = 0x0F;										//== 两个连续电平认为捕获
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		//=== 溢出中断与捕获中断 测量输入频率
		TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
		
		//=== 优先级较高
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ALED_RST;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = ALED_RST1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM2, ENABLE);
}

/*************************************************************************************************/
/* 功能：定时器TIM3 中断配置初始化     																								 	 				 */
/* 输入：中断频率                          																					 						 */
/* 输出：无			                                                                                 */
/* 描述：定时器TIM3 中断配置初始化                	 			 																			 */
/*			 PB5---TIM3->CH2*/
/*************************************************************************************************/
void TIM3_Init(Uint16 Frq)
{
//		GPIO_InitTypeDef 				GPIO_InitStructure;
//		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef  			TIM_OCInitStructure;
//		
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== 使能定时器TIM3的时钟
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
//		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== 预分频，72M/72=1M
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 时钟不分割
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== 向上计数模式
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== 重复计数设置
//		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== 根据以上配置初始化TIM3
//    
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//== 选择定时器模式:TIM脉冲宽度调制模式1(向上计数模式下，计数值小于比较值时PWM输出通道输出有效电平)
//																																	//==  							TIM脉冲宽度调制模式2(向上计数模式下，计数值小于比较值时PWM输出通道输出无效电平)
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== 比较输出使能
//		TIM_OCInitStructure.TIM_Pulse = 250;                            //== 设置比较值，计数器计数值达到比较值后进行电平翻转
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== 设置有效电平为高电平	
//		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	
//		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
//		                                                                 
//		TIM_Cmd(TIM3,ENABLE);																				//== 使能定时器TIM3
		
		GPIO_InitTypeDef 				GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  			TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//== 使能定时器TIM3的时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
		 
		TIM_TimeBaseStructure.TIM_Period = 100000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 720-1;									//== 预分频，72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 时钟不分割
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== 重复计数设置
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//== 根据以上配置初始化TIM3
    
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//== 选择定时器模式:TIM脉冲宽度调制模式1(向上计数模式下，计数值小于比较值时PWM输出通道输出有效电平)
																																	//==  							TIM脉冲宽度调制模式2(向上计数模式下，计数值小于比较值时PWM输出通道输出无效电平)
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //== 比较输出使能
		TIM_OCInitStructure.TIM_Pulse = 0;                            //== 设置比较值，计数器计数值达到比较值后进行电平翻转
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    	//== 设置有效电平为高电平	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
		                                                                 
		TIM_Cmd(TIM3,ENABLE);																				//== 使能定时器TIM3

		TIM_SetCompare2(TIM3, (TIM_TimeBaseStructure.TIM_Period >> 1));
}

/*************************************************************************************************/
/* 功能：定时器TIM4 中断配置初始化     																								 	 				 */
/* 输入：中断频率                          																					 						 */
/* 输出：无			                                                                                 */
/* 描述：定时器TIM4 中断配置初始化                	 			 																			 */
/*************************************************************************************************/
void TIM4_Init(Uint16 Frq)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef        NVIC_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 				//== 使能定时器TIM4的时钟
		                                                                 
		TIM_TimeBaseStructure.TIM_Period = 10000L/Frq - 1; 				//== 10KHz / TIM_Period = Frq
		TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;									//== 预分频，72M/72=1M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//== 时钟不分割
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //== 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//== 重复计数设置
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//== 根据以上配置初始化TIM4
                                                                     
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);								//== 清除中断标志位
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);									//== 使能中断
		                                                                 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//== 中断优先级分组2，抢占优先级0~3，次优先级0~3，数字越小优先级越高
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;; 						//== 选择TIM4中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RLED_IRQ; 	//== 抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = RLED_IRQ1; 					//== 次优先级等级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//== 使能该中断命令
		NVIC_Init(&NVIC_InitStructure);															//== 初始化中断控制器
		                                                                 
		TIM_Cmd(TIM4,ENABLE);																				//== 使能定时器TIM4
}
