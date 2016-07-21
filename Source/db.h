#ifndef DB_H
#define DB_H

#include "user.h"
#include "basedef.h"
#include "protocol.h"

#define CONFIG_INITTED		0x01//手持机参数已经被初始化
#define CONFIG_NOT_INITTED	0x00//手持机参数还没被初始化

#define DB_CONFIG_NAME		"手持机配置.DBF"
#define DB_BASEINFO_NAME	"表地址信息.DBF"
#define DB_TMP_BASEINFO		"tmp_baseinfo.DBF"//临时存储从集中器读取的表地址信息

#define CONFIG_FILED_CNT		2
#define CONFIG_FIELD_NAME_ID	"f_id"
#define CONFIG_FIELD_NAME_VALUE	"f_value"

#define CONFIG_DELIMITER	','

#define DB_MINFO_LEN_ROWID			3	//数据库中计量点长度
#define DB_MINFO_LEN_METERADDR		14	//数据库中仪表地址长度
#define DB_MINFO_LEN_VENDORID		3	//数据库中厂商代码长度
#define DB_MINFO_LEN_PROTOVER		3	//数据库中仪表协议版本号长度
#define DB_MINFO_LEN_METERTYPE		2	//数据库中仪表类型长度
#define DB_MINFO_LEN_CHANNEL		2	//数据库中通道号长度
#define DB_MINFO_LEN_VALVEPROTOVER	2	//数据库中阀控协议版本号长度
#define DB_MINFO_LEN_VALVEADDR		14	//数据库中阀控地址长度
#define DB_MINFO_LEN_CTLPANELADDR	14	//数据库中温控面板地址长度
#define DB_MINFO_LEN_BUILDID		3	//数据库中楼栋号长度
#define DB_MINFO_LEN_UNITID			2	//数据库中单元号长度
#define DB_MINFO_LEN_ROOMID			4	//数据库中房间号长度
#define DB_MINFO_LEN_GATEWAYID		12	//集中器编号长度

#define DB_MINFO_FIELD_CNT			13	//表基础信息DBF的字段数量

#define DBF_GETBASEFIELD(pInfo)	\
if(DbfFieldGet(minfo_field_rowId, (char*)pInfo->rowId, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_meterAddr, (char*)pInfo->meterAddr, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_vendorId, (char*)pInfo->vendorId, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_protoVer, (char*)pInfo->protoVer, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_meterType, (char*)pInfo->meterType, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_channel, (char*)pInfo->channel, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_valveProtoVer, (char*)pInfo->valveProtoVer, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_valveAddr, (char*)pInfo->valveAddr, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_controlPanelAddr, (char*)pInfo->controlPanelAddr, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_buildId, (char*)pInfo->buildId, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_unitId, (char*)pInfo->unitId, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_roomId, (char*)pInfo->roomId, pDbf) < 0) return ERROR;\
if(DbfFieldGet(minfo_field_gatewayId, (char*)pInfo->gatewayId, pDbf) < 0) return ERROR;

#define DBF_SETBASEFIELD(pInfo) \
if(DbfFieldSet(minfo_field_rowId, (char*)pInfo->rowId, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_meterAddr, (char*)pInfo->meterAddr, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_vendorId, (char*)pInfo->vendorId, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_protoVer, (char*)pInfo->protoVer, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_meterType, (char*)pInfo->meterType, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_channel, (char*)pInfo->channel, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_valveProtoVer, (char*)pInfo->valveProtoVer, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_valveAddr, (char*)pInfo->valveAddr, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_controlPanelAddr, (char*)pInfo->controlPanelAddr, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_buildId, (char*)pInfo->buildId, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_unitId, (char*)pInfo->unitId, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_roomId, (char*)pInfo->roomId, pDbf) < 0) return ERROR;\
if(DbfFieldSet(minfo_field_gatewayId, (char*)pInfo->gatewayId, pDbf) < 0) return ERROR;


typedef enum
{
	config_field_id=0,
	config_field_vale,
}config_field_idx;

typedef enum
{
	minfo_field_rowId = 0,			//计量点
	minfo_field_meterAddr,			//仪表(热量表,水表等)地址
	minfo_field_vendorId,			//厂商代码
	minfo_field_protoVer,			//仪表协议版本号
	minfo_field_meterType,			//仪表类型
	minfo_field_channel,			//通道号
	minfo_field_valveProtoVer,		//阀控协议版本号
	minfo_field_valveAddr,			//阀控地址
	minfo_field_controlPanelAddr,	//温控面板地址
	minfo_field_buildId,			//楼栋号
	minfo_field_unitId,				//单元号
	minfo_field_roomId,				//房间号
	minfo_field_gatewayId			//集中器编号
}meterinfo_field_idx;

typedef struct{//在数据库中的仪表信息结构. 以数值字符串存储的值, 长度必须加1, 以在字符串结尾多加一个'\0'
	U8 rowId[DB_MINFO_LEN_ROWID+1];					//计量点
	U8 meterAddr[DB_MINFO_LEN_METERADDR+1];			//仪表(热量表,水表等)地址
	U8 vendorId[DB_MINFO_LEN_VENDORID+1];			//厂商代码
	U8 protoVer[DB_MINFO_LEN_PROTOVER+1];			//仪表协议版本号
	U8 meterType[DB_MINFO_LEN_METERTYPE+1];			//仪表类型
	U8 channel[DB_MINFO_LEN_CHANNEL+1];				//通道号
	U8 valveProtoVer[DB_MINFO_LEN_VALVEPROTOVER+1];	//阀控协议版本号
	U8 valveAddr[DB_MINFO_LEN_VALVEADDR+1];			//阀控地址
	U8 controlPanelAddr[DB_MINFO_LEN_CTLPANELADDR+1];	//温控面板地址
	U8 buildId[DB_MINFO_LEN_BUILDID+1];				//楼栋号
	U8 unitId[DB_MINFO_LEN_UNITID+1];				//单元号
	U8 roomId[DB_MINFO_LEN_ROOMID+1];				//房间号
	U8 gatewayId[DB_MINFO_LEN_GATEWAYID+1];			//集中器编号
} db_meterinfo_str;

typedef db_meterinfo_str* db_meterinfo_ptr;

extern U8 openDBF(U8* dbfName);
extern U8 closeDBF(void);
extern U8 db_gotoRecord0(void);
extern U8 db_readAllConfig(void);
extern U8 db_getCongfig(u16 configIdx, U8* config);
extern U8 db_getMatchCnt(U8* gatewayId, S32* cnt);
extern U8 db_getMeterInfo(U8* gatewayId, db_meterinfo_ptr pInfo, S32* rowCnt, S32* lastRecId);
extern U8 db_getOneMeterInfo(U8* gatewayId, U16 meterId, db_meterinfo_ptr pInfo);
extern U8 db_modifyGatewayId(U8* gatewayId);
extern U8 reCreateBaseInfoDBF(void);
extern U8 db_storeTempBaseInfo(meter_row_ptr pProtoInfo, U16 infoCnt, U8* gatewayId);
extern U8 db_getOneTempMeterInfo(U16 rowId, db_meterinfo_ptr pDbInfo);
extern U8 db_getNextTempMeterInfo(db_meterinfo_ptr pDbInfo);
extern U8 db_deleteAllRecord(U8* gatewayId);
extern U8 db_cpRecTo(U8* srcDbf, U8* destDbf, U8* gatewayId);
#endif // DB_H
