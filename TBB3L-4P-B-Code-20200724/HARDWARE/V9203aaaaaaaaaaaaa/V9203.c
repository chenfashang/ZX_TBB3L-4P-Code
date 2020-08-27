#include "V9203.h"

//#include "Config.h"

static unsigned char tdo_m,tdo_c;
static unsigned int  tdo_d;
static unsigned char spi_err;
u8 guc_GateCF;                  // CF脉冲的分频率系数，通过查表获取
u8 guc_UnitCF;   
S_JBPM  gs_JbPm;
volatile u8 guc_CommDelayTime;
S_METERC  gs_MeterConstT[MeterConstLen];

V9203_TypeDef gV9203_Info;
EnergyCountValueBuffer  EnergyCountValueTemp;

S_METERC  gs_MeterConstT[MeterConstLen] =      //根据表常数，确定CF的分频系数和每个单位能量的大小
{
  {12000,   12, 1},           // 表常数是12000，则对CF进行12分频，输出的能量大小是0.001 x1
  {10000,   10, 1},           // 表常数是10000，则对CF进行10分频，输出的能量大小是0.001 x1
  {8000,    8,  1},           // 表常数是8000，则对CF进行8分频，输出的能量大小是0.001 x1
  {5000,    5,  1},  
  {3000,    3,  1},  
  {2000,    2,  1}, 
  {1000,    1,  1},  
  {6400,    64, 10},          // 表常数是6400，则对CF进行64分频，输出的能量大小是0.001  x10
  {3200,    32, 10}, 
  {2400,    24, 10},
  {1600,    16, 10}, 
  {1200,    12, 10},
  {600, 6,  10}, 
  {500, 5,  10}, 
  {400, 4,  10}, 
  {200, 2,  10}, 
  {100, 1,  10}, 
};


const u16 CrcTab[256]={ // X16+X12+X5+1 余式表
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};  

//V9203的相关引脚设置
void V9203_GPIO_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_5;	//PM0,PM1,SLEEP				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//ZX0,ZX1,ZX2,IRQ1				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//IRQ0				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DeepSleepDisable();
	PM1Enable();
	PM0Enable();
}

void V9203_Init(void)
{	
	SPI2_CS_ENABLE()
    EnyB_SetMeterRule();	
	SPI2_CS_DISABLE()  
	delay_ms(200); 	//延时200ms
}


void EnyB_SetMeterRule(void)
{
    BroncoInit();
    Jbpmdefault();
		UpdateBroncoPara();
	
    gs_JbPm.ui_MeterC = 400;
    Eny_JbPm_MeterConst();
    WriteBronco(0, RegAP);
    WriteBronco(0, RegBP);
    WriteBronco(0, RegCP);
    WriteBronco(0, RegAQ);
    WriteBronco(0, RegBQ);
    WriteBronco(0, RegCQ);
}
//--------------------------------------------------------------------------
void BroncoInit(void)
{
		u8 i = 0;
    static u32 ready = 0;

    while(ready!=0x100000ff)
		{
			WriteBronco(0x100000ff,0xc000);
			ready=ReadBronco(0xc000);
    }
		/*DataSheet P118*/
    for(;i<56;i++)
    {
			WriteBronco(0,(0xC800+i));
    } 
}

void  WriteBronco(u32 Data,u16 Addr)
{
	u16 dat_h = Data>>16;
	u16 dat_l = Data;
	
	set_data_cmd_flash(0x08,dat_l);
	set_data_cmd_flash(0x0a,dat_h);
	set_data_cmd_flash(0x0c,Addr );
}

void set_data_cmd_flash(u8 cmd, u16 dat)
{
	u8 cksum,cmdb;
	u16 send_dat;

	cmdb = (0x3f&cmd)|0x80;
	cksum = ~((dat&0x00ff) + (dat>>8) + cmdb);
	send_dat=dat;
	//tdo_m = SPIx_ReadWriteByte(SPI3, cmdb);
	tdo_m =	SPI2_ReadWriteByte(cmdb);
	//tdo_d = SPIx_ReadWriteByte(SPI3, send_dat>>8);
	tdo_d = SPI2_ReadWriteByte(send_dat>>8);
	tdo_d = tdo_d<<8;
	//tdo_d += SPIx_ReadWriteByte(SPI3, send_dat);
	tdo_d += SPI2_ReadWriteByte(send_dat);
	//tdo_c = SPIx_ReadWriteByte(SPI3, cksum);
	tdo_c = SPI2_ReadWriteByte(cksum);
} 

u32  ReadBronco(u16 Addr)
{
    return spi_rd_flash(Addr);
}

