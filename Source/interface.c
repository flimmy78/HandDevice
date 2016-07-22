/***************************************************
**	模块功能:	处理用户界面交互
**	模块名字:	interface.c
**	作者：		宋宝善
****************************************************
*/
#include "user.h"
#include "GUI.h"
#include "EDIT.h"
#include "logic.h"
#include "lib.h"
#include "db.h"
#include "protocol.h"
#include "interface.h"

/************************************************************************/
/* widget数组群                                                         */
/************************************************************************/
//集中器程序主界面
static const GUI_WIDGET_CREATE_INFO widgetMainFrame[] =
{
    { FRAMEWIN_CreateIndirect,  "集中器程序主界面", GUI_ID_USER,     	0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "参数设置及控制",   GUI_ID_BUTTON0,     40,10,150,60,0},
    { BUTTON_CreateIndirect,    "信息查询及编辑",   GUI_ID_BUTTON1,     40,90,150,60,0},
    { BUTTON_CreateIndirect,    "系统设置",         GUI_ID_BUTTON2,     40,170,150,60,0}
};

//参数设置及控制界面
static const GUI_WIDGET_CREATE_INFO widgetSetParaAndControl[] =
{
    { FRAMEWIN_CreateIndirect,  "参数设置及控制",   CONFIG_FRAME_IDX,   0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "校时",             GUI_ID_BUTTON0,     5,5,105,60,0},
    { BUTTON_CreateIndirect,    "抄表定时",         GUI_ID_BUTTON1,     130,5,105,60,0},
    { BUTTON_CreateIndirect,    "修改集中器号",     GUI_ID_BUTTON2,     5,75,105,60,0},
    { BUTTON_CreateIndirect,    "GPRS参数",         GUI_ID_BUTTON3,     130,75,105,60,0},
    { BUTTON_CreateIndirect,    "补抄设置",         GUI_ID_BUTTON4,     5,145,105,60,0},
    { BUTTON_CreateIndirect,    "基础信息",         GUI_ID_BUTTON5,     130,145,105,60,0},
    { BUTTON_CreateIndirect,    "基础信息修改",     GUI_ID_BUTTON6,     5,215,105,60,0},
    { BUTTON_CreateIndirect,    "重启集中器",       GUI_ID_BUTTON7,     130,215,105,60,0}
};

//给集中器校时
static const GUI_WIDGET_CREATE_INFO widgetSetTime[] =
{
	{ FRAMEWIN_CreateIndirect,  "校时",		CONFIG_CTIME_FRAME_IDX,		0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 },
	{ BUTTON_CreateIndirect,    "集中器号",	GUI_ID_BUTTON0,             10,26,80,30,0 },
	{ EDIT_CreateIndirect,      "",			GUI_ID_EDIT0,               108,26,110,30,0 },
	{ BUTTON_CreateIndirect,    "退出",		GUI_ID_BUTTON1,             10,221,80,30,0 },
	{ BUTTON_CreateIndirect,    "校时",		GUI_ID_BUTTON2,             138,221,80,30,0 }
};

//基础信息下发
static const GUI_WIDGET_CREATE_INFO widgetBaseInfoIssue[] =
{
    { FRAMEWIN_CreateIndirect,  "基础信息下发",	CONFIG_BASE_INFO_FRAME_IDX, 0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "集中器号",		GUI_ID_BUTTON0,             10,26,80,30,0},
    { EDIT_CreateIndirect,      "",				GUI_ID_EDIT0,               108,26,110,30,0},
    { BUTTON_CreateIndirect,    "退出",			GUI_ID_BUTTON1,             10,221,80,30,0},
    { BUTTON_CreateIndirect,    "下发",			GUI_ID_BUTTON2,             138,221,80,30,0}
};

