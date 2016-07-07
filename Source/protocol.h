#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "basedef.h"
#include "user.h"


#define PROTOCOL_VER 0x03	//Э��汾��

#if PROTOCOL_VER==0x03
#define GATEWAY_PREFIX      0xFB    //������ͨѶЭ���ǰ����
#define GATEWAY_SUFIX       0xFD    //������ͨѶЭ��Ľ�����
#define GATEWAY_START       0x7B    //������ͨѶЭ�����ʼ��
#define GATEWAY_PREFIX_CNT  2       //������ͨѶЭ���ǰ��������
#define GATEWAY_SUFIX_CNT   2       //������ͨѶЭ��Ľ���������
#define GATEWAY_START_CNT   1       //������ͨѶЭ�����ʼ������
#define GATEWAY_VER_LEN     1       //������ͨѶЭ��İ汾�ų���
#define GATEWAY_SADD_LEN    6       //������ͨѶЭ���Դ��ַ����
#define GATEWAY_OADD_LEN    6       //������ͨѶЭ���Ŀ���ַ����
#define GATEWAY_MID_LEN     1       //������ͨѶЭ�����Ϣ���кų���
#define GATEWAY_MSGL_LEN    2       //������ͨѶЭ�����Ϣ�ֽ�������
#define GATEWAY_MT_LEN      1       //������ͨѶЭ�����Ϣ���ͳ���
#define GATEWAY_TS_LEN      6       //������ͨѶЭ��������ճ���
#define GATEWAY_HCK_LEN     1       //������ͨѶЭ���ͷ��У�鳤��
#define GATEWAY_EC_LEN      1       //������ͨѶЭ�����Ϣ��У�鳤��
#define GATEWAY_HEAD_LEN    (GATEWAY_VER_LEN+GATEWAY_SADD_LEN+GATEWAY_OADD_LEN+GATEWAY_MID_LEN\
                            +GATEWAY_MSGL_LEN+GATEWAY_MT_LEN+GATEWAY_TS_LEN)
#define GATEWAY_RETID_OFFSET	(GATEWAY_PREFIX_CNT+GATEWAY_START_CNT+GATEWAY_VER_LEN)//offset that gatewayId in return frame
#define GATEWAY_MAIN_FRAME_LEN	30	//������Э���� �ų���Ϣ�岿�ֵ�֡����(������Ϣ��У��)
#define GATEWAY_FRAME_MAX_LEN	1024//ÿ֡Э����󳤶�
#define GATEWAY_METERINFO_LEN	40//�Ǳ���Ϣ�ĳ���
#define GATEWAY_MAX_METERINFO_CNT	24//������Э����ÿ������·�����������
#define GATEWAY_ISSUE_BODY_HEAD	3//�·����ַ��Ϣͷ�ĳ���
//�쳣��Ӧ����--��ʾ����
#define GAT_EXCEP_FAIL          0x10//ʧ�ܡ��쳣
#define GAT_EXCEP_MEASURE_POINT 0x11//�쳣���鲻��ָ��������š�
#define GAT_EXCEP_CHANNEL_BUSY  0x12//�쳣��MBUS ͨ����æ�����ڲ��ܳ���
#define GAT_EXCEP_PARAM_EXCEED  0x13//������������Χ
#define GAT_EXCEP_PROTO_VER     0x14//��Ϣ��汾�Ŵ���

/*������Э����MSG_TYPE��������� START*/
#define GAT_MT_SVR_CP_1_METER   0x06//��վ�·�������ָ��(0x06)
#define GAT_MT_CLT_CP_1_METER   0x07//����������վ���͵�������(0x07)

#define GAT_MT_SVR_1OF_MFRM     0x11//��վ�����������ض�������(0x11)
#define GAT_MT_SVR_MODIFY_SINFO 0x13//��վ���ĵ��������������Ϣ(0x13)
#define GAT_MT_CLT_MODIFY_SINFO 0x14//��������Ӧ��վ���ĵ����ȱ��ַ��Ϣ(0x14)

#define GAT_MT_SVR_SEND_MINFO   0x0C//��վ�·������Ǳ��ַ(0x0C)
#define GAT_MT_CLT__SEND_MINFO	0x0D//��������Ӧ��վ�·��Ǳ��ַ(0x0D)

#define GAT_MT_SVR_TIME_POINT   0x20//��վ���ö�ʱ����ʱ��(0x20)
#define GAT_MT_CLT_TIME_POINT   0x21//���ö�ʱ����ʱ�䷴��(0x21)

#define GAT_MT_SVR_HISDATA      0x22//��վ������������ʷʱ�������б�����(0x22)
#define GAT_MT_CLT_HISDATA      0x23//����������վ���͹̶�ʱ�����������(0x23)

#define GAT_MT_SVR_TIME_SET     0x24//���ü�����ʱ��ʱ��(0x24)
#define GAT_MT_CLT_TIME_SET     0x25//��������Ӧ����ʱ��(0x25)

#define GAT_MT_SVR_TIME_RD      0x26//��ȡ������ʱ��ʱ��(0x26)
#define GAT_MT_CLT_TIME_RD      0x27//��������Ӧ����ʱ��(0x27)

#define GAT_MT_CLT_LOGIN        0x2C//GPRS��½֡(0x2C)
#define GAT_MT_SVR_LOGIN        0x2D//��վ��ӦGPRS��¼(0x2D)

#define GAT_MT_CLT_HB           0x2E//GPRS����(0x2E)
#define GAT_MT_SVR_HB           0x2F//GPRS������Ӧ(0x2F)

