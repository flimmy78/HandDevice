#ifndef DB_H
#define DB_H

#include "user.h"
#include "basedef.h"
#include "protocol.h"

#define CONFIG_INITTED		0x01//�ֳֻ������Ѿ�����ʼ��
#define CONFIG_NOT_INITTED	0x00//�ֳֻ�������û����ʼ��

#define DB_CONFIG_NAME		"�ֳֻ�����.DBF"
#define DB_BASEINFO_NAME	"���ַ��Ϣ.DBF"
#define DB_TMP_BASEINFO		"tmp_baseinfo.DBF"//��ʱ�洢�Ӽ�������ȡ�ı��ַ��Ϣ

#define CONFIG_FILED_CNT		2
#define CONFIG_FIELD_NAME_ID	"f_id"
#define CONFIG_FIELD_NAME_VALUE	"f_value"

#define CONFIG_DELIMITER	','

#define DB_MINFO_LEN_ROWID			3	//���ݿ��м����㳤��
#define DB_MINFO_LEN_METERADDR		14	//���ݿ����Ǳ��ַ����
#define DB_MINFO_LEN_VENDORID		3	//���ݿ��г��̴��볤��
#define DB_MINFO_LEN_PROTOVER		3	//���ݿ����Ǳ�Э��汾�ų���
#define DB_MINFO_LEN_METERTYPE		2	//���ݿ����Ǳ����ͳ���
#define DB_MINFO_LEN_CHANNEL		2	//���ݿ���ͨ���ų���
#define DB_MINFO_LEN_VALVEPROTOVER	2	//���ݿ��з���Э��汾�ų���
#define DB_MINFO_LEN_VALVEADDR		14	//���ݿ��з��ص�ַ����
#define DB_MINFO_LEN_CTLPANELADDR	14	//���ݿ����¿�����ַ����
#define DB_MINFO_LEN_BUILDID		3	//���ݿ���¥���ų���
#define DB_MINFO_LEN_UNITID			2	//���ݿ��е�Ԫ�ų���
#define DB_MINFO_LEN_ROOMID			4	//���ݿ��з���ų���
#define DB_MINFO_LEN_GATEWAYID		12	//��������ų���

#define DB_MINFO_FIELD_CNT			13	//�������ϢDBF���ֶ�����

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
	minfo_field_rowId = 0,			//������
	minfo_field_meterAddr,			//�Ǳ�(������,ˮ���)��ַ
	minfo_field_vendorId,			//���̴���
	minfo_field_protoVer,			//�Ǳ�Э��汾��
	minfo_field_meterType,			//�Ǳ�����
	minfo_field_channel,			//ͨ����
	minfo_field_valveProtoVer,		//����Э��汾��
	minfo_field_valveAddr,			//���ص�ַ
	minfo_field_controlPanelAddr,	//�¿�����ַ
	minfo_field_buildId,			//¥����
	minfo_field_unitId,				//��Ԫ��
	minfo_field_roomId,				//�����
	minfo_field_gatewayId			//���������
}meterinfo_field_idx;

typedef struct{//�����ݿ��е��Ǳ���Ϣ�ṹ. ����ֵ�ַ����洢��ֵ, ���ȱ����1, �����ַ�����β���һ��'\0'
	U8 rowId[DB_MINFO_LEN_ROWID+1];					//������
	U8 meterAddr[DB_MINFO_LEN_METERADDR+1];			//�Ǳ�(������,ˮ���)��ַ
	U8 vendorId[DB_MINFO_LEN_VENDORID+1];			//���̴���
	U8 protoVer[DB_MINFO_LEN_PROTOVER+1];			//�Ǳ�Э��汾��
	U8 meterType[DB_MINFO_LEN_METERTYPE+1];			//�Ǳ�����
	U8 channel[DB_MINFO_LEN_CHANNEL+1];				//ͨ����
	U8 valveProtoVer[DB_MINFO_LEN_VALVEPROTOVER+1];	//����Э��汾��
	U8 valveAddr[DB_MINFO_LEN_VALVEADDR+1];			//���ص�ַ
	U8 controlPanelAddr[DB_MINFO_LEN_CTLPANELADDR+1];	//�¿�����ַ
	U8 buildId[DB_MINFO_LEN_BUILDID+1];				//¥����
	U8 unitId[DB_MINFO_LEN_UNITID+1];				//��Ԫ��
	U8 roomId[DB_MINFO_LEN_ROOMID+1];				//�����
	U8 gatewayId[DB_MINFO_LEN_GATEWAYID+1];			//���������
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
