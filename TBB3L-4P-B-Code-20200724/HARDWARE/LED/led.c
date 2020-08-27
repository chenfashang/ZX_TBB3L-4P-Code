#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//LED��������
//�޸�����:2020/06/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �㽭�������� 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PA4��PA5��PA6��PA7Ϊ�����.��ʹ����Ӧ�˿ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOA�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;		//LED1��LED2��LED3��LED4-->PA.4��PA.5 ��PA.6��PA.7�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);						 //PA.4��PA.5��PA.6��PA.7 �����

}


