#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "basedef.h"
#include "user.h"


#define PROTOCOL_VER 0x03	//协议版本号

#if PROTOCOL_VER==0x03
#define GATEWAY_PREFIX      0xFB    //集中器通讯协议的前导符
#define GATEWAY_SUFIX       0xFD    //集中器通讯协议的结束符
#define GATEWAY_START       0x7B    //集中器通讯协议的起始符
#define GATEWAY_PREFIX_CNT  2       //集中器通讯协议的前导符个数
#define GATEWAY_SUFIX_CNT   2       //集中器通讯协议的结束符个数
#define GATEWAY_START_CNT   1       //集中器通讯协议的起始符个数
#define GATEWAY_VER_LEN     1       //集中器通讯协议的版本号长度
#define GATEWAY_SADD_LEN    6       //集中器通讯协议的源地址长度
#define GATEWAY_OADD_LEN    6       //集中器通讯协议的目标地址长度
#define GATEWAY_MID_LEN     1       //集中器通讯协议的消息序列号长度
#define GATEWAY_MSGL_LEN    2       //集中器通讯协议的消息字节数长度
#define GATEWAY_MT_LEN      1       //集中器通讯协议的消息类型长度
#define GATEWAY_TS_LEN      6       //集中器通讯协议的年月日长度
#define GATEWAY_HCK_LEN     1       //集中器通讯协议的头部校验长度
#define GATEWAY_EC_LEN      1       //集中器通讯协议的消息体校验长度
#define GATEWAY_HEAD_LEN    (GATEWAY_VER_LEN+GATEWAY_SADD_LEN+GATEWAY_OADD_LEN+GATEWAY_MID_LEN\
                            +GATEWAY_MSGL_LEN+GATEWAY_MT_LEN+GATEWAY_TS_LEN)
#define GATEWAY_RETID_OFFSET	(GATEWAY_PREFIX_CNT+GATEWAY_START_CNT+GATEWAY_VER_LEN)//offset that gatewayId in return frame

//异常回应代码--表示意义
#define GAT_EXCEP_FAIL          0x10//失败、异常
#define GAT_EXCEP_MEASURE_POINT 0x11//异常，查不到指定计量点号。
#define GAT_EXCEP_CHANNEL_BUSY  0x12//异常，MBUS 通道正忙，现在不能抄表。
#define GAT_EXCEP_PARAM_EXCEED  0x13//参数超出允许范围
#define GAT_EXCEP_PROTO_VER     0x14//消息体版本号错误

/*集中器协议中MSG_TYPE代表的意义 START*/
#define GAT_MT_SVR_CP_1_METER   0x06//主站下发抄单表指令(0x06)
#define GAT_MT_CLT_CP_1_METER   0x07//集中器向主站发送单抄表结果(0x07)

#define GAT_MT_SVR_1OF_MFRM     0x11//主站请求多包传输特定包数据(0x11)
#define GAT_MT_SVR_MODIFY_SINFO 0x13//主站更改单个计量点基础信息(0x13)
#define GAT_MT_CLT_MODIFY_SINFO 0x14//集中器响应主站更改单个热表地址信息(0x14)

#define GAT_MT_SVR_SEND_MINFO   0x0C//主站下发所有仪表地址(0x0C)
#define GAT_MT_CLT__SEND_MINFO	 0x0D//集中器响应主站下发仪表地址(0x0D)

#define GAT_MT_SVR_TIME_POINT   0x20//主站设置定时抄表时间(0x20)
#define GAT_MT_CLT_TIME_POINT   0x21//设置定时抄表时间反馈(0x21)

#define GAT_MT_SVR_HISDATA      0x22//主站向集中器请求历史时间点的所有表数据(0x22)
#define GAT_MT_CLT_HISDATA      0x23//集中器向主站发送固定时间点数据内容(0x23)

#define GAT_MT_SVR_TIME_SET     0x24//设置集中器时钟时间(0x24)
#define GAT_MT_CLT_TIME_SET     0x25//集中器响应设置时间(0x25)

#define GAT_MT_SVR_TIME_RD      0x26//读取集中器时钟时间(0x26)
#define GAT_MT_CLT_TIME_RD      0x27//集中器响应设置时间(0x27)

