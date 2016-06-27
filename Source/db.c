/***************************************************
**	模块功能:	处理数据库相关操作
**	模块名字:	db.c
**	作者：		宋宝善
****************************************************
*/
#include "protocol.h"
#include "db.h"

static com_config_str gComConfig;//串口设置
static U8 gu8svrAdd[GATEWAY_SADD_LEN] = {0};//主站编号
static U8 gu8gwyAdd[GATEWAY_OADD_LEN] = {0};//集中器编号

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
