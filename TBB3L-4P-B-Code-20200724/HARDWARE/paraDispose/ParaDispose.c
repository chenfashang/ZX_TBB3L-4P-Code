#include "ParaDispose.h"
#include "rs485.h"
#include "variable.h"
#include "motor.h"
#include "delay.h"


//#include "Config.h"

/*parameter init:Phase_A,Phase_B,Phase_C*/
ParaDispose_TypeDef gParaDispose_Info[4];
/**/
static u16 X485_Data_Report_Count_Time = 0;
/*how long does it take to save time*/
static u16  electricityValueSaveTime = 0;    
/*electricity flash read end flag*/
static BOOLEAN V9203_ParameterRead_Finish_Flag = FALSE;


void ElectricityValue_Save(void);
void Electricity_Report_Dispose(void);

/**
  * @brief  parameter init.
  * @param  None
  * @retval None
  */
void ParaDispose_init(void)
{
	u8 phaseX = 0;
	for(; phaseX <= BondPhase; phaseX++)//3相
	{
		gParaDispose_Info[phaseX].not_report_time[voltage] = 0;
		gParaDispose_Info[phaseX].not_report_time[current] = 0;
		gParaDispose_Info[phaseX].not_report_time[power] = 0;
		gParaDispose_Info[phaseX].not_report_time[powerFactor] = 0;
		gParaDispose_Info[phaseX].not_report_time[electricity] = 0;
						
		gParaDispose_Info[phaseX].report_num[voltage] = 0;
		gParaDispose_Info[phaseX].report_num[current] = 0;
		gParaDispose_Info[phaseX].report_num[power] = 0;
		gParaDispose_Info[phaseX].report_num[powerFactor] = 0;
		gParaDispose_Info[phaseX].report_num[electricity] = 0;
		
		gParaDispose_Info[phaseX].report_threshold[voltage][0] = 15;
		gParaDispose_Info[phaseX].report_threshold[current][0] = 15;
		gParaDispose_Info[phaseX].report_threshold[power][0] = 15;
		gParaDispose_Info[phaseX].report_threshold[powerFactor][0] = 15;
		
		gParaDispose_Info[phaseX].report_threshold[voltage][1] = 10;
		gParaDispose_Info[phaseX].report_threshold[current][1] = 10;
		gParaDispose_Info[phaseX].report_threshold[power][1] = 10;
		gParaDispose_Info[phaseX].report_threshold[powerFactor][1] = 10;
		
		gParaDispose_Info[phaseX].report_threshold[voltage][2] = 5;
		gParaDispose_Info[phaseX].report_threshold[current][2] = 5;
		gParaDispose_Info[phaseX].report_threshold[power][2] = 5;
		gParaDispose_Info[phaseX].report_threshold[powerFactor][2] = 5;
	}
}

#if 0


/**
  * @brief  voltage parameter, current parameter, power parameter,
  *         power factor parameter and electricity parameter dispose.
  * @param  None
  * @retval None
  */
void X485_Device_Para_Dispose(void)
{
	u8 phase_x = 0;
	
	if(X485_Data_Report_Count_Time >= TIMER_30MIN)
	{
		for(; phase_x <= BondPhase; phase_x++)
		{
			X485_Data_Report_Rule_Adjust(phase_x,voltage);
			X485_Data_Report_Rule_Adjust(phase_x,current);
			X485_Data_Report_Rule_Adjust(phase_x,power);
			X485_Data_Report_Rule_Adjust(phase_x,powerFactor);
		}
		X485_Data_Report_Rule_Adjust(BondPhase, electricLeakage);
		X485_Data_Report_Count_Time = 0;
	}
	#if TEST_ParaDispose
	#else
	if(electricityValueSaveTime >= TIMER_10MIN)
	#endif
	{
		ElectricityValue_Save();
		Electricity_Report_Dispose();
	}
	if(ZX_measure_ElecLeakage_wait_time >= ZX_MEASURE_ELECLEAKAGE_WAIT_MAX_TIME)
	{
		ElectrictLeakage_Dispose();
		ZX_measure_ElecLeakage_wait_time = 0;
	}
}

/**
  * @brief  voltage(current,power,power factor,electricity) parameter dont send time,
  *			    send count time and data dispose time.
  * @param  None
  * @retval None
  */
void X485_Device_Para_Record_Time(void)
{
	u8 phaseX = 0;
	for(phaseX = 0; phaseX <= Phase_C; phaseX++)
	{
		gParaDispose_Info[phaseX].not_report_time[voltage]++;
		gParaDispose_Info[phaseX].not_report_time[current]++;
		if(gUSART3_Info.transformer_flag)
		{
			gParaDispose_Info[phaseX].not_report_time[power]++;
			gParaDispose_Info[phaseX].not_report_time[powerFactor]++;
			gParaDispose_Info[phaseX].not_report_time[electricity]++;
		}
	}
	gParaDispose_Info[BondPhase].not_report_time[electricLeakage]++;
	gParaDispose_Info[BondPhase].not_report_time[electricity]++;
	/*30 minuter*/
	X485_Data_Report_Count_Time++;
	/*10 minuter*/
	electricityValueSaveTime++;
	/*20sec*/
	ZX_measure_ElecLeakage_wait_time++;
}

/**
	* @brief  electricity parpmeter report dispose(difference  upload).    unit:0.1kw.h    10x(10 times increase)
	*	        (Phase A,Phase B,PhaseC or BondPhase,difference if greater than 0.4kw.h upload)												
  * @param  None
  * @retval None
  */
