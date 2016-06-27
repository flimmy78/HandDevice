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


//创建集中器通讯桢, 私有函数
static void createFrame(U8 *sendBuf, u16 *sendLen, gateway_protocol_ptr pProto)
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
	memcpy(pTemp, pProto->MsgBody, u16dataLen);//消息体
	pTemp += u16dataLen;
	*pTemp++ = countCheck(pBodyStart, u16dataLen);//消息体校验
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//协议头长度+协议头校验长度
	lenFrame += u16dataLen;//消息体长度
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//消息体校验长度+结束符长度
	Lib_printf("[%s][%s][%d]lenFrame: %d", FILE_LINE, lenFrame);
	*sendLen = lenFrame;
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
	protoStr.MsgBody = protoStr.ssmmhhDDMMYY;
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

U8 protoA_setTime(U8* buf, U32 len)
{
	U8 *lu8pBuf = buf;
	lu8pBuf += 27;
	if (GATEWAY_ASW_CODE_SUC == (*lu8pBuf))
		return NO_ERR;
	else
		return ERROR;
}

/*
**	广播读集中器号.
**	@gatewatId: 集中器号
**	@idLen:		集中器号的长度
**	@buf:		发送帧
**	@bufSize:	发送帧的长度
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	U8 lu8gatewayId[GATEWAY_SADD_LEN] = { 0 };
	return protoW_setTime(lu8gatewayId, GATEWAY_SADD_LEN, buf, bufSize);
}