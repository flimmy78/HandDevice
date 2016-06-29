#ifndef BASEDEF_H
#define BASEDEF_H

#include "user.h"

#define DEBUG

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

typedef enum {//�ֳֻ����õ�������
	config_com_para = 0,	//�˿���ز���
	config_gateway_id,	//��������
	config_server_id	//��վ��
}em_config_idx;

#endif

