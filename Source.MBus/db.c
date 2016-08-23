/***************************************************
**	模块功能:	处理数据库相关操作
**	模块名字:	db.c
**	作者：		宋宝善
****************************************************
*/


#include "db.h"

static 	sDBF	dbf;
static	sDBF*	pDbf = &dbf;
static	sUART	gComConfig;//串口参数
static	U8		gu8meterType;//热表协议类型
static	U8		gu8valveType;//阀控协议类型
static	U8		gu8hasInitConfig = CONFIG_NOT_INITTED;

S8* configFieldName[] = { "id", "value" };
U8 configFieldSize[] = { DB_CONFIG_ID_LEN, DB_CONFIG_VALUE_LEN };

U8 openDBF(U8* dbfName)
{
	if (DbfOpen((char*)dbfName, pDbf) < 0) {//open
		return ERROR;
	}
	return NO_ERR;
}

U8 closeDBF(void)
{
	if (DbfClose(pDbf) < 0) {
		GUI_MessageBox("\n关闭数据库失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return ERROR;
	}
	return NO_ERR;
}

U8 db_setComConfig(U8 device, U32 baud, U8  mode)
{
	gComConfig.device = device;
	gComConfig.baud = baud;
	gComConfig.mode = mode;
	return NO_ERR;
}

U8 db_meterValveType(U8 meterType, U8 valveType)
{
	gu8meterType = meterType;
	gu8valveType = valveType;
	return NO_ERR;
}

U8 db_createConfigDb(void)
{
	sFILE *fp = FileOpen(DB_CONFIG_NAME, "war");

	if (fp != NULL) {
		FileDelete(fp);
	}
	FileClose(fp);
	if (DbfCreate(DB_CONFIG_NAME, CONFIG_FIELD_CNT, configFieldName, configFieldSize) < 0)
		return ERROR;

	return NO_ERR;
}

U8 db_writeConfig(void)
{
	S8	tmpStr[64] = { 0 };

	if (db_createConfigDb() == ERROR) {
		return ERROR;
	}

	if (openDBF(DB_CONFIG_NAME) == ERROR) {
		return ERROR;
	}

	DbfRecordAppend(pDbf);
	Lib_sprintf(tmpStr, "%d", config_com_para);
	DbfFieldSet(config_field_id, tmpStr, pDbf);
	memset(tmpStr, 0, sizeof(tmpStr));
	Lib_sprintf(tmpStr, "%d%c%d%c%d", gComConfig.baud, \
		CONFIG_DELIMITER, gComConfig.mode, CONFIG_DELIMITER, gComConfig.device);
	DbfFieldSet(config_field_vale, tmpStr, pDbf);
	memset(tmpStr, 0, sizeof(tmpStr));

	DbfRecordAppend(pDbf);
	Lib_sprintf(tmpStr, "%d", config_meter_type);
	DbfFieldSet(config_field_id, tmpStr, pDbf);
	memset(tmpStr, 0, sizeof(tmpStr));
	Lib_sprintf(tmpStr, "%d", gu8meterType);
	DbfFieldSet(config_field_vale, tmpStr, pDbf);
	memset(tmpStr, 0, sizeof(tmpStr));

	DbfRecordAppend(pDbf);
	Lib_sprintf(tmpStr, "%d", config_valve_type);
	DbfFieldSet(config_field_id, tmpStr, pDbf);
	memset(tmpStr, 0, sizeof(tmpStr));
	Lib_sprintf(tmpStr, "%d", gu8valveType);
	DbfFieldSet(config_field_vale, tmpStr, pDbf);
	memset(tmpStr, 0, sizeof(tmpStr));

	if (closeDBF() == ERROR) {
		return ERROR;
	}
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

U8 db_readAllConfig(void)
{
	S8 data[64] = { 0 };
	memset(&gComConfig, 0x00, sizeof(sUART));

	if (openDBF(DB_CONFIG_NAME) == ERROR) {
		GUI_MessageBox("\n配置信息丢失, 请重新设置!\n", "丢失", GUI_MESSAGEBOX_CF_MODAL);
		return ERROR;
	}

	if (DbfGotoRecord(config_com_para, pDbf) < 0) return ERROR;
	if (DbfFieldGet(config_field_vale, data, pDbf) < 0) return ERROR;
	readComConfig(data);
	memset(data, 0, sizeof(data));

	if (DbfGotoRecord(config_meter_type, pDbf) < 0) return ERROR;
	if (DbfFieldGet(config_field_vale, data, pDbf) < 0) return ERROR;
	gu8meterType = Lib_atoi((const char*)data);
	memset(data, 0, sizeof(data));

	if (DbfGotoRecord(config_valve_type, pDbf) < 0) return ERROR;
	if (DbfFieldGet(config_field_vale, data, pDbf) < 0) return ERROR;
	gu8valveType = Lib_atoi((const char*)data);

	gu8hasInitConfig = CONFIG_INITTED;
	if (closeDBF() == ERROR) {
		return ERROR;
	}
	return NO_ERR;
}

U8 db_getCongfig(u16 configIdx, U8* config)
{
	if (gu8hasInitConfig == CONFIG_NOT_INITTED)
		db_readAllConfig();

	switch (configIdx) {
	case config_com_para:
		memcpy(config, (U8*)&gComConfig, sizeof(sUART));
		break;
	case config_meter_type:
		*config = gu8meterType;
		break;
	case config_valve_type:
		*config = gu8valveType;
		break;
	default:
		break;
	}
	return NO_ERR;
}

/*
**	假设外部调用者已打开数据库, 读取表协议的配置信息 
**	@infoIdx:	协议配置表的"ID"值
**	@pDbFrame:	用于存储读到的配置信息指针
*/
U8 db_readOneFrameInfo(U32 infoIdx, db_mFrame_ptr pDbFrame)
{
	DbfGotoRecord(infoIdx, pDbf);
	if (DbfFieldGet(em_db_frame_id, (char*)pDbFrame->id, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_protoType, (char*)pDbFrame->prototype, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_meterType, (char*)pDbFrame->metertype, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_prefix, (char*)pDbFrame->prefix, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_prefixCnt, (char*)pDbFrame->prefixcnt, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_startChar, (char*)pDbFrame->startchar, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_endChar, (char*)pDbFrame->endchar, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_ctlCode, (char*)pDbFrame->controcode, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_dataId, (char*)pDbFrame->dataid, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_dataLen, (char*)pDbFrame->datalen, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_busType, (char*)pDbFrame->bustype, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_addrLen, (char*)pDbFrame->addresslen, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_supAddr, (char*)pDbFrame->supaddr, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_radioAddr, (char*)pDbFrame->radioaddr, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_baud, (char*)pDbFrame->baud, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_databit, (char*)pDbFrame->databit, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_parity, (char*)pDbFrame->parity, pDbf) == ERROR) return ERROR;
	if (DbfFieldGet(em_db_frame_stop, (char*)pDbFrame->stop, pDbf) == ERROR) return ERROR;

	return NO_ERR;
}

U8 db_getFrameInfo(U32 infoIdx, db_mFrame_ptr pDbFrame)
{
	U32 i = 0;
	U32 totalRow = 0;
	U8	tmpStr[6] = { 0 };

	if (openDBF(DB_FRAME_CONFIG_NAME) == ERROR) {
		return ERROR;
	}
	totalRow = DbfRecordCount(pDbf);
	for (i = 0; i < totalRow ; i++) {
		DbfGotoRecord(i, pDbf);
		DbfFieldGet(em_db_frame_id, (S8*)tmpStr, pDbf);
		if (Lib_atoi((const char*)tmpStr) == infoIdx) {
			if (db_readOneFrameInfo(infoIdx, pDbFrame) == ERROR) {
				goto retErr;
			}
			break;
		}
	}
	if (i == totalRow) {//如果i的值为totalRow, 则说明数据库中未找到信息
		goto retErr;
	}
	if (closeDBF() == ERROR) {
		return ERROR;
	}
	return NO_ERR;
retErr:
	closeDBF();
	return ERROR;
}

