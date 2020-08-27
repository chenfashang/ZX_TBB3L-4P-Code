#ifndef _VARIABLE_ONLINE_UPGRADE_H_
#define _VARIABLE_ONLINE_UPGRADE_H_

//#include "Config.h"

//* 宏 ------------------------------------------------------------------------*/

#ifndef NULL
#define NULL        ((void *)0)
#endif

/******************************************************************************/
/*                           内部flash相关                                    */
/******************************************************************************/

//内置flash
#define FW_SPI_FLASH_PROGRAM_STORAGE_START_SECTOR		    512 //APP缓存开始扇区地址
#define FW_FLASH_PROGRAM_STORAGE_SIZE            			  0x00038000    //224k
#define FW_FLASH_BEGIN_ADDR                       			0x08000000
#define FW_SPI_FLASH_PROGRAM_STORAGE_ADDR		      			0x80000            //app download addr 0x08000
#define FW_FLASH_PROGRAM_RUN_ADDR                 			0x08007800         //app start addr  220k-0x8007800--->0x003E800
#define FW_FLASH_END_ADDR					       								0x0803FFFF
#define FW_PROGRAM_DOWNLOAD_INFO_STORAGE_ADDR     			0x0803F800

#define FW_PROGRAM_FILE_NAME_LENGTH               (256)

#define FW_IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define FW_IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define FW_IS_09(c)  ((c >= '0') && (c <= '9'))
#define FW_ISVALIDHEX(c)  FW_IS_AF(c) || FW_IS_af(c) || FW_IS_09(c)
#define FW_ISVALIDDEC(c)  FW_IS_09(c)
#define FW_CONVERTDEC(c)  (c - '0')
#define FW_CONVERTHEX_alpha(c)  (FW_IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define FW_CONVERTHEX(c)   (FW_IS_09(c) ? (c - '0') : FW_CONVERTHEX_alpha(c))
#define FW_OPERATE_FLASH_ERROR_MAX_NUM      3

/*************************************************************/
/*                          系统 部分                        */
/*************************************************************/
#define FW_UPGRADE_DEVICE_NONE   (0x00)    //设备类型为空
#define FW_UPGRADE_DEVICE_ZNE    (0x01)    //设备类型为网关
#define FW_UPGRADE_DEVICE_HOST   (0x02)    //设备类型为终端主机
#define FW_UPGRADE_DEVICE_SLAVE  (0x032)    //设备类型为从机

#define FW_UPGRADE_STATUS_NONE    0X0
#define FW_UPGRADE_STATUS_ELSE    0x01
#define FW_UPGRADE_STATUS_SELF 		0x02

#define FW_UPGRADE_MODE_NONE     				0
#define FW_UPGRADE_MODE_NET_HOST 				1
#define FW_UPGRADE_MODE_SERIAL_HOST 		2
#define FW_UPGRADE_MODE_CHANNEL_SLAVE 	3

/*************************************************************/
/*                         计时 部分                       */
/*************************************************************/
#define FW_THC_UPGRADE_TIMEOUT_WATI_NUM  7500
#define FW_SLAVE_DATA_WAIT_WAIT_NUM      3
/*************************************************************/
/*                         Ymodem 部分                       */
/*************************************************************/
#define FW_YMODEM_PACKET_LEN_INDEX        (0)
#define FW_YMODEM_PACKET_SEQNO_INDEX      (1)
#define FW_YMODEM_PACKET_SEQNO_COMP_INDEX (2)

#define FW_YMODEM_PACKET_HEADER           (3)//3
#define FW_YMODEM_PACKET_TRAILER          (2)
#define FW_YMODEM_PACKET_OVERHEAD         (FW_YMODEM_PACKET_HEADER + FW_YMODEM_PACKET_TRAILER)
#define FW_YMODEM_PACKET_SIZE             (128)
#define FW_YMODEM_PACKET_1K_SIZE          (1024)

#define FW_PACKET_1K_SIZE_TOTAL     1029
#define FW_PACKET_SIZE_TOTAL        133
#define FW_PACKET_END_SIZE_TOTAL    1

