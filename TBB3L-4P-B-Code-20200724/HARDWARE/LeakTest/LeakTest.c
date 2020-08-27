#include "LeakTest.h"

//////////////////////////////////////////////////////////////////////////////////	 
//LED驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PA1为输出口.并使能相应端口的时钟   
void LeakTest_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能GPIOA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		//LeakTest-->PA.1端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA.1输出低

}
 
