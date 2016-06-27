/***************************************************
**	模块功能:	处理业务逻辑
**	模块名字:	logic.c
**	作者：		宋宝善
****************************************************
*/
#include "protocol.h"
#include "logic.h"

U8 logic_setTime(U8* gatewayId)
{
	U8 lu8buf[512] = { 0 };
	U8 lu8readBuf[512] = { 0 };
	U16 lu8bufSize;
	U8 lu8ret;
	U32 lu32Len;
	sUART *pu;
	int i = 0;

	protoW_setTime(gatewayId, GATEWAY_OADD_LEN, lu8buf, &lu8bufSize);
	pu = UartOpen(9600, UART_MODE_8B_NONE_1S, DEVICE_RS485);//打开串口
	Lib_printf("pu is %p\n", pu);
	if (!pu)
		return ERROR;
	Lib_printf("[%s][%s][%d]sendbuf: ", FILE_LINE);
	for (i = 0;i < lu8bufSize;i++)
		Lib_printf("%02X ", lu8buf[i]);
	Lib_printf("\n");
	UartWrite(lu8buf, lu8bufSize, 0, pu);
	lu32Len = UartRead(lu8readBuf, 512, 2000, pu);
	for (i = 0;i < lu32Len;i++)
		Lib_printf("%02X ", lu8readBuf[i]);
	Lib_printf("\n");
	lu8ret = protoA_setTime(lu8readBuf, lu32Len);
	UartClose(pu);
	return lu8ret;
}
