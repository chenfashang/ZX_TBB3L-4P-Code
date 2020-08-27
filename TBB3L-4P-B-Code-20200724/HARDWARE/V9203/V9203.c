//#include "Config.h"
#include "V9203.h"
#include "rs485.h"

static unsigned char tdo_m,tdo_c;
static unsigned int  tdo_d;
//static unsigned char spi_err;
u8 guc_GateCF;                  // CF脉冲的分频率系数，通过查表获取
u8 guc_UnitCF;   
S_JBPM  gs_JbPm;
volatile u8 guc_CommDelayTime;
S_METERC  gs_MeterConstT[MeterConstLen];

V9203_TypeDef gV9203_Info;

static u8 V9203InitErrFlag = 0;
static BOOLEAN V9203InitFlag = FALSE;

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

/**
  * @brief  V9203 GPIO initialization.
  * @param  None
  * @retval None
  */
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

	V9203_PowerDown();	
	V9203InitFlag = FALSE;
}

/**
  * @brief  V9203 initialization.
  * @param  None
  * @retval None
  */
void V9203_Init(void)
{
	static u8 step = 0;
	
	switch(step)
	{
		case 0:
			V9203_WAKEUP();
			step = 1;
			printf("init step = %d\r\n",step);
			break;
		
		case 1:
			SPI2_CS_ENABLE();
			BroncoInit();
			SPI2_CS_DISABLE();
			step = 2;
			printf("init step = %d\r\n",step);
			break;
		
		case 2:
			Jbpmdefault();
			step = 3;
		printf("init step = %d\r\n",step);
			break;	
		
		case 3:
			SPI2_CS_ENABLE();
			WriteBronco(0xaa000000,RegMTPARA0);
			SPI2_CS_DISABLE();
			step = 4;
		printf("init step = %d\r\n",step);
			break;	
		
		case 4:
			step = 5;
		printf("init step = %d\r\n",step);
			break;
	
		case 5:
			SPI2_CS_ENABLE();
			Update_Bronco_Para();
			SPI2_CS_DISABLE();
			step = 6;
		printf("init step = %d\r\n",step);
			break;
	
		case 6:
			step = 7;
		printf("init step = %d\r\n",step);
			break;
	
		case 7:
			SPI2_CS_ENABLE();
			WriteBronco(0x00000000,0xa002);//中断标志清零  
			WriteBronco(0x00000000,0xa002);
			SPI2_CS_DISABLE();
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) != RESET)
			{	
				step = 5;
				if(V9203InitErrFlag >= 2)
				{
					step = 0;
				}
				V9203InitErrFlag++;
			}
			else
			{
				step = 8;		
			}
			printf("init step = %d\r\n",step);
			break;
	
		case 8:
			SPI2_CS_ENABLE();
			//FEED_DOG;
			gs_JbPm.ui_MeterC = 400;
			Eny_JbPm_MeterConst();
			WriteBronco(0, RegAP);
			WriteBronco(0, RegBP);
			WriteBronco(0, RegCP);
			WriteBronco(0, RegPP);
			WriteBronco(0, RegAQ);
			WriteBronco(0, RegBQ);
			WriteBronco(0, RegCQ);
			V9203InitFlag = TRUE;
			step = 0;
			SPI2_CS_DISABLE();
			V9203InitFlag = TRUE;
		printf("init step = %d\r\n",step);
			break;
		
		default:
			step = 0; 
			V9203InitFlag = TRUE;
		printf("init step = %d\r\n",step);
		break;
	}
}

void BroncoInit(void)
{
		u8 i = 0;
    static u32 ready = 0;

	  for(i = 0;i <= 10;i++)
    {
			if(ready!=0x100000ff)
			{
				WriteBronco(0x100000ff,0xc000);
				ready=ReadBronco(0xc000);
			}
		}
		//FEED_DOG;
		/*DataSheet P118*/
    for(i = 0;i<56;i++)
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
	tdo_m = SPI2_ReadWriteByte(cmdb);
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
//        spi_err = 1;
    }
		else
		{
//        spi_err = 0;
		}
    set_data_cmd_flash(0x14, addr);
    mt_dout += (((unsigned long)tdo_d)<<16);
    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
		{
//        spi_err = 1;
    }
		else
		{
//        spi_err = 0;
    }
    return mt_dout;
}

