#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//motor��������
//�޸�����:2020/06/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �㽭�������� 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#define MOTOR_INF PBout(6)// PB6	ֱ�����������1
#define MOTOR_INR PBout(7)// PB7    ֱ�����������2

typedef enum MOTOR_Fun{
	HZ_STATE,
	Reverse,
	Forward,
	Brake
} enum_motor_status;


void MOTOR_Init(void);//��ʼ��
void MOTOR_FR_ctr(enum_motor_status STATE);//������п��ƺ���

		 				    
#endif
