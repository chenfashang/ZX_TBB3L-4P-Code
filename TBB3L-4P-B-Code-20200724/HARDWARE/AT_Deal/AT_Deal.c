#include "AT_Deal.h"
#include "delay.h"

static BOOLEAN X485_AT_Mode_flag = FALSE;

/**
  * @brief  AT command is disposed.
  * @param  gUSART3_Temp:struct UART_TypeDef
	* @retval None
  */
void ThinkHome_AT_Deal(UART_TypeDef* gUSART3_Temp)
{
	u8 report_buf_len = 0;
	u8 i = 0, instr_status = 1;
	u8 Report_buf[64] = {0};
	
	gUSART3_Temp->len = gUSART3_Temp->buffer[TH_AT_LEN_POS];
	
	if((checksum_calc(gUSART3_Temp->buffer, gUSART3_Temp->len, (TH_AT_HEAD_LEN + TH_AT_LENGTH_CODE_LEN))
	 == (gUSART3_Temp->buffer[gUSART3_Temp->len + (TH_AT_HEAD_LEN + TH_AT_LENGTH_CODE_LEN)])))
	{
		//校验码正确，处理接收到的数据
    //1:帧头
    Report_buf[report_buf_len ++] = TH_AT_FIRST_CODE;
    Report_buf[report_buf_len ++] = TH_AT_SECOND_CODE;
    //2:长度高位
    Report_buf[report_buf_len ++] = X485_ZERO_CODE;
    report_buf_len ++;
    //3:指令类型
    Report_buf[report_buf_len ++] = TH_AT_FEED_CODE;
    Report_buf[report_buf_len ++] = X485_ONE_CODE;
    //4:指令码
    for(i = 0; i < TH_AT_CODE_LEN; i ++)
    {
      Report_buf[report_buf_len ++] = gUSART3_Temp->buffer[TH_AT_CODE_POS + i];
    }
		
		switch(gUSART3_Temp->buffer[TH_AT_COMM_POS])
    {
			case TH_AT_SET_CODE:      //AT设置指令
				if((gUSART3_Temp->buffer[TH_AT_CODE_POS] == 'C')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 1] == 'E')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 2] == 'I')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 3] == 'D'))
				{     //设置ThinkHome CC&EC ID
					if(X485_AT_Mode_flag)
					{
						if((gUSART3_Temp->len - TH_AT_CODE_LEN - TH_AT_COMM_LEN - TH_AT_COMM_NUM_LEN) == TH_AT_CEID_LEN)     //ID为16个字节
						{
							//写入 CC ID
							SPI_Flash_Write(X485_Slave_ID_Addr, gUSART3_Temp->buffer + 10, TH_AT_ID_LEN);		
							SPI_Flash_Read(Report_buf + report_buf_len, X485_Slave_ID_Addr, TH_AT_ID_LEN);
							report_buf_len += TH_AT_ID_LEN;	
							for(i = 0; i < TH_AT_ID_LEN; i ++)
							{
								Report_buf[report_buf_len + i] = gUSART3_Temp->buffer[report_buf_len+i] ;
							}
							report_buf_len += TH_AT_ID_LEN;
						}
						else
						{
							instr_status = 0;
						}
					}
					else
					{
						instr_status = 0;
					}
				}
				else if((gUSART3_Temp->buffer[TH_AT_CODE_POS] == 'D')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 1] == 'H')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 2] == 'X')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 3] == 'Y'))
				{
					if(X485_AT_Mode_flag)
					{
						if((gUSART3_Temp->len - TH_AT_CODE_LEN - TH_AT_COMM_LEN - TH_AT_COMM_NUM_LEN) == TH_AT_CEID_LEN)     //ID为16个字节
						{
 
						}
						else
						{
							instr_status = 0;
						}
					}
					else
					{
						instr_status = 0;
					}
				}
				else if((gUSART3_Temp->buffer[TH_AT_CODE_POS] == 'T')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 1] == 'E')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 2] == 'S')
        && (gUSART3_Temp->buffer[TH_AT_CODE_POS + 3] == 'T'))
				{    //测试相关
					if(X485_AT_Mode_flag)
					{
						
					}
					else
					{
						instr_status = 0;
					}				
				}
				else
				{
					instr_status = 0;
				}
				break;
			
			case TH_AT_INQUIRY_CODE:      //AT查询指令
				if(X485_AT_Mode_flag)
				{
					if(gUSART3_Temp->len >= (TH_AT_CODE_LEN + TH_AT_COMM_LEN + TH_AT_COMM_NUM_LEN))
					{    //数据长度正确
						if((gUSART3_Temp->buffer[TH_AT_CODE_POS] == 'C')
							&& (gUSART3_Temp->buffer[TH_AT_CODE_POS + 1] == 'E')
							&& (gUSART3_Temp->buffer[TH_AT_CODE_POS + 2] == 'I')
							&& (gUSART3_Temp->buffer[TH_AT_CODE_POS + 3] == 'D'))
						{     //查询ThinkHome CC&EC ID
							//5:指令数据
							SPI_Flash_Read(Report_buf + report_buf_len, X485_Slave_ID_Addr, TH_AT_CEID_LEN);
							report_buf_len += TH_AT_CEID_LEN;
						}
						else
						{
							instr_status = 0;
						}
					}
					else
					{
						instr_status = 0;
					}
				}
				else
				{
					instr_status = 0;
				}
				break;
			
			default:
				instr_status = 0;
				break;
		}
		
		if(instr_status)
    {
      //2:长度低位
      Report_buf[TH_AT_LEN_L_POS] = report_buf_len - ZNE_COMM_POS;
      //6:校验码
      Report_buf[report_buf_len] = checksum_calc(Report_buf, (report_buf_len - ZNE_COMM_POS), ZNE_COMM_POS); //校验码
      report_buf_len ++;
      //7:帧尾
      Report_buf[report_buf_len ++] = TH_AT_PRE_LAST_CODE;
      Report_buf[report_buf_len ++] = TH_AT_LAST_CODE;

      RS485_SLAVE_Response(Report_buf, report_buf_len);
    }
	}
	/*处理完毕，清除缓存器*/
	for(i = 0; i < UART_BUFFER_SIZE; i ++)
  {
    gUSART3_Temp->buffer[i] = 0;
  }
}

