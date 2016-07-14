/***************************************************
**	模块功能：	常用函数
**	模块名字:	lib.c
**	作者：		宋宝善
****************************************************
*/
#include "basedef.h"
#include "protocol.h"
#include "lib.h"
#include "db.h"
#include "user.h"

static const U8 spaces[] = { ' ', '\t', '\n', '\r', '\0' };

//读取系统时间
U8 readSysTime(sys_time_ptr pTime)
{
	RtcGetDate(&(pTime->u8year), &(pTime->u8month), &(pTime->u8day));
	RtcGetTime(&(pTime->u8hour), &(pTime->u8minute), &(pTime->u8second));

	pTime->u8year	= HEX_TO_BCD(pTime->u8year);
	pTime->u8month  = HEX_TO_BCD(pTime->u8month);
	pTime->u8day    = HEX_TO_BCD(pTime->u8day);
	pTime->u8hour	= HEX_TO_BCD(pTime->u8hour);
	pTime->u8minute = HEX_TO_BCD(pTime->u8minute);
	pTime->u8second = HEX_TO_BCD(pTime->u8second);

	return 0;
}

//校验, 累加和取反加一
U8 countCheck(U8 *data, U16 len)
{
	U8 cs = 0;
	U16 i;
	for (i = 0;i < len;i++, data++)
		cs += *data;

	return ((~cs) + 1);
}

/*
**	判断字符是否是空格.
**	@s:	字符
*/
U8 isSpace(U8 s)
{
	U16 i = 0;
	U16 arrayLen = sizeof(spaces);
	for (i = 0;i < arrayLen;i++)
		if(s == spaces[i])
			return IS_SPACE;
	return NOT_SPACE;
}

/*
**	去掉字符串中的空格.
**	@s:		字符串
**	@len:	字符串长度
*/
U8 trimSpace(U8* s, U16 len)
{
	U8* p = s;
	U8	buf[256] = {0};
	U16 i=0 ,j=0;
	for (i = 0, j = 0;i < len; p++, i++) {
		if(!isSpace(*p)) {
			buf[j] = *p;
			j++;
		}
	}
	memset(s, 0x00, len);
	memcpy(s, buf, j+1);
	return NO_ERR;
}

/*
**	检查字符串是否都是数字.
**	@s:		字符串
**	@len:	字符串长度
*/
U8 isNumber(U8* s, U16 len)
{
	U16 i = 0;
	for (i = 0;i < len; s++, i++) {
		if (!isdigit(*s)) {
			return ERROR;
		}
	}

	return NO_ERR;
}

/*
**	反序输入的字符串至BCD码.
**	@s:		源字符串
**	@sLen:	源字符串长度
**	@t:		目标字符串
**	@tLen:	目标字符串长度
*/
U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen)
{
	U16 i = 0;

	if (sLen % 2)
		return ERROR;

	for (i = 0;i < sLen/2;i++, s += 2) {//convert ascii to hex each two chars
		t[i] = (ASCII_TO_HEX(s[0]) << 4 | ASCII_TO_HEX(s[1]));
	}

	inverseArray(t, i);
	return NO_ERR;
}

void inverseArray(U8* buf, U16 bufSize)
{
	U16 i = 0;
	for (i = 0;i < bufSize / 2; i++) {//swap two symmetric elements
		buf[i] = buf[i] ^ buf[bufSize - i - 1];
		buf[bufSize - i - 1] = buf[i] ^ buf[bufSize - i - 1];
		buf[i] = buf[i] ^ buf[bufSize - i - 1];
	}
}

/*
**	将数字字符串的最前面的'0'去掉.
**	@buf:		字符串
**	@bufSize:	字符串长度
*/
void trimZero(U8* buf, U8 bufSize)
{
	U16 i = 0;
	U8* lu8pBuf = buf;
	U8 tmpBuf[512] = { 0 };
	while (i < bufSize && (*lu8pBuf == '0')) {
		i++;
		lu8pBuf++;
	}
	memcpy(tmpBuf, lu8pBuf, bufSize - i);
	memset(buf, 0, bufSize);
	memcpy(buf, tmpBuf, bufSize - i);
}

/*
**	将不足12位的集中器号前面补零.
**	@data:	用户输入的集中器号
*/
void supplementTo12(U8* data)
{
	U8 lu8tmpStr[EDIT_MAX_LEN] = { 0 };
	U8 lu8InputLen = 0;

	//supplement '0' before data, if lu8InputLen < 2 * GATEWAY_OADD_LEN
	lu8InputLen = STRLEN(data);
	//we use 2 chars to represent a byte, so the mod is 2.
	if (lu8InputLen % 2) {//if lu8InputLen is Odd, lu8InputLen must <= (2 * GATEWAY_OADD_LEN - 1)
		if (lu8InputLen > 2 * GATEWAY_OADD_LEN - 1)
			return;
	}
	else {//if lu8InputLen is Even, lu8InputLen must <= 2 * GATEWAY_OADD_LEN
		if (lu8InputLen > 2 * GATEWAY_OADD_LEN)
			return;
	}
	memset(lu8tmpStr, '0', 2 * GATEWAY_OADD_LEN - lu8InputLen);
	memcpy(lu8tmpStr + (2 * GATEWAY_OADD_LEN - lu8InputLen), data, lu8InputLen);
	memcpy(data, lu8tmpStr, 2 * GATEWAY_OADD_LEN);
}

