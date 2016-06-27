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


//����������ͨѶ��, ˽�к���
static void createFrame(U8 *sendBuf, u16 *sendLen, gateway_protocol_ptr pProto)
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
	memcpy(pTemp, pProto->MsgBody, u16dataLen);//��Ϣ��
	pTemp += u16dataLen;
	*pTemp++ = countCheck(pBodyStart, u16dataLen);//��Ϣ��У��
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//Э��ͷ����+Э��ͷУ�鳤��
	lenFrame += u16dataLen;//��Ϣ�峤��
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//��Ϣ��У�鳤��+����������
	Lib_printf("[%s][%s][%d]lenFrame: %d", FILE_LINE, lenFrame);
	*sendLen = lenFrame;
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
**	�㲥����������.
**	@gatewatId: ��������
**	@idLen:		�������ŵĳ���
**	@buf:		����֡
**	@bufSize:	����֡�ĳ���
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	U8 lu8gatewayId[GATEWAY_SADD_LEN] = { 0 };
	return protoW_setTime(lu8gatewayId, GATEWAY_SADD_LEN, buf, bufSize);
}