#define GAT_MT_CLT_LOGIN        0x2C//GPRS登陆帧(0x2C)
#define GAT_MT_SVR_LOGIN        0x2D//主站回应GPRS登录(0x2D)

#define GAT_MT_CLT_HB           0x2E//GPRS心跳(0x2E)
#define GAT_MT_SVR_HB           0x2F//GPRS心跳回应(0x2F)

#define GAT_MT_SVR_REBOOT       0x34//集中器重启指令(0x34)
#define GAT_MT_CLT_REBOOT       0x35//对重启指令的回应(0x35)集中器收到重启指令后, 会先回应上位机, 5秒后重启

#define GAT_MT_SVR_SHISDATA     0x3C//主站向集中器请求历史时间点某单表的数据(0x3C)
#define GAT_MT_CLT_SHISDATA     0x3D//集中器向主站发送固定时间点某单表的数据(0x3D)

#define GAT_MT_SVR_CPY_IMMDT    0x3E//设置集中器立即抄表(0x3E)
#define GAT_MT_CLT_CPY_IMMDT    0x3F//集中器响应立即抄表设置(0x3F)

#define GAT_MT_SVR_RD_CONFIG    0x90//主站读取集中器参数(0x90)
#define GAT_MT_CLT_RD_CONFIG    0x91//主站读取集中器参数响应(0x91)

#define GAT_MT_SVR_RD_ALLINFO   0x92//主站读取集中器中全部热表地址信息 (0x92 )
#define GAT_MT_CLT_RD_ALLINFO   0x93//集中器向主站发送热表地址信息(0x93)

#define GAT_MT_SVR_CHIP         0x94//主站更改集中器IP及端口号(0x94)
#define GAT_MT_CLT_CHIP         0x95//主站更改集中器IP及端口号响应(0x95)
/*集中器协议中MSG_TYPE代表的意义 END*/

#define	GATEWAY_ASW_CODE_SUC				0x01	//操作成功
#define GATEWAY_ASW_CODE_FAIL				0x10	//操作失败
#define GATEWAY_ASW_CODE_NOREC				0x11	//查不到指定计量点号
#define GATEWAY_ASW_CODE_MBUS_BUSY			0x12	//MBUS正忙
#define GATEWAY_ASW_CODE_EXCEED				0x13//参数出允许范围
#define GATEWAY_ASW_CODE_MSGBODY_VER_ERR	0x14	//消息体版本号错误


#endif

//为避免编译器字节对齐, 全部使用单字节结构
typedef struct {//集中器协议体结构
	U8 SourceAddr[GATEWAY_SADD_LEN];//源地址, 低字节在前
	U8 DestAddr[GATEWAY_OADD_LEN];  //目标地址, 低字节在前
	U8 MsgIndex;                    //消息序列号
	U8 MsgLen[GATEWAY_MSGL_LEN];    //消息体长度, 低字节在前
	U8 MsgType;                     //消息类型
	U8 ssmmhhDDMMYY[GATEWAY_TS_LEN];//秒分时日月年, 低字节在前
	U8 *MsgBody;                    //消息体, 低字节在前
} gateway_protocol_str;//类型名用下划线分隔
typedef gateway_protocol_str* gateway_protocol_ptr;

typedef struct {//集中器仪表基础信息结构
	U8 MeterID[2];          //计量点编号
	U8 MeterAddr[7];        //热计量表地址
	U8 Manufacturer;        //厂商代码
	U8 ProtocolVer;         //热表协议版本
	U8 EquipmentType;       //设备类型
	U8 ChannelIndex;        //通道号
	U8 ValveProtocol;       //阀控协议版本
	U8 ValveAddr[7];        //阀门地址
	U8 ControlPanelAddr[7]; //控制面板地址
	U8 BuildID;             //楼号
	U8 UnitID;              //单元号
	U8 RoomID[2];           //房间号
	U8 Reserved[8];         //预留8字节
}meter_info_str;//类型名用下划线分隔


extern U8 protoW_setTime(U8 *gatewatId, U8 idLen, U8* buf, U16* bufSize);
extern U8 protoA_setTime(U8* buf, U16 bufSize);
extern U8 protoR_radioReadId(U8* buf, U16* bufSize);
extern U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize);

#endif

