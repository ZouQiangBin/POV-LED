#include <LED.h>

/*******************************************************
*函数功能：小四轴LED控制引脚配置
*描		 述：定时器TIM4四路PWM输出引脚	PB6 PB7 PB8 PB9
*入口参数：无
*返 回 值：无
*******************************************************/
void LED_Config(uint16_t Frequence,uint16_t Duty_Cycle)
{	 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);												//=====使能能挂载在APB1的普通定时器TIM4的时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  											//=====使能TIM4四个PWM通道映射的GPIOB外设时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//=====TIM4_CH1 CH2 CH3 CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  														//=====引脚模式设置为复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//=====速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);																			//=====GPIOB初始化上述设置
	
	TIM_TimeBaseStructure.TIM_Period = Duty_Cycle-1; 														//=====计数重载值
	TIM_TimeBaseStructure.TIM_Prescaler = 72000000/(Duty_Cycle*Frequence)-1; 		//=====72M时钟分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 																//=====设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 								//=====TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 														//=====根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//=====选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //=====比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //=====设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //=====输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  										//=====根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 						//=====TIM4的PWM输出通道预装载使能	 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	 	
	
	TIM_Cmd(TIM4, ENABLE);  																			//=====根据以上设置使能TIM4 
}
