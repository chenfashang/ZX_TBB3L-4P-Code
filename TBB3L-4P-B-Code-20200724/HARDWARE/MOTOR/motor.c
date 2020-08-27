#include "motor.h"

//////////////////////////////////////////////////////////////////////////////////	 
//motor驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB6、PB7为输出口.并使能相应端口的时钟		    
//MOTOR IO初始化
void MOTOR_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		// MOTOR_INF、 MOTOR_INR-->PB.6、PB.7端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);			    //PB.6、PB.7 输出低，电机处于高阻状态
}

void MOTOR_FR_ctr(enum_motor_status status)
{
	if( status == 0) //高阻状态
	{
		MOTOR_INR = 0 ;
		MOTOR_INF = 0 ;			
	}
	if( status == 1) //反向运行状态
	{		
		MOTOR_INR = 0 ;	
		MOTOR_INF = 1 ;
	}
	if( status == 2) //正向运行状态
	{
		MOTOR_INR = 1 ;	
		MOTOR_INF = 0 ;	
	}
	if( status == 3) //刹车状态
	{
		MOTOR_INR = 1 ;	
		MOTOR_INF = 1 ;	
	}

}
 