//基础信息修改
static const GUI_WIDGET_CREATE_INFO widgetModifyMeterInfo[] = {
	{ FRAMEWIN_CreateIndirect, "基础信息修改", CONFIG_BASE_INFO_MODIFY_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 95, 4, 66, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 95, 32, 68, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON2, 4, 265, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "下发", GUI_ID_BUTTON3, 141, 265, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器", GUI_ID_BUTTON0, 4, 3, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "计量点", GUI_ID_TEXT0, 4, 34, 71, 17, 0, 0 },
	{ BUTTON_CreateIndirect, "查询", GUI_ID_BUTTON1, 172, 5, 48, 45, 0, 0 },

	{ TEXT_CreateIndirect, "计量点", TEXT_MODIFY_1INFO_ROWID, 4, 67, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_ROWID, 65, 66, 53, 20, 0, 0 },
	{ TEXT_CreateIndirect, "表号", TEXT_MODIFY_1INFO_METERADDR, 4, 93, 46, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_METERADDR, 65, 90, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "厂商", TEXT_MODIFY_1INFO_VENDORID, 4, 115, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VENDORID, 65, 114, 33, 20, 0, 0 },
	{ TEXT_CreateIndirect, "版本", TEXT_MODIFY_1INFO_PROTOVER, 128, 65, 42, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_PROTOVER, 181, 64, 30, 20, 0, 0 },
	{ TEXT_CreateIndirect, "表类型", TEXT_MODIFY_1INFO_METERTYPE, 109, 114, 50, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_METERTYPE, 180, 113, 29, 20, 0, 0 },
	{ TEXT_CreateIndirect, "通道", TEXT_MODIFY_1INFO_CHANNEL, 4, 141, 49, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_CHANNEL, 65, 140, 34, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀控版本", TEXT_MODIFY_1INFO_VALVEPROTOVER, 113, 140, 65, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VALVEPROTOVER, 180, 140, 30, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀控", TEXT_MODIFY_1INFO_VALVEADDR, 4, 166, 54, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VALVEADDR, 65, 164, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "面板", TEXT_MODIFY_1INFO_CONTROLPANELADDR, 4, 188, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_CONTROLPANELADDR, 65, 190, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "楼栋", TEXT_MODIFY_1INFO_BUILDID, 4, 211, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_BUILDID, 65, 212, 42, 20, 0, 0 },
	{ TEXT_CreateIndirect, "单元", TEXT_MODIFY_1INFO_UNITID, 119, 214, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_UNITID, 162, 214, 48, 20, 0, 0 },
	{ TEXT_CreateIndirect, "房间", TEXT_MODIFY_1INFO_ROOMID, 4, 238, 42, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_ROOMID, 65, 235, 53, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetTimeNodes[] = {
	{ FRAMEWIN_CreateIndirect, "抄表时间点", CONFIG_TNODE_FRAME_IDX, 0, 1, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ TEXT_CreateIndirect, "开始时间", GUI_ID_TEXT0, 15, 18, 47, 20, 0, 0 },
	{ TEXT_CreateIndirect, "个数", GUI_ID_TEXT1, 13, 42, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 77, 15, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 77, 40, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "生成", GUI_ID_BUTTON0, 172, 15, 49, 42, 0, 0 },
	{ MULTIEDIT_CreateIndirect, "", GUI_ID_MULTIEDIT0, 14, 73, 200, 121, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器", GUI_ID_BUTTON1, 15, 220, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 126, 220, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON2, 10, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "下发", GUI_ID_BUTTON3, 126, 260, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetModifyGatewayId[] = {
	{ FRAMEWIN_CreateIndirect, "修改集中器编号", CONFIG_MODIFY_GATEWAYNO_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "原编号", GUI_ID_BUTTON0, 10, 30, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 125, 31, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 125, 80, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON1, 9, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "修改", GUI_ID_BUTTON2, 146, 260, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "现编号", GUI_ID_TEXT0, 10, 82, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetModifyGPRS[] = {
	{ FRAMEWIN_CreateIndirect, "修改GPRS参数", CONFIG_GPRS_PARASET_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器号", GUI_ID_BUTTON0, 15, 12, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "IP", GUI_ID_TEXT0, 30, 45, 50, 20, 0, 0 },
	{ TEXT_CreateIndirect, "端口号", GUI_ID_TEXT1, 15, 75, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "APN", GUI_ID_TEXT2, 15, 105, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "读取", GUI_ID_BUTTON1, 15, 159, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON2, 15, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "修改", GUI_ID_BUTTON3, 129, 260, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 12, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 80, 45, 130, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 130, 75, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT3, 130, 105, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetReboot[] = {
	{ FRAMEWIN_CreateIndirect, "重启集中器", CONFIG_REBOOT_GATEWAY_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器号", GUI_ID_BUTTON0, 15, 20, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 20, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON1, 13, 245, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "重启", GUI_ID_BUTTON2, 130, 245, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetRereadParam[] = {
	{ FRAMEWIN_CreateIndirect, "补抄设置", CONFIG_REREAD_PARASET_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器号", GUI_ID_BUTTON0, 15, 20, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 20, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON1, 13, 245, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "下发", GUI_ID_BUTTON2, 130, 245, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "热表补抄次数", GUI_ID_TEXT0, 15, 60, 110, 20, 0, 0 },
	{ TEXT_CreateIndirect, "热表补抄间隔", GUI_ID_TEXT1, 15, 100, 110, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀控补抄次数", GUI_ID_TEXT2, 15, 140, 110, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀控补抄间隔", GUI_ID_TEXT3, 15, 180, 110, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 130, 60, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 130, 100, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT3, 130, 140, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT4, 130, 180, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetQueryEditInfo[] = {
	{ FRAMEWIN_CreateIndirect, "信息查询与编辑", INFOQ_EDIT_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "立即抄表", GUI_ID_BUTTON0, 15, 30, 80, 60, 0, 0 },
	{ BUTTON_CreateIndirect, "历史数据", GUI_ID_BUTTON1, 130, 30, 80, 60, 0, 0 },
	{ BUTTON_CreateIndirect, "表地址", GUI_ID_BUTTON2, 15, 120, 80, 60, 0, 0 },
	{ BUTTON_CreateIndirect, "单抄历\n史数据", GUI_ID_BUTTON3, 130, 120, 80, 60, 0, 0 },
	{ BUTTON_CreateIndirect, "版本查询", GUI_ID_BUTTON4, 15, 210, 80, 60, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetReadMeterImmd[] = {
	{ FRAMEWIN_CreateIndirect, "立即抄表", INFOQ_READ_IMDT_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器号", GUI_ID_BUTTON0, 15, 30, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 30, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON1, 15, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "立即抄表", GUI_ID_BUTTON2, 132, 260, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetQueryVersion[] = {
	{ FRAMEWIN_CreateIndirect, "版本查询", INFOQ_VER_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器号", GUI_ID_BUTTON0, 15, 30, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON1, 15, 240, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "查询", GUI_ID_BUTTON2, 134, 240, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 30, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "软件版本", GUI_ID_TEXT0, 15, 85, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "硬件版本", GUI_ID_TEXT1, 15, 135, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 130, 85, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 130, 135, 80, 20, 0, 0 }
};

//读取基础信息
static const GUI_WIDGET_CREATE_INFO widgetReadBaseInfo[] = {
	{ FRAMEWIN_CreateIndirect, "读取基础信息", CONFIG_BASE_INFO_MODIFY_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 95, 4, 66, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器", GUI_ID_BUTTON0, 4, 3, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "更新到本地", GUI_ID_BUTTON4, 4, 34, 130, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "读取", GUI_ID_BUTTON1, 172, 5, 48, 45, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON2, 4, 265, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "下一个", GUI_ID_BUTTON3, 141, 265, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "计量点", TEXT_MODIFY_1INFO_ROWID, 4, 67, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_ROWID, 65, 66, 53, 20, 0, 0 },
	{ TEXT_CreateIndirect, "表号", TEXT_MODIFY_1INFO_METERADDR, 4, 93, 46, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_METERADDR, 65, 90, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "厂商", TEXT_MODIFY_1INFO_VENDORID, 4, 115, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VENDORID, 65, 114, 33, 20, 0, 0 },
	{ TEXT_CreateIndirect, "版本", TEXT_MODIFY_1INFO_PROTOVER, 128, 65, 42, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_PROTOVER, 181, 64, 30, 20, 0, 0 },
	{ TEXT_CreateIndirect, "表类型", TEXT_MODIFY_1INFO_METERTYPE, 109, 114, 50, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_METERTYPE, 180, 113, 29, 20, 0, 0 },
	{ TEXT_CreateIndirect, "通道", TEXT_MODIFY_1INFO_CHANNEL, 4, 141, 49, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_CHANNEL, 65, 140, 34, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀控版本", TEXT_MODIFY_1INFO_VALVEPROTOVER, 113, 140, 65, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VALVEPROTOVER, 180, 140, 30, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀控", TEXT_MODIFY_1INFO_VALVEADDR, 4, 166, 54, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VALVEADDR, 65, 164, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "面板", TEXT_MODIFY_1INFO_CONTROLPANELADDR, 4, 188, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_CONTROLPANELADDR, 65, 190, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "楼栋", TEXT_MODIFY_1INFO_BUILDID, 4, 211, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_BUILDID, 65, 212, 42, 20, 0, 0 },
	{ TEXT_CreateIndirect, "单元", TEXT_MODIFY_1INFO_UNITID, 119, 214, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_UNITID, 162, 214, 48, 20, 0, 0 },
	{ TEXT_CreateIndirect, "房间", TEXT_MODIFY_1INFO_ROOMID, 4, 238, 42, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_ROOMID, 65, 235, 53, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetHisData[] = {
	{ FRAMEWIN_CreateIndirect, "历史数据查询", INFOQ_HISDATA_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "集中器号", GUI_ID_BUTTON0, 10, 10, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 10, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON1, 10, 275, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "读取", GUI_ID_BUTTON2, 134, 275, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "成功数", GUI_ID_TEXT0, 10, 45, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "失败数", GUI_ID_TEXT1, 10, 80, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "成功率", GUI_ID_TEXT2, 10, 115, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 130, 45, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 130, 80, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT3, 130, 115, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "成功\n详单", GUI_ID_BUTTON3, 10, 190, 60, 60, 0, 0 },
	{ BUTTON_CreateIndirect, "失败\n详单", GUI_ID_BUTTON4, 150, 190, 60, 60, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetHisSheet[] = {
	{ FRAMEWIN_CreateIndirect, "详细清单", INFOQ_HISDATA_SHEET_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 10, 80, 20, 0, 0 },
	{ LISTVIEW_CreateIndirect, "详单", GUI_ID_LISTVIEW0, 5, 35, 220, 227, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", GUI_ID_BUTTON0, 10, 275, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "下一个", GUI_ID_BUTTON1, 134, 275, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "个数", GUI_ID_TEXT0, 10, 10, 80, 20, 0, 0 }
};

static U8 success = NO_ERR;
/************************************************************************/
/* Init函数群                                                           */
/************************************************************************/
//对集中器程序的主界面初始化
static void mainFrameInit( WM_HWIN hDlg )
{
    WM_HWIN pObj;
    pObj = WM_GetDialogItem( hDlg, GUI_ID_TEXT0);
    TEXT_SetTextAlign(pObj,GUI_TA_CENTER);
    TEXT_SetFont(pObj,GUI_GetFontAddress(24));
  	WM_MakeModal( hDlg );                         //设置对话框为独占模式
  	WM_SetFocus( hDlg );                          //将系统焦点定位到对话框内
}

//对参数设置与控制界面初始化
static void setParaAndControlInit(WM_HWIN hDlg)
{
    
}

//对集中器校时界面初始化
static void setTimeInit(WM_HWIN hDlg)
{
	WM_HWIN hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_SetMaxLen(hObj, 2 * GATEWAY_OADD_LEN);
}

//对基础信息下发界面初始化
static void baseInfoIssueInit(WM_HWIN hDlg)
{
    
}

//对修改基础信息界面初始化
static void ModifyOneInfoInit(WM_HWIN hDlg)
{
	WM_HWIN hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERADDR);
	EDIT_SetMaxLen(hObj, 2 * PROTO_LEN_MADDR);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEADDR);
	EDIT_SetMaxLen(hObj, 2 * PROTO_LEN_MADDR);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CONTROLPANELADDR);
	EDIT_SetMaxLen(hObj, 2 * PROTO_LEN_MADDR);
}

static void setTimeNodesInit(WM_HWIN hDlg)
{
	WM_HWIN hObj = WM_GetDialogItem(hDlg, GUI_ID_MULTIEDIT0);
	MULTIEDIT_SetWrapWord(hObj);
}

static void modifyGatewayIdInit(WM_HWIN hDlg)
{
	
}

static void modifyGPRSInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip地址
	EDIT_SetMaxLen(hItem, 20);
}

static void rebootInit(WM_HWIN hDlg){}

static void rereadInit(WM_HWIN hDlg) {}

static void queryEditInfoInit(WM_HWIN hDlg) {}

static void queryVersionInit(WM_HWIN hDlg) {}

static void queryBaseInfoInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
	BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_RED);
}

static void queryHisDataInit(WM_HWIN hDlg){}

static void hisSheetInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
	if (success == NO_ERR) {
		TEXT_SetText(hItem, "成功个数");
	} else {
		TEXT_SetText(hItem, "失败个数");
	}
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0);
	LISTVIEW_AddColumn(hItem, 80, "项目", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hItem, 135, "值", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_AddRow(hItem, NULL);
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_id, "计量点");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_maddr, "表号");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_build, "楼号");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_unit, "单元号");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_room, "房间号");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_intemp, "进水温度");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_outtemp, "回水温度");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_flow, "流量");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_heat, "热量");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_roomtemp, "室内温度");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_vopen, "阀门开度");
	LISTVIEW_SetItemText(hItem, LISTVIEW_COL_ITEM, em_filedidx_fsuc, "成功标志");
	LISTVIEW_SetGridVis(hItem, 1);
}
/************************************************************************/
/* CallBack函数群                                                       */
/************************************************************************/
//主界面的回调函数
void mainCb( WM_MESSAGE* pMsg )
{
    int NCode, Id, keyId;
    WM_HWIN hDlg;

    hDlg = pMsg->hWin;

    switch ( pMsg->MsgId )
    {
    case WM_INIT_DIALOG:
        mainFrameInit(hDlg);
    break;
    case WM_PAINT:
        break;  
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);   
        NCode = pMsg->Data.v;        
        switch (NCode)
        {
        case WM_NOTIFICATION_RELEASED: //触摸屏消息
            GUI_EndDialog(hDlg, Id);
            break;
        default:
            break;
        }
    break;
    case WM_KEY: //按键消息
        keyId = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
        switch ( keyId )
        {
        case GUI_KEY_ESCAPE:
            GUI_EndDialog( hDlg, -1 );
            break;
        case GUI_KEY_UP:
            WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem( hDlg, GUI_ID_BUTTON0)));
            break;
        case GUI_KEY_DOWN:
            WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem( hDlg, GUI_ID_BUTTON0)));
            break;
        default:
            if(keyId>=GUI_KEY_NUM1 && keyId<=GUI_KEY_NUM3)
                GUI_EndDialog( hDlg, keyId+(GUI_ID_BUTTON0-GUI_KEY_NUM0)-1);//让按键"1"对应button0
            break;
        }
        break;
    default:
        WM_DefaultProc( pMsg );
        break;
    }
}

