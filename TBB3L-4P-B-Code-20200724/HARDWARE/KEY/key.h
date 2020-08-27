#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//合分闸驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//#define SW_KEY  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//读取按键SW_KEY
#define SW_KEY PCin(1)   	//PC1 查询方式


 
void KEY_Init(void);//IO初始化 查询方式
void KEY_EXTIX_Init(void);//合分闸按键外部中断初始化
				    
#endif
