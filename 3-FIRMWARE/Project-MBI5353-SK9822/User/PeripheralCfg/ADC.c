/*************************************************************************************************/
/*�ļ���ģ�������C�ļ�						                                                               */
/*��¼��By Zouqb@2020.12.1																																			 */
/*************************************************************************************************/
#include "ADC.h"

Uint16		AdcVal[16] = {0};				//=== 16��ͨ�������ɷ���

/*************************************************************************************************/
/* ���ܣ�ADC�������á�DMAֱ�Ӵ���	       																											 */
/* ���룺��                                     																								 */
/* �������			                                                                                 */
/* ������8ͨ���ɼ�VDD����Ӧ����ΪPB0                             */
/*			 9ͨ���ɼ�+5V����Ӧ����ΪPB1                             */
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
	
		//=== ����DMA1��ͨ��1->ADC1					
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (Uint32)&(ADC1->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (Uint32)AdcVal;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 16;																				//== ����16��Ԫ��
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							//== �����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;												//== �ڴ��ַ�Զ�����(����ָ��������Ԫ�ص�ַ)
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//== ���ִ���16λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;						//== ���ִ���16λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;																//== ѭ������
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);																	//== �����������ó�ʼ��ͨ��1
		DMA_Cmd(DMA1_Channel1, ENABLE);																								//== ʹ��DMA1ͨ��1


		//===	����ADC��Ƶ����6 72M/6=12M,ADC���ʱ�䲻�ܳ���14M
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
		//=== ����ADC1����״̬������ģʽ������ɨ�衢����������Ҷ���
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//== ��������ֶ�ת��;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 16;
		ADC_Init(ADC1,&ADC_InitStructure);
		
		//=== PB0Ϊͨ��8��PB1Ϊͨ��9������ʱ�� * ͨ���� = (12.5+239.5) / 12 * 16 = 336us�����ܱȽϺĵ�
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
		
		//=== ʹ��ADC1
		ADC_DMACmd(ADC1,ENABLE);
		ADC_Cmd(ADC1,ENABLE);

		//=== ADC1��λУ׼��ʼֱ��У׼���
		ADC_ResetCalibration(ADC1);
		while(ADC_GetResetCalibrationStatus(ADC1));
		
		//=== ADC1����У׼��ʼֱ��У׼���
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1));
		
		//=== У׼��ɡ�ֱ�ӿ�ʼADCת��
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}		

/*************************************************************************************************/
/* ���ܣ�ACD��ʼ����ص�ѹ���       																											 			 */
/* ���룺��                                     																								 */
/* �������			                                                                                 */
/* ������8ͨ���ɼ�VDD����Ӧ����ΪPB0                             */
/*			 9ͨ���ɼ�+5V����Ӧ����ΪPB1                             */
/*************************************************************************************************/
void Get_AdcValue(Uint16 *p)   
{
		static Uint8 AdcErRAxisCntY = 0;
	
		//=== �鿴DMA�Ƿ�����ɣ���ɺ�����ñ�־��δ��������
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET)
		{
				AdcErRAxisCntY = 0;
				*(p + 0) = (AdcVal[0] + AdcVal[1] + AdcVal[2]   + AdcVal[3]   + AdcVal[4]   + AdcVal[5]   + AdcVal[6]   + AdcVal[7]) >> 3;
				*(p + 1) = (AdcVal[8] + AdcVal[9] + AdcVal[10]  + AdcVal[11]  + AdcVal[12]  + AdcVal[13]  + AdcVal[14]  + AdcVal[15])>> 3;
			
				//== �����ɱ�־
				DMA_ClearFlag(DMA1_FLAG_TC1);
			
				//== ��������һ�δ���
				ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		}
		else
		{
				AdcErRAxisCntY++;
		}
	
		if(AdcErRAxisCntY > 3)
		{
				//== ADCת������
		}
}

