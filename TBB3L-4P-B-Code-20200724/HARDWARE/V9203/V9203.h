#ifndef __V9203_H
#define __V9203_H

#include "sys.h"
#include "spi.h"
#include "common.h"


//=============================================================
//Bronco模拟控制寄存器
//=============================================================
#define   RegANCtrl0   0x8000
#define   RegANCtrl1   0x8001
#define   RegANCtrl2   0x8002
#define   RegANCtrl3   0x8003
//=============================================================
//Bronco 计量配置寄存器  
//=============================================================
#define   RegMTPARA0   0xC000
#define   RegMTPARA1   0xC001
#define   RegMTPARA2   0xC002
#define   RegMTPARA3   0xC003

#define   ZZCPSEL     0xEC15
#define   ZZPA0          0xEC23
#define   ZZPA1          0xEC24
#define   ZZQA0         0xEC47
#define   ZZQA1         0xEC48
#define   ZZAPPA       0xEC05
#define   ZZPCF0A     0XEC34


//=============================================================
//Bronco 直流分量寄存器  
//=============================================================
#define   RegDCAU       0xF02C
#define   RegDCAI        0xF02D
#define   RegDCBU       0xF02E
#define   RegDCBI        0xF02F
#define   RegDCCU       0xF030
#define   RegDCCI        0xF031

//=============================================================
//Bronco 频率相位寄存器  
//=============================================================

#define   RegAFREQ      0xC008
#define   RegBFREQ      0xC009
#define   RegCFREQ      0xC00A

#define   RegBUPHA      0xC00B
#define   RegCUPHA      0xC00C
#define   RegAIPHA       0xC00D
#define   RegBIPHA       0xC00E
#define   RegCIPHA       0xC00F


//=============================================================
//Bronco 电压/电流瞬时有效值寄存器  
//=============================================================
#define   RegARTIA     0xE83E
#define   RegARTIB    0xE83F
#define   RegARTIC     0xE840

#define   RegARTUA     0xE842
#define   RegARTUB     0xE843
#define   RegARTUC     0xE844
#define   RegARTIN    0xE841

//=============================================================
//Bronco 有功/无功功率寄存器  
//=============================================================
#define   RegMAPSUM0      0xE8EE
#define   RegMAPSUM1      0xE8EF

#define   RegMAQSUM0      0xE8F3
#define   RegMAQSUM1      0xE8F4

//=============================================================
//Bronco 电压/电流有效值校正寄存器  
//=============================================================
#define   RegWARTIA     0xE968
#define   RegWARTIB     0xE969
#define   RegWARTIC     0xE96A
#define   RegWARTIN     0xE96B

#define   RegWARTUA     0xE96C
#define   RegWARTUB     0xE96D
#define   RegWARTUC     0xE96E

#define   RegAllCF      0XD008  
#define   RegAllCFN      0XD009  
//=============================================================
//Bronco 全波有功/无功功率校正寄存器  
//=============================================================
#define   RegWAPTA0      0xE95A
#define   RegWAPTB0      0xE95E
#define   RegWAPTC0      0xE962

#define   RegWAQTA      0xE965
#define   RegWAQTB      0xE966
#define   RegWAQTC      0xE967

#define  RegWAEC0      0XE954       //角差


//=============================================================
//Bronco 基波有功/无功功率校正寄存器  
//=============================================================
#define   RegWBPTA      0xE970   
#define   RegWBPTB      0xE971
#define   RegWBPTC      0xE972

#define   RegWBQTA      0xE973
#define   RegWBQTB      0xE974
#define   RegWBQTC      0xE975


//=============================================================
//Bronco 全波有功/无功功率二次补偿校正寄存器  
//=============================================================
#define   RegWWAPTA     0XE99C //0xE98E 
#define   RegWWAPTB     0XE99D //0xE98F
#define   RegWWAPTC     0XE99E //0xE990

#define   RegWWAQTA      0xE991
#define   RegWWAQTB      0xE992
#define   RegWWAQTC      0xE993


//=============================================================
//Bronco 门限值寄存器  
//=============================================================
#define   RegZZEGYTHH     0xEC1E
#define   RegZZEGYTHL     0xEC1F