//参数设置与控制界面的回调函数
void setParaAndControlCb( WM_MESSAGE* pMsg )
{
    int NCode, Id, keyId;
    WM_HWIN hDlg;

    hDlg = pMsg->hWin;

    switch ( pMsg->MsgId )
    {
    case WM_INIT_DIALOG:
        setParaAndControlInit(hDlg);
    break;
    case WM_PAINT:
    break;  
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);   
        NCode = pMsg->Data.v;        
        switch (NCode)
        {
        case WM_NOTIFICATION_RELEASED: //触摸屏消息
            GUI_EndDialog(hDlg, Id); 
            break;
        default:
            break;
        }
    break;
    case WM_KEY: //按键消息
        keyId = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
        switch (keyId)
        {
        case GUI_KEY_ESCAPE:
            GUI_EndDialog( hDlg, -1 );
            break;
        case GUI_KEY_UP:
            WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem( hDlg, GUI_ID_BUTTON0)));
            break;
        case GUI_KEY_DOWN:
            WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem( hDlg, GUI_ID_BUTTON0)));
            break;
        default:
            if(keyId>=GUI_KEY_NUM1 && keyId<=GUI_KEY_NUM8)
                GUI_EndDialog( hDlg, keyId+(GUI_ID_BUTTON0-GUI_KEY_NUM0)-1);//让按键"1"对应button0
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

