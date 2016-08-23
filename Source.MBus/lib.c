#include <math.h>
#include "limits.h"
#include "lib.h"

void printBuf(U8* buf, U16 bufSize, const char* file, const char* func, U32 line)
{
	U8	strBuf[2048] = { 0 };
#ifdef DEBUG
	U16 i = 0;
	Lib_printf("[%s][%s][%d]buf: ", file, func, line);
	for (i = 0; i < bufSize; i++)
		Lib_printf("%02X ", buf[i]);
	Lib_printf("\n");
#endif // DEBUG
	bytesToStr(buf, bufSize, strBuf, file, func, line);
	writeByteToFile(strBuf, strlen((const char*)strBuf), "log.txt");
}

//读取系统时间
U8 readSysTime(sys_time_ptr pTime)
{
	RtcGetDate(&(pTime->u8year), &(pTime->u8month), &(pTime->u8day));
	RtcGetTime(&(pTime->u8hour), &(pTime->u8minute), &(pTime->u8second));

	pTime->u8year = HEX_TO_BCD(pTime->u8year);
	pTime->u8month = HEX_TO_BCD(pTime->u8month);
	pTime->u8day = HEX_TO_BCD(pTime->u8day);
	pTime->u8hour = HEX_TO_BCD(pTime->u8hour);
	pTime->u8minute = HEX_TO_BCD(pTime->u8minute);
	pTime->u8second = HEX_TO_BCD(pTime->u8second);

	return 0;
}

//校验, 累加和
U8 countCheck(U8 *buf, U16 bufSize)
{
	U8 cs = 0;
	U16 i;
	for (i = 0; i < bufSize; i++, buf++)
		cs += *buf;

	return cs;
}

//亿林家的协议校验方式
U8 chkElsonic(U8 *buf, U16 bufSize)
{
	return (countCheck(buf, bufSize) ^ ELSONIC_CHK_CODE);
}

/*
**	去掉字符串中的空格.
**	@s:		字符串
**	@len:	字符串长度
*/
U8 trimSpace(U8* s, U16 len)
{
	U8* p = s;
	U8	buf[256] = { 0 };
	U16 i = 0, j = 0;
	for (i = 0, j = 0; i < len; p++, i++) {
		if (!isspace(*p)) {
			buf[j] = *p;
			j++;
		}
	}
	memset(s, 0x00, len);
	memcpy(s, buf, j + 1);
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
	if (len == 0) {
		return ERROR;
	}
	for (i = 0; i < len; s++, i++) {
		if (!isdigit(*s)) {
			return ERROR;
		}
	}

	return NO_ERR;
}

