#ifndef BASEDEF_H
#define BASEDEF_H

#include "user.h"

#define  FILE_LINE   __FILE__,__FUNCTION__,__LINE__

#define NO_ERR	0x00//无错
#define ERROR	0x01//有错



typedef struct{//系统时间结构, 倒序存储以方便使用
	U8 u8second;	//秒
	U8 u8minute;	//分
	U8 u8hour;		//时
	U8 u8day;		//日
	U8 u8month;		//月
	U8 u8year;		//年
}sys_time_str;
typedef sys_time_str* sys_time_ptr;


typedef enum{//波特率的索引号
	baud1200 = 1200,
	baud2400 = 2400,
	baud4800 = 4800,
	baud9600 = 9600,
	baud19200 = 19200,
	baud38400 = 38400,
	baud57600 = 57600,
	baud115200 = 115200
}em_baudrate;

typedef enum{
	data5 = 5,
	data6 = 6,
	data7 = 7,
	data8 = 8,
	UnknownDataBits = -1
}em_databits;

typedef enum{
	NoParity = 0,
	EvenParity = 2,
	OddParity = 3,
	SpaceParity = 4,
	MarkParity = 5,
	UnknownParity = -1
}em_parity;

typedef enum{
	OneStop = 1,
	TwoStop = 2,
	OneAndHalfStop = 3,
	UnknownStopBits = -1
}em_stopbits;

typedef enum {
	NoFlowControl=0,
	HardwareControl,
	SoftwareControl,
	UnknownFlowControl = -1
}em_flowcontrol;

typedef struct{//端口参数设置
	U8 u8comType;	//端口类型, 使用user.h定义的{DEVICE_RS485, DEVICE_RS232, DEVICE_MBUS}等
	em_baudrate emBaud;//波特率
	em_databits emDataBits;//数据位
	em_parity	emParity;//校验位
	em_stopbits	emStopBits;//停止位
}com_config_str;
typedef com_config_str* com_config_ptr;

typedef enum {//手持机设置的索引号
	config_com_para = 0,	//端口相关参数
	config_gateway_id,	//集中器号
	config_server_id	//主站号
}em_config_idx;

#endif

