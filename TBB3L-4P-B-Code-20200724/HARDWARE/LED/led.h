#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//LED��������
//�޸�����:2020/06/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �㽭�������� 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#define LED1 PAout(4)// PA4	��բָʾ��
#define LED2 PAout(5)// PA5 ��բָʾ��
#define LED3 PAout(6)// PA6 ͨѶָʾ��
#define LED4 PAout(7)// PA7 ����ָʾ��


void LED_Init(void);//��ʼ��
		 				    
#endif