unsigned long spi_rd_flash(unsigned int addr)
{
    unsigned long mt_dout;

    set_data_cmd_flash(0x10, addr); 
    set_data_cmd_flash(0x12, addr);
    mt_dout = (unsigned long)tdo_d;
    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
		{
        spi_err = 1;
    }
		else
		{
        spi_err = 0;
		}
    set_data_cmd_flash(0x14, addr);
    mt_dout += (((unsigned long)tdo_d)<<16);
    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
		{
        spi_err = 1;
    }
		else
		{
        spi_err = 0;
    }
    
    return mt_dout;
}
//----------------------------------------------------------------
void JbpmInit(u8 stat)
{
    if(stat == JBINI)
    {
        Jbpmdefault();
    }
    else
    {
         if(gs_JbPm.ui_JbCRC == do_CRC((u8 *)&gs_JbPm,sizeof(S_JBPM)-2))
         {
              UpdateBroncoPara();
         }
         else
         {
              if(gs_JbPm.ui_JbCRC == do_CRC((u8 *)&gs_JbPm,sizeof(S_JBPM)-2))
              {
                   UpdateBroncoPara();
              }
              else
              {     
                   if(gs_JbPm.ui_JbCRC == do_CRC((u8 *)&gs_JbPm,sizeof(S_JBPM)-2))
                   {
                       UpdateBroncoPara();
                   }
                   else
                   {
                       Jbpmdefault();
                       UpdateBroncoPara();
                   }
              }
         }
    } 
}

void Jbpmdefault(void)
{
    gs_JbPm.ui_MeterC=400; //6400;                 // 表常数
    gs_JbPm.ui_Un=2200;                           // 标称电压
    gs_JbPm.ui_Ib=25000;  //1500;                  // 标称电流
    gs_JbPm.ui_Resve1=0;
            
    gs_JbPm.RacEGYTH   = 0xD62FC3;                 //0xd2ace2;能量累加门限值
    gs_JbPm.RacCTHH= 0x000221E5;      //起动，潜动门限值
    gs_JbPm.RacCTHL= 0x0001EB4E;      // 起动，潜动门限值
    gs_JbPm.RacZZDCUM =  0xfff00000;       //0x0134 电流检测门限值
    gs_JbPm.RacWAEC0 = 0x00;        //角差
            
    gs_JbPm.RacMTPARA0 = 0x000000ff;
    gs_JbPm.RacMTPARA1 = 0x00000000;
    gs_JbPm.RacMTPARA2 = 0x030080ff;//0x00000000开启CF能量计数
    gs_JbPm.RacANCtrl0 = 0x00001111;  //Current 4倍   
    gs_JbPm.RacANCtrl1 = 0x00000000;      
    gs_JbPm.RacANCtrl2 = 0x7F005400;      //0x77005400;  0xF7005400;  
    gs_JbPm.RacANCtrl3 = 0x00000406;      //0x00000406; 
    gs_JbPm.gs_JBA.RacWARTU = 0x0 ;  //全波电压有效值比差寄存器0xFC9A0D98; 0xFD51F4B2
    gs_JbPm.gs_JBA.RacWARTI = 0x0;  //全波电流有效值比差寄存器0x21A8301B  0x3206B3A0
    gs_JbPm.gs_JBA.RacWAPT = 0x0;   //全波有功功率比差寄存器 0xEBA74B27 0x21E51894 0xD9A37181
    gs_JbPm.gs_JBA.RacWWAPT = 0x00000000;  //全波有功功率二次补偿寄存器
    gs_JbPm.gs_JBA.RacREWWAPT = 0x00000000;  //全波无功功功率二次补偿寄存器 
            
    gs_JbPm.gs_JBB.RacWARTU = 0x0;  //全波电压有效值比差寄存器0xFD6F2E2F
    gs_JbPm.gs_JBB.RacWARTI = 0x0;  //全波电流有效值比差寄存器0xE4913EB 0x3206B3A0
    gs_JbPm.gs_JBB.RacWAPT = 0X0;   //全波有功功率比差寄存器0xECC04599 0xF5DC2F3 0xD9E40780
    gs_JbPm.gs_JBB.RacWWAPT = 0x00000000;  //全波有功功率二次补偿寄存器
    gs_JbPm.gs_JBB.RacREWWAPT = 0x00000000;  //全波无功功功率二次补偿寄存器   
            
    gs_JbPm.gs_JBC.RacWARTU = 0x0;  //全波电压有效值比差寄存器0xFD0996B1
    gs_JbPm.gs_JBC.RacWARTI = 0x0;  //全波电流有效值比差寄存器0x3206B3A0
    gs_JbPm.gs_JBC.RacWAPT = 0X0;   //全波有功功率比差寄存器0xEC4A811B 0xD943CFE0
    gs_JbPm.gs_JBC.RacWWAPT = 0x00000000;  //全波有功功率二次补偿寄存器
    gs_JbPm.gs_JBC.RacREWWAPT = 0x00000000;  //全波无功功功率二次补偿寄存器     
                
    gs_JbPm.ui_Resve2=0;
    gs_JbPm.ul_PG = 25;                    //功率比例系数
    gs_JbPm.ul_URmG = 20830;             //电压通道比例系数
    gs_JbPm.ul_I1RmG = 10000;            //电流通道比例系数,
            
    gs_JbPm.ui_JbCRC=do_CRC((u8*)&gs_JbPm,sizeof(S_JBPM)-2);     // 校表参数的CRC结果
}

