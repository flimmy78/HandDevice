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

/*
**	向串口读写数据.
**	@buf:		发送与接收数据缓冲区
**	@bufSize:	缓冲区长度
*/
U8 logic_sendAndRead(U8* buf, U16* bufSize)
{
	sUART *pu;
	sUART comConfig;
	db_getCongfig(config_com_para, (U8*)&comConfig);

	pu = UartOpen(comConfig.baud, comConfig.mode, comConfig.device);//打开串口
	if (!pu)
		return ERROR;

	UartWrite(buf, *bufSize, 0, pu);
	*bufSize = UartRead(buf, 512, 2000, pu);
	if (*bufSize == 0) {//如果超时后没有读到数据, 返回错误
		UartClose(pu);
		return ERROR;
	}

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
	U16 lu8bufSize = 0;

	protoW_setTime(gatewayId, GATEWAY_OADD_LEN, lu8buf, &lu8bufSize);
	if (logic_sendAndRead(lu8buf, &lu8bufSize) == ERROR) {
		return ERROR;
	}
	return protoA_retFrame(lu8buf, lu8bufSize, GAT_MT_CLT_TIME_SET, 0);
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
	if (logic_sendAndRead(lu8buf, &lu8bufSize) == ERROR) {
		return ERROR;
	}
	protoA_radioReadId(gatewayId, GATEWAY_OADD_LEN, lu8buf, lu8bufSize);
	return NO_ERR;
}

/*
**	下发表地址给集中器.
**	@gatewayId:	集中器编号, 12位正序的ascii码
*/
U8 logic_issueMeterInfo(U8* gatewayId)
{
	db_meterinfo_str dbmeterInfoArray[GATEWAY_MAX_METERINFO_CNT] = { 0 };
	meter_row_str protoMeterInfoAarray[GATEWAY_MAX_METERINFO_CNT] = { 0 };
	meterinfo_bodyHead_str BodyHeadStr;

	const db_meterinfo_ptr pDbInfo = &dbmeterInfoArray[0];
	const meter_row_ptr	pProtoInfo = &protoMeterInfoAarray[0];
	const meterinfo_bodyHead_ptr pBodyHead = &BodyHeadStr;
	U8 lu8gatewayId[GATEWAY_OADD_LEN] = { 0 };

	S32 totalRowCnt = 0, totalFrames = 0;
	S32 currentFrame = 0, j = 0;
	S32 rowCntPerFrame = 0;
	S32 alreadySendRows = 0;
	S32 leftRows = 0;
	S32 lastRecId = 0;

	U8 buf[GATEWAY_FRAME_MAX_LEN] = { 0 };
	U16 bufSize = 0;

	if (openDBF(DB_BASEINFO_NAME) == ERROR) return ERROR;//open dbf
	if (db_getMatchCnt(gatewayId, &totalRowCnt) == ERROR) {
		return ERROR;
	}
	if (db_gotoRecord0() == ERROR) {
		return ERROR;
	}

	totalFrames = (totalRowCnt / GATEWAY_MAX_METERINFO_CNT) + ((totalRowCnt%GATEWAY_MAX_METERINFO_CNT) ? 1 : 0);
	if (db_gotoRecord0() == ERROR) {
		return ERROR;
	}

	Lib_printf("totalFrames: %d\n", totalFrames);
	for (currentFrame = 0, lastRecId = 0, alreadySendRows = 0;currentFrame < totalFrames;\
		currentFrame++, alreadySendRows += rowCntPerFrame) {
		leftRows = totalRowCnt - alreadySendRows;
		rowCntPerFrame = leftRows>GATEWAY_MAX_METERINFO_CNT ? GATEWAY_MAX_METERINFO_CNT: leftRows;
		Lib_printf("rowCntPerFrame: %d\n", rowCntPerFrame);
		if (db_getMeterInfo(gatewayId, pDbInfo, &rowCntPerFrame, &lastRecId) == ERROR)
			return ERROR;
		
		//dbInfo to protoInfo
		for (j = 0;j < rowCntPerFrame;j++) {
			asciiToProtoBin(pDbInfo + j, pProtoInfo + j);
		}
		//assemble frame
		pBodyHead->seq = (U8)currentFrame;
		pBodyHead->thisRows = (U8)rowCntPerFrame;
		pBodyHead->totalRows = (U8)totalRowCnt;
		inverseStrToBCD(gatewayId, 2*GATEWAY_OADD_LEN, lu8gatewayId, GATEWAY_OADD_LEN);
		protoW_issueMinfo(buf, &bufSize, lu8gatewayId, pBodyHead, pProtoInfo);
		//write and read frame
		if (logic_sendAndRead(buf, &bufSize) == ERROR)
			return ERROR;
		//return analyse result, if result is fail, return to user
		if (protoA_retFrame(buf, bufSize, GAT_MT_CLT_SEND_MINFO, pBodyHead->seq) == ERROR)
			return ERROR;
		
		//init dbInfo and protoInfo to 0
		memset(pDbInfo, 0, GATEWAY_MAX_METERINFO_CNT*sizeof(db_meterinfo_str));
		memset(pProtoInfo, 0, GATEWAY_MAX_METERINFO_CNT*sizeof(meter_row_str));
	}
	closeDBF();
	return NO_ERR;
}

