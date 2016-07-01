#ifndef DB_H
#define DB_H

#include "user.h"
#include "basedef.h"

#define CONFIG_INITTED		0x01//�ֳֻ������Ѿ�����ʼ��
#define CONFIG_NOT_INITTED	0x00//�ֳֻ�������û����ʼ��

#define DB_CONFIG_NAME		"�ֳֻ�����.DBF"
#define DB_BASEINFO_NAME	"���ַ��Ϣ.DBF"

#define CONFIG_FILED_CNT		2
#define CONFIG_FIELD_NAME_ID	"f_id"
#define CONFIG_FIELD_NAME_VALUE	"f_value"

#define CONFIG_DELIMITER	','

typedef enum
{
	config_field_id=0,
	config_field_vale,
}config_field_idx;

extern U8 db_readAllConfig(void);
extern U8 db_getCongfig(u16 configIdx, U8* config);

#endif // DB_H