#define FW_FILE_SIZE_LENGTH        (16)

#define FW_YMODEM_SOH                     (0x01)  //128字节数据包开始
#define FW_YMODEM_STX                     (0x02)  //1024字节的数据包开始
#define FW_YMODEM_EOT                     (0x04)  //结束传输
#define FW_YMODEM_ACK                     (0x06)  //回应
#define FW_YMODEM_NAK                     (0x15)  //没回应
#define FW_YMODEM_CA                      (0x18)  //这两个相继中止转移
#define FW_YMODEM_CRC16                   (0x43)  //'C' == 0x43, 需要 16-bit CRC 
#define FW_YMODEM_SUCC             				(0x53)   //'S' 升级成功
#define FW_YMODEM_POWERON          				(0x50)    //'P' 重新上电
#define FW_YMODEM_ABORT1                  (0x41)  //'A' == 0x41, 用户终止 
#define FW_YMODEM_ABORT2                  (0x61)  //'a' == 0x61, 用户终止
#define FW_YMODEM_NAK_TIMEOUT             (0x100000)
#define FW_YMODEM_MAX_ERRORS              (10)
#define FW_YMODEM_2CA                     (0x30) 
#define FW_YMODEM_FAIL_CODE00             (0xf0)    //'F' 升级失败
#define FW_YMODEM_FAIL_CODE01             (0xf1)    //'F' 升级失败
#define FW_YMODEM_FAIL_CODE02             (0xf2)    //'F' 升级失败
#define FW_YMODEM_FAIL_CODE03             (0xf3)    //'F' 升级失败
#define FW_YMODEM_FAIL_CODE04             (0xf4)    //'F' 升级失败
#define FW_YMODEM_FAIL_CODE05             (0xf5)    //'F' 升级失败
#define FW_YMODEM_FAIL_CODE06             (0xf6)
#define FW_YMODEM_FAIL_CODE07             (0xf7)    //'F' ????
#define FW_YMODEM_FAIL_CODE08             (0xf8)    //'F' ????
#define FW_YMODEM_FAIL_CODE09             (0xf9)    //'F' ????
#define FW_YMODEM_FAIL_CODE10             (0xfa)    //'F' ????
#define FW_YMODEM_FAIL_CODE11             (0xfb)    //'F' ????
#define FW_YMODEM_FAIL_CODE12             (0xfc)    //'F' ????
#define FW_YMODEM_FAIL_CODE13             (0xfd)
#define FW_YMODEM_FAIL_CODE14             (0xfe)    //'F' ????
#define FW_YMODEM_FAIL_CODE15             (0xff)    //'F' ????

#define FW_PENETRATE_FEED_LEN                   1
/***********************************************************************************/

#define FW_THC_REPORT_C_WAIT_NUM            2000
#define FW_UPGRADE_DELAY_WAIT_200MS         50
#define FW_UPGRADE_DELAY_WAIT_1000MS        200
#define FW_UART3_TTL_END_NUM                200            //

#define FW_PROGRAM_DOWNLOAD_STATUS_NO  		 0x4e446c64     //NDld
#define	FW_PROGRAM_DOWNLOAD_STATUS_ING  	 0x6f6c496e     //olIn
#define	FW_PROGRAM_DOWNLOAD_STATUS_FINISH  0x775e4649  //onFi

#define FW_DOWNLOAD_PACKAGE_MAX_NUM      0xff  //最大只能接收255k大小的程序
#define FW_UPGRADE_TOTAL_WAIT_NUM        150000
#define FW_NOTICE_CHANNEL_SLAVE_EXIT_CODE           (0x98)

#define FW_BROADCAST_CODE                    (0x00)  //128???????

//???á11ì?±?D?oíbootloader?Dμ?ò??ù
typedef struct PRE_DOWNLOAD_INFO{
  UINT32 download_status;
	UINT8 file_name[FW_PROGRAM_FILE_NAME_LENGTH];
	UINT32 file_size;
	UINT32 write_flash_size;
	UINT32 storage_addr;
	UINT32 run_addr;  
	UINT8 re_write_flash_num;
}FW_program_download_info;