U8 logic_queryOneMeterInfo(U8* gatewayId, U16 meterId, db_meterinfo_ptr pDbInfo)
{
	if (openDBF(DB_BASEINFO_NAME) == ERROR) return ERROR;//open dbf
	if (db_gotoRecord0() != NO_ERR) {
		return ERROR;
	}
	if (db_getOneMeterInfo(gatewayId, meterId, pDbInfo) == ERROR) {
		return ERROR;
	}
	closeDBF();
	return NO_ERR;
}

U8 logic_issueOneMeterInfo(U8* gatewayId, db_meterinfo_ptr pDbInfo)
{
	U8 lu8gatewayId[GATEWAY_OADD_LEN] = { 0 };
	meter_row_str protoMeterInfo;
	U8 buf[GATEWAY_FRAME_MAX_LEN] = { 0 };
	U16 bufSize = 0;

	inverseStrToBCD(gatewayId, 2 * GATEWAY_OADD_LEN, lu8gatewayId, GATEWAY_OADD_LEN);

	asciiToProtoBin(pDbInfo, &protoMeterInfo);
	if (protoW_modifyOneMinfo(buf, &bufSize, lu8gatewayId, &protoMeterInfo) == ERROR)
		return ERROR;
	if(logic_sendAndRead(buf, &bufSize) == ERROR)
		return ERROR;
	if (protoA_retFrame(buf, bufSize, GAT_MT_CLT_MODIFY_SINFO, 0) == ERROR)
		return ERROR;
	
	return NO_ERR;
}


U8 logic_genTimeNodesStr(U8* buf, U16 bufSize, U8* startTime, U16 nodes)
{
	time_node_str timeNodes[MAX_TIME_NODE] ={{0,0}};
	if (calcTimeNode(buf, bufSize, startTime, nodes, &timeNodes[0]) == ERROR)
		return ERROR;
	return NO_ERR;
}

U8 logic_issueTimeNodes(U8* buf, U16 bufSize, U8* gatewayId)
{
	U16 timeCnt = 0;
	U8	lu8gatewayId[GATEWAY_OADD_LEN] = { 0 };
	U8	pTimeNode[128] = { 0 };
	U8	bufSend[1024] = { 0 };
	U16	bufSendSize = 0;

	inverseStrToBCD(gatewayId, 2 * GATEWAY_OADD_LEN, lu8gatewayId, GATEWAY_OADD_LEN);
	strToTimeNode(buf, bufSize, pTimeNode, &timeCnt);
	protoW_tmNode(bufSend, &bufSendSize, lu8gatewayId, timeCnt, pTimeNode);
	logic_sendAndRead(bufSend, &bufSendSize);
	if (protoA_retFrame(bufSend, bufSendSize, GAT_MT_CLT_TIME_POINT, 0) == ERROR)
		return ERROR;
	return NO_ERR;
}

U8 logic_modifyGatewayId(U8* originalId, U8* targetId)
{
	U8 lu8originalId[GATEWAY_OADD_LEN] = { 0 };
	U8 lu8targetId[GATEWAY_OADD_LEN] = { 0 };
	U8 buf[256] = { 0 };
	U16 bufSize = 0;

	inverseStrToBCD(originalId, STRLEN(originalId), lu8originalId, GATEWAY_OADD_LEN);
	inverseStrToBCD(targetId, STRLEN(targetId), lu8targetId, GATEWAY_OADD_LEN);

	protoW_modifyGatewayId(buf, &bufSize, lu8originalId, lu8targetId);
	logic_sendAndRead(buf, &bufSize);
	if (protoA_retFrame(buf, bufSize, GAT_MT_CLT_MID, 0) == ERROR)
		return ERROR;
	if (db_modifyGatewayId(targetId) == ERROR)
		return ERROR; 
	PRINT_LINE()
	return NO_ERR;
}