void Electricity_Report_Dispose(void)
{
	u8 phase_x = 0;
	
	ZX_measure_power_consume_diff_data[Phase_A].UINT32 = gV9203_Info.electricityValue[Phase_A].UINT32 - 
													                             ZX_measure_Electricity_Old[Phase_A].UINT32;
	ZX_measure_power_consume_diff_data[Phase_B].UINT32 = gV9203_Info.electricityValue[Phase_B].UINT32 - 
													                             ZX_measure_Electricity_Old[Phase_B].UINT32;
	ZX_measure_power_consume_diff_data[Phase_C].UINT32 = gV9203_Info.electricityValue[Phase_C].UINT32 - 
													                             ZX_measure_Electricity_Old[Phase_C].UINT32;
	ZX_measure_power_consume_diff_data[BondPhase].UINT32 = gV9203_Info.electricityValue[BondPhase].UINT32 - 
													                               ZX_measure_Electricity_Old[BondPhase].UINT32;
	
	if(ZX_measure_power_consume_diff_data[Phase_A].UINT32 >= 1)
	{
		ZX_Measure_Report_Flag_Info.power_consum[Phase_A] = 1;
		gParaDispose_Info[Phase_A].not_report_time[electricity] = 0;
		if(ZX_measure_power_consume_report_data[Phase_A].UINT32)
		{
			ZX_measure_power_consume_report_data[Phase_A].UINT32 += ZX_measure_power_consume_diff_data[Phase_A].UINT32;
		}
		else
		{
			ZX_measure_power_consume_report_data[Phase_A].UINT32 = ZX_measure_power_consume_diff_data[Phase_A].UINT32;
		}
	}
	else if(gParaDispose_Info[Phase_A].not_report_time[electricity] >= TIMER_30MIN)
	{
		ZX_Measure_Report_Flag_Info.power_consum[Phase_A] = 1;
		gParaDispose_Info[Phase_A].not_report_time[electricity] = 0;
	}
	
	
	if(ZX_measure_power_consume_diff_data[Phase_B].UINT32 >= 1)
	{
		ZX_Measure_Report_Flag_Info.power_consum[Phase_B] = 1;
		gParaDispose_Info[Phase_B].not_report_time[electricity] = 0;
		if(ZX_measure_power_consume_report_data[Phase_B].UINT32)
		{
			ZX_measure_power_consume_report_data[Phase_B].UINT32 += ZX_measure_power_consume_diff_data[Phase_B].UINT32;
		}
		else
		{
			ZX_measure_power_consume_report_data[Phase_B].UINT32 = ZX_measure_power_consume_diff_data[Phase_B].UINT32;
		}
	}
	else if(gParaDispose_Info[Phase_B].not_report_time[electricity] >= TIMER_30MIN)
	{
		ZX_Measure_Report_Flag_Info.power_consum[Phase_B] = 1;
		gParaDispose_Info[Phase_B].not_report_time[electricity] = 0;
	}
	
	if(ZX_measure_power_consume_diff_data[Phase_C].UINT32 >= 1)
	{
		ZX_Measure_Report_Flag_Info.power_consum[Phase_C] = 1;
		gParaDispose_Info[Phase_C].not_report_time[electricity] = 0;
		if(ZX_measure_power_consume_report_data[Phase_C].UINT32)
		{
			ZX_measure_power_consume_report_data[Phase_C].UINT32 += ZX_measure_power_consume_diff_data[Phase_C].UINT32;
		}
		else
		{
			ZX_measure_power_consume_report_data[Phase_C].UINT32 = ZX_measure_power_consume_diff_data[Phase_C].UINT32;
		}
	}
	else if(gParaDispose_Info[Phase_C].not_report_time[electricity] >= TIMER_30MIN)
	{
		ZX_Measure_Report_Flag_Info.power_consum[Phase_C] = 1;
		gParaDispose_Info[Phase_C].not_report_time[electricity] = 0;
	}
	
	
	if(ZX_measure_power_consume_diff_data[BondPhase].UINT32 >= 1)
	{   
		ZX_Measure_Report_Flag_Info.power_consum[BondPhase] = 1;
		gParaDispose_Info[BondPhase].not_report_time[electricity] = 0;
		if(ZX_measure_power_consume_report_data[BondPhase].UINT32)
		{
			ZX_measure_power_consume_report_data[BondPhase].UINT32 += ZX_measure_power_consume_diff_data[BondPhase].UINT32;
		}
		else
		{
			ZX_measure_power_consume_report_data[BondPhase].UINT32 = ZX_measure_power_consume_diff_data[BondPhase].UINT32;
		}
	}
	else if(gParaDispose_Info[BondPhase].not_report_time[electricity] >= TIMER_30MIN)
	{
		ZX_Measure_Report_Flag_Info.power_consum[BondPhase] = 1;
		gParaDispose_Info[BondPhase].not_report_time[electricity] = 0;
	}
	
	for(phase_x = 0; phase_x <= BondPhase; phase_x++)
	{
		ZX_measure_Electricity_Old[phase_x].UINT32 = gV9203_Info.electricityValue[phase_x].UINT32;
	}
}

/**
	* @brief  read power constumption from 	W25Qflash.    unit:0.1kw.h  10x(10 times increase)
	*	        (Phase A,Phase B,PhaseC and BondPhase power constumption)
  * @param  None
  * @retval None
  */