typedef struct
{
  UINT8 package_idx;
	UINT8 package_size;
	UINT16 crc_data;
}FW_program_download_crc_info;



typedef struct
{
  UINT8 data_buf[FW_PACKET_1K_SIZE_TOTAL+2];  //·à?1ò?3?
	UINT8 no_deal_flag;
	UINT16 package_deta_num;
	UINT16 data_recv_num;
}FW_channel_slave_data_get_info;

typedef union
{
	struct
	{
		UINT8  F0:1;
		UINT8  F1:1;
		UINT8  F2:1;
		UINT8  F3:1;
		UINT8  F4:1;
		UINT8  F5:1;
		UINT8  F6:1;
		UINT8  F7:1;			
	}oneBit;
	UINT8 allBits;
}FW_bit_define;

extern volatile FW_bit_define FW_bit_flag[2];

#define  g_FW_upgrade_net_timeout_enable_flag              FW_bit_flag[0].oneBit.F4
#define  g_FW_upgrade_net_timeout_flag                     FW_bit_flag[0].oneBit.F5
#define  g_FW_net_report_ACK_flag                          FW_bit_flag[0].oneBit.F6
#define  g_FW_net_report_ACK_enable_flag                   FW_bit_flag[0].oneBit.F7

#define  g_FW_upgrade_total_time_flag              				 FW_bit_flag[1].oneBit.F1
#define  g_FW_channel_slave_data_get_start_flag            FW_bit_flag[1].oneBit.F2
#define  g_FW_channel_slave_data_get_flag                  FW_bit_flag[1].oneBit.F3

extern volatile UINT8 g_FW_upgrade_mode;      //固件升级模式;1:平台升级;2:串口升级 
extern volatile UINT8 g_FW_channel_slave_upgrade_status;        //从机进入在线升级标志
extern UINT8 g_FW_upgrade_device_type;         //网关升级，主机升级，从机升级
extern UINT8  g_FW_program_package_data_buf[];
extern UINT8 g_FW_upgrade_device_road_num;
extern FW_program_download_info g_FW_download_info;  
extern UINT32 g_FW_program_download_real_size;
extern UINT16  g_FW_download_package_num;     //下载的正式文件的包数
extern UINT16 g_FW_upgrade_net_report_ACK_beat_num;
extern UINT32 g_FW_upgrade_total_time_beat_num;   //??è??ú??éy??oó￡?×ü12?ü??è?μ?ê±??
extern u8 g_FW_flash_operate_status;
extern UINT16 g_FW_upgrade_net_timeout_beat_num;
extern FW_program_download_crc_info g_FW_download_crc_record[FW_DOWNLOAD_PACKAGE_MAX_NUM];    //????????????CRC????
extern UINT16 g_FW_channel_slave_data_wait_beat_num;
extern volatile FW_channel_slave_data_get_info g_FW_channel_slave_data;
extern UINT16 g_channel_slave_recv_data_num;
extern UINT8 g_FW_channel_slave_recv_data;

/* */
void FW_jump2addr(UINT32 appxaddr);
void FW_SYS_SoftReset(void);

UINT32 FW_spi_flash_calculate_sector(__IO UINT32 Size);
INT8 FW_spi_flash_erase_sector(UINT32 page_addr);
UINT8 FW_flash_programme_word(UINT32 addr, UINT32 data);
INT8 FW_flash_erase_page(UINT32 page_addr);
u8 FW_spi_flash_programme(UINT32 addr, UINT8 *data, UINT32 len);

void FW_upgrade_init(void);
void FW_timer_deal(void);

UINT16 FW_ymodem_calculate_crc(UINT8 *, UINT16);

void FW_check_upgrade(void);

void FW_channel_slave_download(void);
INT32 FW_channel_slave_ymodem_receive(void);
void FW_channel_slave_send_data(u8 const *,u8);
void FW_channel_slave_data_get(CUINT8);
void FW_spi_read_flash(UINT32 Addr, UINT8 *Buff, UINT16 Len);

#endif
