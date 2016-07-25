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

	memset(pTemp, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//ǰ����
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = GATEWAY_START;//��ʼ��
	pHeadStart = pTemp;
	lenFrame += GATEWAY_PREFIX_CNT + GATEWAY_START_CNT;

	*pTemp++ = GATEWAY_PROTOCOL_VER;//�汾��

	memcpy(pTemp, pProto->SourceAddr, GATEWAY_SADD_LEN);//Դ��ַ
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->DestAddr, GATEWAY_OADD_LEN);//Ŀ���ַ
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->MsgIndex;//MID ��Ϣ����
	memcpy(pTemp, (U8*)&pProto->MsgLen, GATEWAY_MSGL_LEN);//��Ϣ�峤��
	pTemp += GATEWAY_MSGL_LEN;
	*pTemp++ = pProto->MsgType;//��Ϣ����

	memcpy(pTemp, pProto->ssmmhhDDMMYY, GATEWAY_TS_LEN);//ʱ��
	pTemp += GATEWAY_TS_LEN;

	*pTemp++ = countCheck(pHeadStart, GATEWAY_HEAD_LEN);//��ϢͷУ��
	pBodyStart = pTemp;//��Ϣ��У���ֽ�Ԥ��

	memcpy(pTemp, pProto->pMsgBody, pProto->MsgLen);//��Ϣ��
	pTemp += pProto->MsgLen;
	*pTemp++ = countCheck(pBodyStart, pProto->MsgLen);//��Ϣ��У��
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//Э��ͷ����+Э��ͷУ�鳤��
	lenFrame += pProto->MsgLen;//��Ϣ�峤��
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//��Ϣ��У�鳤��+����������
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
**	�����������ķ���֡���ȼ�У��ĺϷ���.
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
**	return:		�Ϸ�����NO_ERR; �Ƿ�����ERROR, ��ͬ
*/
U8 protoA_retFrameLen(U8* buf, U16 bufSize)
{
	U8	lu8data = 0;
	U16	lu16Length = 0;
	protocol_head_str protoHeadStr = { 0 };

	//�����Ϣͷ������
	if (bufSize < sizeof(protocol_head_str)) {
		return ERROR;
	}

	memcpy((U8*)&(protoHeadStr), buf, sizeof(protocol_head_str));

	//�������buf�ĳ���
	lu16Length = sizeof(protocol_head_str) + protoHeadStr.MsgLength + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
	if (bufSize != lu16Length) {
		return ERROR;
	}

	//���ǰ����, ��ʼ����Э��汾
	if (protoHeadStr.prefix[0] != GATEWAY_PREFIX || \
		protoHeadStr.prefix[1] != GATEWAY_PREFIX || \
		protoHeadStr.start != GATEWAY_START || \
		protoHeadStr.protoVer != GATEWAY_PROTOCOL_VER) {
		return ERROR;
	}

	//��������
	if (buf[bufSize-1] != GATEWAY_SUFIX ||\
		buf[bufSize - 2] != GATEWAY_SUFIX) {
		return ERROR;
	}

	//���ͷ��У��
	lu8data = countCheck(buf + GATEWAY_PREFIX_CNT + GATEWAY_START_CNT, GATEWAY_HEAD_LEN);
	if (lu8data != protoHeadStr.HeadCheck) {
		return ERROR;
	}

	//�����Ϣ��У��
	lu8data = countCheck(buf + sizeof(protocol_head_str), protoHeadStr.MsgLength);
	if (buf[bufSize - 3] != lu8data) {
		return ERROR;
	}

	return NO_ERR;
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

	//�����Ϣ�Ϸ���
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;

	//�ɹ�״̬
	data = buf[GATEWAY_STATCODE_OFFSET];
	if (GATEWAY_ASW_CODE_SUC != data)
		return ERROR;

	//��Ϣ����
	data = buf[GATEWAY_ASWCODE_OFFSET];
	if (data != msgType)
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
	protoStr.MsgLen = GATEWAY_TS_LEN;
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
**	�Ӽ���������֡, ��ȡ��������.
**	@gatewatId: ��������, ����Ϣ�еķ���, ��Ϊ����
**	@idLen:		�������ŵĳ���
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
*/
U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize)
{
	//�����Ϣ�Ϸ���
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	buf += GATEWAY_RETID_OFFSET;
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

	if (gatewayId == NULL)
		return ERROR;
	memcpy(protoStr.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	db_getCongfig(config_server_id, protoStr.SourceAddr);
	protoStr.MsgIndex = pBodyHead->seq;
	protoStr.MsgLen = GATEWAY_METERINFO_LEN*pBodyHead->thisRows + sizeof(meterinfo_bodyHead_str);
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

	//�����Ϣ�Ϸ���
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

	//�����Ϣ�Ϸ���
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&frameStr, buf, sizeof(protocol_head_str));//������Ϣͷ
	pMsgBody = buf + sizeof(protocol_head_str);//ָ����Ϣ��
	memcpy((U8*)pBodyHead, pMsgBody, sizeof(base_info_head_str));
	if (*infoCnt < pBodyHead->rowCnt)
		return ERROR;
	*infoCnt = pBodyHead->rowCnt;
	pMsgBody += sizeof(base_info_head_str);//ָ����Ϣ��
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

	//�����Ϣ�Ϸ���
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&protoFrameStr, buf, sizeof(protocol_head_str));//������Ϣͷ
	pMsgBody = buf + sizeof(protocol_head_str);//ָ����Ϣ��
	memcpy((U8*)pBodyHead, pMsgBody, sizeof(hisdata_head_str));
	rowCnt = (protoFrameStr.MsgLength - sizeof(hisdata_head_str)) / sizeof(tempControl_messure_hisdata_str);
	if (*hisDataCnt < rowCnt)
		return ERROR;
	*hisDataCnt = rowCnt;
	pMsgBody += sizeof(hisdata_head_str);//ָ����Ϣ��
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