#define GAT_MT_SVR_REBOOT       0x34//����������ָ��(0x34)
#define GAT_MT_CLT_REBOOT       0x35//������ָ��Ļ�Ӧ(0x35)�������յ�����ָ���, ���Ȼ�Ӧ��λ��, 5�������

#define GAT_MT_SVR_SHISDATA     0x3C//��վ������������ʷʱ���ĳ���������(0x3C)
#define GAT_MT_CLT_SHISDATA     0x3D//����������վ���͹̶�ʱ���ĳ���������(0x3D)

#define GAT_MT_SVR_CPY_IMMDT    0x3E//���ü�������������(0x3E)
#define GAT_MT_CLT_CPY_IMMDT    0x3F//��������Ӧ������������(0x3F)

#define GAT_MT_SVR_RD_CONFIG    0x90//��վ��ȡ����������(0x90)
#define GAT_MT_CLT_RD_CONFIG    0x91//��վ��ȡ������������Ӧ(0x91)

#define GAT_MT_SVR_RD_ALLINFO   0x92//��վ��ȡ��������ȫ���ȱ��ַ��Ϣ (0x92 )
#define GAT_MT_CLT_RD_ALLINFO   0x93//����������վ�����ȱ��ַ��Ϣ(0x93)

#define GAT_MT_SVR_CHIP         0x94//��վ���ļ�����IP���˿ں�(0x94)
#define GAT_MT_CLT_CHIP         0x95//��վ���ļ�����IP���˿ں���Ӧ(0x95)
/*������Э����MSG_TYPE��������� END*/

#define	GATEWAY_ASW_CODE_SUC				0x01	//�����ɹ�
#define GATEWAY_ASW_CODE_FAIL				0x10	//����ʧ��
#define GATEWAY_ASW_CODE_NOREC				0x11	//�鲻��ָ���������
#define GATEWAY_ASW_CODE_MBUS_BUSY			0x12	//MBUS��æ
#define GATEWAY_ASW_CODE_EXCEED				0x13//����������Χ
#define GATEWAY_ASW_CODE_MSGBODY_VER_ERR	0x14	//��Ϣ��汾�Ŵ���


#endif

#define PROTO_LEN_MADDR		7//Э�����Ǳ��ַ����
#define PROTO_LEN_VADDR		7//Э���з��ŵ�ַ����
#define PROTO_LEN_ROWID		2//Э���м������ų���
#define PROTO_LEN_ROOMID	2//Э���з����ų���
#define PROTO_LEN_RSV		8//�����ֶ�

//Ϊ����������ֽڶ���, ȫ��ʹ�õ��ֽڽṹ
typedef struct {//������Э����ṹ
	U8 SourceAddr[GATEWAY_SADD_LEN];//Դ��ַ, ���ֽ���ǰ
	U8 DestAddr[GATEWAY_OADD_LEN];  //Ŀ���ַ, ���ֽ���ǰ
	U8 MsgIndex;                    //��Ϣ���к�
	U8 MsgLen[GATEWAY_MSGL_LEN];    //��Ϣ�峤��, ���ֽ���ǰ
	U8 MsgType;                     //��Ϣ����
	U8 ssmmhhDDMMYY[GATEWAY_TS_LEN];//���ʱ������, ���ֽ���ǰ
	U8 *MsgBody;                    //��Ϣ��, ���ֽ���ǰ
} gateway_protocol_str;//���������»��߷ָ�, ��ͬ
typedef gateway_protocol_str* gateway_protocol_ptr;

typedef struct{//�������Ǳ������Ϣ�ṹ
	U8 rowId[PROTO_LEN_ROWID];		//������, ����Ψһ��ʾһ��(Little Ending, Hex)
	U8 meterAddr[PROTO_LEN_MADDR];  //�Ǳ�(������,ˮ���)��ַ(Little Ending, BCD)
	U8 vendorId;					//���̴���(Hex)
	U8 protoVer;					//�Ǳ�Э��汾��(Hex)
	U8 meterType;					//�Ǳ�����(Hex)
	U8 channel;						//ͨ����(Hex)
	U8 valveProtoVer;				//����Э��汾��(Hex)
	U8 valveAddr[PROTO_LEN_VADDR];  //���ص�ַ(Little Ending, BCD)
	U8 controlPanelAddr[PROTO_LEN_VADDR]; //�¿�����ַ(Little Ending, BCD)
	U8 buildId;						//¥����(Hex)
	U8 unitId;						//��Ԫ��(Hex)
	U8 roomId[PROTO_LEN_ROOMID];    //�����(Little Ending, Hex)
	U8 reserved[PROTO_LEN_RSV];     //�����ֶ�
}meter_row_str;
typedef meter_row_str* meter_row_ptr;

typedef struct
{
	U8 totalRows;			//һ���ж����б��ַ
	U8 seq;					//��֡��������
	U8 thisRows;			//��֡���������б��ַ
}meterinfo_bodyHead_str;
typedef meterinfo_bodyHead_str* meterinfo_bodyHead_ptr;

extern U8 protoW_setTime(U8 *gatewatId, U8 idLen, U8* buf, U16* bufSize);
extern U8 protoA_setTime(U8* buf, U16 bufSize);
extern U8 protoR_radioReadId(U8* buf, U16* bufSize);
extern U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize);
extern U8 protoW_issueMinfo(U8*, U16*, U8*, meterinfo_bodyHead_ptr, meter_row_ptr);
extern U8 protoA_issueMinfo(U8* buf, U16 bufSize);



#endif