void W25Q_PowerConsume_Read(void)
{
  u16 x485_slave_down_addr;
  u16 x485_slave_up_addr;
  u16 tmp_addr_offset;
  u16 x485_now_addr; 
  u8 electricityValueBuf[16] = {0};
  u8 offset = 0, phase_x = 0, i = 0;
	u16 THP_Power_Consum_Status_addr = 0;
 
  THP_Power_Consum_Status_addr = X485_Slave_Power_Consum_Addr;//0x8000
  
	/*0x1000  a sector*/
  tmp_addr_offset = FLASH_SECTOR_SIZE;
  
  x485_slave_down_addr = THP_Power_Consum_Status_addr;//0x8000
  x485_slave_up_addr = THP_Power_Consum_Status_addr + tmp_addr_offset;//0x9000	

  while(tmp_addr_offset >= 32)
  {
		tmp_addr_offset /= 2;//2048  
		
		x485_now_addr = x485_slave_down_addr + tmp_addr_offset;//0x8500
		SPI_Flash_Read(ZX_measure_Electricity_Old[Phase_A].UINT8, x485_now_addr, 4);  
		if(ZX_measure_Electricity_Old[Phase_A].UINT32 == 0xFFFFFFFF)
		{//first half
			x485_slave_up_addr = x485_now_addr;		
		}	
		else
		{//second half
			x485_slave_down_addr = x485_now_addr;
		}
  }
  SPI_Flash_Read(ZX_measure_Electricity_Old[Phase_A].UINT8, x485_slave_up_addr, 4);
	/* determine write address*/
	if(x485_slave_up_addr == THP_Power_Consum_Status_addr+FLASH_SECTOR_SIZE)// addr == 0x9000
	{
		THP_Power_Consum_Status_addr = x485_slave_down_addr;
	}
	else
	{
		if(ZX_measure_Electricity_Old[Phase_A].UINT32 == 0xFFFFFFFF)
		{
			THP_Power_Consum_Status_addr = x485_slave_down_addr;
		}
		else
		{
			THP_Power_Consum_Status_addr = x485_slave_up_addr;
		}
	}
  
  SPI_Flash_Read(electricityValueBuf, THP_Power_Consum_Status_addr, SLAVE_POWER_CONSUM_DATA_NUM);//16
  
  for(; phase_x <= BondPhase; phase_x++)
  {
		offset = phase_x << 2;
		for(i = 0; i < 4; i++)
			gV9203_Info.electricityValue[phase_x].UNIT8[3-i] = electricityValueBuf[i+offset];
  } 
  /*determine whether write in*/
  if(gV9203_Info.electricityValue[Phase_A].UINT32 == 0xFFFFFFFF)
  {
    gV9203_Info.electricityValue[Phase_A].UINT32 = 0;
		gV9203_Info.electricityValue[Phase_B].UINT32 = 0;
		gV9203_Info.electricityValue[Phase_C].UINT32 = 0;
		gV9203_Info.electricityValue[BondPhase].UINT32 = 0;
  } 
  ZX_measure_Electricity_Old[Phase_A].UINT32 = gV9203_Info.electricityValue[Phase_A].UINT32;
  ZX_measure_Electricity_Old[Phase_B].UINT32 = gV9203_Info.electricityValue[Phase_B].UINT32;
  ZX_measure_Electricity_Old[Phase_C].UINT32 = gV9203_Info.electricityValue[Phase_C].UINT32;
  ZX_measure_Electricity_Old[BondPhase].UINT32 = gV9203_Info.electricityValue[BondPhase].UINT32;
	/* don't use the first few addresses(16 byte)*/
  gV9203_Info.energyCountValue_WriteAddr = THP_Power_Consum_Status_addr + SLAVE_POWER_CONSUM_DATA_NUM;
}

void ElectricityValue_Save(void)
{
	u8 phase_x = 0, AddrOffset = 0;
	u8 tempBuf[16] = {0};
	
	electricityValueSaveTime = 0;
	
	if((gV9203_Info.electricityValue[BondPhase].UINT32  > ZX_measure_Electricity_Old[BondPhase].UINT32) || \
	 (gV9203_Info.electricityValue[Phase_B].UINT32  > ZX_measure_Electricity_Old[Phase_B].UINT32) || \
	 (gV9203_Info.electricityValue[Phase_C].UINT32  > ZX_measure_Electricity_Old[Phase_C].UINT32) || \
	 (gV9203_Info.electricityValue[Phase_A].UINT32  > ZX_measure_Electricity_Old[Phase_A].UINT32))
	{
		for(phase_x = 0; phase_x <= BondPhase; phase_x++)
		{
			AddrOffset = phase_x << 2;
			tempBuf[0+AddrOffset] = gV9203_Info.electricityValue[phase_x].UNIT8[3];
			tempBuf[1+AddrOffset] = gV9203_Info.electricityValue[phase_x].UNIT8[2];
			tempBuf[2+AddrOffset] = gV9203_Info.electricityValue[phase_x].UNIT8[1];
			tempBuf[3+AddrOffset] = gV9203_Info.electricityValue[phase_x].UNIT8[0];
		}
		if((RCC_GetFlagStatus(RCC_FLAG_LPWRRST) == RESET))
		{
			if(gV9203_Info.energyCountValue_WriteAddr >= 0x9000)
			{
				gV9203_Info.energyCountValue_WriteAddr = X485_Slave_Power_Consum_Addr + SLAVE_POWER_CONSUM_DATA_NUM;//0x8000+16
				SPI_Flash_Erase_Sector(8);
			}			
			SPI_Flash_Write(gV9203_Info.energyCountValue_WriteAddr, tempBuf, SLAVE_POWER_CONSUM_DATA_NUM);
			gV9203_Info.energyCountValue_WriteAddr += 16;
		}
	}
}

/**
  * @brief  EleLeakage report status dispose.
  * @param  None
  * @retval None
  */
void X485_Device_Report_EleLeakage_Data(u32 new_data)
{
	ZX_Measure_Report_Flag_Info.elec_leakage =1;
	
	ZX_measure_ElecLeakage_data.UINT32 = new_data;
  gParaDispose_Info[BondPhase].not_report_time[electricLeakage] = 0;
	if(gParaDispose_Info[BondPhase].report_num[electricLeakage] < MAXIMUM_REPORT_COUNT)
		gParaDispose_Info[BondPhase].report_num[electricLeakage]++;
}

/**
  * @brief  Electrict leakage parameter dispose.
  * @param  None
  * @retval None
  */