u16 do_CRC(u8 *str,u8 len)
{
    u8 ucData;
    u16 crc;

    crc = 0x5555;

    for( ;len > 0; len--)
    {
        ucData = (u8) (crc / 0x100);    //暂存crc高8位
        crc <<= 8;
        crc ^= CrcTab[ucData ^ (*str)];     //高字节和当前数据XOR再查表
        str++;
    }
    return crc;
}

void UpdateBroncoPara(void)
{
    u32 sum = 0;
    u8 ok;
	
label :  WriteBronco(0xaa000000,RegMTPARA0); //spi_wr_flash(0xc000,0xaa00,0x0000);  //MT_PARA0          2.27 add 防CF闪
    delay_ms(25);
	
    WriteBronco(0x000000ff,RegMTPARA0);//spi_wr_flash(0xC000,0x0000,0x00ff);    //MT_PARA0
    sum+=0x000000ff;
    WriteBronco(gs_JbPm.RacMTPARA1,RegMTPARA1); //spi_wr_flash(0xC001,0x0004,0x0404);   //MT_PARA1
    sum+= gs_JbPm.RacMTPARA1;
    WriteBronco(gs_JbPm.RacMTPARA2,RegMTPARA2); //spi_wr_flash(0xC002,0x0701,0x80ff);   //MT_PARA2 -不分段，分2次发32bit，DMA开UA，非didt，AD全开
    sum+=gs_JbPm.RacMTPARA2;
    WriteBronco(0x00000007,ZZAPPA); //spi_wr_flash(0xEC05,  0x0000,0x0007);  //全波/基波合相视在功率组合     A+B+C 
    sum+=0x00000007; 
    WriteBronco(0x00000001,ZZCPSEL); //spi_wr_flash(0xEC15, 0x0000,0x0001);  //启动潜动模式选择            根据电流有效值
    sum+=0x00000001;
    WriteBronco(gs_JbPm.RacANCtrl0,RegANCtrl0); //spi_wr_flash(0x8000,0x000f,0x1111);   //模拟控制寄存器0       0x000f1111      电压2倍增益，电流4倍增益
    sum+=gs_JbPm.RacANCtrl0;
    WriteBronco(gs_JbPm.RacANCtrl1,RegANCtrl1); //spi_wr_flash(0x8001,0x0000,0x0000);   //模拟控制寄存器1
    sum+=gs_JbPm.RacANCtrl1;
    WriteBronco(gs_JbPm.RacANCtrl2,RegANCtrl2); //spi_wr_flash(0x8002,0x7700,0x5440);   //模拟控制寄存器2   0x77005440
    sum+=gs_JbPm.RacANCtrl2;
    WriteBronco(gs_JbPm.RacANCtrl3,RegANCtrl3); //spi_wr_flash(0x8003,0x0000,0x0406);   //模拟控制寄存器3 //降频率0x000d0406
    sum+=gs_JbPm.RacANCtrl3;
    WriteBronco( gs_JbPm.RacEGYTH,RegZZEGYTHL);  //spi_wr_flash(0xEC1F,0x2C7B,0xDF00);  //门限低位
    sum+= gs_JbPm.RacEGYTH;
    WriteBronco(0x00000000,RegZZEGYTHH); //spi_wr_flash(0xEC1E,0x0000,0x0000);  //门限高位
    sum+=0x00000000;
    WriteBronco(gs_JbPm.RacCTHH,RegCTHH); //spi_wr_flash(0xE8AA,    0x0002,0x21E5);                   //启动门限值上限
    sum+=gs_JbPm.RacCTHH;
    WriteBronco(gs_JbPm.RacCTHL,RegCTHL); //spi_wr_flash(0xE8AB,    0x0001,0xEB4E);                   //启动门限值下限
    sum+=gs_JbPm.RacCTHL;
    
    WriteBronco(gs_JbPm.gs_JBA.RacWAPT,RegWAPTA0); //spi_wr_flash(0xE95A,0x0000,0x0000);    //有功A比差0    
    sum+=gs_JbPm.gs_JBA.RacWAPT;
    WriteBronco(gs_JbPm.gs_JBB.RacWAPT,RegWAPTB0); //spi_wr_flash(0xE95E,0x0000,0x0000);  //有功B比差0
    sum += (gs_JbPm.gs_JBB.RacWAPT);
    WriteBronco(gs_JbPm.gs_JBC.RacWAPT,RegWAPTC0); //spi_wr_flash(0xE962,0x0000,0x0000);    //有功C比差0  
    sum+=gs_JbPm.gs_JBC.RacWAPT;
   WriteBronco(gs_JbPm.gs_JBA.RacWAPT,RegWAQTA); //spi_wr_flash(0xE965,0x0000,0x0000);  //无功A比差
   sum+=gs_JbPm.gs_JBA.RacWAPT;
   WriteBronco(gs_JbPm.gs_JBB.RacWAPT,RegWAQTB); //spi_wr_flash(0xE966,0x0000,0x0000);  //无功B比差
   sum+=gs_JbPm.gs_JBB.RacWAPT;
   WriteBronco(gs_JbPm.gs_JBC.RacWAPT,RegWAQTC); //spi_wr_flash(0xE967,0x0000,0x0000);  //无功C比差
   sum+=gs_JbPm.gs_JBC.RacWAPT;

   WriteBronco( gs_JbPm.gs_JBA.RacWARTI,RegWARTIA);     //A电流有效值比差
   sum+=gs_JbPm.gs_JBA.RacWARTI;
   WriteBronco( gs_JbPm.gs_JBB.RacWARTI,RegWARTIB);     //B电流有效值比差
   sum+=gs_JbPm.gs_JBB.RacWARTI;
   WriteBronco( gs_JbPm.gs_JBC.RacWARTI,RegWARTIC);     //C电流有效值比差
   sum+=gs_JbPm.gs_JBC.RacWARTI;
   WriteBronco( 0X000,RegWARTIN);   //N电流有效值比差

   sum+=0x00;
    WriteBronco( gs_JbPm.gs_JBA.RacWARTU,RegWARTUA);    //A电压有效值比差
   sum+=gs_JbPm.gs_JBA.RacWARTU;
   WriteBronco( gs_JbPm.gs_JBB.RacWARTU,RegWARTUB);     //B电压有效值比差
   sum+=gs_JbPm.gs_JBB.RacWARTU;
   WriteBronco(gs_JbPm.gs_JBC.RacWARTU,RegWARTUC);  //C电压有效值比差
   sum+=gs_JbPm.gs_JBC.RacWARTU;
   WriteBronco(0x00000000,RegWBPTA); //spi_wr_flash(0xE970,0x0000,0x0000);  //基波有功A比差0    
   sum+=0x00000000;
   WriteBronco(0x00000000,RegWBPTB); //spi_wr_flash(0xE971,0x0000,0x0000);  //基波有功B比差0
   sum+=0x00000000;
   WriteBronco(0x00000000,RegWBPTC); //spi_wr_flash(0xE972,0x0000,0x0000);  //基波有功C比差0  
   sum+=0x00000000;
   WriteBronco(0x00000000,RegWBQTA); // spi_wr_flash(0xE973,0x0000,0x0000);  //基波无功A比差
   sum+=0x00000000;
   WriteBronco(0x00000000,RegWBQTB); //spi_wr_flash(0xE974,0x0000,0x0000);  //基波无功B比差
   sum+=0x00000000;
   WriteBronco(0x00000000,RegWBQTC); //spi_wr_flash(0xE975,0x0000,0x0000);  //基波无功C比差
   sum+=0x00000000;
   WriteBronco(gs_JbPm.RacWAEC0,RegWAEC0); //spi_wr_flash(0xE954,0x0082,0x8282);    //角差
   sum+=gs_JbPm.RacWAEC0;
   WriteBronco(gs_JbPm.RacZZDCUM,RegZZDCUM);  //spi_wr_flash(0xEC1D,0xfff0,0x0000); //UM通道直流分量  预设负数值 电流检测中断
   sum+=gs_JbPm.RacZZDCUM;
	 WriteBronco(0x0000002A,0xEC23); //spi_wr_flash(0xEC23,0x0000,0x0015); //有功和相0配置
   sum+=0x2A;
   WriteBronco(0x0000002A,0xEC24); //spi_wr_flash(0xEC24,0x0000,0x002A);    //有功和相1配置
   sum+=0x2a;
    WriteBronco(0x0000002A,0xEC47); //spi_wr_flash(0xEC47,0x0000,0x0015);   //无功和相0配置
   sum+=0x2A;
    WriteBronco(0x0000002A,0xEC48);  //spi_wr_flash(0xEC48,0x0000,0x002A);  //无功和相1配置
   sum+=0x2a;

   WriteBronco(0x00002211,0xEC34); //spi_wr_flash(0xEC34,0x0000,0x2211);    //高速CF来源选择
   sum+=0x2211;
   WriteBronco(0x00008000,0xA000); //spi_wr_flash(0xa000,0x0000,0x8000);    //中断0
   sum+=0x8000;
   WriteBronco(0x00008000,0xA001); //spi_wr_flash(0xa001,0x0000,0x8000);    //中断1
   sum+=0x8000;
   sum=0xffffffff-sum;
   WriteBronco(sum,0xC003); //spi_wr_flash(0xC003,(sum&0xffff0000)>>16,(sum&0x0000ffff));   //mt_para3 自检和  SUM+x=0xffffffff
	delay_ms(100);
   WriteBronco(0x00000000,0xa002); //spi_wr_flash(0xa002,0,0);   //中断标志清零  
	 WriteBronco(0x00000000,0xa002);
   ok = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11);
   if(ok != RESET)
   {
     goto label;
   }
}

