/***************************************************
**	ģ�鹦��:	����ҵ���߼�
**	ģ������:	logic.c
**	���ߣ�		�α���
****************************************************
*/
#include "protocol.h"
#include "db.h"
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

	pu = UartOpen(comConfig.baud, comConfig.mode, comConfig.device);//�򿪴���
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
**	���ü�����ʱ��.
**	@gatewayId:	���������(12λASCII��)
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
**	�㲥��ȡ���������.
**	@gatewayId:	���������
**	@idLen:		��������ų���
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
