/***************************************************
**	模块功能:	组织以及解析协议
**	模块名字:	protocol.c
**	作者：		宋宝善
**	前缀proto意为协议库模块的函数;
**	W(write)意为"写入", 即改写集中器的相关参数;
**	R(read)意为"读取", 即读取集中器的相关参数;
**	X(eXcute)意为"执行", 即让集中器执行某种命令;
**	A(analyze)意为"解析", 即解析集中器返回的帧;
****************************************************
*/
#include "basedef.h"
#include "lib.h"
#include "protocol.h"
#include "db.h"

/*
**	创建集中器发送帧.
**	@buf:	发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@pProto:	发送帧额外信息
*/
static void createFrame(U8 *buf, U16 *bufSize, gateway_protocol_ptr pProto)
{
	U8 *pTemp = buf;

	memcpy(pTemp, pProto->head_str.prefix, GATEWAY_PREFIX_CNT);//前导符
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = pProto->head_str.start;//起始符
	*pTemp++ = pProto->head_str.protoVer;//版本号

	memcpy(pTemp, pProto->head_str.SourceAddr, GATEWAY_SADD_LEN);//源地址
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->head_str.DestAddr, GATEWAY_OADD_LEN);//目标地址
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->head_str.MsgID;//MID 消息序列
	memcpy(pTemp, (U8*)(&pProto->head_str.bodyLen), sizeof(U16));//消息体长度
	pTemp += sizeof(U16);
	*pTemp++ = pProto->head_str.MsgType;//消息类型

	memcpy(pTemp, (U8*)(&(pProto->head_str.sysTime)), sizeof(sys_time_str));//时间
	pTemp += sizeof(sys_time_str);

	*pTemp++ = pProto->head_str.HeadCheck;//消息头校验

	memcpy(pTemp, pProto->pMsgBody, pProto->head_str.bodyLen);//消息体
	pTemp += pProto->head_str.bodyLen;
	*pTemp++ = pProto->bodyChk;//消息体校验
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);//结束符
	*bufSize = sizeof(protocol_head_str) + pProto->head_str.bodyLen + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
}

