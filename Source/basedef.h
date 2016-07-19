#ifndef BASEDEF_H
#define BASEDEF_H

#include "user.h"

#define DEBUG

#define	FILE_LINE   __FILE__,__FUNCTION__,__LINE__
#define	PRINT_LINE()	Lib_printf("[%s][%s][%d]\n", FILE_LINE);
#define NO_ERR	0x00//无错
#define ERROR	0x01//有错

#define WM_USER_EXIT	-1
#define EDIT_MAX_LEN	128

#define MAX_TIME_NODE	24//抄表时间点不超过24个点
#define MINUTES_PERDAY	1440//一天有多少分钟

#define STRLEN(c)	strlen((const char*)(c))
#define IP_DELIM	'.'
typedef struct{//系统时间结构, 倒序存储以方便使用
	U8 u8second;	//秒
	U8 u8minute;	//分
	U8 u8hour;		//时
	U8 u8day;		//日
	U8 u8month;		//月
	U8 u8year;		//年
}sys_time_str;
typedef sys_time_str* sys_time_ptr;

typedef struct{//用一个字节表示分钟, 超过60则进位, 对于一般的时间相加足够
	U8 u8minute;	//分, Hex
	U8 u8hour;		//时, Hex
}time_node_str;
typedef time_node_str* time_node_ptr;

//手持机设置的索引号, 在数据库中的行号也与此对应
typedef enum {
	config_com_para = 0,//端口相关参数
	config_gateway_id,	//集中器号
	config_server_id	//主站号
}config_rowidx;


#endif

