#ifndef __AT_DEAL_H
#define __AT_DEAL_H

#include "sys.h" 
#include "common.h"

//AT指令 常量定义
#define TH_AT_HEAD_LEN                                  2       //指令头长：0X5A,0XA5
#define TH_AT_LENGTH_CODE_LEN                           2       //指令长度表示位长
#define TH_AT_COMM_LEN                                  1       //指令帧类型位长
#define TH_AT_COMM_NUM_LEN                              1       //指令帧序号位长
#define TH_AT_CODE_LEN                                  4       //AT指令类型长度
#define TH_AT_ID_LEN                                    8       //指令PAN ID长
#define TH_AT_CEID_LEN                                  16      //指令CE ID长
#define TH_AT_AT_MODE_IN_LEN                            2      //指令AT模式切换长度
#define TH_AT_DHXY_LEN                                  4       //硬件版本号 + RS485地址长度
#define TH_AT_RESERVED_LEN                              2       //指令保留位长
#define TH_AT_CHECK_CODE_LEN                            1       //指令校验码长度
#define TH_AT_END_LEN                                   2       //指令尾长：0X5B,0XB5
#define TH_AT_EX_CHECK_LEN                              7       //指令除需校验计算外的长度
#define TH_AT_ID_REFER_LEN                              21      //ID反馈指令长度
#define TH_AT_EX_DATA_LEN                               13      //指令除需校验计算外的长度

#define TH_AT_LEN_POS                                   3       //指令长度位位置
#define TH_AT_COMM_POS                                  4       //指令类型位置
#define TH_AT_CODE_POS                                  6       //AT指令判断位置
#define TH_AT_LEN_H_POS                     2       //指令长度高位位置
#define TH_AT_LEN_L_POS                     3       //指令长度低位位置
#define TH_AT_DATA_START_POS                10      //AT指令数据起始位置
#define ZNE_COMM_POS                        4       //平台指令类型位置


#define TH_AT_FIRST_CODE                                0x5A    //指令头字节
#define TH_AT_SECOND_CODE                               0xA5    //指令次字节
#define TH_AT_PRE_LAST_CODE                             0x5B    //指令倒数第二个字节
#define TH_AT_LAST_CODE                                 0xB5    //指令尾字节

#define TH_AT_SET_CODE                                  0x08    //AT设置指令
#define TH_AT_INQUIRY_CODE                              0x18    //AT查询指令
#define TH_AT_FEED_CODE                                 0x88    //AT反馈指令

#define DEVICE_ID_LEN                       8           //从设备ID长度
#define DEVICE_VERSION_LEN                  4
#define DEVICE_TYPE_LEN                     2
#define DEVICE_ENCRY_MODE_LEN               2

//-------------------------
//数据操作指令
#define X485_POSITIVE_CODE                   0x00        //正数
#define X485_NEGATIVE_CODE                   0x01        //负数
#define X485_ZERO_CODE                       0x00        //数据位为0
#define X485_ONE_CODE                        0x01        //数据位为1
#define X485_TWO_CODE                        0x02        //数据位为2
#define X485_THREE_CODE                      0x03        //数据位为3
#define X485_FOUR_CODE                       0x04        //数据位为4
#define X485_FIVE_CODE                       0x05        //数据位为5
#define X485_TEN_CODE                        0x0A        //数据位为10                   
#define X485_Slave_ID_Addr                   0x01        //ID地址


void UART3_AT_Deal(void);
void Set_X485_AT_Mode_flag_Value(BOOLEAN pBuf);
BOOLEAN Get_X485_AT_Mode_flag_Value(void);
void X485_Slave_ID_Init(void); 
#endif
