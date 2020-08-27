#include "delay.h"
#include "sys.h"
#include "rs485.h"
//#include "usart.h"
//#include "wdg.h" //��ʱ�رտ��Ź����ȳ�����Ϻ��ٿ���
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
 �˹��̳��������㽭��������4P©��
 ���������ѹ�������ɼ���©���⡢��բ��բ�ȹ��� 
 ����֧�֣�
 �㽭�����������޹�˾  
 ���ߣ�
************************************************/

u8 motor_run_flag = 0; //0,���裻1������2������3��ɲ�� 
int temp = 0;
u16 len=0,t=0,times=0;

			 	
int main(void)
{
	u8 flag = 0;
	
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	RS485_uart_init(9600);	//��ʼ��RS485����������9600   
	//IWDG_Init(4,625);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s,����ʱ���ÿ��Ź�
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�	
	KEY_EXTIX_Init();			//�Ϸ�բ�������ⲿ�жϳ�ʼ��
	LeakTest_Init();		//©����Գ�ʼ��
	ITR_EXTIX_Init();		//�г̿����жϳ�ʼ��
	MOTOR_Init();			//�����ʼ�� 
	Adc_Init();				//NTC�¶Ȳɼ���ADC��ʼ��
	TIM3_Int_Init(499,71);//��ʱ���ж��̣߳�500usһ���ж� Tout= ((arr+1)*(psc+1))/Tclk��
	SPI2_Init();			 //��ʼ��SPI2��
	V9203_GPIO_Init();  //V9203�����GPIO��������
	
	
	
	while(1)
	{
		//IWDG_Feed();//ι�� ��ʱ�رտ��Ź����ȳ�����Ϻ��ٿ���
	
		//LED�Ʋ���
	//	while(1)
	//	{		
			LED1 = ~LED1;
			LED2 = ~LED2;
			LED3 = ~LED3;
			LED4 = ~LED4;
		//	delay_ms(500);//��ʱ500ms
		
	//	}
	
		
		//����Ϸ�բ����
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
	
		//����2ͨ��
	//	while(1)
	//	{	
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				printf("\r\n�����͵���ϢΪ:\r\n");
				for(t=0;t<len;t++)
				{
					USART_SendData(USART2, USART_RX_BUF[t]);//�򴮿�2��������
					while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
			}
			else
			{
				times++;
				if(times%5==0)
				{				
					printf("\r\n�㽭��������4P©��\r\n");				
				}
				if(times%3==0)printf("\r\n����������,�Իس�������\r\n");  
				if(times%2==0) LED2=!LED2;//��˸LED,��ʾϵͳ��������.
				delay_ms(1);   
			}	
			
		//	delay_ms(100);
		
	//	}

	
			//ADCת��
	//	while(1)
	//	{		
			for(temp = 0;temp < 4;temp++)
				printf("�ɼ�TADC_%d��ֵ:%f \r\n",temp,Read_Adc(temp));
			temp = 0;
			printf("\r\n");		
		//	delay_ms(500);	
	//	}
		
		
		//©�����
//		while(1)
//		{
//			for(temp = 0;temp < 5;temp++)		
//				delay_ms(1000);//��ʱ5S	
//			temp = 0 ;	
//			LED1 = ~LED1;		
//			LeakTest = 1;//������				
//			printf("©���������ߵ�ƽ\r\n");
//			
//			for(temp = 0;temp < 5;temp++)		
//				delay_ms(1000);//��ʱ5S				
//			temp = 0 ;			
//			LED1 = ~LED1;
//			LeakTest = 0;
//			printf("©���������͵�ƽ\r\n");
//		}
//		
		
		//V9203,������ѹ����������ݵĲɼ��Լ���ȡ	
		while(1)
		{	
			//V9203_Init();
			if(!(flag = Get_V9203_init_flag()))
			{    printf("flag %d\r\n",flag);
				V9203_Init();
				delay_ms(500);
			}
			
			//��ȡ��ؼĴ�����ֵ
			if(Get_V9203_init_flag())
			{
			    V9203_resgiterRead();
			}
			delay_ms(70);			
			LED1 = ~LED1;		
		}		
 		
	
	}
}


