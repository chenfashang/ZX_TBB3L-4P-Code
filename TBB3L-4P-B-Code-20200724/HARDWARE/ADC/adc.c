 #include "adc.h"
 #include "delay.h"

//////////////////////////////////////////////////////////////////////////////////
//ADC ����	
//�޸�����:2020/7/17
//��Ȩ���У�����ؾ���
//Copyright(C) ���������Ƽ����޹�˾- 2020-									  
////////////////////////////////////////////////////////////////////////////////// 
	   
volatile vu16 ADCConvertedValue[Sample_Num][Channel_Num];//���ٵ�һ���ڴ棬�������ADCת�������Ҳ��DMA��Ŀ���ַ,4ͨ����ÿͨ���ɼ�10�κ�ȡƽ����



//��ʼ��ADC������DMA
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3

void  Adc_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 	
	DMA_InitTypeDef DMA_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��GPIOB,GPIOC,ADC1ͨ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA���� 
	
	/************ADC_Init****************/
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	ADC_DeInit(ADC1);  //��λADC1 

	//PC4��PC5 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	//PB0��PB1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 4;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ��� 
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	// ����ADCͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_239Cycles5);
	
	
	/* ��λDMA1��ͨ��1 */
	DMA_DeInit(DMA1_Channel1);	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//ADC��ַ,�ò������Զ���DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCConvertedValue; //�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����(�����赽�ڴ�)
	DMA_InitStructure.DMA_BufferSize = Channel_Num * Sample_Num; //����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ������Ҳ����Adc_Data�Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ; //�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; //DMAģʽ��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; //���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   //��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA1��1ͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}



//���ADCת����ֵ
//Channel��ֵΪ0,1,2,3����Ӧ�������Թ���14��15��8��9ͨ��
u16 ReadADCAverageValue(uint16_t Channel)
{
	u8 i;
	u32 sum = 0;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	for(i=0; i<Sample_Num; i++)
	{
		sum+=ADCConvertedValue[i][Channel];		//ĳ��ͨ������Sample_Num��ֵ֮��
	}
	return (sum/Sample_Num);	//ȡƽ��ֵ
}


//���ADCֵ
//Channel��ֵΪ0,1,2,3����Ӧ�������Թ���14��15��8��9ͨ��
float  Read_Adc(uint16_t Channel)   
{
  	u16  ADC_Value;
	
	ADC_Value = ReadADCAverageValue(Channel);	

	return (float)ADC_Value/4096 * 3.3;//��ƽ��ֵ��ת���ɵ�ѹֵ		

}






















