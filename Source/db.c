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

S8* fieldname[] = { "id", "maddr", "venderid", "protover", "mtype",\
					"channel", "vproto", "vaddr", "caddr", "build", \
					"unit", "room", "gateway" };
U8 fieldsize[] = {
	DB_MINFO_LEN_ROWID,
	DB_MINFO_LEN_METERADDR,
	DB_MINFO_LEN_VENDORID,
	DB_MINFO_LEN_PROTOVER,
	DB_MINFO_LEN_METERTYPE,
	DB_MINFO_LEN_CHANNEL,
	DB_MINFO_LEN_VALVEPROTOVER,
	DB_MINFO_LEN_VALVEADDR,
	DB_MINFO_LEN_CTLPANELADDR,
	DB_MINFO_LEN_BUILDID,
	DB_MINFO_LEN_UNITID,
	DB_MINFO_LEN_ROOMID,
	DB_MINFO_LEN_GATEWAYID
};

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
	if (STRLEN(data) != 2 * GATEWAY_OADD_LEN) return;
	inverseStrToBCD(data, 2 * GATEWAY_OADD_LEN, gu8gwyAdd, GATEWAY_OADD_LEN);
}

void readServerId(U8* data)
{
	if (STRLEN(data) != 2 * GATEWAY_OADD_LEN) return;
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
	if (*cnt <= 0) {
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
**	@lastRecId:	上一次查询后指向的记录号
*/
U8 db_getMeterInfo(U8* gatewayId, db_meterinfo_ptr pInfo, S32* rowCnt, S32* lastRecId)
{
	U8	lu8gatewayBuf[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	S32 actualCnt = 0, totalRows = 0;

	if (*lastRecId<0)
		return ERROR;

	totalRows = DbfRecordCount(pDbf);
	if (*lastRecId >= (totalRows-1))//如果上一次已到达最后一行, 则不必再向下遍历
		return NO_ERR;
	if(*lastRecId > 0)//如果是从0开始遍历, 说明是第一次进入, 不必向下移动
		*lastRecId += 1;
	DbfGotoRecord(*lastRecId, pDbf);
	while (actualCnt < *rowCnt && *lastRecId < totalRows) {
		DbfGotoRecord(*lastRecId, pDbf);
		if (DbfFieldGet(minfo_field_gatewayId, (char*)lu8gatewayBuf, pDbf) < 0)
			return ERROR;
		if (strcmp((const char*)lu8gatewayBuf, (const char*)gatewayId) == 0) {
			DBF_GETBASEFIELD(pInfo)
			actualCnt++;
			pInfo++;
		}
		*lastRecId += 1;
		if (*lastRecId >= totalRows) {
			break;
		}
	}
	*lastRecId = DbfGetCurrentRecord(pDbf);
    if (actualCnt)
		*rowCnt = actualCnt;
	else
		*rowCnt = 0;

	return NO_ERR;
}

/*
**	从数据库定位一行基础信息.
**	@gatewayId:	数据库中的信息指针
**	@pInfo:		用于缓存仪表信息(数组)
**	@rowCnt:	欲读取的行数, 程序返回实际读取的行数
*/
U8 db_getOneMeterInfo(U8* gatewayId, U16 meterId, db_meterinfo_ptr pInfo)
{
	S32 totalRec = 0, currentRec = 0;
	U16 lu16MeterId;
	U8 lu8MeterId[DB_MINFO_LEN_ROWID + 1] = { 0 };
	U8 lu8gatewayId[DB_MINFO_LEN_GATEWAYID + 1] = { 0 };
	totalRec = DbfRecordCount(pDbf);

	while (currentRec < totalRec) {
		DbfGotoRecord(currentRec, pDbf);
		DbfFieldGet(minfo_field_gatewayId, (S8*)lu8gatewayId, pDbf);
		if (strcmp((const char*)lu8gatewayId, (const char*)gatewayId) == 0) {
			DbfFieldGet(minfo_field_rowId, (char*)lu8MeterId, pDbf);
			lu16MeterId = Lib_atoi((const char*)lu8MeterId);
			if (meterId == lu16MeterId) {
				DBF_GETBASEFIELD(pInfo)
				return NO_ERR;
			}
		}
		currentRec += 1;
	}

	return ERROR;
}

U8 db_modifyGatewayId(U8* gatewayId)
{
	if (openDBF(DB_CONFIG_NAME) == ERROR)
		return ERROR;
	if (DbfGotoRecord(config_gateway_id, pDbf) == ERROR)
		return ERROR;
	if (DbfFieldSet(config_field_vale, (char*)gatewayId, pDbf) == ERROR)
		return ERROR;
	inverseStrToBCD(gatewayId, 2 * GATEWAY_OADD_LEN, gu8gwyAdd, GATEWAY_OADD_LEN);
	if (closeDBF() == ERROR)
		return ERROR;
	return NO_ERR;
}

U8 reCreateBaseInfoDBF()
{
	sFILE* fp = NULL;

	fp = FileOpen(DB_TMP_BASEINFO, "war");
	if (fp != NULL) {
		FileDelete(fp);
		FileClose(fp);
	}
	PRINT_LINE()
	if (DbfCreate(DB_TMP_BASEINFO, DB_MINFO_FIELD_CNT, fieldname, fieldsize) != DBF_OPER_OK)
		return ERROR;
	PRINT_LINE()
	return NO_ERR;
}

U8 db_storeTempBaseInfo(meter_row_ptr pProtoInfo, U16 infoCnt, U8* gatewayId)
{
	U16 i = 0;
	db_meterinfo_str dbInfoStr = { 0 };

	for (i = 0; i < infoCnt ; i++) {
		DbfRecordAppend(pDbf);
		protoBinToAscii(pProtoInfo+i, &dbInfoStr, gatewayId);
		DBF_SETBASEFIELD((&dbInfoStr))
	}
	return NO_ERR;
}

U8 db_getOneTempMeterInfo(U16 rowId, db_meterinfo_ptr pDbInfo)
{
	DbfGotoRecord(rowId, pDbf);
	DBF_GETBASEFIELD(pDbInfo)
	return NO_ERR;
}