/*
**	检查字符串是否都是16进制字符.
**	@s:		字符串
**	@len:	字符串长度
*/
U8 isHex(U8* s, U16 len)
{
	U16 i = 0;
	if (len == 0) {
		return ERROR;
	}
	for (i = 0; i < len; s++, i++) {
		if (!ishex(*s)) {
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

	for (i = 0; i < sLen / 2; i++, s += 2) {//convert ascii to hex each two chars
		t[i] = (ASCII_TO_HEX(s[0]) << 4 | ASCII_TO_HEX(s[1]));
	}

	inverseArray(t, i);
	return NO_ERR;
}

void inverseArray(U8* buf, U16 bufSize)
{
	U16 i = 0;
	for (i = 0; i < bufSize / 2; i++) {//swap two symmetric elements
		buf[i] = buf[i] ^ buf[bufSize - i - 1];
		buf[bufSize - i - 1] = buf[i] ^ buf[bufSize - i - 1];
		buf[i] = buf[i] ^ buf[bufSize - i - 1];
	}
}

/*
**	将不足14位的表号前面补零.
**	@data:	用户输入的集中器号
*/
void supplementAddr(U8* data)
{
	U8 lu8tmpStr[EDIT_MAX_LEN] = { 0 };
	U8 lu8InputLen = 0;

	//supplement '0' before data, if lu8InputLen < 2 * GATEWAY_OADD_LEN
	lu8InputLen = STRLEN(data);
	//we use 2 chars to represent a byte, so the mod is 2.
	if (lu8InputLen % 2) {//if lu8InputLen is Odd, lu8InputLen must <= (2 * GATEWAY_OADD_LEN - 1)
		if (lu8InputLen > 2 * METER_ADDR_LEN - 1)
			return;
	}
	else {//if lu8InputLen is Even, lu8InputLen must <= 2 * GATEWAY_OADD_LEN
		if (lu8InputLen > 2 * METER_ADDR_LEN)
			return;
	}
	memset(lu8tmpStr, '0', 2 * METER_ADDR_LEN - lu8InputLen);
	memcpy(lu8tmpStr + (2 * METER_ADDR_LEN - lu8InputLen), data, lu8InputLen);
	memcpy(data, lu8tmpStr, 2 * METER_ADDR_LEN);
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
**	将数字字符串转为长整数.
**	参考: http://opensource.apple.com//source/xnu/xnu-1456.1.26/bsd/libkern/strtol.c
**	@nptr:		数字字符串
**	@endptr:	字符串长度
**	@base:		基数
**	return:		返回转换的整数
*/
U32 stringToL(const char* nptr, char **endptr, U8 base)
{
	const char *s = nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;//限度的绝对值
	int cutlim;
	int neg = 0, any;

	do {//略过空白
		c = *s++;
	} while (isspace(c));

	if (c == '-') {//符号位
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;

	if ((base == 0 || base == 16) && \
		c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	} else if ((base == 0 || base == 2) && \
		c == '0' && (*s == 'b' || *s == 'B')) {
		c = s[1];
		s += 2;
		base = 2;
	}

	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	**	为了不超过长整数的范围, 
	**	设置cutoff为比不带符号的最大整数少1位.
	**	如果下一个字符的数字值比cutlim还大时,
	**	就不能继续计算了.
	**	比如输入的字符串为"-2147483649"
	**	base为10
	**	当计算到cutoff=214748364时, 
	**	字串的下一个字符为9, 大于cutlim
	**	(此时为8), 就不能再往下计算了
	*/
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;

	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= (isupper(c) ? ('A' - 10) : ('a' - 10));
		else
			break;

		if (c >= base)
			break;

		if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)//超限, 不计算
			any = -1;
		else {//没超限
			any = 1;
			acc *= base;
			acc += c;
		}
	}

	if (any < 0) {//超限, 返回极限值
		acc = (neg ? LONG_MIN : LONG_MAX);
	} else if (neg)//未超限, 但为负数
		acc = -acc;

	if (endptr != NULL)
		*endptr = (char *)(any ? s - 1 : nptr);

	return (acc);
}

U8 dbFrameToProto(db_mFrame_ptr pDbFrame, meter_frame_info_ptr pProtoFrame)
{
	pProtoFrame->protoType = Lib_atoi((const char*)pDbFrame->prototype);
	pProtoFrame->meterType = ASCSTR_TO_HEX(pDbFrame->metertype);
	pProtoFrame->prefix = ASCSTR_TO_HEX(pDbFrame->prefix);
	pProtoFrame->prefixCnt = Lib_atoi((const char*)pDbFrame->prefixcnt);
	pProtoFrame->startChar = ASCSTR_TO_HEX(pDbFrame->startchar);
	pProtoFrame->endChar = ASCSTR_TO_HEX(pDbFrame->endchar);
	pProtoFrame->ctlCode= ASCSTR_TO_HEX(pDbFrame->controcode);
	pProtoFrame->dataIdentify= ASCSTR_TO_HEX(pDbFrame->dataid);
	pProtoFrame->dataLen= Lib_atoi((const char*)pDbFrame->datalen);
	pProtoFrame->busType= Lib_atoi((const char*)pDbFrame->bustype);
	pProtoFrame->addrLen= Lib_atoi((const char*)pDbFrame->addresslen);
	pProtoFrame->radioAddr= ASCSTR_TO_HEX(pDbFrame->radioaddr);

	return NO_ERR;
}

U8 binAddrToStr(U8* binAddr, U8* strAddr)
{
	Lib_sprintf((char*)strAddr, "%02x%02x%02x%02x%02x%02x%02x", \
		binAddr[6], binAddr[5], binAddr[4], binAddr[3], \
		binAddr[2], binAddr[1], binAddr[0]);
	return NO_ERR;
}

/*
**	将两字节4位整数, 12位小数
**	的定点数, 转化为浮点数表示
**	的百分比误差
*/
float getErrByCoe(U16 coe)
{
	/*
	 *	老表程序使用2字节表示误差,
	 *	高字节的高四位表示整数,
	 *	高字节的低四位和低字节的8位
	 *	共12位表示小数,
	 *	所以得到的2字节无符号整数要将小数点
	 *	向左移动12位, 即除以2^12 = 4096
	 *	才可以得到原数
	 *	系数与误差之间的关系:
	 *	假设Vc为仪表计算得到的值, Vd为仪表示值
	 *	Vs为实际值, Coe为流量系数
	 *	此时认为Vc*Coe=Vs=Vd
	 *	则 Err = (V1-V0)/V0
	 *	而理论上, V1*Coe = V0
	 *	通过上式消去V1, V0
	 *	得:
	 *	Coe = 1/(1+Err)
	 *	也即:
	 *	Err = ((1/Coe) -1)
	 *	乘以100, 得到百分比
	 *	注意: 测试得到的误差, 即认为此时的热表示值
	 *	与标准值相等(因为当前认为经误差修正过的
	 *	热表, 得到的示值就是标准值)
	 */
	printBuf((U8*)&coe, sizeof(U16), FILE_LINE);
	if (coe>0) {
		return (4096.0 / coe - 1.0)*100.0;
	} else {//如果原系数是0, 则误差为无穷大, 为显示给用户和计算方便, 取100%
		return 100.0;
	}
}

/*
**	将浮点数表示的百分比误差
**	转化为两字节4位整数, 12位小数
**	的定点数
*/
U16 getCoeByPercentErr(float percentErr, U16 oldCoe)
{
	/*
	 *	假设Vc为仪表计算得到的值,
	 *	CoeO为老的系数, ErrN为新误差
	 *	CoeN为新的系数,
	 *	Vd为仪表示值, 
	 *	V0为实际值, 
	 *	有Vd = Vc*CoeO, 1
	 *	有V0 = Vc*CoeN, 2
	 *	而ErrN = (Vd-V0)/V0, 3, 可得 Vd/V0 = (1+ErrN), 4
	 *	式2 除以 式1 得, CoeN/CoeO = V0/Vd, 5
	 *	故而 CoeN = CoeO*(V0/Vd) = CoeO/(1+ErrN)
	 */
	U16 newCoeFix = 0;
	float newCoeFoat = 0.0;
	float oldCoeFloat = (oldCoe == 0 ? 1.0 : (oldCoe / 4096.0));

	newCoeFoat = (oldCoeFloat / (1.0 + percentErr / 100.0));
	newCoeFix = (int)(newCoeFoat * 4096.0);
	printBuf((U8*)&newCoeFix, sizeof(U16), FILE_LINE);
	return newCoeFix;
}

U8 coeToErr(flow_coe_ptr pCoe, flow_error_ptr pFloatErr)
{
	pFloatErr->bigErr = getErrByCoe(pCoe->bigErr);
	pFloatErr->mid2Err = getErrByCoe(pCoe->mid2Err);
	pFloatErr->mid1Err = getErrByCoe(pCoe->mid1Err);
	pFloatErr->smallErr = getErrByCoe(pCoe->smallErr);
	return NO_ERR;
}

U8 ErrTocoe(flow_coe_ptr pOldCoe, flow_error_ptr pFloatErr, flow_coe_ptr pNewCoe)
{
	pNewCoe->bigErr = getCoeByPercentErr(pFloatErr->bigErr, pOldCoe->bigErr);
	pNewCoe->mid2Err = getCoeByPercentErr(pFloatErr->mid2Err, pOldCoe->mid2Err);
	pNewCoe->mid1Err = getCoeByPercentErr(pFloatErr->mid1Err, pOldCoe->mid1Err);
	pNewCoe->smallErr = getCoeByPercentErr(pFloatErr->smallErr, pOldCoe->smallErr);
	return NO_ERR;
}

U8 binErrToStr(flow_error_ptr pBinErr, flow_err_string_ptr pStrErr)
{
	Lib_sprintf((S8*)pStrErr->bigErr, "%1.3f", pBinErr->bigErr);
	Lib_sprintf((S8*)pStrErr->mid2Err, "%1.3f", pBinErr->mid2Err);
	Lib_sprintf((S8*)pStrErr->mid1Err, "%1.3f", pBinErr->mid1Err);
	Lib_sprintf((S8*)pStrErr->smallErr, "%1.3f", pBinErr->smallErr);
	return NO_ERR;
}

U8 stringErrToBin(flow_err_string_ptr pStrErr, flow_error_ptr pBinErr)
{
	pBinErr->bigErr = Lib_atof((const char*)pStrErr->bigErr);
	pBinErr->mid2Err = Lib_atof((const char*)pStrErr->mid2Err);
	pBinErr->mid1Err = Lib_atof((const char*)pStrErr->mid1Err);
	pBinErr->smallErr = Lib_atof((const char*)pStrErr->smallErr);
	return NO_ERR;
}

U8 bytesToStr(U8* buf, U16 bufSize, U8* strBuf, const char* file, const char* func, U32 line)
{
	U16	i = 0;
	S8	tmpStr[4] = { 0 };
	sys_time_str timeStr;

	if (bufSize == 0) {
		return NO_ERR;
	}
	readSysTime(&timeStr);

	Lib_sprintf((S8*)strBuf, "[20%02X-%02X-%02X %02X:%02X:%02X][%s, %s, %d]", \
		timeStr.u8year, timeStr.u8month, timeStr.u8day, \
		timeStr.u8hour, timeStr.u8minute, timeStr.u8second, file, func, line);
	for (i = 0; i < bufSize-1;i++) {
		Lib_sprintf(tmpStr, "%02X ", buf[i]);
		strcat((S8*)strBuf, (const char*)tmpStr);
	}

	Lib_sprintf(tmpStr, "%02X\n", buf[i]);
	strcat((S8*)strBuf, (const char*)tmpStr);
	return NO_ERR;
}

U8 writeByteToFile(U8* buf, U16 bufSize, S8* fileName)
{
	sFILE *fp = FileOpen(fileName, "war");

	if (fp == NULL) {
		return ERROR;
	}
	FileWrite(buf, bufSize, fp);
	FileClose(fp);
	return NO_ERR;
}

/*
**	判断一个字符串是否是浮点数
**	@buf		字符串缓存
**	@bufSize	字符串缓存长度
*/
U8 isFloat(U8* buf, U16 bufSize)
{
	em_float_state state = float_state_init;
	S8 c;
	S8 flag = 0;
	U16 i = 0;

	for (i = 0; i < bufSize; i++) {
		c = buf[i];
		switch (state) {
		case float_state_init://初始状态只接受符号或数字
			if (c == '+' || c == '-') {
				state = float_state_sign;
			}
			else if (isdigit(c)) {
				state = float_state_digit;
			} else {
				state = float_state_err;
				goto result;
			}
			break;
		case float_state_sign://符号状态只接受数字
			if (isdigit(c)) {
				state = float_state_digit;
			} else {
				state = float_state_err;
				goto result;
			}
			break;
		case float_state_digit://数字状态只接受数字和小数点
			if (c == '.') {
				if (flag == 0) {//如果没有接收过小数点, 则跳转到小数点状态;
					flag = 1;
					state = float_state_dec_point;
				} else {//如果接受过小数点, 则直接跳转到错误状态
					state = float_state_err;
					goto result;
				}
			} else if ((c != '.') && !isdigit(c)) {
				state = float_state_err;
			}
			break;
		case float_state_dec_point://小数点状态只接受数字
			if (isdigit(c)) {
				state = float_state_digit;
			} else {
				state = float_state_err;
				goto result;
			}
			break;
		default:
			state = float_state_err;
			goto result;
		}
	}
result:
	//float_state_digit 是唯一的接受状态
	return ((state == float_state_digit) ? NO_ERR : ERROR);
}

/*
**	将一个BCD码表示的字节序列转化为真正的整数
**	@BCD		BCD字节缓存, 小端
**	@byteCnt	字节缓存长度
*/
U32 BCDtoHex(U8* BCD, U8 byteCnt)
{
	U32 hex = 0;
	U16 i = 0;

	for (i = 0; i < byteCnt; i++) {
		hex += BCD_TO_HEX(BCD[i]) * pow(100, i);
	}
	return hex;
}

U8 openTimeToStr(vElsonic_vopen_frame_ptr pOpenTime, U8* buf)
{
	U32 hex = BCDtoHex(pOpenTime->openHour, ELSONIC_OPEN_HOUR_LEN);

	Lib_sprintf((S8*)buf, "%d:%d", hex, pOpenTime->openMin);
	return NO_ERR;
}














