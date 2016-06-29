#ifndef LIB_H
#define LIB_H

#define HEX_TO_BCD(x) (((x)/0x0A)*0x10+((x)%0x0A))
#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))

#define ASCII_TO_HEX(c) ((c >='0' && c <='9')?(c-'0'):((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):0)))
#define isdigit(c)	((unsigned) ((c)-'0') < 10)

#define IS_SPACE	0x01
#define NOT_SPACE	0x00

U8 readSysTime(sys_time_ptr pTime);
U8 countCheck(U8 *_data, U16 _len);
U8 trimSpace(U8* s, U16 len);
U8 isNumber(U8* s, U16 len);
U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen);
void inserseArray(U8* s, U16 sLen);
#endif