void ElectrictLeakage_Dispose(void)
{
	static u16 OldelectricLeakage = 0;
	u16 differenceElectricLeakage = 0;
	u32 tempBuf = 0;
	
//	tempBuf = DATA_GetGuiIdnLcdVal() * 10;
	
	if(ZX_measure_elecLeakage_check_num < ELECLEAKAGE_AVG_LIMIT)
	{
		ZX_measure_elecLeakage_sum_data.UINT32 += tempBuf;
		ZX_measure_elecLeakage_check_num++;
	}
	
	if(OldelectricLeakage >= tempBuf)
		differenceElectricLeakage = OldelectricLeakage - tempBuf;
	else differenceElectricLeakage = tempBuf - OldelectricLeakage;
	
	if(differenceElectricLeakage >= 5)
	{
		if(gParaDispose_Info[BondPhase].not_report_time[electricLeakage] <= TIMER_10MIN)
		{		
			if(differenceElectricLeakage >= gParaDispose_Info[BondPhase].report_threshold[electricLeakage][0])
				X485_Device_Report_EleLeakage_Data(tempBuf);
		}
		else if(gParaDispose_Info[BondPhase].not_report_time[electricLeakage] <= TIMER_20MIN)
		{
			if(differenceElectricLeakage >= gParaDispose_Info[BondPhase].report_threshold[electricLeakage][1])
				X485_Device_Report_EleLeakage_Data(tempBuf);				
		}
		else
		{
			if(differenceElectricLeakage >= gParaDispose_Info[BondPhase].report_threshold[electricLeakage][2])
				X485_Device_Report_EleLeakage_Data(tempBuf);			
		}
	}
	OldelectricLeakage = tempBuf;
}

/**
  * @brief  Reported number of processing.
  * @param  report_num:the number of electricity parameter reported.
  * @param  parameter:electricity parameter.This parameter can be:voltage(0),current(1),power(2),powerFactor(3)
  * @param  phaseX: phase.This parameter can be:Phase_A(0),Phase_B(1),Phase_C(2) or BondPhase(3).
  * @retval None
  */
void X485_Data_Report_Rule_Adjust( u8 phaseX,u8 parameter)
{
	if(gParaDispose_Info[phaseX].report_num[parameter] < 5)
	{
		if(gParaDispose_Info[phaseX].report_threshold[parameter][0] > 15)
		{
			gParaDispose_Info[phaseX].report_threshold[parameter][0] -= 5;
			gParaDispose_Info[phaseX].report_threshold[parameter][1] -= 5;
			gParaDispose_Info[phaseX].report_threshold[parameter][2] -= 5;
		}
	}
	else if(gParaDispose_Info[phaseX].report_num[parameter] > 10)
	{
		if(gParaDispose_Info[phaseX].report_threshold[parameter][0] < 80)
		{
			gParaDispose_Info[phaseX].report_threshold[parameter][0] += 5;
			gParaDispose_Info[phaseX].report_threshold[parameter][1] += 5;
			gParaDispose_Info[phaseX].report_threshold[parameter][2] += 5;
		}
	}
	gParaDispose_Info[phaseX].report_num[parameter] = 0;
}

#endif

/**
  * @brief  ELectricity Value dispose.
  * @param  None
  * @retval None
  */
void ELectricityValue_Dispose(u8 phase_x, u32 pBuf)
{
	static u32 oldEnergyCountValue[4] = {0};
	u32 newEnergyCountValue = 0, 
	    energyCountValue_Temp = 0;		
	
	newEnergyCountValue = pBuf / 80;
	
	/* gV9203_Info.energyCountValue[phase] = 8 = 0.01kwh,electricity precision 0.1kwh*/
	if(oldEnergyCountValue[phase_x] > newEnergyCountValue)
		energyCountValue_Temp = maxEnergyCountValue - oldEnergyCountValue[phase_x] + newEnergyCountValue;	
	else 
		energyCountValue_Temp = newEnergyCountValue - oldEnergyCountValue[phase_x];
	
	gV9203_Info.electricityValue[phase_x].UINT32 += energyCountValue_Temp;//0.1kwh
	
	if(gV9203_Info.electricityValue[BondPhase].UINT32 >= 0xFFFFFEFF)
	{
		gV9203_Info.electricityValue[BondPhase].UINT32 = 0;
		gV9203_Info.electricityValue[Phase_A].UINT32 = 0;
		gV9203_Info.electricityValue[Phase_B].UINT32 = 0;
		gV9203_Info.electricityValue[Phase_C].UINT32 = 0;
		ZX_measure_power_consume_report_data[BondPhase].UINT32 = 0;
		ZX_measure_power_consume_report_data[Phase_A].UINT32 = 0;
		ZX_measure_power_consume_report_data[Phase_B].UINT32 = 0;
		ZX_measure_power_consume_report_data[Phase_C].UINT32 = 0;
	}
	oldEnergyCountValue[phase_x] = newEnergyCountValue;	
	printf("能量相名:%d		值是:%d \r\n",phase_x,newEnergyCountValue);
}



/**
  * @brief  voltage report status dispose.
  * @param  None
  * @retval None
  */
void X485_Device_Report_Vol_Data(u8 phase_x, u16 new_vol)
{  
	ZX_Measure_Report_Flag_Info.vol[phase_x] =1;
	
	gV9203_Info.voltageValue[phase_x].UINT16 = new_vol;
  gParaDispose_Info[phase_x].not_report_time[voltage] = 0;
	if(gParaDispose_Info[phase_x].report_num[voltage] < MAXIMUM_REPORT_COUNT)
		gParaDispose_Info[phase_x].report_num[voltage]++;
}



/**
  * @brief  voltage parameter dispose.
  * @param  None
  * @retval None
  */
