/**
	@brief   This file provides all the system parameter.
 */
//#include "Config.h"
#include "variable.h"
 
u16 ZX_measure_vol_check_num[3];
u16 ZX_measure_cur_check_num[3];
u16 ZX_measure_powerfactor_check_num[3];
u16 ZX_measure_act_power_check_num[3];
u16 ZX_measure_elecLeakage_check_num;

ZX_Word_Package ZX_measure_vol_sum_data[3];
ZX_Word_Package ZX_measure_cur_sum_data[3];
ZX_Word_Package ZX_measure_powerfactor_sum_data[3];
ZX_Word_Package ZX_measure_act_power_sum_data[3];
ZX_Word_Package ZX_measure_elecLeakage_sum_data;

ZX_Word_Package ZX_measure_power_consume_report_data[4];
ZX_Word_Package ZX_measure_power_consume_diff_data[4];
ZX_Word_Package ZX_measure_Electricity_Old[4];
ZX_Word_Package ZX_measure_ElecLeakage_data;
u8 ZX_measure_ElecLeakage_wait_time;

//u8 ZX_Power_Consume_Data_Flash_Write_Flag; 

ZX_Safe_Alarm_Data  ZX_Safe_Alarm_Record_Data;

ZX_Safe_Type_Flag ZX_Safe_Alarm_Set_Report_Flag;
ZX_Safe_Type_Flag ZX_Safe_Alarm_Report_Flag;
ZX_Coil_Reg_Type  ZX_Coil_Register;
ZX_Safe_Type_Flag ZX_Alarm_Recover_Info;

ZX_Safe_Alarm_Set_Data_TypeDef ZX_Safe_Alarm_Set_Data_Info;		
     
ZX_Measure_Report_Flag_TypeDef ZX_Measure_Report_Flag_Info;

Sys_TypeDef  gSys_Info;

const u8 Device_Software_Version[4] = {1,1,1,0};  //Èí¼þ°æ±¾ºÅ;

