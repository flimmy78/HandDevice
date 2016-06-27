#ifndef LIB_H
#define LIB_H

#define HEX_TO_BCD(x) (((x)/0x0A)*0x10+((x)%0x0A))
#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))


U8 readSysTime(sys_time_ptr pTime);
U8 countCheck(U8 *_data, U16 _len);
#endif