U8 proto_assembleFrame(U8* buf, U16* bufSize, U8* gatewayId, \
	U8 MsgIndex, U8 MsgType, U16 bodyLen, U8* pMsgBody)
{
	gateway_protocol_str protoStr = { 0 };

	memset(protoStr.head_str.prefix, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//前导符
	memset(&(protoStr.head_str.start), GATEWAY_START, GATEWAY_START_CNT);//开始符
	memset(&(protoStr.head_str.protoVer), GATEWAY_PROTOCOL_VER, 1);//协议版本号

	db_getCongfig(config_server_id, protoStr.head_str.SourceAddr);
	memcpy(protoStr.head_str.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	protoStr.head_str.MsgID = MsgIndex;
	protoStr.head_str.bodyLen = bodyLen;
	protoStr.head_str.MsgType = MsgType;
	readSysTime(&(protoStr.head_str.sysTime));
	protoStr.head_str.HeadCheck = countCheck(&(protoStr.head_str.protoVer), \
		(sizeof(protocol_head_str) - GATEWAY_PREFIX_CNT - GATEWAY_START_CNT));
	protoStr.pMsgBody = pMsgBody;
	protoStr.bodyChk = countCheck(protoStr.pMsgBody, bodyLen);
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

/*
**	分析集中器的返回帧长度及校验的合法性.
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
**	return:		合法返回NO_ERR; 非法返回ERROR, 下同
*/
U8 protoA_retFrameLen(U8* buf, U16 bufSize)
{
	U8	lu8data = 0;
	U16	lu16Length = 0;
	protocol_head_str protoHeadStr = { 0 };

	//检查消息头部长度
	if (bufSize < sizeof(protocol_head_str)) {
		return ERROR;
	}

	memcpy((U8*)&(protoHeadStr), buf, sizeof(protocol_head_str));

	//检查整个buf的长度
	lu16Length = sizeof(protocol_head_str) + protoHeadStr.bodyLen + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
	if (bufSize != lu16Length) {
		return ERROR;
	}

	//检查前导符, 开始符和协议版本
	if (protoHeadStr.prefix[0] != GATEWAY_PREFIX || \
		protoHeadStr.prefix[1] != GATEWAY_PREFIX || \
		protoHeadStr.start != GATEWAY_START || \
		protoHeadStr.protoVer != GATEWAY_PROTOCOL_VER) {
		return ERROR;
	}

	//检查结束符
	if (buf[bufSize-1] != GATEWAY_SUFIX ||\
		buf[bufSize - 2] != GATEWAY_SUFIX) {
		return ERROR;
	}

	//检查头部校验
	lu8data = countCheck(&(protoHeadStr.protoVer), \
		(&(protoHeadStr.HeadCheck) - &(protoHeadStr.protoVer)));
	if (lu8data != protoHeadStr.HeadCheck) {
		return ERROR;
	}

	//检查消息体校验
	lu8data = countCheck(buf + sizeof(protocol_head_str), protoHeadStr.bodyLen);
	if (buf[bufSize - 3] != lu8data) {
		return ERROR;
	}

	return NO_ERR;
}

/*
**	分析集中器的返回帧.
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
**	@msgType:	集中器返回帧的消息类型,
**				而不是服务器下发帧的类型
**	@seq:		下发帧的序列号
*/
U8 protoA_retFrame(U8* buf, U16 bufSize, U8 msgType, U8 seq)
{
	U8 data;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;

	//成功状态
	data = buf[GATEWAY_STATCODE_OFFSET];
	if (GATEWAY_ASW_CODE_SUC != data)
		return ERROR;

	//消息类型
	data = buf[GATEWAY_ASWCODE_OFFSET];
	if (data != msgType)
		return ERROR;

	return NO_ERR;
}

/*
**	设置集中器时间.
**	@gatewatId: 集中器号
**	@idLen:		集中器号的长度
**	@buf:		发送帧
**	@bufSize:	发送帧的长度
*/
U8 protoW_setTime(U8* buf, U16* bufSize, U8 *gatewayId)
{
	sys_time_str sysTime;
	readSysTime(&sysTime);
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_TIME_SET, sizeof(sys_time_str), (U8*)&sysTime);
}

/*
**	广播读集中器号.
**	@buf:		发送帧
**	@bufSize:	发送帧的长度
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	U8 gatewayId[GATEWAY_OADD_LEN] = { 0 };
	return protoW_setTime(buf, bufSize, gatewayId);
}

/*
**	从集中器返回帧, 读取集中器号.
**	@gatewatId: 集中器号, 从消息中的反序, 变为正序
**	@idLen:		集中器号的长度
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
*/
U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize)
{
	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	buf += GATEWAY_RETID_OFFSET;
	memcpy(gatewayId, buf, GATEWAY_OADD_LEN);
	inverseArray(gatewayId, GATEWAY_OADD_LEN);

	return NO_ERR;
}

/*
**	下发表地址给集中器.
**	@buf:		发送缓存
**	@bufSize:	发送缓存长度
**	@gatewayId:	集中器号(已翻转的bin格式)
**	@pBodyHead:	消息头的指针
**	@pProtoInfo:消息体的指针
*/
U8 protoW_issueMinfo(U8* buf, U16* bufSize, U8* gatewayId, \
	meterinfo_bodyHead_ptr pBodyHead, meter_row_ptr	pProtoInfo)
{
	U8 bufMsgBody[GATEWAY_FRAME_MAX_LEN] = {0};//用于暂存消息体
	U16 bodyLen = 0;

	bodyLen = (sizeof(meter_row_str)*(pBodyHead->thisRows) + sizeof(meterinfo_bodyHead_str));
	memcpy(bufMsgBody, (U8*)pBodyHead, sizeof(meterinfo_bodyHead_str));//复制消息头
	memcpy(bufMsgBody + sizeof(meterinfo_bodyHead_str), (U8*)pProtoInfo, \
		pBodyHead->thisRows * sizeof(meter_row_str));//复制消息体

	return proto_assembleFrame(buf, bufSize, gatewayId, pBodyHead->seq, \
		GAT_MT_SVR_SEND_MINFO, bodyLen, bufMsgBody);
}

/*
**	单独修改一个计量点的表地址.
**	@buf:		发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@gatewayId:	已反序的集中器ID
**	@pProto:	发送帧额外信息
*/
U8 protoW_modifyOneMinfo(U8* buf, U16* bufSize, U8* gatewayId, meter_row_ptr pProtoInfo)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_MODIFY_SINFO, sizeof(meter_row_str), (U8*)pProtoInfo);
}