void init_variable(void)
{
	ZX_Safe_Alarm_Set_Report_Flag.all_bits = 0;

	/*average check number*/
	ZX_measure_vol_check_num[Phase_A] = 0;
	ZX_measure_vol_check_num[Phase_B] = 0;
	ZX_measure_vol_check_num[Phase_C] = 0;
	
	ZX_measure_cur_check_num[Phase_A] = 0;
	ZX_measure_cur_check_num[Phase_B] = 0;
	ZX_measure_cur_check_num[Phase_C] = 0;
	
	ZX_measure_powerfactor_check_num[Phase_A] = 0;
	ZX_measure_powerfactor_check_num[Phase_B] = 0;
	ZX_measure_powerfactor_check_num[Phase_C] = 0;
	
	ZX_measure_act_power_check_num[Phase_A] = 0;
	ZX_measure_act_power_check_num[Phase_B] = 0;
	ZX_measure_act_power_check_num[Phase_C] = 0;
	
	ZX_measure_elecLeakage_check_num = 0;
	/**/
	ZX_measure_vol_sum_data[Phase_A].UINT32 = 0;
	ZX_measure_vol_sum_data[Phase_B].UINT32 = 0;
	ZX_measure_vol_sum_data[Phase_C].UINT32 = 0;
	
	ZX_measure_cur_sum_data[Phase_A].UINT32 = 0;
	ZX_measure_cur_sum_data[Phase_B].UINT32 = 0;
	ZX_measure_cur_sum_data[Phase_C].UINT32 = 0;
	
	ZX_measure_powerfactor_sum_data[Phase_A].UINT32 = 0;
	ZX_measure_powerfactor_sum_data[Phase_B].UINT32 = 0;
	ZX_measure_powerfactor_sum_data[Phase_C].UINT32 = 0;
	
	ZX_measure_act_power_sum_data[Phase_A].UINT32 = 0;
	ZX_measure_act_power_sum_data[Phase_B].UINT32 = 0;
	ZX_measure_act_power_sum_data[Phase_C].UINT32 = 0;
	
	ZX_measure_elecLeakage_sum_data.UINT32 = 0;
	
	ZX_measure_power_consume_report_data[Phase_A].UINT32 = 0;
	ZX_measure_power_consume_report_data[Phase_B].UINT32 = 0;
	ZX_measure_power_consume_report_data[Phase_C].UINT32 = 0;
	ZX_measure_power_consume_report_data[BondPhase].UINT32 = 0;
	
	ZX_measure_Electricity_Old[Phase_A].UINT32 = 0;
	ZX_measure_Electricity_Old[Phase_B].UINT32 = 0;
	ZX_measure_Electricity_Old[Phase_C].UINT32 = 0;
	ZX_measure_Electricity_Old[BondPhase].UINT32 = 0;
	
	ZX_measure_ElecLeakage_data.UINT32 = 0;
	ZX_measure_ElecLeakage_wait_time = 0;
//	ZX_Power_Consume_Data_Flash_Write_Flag = 0;
	
	/*alarm info*/
	ZX_Safe_Alarm_Record_Data.current_lock = 0;
	ZX_Safe_Alarm_Record_Data.lock = 0;
	ZX_Safe_Alarm_Record_Data.umbalances = 0;
	ZX_Safe_Alarm_Record_Data.cur_over.UINT32 = 0;
	ZX_Safe_Alarm_Record_Data.cur_over_phaseX = 5;
	ZX_Safe_Alarm_Record_Data.ele_leakage.UINT32 = 0;
	ZX_Safe_Alarm_Record_Data.short_cur.UINT32 = 0;
	ZX_Safe_Alarm_Record_Data.short_cur_phaseX = 0;
	ZX_Safe_Alarm_Record_Data.vol[Phase_A].UINT16 = 0;
	ZX_Safe_Alarm_Record_Data.vol[Phase_B].UINT16 = 0;
	ZX_Safe_Alarm_Record_Data.vol[Phase_C].UINT16 = 0;	
	/**/
	ZX_Safe_Alarm_Report_Flag.all_bits = 0;
	/*coil*/
	ZX_Coil_Register.cur_over = 0;
	ZX_Coil_Register.elec_leakage = 0;
	ZX_Coil_Register.phase_loss[Phase_A].value = 0;
	ZX_Coil_Register.phase_loss[Phase_B].value = 0;
	ZX_Coil_Register.phase_loss[Phase_C].value = 0;
	ZX_Coil_Register.remote_ctrl = 0;
	ZX_Coil_Register.short_cur = 0;
	ZX_Coil_Register.umbalance = 0;
	ZX_Coil_Register.vol_high[Phase_A].value = 0;
	ZX_Coil_Register.vol_high[Phase_B].value = 0;
	ZX_Coil_Register.vol_high[Phase_C].value = 0;
	ZX_Coil_Register.vol_low[Phase_A].value = 0;
	ZX_Coil_Register.vol_low[Phase_B].value = 0;
	ZX_Coil_Register.vol_low[Phase_C].value = 0;
	/**/
	ZX_Alarm_Recover_Info.all_bits = 0;
	/**/
	ZX_Safe_Alarm_Set_Data_Info.cur_rated.UINT32 = 0;
	ZX_Safe_Alarm_Set_Data_Info.ele_leakage.UINT32 = 0;
	ZX_Safe_Alarm_Set_Data_Info.phase_loss.UINT16 = 0;
	ZX_Safe_Alarm_Set_Data_Info.umbalances.UINT16 = 0;
	ZX_Safe_Alarm_Set_Data_Info.vol_high.UINT16 = 0;
	ZX_Safe_Alarm_Set_Data_Info.vol_low.UINT16 = 0;
	/**/
	ZX_Measure_Report_Flag_Info.elec_leakage = 0;
//	ZX_Measure_Report_Flag_Info.switch_power = 1;
	ZX_Measure_Report_Flag_Info.remote_ctrl = 0;
	ZX_Measure_Report_Flag_Info.cur[Phase_A] = 0;
	ZX_Measure_Report_Flag_Info.cur[Phase_B] = 0;
	ZX_Measure_Report_Flag_Info.cur[Phase_C] = 0;
	ZX_Measure_Report_Flag_Info.vol[Phase_A] = 0;
	ZX_Measure_Report_Flag_Info.vol[Phase_B] = 0;
	ZX_Measure_Report_Flag_Info.vol[Phase_C] = 0;
	ZX_Measure_Report_Flag_Info.act_power[Phase_A] = 0;
	ZX_Measure_Report_Flag_Info.act_power[Phase_B] = 0;
	ZX_Measure_Report_Flag_Info.act_power[Phase_C] = 0;
	ZX_Measure_Report_Flag_Info.powerfactor[Phase_A] = 0;
	ZX_Measure_Report_Flag_Info.powerfactor[Phase_B] = 0;
	ZX_Measure_Report_Flag_Info.powerfactor[Phase_C] = 0;
	ZX_Measure_Report_Flag_Info.power_consum[Phase_A] = 0;
	ZX_Measure_Report_Flag_Info.power_consum[Phase_B] = 0;
	ZX_Measure_Report_Flag_Info.power_consum[Phase_C] = 0;
}