void userSetTime(WM_HWIN hObj)
{
	U8 lu8InputBuf[EDIT_MAX_LEN] = { 0 };
	U8 lu8gatewayId[EDIT_MAX_LEN] = { 0 };

	EDIT_GetText(hObj, (char*)lu8InputBuf, EDIT_MAX_LEN);

	trimSpace(lu8InputBuf, EDIT_MAX_LEN);
	if (isNumber(lu8InputBuf, STRLEN(lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	supplementTo12(lu8InputBuf);
	inverseStrToBCD(lu8InputBuf, 2 * GATEWAY_OADD_LEN, lu8gatewayId, GATEWAY_OADD_LEN);

	if (logic_setTime(lu8gatewayId) == NO_ERR) {
		GUI_MessageBox("\n设置时间成功!\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n设置时间失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void radioReadGatewayId(WM_HWIN hObj)
{
	U8 gatewayId[GATEWAY_OADD_LEN] = { 0 };
	char gatewayStr[2 * GATEWAY_OADD_LEN] = { 0 };
	logic_readGatewayId(gatewayId);
	sprintf(gatewayStr, "%02X%02X%02X%02X%02X%02X", \
		gatewayId[0], gatewayId[1], gatewayId[2], \
		gatewayId[3], gatewayId[4], gatewayId[5]);
	trimZero((U8*)gatewayStr, 2 * GATEWAY_OADD_LEN);
	EDIT_SetText(hObj, gatewayStr);
}

void setTimeCb(struct WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		setTimeInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id)	{
			case GUI_ID_BUTTON0://radio read gateway's Id
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://Exit
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://下发集中器时间
				userSetTime(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			default:
				break;
			}
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://radio read gateway's Id
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM3://下发集中器时间
			userSetTime(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_ENTER:
			userSetTime(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}	
}

void userIssueInfo(WM_HWIN hObj)
{
	U8 lu8InputBuf[EDIT_MAX_LEN] = { 0 };

	EDIT_GetText(hObj, (char*)lu8InputBuf, EDIT_MAX_LEN);

	trimSpace(lu8InputBuf, EDIT_MAX_LEN);
	if (isNumber(lu8InputBuf, STRLEN(lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	supplementTo12(lu8InputBuf);

	if (logic_issueMeterInfo(lu8InputBuf) == NO_ERR) {
		GUI_MessageBox("\n下发表地址成功!\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n下发表地址失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void baseInfoIssueCb(WM_MESSAGE* pMsg)
{
    int NCode, Id;
    WM_HWIN hDlg;

    hDlg = pMsg->hWin;

    switch ( pMsg->MsgId )
    {
    case WM_INIT_DIALOG:
        baseInfoIssueInit(hDlg);
		break;
    case WM_PAINT:
		break;  
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);   
        NCode = pMsg->Data.v;        
        switch (NCode)
        {
        case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://radio read gateway's Id
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://Exit
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://下发仪表基础信息
				userIssueInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			default:
				break;
			}
            break;
        default:
            break;
        }
		break;
    case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://radio read gateway's Id
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM3://下发集中器时间
			userIssueInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_ENTER:
			userIssueInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
    default:
        WM_DefaultProc(pMsg);
    }
}

void setListView(WM_HWIN hDlg, db_meterinfo_ptr pInfo)
{
	WM_HWIN hObj;

	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROWID);
	EDIT_SetText(hObj, (const char*)pInfo->rowId);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERADDR);
	EDIT_SetText(hObj, (const char*)pInfo->meterAddr);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VENDORID);
	EDIT_SetText(hObj, (const char*)pInfo->vendorId);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_PROTOVER);
	EDIT_SetText(hObj, (const char*)pInfo->protoVer);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERTYPE);
	EDIT_SetText(hObj, (const char*)pInfo->meterType);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CHANNEL);
	EDIT_SetText(hObj, (const char*)pInfo->channel);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEPROTOVER);
	EDIT_SetText(hObj, (const char*)pInfo->valveProtoVer);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEADDR);
	EDIT_SetText(hObj, (const char*)pInfo->valveAddr);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CONTROLPANELADDR);
	EDIT_SetText(hObj, (const char*)pInfo->controlPanelAddr);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_BUILDID);
	EDIT_SetText(hObj, (const char*)pInfo->buildId);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_UNITID);
	EDIT_SetText(hObj, (const char*)pInfo->unitId);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROOMID);
	EDIT_SetText(hObj, (const char*)pInfo->roomId);
}