/**
  * @brief  TIM3 20ms task.
  * @param  None
	* @retval None
  */
void UART3_AT_Deal(void)
{
  //指令处理
  if(!gUSART3_Info.finish_flag)
  {     //有待处理指令
    if((gUSART3_Info.buffer[0] == TH_AT_FIRST_CODE)
       &&(gUSART3_Info.buffer[1] == TH_AT_SECOND_CODE))
    {
      ThinkHome_AT_Deal(&gUSART3_Info);      
    }

    gUSART3_Info.finish_flag = TRUE;
    X485_Slave_Data_Wait_Flag = FALSE;		
    X485_Host_Data_Get_Start_Flag = FALSE;
  }
}

/**
  * @brief  whether the 'OK' key is pressed.
  * @param  None
	* @retval TRUE:press the key; FALSE:no key pressed;
  */
BOOLEAN KeyStUp(void)
{
//	u8 Read_GPIO_PC6 = 0;
//	
//	GPIO_SetBits(GPIOD,GPIO_Pin_13);//1 
//	GPIO_ResetBits(GPIOD,GPIO_Pin_12);//0
//	Read_GPIO_PC6 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
//	
//	if(!Read_GPIO_PC6)
//		return TRUE;
//	else return FALSE;
}

/**
  * @brief  Disposed according to whether the key is pressed.
  * @param  None
  * @retval None
  */
void X485_Slave_ID_Init(void)
{
	BOOLEAN X485_AT_Mode_Wait_flag = FALSE;
	u16 time_count_enter = 0;
	u16 time_count_exit = 700;
	static u8 LCD_flag = 0;
	
	if(KeyStUp())                                       
  {
    delay_ms(80);   
    if(KeyStUp())
    {
      X485_AT_Mode_Wait_flag = TRUE;
    }
  }
	while(X485_AT_Mode_Wait_flag)
	{
		delay_ms(10);
		time_count_exit--;
		if(KeyStUp())
			time_count_enter++;
		else 
		{
			time_count_enter = 0;
		}
		if(time_count_exit < 2)
		{
			X485_AT_Mode_Wait_flag = FALSE;
			Set_X485_AT_Mode_flag_Value(FALSE);
		}
		if(time_count_enter > 350)
			Set_X485_AT_Mode_flag_Value(TRUE);
		while(Get_X485_AT_Mode_flag_Value())
		{
			static u16 time_count = 0;
				
			if(!LCD_flag)
			{
				LCD_flag = 1;
				USART_Cmd(USART3, ENABLE);
			}
			UART3_AT_Deal();
			time_count++;
			if(time_count >= 0xD00)
			{
				time_count = 0;
				Timer_USART3_Deal();
      }	
		}
	}
}

/**
  * @brief  set X485_AT_Mode_flag value.
  * @param  None
	* @retval None
  */
void Set_X485_AT_Mode_flag_Value(BOOLEAN pBuf)
{
	X485_AT_Mode_flag = pBuf;
}

/**
  * @brief  get X485_AT_Mode_flag.
  * @param  None
	* @retval None
  */
BOOLEAN Get_X485_AT_Mode_flag_Value(void)
{
	return X485_AT_Mode_flag;
}
