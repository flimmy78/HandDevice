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

void lib_printBuf(U8* buf, U16 bufSize, const char* file, const char* func, U32 line)
{
#ifdef DEBUG
	U16 i = 0;
	Lib_printf("[%s][%s][%d]buf: ", file, func, line);
	for (i = 0; i < bufSize; i++)
		Lib_printf("%02X ", buf[i]);
	Lib_printf("\n");
#endif // DEBUG
}

U8 lib_printBufToUart(U8* buf, U16 bufSize, const char* file, const char* func, U32 line)
{
	sUART *pu;
	sUART comConfig;
	U8 debugStr[1024] = { 0 };
	U8 oneByte[3] = { 0 };
	U16 i = 0;
	db_getCongfig(config_com_para, (U8*)&comConfig);

#ifdef DEBUG
	sprintf((char*)debugStr, "[%s][%s][%d]buf: ", file, func, line);
	for (i = 0; i < bufSize; i++) {
		sprintf((char*)oneByte, "%02X ", buf[i]);
		strcat((char*)debugStr, (char*)oneByte);
	}
	strcat((char*)debugStr,"\n");
	pu = UartOpen(comConfig.baud, comConfig.mode, comConfig.device);//打开串口
	if (!pu)
		return ERROR;
	UartWrite(debugStr, STRLEN(debugStr), 0, pu);
	UartClose(pu);
#endif // DEBUG
	return NO_ERR;
}

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
** 计算时间字符串合法与否
** 字符串模式匹配: "\d[1-2](\:\d[1-2])?"
** 即有1到2个数字表示小时, 如果有时间分隔符(如':'或'.')
** 则分隔符后必须接1到2个数字的分钟.
** @timeStr:	时间字符串
** @strLen:		时间字符串长度
** @pState:		返回扫描完时间字符串后的停留状态
*/
U8 timeLegal(U8* timeStr, U16 strLen, em_time_state* pState)
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

result:
	if(pState)
		*pState = state;
	//扫描完字符串后, 如果状态停在小时或分隔符状态, 则合法
	if (state == tm_state_hour || state == tm_state_min)
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

	pRes->u8hour = pT1->u8hour + pT2->u8hour;
	pRes->u8minute = pT1->u8minute + pT2->u8minute;
	carry = pRes->u8minute / 60;
	pRes->u8hour += carry;
	pRes->u8hour %= 24;
	pRes->u8minute %= 60;
	return NO_ERR;
}

/*
**	把字符形式的时间转化为二进制形式的时间.
**	@timeStr:	时间字符串
**	@timeSize:	字符串长度
**	@pRes:		结果
*/
U8 timeStrToBin(U8* timeStr, U8 timeSize, time_node_ptr pRes)
{
	U8* p = timeStr;
	em_time_state state;
	if (timeLegal(timeStr, timeSize, &state) == ERROR)
		return ERROR;
	if (state == tm_state_hour) {//如果状态停在小时状态, 则字符串必全为数字
		pRes->u8hour = Lib_atoi((const char*)timeStr);
		pRes->u8minute = 0;
	} else if (state==tm_state_min) {
		//先把分隔符修改为字符串的结束符'\0'
		while (!isDelim(*p) && (*p != '\0')) p++;
		*p = 0;
		//求小时
		pRes->u8hour = Lib_atoi((const char*)timeStr);
		p++;//由于p指向的位置为'\0', 所以向后移动一位才能到达分钟
		pRes->u8minute = Lib_atoi((const char*)p);//p指向了分钟位置, 而timeStr的结束符也是p的结束符
	}

	return NO_ERR;
}