/*
**	下发抄表时间点.
**	@buf:		发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@gatewayId:	已反序的集中器ID
**	@timeCnt:	时间点个数
**	@pTimeNode:	时间点指针
*/
U8 protoW_tmNode(U8* buf, U16* bufSize, U8* gatewayId, U8 timeCnt, time_node_ptr pTimeNode)
{
	U8	bodyBuf[GATEWAY_FRAME_MAX_LEN] = { 0 };
	U16 bodyLen = 0;

	bodyLen = GATEWAY_TIMENODE_CNT_LEN + timeCnt*sizeof(time_node_str);
	memcpy(bodyBuf, &timeCnt, GATEWAY_TIMENODE_CNT_LEN);
	memcpy(bodyBuf + GATEWAY_TIMENODE_CNT_LEN, (U8*)pTimeNode, timeCnt * sizeof(time_node_str));
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_TIME_POINT, bodyLen, bodyBuf);
}

U8 protoW_modifyGatewayId(U8* buf, U16* bufSize, U8* lu8originalId, U8* lu8targetId)
{
	U8	bodyBuf[GATEWAY_SADD_LEN + GATEWAY_OADD_LEN] = { 0 };
	U16 bodyLen = GATEWAY_SADD_LEN + GATEWAY_OADD_LEN;

	db_getCongfig(config_server_id, bodyBuf);
	memcpy(bodyBuf + GATEWAY_SADD_LEN, lu8targetId, GATEWAY_OADD_LEN);
	return proto_assembleFrame(buf, bufSize, lu8originalId, 0x00, \
		GAT_MT_SVR_MID, bodyLen, bodyBuf);
}

U8 protoR_GPRSParam(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_RD_CONFIG, 0, NULL);
}

U8 protoA_GPRSParam(U8* buf, U16 bufSize, gateway_params_ptr pParam)
{
	protocol_head_str headStr = { 0 };

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&headStr, buf, sizeof(protocol_head_str));
	memcpy((U8*)pParam, buf + sizeof(protocol_head_str), headStr.bodyLen);
	return NO_ERR;
}

U8 protoW_modifyGPRS(U8* buf, U16* bufSize, U8* gatewayId, gprs_param_ptr pGPRSParam)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CHIP, sizeof(gprs_param_str), (U8*)pGPRSParam);
}

U8 protoX_reboot(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_REBOOT, 0, NULL);
}

U8 protoW_rereadParam(U8* buf, U16* bufSize, U8* gatewayId, reread_param_ptr pParam)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_REREAD, sizeof(reread_param_str), (U8*)pParam);
}

U8 protoX_readMeterImmd(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CPY_IMMDT, 0, NULL);
}

U8 protoR_readBaseInfo(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_RD_ALLINFO, 0, NULL);
}