void Eny_JbPm_MeterConst(void)
{
    u8 i;
    for (i = 0; i < MeterConstLen; i++)                                // 根据脉冲才常数实现走字控制
    {                                                           // 根据表常数，确定CF脉冲分频系数和单位能量大小
        if (gs_MeterConstT[i].ui_MC == gs_JbPm.ui_MeterC)
        {                                                       // 根据脉冲常数来确定CF分频系数和单位能量大小
            guc_GateCF = gs_MeterConstT[i].uc_CFGate;           // 出的脉冲个数
            guc_UnitCF = gs_MeterConstT[i].uc_CFUinit;          // 单位大小
            break;
        }
    }

    if (i >= MeterConstLen)                                     // 查找表格中，发现没有对应的常数的处理
    {                                                           // 如果表格中没有对应项，则设置状态，报警
        guc_GateCF = 10;                                        // 用一个比较大的数据，容易看出
        guc_UnitCF = 1;
    }
}


//-------------------------------------------------------------------------------------------------------------------------//
#if 1
void ELectricityValue_Save(void)
{
	static u32 oldEnergyCountValue[4] = {0};
	u8 bufferTemp[32] = {0}, i = 0, phaseX = 0;
	u32 StartAddr[2] = {0x008000,0x009000}, StartAddrTemp = 0;
	BOOLEAN sectorChange = FALSE; 
	
	if(gV9203_Info.energyCountValue_AddrOffset >= 4)//123
	{
			gV9203_Info.energyCountValue_AddrOffset = 0;	
			/*sector1*/
			if(gV9203_Info.SectorChoose)
				gV9203_Info.SectorChoose = FALSE;
			/*sector2*/
			else gV9203_Info.SectorChoose = TRUE;
			sectorChange = TRUE;	
	}
	
	if(((gV9203_Info.energyCountValue[Phase_A]  > oldEnergyCountValue[Phase_A]) || (gV9203_Info.energyCountValue[Phase_B]  > oldEnergyCountValue[Phase_B]) ||\
		 (gV9203_Info.energyCountValue[Phase_C]  > oldEnergyCountValue[Phase_C]) || (gV9203_Info.energyCountValue[BondPhase]  > oldEnergyCountValue[BondPhase]))\
	   && gV9203_Info.energyCountSaveTime >= 4)//minuter(10)
	{
		for(; phaseX < 4; phaseX++)
		{
			EnergyCountValueTemp.allBits = gV9203_Info.energyCountValue[phaseX];
			
			for(i = 0; i < 8; i++)
				bufferTemp[i+(phaseX<<3)] = EnergyCountValueTemp.oneBit.temp[7-i];
		}    
		if(gV9203_Info.SectorChoose)
			StartAddrTemp = StartAddr[1];
		else StartAddrTemp = StartAddr[0];
		gV9203_Info.energyCountValue_WriteAddr = StartAddrTemp + (gV9203_Info.energyCountValue_AddrOffset << 5) + 0x7D;
		//SPI_Flash_Write(gV9203_Info.energyCountValue_WriteAddr, bufferTemp, 32);
		
		bufferTemp[0] = gV9203_Info.energyCountValue_AddrOffset;
		//SPI_Flash_Write(StartAddrTemp + gV9203_Info.energyCountValue_AddrOffset, bufferTemp, 1);
		gV9203_Info.energyCountValue_AddrOffset += 1;			
	}
	
//	if(sectorChange)
//	{
//		if(gV9203_Info.SectorChoose)
//			//SPI_Flash_Erase_Sector_V9203(8);
//		else
//			//SPI_Flash_Erase_Sector_V9203(9);
//		sectorChange = FALSE;
//	}	
}

