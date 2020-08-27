#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define Channel_Num  4			//ת��ͨ������Ŀ
#define Sample_Num  10			//ÿ��ͨ��ת���Ĵ���
	   
extern volatile vu16 ADCConvertedValue[Sample_Num][Channel_Num];//���ٵ�һ���ڴ棬�������ADCת�������
																//Ҳ��DMA��Ŀ���ַ,4ͨ����ÿͨ���ɼ�10�κ���ȡƽ����

void  Adc_Init(void);
u16 ReadADCAverageValue(uint16_t Channel);
float  Read_Adc(uint16_t Channel);
 
#endif 