/*
**	将数据库读取上来的表地址数据结构转化为协议结构.
**	@pDbInfo:	数据库中的信息指针
**	@pProtoInfo:	协议帧指针
*/
void asciiToProtoBin(db_meterinfo_ptr pDbInfo, meter_row_ptr pProtoInfo)
{
	U16 i = 0;

	i = Lib_atoi((const char*)pDbInfo->rowId);
	pProtoInfo->rowId[0] = (U8)i;//L
	pProtoInfo->rowId[1] = (U8)(i >> 8);//H
	inverseStrToBCD(pDbInfo->meterAddr, DB_MINFO_LEN_METERADDR, pProtoInfo->meterAddr, PROTO_LEN_MADDR);
	pProtoInfo->vendorId = Lib_atoi((const char*)pDbInfo->vendorId);
	pProtoInfo->protoVer = Lib_atoi((const char*)pDbInfo->protoVer);
	pProtoInfo->meterType = (ASCII_TO_HEX(pDbInfo->meterType[0]) << 4 | ASCII_TO_HEX(pDbInfo->meterType[1]));
	pProtoInfo->channel = Lib_atoi((const char*)pDbInfo->channel);
	pProtoInfo->valveProtoVer = Lib_atoi((const char*)pDbInfo->valveProtoVer);
	inverseStrToBCD(pDbInfo->valveAddr, DB_MINFO_LEN_METERADDR, pProtoInfo->valveAddr, PROTO_LEN_VADDR);
	inverseStrToBCD(pDbInfo->controlPanelAddr, DB_MINFO_LEN_METERADDR, pProtoInfo->controlPanelAddr, PROTO_LEN_VADDR);
	pProtoInfo->buildId = Lib_atoi((const char*)pDbInfo->buildId);
	pProtoInfo->unitId = Lib_atoi((const char*)pDbInfo->unitId);
	i = Lib_atoi((const char*)pDbInfo->roomId);
	pProtoInfo->roomId[0] = (U8)i;//L
	pProtoInfo->roomId[1] = (U8)(i >> 8);//H
	memset(pProtoInfo->reserved, 0, PROTO_LEN_RSV);
}

/*
** time's pattern is "\d[1-2](\:\d[1-2])?"
** that is,
** without ':', it has 1~2 digits;
** with ':', there are 1~2 digits before ':',
** and 1~2 digits following ':'.
*/
U8 timeLegal(U8* timeStr, U16 strLen)
{
	U16 i = 0, digitalLen = 0;
	U8 data, value[3] = { 0 };
	em_time_state state = tm_state_init;

	for (i = 0; i < strLen; i++) {
		data = timeStr[i];
		switch (state) {
		case tm_state_init:
			if (isdigit(data)) {
				value[digitalLen] = data;
				digitalLen++;
				state = tm_state_hour;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		case tm_state_hour:
			if (isdigit(data)) {
				if (digitalLen == 2) {
					state = tm_state_end_illegal;
					goto result;
				}
				value[digitalLen] = data;
				if (Lib_atoi((const char*)value) > 23) {
					state = tm_state_end_illegal;
					goto result;
				}
				digitalLen++;
			} else if (isDelim(data)) {
				digitalLen = 0;
				memset(value, 0, sizeof(value));
				state = tm_state_delim;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		case tm_state_delim:
			if (isdigit(data)) {
				state = tm_state_min;
				value[digitalLen] = data;
				digitalLen++;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		case tm_state_min:
			if (isdigit(data)) {
				if (digitalLen == 2) {
					state = tm_state_end_illegal;
					goto result;
				}
				value[digitalLen] = data;
				digitalLen++;
				if (Lib_atoi((const char*)value) > 59) {
					state = tm_state_end_illegal;
					goto result;
				}
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		default:
			break;
		}
	}

	if (state == tm_state_hour || state == tm_state_min) {//扫描完字符串后, 如果状态停在小时或分隔符状态, 则不合法
		state = tm_state_end_legal;
	} else {//如果状态停在小时或分钟的状态, 则认为是合法的
		state = tm_state_end_illegal;
	}
result:
	if (state == tm_state_end_legal)
		return NO_ERR;
	else
		return ERROR;
}

/*
**	时间相加(时:分 + 时:分).
**	@pT1:	第一个时间
**	@pT2:	第二个时间
**	@pRes:	结果
*/
U8 addTime(time_node_ptr pT1, time_node_ptr pT2, time_node_ptr pRes)
{
	U8 carry = 0;

	if (pT1->u8hour >24 || pT2->u8hour>24 || pT1->u8minute>60 || pT2->u8minute>60 )
		return ERROR;


	return NO_ERR;
}

/*
**	用户设定的时间点转化为字符串.
**	@buf:		字符串缓存
**	@startTime:	开始时间
**	@timeCnt:	时间点个数
*/
U8 calcTimeNode(U8* buf, U16 bufSize, U8* startTime, U8 timeCnt)
{
	U32 interval = 0;
	U8	delim = ';';
	time_node_str tmNodes[MAX_TIME_NODE] = { {0} };

	if (timeCnt > MAX_TIME_NODE)
		return ERROR;
	if (timeLegal(startTime, STRLEN(startTime)) == ERROR)
		return ERROR;

	interval = MINUTES_PERDAY / timeCnt;

	return NO_ERR;
}

/*
**	将用';'分隔的时间点字符串转化为集中器用的时间点格式(BCD).
**	@buf:		字符串缓存
**	@pTimeNode:	时间点序列
*/
U8 strToTimeNode(U8* buf, U8* pTimeNode)
{
	return NO_ERR;
}
