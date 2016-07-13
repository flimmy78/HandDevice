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
	U16 u16dataLen = 0;

	memset(pTemp, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//前导符
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = GATEWAY_START;//起始符
	pHeadStart = pTemp;
	lenFrame += GATEWAY_PREFIX_CNT + GATEWAY_START_CNT;

	*pTemp++ = PROTOCOL_VER;//版本号

	memcpy(pTemp, pProto->SourceAddr, GATEWAY_SADD_LEN);//源地址
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->DestAddr, GATEWAY_OADD_LEN);//目标地址
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->MsgIndex;//MID 消息序列
	memcpy(pTemp, pProto->MsgLen, GATEWAY_MSGL_LEN);//消息体长度
	pTemp += GATEWAY_MSGL_LEN;
	*pTemp++ = pProto->MsgType;//消息类型

	memcpy(pTemp, pProto->ssmmhhDDMMYY, GATEWAY_TS_LEN);//时间
	pTemp += GATEWAY_TS_LEN;

	*pTemp++ = countCheck(pHeadStart, GATEWAY_HEAD_LEN);//消息头校验
	pBodyStart = pTemp;//消息体校验字节预留

	u16dataLen = (pProto->MsgLen[1] << 8 | pProto->MsgLen[0]);//消息体长度
	memcpy(pTemp, pProto->pMsgBody, u16dataLen);//消息体
	pTemp += u16dataLen;
	*pTemp++ = countCheck(pBodyStart, u16dataLen);//消息体校验
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//协议头长度+协议头校验长度
	lenFrame += u16dataLen;//消息体长度
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//消息体校验长度+结束符长度
	*sendLen = lenFrame;
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
	U16	retLen;

	//成功状态
	data = buf[GATEWAY_STATCODE_OFFSET];
	if (GATEWAY_ASW_CODE_SUC != data)
		return ERROR;

	//消息类型
	data = buf[GATEWAY_ASWCODE_OFFSET];
	if (data != msgType)
		return ERROR;

	//消息长度
	if ((data == GAT_MT_CLT_SEND_MINFO) || (data == GAT_MT_CLT_MODIFY_SINFO)) {
		data = buf[GATEWAY_SEQCODE_OFFSET];//消息序列号
		if (data != seq)
			return ERROR;
		retLen = GATEWAY_WITHSEQ_LEN;
	}
	else {
		retLen = GATEWAY_WITHOUTSEQ_LEN;
	}

	if (bufSize != retLen)
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
	protoStr.MsgLen[0] = GATEWAY_TS_LEN;
	protoStr.MsgLen[1] = 0x00;
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
	U8 lu8gatewayId[GATEWAY_OADD_LEN] = { 0 };
	return protoW_setTime(lu8gatewayId, GATEWAY_OADD_LEN, buf, bufSize);
}

/*
**	从集中器返回帧, 读取集中器号.
**	@gatewatId: 集中器号
**	@idLen:		集中器号的长度
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
*/
U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize)
{
	if (bufSize<(GATEWAY_RETID_OFFSET+ GATEWAY_OADD_LEN) || idLen < GATEWAY_OADD_LEN) {
		return ERROR;
	}
	buf += GATEWAY_RETID_OFFSET;
	memset(gatewayId, 0, idLen);
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
	U16 bodyLen = 0;//消息体长度

	if (gatewayId == NULL)
		return ERROR;

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);

	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = pBodyHead->seq;

	bodyLen = GATEWAY_METERINFO_LEN*pBodyHead->thisRows + sizeof(meterinfo_bodyHead_str);
	memcpy(protoStr.MsgLen, (U8*)&bodyLen, GATEWAY_MSGL_LEN);//芯片使用小端地址, 无需翻转

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
	U16 msgLen = GATEWAY_METERINFO_LEN;

	if (gatewayId == NULL)
		return ERROR;

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = 0x00;
	memcpy(protoStr.MsgLen, (U8*)&msgLen, GATEWAY_MSGL_LEN);
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
	protoStr.MsgLen[0] = GATEWAY_TIMENODE_CNT_LEN + timeCnt*GATEWAY_TIMENODE_LEN;
	protoStr.MsgLen[1] = 0x00;
	protoStr.MsgType = GAT_MT_SVR_TIME_POINT;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);

	memcpy(bodyBuf, &timeCnt, GATEWAY_TIMENODE_CNT_LEN);
	memcpy(bodyBuf+ GATEWAY_TIMENODE_CNT_LEN, pTimeNode, timeCnt*GATEWAY_TIMENODE_LEN);
	protoStr.pMsgBody = bodyBuf;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}
