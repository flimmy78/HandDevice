#ifndef LOGIC_H
#define LOGIC_H

#include "basedef.h"

U8 logic_setTime(U8*);
U8 logic_readGatewayId(U8* gatewayId);
void printBuf(U8* buf, U16 bufSize, const char* file, const char* func, u32 line);

#endif
