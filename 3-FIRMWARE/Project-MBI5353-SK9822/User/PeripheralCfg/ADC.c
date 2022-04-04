/*************************************************************************************************/
/*文件：模拟量监测C文件						                                                               */
/*记录：By Zouqb@2020.12.1																																			 */
/*************************************************************************************************/
#include "ADC.h"

Uint16		AdcVal[16] = {0};				//=== 16个通道、自由分配

/*************************************************************************************************/
/* 功能：ADC工作配置、DMA直接传输	       																											 */
/* 输入：无                                     																								 */
/* 输出：无			                                                                                 */
/* 描述：8通道采集VDD，对应引脚为PB0                             */
/*			 9通道采集+5V，对应引脚为PB1                             */
/*************************************************************************************************/
void  Adc_Init(void)
{    
		GPIO_InitTypeDef GPIO_InitStructure;
		DMA_InitTypeDef	 DMA_InitStructure;
		ADC_InitTypeDef  ADC_InitStructure;  
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1 , ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;			
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);				
	
		//=== 配置DMA1：通道1->ADC1					
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&(ADC1->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)AdcVal;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 16;																				//== 数组16个元素
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							//== 外设地址不递增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;												//== 内存地址自动递增(这里指的是数组元素地址)
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//== 半字传输16位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;						//== 半字传输16位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;																//== 循环缓存
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);																	//== 根据以上设置初始化通道1
		DMA_Cmd(DMA1_Channel1, ENABLE);																								//== 使能DMA1通道1


		//===	设置ADC分频因子6 72M/6=12M,ADC最大时间不能超过14M
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
		//=== 配置ADC1工作状态：独立模式、单次扫描、软件触发、右对齐
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//== 这里采用手动转换;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 16;
		ADC_Init(ADC1,&ADC_InitStructure);
		
		//=== PB0为通道8、PB1为通道9、采样时间 * 通道数 = (12.5+239.5) / 12 * 16 = 336us、可能比较耗电
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,2, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,3, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,4, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,5, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,6, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,7, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,8, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,9, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,10,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,11,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,12,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,13,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,14,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,15,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,16,ADC_SampleTime_239Cycles5);
		
		//=== 使能ADC1
		ADC_DMACmd(ADC1,ENABLE);
		ADC_Cmd(ADC1,ENABLE);

		//=== ADC1复位校准开始直到校准完成
		ADC_ResetCalibration(ADC1);
		while(ADC_GetResetCalibrationStatus(ADC1));
		
		//=== ADC1测量校准开始直到校准完成
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1));
		
		//=== 校准完成、直接开始ADC转换
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}		

/*************************************************************************************************/
/* 功能：ACD初始化电池电压检测       																											 			 */
/* 输入：无                                     																								 */
/* 输出：无			                                                                                 */
/* 描述：8通道采集VDD，对应引脚为PB0                             */
/*			 9通道采集+5V，对应引脚为PB1                             */
/*************************************************************************************************/
void Get_AdcValue(Uint16 *p)   
{
		static Uint8 AdcErRAxisCntY = 0;
	
		//=== 查看DMA是否传输完成，完成后清除该标志，未完成则计数
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET)
		{
				AdcErRAxisCntY = 0;
				*(p + 0) = (AdcVal[0] + AdcVal[1] + AdcVal[2]   + AdcVal[3]   + AdcVal[4]   + AdcVal[5]   + AdcVal[6]   + AdcVal[7]) >> 3;
				*(p + 1) = (AdcVal[8] + AdcVal[9] + AdcVal[10]  + AdcVal[11]  + AdcVal[12]  + AdcVal[13]  + AdcVal[14]  + AdcVal[15])>> 3;
			
				//== 清除完成标志
				DMA_ClearFlag(DMA1_FLAG_TC1);
			
				//== 重新启动一次传输
				ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		}
		else
		{
				AdcErRAxisCntY++;
		}
	
		if(AdcErRAxisCntY > 3)
		{
				//== ADC转换故障
		}
}

