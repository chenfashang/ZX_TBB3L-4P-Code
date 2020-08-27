#ifndef __PARADISPOSE_H
#define __PARADISPOSE_H

#include "sys.h"
#include "V9203.h"

/*------parameter set-------*/
#define X485_DATA_REPORT_LOW_NUM           5
#define X485_DATA_REPORT_HIGH_NUM         10
#define X485_DATA_REPORT_THRESHOLD_BASE_MULTI_NUM   10  //放大10倍
#define X485_DATA_REPORT_THRESHOLD_HIGH_MULTI_NUM   30  //放大30倍
#define X485_DATA_REPORT_THRESHOLD_INTER_MULTI_NUM  5 
#define TIMER_20SEC  20
#define TIMER_1MIN   60
#define TIMER_10MIN  600 
#define TIMER_20MIN  1200
#define TIMER_30MIN  1800
#define TIMER_1HOUR  3600
#define electricLeakage 0

#define VOL_REPORT_PERCENTAGE_MAGNIFICATION      						1000  
#define CUR_REPORT_PERCENTAGE_MAGNIFICATION      						1000    //百分比放大倍数
#define POWER_REPORT_PERCENTAGE_MAGNIFICATION    						1000    //百分比放大倍数
#define POWERFACTOR_REPORT_PERCENTAGE_MAGNIFICATION         1000    //百分比放大倍数

#define MEASURE_DATA_REPORT_THRESHOLD_01_POS    0
#define MEASURE_DATA_REPORT_THRESHOLD_02_POS    1
#define MEASURE_DATA_REPORT_THRESHOLD_03_POS    2

#define X485_Slave_Power_Consum_Addr       			0x8000
#define FLASH_SECTOR_SIZE                  			0x1000
#define SLAVE_POWER_CONSUM_DATA_NUM        			16

#define MAXIMUM_REPORT_COUNT          					0xEFFFFFEF

#define ZX_MEASURE_ELECLEAKAGE_WAIT_MAX_TIME    15

/*------parameter define-----*/
typedef struct
{
	u32 report_num[5];	
	u32 not_report_time[5];
/*voltage(0),current(1),power(2),powerFactor(3)
	  15(0),        10(1),        5(2)*/
	u8 report_threshold[4][3];
}ParaDispose_TypeDef;


/*----parameter init----------*/
extern ParaDispose_TypeDef gParaDispose_Info[4];

/*------function define------*/
void ParaDispose_init(void);
void X485_Device_Para_Dispose(void);
void X485_Device_Para_Record_Time(void);

void ElectrictLeakage_Dispose(void);
	
void X485_Data_Report_Rule_Adjust(u8 phaseX, u8 parameter);

void V9203_resgiterRead(void);
void W25Q_PowerConsume_Read(void);

u16 Over_Voltage_Parameter_Protection(u16 pBuf);
u16 Under_Voltage_Parameter_Protection(u16 pBuf);

void Voltage_Parameter_Dispose(u32 pBuf, u8 phase_x);

BOOLEAN Get_V9203_ParameterRead_Finish_Flag(void);
void Set_V9203_ParameterRead_Finish(void);
#endif

/******************* (C) COPYRIGHT 2020-???? WHO IS A PIPIXIA *****END OF FILE****/