void ELectricityValue_Dispose(u8 phase_x, u32 pBuf)
{
	static u32 oldEnergyCountValue[4] = {0};
	u32 newEnergyCountValue = 0, energyCountValue_Temp = 0;
	u8 phaseX = 0;
	
	newEnergyCountValue = pBuf;	
	/* gV9203_Info.energyCountValue[phase] = 8 = 0.01kwh，electricity precision 0.1kwh*/
	if(oldEnergyCountValue[phase_x] > newEnergyCountValue)
		energyCountValue_Temp = maxEnergyCountValue - oldEnergyCountValue[phase_x] + newEnergyCountValue;	
	else 
		energyCountValue_Temp = newEnergyCountValue - oldEnergyCountValue[phase_x];

	/* energyCountValue accumulation */
	gV9203_Info.energyCountValue[phase_x] += energyCountValue_Temp;
	/* precision 0.1kwh (energyCountValue_Temp / 8 / 100 * 10)*/
	gV9203_Info.electricityValue[phase_x] = gV9203_Info.energyCountValue[phase_x] / 80;	
	/*clear buffer*/
	if((phase_x == Phase_A) || (phase_x == Phase_B) || (phase_x == Phase_C))
	{
		if(gV9203_Info.electricityValue[phase_x] >= maxElectricityValue)
		{
			gV9203_Info.electricityValue[phase_x] = 0;
			gV9203_Info.energyCountValue[phase_x] = 0;		
		}
	}
	if(gV9203_Info.electricityValue[BondPhase] >= 0x2FFFFFFFD)
	{
		gV9203_Info.electricityValue[phase_x] = 0;
		gV9203_Info.energyCountValue[phase_x] = 0;		
	}		

	oldEnergyCountValue[phaseX] = pBuf;	
}
#endif

