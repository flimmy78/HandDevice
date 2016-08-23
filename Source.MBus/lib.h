#ifndef LIB_H
#define LIB_H

#include "user.h"
#include "base.h"
#include "protocol.h"
#include "db.h"


#define HEX_TO_BCD(x) (((x)/0x0A)*0x10+((x)%0x0A))
#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))

#define ASCII_TO_HEX(c) ((c >='0' && c <='9')?(c-'0'):((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):0)))
#define ASCSTR_TO_HEX(str)	stringToL((const char*)(str), NULL, 16)//以16进制表示的字符串转化为16进制数

#define isspace(c)	(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\12' || c == '\0')
#define isdigit(c)	((unsigned) ((c)-'0') < 10)
#define ishex(c)	((unsigned) ((c)-'0') < 10 || (unsigned) ((c)-'a') < 6 || (unsigned) ((c)-'A') < 6 )
#define isalpha(c)	((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define isupper(c)	(c >= 'A' && c <= 'Z')
#define STRLEN(c)	strlen((const char*)(c))
#define IS_SPACE	0x01
#define NOT_SPACE	0x00

typedef enum {
	float_state_init = 0,
	float_state_sign,
	float_state_digit,
	float_state_dec_point,
	float_state_err
} em_float_state;

extern void printBuf(U8* buf, U16 bufSize, const char* file, const char* func, U32 line);
extern U8 readSysTime(sys_time_ptr pTime);
extern U8 countCheck(U8 *data, U16 len);
extern U8 chkElsonic(U8 *buf, U16 bufSize);
extern U8 isNumber(U8* s, U16 len);
extern U8 isHex(U8* s, U16 len);
extern U8 isFloat(U8* buf, U16 bufSize);
extern U8 trimSpace(U8* s, U16 len);
extern U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen);
extern void trimZero(U8* buf, U8 bufSize);
extern void inverseArray(U8* buf, U16 bufSize);
extern U8 dbFrameToProto(db_mFrame_ptr pDbFrame, meter_frame_info_ptr pProtoFrame);
extern U8 binAddrToStr(U8* binAddr, U8* strAddr);
extern U8 coeToErr(flow_coe_ptr pCoe, flow_error_ptr pFloatErr);
extern U8 ErrTocoe(flow_coe_ptr pOldCoe, flow_error_ptr pFloatErr, flow_coe_ptr pNewCoe);
extern U8 binErrToStr(flow_error_ptr pBinErr, flow_err_string_ptr pStrErr);
extern U8 stringErrToBin(flow_err_string_ptr pStrErr, flow_error_ptr pBinErr);
extern U8 bytesToStr(U8* buf, U16 bufSize, U8* strBuf, const char* file, const char* func, U32 line);
extern void supplementAddr(U8* data);
extern U8 writeByteToFile(U8* buf, U16 bufSize, S8* fileName);
extern U8 openTimeToStr(vElsonic_vopen_frame_ptr pOpenTime, U8* buf);
#endif
