#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//motor驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#define MOTOR_INF PBout(6)// PB6	直流电机控制线1
#define MOTOR_INR PBout(7)// PB7    直流电机控制线2

typedef enum MOTOR_Fun{
	HZ_STATE,
	Reverse,
	Forward,
	Brake
} enum_motor_status;


void MOTOR_Init(void);//初始化
void MOTOR_FR_ctr(enum_motor_status STATE);//电机运行控制函数

		 				    
#endif
