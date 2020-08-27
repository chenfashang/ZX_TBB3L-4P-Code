#include "stm32f10x.h"
#include "TrSwitch.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//行程开关驱动代码
//修改日期:2020/06/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 浙江智轩电气 2020-
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
								    
//行程开关初始化函数
void ITR_SW_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTC，PORTD时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;//行程开关PC10、PC11、PC12对应ITR1_SW、ITR2_SW、ITR3_SW
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.10、GPIOC.11、GPIOC.12
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//行程开关PD2对应ITR4_SW
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.2		
}


//行程开关外部中断10,11,12和2初始化
void ITR_EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    ITR_SW_Init();	 //	行程开关端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOC.10 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	//行程开关ITR1所在的中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	  //GPIOC.11 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;	//行程开关ITR2所在的中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	  //GPIOC.12 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	//行程开关ITR3所在的中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能行程开关ITR1\ITR2\ITR3所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
	
	   //GPIOD.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//行程开关ITR4所在的中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;		//使能行程开关ITR4所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 	
 
}


//外部中断10,11,12服务程序
void EXTI15_10_IRQHandler(void)
{
	u8  count = 0; 
	
	delay_ms(10);//消抖 
	
	if(ITR1_SW == RESET)    //中断10处理
      {
		  	while(1)
			{
				count++; 
						
				LED1 = ~LED1;						
				delay_ms(500);				
				LED1 = ~LED1;								
				delay_ms(500);
				
				if(count == 5) 
				{
					count = 0;
					break;
				}				
			}		  
           
           EXTI_ClearITPendingBit(EXTI_Line10);
	  }	 
	 if(ITR2_SW == RESET)    //中断11处理
      {
		  	while(1)
			{
				count++; 						
				
				LED2 = ~LED2;		
				delay_ms(500);				
				LED2 = ~LED2;				
				delay_ms(500);	
				
				if(count == 5) 
				{
					count = 0;
					break;
				}				
			}		  
           
           EXTI_ClearITPendingBit(EXTI_Line11);
	  }	
	  if(ITR3_SW == RESET)    //中断12处理
      {
		  	while(1)
			{
				count++; 						
				
				LED3 = ~LED3;		
				delay_ms(500);				
				LED3 = ~LED3;						
				delay_ms(500);	
				
				if(count == 5) 
				{
					count = 0;
					break;
				}				
			}		  
           
           EXTI_ClearITPendingBit(EXTI_Line12);
	  }	 

}


//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	u8  count = 0; 
	delay_ms(10);//消抖
	if(ITR4_SW == RESET)	  //按键KEY2
	{
			while(1)
			{
				count++; 
						
				LED4 = ~LED4;				
				delay_ms(500);				
				LED4 = ~LED4;								
				delay_ms(500);	
				
				if(count == 5) 
				{
					count = 0;
					break;
				}				
			}
			
			EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
	}		 
	
}



