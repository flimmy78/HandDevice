/***************************************************
**	ģ�鹦��:	��֯�Լ�����Э��
**	ģ������:	protocol.c
**	���ߣ�		�α���
**	ǰ׺proto��ΪЭ���ģ��ĺ���;
**	W(write)��Ϊ"д��", ����д����������ز���;
**	R(read)��Ϊ"��ȡ", ����ȡ����������ز���;
**	X(eXcute)��Ϊ"ִ��", ���ü�����ִ��ĳ������;
**	A(analyze)��Ϊ"����", ���������������ص�֡;
****************************************************
*/
#include "basedef.h"
#include "lib.h"
#include "protocol.h"
#include "db.h"

/*
**	��������������֡.
**	@buf:	���ͻ�����
**	@bufSize:	���ͻ���������
**	@pProto:	����֡������Ϣ
*/
static void createFrame(U8 *buf, U16 *bufSize, gateway_protocol_ptr pProto)
{
	U8 *pTemp = buf;

	memcpy(pTemp, pProto->head_str.prefix, GATEWAY_PREFIX_CNT);//ǰ����
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = pProto->head_str.start;//��ʼ��
	*pTemp++ = pProto->head_str.protoVer;//�汾��

	memcpy(pTemp, pProto->head_str.SourceAddr, GATEWAY_SADD_LEN);//Դ��ַ
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->head_str.DestAddr, GATEWAY_OADD_LEN);//Ŀ���ַ
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->head_str.MsgID;//MID ��Ϣ����
	memcpy(pTemp, (U8*)(&pProto->head_str.bodyLen), sizeof(U16));//��Ϣ�峤��
	pTemp += sizeof(U16);
	*pTemp++ = pProto->head_str.MsgType;//��Ϣ����

	memcpy(pTemp, (U8*)(&(pProto->head_str.sysTime)), sizeof(sys_time_str));//ʱ��
	pTemp += sizeof(sys_time_str);

	*pTemp++ = pProto->head_str.HeadCheck;//��ϢͷУ��

	memcpy(pTemp, pProto->pMsgBody, pProto->head_str.bodyLen);//��Ϣ��
	pTemp += pProto->head_str.bodyLen;
	*pTemp++ = pProto->bodyChk;//��Ϣ��У��
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);//������
	*bufSize = sizeof(protocol_head_str) + pProto->head_str.bodyLen + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
}

U8 proto_assembleFrame(U8* buf, U16* bufSize, U8* gatewayId, \
	U8 MsgIndex, U8 MsgType, U16 bodyLen, U8* pMsgBody)
{
	gateway_protocol_str protoStr = { 0 };

	memset(protoStr.head_str.prefix, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//ǰ����
	memset(&(protoStr.head_str.start), GATEWAY_START, GATEWAY_START_CNT);//��ʼ��
	memset(&(protoStr.head_str.protoVer), GATEWAY_PROTOCOL_VER, 1);//Э��汾��

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
	lu16Length = sizeof(protocol_head_str) + protoHeadStr.bodyLen + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
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
	lu8data = countCheck(&(protoHeadStr.protoVer), \
		(&(protoHeadStr.HeadCheck) - &(protoHeadStr.protoVer)));
	if (lu8data != protoHeadStr.HeadCheck) {
		return ERROR;
	}

	//�����Ϣ��У��
	lu8data = countCheck(buf + sizeof(protocol_head_str), protoHeadStr.bodyLen);
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
U8 protoW_setTime(U8* buf, U16* bufSize, U8 *gatewayId)
{
	sys_time_str sysTime;
	readSysTime(&sysTime);
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_TIME_SET, sizeof(sys_time_str), (U8*)&sysTime);
}

/*
**	�㲥����������.
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	U8 gatewayId[GATEWAY_OADD_LEN] = { 0 };
	return protoW_setTime(buf, bufSize, gatewayId);
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
	U8 bufMsgBody[GATEWAY_FRAME_MAX_LEN] = {0};//�����ݴ���Ϣ��
	U16 bodyLen = 0;

	bodyLen = (sizeof(meter_row_str)*(pBodyHead->thisRows) + sizeof(meterinfo_bodyHead_str));
	memcpy(bufMsgBody, (U8*)pBodyHead, sizeof(meterinfo_bodyHead_str));//������Ϣͷ
	memcpy(bufMsgBody + sizeof(meterinfo_bodyHead_str), (U8*)pProtoInfo, \
		pBodyHead->thisRows * sizeof(meter_row_str));//������Ϣ��

	return proto_assembleFrame(buf, bufSize, gatewayId, pBodyHead->seq, \
		GAT_MT_SVR_SEND_MINFO, bodyLen, bufMsgBody);
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
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_MODIFY_SINFO, sizeof(meter_row_str), (U8*)pProtoInfo);
}

/*
**	�·�����ʱ���.
**	@buf:		���ͻ�����
**	@bufSize:	���ͻ���������
**	@gatewayId:	�ѷ���ļ�����ID
**	@timeCnt:	ʱ������
**	@pTimeNode:	ʱ���ָ��
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

	//�����Ϣ�Ϸ���
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
	U16	hisDataLen = 0;//��ʷ���ݳ��ȼ���(��������ʷ��Ϣͷ��: �������, ���к�, �洢ʱ��)
	const U16	protoHeadLen = sizeof(protocol_head_str);
	const U16	hisHeadLen = sizeof(hisdata_head_str);
	const U16	cj188Len = sizeof(CJ188_Format);
	const U16	fixMeterLen = sizeof(hisdata_meter_fix_str);
	const U16	fixValveLen = sizeof(hisdata_valve_fix_str);

	//�����Ϣ�Ϸ���
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&protoFrameStr, buf, protoHeadLen);//������Ϣͷ
	pMsgBody = buf + protoHeadLen;//ָ����Ϣ��
	memcpy((U8*)pBodyHead, pMsgBody, hisHeadLen);

	pMsgBody += hisHeadLen;//ָ����Ϣ��
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
	//�����Ϣ�Ϸ���
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;

	memcpy((U8*)pHisData, buf+sizeof(protocol_head_str), sizeof(tempControl_messure_hisdata_str));
	return NO_ERR;
}

