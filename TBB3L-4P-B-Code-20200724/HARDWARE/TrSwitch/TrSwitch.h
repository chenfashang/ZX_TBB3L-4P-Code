#ifndef __TRSWITCH_H
#define __TRSWITCH_H 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//行程开关驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	 


#define ITR1_SW PCin(10)   	//PC10 行程开关ITR1
#define ITR2_SW PCin(11)   	//PC11 行程开关ITR2
#define ITR3_SW PCin(12)   	//PC12 行程开关ITR3
#define ITR4_SW PDin(2)   	//PD2  行程开关ITR4


//#define ITR1_SW  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//行程开关ITR1
//#define ITR2_SW  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//行程开关ITR2
//#define ITR3_SW  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//行程开关ITR3
//#define ITR4_SW  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//行程开关ITR4


 
void ITR_SW_Init(void);//IO初始化

void ITR_EXTIX_Init(void);//行程开关外部中断10,11,12和2初始化
					    
#endif
