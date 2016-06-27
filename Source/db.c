/***************************************************
**	ģ�鹦��:	�������ݿ���ز���
**	ģ������:	db.c
**	���ߣ�		�α���
****************************************************
*/
#include "protocol.h"
#include "db.h"

static com_config_str gComConfig;//��������
static U8 gu8svrAdd[GATEWAY_SADD_LEN] = {0};//��վ���
static U8 gu8gwyAdd[GATEWAY_OADD_LEN] = {0};//���������

static U8 gu8hasInitConfig = CONFIG_NOT_INITTED;
U8 db_hasInitConfig()
{
	return gu8hasInitConfig;
}

U8 db_readAllConfig(void)
{
	gu8hasInitConfig = CONFIG_INITTED;
	return gu8hasInitConfig;
}

U8 db_getCongfig(em_config_idx configIdx, U8* config)
{
	if (!db_hasInitConfig())
		return ERROR;

	switch (configIdx){
	case config_com_para:
		memcpy(config, (U8*)&gComConfig, sizeof(com_config_str));
		break;
	case config_gateway_id:
		memcpy(config, gu8gwyAdd, GATEWAY_OADD_LEN);
		break;
	case config_server_id:
		memcpy(config, gu8svrAdd, GATEWAY_SADD_LEN);
		break;
	default:
		break;
	}
	return NO_ERR;
}
