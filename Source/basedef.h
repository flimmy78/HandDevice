#ifndef BASEDEF_H
#define BASEDEF_H

#include "user.h"

//#define DEBUG

#define  FILE_LINE   __FILE__,__FUNCTION__,__LINE__

#define NO_ERR	0x00//�޴�
#define ERROR	0x01//�д�

#define WM_USER_EXIT	-1
#define EDIT_MAX_LEN	128

#define MAX_TIME_NODE	24//����ʱ��㲻����24����
#define MINUTES_PERDAY	1440//һ���ж��ٷ���

#define STRLEN(c)	strlen((const char*)(c))

typedef struct{//ϵͳʱ��ṹ, ����洢�Է���ʹ��
	U8 u8second;	//��
	U8 u8minute;	//��
	U8 u8hour;		//ʱ
	U8 u8day;		//��
	U8 u8month;		//��
	U8 u8year;		//��
}sys_time_str;
typedef sys_time_str* sys_time_ptr;

typedef struct
{
	U8 u8hour;		//ʱ
	U8 u8minute;	//��
}time_node_str;
typedef time_node_str time_node_ptr;

//�ֳֻ����õ�������, �����ݿ��е��к�Ҳ��˶�Ӧ
typedef enum {
	config_com_para = 0,//�˿���ز���
	config_gateway_id,	//��������
	config_server_id	//��վ��
}config_rowidx;


#endif

