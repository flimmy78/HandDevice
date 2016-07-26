#ifndef LIB_H
#define LIB_H

#include "protocol.h"
#include "db.h"

#define HEX_TO_BCD(x) (((x)/0x0A)*0x10+((x)%0x0A))
#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))

#define ASCII_TO_HEX(c) ((c >='0' && c <='9')?(c-'0'):((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):0)))
#define isdigit(c)	((unsigned) ((c)-'0') < 10)

#define IS_SPACE	0x01
#define NOT_SPACE	0x00

#define	TIME_DELIM	':'//小时和分钟之间的分隔符
#define	TIME_NODE_DELIM	';'//两个时间点之间的分隔符
#define isDelim(c)	((c==':') || (c=='.'))

typedef enum {//时间字符串跳转状态
	tm_state_init = 0,		//初始状态
	tm_state_hour,			//小时状态
	tm_state_delim,			//分隔符状态
	tm_state_min,			//分钟状态
	tm_state_end_legal,		//合法终状态
	tm_state_end_illegal	//不合法终状态
} em_time_state;

extern void lib_printBuf(U8* buf, U16 bufSize, const char* file, const char* func, U32 line);
extern U8 lib_printBufToUart(U8* buf, U16 bufSize, const char* file, const char* func, U32 line);
extern U8 readSysTime(sys_time_ptr pTime);
extern U8 countCheck(U8 *_data, U16 _len);
extern U8 trimSpace(U8* s, U16 len);
extern U8 isNumber(U8* s, U16 len);
extern U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen);
extern void inverseArray(U8* s, U16 sLen);
extern void trimZero(U8* buf, U8 bufSize);
extern void supplementTo12(U8* data);
extern void asciiToProtoBin(db_meterinfo_ptr pDbInfo, meter_row_ptr pProtoInfo);
extern void protoBinToAscii(meter_row_ptr pProtoInfo, db_meterinfo_ptr pDbInfo, U8* gatewayId);
extern U8 timeLegal(U8* timeStr, U16 strLen, em_time_state* pState);
extern U8 addTime(time_node_ptr pT1, time_node_ptr pT2, time_node_ptr pRes);
extern U8 timeStrToBin(U8* timeStr, U8 timeSize, time_node_ptr pRes);
extern U8 calcTimeNode(U8* buf, U16 bufSize, U8* startTime, U8 timeCnt, time_node_ptr pTimeNodes);
extern U8 timeStrToBCD(time_node_ptr pTimeStr);
extern U8 strToTimeNode(U8* buf, U16 bufSize, time_node_ptr pTimeNode, U8* timeCnt);
extern U8 IpLegal(U8* buf, U16 bufSize);
extern U8 strIpToHex(U8* svrStr, gprs_param_ptr pGPRSParam);
extern U8 binHisToAsciiHis(db_hisdata_ptr pDbHisData, tempControl_messure_hisdata_ptr pBinHisData);


#endif//LIB_H
