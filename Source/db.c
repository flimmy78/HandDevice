/***************************************************
**	模块功能:	处理数据库相关操作
**	模块名字:	db.c
**	作者：		宋宝善
****************************************************
*/
#include "protocol.h"
#include "logic.h"
#include "lib.h"
#include "db.h"

static 	sDBF	dbf;
static	sDBF*	pDbf = &dbf;
static sUART gComConfig;//com config
static U8 gu8svrAdd[GATEWAY_SADD_LEN] = {0};//主站编号, 反序的
static U8 gu8gwyAdd[GATEWAY_OADD_LEN] = {0};//集中器编号, 反序的

static U8 gu8hasInitConfig = CONFIG_NOT_INITTED;
U8 db_hasInitConfig()
{
	return gu8hasInitConfig;
}

U8 openDBF(U8* dbfName)
{
	if (DbfOpen((char*)dbfName, pDbf) < 0) {//open
		GUI_MessageBox("\n配置信息丢失, 请重新设置!\n", "丢失", GUI_MESSAGEBOX_CF_MODAL);
		return ERROR;
	}
	return NO_ERR;
}

U8 closeDBF(void)
{
	if (DbfClose(pDbf)<0) {
		GUI_MessageBox("\n关闭数据库失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return ERROR;
	}
	return NO_ERR;
}

U8 db_gotoRecord0()
{
	if (DBF_OPER_OK != DbfGotoRecord(0, pDbf) < 0) {
		return ERROR;
	}
	return NO_ERR;
}

U8 db_writeConfigDb(void)
{
	//DbfCreate();
	return NO_ERR;
}

static void readComConfig(S8* data)
{
	S8* pData = data;
	U16 configLen = 0;
	U8 tempBuf[32] = { 0 };
	//read baud
	while (pData[configLen] != CONFIG_DELIMITER) configLen++;
	memcpy(tempBuf, pData, configLen);
	gComConfig.baud = Lib_atoi((const char*)tempBuf);
	pData += configLen + 1;
	memset(tempBuf, 0, 32);
	//read mode
	configLen = 0;
	while (pData[configLen] != CONFIG_DELIMITER) configLen++;
	memcpy(tempBuf, pData, configLen);
	gComConfig.mode = Lib_atoi((const char*)tempBuf);
	pData += configLen + 1;
	memset(tempBuf, 0, 32);
	//read device
	configLen = 0;
	while (pData[configLen] != '\0') configLen++;
	memcpy(tempBuf, pData, configLen);
	gComConfig.device = Lib_atoi((const char*)tempBuf);
}

void readGatewayId(U8* data)
{
	if (strlen((const char*)data) != 2 * GATEWAY_OADD_LEN) return;
	inverseStrToBCD(data, 2 * GATEWAY_OADD_LEN, gu8gwyAdd, GATEWAY_OADD_LEN);
}

void readServerId(U8* data)
{
	if (strlen((const char*)data) != 2 * GATEWAY_OADD_LEN) return;
	inverseStrToBCD(data, 2 * GATEWAY_OADD_LEN, gu8svrAdd, GATEWAY_OADD_LEN);
}

U8 db_readAllConfig(void)
{
	S8 data[64] = { 0 };
	s32 iRet = 0;
	memset(&gComConfig, 0x00, sizeof(sUART));

	if (openDBF(DB_CONFIG_NAME) == ERROR) return ERROR;//open dbf
	//read com config
	iRet = DbfGotoRecord(config_com_para, pDbf);
	if (iRet<0) return ERROR;
	iRet = DbfFieldGet(config_field_vale, data, pDbf);
	if (iRet<0) return ERROR;
	readComConfig(data);
	//read gatewayId
	iRet = DbfGotoRecord(config_gateway_id, pDbf);
	if (iRet < 0) return ERROR;
	iRet = DbfFieldGet(config_field_vale, data, pDbf);
	if (iRet < 0) return ERROR;
	readGatewayId((U8*)data);
	//read serverId
	iRet = DbfGotoRecord(config_server_id, pDbf);
	if (iRet < 0) return ERROR;
	iRet = DbfFieldGet(config_field_vale, data, pDbf);
	if (iRet < 0) return ERROR;
	readServerId((U8*)data);

	gu8hasInitConfig = CONFIG_INITTED;
	closeDBF();
	return NO_ERR;
}

U8 db_getCongfig(u16 configIdx, U8* config)
{
	if (!db_hasInitConfig())
		db_readAllConfig();

	switch (configIdx){
	case config_com_para:
		memcpy(config, (U8*)&gComConfig, sizeof(sUART));
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

U8 db_getMatchCnt(U8* gatewayId, S32* cnt)
{
	*cnt = DbfGetMatchCount(minfo_field_gatewayId, (char*)gatewayId, DBF_LOCATE_MATCH_ALL, pDbf);
	if (*cnt < 0) {
		return ERROR;
	}
	return NO_ERR;
}

/*
**	从数据库读取表地址.
**	本函数借助了创伦库中处理DBF的一个机制,
**	读取了一行后, currentRecord就不再变化,
**	否则得记住当前读到第几行
**	@gatewayId:	数据库中的信息指针
**	@pInfo:		用于缓存仪表信息(数组)
**	@rowCnt:	欲读取的行数, 程序返回实际读取的行数
*/
U8 db_getMeterInfo(U8* gatewayId, db_meterinfo_ptr pInfo, S32* rowCnt)
{
	S32 i = 0, actualCnt = 0;

	while (actualCnt < *rowCnt) {
		i = DbfRecordLocate(minfo_field_gatewayId, (char*)gatewayId, DBF_LOCATE_DOWN, DBF_LOCATE_MATCH_ALL, pDbf);
		if (i >= 0) {
			DbfFieldGet(minfo_field_rowId, (char*)pInfo->rowId, pDbf);
			DbfFieldGet(minfo_field_meterAddr, (char*)pInfo->meterAddr, pDbf);
			DbfFieldGet(minfo_field_vendorId, (char*)pInfo->vendorId, pDbf);
			DbfFieldGet(minfo_field_protoVer, (char*)pInfo->protoVer, pDbf);
			DbfFieldGet(minfo_field_meterType, (char*)pInfo->meterType, pDbf);
			DbfFieldGet(minfo_field_channel, (char*)pInfo->channel, pDbf);
			DbfFieldGet(minfo_field_valveProtoVer, (char*)pInfo->valveProtoVer, pDbf);
			DbfFieldGet(minfo_field_valveAddr, (char*)pInfo->valveAddr, pDbf);
			DbfFieldGet(minfo_field_controlPanelAddr, (char*)pInfo->controlPanelAddr, pDbf);
			DbfFieldGet(minfo_field_buildId, (char*)pInfo->buildId, pDbf);
			DbfFieldGet(minfo_field_unitId, (char*)pInfo->unitId, pDbf);
			DbfFieldGet(minfo_field_roomId, (char*)pInfo->roomId, pDbf);
			actualCnt++;
			pInfo++;
		} else {
			return ERROR;
		}
	}
	if (actualCnt)
		*rowCnt = actualCnt;
	else
		*rowCnt = 0;

	return NO_ERR;
}
