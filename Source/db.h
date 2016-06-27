#ifndef DB_H
#define DB_H

#include "user.h"
#include "basedef.h"

#define CONFIG_INITTED		0x01//手持机参数已经被初始化
#define CONFIG_NOT_INITTED	0x00//手持机参数还没被初始化

U8 db_readAllConfig(void);
U8 db_getCongfig(em_config_idx configIdx, U8* config);

#endif // DB_H
