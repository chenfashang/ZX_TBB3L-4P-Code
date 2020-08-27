#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//LED驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#define LED1 PAout(4)// PA4	合闸指示灯
#define LED2 PAout(5)// PA5 分闸指示灯
#define LED3 PAout(6)// PA6 通讯指示灯
#define LED4 PAout(7)// PA7 运行指示灯


void LED_Init(void);//初始化
		 				    
#endif