void Voltage_Parameter_Dispose(u32 pBuf,u8 phase_x)
{
	u16 TempBuf = 0;
	u16 para_data_diff = 0;
	
	if(pBuf > 312450)//312450/20830=15/1.5V
	{
		TempBuf = pBuf / gs_JbPm.ul_URmG;//0.1
	}
	else 
	{
		if(gV9203_Info.voltageValue[phase_x].UINT16)
		{
			X485_Device_Report_Vol_Data(phase_x, TempBuf);
		}
	}
	
	if(ZX_measure_vol_check_num[phase_x] < VOL_AVG_LIMIT)
	{
		ZX_measure_vol_sum_data[phase_x].UINT32 += TempBuf;
		ZX_measure_vol_check_num[phase_x]++;
	}

	if(TempBuf >= gV9203_Info.voltageValue[phase_x].UINT16)
	{
		para_data_diff = TempBuf - gV9203_Info.voltageValue[phase_x].UINT16;
	}
	else
	{
		para_data_diff = gV9203_Info.voltageValue[phase_x].UINT16 - TempBuf;
	}
	
	if(para_data_diff >= 10)
	{
		u16 report_diff_data;
		if(gParaDispose_Info[phase_x].not_report_time[voltage] <= TIMER_10MIN)
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[voltage][MEASURE_DATA_REPORT_THRESHOLD_01_POS] * 
												 TempBuf / VOL_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Vol_Data(phase_x, TempBuf);
			}
		}
		else if((gParaDispose_Info[phase_x].not_report_time[voltage] > TIMER_10MIN) && 
						(gParaDispose_Info[phase_x].not_report_time[voltage] <= TIMER_20MIN))
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[voltage][MEASURE_DATA_REPORT_THRESHOLD_02_POS] * 
												 TempBuf / VOL_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Vol_Data(phase_x, TempBuf);
			}
		}
		else
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[voltage][MEASURE_DATA_REPORT_THRESHOLD_03_POS] * 
												 TempBuf / VOL_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Vol_Data(phase_x, TempBuf);
			}
		}
	}
	
	printf("电压相名:%d		值是:%d \r\n",phase_x,TempBuf);
}



/**
  * @brief  current report status dispose.
  * @param  None
  * @retval None
  */
void X485_Device_Report_Current_Data(u8 phase_x, u32 cur_new_data)
{
	ZX_Measure_Report_Flag_Info.cur[phase_x] =1;
	
	gV9203_Info.currentValue[phase_x].UINT32 = cur_new_data;
  gParaDispose_Info[phase_x].not_report_time[current] = 0;
	if(gParaDispose_Info[phase_x].report_num[current] < MAXIMUM_REPORT_COUNT)
		gParaDispose_Info[phase_x].report_num[current]++;
}



/**
  * @brief  current parameter dispose.
  * @param  None
  * @retval None
  */
void Current_Paramenter_Dispose(u32 pBuf,u8 phase_x)
{
	u32 TempBuf = 0;
	u16 para_data_diff = 0;
	
	if(pBuf <= 200000)
	{
		if(gV9203_Info.currentValue[phase_x].UINT32)
		{
			X485_Device_Report_Current_Data(phase_x, TempBuf);
		}
	}
	else
	{
		TempBuf = pBuf / gs_JbPm.ul_I1RmG;//0.01
	}
	
	if(ZX_measure_cur_check_num[phase_x] < CUR_AVG_LIMIT)
	{
		ZX_measure_cur_sum_data[phase_x].UINT32 += TempBuf;
		ZX_measure_cur_check_num[phase_x]++;
	}

	if(TempBuf >= gV9203_Info.currentValue[phase_x].UINT32)
	{
		para_data_diff = TempBuf - gV9203_Info.currentValue[phase_x].UINT32;
	}
	else
	{
		para_data_diff = gV9203_Info.currentValue[phase_x].UINT32 - TempBuf;
	}
	
	if(para_data_diff >= 30)
	{
		u32 report_diff_data;
		if(gParaDispose_Info[phase_x].not_report_time[current] <= TIMER_10MIN)
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[current][MEASURE_DATA_REPORT_THRESHOLD_01_POS] * 
												 TempBuf / CUR_REPORT_PERCENTAGE_MAGNIFICATION;
			{
				if(para_data_diff >= report_diff_data)
				{
					X485_Device_Report_Current_Data(phase_x, TempBuf);
				}
			}
		}
		else if((gParaDispose_Info[phase_x].not_report_time[current] > TIMER_10MIN) && 
						(gParaDispose_Info[phase_x].not_report_time[current] <= TIMER_20MIN))
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[current][MEASURE_DATA_REPORT_THRESHOLD_02_POS] * 
												 TempBuf / CUR_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Current_Data(phase_x, TempBuf);
			}
		}
		else
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[current][MEASURE_DATA_REPORT_THRESHOLD_03_POS] * 
												 TempBuf / CUR_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Current_Data(phase_x, TempBuf);
			}
		}
	}
	
	printf("电流相名:%d		值是:%d \r\n",phase_x,TempBuf);
}



/**
  * @brief  current parameter dispose.
  * @param  None
  * @retval None
  */