void Jbpmdefault(void)
{
	gs_JbPm.ui_MeterC=400; //6400;                 // 表常数
	gs_JbPm.ui_Un=2200;                           // 标称电压
	gs_JbPm.ui_Ib=25000;  //1500;                  // 标称电流
	gs_JbPm.ui_Resve1=0;
					
	gs_JbPm.RacEGYTH   = 0x15742b;                 //0xd2ace2;能量累加门限值
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
	gs_JbPm.ul_PG = 50;                    //功率比例系数
	gs_JbPm.ul_URmG = 20830;             //电压通道比例系数
	gs_JbPm.ul_I1RmG = 200000;            //A、B、C三相电流通道比例系数,
	gs_JbPm.ul_InRmG = 1020000;            //零线通道比例系数,
	//FEED_DOG;				
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

void Update_Bronco_Para(void)
{
	u32 sum = 0;
	//FEED_DOG;
	WriteBronco(0x000000ff,RegMTPARA0); //MT_PARA0
	sum+=0x000000ff;
	WriteBronco(gs_JbPm.RacMTPARA1,RegMTPARA1);//MT_PARA1
	sum+= gs_JbPm.RacMTPARA1;
	WriteBronco(gs_JbPm.RacMTPARA2,RegMTPARA2);//MT_PARA2 -不分段，分2次发32bit，DMA开UA，非didt，AD全开
	sum+=gs_JbPm.RacMTPARA2;
	WriteBronco(0x00000007,ZZAPPA);//全波/基波合相视在功率组合     A+B+C 
	sum+=0x00000007; 
	WriteBronco(0x00000001,ZZCPSEL);//启动潜动模式选择            根据电流有效值
	sum+=0x00000001;
	WriteBronco(gs_JbPm.RacANCtrl0,RegANCtrl0);//模拟控制寄存器0       0x000f1111      电压2倍增益，电流4倍增益
	sum+=gs_JbPm.RacANCtrl0;
	WriteBronco(gs_JbPm.RacANCtrl1,RegANCtrl1); //模拟控制寄存器1
	sum+=gs_JbPm.RacANCtrl1;
	WriteBronco(gs_JbPm.RacANCtrl2,RegANCtrl2);//模拟控制寄存器2   0x77005440
	sum+=gs_JbPm.RacANCtrl2;
	WriteBronco(gs_JbPm.RacANCtrl3,RegANCtrl3);//模拟控制寄存器3 //降频率0x000d0406
	sum+=gs_JbPm.RacANCtrl3;
	WriteBronco( gs_JbPm.RacEGYTH,RegZZEGYTHL);//门限低位
	sum+= gs_JbPm.RacEGYTH;
	WriteBronco(0x00000000,RegZZEGYTHH);//门限高位
	sum+=0x00000000;
	WriteBronco(gs_JbPm.RacCTHH,RegCTHH);//启动门限值上限
	sum+=gs_JbPm.RacCTHH;
	WriteBronco(gs_JbPm.RacCTHL,RegCTHL);//启动门限值下限
	sum+=gs_JbPm.RacCTHL;
	
 // FEED_DOG; 
	WriteBronco(gs_JbPm.gs_JBA.RacWAPT,RegWAPTA0);//有功A比差0    
	sum+=gs_JbPm.gs_JBA.RacWAPT;
	WriteBronco(gs_JbPm.gs_JBB.RacWAPT,RegWAPTB0);//有功B比差0
	sum += (gs_JbPm.gs_JBB.RacWAPT);
	WriteBronco(gs_JbPm.gs_JBC.RacWAPT,RegWAPTC0);//有功C比差0  
	sum+=gs_JbPm.gs_JBC.RacWAPT;
	WriteBronco(gs_JbPm.gs_JBA.RacWAPT,RegWAQTA);//无功A比差
	sum+=gs_JbPm.gs_JBA.RacWAPT;
	WriteBronco(gs_JbPm.gs_JBB.RacWAPT,RegWAQTB);//无功B比差
	sum+=gs_JbPm.gs_JBB.RacWAPT;
	WriteBronco(gs_JbPm.gs_JBC.RacWAPT,RegWAQTC);//无功C比差
	sum+=gs_JbPm.gs_JBC.RacWAPT;

  WriteBronco( gs_JbPm.gs_JBA.RacWARTI,RegWARTIA);     //A电流有效值比差
  sum+=gs_JbPm.gs_JBA.RacWARTI;
  WriteBronco( gs_JbPm.gs_JBB.RacWARTI,RegWARTIB);     //B电流有效值比差
  sum+=gs_JbPm.gs_JBB.RacWARTI;
  WriteBronco( gs_JbPm.gs_JBC.RacWARTI,RegWARTIC);     //C电流有效值比差
  sum+=gs_JbPm.gs_JBC.RacWARTI;
  WriteBronco( 0X000,RegWARTIN);   //N电流有效值比差
	
	//FEED_DOG;
  sum+=0x00;
  WriteBronco( gs_JbPm.gs_JBA.RacWARTU,RegWARTUA);    //A电压有效值比差
  sum+=gs_JbPm.gs_JBA.RacWARTU;
  WriteBronco( gs_JbPm.gs_JBB.RacWARTU,RegWARTUB);     //B电压有效值比差
  sum+=gs_JbPm.gs_JBB.RacWARTU;
  WriteBronco(gs_JbPm.gs_JBC.RacWARTU,RegWARTUC);  //C电压有效值比差
  sum+=gs_JbPm.gs_JBC.RacWARTU;
  WriteBronco(0x00000000,RegWBPTA); //基波有功A比差0    
  sum+=0x00000000;
  WriteBronco(0x00000000,RegWBPTB);//基波有功B比差0
  sum+=0x00000000;
  WriteBronco(0x00000000,RegWBPTC);//基波有功C比差0  
  sum+=0x00000000;
  WriteBronco(0x00000000,RegWBQTA);//基波无功A比差
  sum+=0x00000000;
  WriteBronco(0x00000000,RegWBQTB);//基波无功B比差
  sum+=0x00000000;
  WriteBronco(0x00000000,RegWBQTC);//基波无功C比差
  sum+=0x00000000;
  WriteBronco(gs_JbPm.RacWAEC0,RegWAEC0);//角差
  sum+=gs_JbPm.RacWAEC0;
  WriteBronco(gs_JbPm.RacZZDCUM,RegZZDCUM);//UM通道直流分量  预设负数值 电流检测中断
  sum+=gs_JbPm.RacZZDCUM;
  WriteBronco(0x0000002A,0xEC23);//有功和相0配置
  sum+=0x2A;
  WriteBronco(0x0000002A,0xEC24);//有功和相1配置
  sum+=0x2a;
  WriteBronco(0x0000002A,0xEC47);//无功和相0配置
  sum+=0x2A;
  WriteBronco(0x0000002A,0xEC48);//无功和相1配置
  sum+=0x2a;
	
	//FEED_DOG;
  WriteBronco(0x00002211,0xEC34);//高速CF来源选择
  sum+=0x2211;
  WriteBronco(0x00008000,0xA000);//中断0
  sum+=0x8000;
  WriteBronco(0x00008000,0xA001);//中断1
  sum+=0x8000;
  sum=0xffffffff-sum;
  WriteBronco(sum,0xC003);//mt_para3 自检和  SUM+x=0xffffffff
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

void V9203_PowerDown(void)   
{
	DeepSleepEnable();
//	PM1Disable();
//	PM0Disable();
}

void V9203_WAKEUP(void) 
{
	DeepSleepDisable();
	PM1Enable();
	PM0Enable();
}

/**
  * @brief  get V9203 initialization  error flag.
  * @param  None
  * @retval None
  */
u8 Get_V9203_init_err_flag(void)
{
	return V9203InitErrFlag;
}

/**
  * @brief  set V9203 initialization flag.
  * @param  None
  * @retval None
  */
void Set_V9203_init_flag(void)
{
	 V9203InitFlag = FALSE;
}

/**
  * @brief  get V9203 initialization flag.
  * @param  None
  * @retval None
  */
u8 Get_V9203_init_flag(void)
{
	return V9203InitFlag;
}

void V9203_Reboot(void)
{
	Set_V9203_init_flag();
	Set_V9203_ParameterRead_Finish(); 
}
/******************* (C) COPYRIGHT 2020-???? *****END OF FILE****/
