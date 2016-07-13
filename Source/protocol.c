/***************************************************
**	ģ�鹦��:	��֯�Լ�����Э��
**	ģ������:	protocol.c
**	���ߣ�		�α���
**	ǰ׺proto��ΪЭ���ģ��ĺ���;
**	W��Ϊ"д��", ����д����������ز���;
**	R��Ϊ"��ȡ", ����ȡ����������ز���;
**	X��Ϊ"ִ��", ���ü�����ִ��ĳ������;
**	A��Ϊ"����", ���������������ص�֡;
****************************************************
*/
#include "basedef.h"
#include "lib.h"
#include "protocol.h"
#include "db.h"


/*
**	��������������֡.
**	@sendBuf:	���ͻ�����
**	@sendLen:	���ͻ���������
**	@pProto:	����֡������Ϣ
*/
static void createFrame(U8 *sendBuf, U16 *sendLen, gateway_protocol_ptr pProto)
{
	U8 *pTemp = sendBuf;
	U16 lenFrame = 0;
	U8 *pHeadStart = NULL;//��Ϣͷ��ʼλ��, ���ڼ�����Ϣͷ��У��ֵ
	U8 *pBodyStart = NULL;//��Ϣ����ʼλ��, ���ڼ�����Ϣ���У��ֵ
	U16 u16dataLen = 0;

	memset(pTemp, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//ǰ����
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = GATEWAY_START;//��ʼ��
	pHeadStart = pTemp;
	lenFrame += GATEWAY_PREFIX_CNT + GATEWAY_START_CNT;

	*pTemp++ = PROTOCOL_VER;//�汾��

	memcpy(pTemp, pProto->SourceAddr, GATEWAY_SADD_LEN);//Դ��ַ
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->DestAddr, GATEWAY_OADD_LEN);//Ŀ���ַ
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->MsgIndex;//MID ��Ϣ����
	memcpy(pTemp, pProto->MsgLen, GATEWAY_MSGL_LEN);//��Ϣ�峤��
	pTemp += GATEWAY_MSGL_LEN;
	*pTemp++ = pProto->MsgType;//��Ϣ����

	memcpy(pTemp, pProto->ssmmhhDDMMYY, GATEWAY_TS_LEN);//ʱ��
	pTemp += GATEWAY_TS_LEN;

	*pTemp++ = countCheck(pHeadStart, GATEWAY_HEAD_LEN);//��ϢͷУ��
	pBodyStart = pTemp;//��Ϣ��У���ֽ�Ԥ��

	u16dataLen = (pProto->MsgLen[1] << 8 | pProto->MsgLen[0]);//��Ϣ�峤��
	memcpy(pTemp, pProto->pMsgBody, u16dataLen);//��Ϣ��
	pTemp += u16dataLen;
	*pTemp++ = countCheck(pBodyStart, u16dataLen);//��Ϣ��У��
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//Э��ͷ����+Э��ͷУ�鳤��
	lenFrame += u16dataLen;//��Ϣ�峤��
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//��Ϣ��У�鳤��+����������
	*sendLen = lenFrame;
}

/*
**	�����������ķ���֡.
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
**	@msgType:	����������֡����Ϣ����,
**				�����Ƿ������·�֡������
**	@seq:		�·�֡�����к�
*/
U8 protoA_retFrame(U8* buf, U16 bufSize, U8 msgType, U8 seq)
{
	U8 data;
	U16	retLen;

	//�ɹ�״̬
	data = buf[GATEWAY_STATCODE_OFFSET];
	if (GATEWAY_ASW_CODE_SUC != data)
		return ERROR;

	//��Ϣ����
	data = buf[GATEWAY_ASWCODE_OFFSET];
	if (data != msgType)
		return ERROR;

	//��Ϣ����
	if ((data == GAT_MT_CLT_SEND_MINFO) || (data == GAT_MT_CLT_MODIFY_SINFO)) {
		data = buf[GATEWAY_SEQCODE_OFFSET];//��Ϣ���к�
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
**	���ü�����ʱ��.
**	@gatewatId: ��������
**	@idLen:		�������ŵĳ���
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
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
**	�㲥����������.
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	U8 lu8gatewayId[GATEWAY_OADD_LEN] = { 0 };
	return protoW_setTime(lu8gatewayId, GATEWAY_OADD_LEN, buf, bufSize);
}

/*
**	�Ӽ���������֡, ��ȡ��������.
**	@gatewatId: ��������
**	@idLen:		�������ŵĳ���
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
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
**	�·����ַ��������.
**	@buf:		���ͻ���
**	@bufSize:	���ͻ��泤��
**	@gatewayId:	��������(�ѷ�ת��bin��ʽ)
**	@pBodyHead:	��Ϣͷ��ָ��
**	@pProtoInfo:��Ϣ���ָ��
*/
U8 protoW_issueMinfo(U8* buf, U16* bufSize, U8* gatewayId, \
	meterinfo_bodyHead_ptr pBodyHead, meter_row_ptr	pProtoInfo)
{
	gateway_protocol_str protoStr;
	U8 bufMsgBody[GATEWAY_FRAME_MAX_LEN] = {0};//�����ݴ���Ϣ��
	U16 bodyLen = 0;//��Ϣ�峤��

	if (gatewayId == NULL)
		return ERROR;

	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);

	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = pBodyHead->seq;

	bodyLen = GATEWAY_METERINFO_LEN*pBodyHead->thisRows + sizeof(meterinfo_bodyHead_str);
	memcpy(protoStr.MsgLen, (U8*)&bodyLen, GATEWAY_MSGL_LEN);//оƬʹ��С�˵�ַ, ���跭ת

	protoStr.MsgType = GAT_MT_SVR_SEND_MINFO;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);

	memcpy(bufMsgBody, (U8*)pBodyHead, sizeof(meterinfo_bodyHead_str));//������Ϣͷ
	memcpy(bufMsgBody + sizeof(meterinfo_bodyHead_str), (U8*)pProtoInfo, \
		pBodyHead->thisRows*sizeof(meter_row_str));//������Ϣ��
	protoStr.pMsgBody = bufMsgBody;//ָ��ָ����Ϣ��, �ֲ������ں�������ǰһֱ��Ч
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

/*
**	�����޸�һ��������ı��ַ.
**	@buf:		���ͻ�����
**	@bufSize:	���ͻ���������
**	@gatewayId:	�ѷ���ļ�����ID
**	@pProto:	����֡������Ϣ
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
**	�·�����ʱ���.
**	@buf:		���ͻ�����
**	@bufSize:	���ͻ���������
**	@gatewayId:	�ѷ���ļ�����ID
**	@timeCnt:	ʱ������
**	@pTimeNode:	ʱ���ָ��
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