void LeakCurrent_Paramenter_Dispose(u32 pBuf,u8 phase_x)
{
	u32 TempBuf = 0;
	u16 para_data_diff = 0;
	
	if(pBuf <= 1020000)
	{
		if(gV9203_Info.currentValue[phase_x].UINT32)
		{
			X485_Device_Report_Current_Data(phase_x, TempBuf);
		}
	}
	else
	{
		TempBuf = pBuf / gs_JbPm.ul_InRmG;//0.01
//		//大于等于22毫安，就跳闸
//		if(TempBuf >= 2.2)
//		{
//			MOTOR_FR_ctr(Reverse);//转动电机跳闸
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(1000);			
//			MOTOR_FR_ctr(HZ_STATE);//跳闸后停止	
//			printf("漏电超过22毫安，跳闸	");
//		
//		}		
	}
	
	if(ZX_measure_cur_check_num[phase_x] < CUR_AVG_LIMIT)
	{
		ZX_measure_cur_sum_data[phase_x].UINT32 += TempBuf;
		ZX_measure_cur_check_num[phase_x]++;
	}

	if(TempBuf >= gV9203_Info.currentValue[phase_x].UINT32)
	{
		para_data_diff = TempBuf - gV9203_Info.currentValue[phase_x].UINT32;
	}
	else
	{
		para_data_diff = gV9203_Info.currentValue[phase_x].UINT32 - TempBuf;
	}
	
	if(para_data_diff >= 30)
	{
		u32 report_diff_data;
		if(gParaDispose_Info[phase_x].not_report_time[current] <= TIMER_10MIN)
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[current][MEASURE_DATA_REPORT_THRESHOLD_01_POS] * 
												 TempBuf / CUR_REPORT_PERCENTAGE_MAGNIFICATION;
			{
				if(para_data_diff >= report_diff_data)
				{
					X485_Device_Report_Current_Data(phase_x, TempBuf);
				}
			}
		}
		else if((gParaDispose_Info[phase_x].not_report_time[current] > TIMER_10MIN) && 
						(gParaDispose_Info[phase_x].not_report_time[current] <= TIMER_20MIN))
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[current][MEASURE_DATA_REPORT_THRESHOLD_02_POS] * 
												 TempBuf / CUR_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Current_Data(phase_x, TempBuf);
			}
		}
		else
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[current][MEASURE_DATA_REPORT_THRESHOLD_03_POS] * 
												 TempBuf / CUR_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Current_Data(phase_x, TempBuf);
			}
		}
	}
	
	printf("漏电流相名:%d		值是:%d \r\n",phase_x,TempBuf);
	
	
}



/**
  * @brief  power report status dispose.
  * @param  None
  * @retval None
  */
void X485_Device_Report_Power_Data(u8 phase_x, u32 new_power)
{
	ZX_Measure_Report_Flag_Info.act_power[phase_x] =1;
	
	gV9203_Info.powerValue[phase_x].UINT32 = new_power;
  gParaDispose_Info[phase_x].not_report_time[power] = 0;
	if(gParaDispose_Info[phase_x].report_num[power] < MAXIMUM_REPORT_COUNT)
		gParaDispose_Info[phase_x].report_num[power]++;	
}

/**
  * @brief  power parameter dispose.
  * @param  None
  * @retval None
  */
void Power_Paramenter_Dispose(u32 pBuf,u8 phase_x)
{
	u32 TempBuf = 0;
	u32 para_data_diff = 0;
	
	if(gV9203_Info.currentValue[phase_x].UINT32)
	{
		if(pBuf & 0x80000000)
		{
			pBuf = (~pBuf)+1;
		}
		
		TempBuf = pBuf / gs_JbPm.ul_PG;//1
	}
	else
	{
		if(gV9203_Info.currentValue[phase_x].UINT32)
		{
			X485_Device_Report_Power_Data(phase_x, TempBuf);
		}
	}
	
	if(ZX_measure_act_power_check_num[phase_x] < POWER_AVG_LIMIT)
	{
		ZX_measure_act_power_sum_data[phase_x].UINT32 += TempBuf;
		ZX_measure_act_power_check_num[phase_x]++;
	}
	
	if(TempBuf >= gV9203_Info.powerValue[phase_x].UINT32)
	{
		para_data_diff = TempBuf - gV9203_Info.powerValue[phase_x].UINT32;
	}
	else
	{
		para_data_diff = gV9203_Info.powerValue[phase_x].UINT32 - TempBuf;
	}
		
	if(para_data_diff >= 55)
	{
		u32 report_diff_data;
		if(gParaDispose_Info[phase_x].not_report_time[power] <= TIMER_10MIN)
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[power][MEASURE_DATA_REPORT_THRESHOLD_01_POS] * 
												 TempBuf / POWER_REPORT_PERCENTAGE_MAGNIFICATION;
			{
				if(para_data_diff >= report_diff_data)
				{//??
					X485_Device_Report_Power_Data(phase_x, TempBuf);
				}
			}
		}
		else if((gParaDispose_Info[phase_x].not_report_time[power] > TIMER_10MIN) && 
						(gParaDispose_Info[phase_x].not_report_time[power] <= TIMER_20MIN))
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[power][MEASURE_DATA_REPORT_THRESHOLD_02_POS] * 
												 TempBuf / POWER_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Power_Data(phase_x, TempBuf);
			}
		}
		else
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[power][MEASURE_DATA_REPORT_THRESHOLD_03_POS] * 
												 TempBuf / POWER_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Power_Data(phase_x, TempBuf);
			}
		}
	}
	
	printf("功率相名:%d		值是:%d \r\n",phase_x,TempBuf);
}



/**
  * @brief  power factor report status dispose.
  * @param  None
  * @retval None
  */
void X485_Device_Report_Powerfactor_Data(u8 phase_x, u16 new_data)
{
	ZX_Measure_Report_Flag_Info.powerfactor[phase_x] =1;
	
	gV9203_Info.powerFactorValue[phase_x] = new_data;
  gParaDispose_Info[phase_x].not_report_time[powerFactor] = 0;
	if(gParaDispose_Info[phase_x].report_num[powerFactor] < MAXIMUM_REPORT_COUNT)
		gParaDispose_Info[phase_x].report_num[powerFactor]++;		
}

/**
  * @brief  power factor parameter dispose.
  * @param  None
  * @retval None
  */