#define   RegCTHH       0xE8AA
#define   RegCTHL       0xE8AB


//=============================================================
//Bronco 校验和寄存器  
//=============================================================
#define   RegCKSUM     0xC003
//=============================================================
//Bronco 电流检测门限值寄存器  
//=============================================================
#define  RegZZDCUM    0xEC1D


//=============================================================
//Bronco 全波功率因数寄存器
//=============================================================
#define RegMAFA  0xE8E4
#define RegMAFB  0xE8E5
#define RegMAFC  0xE8E6
#define RegMAFS  0xE8E7

//=============================================================
//Bronco 分相能量脉冲计数器
//=============================================================
#define RegAP  0xE89F
#define RegBP  0xE8A0
#define RegCP  0xE8A1
#define RegPP  0xE8A2

#define RegAQ  0xE8A4
#define RegBQ  0xE8A5
#define RegCQ  0xE8A6

//=============================================================
//Bronco 相序中断寄存器
//=============================================================
#define RegIRQ0  0xA000
#define RegIRQ1  0xA001
#define RegIRQFLAG 0xA002

#define APhy  0x01
#define BPhy  0x02
#define CPhy  0x03
#define ABCPhy 0x00


#define RMSUA        0xE944
#define RMSUB        0xE945
#define RMSUC        0xE946

#define RMSI1A       0xE90E
#define RMSI1B       0xE90F
#define RMSI1C       0xE910
#define RMSI1N	     0xE911

#define DATAPA         0xE8E8//A 相全波平均有功功率寄存器
#define DATAPB         0xE8E9//B 相全波平均有功功率寄存器 
#define DATAPC       	 0xE8EA//C 相全波平均有功功率寄存器 
#define DATAPABC       0xE8EE//全波合相平均有功功率 0 寄存器
#define DATAPAPP1      0xE84E//全波合相平均有功功率 1 寄存器  
#define DATAPAPP2      0xE88C//基波合相视在功率瞬时值寄存器  

#define DATAQA        0xE8F0//A 相全波平均无功功率寄存器 
#define DATAQB        0xE8F1//B
#define DATAQC        0xE8F2//C
#define DATAQABC       0xE8F3//全波合相平均无功功率 0 寄存器

#define DATAFREQA    0xC008//A 相频率寄存器
#define DATAFREQB    0xC009//B
#define DATAFREQC    0xC00A//C

#define DATAFREQ        0xC008
#define DATAAFS      		0xE814//全波合相瞬时功率因数寄存器
#define MeterConstLen    17  // 表常数列表的个数

#define VOL_AVG_LIMIT      		 0xFFFE//电压平均值次数限制
#define CUR_AVG_LIMIT      		 0x1F40//电流平均值次数限制
#define POWER_AVG_LIMIT    		 0x1F40//功率平均值次数限制
#define POWERFACTOR_AVG_LIMIT  0xFFFE//功率因素平均值次数限制
#define ELECLEAKAGE_AVG_LIMIT  0xFFFE//漏电电流平均值次数限制


typedef struct meterc 
{                                                    
  u16 ui_MC;             // 脉冲常数
  u8 uc_CFGate;          // CF脉冲门限值
  u8 uc_CFUinit;         // 每个单位能量的大小
}S_METERC; 

extern S_METERC  gs_MeterConstT[MeterConstLen]; 

typedef struct
{
    u32  RacWARTU;           //全波电压有效值比差寄存器
    u32  RacWARTI;           //全波电流有效值比差寄存器
    u32  RacWAPT;            //全波有功功率比差寄存器
    u32  RacWWAPT;           //全波有功功率二次补偿寄存器                
    u32  RacREWWAPT;         //全波无功功功率二次补偿寄存器                
}S_JBRE;

