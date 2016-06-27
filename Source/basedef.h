#ifndef BASEDEF_H
#define BASEDEF_H

#include "user.h"

#define  FILE_LINE   __FILE__,__FUNCTION__,__LINE__

#define NO_ERR	0x00//�޴�
#define ERROR	0x01//�д�



typedef struct{//ϵͳʱ��ṹ, ����洢�Է���ʹ��
	U8 u8second;	//��
	U8 u8minute;	//��
	U8 u8hour;		//ʱ
	U8 u8day;		//��
	U8 u8month;		//��
	U8 u8year;		//��
}sys_time_str;
typedef sys_time_str* sys_time_ptr;


typedef enum{//�����ʵ�������
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

typedef struct{//�˿ڲ�������
	U8 u8comType;	//�˿�����, ʹ��user.h�����{DEVICE_RS485, DEVICE_RS232, DEVICE_MBUS}��
	em_baudrate emBaud;//������
	em_databits emDataBits;//����λ
	em_parity	emParity;//У��λ
	em_stopbits	emStopBits;//ֹͣλ
}com_config_str;
typedef com_config_str* com_config_ptr;

typedef enum {//�ֳֻ����õ�������
	config_com_para = 0,	//�˿���ز���
	config_gateway_id,	//��������
	config_server_id	//��վ��
}em_config_idx;

#endif