/* resgiter value dispose*/
void resgiterPara_Dispose(u32 pBuf, u8 parameter,u8 phase_x)
{
	/* energy count resgiter value*/
	u32 CFP[4] = {0};
	u8 i = 0;
	
	switch(parameter)
	{
		case voltage:
			gV9203_Info.voltageValue[phase_x] = pBuf / gs_JbPm.ul_URmG;//0.1
			break;
	
		case current:
			gV9203_Info.currentValue[phase_x] = pBuf / gs_JbPm.ul_I1RmG;//0.01
			break;
		
		case power:
			gV9203_Info.powerValue[phase_x] = (u32)(pBuf * 10 / gs_JbPm.ul_PG);//0.1
			break;
		
		case powerFactor: 
			if((pBuf / pow(2,31)) >= 1)
				gV9203_Info.powerFactorValue[phase_x] = 100;//0.01
			else 
				gV9203_Info.powerFactorValue[phase_x] = (u16)(pBuf / pow(2,31) * 100);
			break;

		case energyCount:		
			#if 0
			if(gV9203_Info.energyCountSaveTime >= 4)//minuter(10)
			{
				for(i = Phase_A; i <= BondPhase; i++)
				{
					if(i != phase_x)
						CFP[i] = ReadBronco(0xE89F+i);
				}
				for(i = 0; i < 4; i++)
					ELectricityValue_Dispose(i, CFP[i]);
				ELectricityValue_Save();
				gV9203_Info.energyCountSaveTime = 0;
			}
			else ELectricityValue_Dispose(phase_x, pBuf);
			#endif
			#if 1
			if(gV9203_Info.energyCountSaveTime >= 4)//minuter(10) TIMER_10MIN
			{
				for(i = Phase_A; i <= BondPhase; i++)
				{
					 CFP[Phase_A+i] = pBuf;
				}
				for(i = 0; i < 4; i++)
					ELectricityValue_Dispose(i, CFP[i]);
				ELectricityValue_Save();
				gV9203_Info.energyCountSaveTime = 0;
			}
			else ELectricityValue_Dispose(phase_x, pBuf);
			#endif
			break;
		
		default: 
			break;		
	}
}

