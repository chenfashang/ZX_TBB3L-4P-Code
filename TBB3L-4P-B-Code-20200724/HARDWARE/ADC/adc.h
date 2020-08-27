#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define Channel_Num  4			//转换通道的数目
#define Sample_Num  10			//每个通道转换的次数
	   
extern volatile vu16 ADCConvertedValue[Sample_Num][Channel_Num];//开辟的一段内存，用来存放ADC转换结果，
																//也是DMA的目标地址,4通道，每通道采集10次后面取平均数

void  Adc_Init(void);
u16 ReadADCAverageValue(uint16_t Channel);
float  Read_Adc(uint16_t Channel);
 
#endif 
