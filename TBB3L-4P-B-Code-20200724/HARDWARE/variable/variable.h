/**
	@brief   This file provides all the system parameter.
 */

#ifndef __VARIABLE_H
#define __VARIABLE_H

#include "sys.h"
#include "V9203.h"

/** 
  *@defgroup device related parameters
  */
#define FLASH_OFFEST     0x7800  //0x8007800

/** 
  *@defgroup device related parameters
  */
#define DEVICE_ID_LEN                       8           //从设备ID长度
#define DEVICE_VERSION_LEN                  4
#define DEVICE_TYPE_LEN                     2
#define DEVICE_ENCRY_MODE_LEN               2

/** 
  *@defgroup device related parameters
  */
//安全信息
#define ZX_Safe_Data_Now_Report_Flag       0x1e	 	

//设置信息全上报标志位
#define ZX_Safe_Alarm_Set_All_Report_Flag  0x350447
//报警信息全上报标志位
#define ZX_Safe_Alarm_All_Report_Flag      0x113BBB


/** 
  * @brief  test flage(include file name：TEST_file name)  
  */	
#define TEST_main                0
#define TEST_FW_Upgrade_init     0

#define TEST_X485_Upstream       0
#define TEST_FW_Upgrade_download 0

#define TEST_FW_Upgrade_ymodem   0

#define TEST_simulation          0
#define TEST_ParaDispose         0      
#define TEST_Data_process_ard    0   //显示画面选择

#define TEST_Ard                 0   //背光灯

typedef struct
{
  u8 id[DEVICE_ID_LEN];
  u8 hw_version[DEVICE_VERSION_LEN];
  u8 device_type[DEVICE_TYPE_LEN];
  u16 encry_mode; //encryption method
  u16 encry_code; //key
}Sys_TypeDef;//TBM-S3

typedef union
{
  u8 UINT8[2];
  u16 UINT16;
}ZX_Half_Word_Package;

typedef union
{
  u8 UINT8[4];
  u32 UINT32;
}ZX_Word_Package;

typedef struct
{
  ZX_Word_Package cur_over;
	ZX_Word_Package short_cur;
	ZX_Word_Package ele_leakage;
	u8 umbalances;
	
  ZX_Half_Word_Package vol[3];
	u8 cur_over_phaseX;
	u8 short_cur_phaseX;
	u8 lock;
	u8 current_lock;       
}ZX_Safe_Alarm_Data;  //安全数据


typedef struct
{
  ZX_Word_Package cur_rated;
	ZX_Word_Package ele_leakage;
	ZX_Half_Word_Package umbalances;
  ZX_Half_Word_Package vol_high;
  ZX_Half_Word_Package vol_low;
	ZX_Half_Word_Package phase_loss;
}ZX_Safe_Alarm_Set_Data_TypeDef;  //设置数据

typedef struct
{
	u8 vol[3];
	u8 cur[3];
	u8 act_power[3];
	u8 powerfactor[3];
	u8 power_consum[4];
	u8 remote_ctrl;
	u8 elec_leakage;
	u8 switch_power;
}ZX_Measure_Report_Flag_TypeDef;

typedef union
{
  struct
  {
    u32  elec_leakage:1;         //1
    u32  cur_over:1;						 //2
		u32  vol_low:1;							 //3
    u32  vol_low_A:1;						 //4
		u32  vol_low_B:1;						 //5
		u32  vol_low_C:1;						 //6
		u32  vol_high:1;						 //7
		u32  vol_high_A:1;					 //8
		u32  vol_high_B:1;					 //9
		u32  vol_high_C:1;					 //10
		u32  phase_loss:1;					 //11
		u32  phase_loss_A:1;				 //12
		u32  phase_loss_B:1;				 //13
		u32  phase_loss_C:1;				 //14
    u32  temp_high:1;				     //15
    u32  strike_fire:1;  //打火    16
    u32  short_cur:1;				     //17
    u32  vol_fault_act:1;				 //18
    u32  lock:1;				         //19
    u32  gear_fault:1;				   //20
    u32  umbalances:1;				   //21
		u32  cur_rated:1;            //22
  }oneBit;
  u32 all_bits;
}ZX_Safe_Type_Flag;

typedef struct
{
	union 
	{
		u8 value:1;
	}phase_loss[3];
	union 
	{
		u8 value:1;
	}vol_low[3];
	union 
	{
		u8 value:1;
	}vol_high[3];
	u8 cur_over;
	u8 short_cur;
	u8 umbalance;
	u8 elec_leakage;
	u8  remote_ctrl;
}ZX_Coil_Reg_Type;

extern ZX_Safe_Type_Flag    ZX_Safe_Alarm_Report_Flag;   //报警设置信息上报标志位
extern ZX_Coil_Reg_Type     ZX_Coil_Register;            //异常标志位价
extern ZX_Safe_Type_Flag    ZX_Safe_Alarm_Set_Report_Flag;   //报警设置信息上报标志位
extern ZX_Safe_Type_Flag    ZX_Alarm_Recover_Info;

extern ZX_Measure_Report_Flag_TypeDef     ZX_Measure_Report_Flag_Info;       //计量参数信息上报
extern ZX_Safe_Alarm_Data         				ZX_Safe_Alarm_Record_Data;       //报警信息实际值
extern ZX_Safe_Alarm_Set_Data_TypeDef  		ZX_Safe_Alarm_Set_Data_Info;		//设置报警值
//总值，用来计算平均值
extern ZX_Word_Package ZX_measure_vol_sum_data[3];
extern ZX_Word_Package ZX_measure_cur_sum_data[3];
extern ZX_Word_Package ZX_measure_powerfactor_sum_data[3];
extern ZX_Word_Package ZX_measure_act_power_sum_data[3];
extern ZX_Word_Package ZX_measure_elecLeakage_sum_data;
//电量
extern ZX_Word_Package ZX_measure_power_consume_report_data[4];
extern ZX_Word_Package ZX_measure_power_consume_diff_data[4];
extern ZX_Word_Package ZX_measure_Electricity_Old[4];
//漏电
extern ZX_Word_Package ZX_measure_ElecLeakage_data;
extern u8 ZX_measure_ElecLeakage_wait_time;
//电量存储标志位
//extern u8 ZX_Power_Consume_Data_Flash_Write_Flag;

//采集次数，用来计算平均值
extern u16 ZX_measure_vol_check_num[3];
extern u16 ZX_measure_cur_check_num[3];
extern u16 ZX_measure_powerfactor_check_num[3];
extern u16 ZX_measure_act_power_check_num[3];
extern u16 ZX_measure_elecLeakage_check_num;
//system
extern Sys_TypeDef  gSys_Info;

extern const u8 Device_Software_Version[4];  //设备软件版本；切记每次修改后更新
void init_variable(void);
#endif
