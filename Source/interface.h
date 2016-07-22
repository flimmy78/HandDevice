#ifndef INTERFACE_H
#define INTERFACE_H

#include "basedef.h"

#define CL998_LCD_XLEN  240//创伦998手持机屏幕宽度, 参考"手持机光盘(新)\1、文档\C_产品宣传资料\三款产品参数.pdf"
#define CL998_LCD_YLEN  320//创伦998手持机屏幕高度, 参考"手持机光盘(新)\1、文档\C_产品宣传资料\三款产品参数.pdf"

#define CONFIG_FRAME_IDX                    (GUI_ID_USER+0x10)//参数设置与控制界面基础编号
#define CONFIG_CTIME_FRAME_IDX              (GUI_ID_USER+0x11)//校时界面的编号
#define CONFIG_TNODE_FRAME_IDX              (GUI_ID_USER+0x12)//抄表定时界面的编号
#define CONFIG_MODIFY_GATEWAYNO_FRAME_IDX   (GUI_ID_USER+0x13)//修改集中器号界面的编号
#define CONFIG_GPRS_PARASET_FRAME_IDX       (GUI_ID_USER+0x14)//GPRS参数界面的编号
#define CONFIG_REREAD_PARASET_FRAME_IDX     (GUI_ID_USER+0x15)//补抄设置界面的编号
#define CONFIG_BASE_INFO_FRAME_IDX          (GUI_ID_USER+0x16)//基础信息下发界面的编号
#define CONFIG_BASE_INFO_MODIFY_FRAME_IDX   (GUI_ID_USER+0x17)//基础信息修改界面的编号
#define CONFIG_REBOOT_GATEWAY_FRAME_IDX     (GUI_ID_USER+0x18)//重启集中器界面的编号

#define INFOQ_EDIT_FRAME_IDX            	(GUI_ID_USER+0x20)//信息查询及编辑界面的编号
#define INFOQ_READ_IMDT_FRAME_IDX       	(GUI_ID_USER+0x21)//立即抄表界面的编号
#define INFOQ_HISDATA_FRAME_IDX         	(GUI_ID_USER+0x22)//历史数据界面的编号
#define INFOQ_READ_BASEINFO_FRAME_IDX   	(GUI_ID_USER+0x23)//读取基础信息界面的编号
#define INFOQ_SIGLE_READ_FRAME_IDX      	(GUI_ID_USER+0x24)//单抄界面的编号
#define INFOQ_VER_FRAME_IDX             	(GUI_ID_USER+0x25)//程序版本查询界面的编号
#define INFOQ_HISDATA_SHEET_IDX				(GUI_ID_USER+0x26)//查询失败/成功详单界面的编号

#define COM_CONFIG_FRAME_IDX  				(GUI_ID_USER+0x30)//手持机基本设置界面的编号

#define EDIT_MODIFY_1INFO_ROWID				GUI_ID_USER+0x101//计量点
#define EDIT_MODIFY_1INFO_METERADDR			GUI_ID_USER+0x102//仪表(热量表,水表等)地址
#define EDIT_MODIFY_1INFO_VENDORID			GUI_ID_USER+0x103//厂商代码
#define EDIT_MODIFY_1INFO_PROTOVER			GUI_ID_USER+0x104//仪表协议版本号
#define EDIT_MODIFY_1INFO_METERTYPE			GUI_ID_USER+0x105//仪表类型
#define EDIT_MODIFY_1INFO_CHANNEL			GUI_ID_USER+0x106//通道号
#define EDIT_MODIFY_1INFO_VALVEPROTOVER		GUI_ID_USER+0x107//阀控协议版本号
#define EDIT_MODIFY_1INFO_VALVEADDR			GUI_ID_USER+0x108//阀控地址
#define EDIT_MODIFY_1INFO_CONTROLPANELADDR	GUI_ID_USER+0x109//温控面板地址
#define EDIT_MODIFY_1INFO_BUILDID			GUI_ID_USER+0x10A//楼栋号
#define EDIT_MODIFY_1INFO_UNITID			GUI_ID_USER+0x10B//单元号
#define EDIT_MODIFY_1INFO_ROOMID			GUI_ID_USER+0x10C//房间号

#define TEXT_MODIFY_1INFO_ROWID				GUI_ID_USER+0x111//计量点
#define TEXT_MODIFY_1INFO_METERADDR			GUI_ID_USER+0x112//仪表(热量表,水表等)地址
#define TEXT_MODIFY_1INFO_VENDORID			GUI_ID_USER+0x113//厂商代码
#define TEXT_MODIFY_1INFO_PROTOVER			GUI_ID_USER+0x114//仪表协议版本号
#define TEXT_MODIFY_1INFO_METERTYPE			GUI_ID_USER+0x115//仪表类型
#define TEXT_MODIFY_1INFO_CHANNEL			GUI_ID_USER+0x116//通道号
#define TEXT_MODIFY_1INFO_VALVEPROTOVER		GUI_ID_USER+0x117//阀控协议版本号
#define TEXT_MODIFY_1INFO_VALVEADDR			GUI_ID_USER+0x118//阀控地址
#define TEXT_MODIFY_1INFO_CONTROLPANELADDR	GUI_ID_USER+0x119//温控面板地址
#define TEXT_MODIFY_1INFO_BUILDID			GUI_ID_USER+0x11A//楼栋号
#define TEXT_MODIFY_1INFO_UNITID			GUI_ID_USER+0x11B//单元号
#define TEXT_MODIFY_1INFO_ROOMID			GUI_ID_USER+0x11C//房间号


#define LISTVIEW_COL_ITEM	0//修改单行基础信息时, 表头列索引
#define LISTVIEW_COL_VALUE	1//修改单行基础信息时, 值列索引

extern int maingui(void);

#endif//INTERFACE_H
