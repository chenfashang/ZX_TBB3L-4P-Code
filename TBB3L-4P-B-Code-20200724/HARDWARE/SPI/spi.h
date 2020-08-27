#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//SPI2_CS �궨��
/******************************************************************************************************************/ 
//#define	GPIO_SPI2_CS 		 PBout(12)  	 //SPI��Ƭѡ�ź�
#define	GPIO_SPI2_CS 		 GPIO_Pin_12  	     //SPI��Ƭѡ�ź�
#define SPI2_CS_DISABLE() 	 GPIO_SetBits(GPIOB, GPIO_SPI2_CS);   
#define SPI2_CS_ENABLE()	 GPIO_ResetBits(GPIOB, GPIO_SPI2_CS); 



 				  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

