#include "LeakTest.h"

//////////////////////////////////////////////////////////////////////////////////	 
//LED��������
//�޸�����:2020/06/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �㽭�������� 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PA1Ϊ�����.��ʹ����Ӧ�˿ڵ�ʱ��   
void LeakTest_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOA�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//LeakTest-->PA.1�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA.1�����

}
 
