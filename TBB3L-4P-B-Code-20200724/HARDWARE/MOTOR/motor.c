#include "motor.h"

//////////////////////////////////////////////////////////////////////////////////	 
//motor��������
//�޸�����:2020/06/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �㽭�������� 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB6��PB7Ϊ�����.��ʹ����Ӧ�˿ڵ�ʱ��		    
//MOTOR IO��ʼ��
void MOTOR_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		// MOTOR_INF�� MOTOR_INR-->PB.6��PB.7�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);			    //PB.6��PB.7 ����ͣ�������ڸ���״̬
}

void MOTOR_FR_ctr(enum_motor_status status)
{
	if( status == 0) //����״̬
	{
		MOTOR_INR = 0 ;
		MOTOR_INF = 0 ;			
	}
	if( status == 1) //��������״̬
	{		
		MOTOR_INR = 0 ;	
		MOTOR_INF = 1 ;
	}
	if( status == 2) //��������״̬
	{
		MOTOR_INR = 1 ;	
		MOTOR_INF = 0 ;	
	}
	if( status == 3) //ɲ��״̬
	{
		MOTOR_INR = 1 ;	
		MOTOR_INF = 1 ;	
	}

}
 
