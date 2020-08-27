#include "Config.h"

VUINT8 g_FW_channel_slave_upgrade_status;        //′ó?ú??è??ú??éy??±ê??
volatile UINT8 g_FW_upgrade_mode;      //??????;1:????;2:????;3:??????

volatile FW_bit_define FW_bit_flag[2];

UINT8 g_FW_upgrade_device_type;
UINT8 g_FW_channel_slave_recv_data;
UINT8 g_FW_program_package_data_buf[FW_YMODEM_PACKET_1K_SIZE + FW_YMODEM_PACKET_OVERHEAD];
FW_program_download_info g_FW_download_info; 
UINT16 g_FW_upgrade_net_timeout_beat_num;   //允许平台不回应的时间
UINT16 g_FW_upgrade_channel_slave_timeout_beat_num;   //允许从机不回应的时间
UINT32 g_FW_program_download_real_size;
UINT16  g_FW_download_package_num;     //??????????
FW_program_download_crc_info g_FW_download_crc_record[FW_DOWNLOAD_PACKAGE_MAX_NUM];    //????????????CRC????
UINT16 g_FW_upgrade_net_report_ACK_beat_num;
UINT32 g_FW_upgrade_total_time_beat_num;   //???????,????????
UINT8 g_FW_flash_operate_status;
UINT8 g_FW_upgrade_device_road_num;
UINT16 g_FW_channel_slave_data_wait_beat_num;
volatile FW_channel_slave_data_get_info g_FW_channel_slave_data;

UINT16 g_channel_slave_recv_data_num;
