#ifndef BASE_H
#define BASE_H

//#define DEBUG
#define	FILE_LINE   __FILE__,__FUNCTION__,__LINE__
#define	PRINT_LINE()	Lib_printf("[%s][%s][%d]\n", FILE_LINE);
#define NO_ERR	0x00//无错
#define ERROR	0xEE//有错
#define WM_USER_EXIT	-1//界面异常返回值
#define EDIT_MAX_LEN	128
#define	FRAME_MAX_LEN	256

#define UART_WAIT_SHORT	2000//串口等待时间-短-
#define UART_WAIT_MID	7000//串口等待时间-中-
#define UART_WAIT_LONG	40000//串口等待时间-长-

#define ADDR_HEX	0x01//阀门的地址是16进制字符串
#define ADDR_BCD	0x00//阀门的地址是BCD字符串

#pragma pack(push)
#pragma pack(1)
typedef struct {//系统时间结构, 倒序存储以方便使用
	U8 u8second;	//秒
	U8 u8minute;	//分
	U8 u8hour;		//时
	U8 u8day;		//日
	U8 u8month;		//月
	U8 u8year;		//年
}sys_time_str;
typedef sys_time_str* sys_time_ptr;


typedef struct {
	U8	bigErr[20];
	U8	mid2Err[20];
	U8	mid1Err[20];
	U8	smallErr[20];
} flow_err_string_str;
typedef flow_err_string_str* flow_err_string_ptr;

#pragma pack(pop)

//手持机设置的索引号, 在数据库中的行号也与此对应
typedef enum {
	config_com_para = 0,//端口相关参数
	config_meter_type,	//仪表类型
	config_valve_type	//阀控类型
}config_rowidx;

typedef enum {
	em_device_rs485 = 0,
	em_device_mbus
}em_device_idx;

typedef enum {
	em_baud_1200 = 0,
	em_baud_2400,
	em_baud_4800,
	em_baud_9600,
	em_baud_19200,
	em_baud_38400,
	em_baud_57600,
	em_baud_115200
} em_baud_idx;

typedef enum {
	em_databit_8 = 0,
	em_databit_7,
	em_databit_9
}em_databit_idx;

typedef enum {
	em_parity_none = 0,
	em_parity_odd,
	em_parity_even
}em_parity_idx;

typedef enum {
	em_stop_1 = 0,
	em_stop_2,
	em_stop_1_5
}em_stop_idx;

typedef struct {
	em_databit_idx	databit;
	em_parity_idx	parity;
	em_stop_idx		stop;
} uart_mode_str;

typedef enum {//仪表类型索引
	em_mtype_deluUltro = 0,	//德鲁超声波热量表
	em_mtype_deluMachenical	//德鲁机械式热量表
} em_mtype_idx;

typedef enum {//阀控类型索引
	em_vtype_yilin = 0,	//亿林阀控, 非德鲁协议
	em_vtype_yilindelu,	//亿林阀控, 德鲁协议
	em_vtype_bainuan	//百暖会协议
} em_vtype_idx;

#endif // ifndef BASE_H