/* reading resgiter value */
void V9203_resgiterPara()
{
	static u8 para = 13;
	/* average voltage resgiter value*/
	static u32 MU[3] = {0}, 
	/* average current resgiter value*/
				     MI[3] = {0}, 
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
				MU[Phase_A] = ReadBronco(0xE944);	
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MU[0], voltage, Phase_A);
				para += 1;
				break;
		
		case 2:
				SPI2_CS_ENABLE();
				/* average voltage resgiter value B */
				MU[Phase_B] = ReadBronco(0xE945);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MU[1], voltage, Phase_B);
				para += 1;
				break;
		
		case 3:
				SPI2_CS_ENABLE();
				/* average voltage resgiter value C */
				MU[Phase_C] = ReadBronco(0xE946);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MU[2], voltage, Phase_C);
				para += 1;
				break;		
			
		/*----- current -----*/
		case 4:
				SPI2_CS_ENABLE();
				/* average current resgiter value A */
				MI[Phase_A] = ReadBronco(0xE90E);		
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MI[Phase_A], current, Phase_A);
				para += 1;
				break;

		case 5:
				SPI2_CS_ENABLE();
				/* average current resgiter value B */
				MI[Phase_B] = ReadBronco(0xE90F);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MI[Phase_B], current, Phase_B);
				para += 1;
				break;

		case 6:
				SPI2_CS_ENABLE();
				/* average current resgiter value C */
				MI[Phase_C] = ReadBronco(0xE910);			
				SPI2_CS_DISABLE();			
				resgiterPara_Dispose(MI[Phase_C], current, Phase_C);
				para += 1;
				break;		
		
		/*----- power -----*/
		case 7:
				SPI2_CS_ENABLE();
				/* average power resgiter value A */
				MAP[Phase_A] = ReadBronco(0xE8E8);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MAP[0], power, Phase_A);
				para += 1;
				break;
		
		case 8:
				SPI2_CS_ENABLE();
				/* average power resgiter value B */
				MAP[Phase_B] = ReadBronco(0xE8E9);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MAP[1], power, Phase_B);
				para += 1;
				break;
		
		case 9:
				SPI2_CS_ENABLE();
				/* average power resgiter value C */
				MAP[Phase_C] = ReadBronco(0xE8EA);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MAP[2], power, Phase_C);
				para += 1;
				break;
		
		/*----- power factor -----*/
		case 10:
				SPI2_CS_ENABLE();
				/* average power factor resgiter value A */
				MAF[Phase_A] = ReadBronco(0xE8E4);	
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MAF[0], powerFactor, Phase_A);
				para += 1;
				break;
		
		case 11:
				SPI2_CS_ENABLE();
				/* average power factor resgiter value B */
				MAF[Phase_B] = ReadBronco(0xE8E5);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MAF[1], powerFactor, Phase_B);
				para += 1;
				break;
		
		case 12:
				SPI2_CS_ENABLE();
				/* average power factor resgiter value C */
				MAF[Phase_C] = ReadBronco(0xE8E6);			
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(MAF[2], powerFactor, Phase_C);
				para += 1;
				break;
		
		/*----- energy -----*/				
		case 13:
					SPI2_CS_ENABLE();
					/* energy count resgiter value A */
//					CFP[Phase_A] = ReadBronco(0xE89F);		
					CFP[Phase_A] += 1000;		
					SPI2_CS_DISABLE();
					resgiterPara_Dispose(CFP[0], energyCount, Phase_A);
					para += 1;
					break;
			
			case 14:
					SPI2_CS_ENABLE();
					/* energy count resgiter value B */
//					CFP[Phase_B] = ReadBronco(0xE8A0);	
					CFP[Phase_B] += 1000;
					SPI2_CS_DISABLE();
					resgiterPara_Dispose(CFP[1], energyCount, Phase_B);
					para += 1;
					break;
			
			case 15:
					SPI2_CS_ENABLE();
					/* energy count resgiter value C */
//  				CFP[Phase_C] = ReadBronco(0xE8A1);	
					CFP[Phase_C] += 1000;
					SPI2_CS_DISABLE();
					resgiterPara_Dispose(CFP[2], energyCount, Phase_C);
					para += 1;
					break;
			
			case 16:
				SPI2_CS_ENABLE();
				/* energy count resgiter value A+B+C */
//					CFP[BondPhase] = ReadBronco(0xE8A2);
				CFP[BondPhase] += 1000;		
				SPI2_CS_DISABLE();
				resgiterPara_Dispose(CFP[3], energyCount, BondPhase);
				para = 1;
				break;
				
			default: 
				para = 1; 
				break;
	}   
}



