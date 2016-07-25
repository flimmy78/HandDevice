/***************************************************
**	模块功能:	组织以及解析协议
**	模块名字:	protocol.c
**	作者：		宋宝善
**	前缀proto意为协议库模块的函数;
**	W意为"写入", 即改写集中器的相关参数;
**	R意为"读取", 即读取集中器的相关参数;
**	X意为"执行", 即让集中器执行某种命令;
**	A意为"解析", 即解析集中器返回的帧;
****************************************************
*/
#include "basedef.h"
#include "lib.h"
#include "protocol.h"
#include "db.h"


/*
**	创建集中器发送帧.
**	@sendBuf:	发送缓冲区
**	@sendLen:	发送缓冲区长度
**	@pProto:	发送帧额外信息
*/
static void createFrame(U8 *sendBuf, U16 *sendLen, gateway_protocol_ptr pProto)
{
	U8 *pTemp = sendBuf;
	U16 lenFrame = 0;
	U8 *pHeadStart = NULL;//消息头起始位置, 用于计算消息头的校验值
	U8 *pBodyStart = NULL;//消息体起始位置, 用于计算消息体的校验值

	memset(pTemp, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//前导符
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = GATEWAY_START;//起始符
	pHeadStart = pTemp;
	lenFrame += GATEWAY_PREFIX_CNT + GATEWAY_START_CNT;

	*pTemp++ = GATEWAY_PROTOCOL_VER;//版本号

	memcpy(pTemp, pProto->SourceAddr, GATEWAY_SADD_LEN);//源地址
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->DestAddr, GATEWAY_OADD_LEN);//目标地址
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->MsgIndex;//MID 消息序列
	memcpy(pTemp, (U8*)&pProto->MsgLen, GATEWAY_MSGL_LEN);//消息体长度
	pTemp += GATEWAY_MSGL_LEN;
	*pTemp++ = pProto->MsgType;//消息类型

	memcpy(pTemp, pProto->ssmmhhDDMMYY, GATEWAY_TS_LEN);//时间
	pTemp += GATEWAY_TS_LEN;

	*pTemp++ = countCheck(pHeadStart, GATEWAY_HEAD_LEN);//消息头校验
	pBodyStart = pTemp;//消息体校验字节预留

	memcpy(pTemp, pProto->pMsgBody, pProto->MsgLen);//消息体
	pTemp += pProto->MsgLen;
	*pTemp++ = countCheck(pBodyStart, pProto->MsgLen);//消息体校验
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//协议头长度+协议头校验长度
	lenFrame += pProto->MsgLen;//消息体长度
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//消息体校验长度+结束符长度
	*sendLen = lenFrame;
}

