#ifndef DB_H
#define DB_H

#include "user.h"
#include "base.h"

#define CONFIG_FIELD_CNT	2	//配置数据库的字段数量
#define DB_CONFIG_ID_LEN	1	//配置数据库中ID字段的长度
#define DB_CONFIG_VALUE_LEN	20	//配置数据库中VALUE字段的长度

#define CONFIG_INITTED		0x01//手持机参数已经被初始化
#define CONFIG_NOT_INITTED	0x00//手持机参数还没被初始化

#define DB_CONFIG_NAME			"MBUS配置.DBF"
#define DB_FRAME_CONFIG_NAME	"抄表配置.DBF"

#define CONFIG_DELIMITER	','


typedef struct {
	U8 id[4];
	U8 prototype[3];
	U8 metertype[3];
	U8 prefix[3];
	U8 prefixcnt[4];
	U8 startchar[3];
	U8 endchar[3];
	U8 controcode[3];
	U8 dataid[5];
	U8 datalen[4];
	U8 bustype[3];
	U8 addresslen[2];
	U8 supaddr[7];
	U8 radioaddr[3];
	U8 baud[3];
	U8 databit[3];
	U8 parity[3];
	U8 stop[3];
} db_mFrame_str;
typedef db_mFrame_str* db_mFrame_ptr;

typedef enum
{
	config_field_id = 0,
	config_field_vale,
}config_field_idx;


typedef enum {//帧配置表的字段索引
	em_db_frame_id = 0,
	em_db_frame_protoType,
	em_db_frame_meterType,
	em_db_frame_prefix,
	em_db_frame_prefixCnt,
	em_db_frame_startChar,
	em_db_frame_endChar,
	em_db_frame_ctlCode,
	em_db_frame_dataId,
	em_db_frame_dataLen,
	em_db_frame_busType,
	em_db_frame_addrLen,
	em_db_frame_supAddr,
	em_db_frame_radioAddr,
	em_db_frame_baud,
	em_db_frame_databit,
	em_db_frame_parity,
	em_db_frame_stop
}em_frame_field_idx;




extern U8 openDBF(U8* dbfName);
extern U8 closeDBF(void);
extern U8 db_setComConfig(U8 device, U32 baud, U8  mode);
extern U8 db_meterValveType(U8 meterType, U8 valveType);
extern U8 db_writeConfig(void);
extern U8 db_readAllConfig(void);
extern U8 db_getCongfig(u16 configIdx, U8* config);
extern U8 db_getFrameInfo(U32 infoIdx, db_mFrame_ptr pDbFrame);


#endif // DB_H
