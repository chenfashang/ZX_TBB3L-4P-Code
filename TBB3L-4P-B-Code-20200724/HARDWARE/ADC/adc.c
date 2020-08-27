 #include "adc.h"
 #include "delay.h"

//////////////////////////////////////////////////////////////////////////////////
//ADC 代码	
//修改日期:2020/7/17
//版权所有，盗版必究。
//Copyright(C) 智轩电气科技有限公司- 2020-									  
////////////////////////////////////////////////////////////////////////////////// 
	   
volatile vu16 ADCConvertedValue[Sample_Num][Channel_Num];//开辟的一段内存，用来存放ADC转换结果，也是DMA的目标地址,4通道，每通道采集10次后取平均数



//初始化ADC并开启DMA
//这里我们仅以规则通道为例
//我们默认将开启通道0~3

void  Adc_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 	
	DMA_InitTypeDef DMA_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1, ENABLE );	  //使能GPIOB,GPIOC,ADC1通道时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输 
	
	/************ADC_Init****************/
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	ADC_DeInit(ADC1);  //复位ADC1 

	//PC4、PC5 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	//PB0、PB1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 4;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器 
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	// 配置ADC通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_239Cycles5);
	
	
	/* 复位DMA1的通道1 */
	DMA_DeInit(DMA1_Channel1);	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//ADC地址,该参数用以定义DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCConvertedValue; //该参数用以定义DMA内存基地址(转换结果保存的地址)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //方向(从外设到内存)
	DMA_InitStructure.DMA_BufferSize = Channel_Num * Sample_Num; //定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是Adc_Data的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //设定外设地址寄存器递增与否,此处设为不变 Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //用来设定内存地址寄存器递增与否,此处设为递增，Enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ; //外设数据单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //内存数据单位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; //DMA模式：循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; //优先级：高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   //禁止内存到内存的传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //配置DMA1的1通道
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}



//获得ADC转换的值
//Channel的值为0,1,2,3。对应数据来自规则14、15、8、9通道
u16 ReadADCAverageValue(uint16_t Channel)
{
	u8 i;
	u32 sum = 0;
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	for(i=0; i<Sample_Num; i++)
	{
		sum+=ADCConvertedValue[i][Channel];		//某个通道采样Sample_Num次值之和
	}
	return (sum/Sample_Num);	//取平均值
}


//获得ADC值
//Channel的值为0,1,2,3。对应数据来自规则14、15、8、9通道
float  Read_Adc(uint16_t Channel)   
{
  	u16  ADC_Value;
	
	ADC_Value = ReadADCAverageValue(Channel);	

	return (float)ADC_Value/4096 * 3.3;//求平均值并转换成电压值		

}






