void queryOneInfo(WM_HWIN hDlg)
{
	U8 lu8GatewayId[EDIT_MAX_LEN] = { 0 };
	U8 lu8MeterId[EDIT_MAX_LEN] = { 0 };
	db_meterinfo_str oneMeterInfoStr;
	WM_HWIN hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);

	EDIT_GetText(hObj, (char*)lu8GatewayId, EDIT_MAX_LEN);
	trimSpace(lu8GatewayId, STRLEN(lu8GatewayId));
	if (isNumber(lu8GatewayId, STRLEN(lu8GatewayId)) == ERROR) {
		GUI_MessageBox("\n集中器请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (STRLEN(lu8GatewayId) > DB_MINFO_LEN_GATEWAYID) {
		GUI_MessageBox("\n集中器号过长!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(lu8GatewayId);

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
	EDIT_GetText(hObj, (char*)lu8MeterId, EDIT_MAX_LEN);
	trimSpace(lu8MeterId, STRLEN(lu8MeterId));
	if (isNumber(lu8MeterId, STRLEN(lu8MeterId)) == ERROR) {
		GUI_MessageBox("\n计量点请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	if (logic_queryOneMeterInfo(lu8GatewayId, Lib_atoi((char*)lu8MeterId), &oneMeterInfoStr) == NO_ERR) {
		setListView(hDlg, &oneMeterInfoStr);
	}
	else {
		GUI_MessageBox("\n查询计量点信息失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
}

U8 readListView(WM_HWIN hDlg, db_meterinfo_ptr pInfo)
{
	WM_HWIN hObj;

	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROWID);
	EDIT_GetText(hObj, (char*)pInfo->rowId, DB_MINFO_LEN_ROWID + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERADDR);
	EDIT_GetText(hObj, (char*)pInfo->meterAddr, DB_MINFO_LEN_METERADDR + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VENDORID);
	EDIT_GetText(hObj, (char*)pInfo->vendorId, DB_MINFO_LEN_VENDORID + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_PROTOVER);
	EDIT_GetText(hObj, (char*)pInfo->protoVer, DB_MINFO_LEN_PROTOVER + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERTYPE);
	EDIT_GetText(hObj, (char*)pInfo->meterType, DB_MINFO_LEN_METERTYPE + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CHANNEL);
	EDIT_GetText(hObj, (char*)pInfo->channel, DB_MINFO_LEN_CHANNEL + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEPROTOVER);
	EDIT_GetText(hObj, (char*)pInfo->valveProtoVer, DB_MINFO_LEN_VALVEPROTOVER + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEADDR);
	EDIT_GetText(hObj, (char*)pInfo->valveAddr, DB_MINFO_LEN_VALVEADDR + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CONTROLPANELADDR);
	EDIT_GetText(hObj, (char*)pInfo->controlPanelAddr, DB_MINFO_LEN_CTLPANELADDR + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_BUILDID);
	EDIT_GetText(hObj, (char*)pInfo->buildId, DB_MINFO_LEN_BUILDID + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_UNITID);
	EDIT_GetText(hObj, (char*)pInfo->unitId, DB_MINFO_LEN_UNITID + 1);
	hObj = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROOMID);
	EDIT_GetText(hObj, (char*)pInfo->roomId, DB_MINFO_LEN_ROOMID + 1);


	return NO_ERR;
}

void userIssueOneInfo(WM_HWIN hDlg)
{
	WM_HWIN hObj;
	U8 lu8GatewayId[EDIT_MAX_LEN] = { 0 };
	db_meterinfo_str oneMeterInfoStr;

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hObj, (char*)lu8GatewayId, EDIT_MAX_LEN);
	trimSpace(lu8GatewayId, STRLEN(lu8GatewayId));
	if (isNumber(lu8GatewayId, STRLEN(lu8GatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(lu8GatewayId);

	if (readListView(hDlg, &oneMeterInfoStr) == ERROR) {
		return;
	}
	
	if (logic_issueOneMeterInfo(lu8GatewayId, &oneMeterInfoStr) == NO_ERR) {
		GUI_MessageBox("\n修改单行数据成功!\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n修改单行数据失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void modifyOneInfoCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		ModifyOneInfoInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读取集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://从DBF查询对应计量点的基础信息
				queryOneInfo(hDlg);
				break;
			case GUI_ID_BUTTON2://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://下发单行仪表基础信息
				userIssueOneInfo(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读取集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;		
		case GUI_KEY_NUM2://从DBF查询对应计量点的基础信息
			queryOneInfo(hDlg);
			break;
		case GUI_KEY_NUM3://下发单行仪表基础信息
			userIssueOneInfo(hDlg);
			break;
		case GUI_KEY_NUM4://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_ENTER:
			userIssueOneInfo(hDlg);
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userGenTimeNodes(WM_HWIN hDlg)
{
	WM_HWIN hEdit;
	U8 startTime[10] = { 0 };
	U8 nodes[5] = { 0 };
	U8 bufTimeNodes[1024] = { 0 };

	hEdit = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hEdit, (char*)startTime, 10);
	if (timeLegal(startTime, STRLEN(startTime), NULL) == ERROR) {
		GUI_MessageBox("\n您输入的时间格式错误!\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	hEdit = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
	EDIT_GetText(hEdit, (char*)nodes, 5);
	if (isNumber(nodes, STRLEN(nodes)) == ERROR) {
		GUI_MessageBox("\n时间点个数, 请输入数字!\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (Lib_atoi((const char*)nodes) > 24) {
		GUI_MessageBox("\n时间点个数必须小于24!\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
	}
	if (logic_genTimeNodesStr(bufTimeNodes, 1024, startTime, Lib_atoi((const char*)nodes)) == ERROR) {
		GUI_MessageBox("\n生成时间点失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	hEdit = WM_GetDialogItem(hDlg, GUI_ID_MULTIEDIT0);
	MULTIEDIT_SetText(hEdit, (const char*)bufTimeNodes);
}

void userIssueTimeNodes(WM_HWIN hDlg)
{
	WM_HWIN hEdit;
	U8 timeNodeBuf[128] = { 0 };
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1];

	hEdit = WM_GetDialogItem(hDlg, GUI_ID_MULTIEDIT0);
	MULTIEDIT_GetText(hEdit, (char*)timeNodeBuf, 128);

	if (logic_issueTimeNodes(timeNodeBuf, STRLEN(timeNodeBuf), gatewayId) == ERROR) {
		GUI_MessageBox("\n下发抄表时间点失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n下发抄表时间点成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void setTimeNodesCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		setTimeNodesInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://生成抄表时间点
				userGenTimeNodes(hDlg);
				break;
			case GUI_ID_BUTTON1://广播读取集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
				break;
			case GUI_ID_BUTTON2://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://下发抄表时间点
				userIssueTimeNodes(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://生成抄表时间点
			userGenTimeNodes(hDlg);
			break;
		case GUI_KEY_NUM2://广播读取集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
			break;
		case GUI_KEY_NUM3://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM4://下发抄表时间点
			userIssueTimeNodes(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userModifyGatewayId(WM_HWIN hDlg)
{
	WM_HWIN hObj;
	U8	originalId[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	U8	targetId[2 * GATEWAY_OADD_LEN + 1] = { 0 };

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hObj, (char*)originalId, EDIT_MAX_LEN);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
	EDIT_GetText(hObj, (char*)targetId, EDIT_MAX_LEN);

	if ((isNumber(originalId, STRLEN(originalId))==ERROR) || (isNumber(targetId, STRLEN(targetId)) == ERROR)) {
		GUI_MessageBox("\n请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	supplementTo12(originalId);
	supplementTo12(targetId);
	if (logic_modifyGatewayId(originalId, targetId) == ERROR) {
		GUI_MessageBox("\n修改集中器号失败!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n修改集中器号成功!\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void modifyGatewayIdCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		modifyGatewayIdInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://修改集中器号
				userModifyGatewayId(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://修改集中器号
			userModifyGatewayId(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userQueryGPRS(WM_HWIN hDlg)
{
	WM_HWIN hObj;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	U8 apnId[3] = { 0 };
	U8 serverIp[20] = { 0 };
	U8 port[5] = { 0 };

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);//集中器id
	EDIT_GetText(hObj, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logic_readGPRSParam(gatewayId, apnId, serverIp, port) == ERROR) {
		GUI_MessageBox("\n读取参数\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip地址
		EDIT_SetText(hObj, (const char*)serverIp);
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);//端口号
		EDIT_SetText(hObj, (const char*)port);
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT3);//apn编号
		EDIT_SetText(hObj, (const char*)apnId);
	}
}

void userModifyGPRS(WM_HWIN hDlg)
{
	WM_HWIN hObj;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	U8 apnId[3] = { 0 };
	U8 serverIp[20] = { 0 };
	U8 port[5] = { 0 };

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);//集中器id
	EDIT_GetText(hObj, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip地址
	EDIT_GetText(hObj, (char*)serverIp, 20);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);//端口号
	EDIT_GetText(hObj, (char*)port, 5);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT3);//apn编号
	EDIT_GetText(hObj, (char*)apnId, 3);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR \
		|| isNumber(port, STRLEN(port)) == ERROR \
		|| isNumber(apnId, STRLEN(apnId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (IpLegal(serverIp, STRLEN(serverIp)) == ERROR) {
		GUI_MessageBox("\nIP地址非法\n", "非法", GUI_MESSAGEBOX_CF_MODAL);
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip地址
		EDIT_SetText(hObj, "");
		return;
	}
	supplementTo12(gatewayId);
	if(logic_modifyGPRSParam(gatewayId, apnId, serverIp, port) == ERROR)
		GUI_MessageBox("\n修改GPRS参数失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	else
		GUI_MessageBox("\n修改GPRS参数成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
}

void modifyGPRSCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		modifyGPRSInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://查询现有参数
				userQueryGPRS(hDlg);
				break;
			case GUI_ID_BUTTON2://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://修改参数
				userModifyGPRS(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
			break;
		case GUI_KEY_NUM2://查询现有参数
			userQueryGPRS(hDlg);
			break;
		case GUI_KEY_NUM3://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM4://修改参数
			userModifyGPRS(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userReboot(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logic_reboot(gatewayId) == ERROR) {
		GUI_MessageBox("\n重启集中器失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n5秒后重启集中器\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void rebootCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		rebootInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://重启
				userReboot(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://重启
			userReboot(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userIssueRereadParam(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	U8 mReadCnt[3] = { 0 };//热表补抄次数
	U8 mReadIntv[7] = { 0 };//热表补抄间隔
	U8 vReadCnt[3] = { 0 };//阀控补抄次数
	U8 vReadIntv[7] = { 0 };//阀控补抄间隔

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
	EDIT_GetText(hItem, (char*)mReadCnt, 3);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);
	EDIT_GetText(hItem, (char*)mReadIntv, 7);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT3);
	EDIT_GetText(hItem, (char*)vReadCnt, 3);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT4);
	EDIT_GetText(hItem, (char*)vReadIntv, 7);

	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR ||\
		isNumber(mReadCnt, STRLEN(mReadCnt)) == ERROR || \
		isNumber(mReadIntv, STRLEN(mReadIntv)) == ERROR || \
		isNumber(vReadCnt, STRLEN(vReadCnt)) == ERROR || \
		isNumber(vReadIntv, STRLEN(vReadIntv)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logit_issueRereadParam(gatewayId, mReadCnt, mReadIntv, vReadCnt, vReadIntv) == ERROR) {
		GUI_MessageBox("\n设置失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n设置成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void rereadCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		rereadInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://下发
				userIssueRereadParam(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://下发
			userIssueRereadParam(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

//参数设置与控制界面的回调函数
void queryEditInfoCb(WM_MESSAGE* pMsg)
{
	int NCode, Id, keyId;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		queryEditInfoInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			GUI_EndDialog(hDlg, Id);
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		keyId = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		switch (keyId)
		{
		case GUI_KEY_ESCAPE:
			GUI_EndDialog(hDlg, -1);
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			if (keyId >= GUI_KEY_NUM1 && keyId <= GUI_KEY_NUM8)
				GUI_EndDialog(hDlg, keyId + (GUI_ID_BUTTON0 - GUI_KEY_NUM0) - 1);//让按键"1"对应button0
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userReadMeterImmd(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logic_readMeterImmd(gatewayId) == ERROR) {
		GUI_MessageBox("\n立即抄表失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n立即抄表成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void readMeterImmdCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		rereadInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://立即抄表
				userReadMeterImmd(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://立即抄表
			userReadMeterImmd(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userQueryVersion(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	U8 softVersion[10] = { 0 };//热表补抄次数
	U8 hardVersion[10] = { 0 };//热表补抄间隔

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);

	if (logic_readVersion(gatewayId, hardVersion, softVersion) == ERROR) {
		GUI_MessageBox("\n查询版本失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
		EDIT_SetText(hItem, (char*)softVersion);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);
		EDIT_SetText(hItem, (char*)hardVersion);
	}
}

void queryVersionCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		queryVersionInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://查询版本
				userQueryVersion(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://查询版本
			userQueryVersion(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userReadBaseinfo(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };
	db_meterinfo_str dbBaseInfoStr = { 0 };

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);

	if (logic_readBaseInfo(gatewayId, &dbBaseInfoStr) == ERROR) {
		GUI_MessageBox("\n读取表地址失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROWID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.rowId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERADDR);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.meterAddr);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VENDORID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.vendorId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_PROTOVER);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.protoVer);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERTYPE);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.meterType);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CHANNEL);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.channel);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEPROTOVER);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.valveProtoVer);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEADDR);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.valveAddr);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CONTROLPANELADDR);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.controlPanelAddr);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_BUILDID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.buildId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_UNITID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.unitId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROOMID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.roomId);
	}
}

void userReadNextBaseinfo(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	db_meterinfo_str dbBaseInfoStr = { 0 };
	if (logic_readNextTempInfo(&dbBaseInfoStr) == ERROR) {
		GUI_MessageBox("\n读取下一个失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROWID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.rowId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERADDR);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.meterAddr);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VENDORID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.vendorId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_PROTOVER);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.protoVer);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_METERTYPE);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.meterType);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CHANNEL);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.channel);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEPROTOVER);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.valveProtoVer);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_VALVEADDR);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.valveAddr);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_CONTROLPANELADDR);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.controlPanelAddr);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_BUILDID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.buildId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_UNITID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.unitId);
		hItem = WM_GetDialogItem(hDlg, EDIT_MODIFY_1INFO_ROOMID);
		EDIT_SetText(hItem, (const char*)dbBaseInfoStr.roomId);
	}
}

void userUpdateDbf(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logic_updateBaseInfo(gatewayId) == ERROR) {
		GUI_MessageBox("\n更新表地址失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n更新表地址成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void ReadBaseInfoCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		queryBaseInfoInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://读取基础信息
				userReadBaseinfo(hDlg);
				break;
			case GUI_ID_BUTTON2://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://下一个
				userReadNextBaseinfo(hDlg);
				break;
			case GUI_ID_BUTTON4:
				userUpdateDbf(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://读取基础信息
			userReadBaseinfo(hDlg);
			break;
		case GUI_KEY_NUM3://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM4://下一个
			userReadNextBaseinfo(hDlg);
			break;
		case GUI_KEY_NUM5:
			userUpdateDbf(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userReadAllHisData(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 gatewayId[2 * GATEWAY_OADD_LEN + 1] = { 0 };

	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_GetText(hItem, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n请输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logic_readHisData(gatewayId) == ERROR) {
		GUI_MessageBox("\n读取历史数据失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n读取历史数据成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
}

void hisSheetCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		hisSheetInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON1://下一个
				break;
			case GUI_ID_BUTTON4:
				userUpdateDbf(hDlg);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM2://下一个
			break;
		case GUI_KEY_NUM5:
			userUpdateDbf(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void userReadHisSheet(U8 suc)
{
	int iRet;
	success = suc;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetHisSheet, GUI_COUNTOF(widgetHisSheet), &hisSheetCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void queryHisDataCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		queryHisDataInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case GUI_ID_BUTTON0://广播读集中器号
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://读取历史数据
				userReadAllHisData(hDlg);
				break;
			case GUI_ID_BUTTON3://成功详单
				userReadHisSheet(NO_ERR);
				break;
			case GUI_ID_BUTTON4://失败详单
				userReadHisSheet(ERROR);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://广播读集中器号
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://读取历史数据
			userReadAllHisData(hDlg);
			break;
		case GUI_KEY_NUM4://成功详单
			userReadHisSheet(NO_ERR);
			break;
		case GUI_KEY_NUM5://失败详单
			userReadHisSheet(ERROR);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}
/************************************************************************/
/* 创建界面函数群                                                       */
/************************************************************************/
void setTimeConfig()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetSetTime, GUI_COUNTOF(widgetSetTime), &setTimeCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void baseinfoIssue()
{
    int iRet;
    while(1){
        iRet = GUI_ExecDialogBox(widgetBaseInfoIssue, GUI_COUNTOF(widgetBaseInfoIssue), &baseInfoIssueCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
    }
}

void modifyOneInfo()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetModifyMeterInfo, GUI_COUNTOF(widgetModifyMeterInfo), &modifyOneInfoCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void setTimeNodes()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetTimeNodes, GUI_COUNTOF(widgetTimeNodes), &setTimeNodesCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void modifyGatewayId()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetModifyGatewayId, GUI_COUNTOF(widgetModifyGatewayId), &modifyGatewayIdCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void modifyGPRS()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetModifyGPRS, GUI_COUNTOF(widgetModifyGPRS), &modifyGPRSCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void reboot()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetReboot, GUI_COUNTOF(widgetReboot), &rebootCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void rereadParam()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetRereadParam, GUI_COUNTOF(widgetRereadParam), &rereadCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

int dispConfig(){
    int iRet;
    while(1){
        iRet = GUI_ExecDialogBox(widgetSetParaAndControl, GUI_COUNTOF(widgetSetParaAndControl), &setParaAndControlCb, WM_HBKWIN, 0, 0);
        switch(iRet){
        case GUI_ID_BUTTON0://校时
			setTimeConfig();
            break;
        case GUI_ID_BUTTON1://抄表定时
			setTimeNodes();
            break;
        case GUI_ID_BUTTON2://修改集中器号
			modifyGatewayId();
            break;
        case GUI_ID_BUTTON3://GPRS参数
			modifyGPRS();
            break;
        case GUI_ID_BUTTON4://补抄设置
			rereadParam();
            break;
        case GUI_ID_BUTTON5://基础信息下发
            baseinfoIssue();
            break;
        case GUI_ID_BUTTON6://基础信息修改
			modifyOneInfo();
            break;
        case GUI_ID_BUTTON7://重启集中器
			reboot();
            break;
        case 0:
            return 0;
        case -1:
            return -1;
        default:
			break;
        }
    }
}

void readMeterImmd()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetReadMeterImmd, GUI_COUNTOF(widgetReadMeterImmd), &readMeterImmdCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void queryVersion()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetQueryVersion, GUI_COUNTOF(widgetQueryVersion), &queryVersionCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void queryBaseInfo()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetReadBaseInfo, GUI_COUNTOF(widgetReadBaseInfo), &ReadBaseInfoCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void queryHisData()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetHisData, GUI_COUNTOF(widgetHisData), &queryHisDataCb, WM_HBKWIN, 0, 0);

		switch (iRet) {
		case GUI_ID_BUTTON3://成功详单
			break;
		case GUI_ID_BUTTON4://失败详单
			break;
		case WM_USER_EXIT:
			return;
		default:
			break;
		}
	}
}

int queryEditInfo()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetQueryEditInfo, GUI_COUNTOF(widgetQueryEditInfo), &queryEditInfoCb, WM_HBKWIN, 0, 0);
		switch (iRet) {
		case GUI_ID_BUTTON0://立即抄表
			readMeterImmd();
			break;
		case GUI_ID_BUTTON1://查询历史数据
			queryHisData();
			break;
		case GUI_ID_BUTTON2://读取表信息
			queryBaseInfo();
			break;
		case GUI_ID_BUTTON3://透传一个计量点的数据
			break;
		case GUI_ID_BUTTON4://软硬件版本查询
			queryVersion();
			break;
		case 0:
			return 0;
		case -1:
			return -1;
		default:
			break;
		}
	}
}

int maingui( void ){
    int iRet;

	while (1) {
        iRet=GUI_ExecDialogBox(widgetMainFrame, GUI_COUNTOF(widgetMainFrame), &mainCb, WM_HBKWIN, 0, 0);//创建并执行对话框
        switch(iRet){
        case GUI_ID_BUTTON0:
            dispConfig();
			break;
        case GUI_ID_BUTTON1:
			queryEditInfo();
			break;
        case GUI_ID_BUTTON2:
			break;
        case GUI_ID_BUTTON5:
            return 0;
        case -1:
            return -1;
        default:
			break;
        }
    }
}

