#include "delay.h"
#include "sys.h"
#include "rs485.h"
//#include "usart.h"
//#include "wdg.h" //暂时关闭看门狗，等程序完毕后再开启
#include "led.h"
#include "key.h"
#include "LeakTest.h"
#include "TrSwitch.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include "spi.h"
#include "V9203.h"
#include "ParaDispose.h"
//#include "AT_Deal.h"
 
/************************************************
 此工程程序用于浙江智轩电气4P漏保
 包括三相电压、电流采集、漏电检测、合闸分闸等功能 
 技术支持：
 浙江智轩电气有限公司  
 作者：
************************************************/

u8 motor_run_flag = 0; //0,高阻；1，反向；2，正向；3，刹车 
int temp = 0;
u16 len=0,t=0,times=0;

			 	
int main(void)
{
	u8 flag = 0;
	
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	RS485_uart_init(9600);	//初始化RS485，波特率是9600   
	//IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s,先暂时不用看门狗
	LED_Init();		  		//初始化与LED连接的硬件接口	
	KEY_EXTIX_Init();			//合分闸按键，外部中断初始化
	LeakTest_Init();		//漏电测试初始化
	ITR_EXTIX_Init();		//行程开关中断初始化
	MOTOR_Init();			//电机初始化 
	Adc_Init();				//NTC温度采集的ADC初始化
	TIM3_Int_Init(499,71);//定时器中断线程，500us一次中断 Tout= ((arr+1)*(psc+1))/Tclk，
	SPI2_Init();			 //初始化SPI2口
	V9203_GPIO_Init();  //V9203的相关GPIO引脚配置
	
	
	
	while(1)
	{
		//IWDG_Feed();//喂狗 暂时关闭看门狗，等程序完毕后再开启
	
		//LED灯测试
	//	while(1)
	//	{		
			LED1 = ~LED1;
			LED2 = ~LED2;
			LED3 = ~LED3;
			LED4 = ~LED4;
		//	delay_ms(500);//延时500ms
		
	//	}
	
		
		//电机合分闸控制
	//	while(1)
	//	{
			switch(motor_run_flag)
			{
				case 0:
						MOTOR_FR_ctr(HZ_STATE);	
						LED1 = ~LED1;
						break;
				case 1:
						MOTOR_FR_ctr(Reverse);	
						LED2 = ~LED2;
						break;
				case 2:
						MOTOR_FR_ctr(Forward);	
						LED3 = ~LED3;
						break;				
				case 3:
						MOTOR_FR_ctr(Brake);
						LED4 = ~LED4;
						break;				
			}
		
		//	delay_ms(500);
	
	//	}
	
		//串口2通信
	//	while(1)
	//	{	
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				printf("\r\n您发送的消息为:\r\n");
				for(t=0;t<len;t++)
				{
					USART_SendData(USART2, USART_RX_BUF[t]);//向串口2发送数据
					while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
				}
				printf("\r\n\r\n");//插入换行
				USART_RX_STA=0;
			}
			else
			{
				times++;
				if(times%5==0)
				{				
					printf("\r\n浙江智轩电气4P漏保\r\n");				
				}
				if(times%3==0)printf("\r\n请输入数据,以回车键结束\r\n");  
				if(times%2==0) LED2=!LED2;//闪烁LED,提示系统正在运行.
				delay_ms(1);   
			}	
			
		//	delay_ms(100);
		
	//	}

	
			//ADC转换
	//	while(1)
	//	{		
			for(temp = 0;temp < 4;temp++)
				printf("采集TADC_%d的值:%f \r\n",temp,Read_Adc(temp));
			temp = 0;
			printf("\r\n");		
		//	delay_ms(500);	
	//	}
		
		
		//漏电测试
//		while(1)
//		{
//			for(temp = 0;temp < 5;temp++)		
//				delay_ms(1000);//延时5S	
//			temp = 0 ;	
//			LED1 = ~LED1;		
//			LeakTest = 1;//输出检测				
//			printf("漏电测试输出高电平\r\n");
//			
//			for(temp = 0;temp < 5;temp++)		
//				delay_ms(1000);//延时5S				
//			temp = 0 ;			
//			LED1 = ~LED1;
//			LeakTest = 0;
//			printf("漏电测试输出低电平\r\n");
//		}
//		
		
		//V9203,电流电压功率相关数据的采集以及读取	
		while(1)
		{	
			//V9203_Init();
			if(!(flag = Get_V9203_init_flag()))
			{    printf("flag %d\r\n",flag);
				V9203_Init();
				delay_ms(500);
			}
			
			//读取相关寄存器的值
			if(Get_V9203_init_flag())
			{
			    V9203_resgiterRead();
			}
			delay_ms(70);			
			LED1 = ~LED1;		
		}		
 		
	
	}
}