#if 1
void electricitySectorRead(u32 pAddr)
{
	u16 i = 0;
	u8 phaseX = 0;
	u8 AddrOffset[123] = {0};
	u8 energyCountValueBuf[32] = {0};
	u8 bufOffset = 0;
	
//	SPI_Flash_Read(AddrOffset, pAddr, 123);	
	for(i = 0; i < 123; i++)
	{
		if((AddrOffset[i+1] - AddrOffset[i] != 1) && (AddrOffset[i] < 123))
			break;
	}
	gV9203_Info.energyCountValue_AddrOffset = i;

//	SPI_Flash_Read(energyCountValueBuf, pAddr+i*32+0x7D, 32);
	for(; phaseX < 4; phaseX++)
	{
		for(i = 0; i < 8; i++)
			EnergyCountValueTemp.oneBit.temp[7-i] = energyCountValueBuf[i+bufOffset];
		bufOffset += 8;
		gV9203_Info.energyCountValue[phaseX] = EnergyCountValueTemp.allBits;
		gV9203_Info.electricityValue[phaseX] = gV9203_Info.energyCountValue[phaseX] / 80;	
	}
}
	
void V9203_ParameterRead(void)
{
	#if 0
	u32 StartAddr[2] = {0x008000,0x009000};	
	u8 energyCountValueBuf1[32] = {0},energyCountValueBuf2[32] = {0};
	u8 bufTemp1 = 0, bufTemp2 = 0;
	u8 i = 0, FlagStatus = 0, phaseX = 0;
	u32 energyCountValueTemp1[4] = {0}, energyCountValueTemp2[4] = {0};
	
	/*首字节是0XFF判断，是否写入扇区*/
	bufTemp1 = SPIFlash_ReadByte(StartAddr[0]);
	bufTemp2 = SPIFlash_ReadByte(StartAddr[1]);
	
	if(bufTemp1 != 0xff)
	{
		/*两个扇区都有存，由于断电另一个扇区未清除*/
		if(bufTemp2 != 0xff)
			FlagStatus = 1;
		/*其中一个扇区有,1有2没有*/
		else
			FlagStatus = 2;
	}
	else
	{
		/*其中一个扇区有，1没有2有*/
		if(bufTemp2 != 0xff)
			FlagStatus = 3;
		/*两个扇区都没有*/
		else
			FlagStatus = 0;
	}
	
	switch(FlagStatus)
	{
		case 0:
			break;
		
		case 1:
			SPI_Flash_Read(energyCountValueBuf1, StartAddr[0]+0x7D, 32);
			SPI_Flash_Read(energyCountValueBuf2, StartAddr[1]+0x7D, 32);
		
			for(; phaseX < 4; phaseX++)
			{
				for(i=0; i<8; i++)
				{
					EnergyCountValueTemp.oneBit.temp[7-i] = energyCountValueBuf1[i+(phaseX<<3)];
				}
				energyCountValueTemp1[i] = EnergyCountValueTemp.allBits;
				
				for(i=0; i<8; i++)
				{
					EnergyCountValueTemp.oneBit.temp[7-i] = energyCountValueBuf2[i+(phaseX<<3)];
				}
				energyCountValueTemp2[i] = EnergyCountValueTemp.allBits;
			}
			if((energyCountValueTemp1[0] > energyCountValueTemp2[0]) || (energyCountValueTemp1[1] > energyCountValueTemp2[1]) ||\
				 (energyCountValueTemp1[2] > energyCountValueTemp2[2]) || (energyCountValueTemp1[3] > energyCountValueTemp2[3]))
			{
				SPI_Flash_Erase_Sector(9);
				gV9203_Info.SectorChoose = FALSE;
				electricitySectorRead(StartAddr[0]);
			}	
			else 
			{
				SPI_Flash_Erase_Sector(8);
				gV9203_Info.SectorChoose = TRUE;
				electricitySectorRead(StartAddr[1]);
			}
			break;
		
		case 2:
			/*Sector1*/
			gV9203_Info.SectorChoose = FALSE;
			electricitySectorRead(StartAddr[0]);
			break;
		case 3:
			/*Sector2*/
			gV9203_Info.SectorChoose = TRUE;
			electricitySectorRead(StartAddr[1]);
			break;	
			
		default: 
			break;
	}
	#endif
}

void V9203_energyCountSaveTime(void)
{
	gV9203_Info.energyCountSaveTime++;	
}

#endif
