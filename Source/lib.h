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

#define	TIME_DELIM	':'
#define isDelim(c)	((c==':') || (c=='.'))

typedef enum {//Ê±¼ä×Ö·û´®Ìø×ª×´Ì¬
	tm_state_init = 0,		//³õÊ¼×´Ì¬
	tm_state_hour,			//Ð¡Ê±×´Ì¬
	tm_state_delim,			//·Ö¸ô·û×´Ì¬
	tm_state_min,			//·ÖÖÓ×´Ì¬
	tm_state_end_legal,		//ºÏ·¨ÖÕ×´Ì¬
	tm_state_end_illegal	//²»ºÏ·¨ÖÕ×´Ì¬
} em_time_state;

extern U8 readSysTime(sys_time_ptr pTime);
extern U8 countCheck(U8 *_data, U16 _len);
extern U8 trimSpace(U8* s, U16 len);
extern U8 isNumber(U8* s, U16 len);
extern U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen);
extern void inverseArray(U8* s, U16 sLen);
extern void trimZero(U8* buf, U8 bufSize);
extern void suppplementTo12(U8* data);
extern void asciiToProtoBin(db_meterinfo_ptr pDbInfo, meter_row_ptr pProtoInfo);

#endif//LIB_H