/*
**	用户设定的时间点转化为字符串.
**	@buf:		字符串缓存
**	@startTime:	开始时间
**	@timeCnt:	时间点个数
*/
U8 calcTimeNode(U8* buf, U16 bufSize, U8* startTime, U8 timeCnt, time_node_ptr pTimeNodes)
{
	U16 i = 0;
	em_time_state state = tm_state_init;
	time_node_str timeInterval = { 0,0 };
	time_node_str startTimeStr = { 0,0 };
	U8 lu8Time[10] = { 0 };
	U8 hour = 0;

	if (timeCnt > MAX_TIME_NODE)
		return ERROR;
	//24个小时不能被下列数整除
	if (timeCnt == 7 || timeCnt == 11 || timeCnt == 13 || timeCnt == 14 || timeCnt == 17 || timeCnt == 19 || timeCnt == 21 || timeCnt == 23)
		return ERROR;
	if (timeLegal(startTime, STRLEN(startTime), &state) == ERROR)
		return ERROR;
	timeStrToBin(startTime, STRLEN(startTime), &startTimeStr);
	hour = (24 / timeCnt);
	for (i = 0; i < timeCnt ; i++) {
		timeInterval.u8hour = i*hour;
		addTime(&startTimeStr, &timeInterval, &(pTimeNodes[i]));
		sprintf((char*)lu8Time, "%02d%c%02d%c", pTimeNodes[i].u8hour, TIME_DELIM, pTimeNodes[i].u8minute, TIME_NODE_DELIM);
		strcat((char*)buf, (char*)lu8Time);
	}
	return NO_ERR;
}

U8 timeStrToBCD(time_node_ptr pTimeStr)
{
	pTimeStr->u8minute = HEX_TO_BCD(pTimeStr->u8minute);
	pTimeStr->u8hour = HEX_TO_BCD(pTimeStr->u8hour);
	return NO_ERR;
}

/*
**	将用';'分隔的时间点字符串转化为集中器用的时间点格式(BCD).
**	@buf:		字符串缓存
**	@pTimeNode:	时间点序列
*/
U8 strToTimeNode(U8* buf, U16 bufSize, U8* pTimeNode, U16* timeCnt)
{
	U8 lu8time[10] = { 0 };
	U8* p = buf;
	U16 i = 0;

	if (*p == TIME_NODE_DELIM)
		p++;

	for (*timeCnt=0, i = 0; *p !='\0' ; p++) {
		if (*p!=TIME_NODE_DELIM) {
			lu8time[i] = *p;
			i++;
		} else {
			if (timeStrToBin(lu8time, STRLEN(lu8time), &(((time_node_ptr)pTimeNode)[*timeCnt])) == ERROR)
				return ERROR;
			if (timeStrToBCD(&((time_node_ptr)pTimeNode)[*timeCnt]) == ERROR)
				return ERROR;
			*timeCnt += 1;
			i = 0;
			memset(lu8time, 0, 10);
		}
	}
	return NO_ERR;
}

U8 IpLegal(U8* buf, U16 bufSize)
{
	U8* p = buf;
	U16 i = 0, j = 0;
	U8	delimCnt = 0;
	U8	value[10] = { 0 };

	for (i = 0; i < bufSize ; p++, i++) {
		if (*p == IP_DELIM) {
			delimCnt++;
			if (Lib_atoi((const char*)value) > 255) {
				return ERROR;
			}
			j = 0;
			memset(value, 0, 10);
		} else if (isdigit(*p)) {
			value[j] = *p;
			j++;
		} else{
			return ERROR;
		}
	}
	if (delimCnt != 3)
		return ERROR;
	if (Lib_atoi((const char*)value) > 255) {
		return ERROR;
	}
	return NO_ERR;
}

U8 strIpToHex(U8* svrStr, gprs_param_ptr pGPRSParam)
{
	U16 value = 0;
	U8	j = 3, delimCnt = 0;
	U8* p = svrStr;
	U8* q = svrStr;

	while ( *p != '\0') {
		if (*p == IP_DELIM) {
			*p = '\0';
			value = Lib_atoi((const char*)q);
			if (value > 255) {
				return ERROR;
			} else {
				pGPRSParam->IPAddr[j] = (U8)value;
				j--;
			}
			delimCnt++;
			if (delimCnt>3)//多于3个分隔符, 返回错误
				return ERROR;

			q = p + 1;
		}
		p++;
	}
	if (delimCnt == 3) {
		value = Lib_atoi((const char*)q);
		if (value > 255) {
			return ERROR;
		}
		else {
			pGPRSParam->IPAddr[j] = (U8)value;
		}
	}
	return NO_ERR;
}