void PowerFactor_Paramenter_Dispose(u32 pBuf, u8 phase_x)
{
	u16 TempBuf = 0;
	u16 para_data_diff = 0;
	
	/*电压从正常到断相功率因素可能会波动*/
	if((pBuf <= 0xCCCCCC8) || 
		 (gV9203_Info.voltageValue[phase_x].UINT16 < 0x15) || 
	   (gV9203_Info.currentValue[phase_x].UINT32 == 0))
	{
		if(gV9203_Info.powerFactorValue[phase_x])
		{
			X485_Device_Report_Powerfactor_Data(phase_x, TempBuf);
		}
	}
	else
	{
		TempBuf = pBuf / 0x147AE14;
	}

	if(ZX_measure_powerfactor_check_num[phase_x] < POWERFACTOR_AVG_LIMIT)
	{
		ZX_measure_powerfactor_sum_data[phase_x].UINT32 += TempBuf;
		ZX_measure_powerfactor_check_num[phase_x]++;
	}
	
	if(TempBuf >= gV9203_Info.powerFactorValue[phase_x])
	{
		para_data_diff = TempBuf - gV9203_Info.powerFactorValue[phase_x];
	}
	else
	{
		para_data_diff = gV9203_Info.powerFactorValue[phase_x] - TempBuf;
	}

	if(para_data_diff >= 5)
	{
		u8 report_diff_data;
		if(gParaDispose_Info[phase_x].not_report_time[powerFactor] <= TIMER_10MIN)
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[powerFactor][MEASURE_DATA_REPORT_THRESHOLD_01_POS] * 
												 TempBuf /
												 POWERFACTOR_REPORT_PERCENTAGE_MAGNIFICATION;
			{
				if(para_data_diff >= report_diff_data)
				{//??
					X485_Device_Report_Powerfactor_Data(phase_x, TempBuf);
				}
			}
		}
		else if((gParaDispose_Info[phase_x].not_report_time[powerFactor] > TIMER_10MIN) && 
						(gParaDispose_Info[phase_x].not_report_time[powerFactor] <= TIMER_20MIN))
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[powerFactor][MEASURE_DATA_REPORT_THRESHOLD_02_POS] * 
												 TempBuf /
												 POWERFACTOR_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Powerfactor_Data(phase_x, TempBuf);
			}
		}
		else
		{
			report_diff_data = gParaDispose_Info[phase_x].report_threshold[powerFactor][MEASURE_DATA_REPORT_THRESHOLD_03_POS] * 
												 TempBuf /
												 POWERFACTOR_REPORT_PERCENTAGE_MAGNIFICATION;
			if(para_data_diff >= report_diff_data)
			{
				X485_Device_Report_Powerfactor_Data(phase_x, TempBuf);
			}
		}
	}
	
	printf("功率因素相名:%d		值是:%d \r\n",phase_x,TempBuf);
}



/**
  * @brief  resgiter value dispose(voltage,current,power..........).
  * @param  None
  * @retval None
  */
void resgiterPara_Dispose(u32 pBuf, u8 parameter,u8 phase_x)
{
	/* energy count resgiter value*/	
	switch(parameter)
	{
		case voltage:
			Voltage_Parameter_Dispose(pBuf, phase_x);
			break;

		case current:
			Current_Paramenter_Dispose(pBuf, phase_x);
			break;
		
		case leakCurrent:
			LeakCurrent_Paramenter_Dispose(pBuf, phase_x);
			break;
		
		case power:
			Power_Paramenter_Dispose(pBuf, phase_x);
			break;
		
		case powerFactor: 
			PowerFactor_Paramenter_Dispose(pBuf, phase_x);
			break;

		case energyCount:		
			ELectricityValue_Dispose(phase_x, pBuf);
			break;
		
		default: 
			break;		
	}
}



/**
  * @brief  reading resgiter value(voltage,current,power..........).
  * @param  None
  * @retval None
  */