U8 proto_assembleFrame(U8* buf, U16* bufSize, U8* gatewayId, \
	U8 MsgIndex, U8 MsgType, U16 MsgLen, U8* pMsgBody)
{
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = MsgIndex;
	protoStr.MsgType = MsgType;
	protoStr.MsgLen = MsgLen;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = pMsgBody;
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
	lu16Length = sizeof(protocol_head_str) + protoHeadStr.MsgLength + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
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
	lu8data = countCheck(buf + GATEWAY_PREFIX_CNT + GATEWAY_START_CNT, GATEWAY_HEAD_LEN);
	if (lu8data != protoHeadStr.HeadCheck) {
		return ERROR;
	}

	//检查消息体校验
	lu8data = countCheck(buf + sizeof(protocol_head_str), protoHeadStr.MsgLength);
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
U8 protoW_setTime(U8 *gatewatId, U8 idLen, U8* buf, U16* bufSize)
{
	gateway_protocol_str protoStr;
	if (idLen != GATEWAY_OADD_LEN)
		return ERROR;

	if(gatewatId)
		memcpy(protoStr.DestAddr, gatewatId, GATEWAY_OADD_LEN);
	else
		db_getCongfig(config_gateway_id, protoStr.DestAddr);

	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgLen = GATEWAY_TS_LEN;
	protoStr.MsgType  = GAT_MT_SVR_TIME_SET;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = protoStr.ssmmhhDDMMYY;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

/*
**	广播读集中器号.
**	@buf:		发送帧
**	@bufSize:	发送帧的长度
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	gateway_protocol_str protoStr;

	memset(protoStr.DestAddr, 0, GATEWAY_OADD_LEN);
	memset(protoStr.SourceAddr, 0, GATEWAY_SADD_LEN);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgLen = GATEWAY_TS_LEN;
	protoStr.MsgType = GAT_MT_SVR_TIME_SET;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = protoStr.ssmmhhDDMMYY;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
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
	gateway_protocol_str protoStr;
	U8 bufMsgBody[GATEWAY_FRAME_MAX_LEN] = {0};//用于暂存消息体

	if (gatewayId == NULL)
		return ERROR;
	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = pBodyHead->seq;
	protoStr.MsgLen = GATEWAY_METERINFO_LEN*pBodyHead->thisRows + sizeof(meterinfo_bodyHead_str);
	protoStr.MsgType = GAT_MT_SVR_SEND_MINFO;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	memcpy(bufMsgBody, (U8*)pBodyHead, sizeof(meterinfo_bodyHead_str));//复制消息头
	memcpy(bufMsgBody + sizeof(meterinfo_bodyHead_str), (U8*)pProtoInfo, \
		pBodyHead->thisRows*sizeof(meter_row_str));//复制消息体
	protoStr.pMsgBody = bufMsgBody;//指针指向消息体, 局部变量在函数返回前一直有效
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
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
	gateway_protocol_str protoStr;

	if (gatewayId == NULL)
		return ERROR;
	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgLen = GATEWAY_METERINFO_LEN;
	protoStr.MsgType = GAT_MT_SVR_MODIFY_SINFO;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = (U8*)pProtoInfo;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

/*
**	下发抄表时间点.
**	@buf:		发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@gatewayId:	已反序的集中器ID
**	@timeCnt:	时间点个数
**	@pTimeNode:	时间点指针
*/
U8 protoW_tmNode(U8* buf, U16* bufSize, U8* gatewayId, U8 timeCnt, U8* pTimeNode)
{
	gateway_protocol_str protoStr;
	U8	bodyBuf[GATEWAY_TIMENODE_MAX_CNT + 1] = { 0 };
	if (gatewayId == NULL)
		return ERROR;

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgLen = GATEWAY_TIMENODE_CNT_LEN + timeCnt*GATEWAY_TIMENODE_LEN;
	protoStr.MsgType = GAT_MT_SVR_TIME_POINT;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);

	memcpy(bodyBuf, &timeCnt, GATEWAY_TIMENODE_CNT_LEN);
	memcpy(bodyBuf+ GATEWAY_TIMENODE_CNT_LEN, pTimeNode, timeCnt*GATEWAY_TIMENODE_LEN);
	protoStr.pMsgBody = bodyBuf;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoW_modifyGatewayId(U8* buf, U16* bufSize, U8* lu8originalId, U8* lu8targetId)
{
	gateway_protocol_str protoStr;
	U8	bodyBuf[2*GATEWAY_OADD_LEN] = { 0 };

	memcpy(protoStr.DestAddr, lu8originalId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgLen = GATEWAY_SADD_LEN + GATEWAY_OADD_LEN;
	protoStr.MsgType = GAT_MT_SVR_MID;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	memcpy(bodyBuf, protoStr.SourceAddr, GATEWAY_SADD_LEN);
	memcpy(bodyBuf + GATEWAY_SADD_LEN, lu8targetId, GATEWAY_OADD_LEN);
	protoStr.pMsgBody = bodyBuf;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoR_GPRSParam(U8* buf, U16* bufSize, U8* gatewayId)
{
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_RD_CONFIG;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = NULL;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoA_GPRSParam(U8* buf, U16 bufSize, gateway_params_ptr pParam)
{
	U16 msgLen = 0;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&msgLen, buf + GATEWAY_BODYLEN_OFFSET, GATEWAY_MSGL_LEN);
	memcpy((U8*)pParam, buf + GATEWAY_BODY_OFFSET, msgLen);
	return NO_ERR;
}

U8 protoW_modifyGPRS(U8* buf, U16* bufSize, U8* gatewayId, gprs_param_ptr pGPRSParam)
{
	gateway_protocol_str protoStr;

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgLen = sizeof(gprs_param_str);
	protoStr.MsgType = GAT_MT_SVR_CHIP;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = (U8*)pGPRSParam;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoX_reboot(U8* buf, U16* bufSize, U8* gatewayId)
{
	gateway_protocol_str protoStr = {0};

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_REBOOT;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = NULL;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoW_rereadParam(U8* buf, U16* bufSize, U8* gatewayId, reread_param_ptr pParam)
{
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_REREAD;
	protoStr.MsgLen = sizeof(reread_param_str);
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = (U8*)pParam;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoX_readMeterImmd(U8* buf, U16* bufSize, U8* gatewayId)
{
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_CPY_IMMDT;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = NULL;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoR_readBaseInfo(U8* buf, U16* bufSize, U8* gatewayId)
{
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_RD_ALLINFO;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = NULL;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
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
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_1OF_MFRM;
	protoStr.MsgLen = 1;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	protoStr.pMsgBody = seq;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoR_readHisData(U8* buf, U16* bufSize, U8* gatewayId, U8* timeNode)
{
	gateway_protocol_str protoStr = { 0 };

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	protoStr.MsgType = GAT_MT_SVR_HISDATA;
	protoStr.MsgLen = GATEWAY_TS_LEN;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	if (timeNode == NULL) {
		protoStr.pMsgBody = protoStr.ssmmhhDDMMYY;
	} else {
		protoStr.pMsgBody = timeNode;
	}
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoA_hisData(U8* buf, U16 bufSize, U16* hisDataCnt, hisdata_head_ptr pBodyHead, tempControl_messure_hisdata_ptr pHisData)
{
	protocol_head_str protoFrameStr = { 0 };
	U8* pMsgBody = NULL;
	U8 i = 0;
	U16 rowCnt = 0;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&protoFrameStr, buf, sizeof(protocol_head_str));//复制消息头
	pMsgBody = buf + sizeof(protocol_head_str);//指向消息体
	memcpy((U8*)pBodyHead, pMsgBody, sizeof(hisdata_head_str));
	rowCnt = (protoFrameStr.MsgLength - sizeof(hisdata_head_str)) / sizeof(tempControl_messure_hisdata_str);
	if (*hisDataCnt < rowCnt)
		return ERROR;
	*hisDataCnt = rowCnt;
	pMsgBody += sizeof(hisdata_head_str);//指向消息体
	for (i = 0; i < rowCnt; i++) {
		memcpy((pHisData + i), (tempControl_messure_hisdata_ptr)pMsgBody + i, \
			sizeof(tempControl_messure_hisdata_str));
	}
	return NO_ERR;
}

U8 protoA_hisDataSuc(tempControl_messure_hisdata_ptr pHisData)
{
	if (pHisData->vState==0xEE) {
		return ERROR;
	}
	return NO_ERR;
}

U8 protoX_readOneMeter(U8* buf, U16* bufSize, U8* gatewayId, U16* pMeterId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CP_1_METER, PROTO_LEN_ROWID, (U8*)pMeterId);
}