typedef struct 
{
    u16 ui_MeterC;           // 表常数
    u16 ui_Un;               // 标称电压
    u16 ui_Ib;               // 标称电流
    u16 ui_Resve1;           //保留
    
    u32  RacEGYTH;         //有功门限值寄存器
    u32  RacCTHH;          //起动/潜动判断门限值寄存器
    u32  RacCTHL;          //起动/潜动判断门限值寄存器
    u32 RacZZDCUM;       	 //电流检测门限值
    u32  RacWAEC0;         //角差0
    u32 RacMTPARA0;
    u32 RacMTPARA1;
    u32 RacMTPARA2;
    u32  RacANCtrl0;         //模拟控制寄存器0
    u32  RacANCtrl1;         //模拟控制寄存器1
    u32  RacANCtrl2;         //模拟控制寄存器2
    u32  RacANCtrl3;         //模拟控制寄存器2
    S_JBRE  gs_JBA;             //A相比差组
    S_JBRE  gs_JBB;             //B相比差组
    S_JBRE  gs_JBC;             //C相比差组
    u32 ul_PG;               //功率比例系数
    u32 ul_URmG;             //电压通道比例系数
    u32 ul_I1RmG;            //电流通道1比例系数
	u32 ul_InRmG;			 //零线通道n比例系数
    u16 ui_Resve2;           //保留
    u16 ui_JbCRC;            // 校表参数的CRC结果
}S_JBPM;

/*-----GPIO config-----*/
#define DeepSleepDisable()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define DeepSleepEnable()   GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define PM1Disable()		GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define PM1Enable()			GPIO_SetBits(GPIOB,GPIO_Pin_9)	
#define PM0Disable()		GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define PM0Enable()			GPIO_SetBits(GPIOB,GPIO_Pin_8)

/*----- phase -----*/
#define	Phase_A 			0
#define	Phase_B 			1
#define	Phase_C 			2
#define BondPhase           3
#define OTHER               4	//用于零线测量漏电流

/*-----parameter-----*/
#define voltage 			0
#define current 			1
#define power				2
#define powerFactor 		3
#define electricity			4
#define energyCount    		5
#define leakCurrent			6 
#define maxEnergyCountValue 0xFFFFFFFF
#define maxElectricityValue 0xFFFFFFFF
#define TIM_50MS            50
#define TIM_200MS           200
/*-----time multiple-----*/
//#define second(X)        X 
//#define minuter(X)       60 * X 
//#define hour(X)          3600* X 

typedef struct
{
	/*0-current A, 1-current B, 2-current C*/
	union
	{
		u8 UNIT8[4];
		u32 UINT32;
	}currentValue[3];
	/*0-voltage A, 1-voltage B, 2-voltage C */
	union
	{
		u8 UNIT8[2];
		u16 UINT16;
	}voltageValue[3];
	/*0-power A, 1-power B, 2-power C*/
	union
	{
		u8 UNIT8[4];
		u32 UINT32;
	}powerValue[3];
	/*0-powerFactor A, 1-powerFactor B, 2-powerFactor C*/
	u8 powerFactorValue[3];
	/*0-energy count A, 1-energy count B, 2-energy count C*/
	u32 energyCountValue[4];
	/*0-Electricity A, 1-Electricity B, 2-Electricity C, 4-Electricity bond phase*/
	union
	{
		u8 UNIT8[4];
		u32 UINT32;
	}electricityValue[4];
	/**/
	u32 energyCountValue_WriteAddr;
	/**/
	u8 energyCountValue_AddrOffset;
	/*choose sector1(FALSE) or sector2(TRUE)*/
//	BOOLEAN SectorChoose;
	u16 index_AddrOffset;
}V9203_TypeDef;
	
extern V9203_TypeDef gV9203_Info;
void V9203_GPIO_Init(void);

extern u8 guc_GateCF;                  // CF脉冲的分频率系数，通过查表获取
extern u8 guc_UnitCF;   
extern S_JBPM  gs_JbPm;
extern volatile u8 guc_CommDelayTime;

void V9203_Init(void);
u8 Get_V9203_init_err_flag(void);
u8 Get_V9203_init_flag(void);
void V9203_Reboot(void);

void BroncoInit(void);
void WriteBronco(u32 Data,u16 Addr);
void set_data_cmd_flash(u8 cmd, u16 dat);
u32  ReadBronco(u16 Addr);
unsigned long spi_rd_flash(unsigned int addr);

void Jbpmdefault(void);
u16 do_CRC(u8 *str,u8 len);

void Update_Bronco_Para(void);
void Eny_JbPm_MeterConst(void);

void V9203_PowerDown(void);   
void V9203_WAKEUP(void); 
#endif
