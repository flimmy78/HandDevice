#ifndef LOGIC_H
#define LOGIC_H

#include "user.h"

extern U8 logic_radioMeterAddr(U8* meterAddr, flow_err_string_ptr pError);
extern U8 logic_saveConfig(U8 device, U32 baud, U8 mode, U8 meterType, U8 valveType);
extern U8 logic_modifyCoe(U8* meterAddr, flow_err_string_ptr pError);
extern U8 logic_readValve(U8* valeAddr, U8* roomTemp, U8* openTime);
extern U8 logic_operValve(U8* valeAddr, U8 openClose);

#endif
