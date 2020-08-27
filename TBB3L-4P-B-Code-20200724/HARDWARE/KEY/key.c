#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"
#include "key.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//合分闸驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

extern u8 motor_run_flag; //0,高阻；1，反向；2，正向；3，刹车
								    
//合分闸按键SW_KEY初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//合分闸按键SW_KEY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.1
}

//合分闸按键SW_KEY外部中断1初始化
void KEY_EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOC.1 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;	//合分闸按键SW_KEY
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}

//合分闸按键SW_KEY外部中断1服务程序 
void EXTI1_IRQHandler(void)
{		
	delay_ms(20);//消抖
	if(SW_KEY == RESET)	 	 //SW_KEY按键按下
	{	
		if(motor_run_flag == 4)
		{
		   motor_run_flag = 0;			
		}	
		else{
			
		motor_run_flag++;
			
		}
		
			
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位  
}