U8 protoA_readBaseInfo(U8* buf, U16 bufSize, U16* infoCnt, base_info_head_ptr pBodyHead, meter_row_ptr pInfo)
{
	protocol_head_str frameStr = { 0 };
	U8* pMsgBody = NULL;
	U8 i = 0;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&frameStr, buf, sizeof(protocol_head_str));//复制消息头
	pMsgBody = buf + sizeof(protocol_head_str);//指向消息体
	memcpy((U8*)pBodyHead, pMsgBody, sizeof(base_info_head_str));
	if (*infoCnt < pBodyHead->rowCnt)
		return ERROR;
	*infoCnt = pBodyHead->rowCnt;
	pMsgBody += sizeof(base_info_head_str);//指向消息体
	for (i = 0; i < *infoCnt ; i++)
		memcpy((U8*)(pInfo+i), pMsgBody+i* sizeof(meter_row_str), sizeof(meter_row_str));
	return NO_ERR;
}

U8 protoR_readMultiInfo(U8* buf, U16* bufSize, U8* gatewayId, U8* seq)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_1OF_MFRM, 1, seq);
}

U8 protoR_readHisData(U8* buf, U16* bufSize, U8* gatewayId, U8* timeNode)
{
	sys_time_str sysTime;
	readSysTime(&sysTime);
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_HISDATA, sizeof(sys_time_str), \
		(timeNode == NULL ? (U8*)&sysTime : timeNode));
}

U8 protoA_hisData(U8* buf, const U16 bufSize, U16* hisDataCnt, hisdata_head_ptr pBodyHead, tempControl_messure_hisdata_ptr pHisData)
{
	protocol_head_str protoFrameStr = { 0 };
	U8* pMsgBody = NULL;
	U8 dataCnt = 0;
	U8	idx = 0;
	U16	hisDataLen = 0;//历史数据长度计数(不包括历史信息头部: 后续标记, 序列号, 存储时间)
	const U16	protoHeadLen = sizeof(protocol_head_str);
	const U16	hisHeadLen = sizeof(hisdata_head_str);
	const U16	cj188Len = sizeof(CJ188_Format);
	const U16	fixMeterLen = sizeof(hisdata_meter_fix_str);
	const U16	fixValveLen = sizeof(hisdata_valve_fix_str);

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&protoFrameStr, buf, protoHeadLen);//复制消息头
	pMsgBody = buf + protoHeadLen;//指向消息体
	memcpy((U8*)pBodyHead, pMsgBody, hisHeadLen);

	pMsgBody += hisHeadLen;//指向消息体
	hisDataLen = (bufSize - protoHeadLen - hisHeadLen - GATEWAY_SUFIX_CNT - GATEWAY_EC_LEN);
	for (idx = 0, dataCnt = 0; dataCnt < hisDataLen; idx++, pHisData++) {
		memcpy(&(pHisData->fixMeter), pMsgBody, fixMeterLen);
		if (pHisData->fixMeter.meterDataLen == 0) {
			memset(&(pHisData->MeterData), ERROR, cj188Len);
		} else {
			memcpy(&(pHisData->MeterData), pMsgBody + fixMeterLen, cj188Len);
		}
		memcpy(&(pHisData->fixValve), pMsgBody + fixMeterLen + pHisData->fixMeter.meterDataLen, fixValveLen);
		pMsgBody += (fixMeterLen + fixValveLen + pHisData->fixMeter.meterDataLen);
		dataCnt += fixMeterLen + fixValveLen + pHisData->fixMeter.meterDataLen;
	}
	*hisDataCnt = idx;
	return NO_ERR;
}

U8 protoA_isHisDataSuc(tempControl_messure_hisdata_ptr pHisData)
{
	if ((pHisData->fixMeter.meterDataLen == 0) || \
		pHisData->fixValve.vState == 0xEE) {
		return ERROR;
	}
	return NO_ERR;
}

U8 protoX_readOneMeter(U8* buf, U16* bufSize, U8* gatewayId, U16* pMeterId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CP_1_METER, PROTO_LEN_ROWID, (U8*)pMeterId);
}

U8 protoA_readOneMeter(U8* buf, U16 bufSize, tempControl_messure_hisdata_ptr pHisData)
{
	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;

	memcpy((U8*)pHisData, buf+sizeof(protocol_head_str), sizeof(tempControl_messure_hisdata_str));
	return NO_ERR;
}