void V9203_resgiterRead(void)
{
	static u8 para = 1;//1
	/* average voltage resgiter value*/
	       u32 MU[3] = {0}, 
	/* average current resgiter value*/
			       MI[5] = {0}, 
	/* average power resgiter value*/			
			       MAP[3] = {0}, 
	/* average power factor resgiter value*/		
			       MAF[3] = {0},
	/* energy count resgiter value*/
			       CFP[4] = {0};
	
	switch(para)
	{
		/*----- voltage -----*/
		case 1:
				SPI2_CS_ENABLE();
				/* average voltage resgiter value A */
				MU[Phase_A] = ReadBronco(RMSUA);		
				SPI2_CS_DISABLE();
		
				printf("RMSUA:%x	",MU[Phase_A]);
		
				resgiterPara_Dispose(MU[Phase_A], voltage, Phase_A);
				para++;
				break;
	
		case 2:
				SPI2_CS_ENABLE();
				/* average voltage resgiter value B */
				MU[Phase_B] = ReadBronco(RMSUB);			
				SPI2_CS_DISABLE();
				printf("RMSUB:%x	",MU[Phase_B]);
				resgiterPara_Dispose(MU[Phase_B], voltage, Phase_B);
				para++;
				break;
		
		case 3:
				SPI2_CS_ENABLE();
				/* average voltage resgiter value C */
				MU[Phase_C] = ReadBronco(RMSUC);			
				SPI2_CS_DISABLE();
				printf("RMSUC:%x	",MU[Phase_C]);
				resgiterPara_Dispose(MU[Phase_C], voltage, Phase_C);
//				if(gUSART3_Info.transformer_flag)
//					para += 1;
//				else para = 1;
				para++;
				break;		
		/*----- current -----*/
		case 4:
				SPI2_CS_ENABLE();
				/* average current resgiter value A */
				MI[Phase_A] = ReadBronco(RMSI1A);		
				SPI2_CS_DISABLE();
				printf("RMSI1A:%x	",MI[Phase_A]);
				resgiterPara_Dispose(MI[Phase_A], current, Phase_A);
				para++;
				break;

		case 5:
				SPI2_CS_ENABLE();
				/* average current resgiter value B */
				MI[Phase_B] = ReadBronco(RMSI1B);			
				SPI2_CS_DISABLE();
				printf("RMSI1B:%x	",MI[Phase_B]);
				resgiterPara_Dispose(MI[Phase_B], current, Phase_B);
				para++;
				break;

		case 6:
				SPI2_CS_ENABLE();
				/* average current resgiter value C */
				MI[Phase_C] = ReadBronco(RMSI1C);			
				SPI2_CS_DISABLE();	
				printf("RMSI1C:%x	",MI[Phase_C]);
				resgiterPara_Dispose(MI[Phase_C], current, Phase_C);
				para++;
				break;
		case 7:
				SPI2_CS_ENABLE();
				/* average current resgiter value N */
				MI[OTHER] = ReadBronco(RMSI1N);			
				SPI2_CS_DISABLE();	
				printf("RMSI1N:%x	",MI[OTHER]);
				resgiterPara_Dispose(MI[OTHER], leakCurrent, OTHER);
				para++;
				break;
	
		/*----- power -----*/
		case 8:
				SPI2_CS_ENABLE();
				/* average power resgiter value A */
				MAP[Phase_A] = ReadBronco(DATAPA);			
				SPI2_CS_DISABLE();
				
				printf("DATAPA:%x	",MAP[Phase_A]);
				resgiterPara_Dispose(MAP[Phase_A], power, Phase_A);
				para++;
				break;
		
		case 9:
				SPI2_CS_ENABLE();
				/* average power resgiter value B */
				MAP[Phase_B] = ReadBronco(DATAPB);			
				SPI2_CS_DISABLE();
				printf("DATAPB:%x	",MAP[Phase_B]);
				resgiterPara_Dispose(MAP[Phase_B], power, Phase_B);
				para++;
				break;
		
		case 10:
				SPI2_CS_ENABLE();
				/* average power resgiter value C */
				MAP[Phase_C] = ReadBronco(DATAPC);			
				SPI2_CS_DISABLE();
				printf("DATAPC:%x	",MAP[Phase_C]);
				resgiterPara_Dispose(MAP[Phase_C], power, Phase_C);
				para++;
				break;
		
		/*----- power factor -----*/
		case 11:
				SPI2_CS_ENABLE();
				/* average power factor resgiter value A */
				MAF[Phase_A] = ReadBronco(RegMAFA);	
				SPI2_CS_DISABLE();
				printf("RegMAFA:%x	",MAF[Phase_A]);
				resgiterPara_Dispose(MAF[Phase_A], powerFactor, Phase_A);
				para++;
				break;
		
		case 12:
				SPI2_CS_ENABLE();
				/* average power factor resgiter value B */
				MAF[Phase_B] = ReadBronco(RegMAFB);			
				SPI2_CS_DISABLE();
				printf("RegMAFB:%x	",MAF[Phase_B]);
				resgiterPara_Dispose(MAF[Phase_B], powerFactor, Phase_B);
				para++;
				break;
		
		case 13:
				SPI2_CS_ENABLE();
				/* average power factor resgiter value C */
				MAF[Phase_C] = ReadBronco(RegMAFC);			
				SPI2_CS_DISABLE();
				printf("RegMAFC:%x	",MAF[Phase_C]);
				
				resgiterPara_Dispose(MAF[Phase_C], powerFactor, Phase_C);
				para++;
				break;
		
		/*----- energy -----*/				
		case 14:
				SPI2_CS_ENABLE();
				/* energy count resgiter value A */
				CFP[Phase_A] = ReadBronco(RegAP);		
//					CFP[Phase_A] += 1000;		
				SPI2_CS_DISABLE();
				
				printf("RegAP:%x	",CFP[Phase_A]);
				resgiterPara_Dispose(CFP[Phase_A], energyCount, Phase_A);
				para++;
				break;
			
			case 15:
				SPI2_CS_ENABLE();
				/* energy count resgiter value B */
				CFP[Phase_B] = ReadBronco(RegBP);	
//					CFP[Phase_B] += 1000;
				SPI2_CS_DISABLE();
				printf("RegBP:%x	",CFP[Phase_B]);
				resgiterPara_Dispose(CFP[Phase_B], energyCount, Phase_B);
				para++;
				break;
			
			case 16:
				SPI2_CS_ENABLE();
				/* energy count resgiter value C */
				CFP[Phase_C] = ReadBronco(RegCP);	
//					CFP[Phase_C] += 1000;
				SPI2_CS_DISABLE();
				printf("RegCP:%x	",CFP[Phase_C]);
				resgiterPara_Dispose(CFP[Phase_C], energyCount, Phase_C);
				para++;
				break;
			
			case 17:
	   			SPI2_CS_ENABLE();
				/* energy count resgiter value A+B+C */
			  CFP[BondPhase] = ReadBronco(RegPP);
//				CFP[BondPhase] += 1000;		
				SPI2_CS_DISABLE();
				printf("RegPP:%x	",CFP[BondPhase]);
				resgiterPara_Dispose(CFP[BondPhase], energyCount, BondPhase);
				para++;
				break;
				
			default: 
				para = 1; 
				printf("########################################################\r\n");
				break;
	}   
}

/**
  * @brief  prevent incorrect input parameters.
  * @param  None
  * @retval None
  */
u16 Over_Voltage_Parameter_Protection(u16 pBuf)
{
	u16 tempBuf = 0;
	
	
	return tempBuf;
}

/**
  * @brief  prevent incorrect input parameters.
  * @param  None
  * @retval None
  */
u16 Under_Voltage_Parameter_Protection(u16 pBuf)
{
	u16 tempBuf = 0;
	
	return tempBuf;
}

BOOLEAN Get_V9203_ParameterRead_Finish_Flag(void)
{
	return V9203_ParameterRead_Finish_Flag;
}

void Set_V9203_ParameterRead_Finish(void)
{
	V9203_ParameterRead_Finish_Flag = TRUE;
}
/******************* (C) COPYRIGHT 2020-???? ZXTEAM*****END OF FILE****/
