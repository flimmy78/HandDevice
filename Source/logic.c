/***************************************************
**	模块功能:	处理业务逻辑
**	模块名字:	logic.c
**	作者：		宋宝善
****************************************************
*/
#include "protocol.h"
#include "db.h"
#include "lib.h"
#include "logic.h"

void printBuf(U8* buf, U16 bufSize, const char* file, const char* func, u32 line)
{
#ifdef DEBUG
	U16 i = 0;
	Lib_printf("[%s][%s][%d]buf: ", file, func, line);
	for (i = 0;i < bufSize;i++)
		Lib_printf("%02X ", buf[i]);
	Lib_printf("\n");
#endif // DEBUG
}

U8 sendAndRead(U8* buf, U16* bufSize)
{
	sUART *pu;
	sUART comConfig;
	db_getCongfig(config_com_para, (U8*)&comConfig);

	pu = UartOpen(comConfig.baud, comConfig.mode, comConfig.device);//打开串口
	if (!pu)
		return ERROR;

	printBuf(buf, *bufSize, FILE_LINE);
	UartWrite(buf, *bufSize, 0, pu);
	*bufSize = UartRead(buf, 512, 2000, pu);
	printBuf(buf, *bufSize, FILE_LINE);

	UartClose(pu);

	return NO_ERR;
}

/*
**	设置集中器时间.
**	@gatewayId:	集中器编号(12位ASCII码)
*/
U8 logic_setTime(U8* gatewayId)
{
	//init lu8readBuf to GATEWAY_ASW_CODE_SUC's complement in case that GATEWAY_ASW_CODE_SUC is set to 0x00
	U8 lu8buf[512] = { ~GATEWAY_ASW_CODE_SUC };
	U16 lu8bufSize;
	U8 lu8ret;

	protoW_setTime(gatewayId, GATEWAY_OADD_LEN, lu8buf, &lu8bufSize);
	sendAndRead(lu8buf, &lu8bufSize);
	lu8ret = protoA_setTime(lu8buf, lu8bufSize);
	return lu8ret;
}

/*
**	广播读取集中器编号.
**	@gatewayId:	集中器编号
**	@idLen:		集中器编号长度
*/
U8 logic_readGatewayId(U8* gatewayId)
{
	U8 lu8buf[512] = { 0 };
	U16 lu8bufSize = 0;

	protoR_radioReadId(lu8buf, &lu8bufSize);
	sendAndRead(lu8buf, &lu8bufSize);
	protoA_radioReadId(gatewayId, GATEWAY_OADD_LEN, lu8buf, lu8bufSize);
	return NO_ERR;
}

U8 logic_issueMeterInfo(U8* gatewayId)
{
	db_meterinfo_str dbmeterInfoArray[GATEWAY_MAX_METERINFO_CNT] = { 0 };
	meter_row_str protoMeterInfoAarray[GATEWAY_MAX_METERINFO_CNT] = { 0 };
	meterinfo_bodyHead_str BodyHeadStr;

	const db_meterinfo_ptr pDbInfo = &dbmeterInfoArray[0];
	const meter_row_ptr	pProtoInfo = &protoMeterInfoAarray[0];
	const meterinfo_bodyHead_ptr pBodyHead = &BodyHeadStr;

	S32 totalRowCnt = 0, totalFrames = 0;
	S32 currentFrame = 0, j = 0;
	S32 rowCntPerFrame = GATEWAY_MAX_METERINFO_CNT;

	U8 buf[GATEWAY_FRAME_MAX_LEN] = { 0 };
	U16 bufSize = 0;

	if (openDBF(DB_CONFIG_NAME) == ERROR) return ERROR;//open dbf
	if (db_getMatchCnt(gatewayId, &totalRowCnt) != NO_ERR) {
		return ERROR;
	}
	if (db_gotoRecord0() != NO_ERR) {
		return ERROR;
	}

	totalFrames = (totalRowCnt / GATEWAY_MAX_METERINFO_CNT) + ((totalRowCnt%GATEWAY_MAX_METERINFO_CNT) ? 1 : 0);
	for (currentFrame = 0;currentFrame < totalFrames;currentFrame++) {//query db rows and issue to gateway
		rowCntPerFrame = GATEWAY_MAX_METERINFO_CNT;
		if (db_getMeterInfo(gatewayId, pDbInfo, &rowCntPerFrame) != NO_ERR) {
			return ERROR;
		}
		//dbInfo to protoInfo
		for (j = 0;j < rowCntPerFrame;j++) {
			asciiToProtoBin(pDbInfo + j, pProtoInfo + j);
		}
		//assemble frame
		pBodyHead->seq = (U8)currentFrame;
		pBodyHead->thisRows = (U8)rowCntPerFrame;
		pBodyHead->totalRows = (U8)totalRowCnt;
		protoW_issueMinfo(buf, &bufSize, gatewayId, pBodyHead, pProtoInfo);
		//write and read frame
		sendAndRead(buf, &bufSize);
		//return analyse result, if result is fail, return to user
		protoA_issueMinfo(buf, bufSize);
		//init dbInfo and protoInfo to 0
		memset(pDbInfo, 0, GATEWAY_MAX_METERINFO_CNT*sizeof(db_meterinfo_str));
		memset(pProtoInfo, 0, GATEWAY_MAX_METERINFO_CNT*sizeof(meter_row_str));
	}
	closeDBF();
	return NO_ERR;
}
