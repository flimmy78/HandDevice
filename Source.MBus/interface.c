/***************************************************
**	模块功能:	处理用户界面交互
**	模块名字:	interface.c
**	作者：		宋宝善
****************************************************
*/

#include "db.h"
#include "protocol.h"
#include "logic.h"
#include "lib.h"
#include "interface.h"


/************************************************************************/
/* widget数组群                                                         */
/************************************************************************/
static const GUI_WIDGET_CREATE_INFO widgetMainFrame[] = {
	{ FRAMEWIN_CreateIndirect, "仪表与阀控", ID_FRAMEWIN_0, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "热量表", ID_BUTTON_0, 70, 35, 80, 50, 0, 0 },
	{ BUTTON_CreateIndirect, "阀控", ID_BUTTON_1, 70, 105, 80, 50, 0, 0 },
	{ BUTTON_CreateIndirect, "设置", ID_BUTTON_2, 70, 175, 80, 50, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetMeterRelate[] = {
	{ FRAMEWIN_CreateIndirect, "仪表程序", ID_FRAMEWIN_1, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "误差", ID_BUTTON_0, 60, 35, 100, 35, 0, 0 },
	{ BUTTON_CreateIndirect, "表号", ID_BUTTON_1, 60, 110, 100, 35, 0, 0 },
	{ BUTTON_CreateIndirect, "时间", ID_BUTTON_2, 60, 185, 100, 35, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetSetMeterErr[] = {
	{ FRAMEWIN_CreateIndirect, "设置误差", ID_FRAMEWIN_0, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "表号", ID_BUTTON_0, 10, 10, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_0, 100, 10, 120, 20, 0, 0 },
	{ TEXT_CreateIndirect, "大", ID_TEXT_0, 10, 50, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "中二", ID_TEXT_1, 10, 90, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "中一", ID_TEXT_2, 10, 130, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "小", ID_TEXT_3, 10, 170, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_1, 110, 50, 100, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_2, 110, 90, 100, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_3, 110, 130, 100, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_4, 110, 170, 100, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", ID_BUTTON_1, 10, 255, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "修改", ID_BUTTON_2, 130, 255, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetSetValve[] = {
	{ FRAMEWIN_CreateIndirect, "阀门信息", ID_FRAMEWIN_0, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_0, 130, 10, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀门号", ID_TEXT_0, 10, 10, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "房间温度", ID_TEXT_1, 10, 50, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_1, 130, 50, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "开阀时间", ID_TEXT_2, 10, 90, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_2, 130, 90, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "强制关阀", ID_BUTTON_0, 10, 270, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "强制开阀", ID_BUTTON_1, 130, 270, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "读阀", ID_BUTTON_2, 10, 140, 80, 20, 0, 0 },
	{ CHECKBOX_CreateIndirect, "", ID_CHECKBOX_0, 110, 140, 107, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetDeviceConfig[] = {
	{ FRAMEWIN_CreateIndirect, "手持机设置", ID_FRAMEWIN_3, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ TEXT_CreateIndirect, "端口", ID_TEXT_0, 10, 10, 60, 20, 0, 0 },
	{ TEXT_CreateIndirect, "波特率", ID_TEXT_1, 10, 40, 60, 20, 0, 0 },
	{ TEXT_CreateIndirect, "数据位", ID_TEXT_2, 10, 70, 60, 20, 0, 0 },
	{ TEXT_CreateIndirect, "校验位", ID_TEXT_3, 10, 100, 60, 20, 0, 0 },
	{ TEXT_CreateIndirect, "停止位", ID_TEXT_4, 10, 130, 60, 20, 0, 0 },
	{ TEXT_CreateIndirect, "表类型", ID_TEXT_5, 10, 160, 60, 20, 0, 0 },
	{ TEXT_CreateIndirect, "阀类型", ID_TEXT_6, 10, 190, 60, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_0, 136, 10, 100, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_1, 135, 40, 100, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_2, 135, 70, 100, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_3, 135, 100, 100, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_4, 135, 130, 100, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_5, 135, 160, 100, 20, 0, 0 },
	{ DROPDOWN_CreateIndirect, "", ID_DROPDOWN_6, 135, 190, 100, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "退出", ID_BUTTON_0, 10, 265, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "保存", ID_BUTTON_1, 135, 265, 80, 20, 0, 0 }
};

S8* gs8deviceStrArray[] = { "RS485","MBUS" };

U8	gu8deviceIntArray[] = { UART_DEVICE_RS485, UART_DEVICE_MBUS };

S8* gs8baudStrArray[] = { "1200","2400","4800","9600","19200","38400","57600","115200" };

U32 gu32baudIntArray[] = { 1200,2400,4800,9600,19200,38400,57600,115200 };

S8* gs8databitStrArray[] = { "8", "7", "9" };

S8* gs8parityStrArray[] = { "无", "奇", "偶" };

S8* gs8stopStrArray[] = { "1" };

S8* gs8meterTypeStrArray[] = { "德鲁超声波热表", "德鲁机械热表"};

S8* gs8valveTypeStrArray[] = { "亿林非德鲁协议", "亿林德鲁协议", "百暖会" };

uart_mode_str	gUart_mode_strMap[] = {
	{ em_databit_7, em_parity_odd, em_stop_1 },
	{ em_databit_7, em_parity_even,em_stop_1 },
	{ em_databit_8, em_parity_none,em_stop_1 },
	{ em_databit_8, em_parity_odd, em_stop_1 },
	{ em_databit_8, em_parity_even,em_stop_1 },
	{ em_databit_9, em_parity_none,em_stop_1 } };


/************************************************************************/
/* Init函数群                                                           */
/************************************************************************/
//对集中器程序的主界面初始化
static void mainFrameInit(WM_HWIN hDlg)
{
	WM_HWIN pObj;
	pObj = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
	TEXT_SetTextAlign(pObj, GUI_TA_CENTER);
	TEXT_SetFont(pObj, GUI_GetFontAddress(24));
	WM_MakeModal(hDlg);                         //设置对话框为独占模式
	WM_SetFocus(hDlg);                          //将系统焦点定位到对话框内
}

static void setMeterErrInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;

	hItem = WM_GetDialogItem(hDlg, ID_TEXT_0);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(hDlg, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(hDlg, ID_TEXT_2);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(hDlg, ID_TEXT_3);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
	EDIT_SetMaxLen(hItem, EDIT_MAX_LEN);
}

static void setValveInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;

	hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
	EDIT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, ID_TEXT_0);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, ID_TEXT_1);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, ID_TEXT_2);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	hItem = WM_GetDialogItem(hDlg, ID_CHECKBOX_0);
	CHECKBOX_SetText(hItem, "16进制地址");
	CHECKBOX_SetState(hItem, 1);
}

static void deviceConfigInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U16 i = 0;
	sUART lComConfig;//com config
	U8 meterType = 0;
	U8 valveType = 0;
	U16 itemCnt = 0;

	db_getCongfig(config_com_para, (U8*)&lComConfig);
	db_getCongfig(config_meter_type, &meterType);
	db_getCongfig(config_valve_type, &valveType);

	for (i = ID_TEXT_0; i <= ID_TEXT_6; i++) {
		hItem = WM_GetDialogItem(hDlg, i);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_0);
	itemCnt = sizeof(gs8deviceStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8deviceStrArray[i]);
		if (gu8deviceIntArray[i] == lComConfig.device) {
			DROPDOWN_SetSel(hItem, i);
		}
	}

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_1);
	itemCnt = sizeof(gs8baudStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8baudStrArray[i]);
		if (gu32baudIntArray[i] == lComConfig.baud) {
			DROPDOWN_SetSel(hItem, i);
		}
	}

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_2);
	itemCnt = sizeof(gs8databitStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8databitStrArray[i]);
	}
	DROPDOWN_SetSel(hItem, gUart_mode_strMap[lComConfig.mode - 1].databit);

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_3);
	itemCnt = sizeof(gs8parityStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8parityStrArray[i]);
	}
	DROPDOWN_SetSel(hItem, gUart_mode_strMap[lComConfig.mode - 1].parity);

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_4);
	itemCnt = sizeof(gs8stopStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8stopStrArray[i]);
	}
	DROPDOWN_SetSel(hItem, gUart_mode_strMap[lComConfig.mode - 1].stop);

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_5);
	itemCnt = sizeof(gs8meterTypeStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8meterTypeStrArray[i]);
	}
	DROPDOWN_SetSel(hItem, meterType);

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_6);
	itemCnt = sizeof(gs8valveTypeStrArray) / sizeof(S8*);
	for (i = 0; i < itemCnt; i++) {
		DROPDOWN_AddString(hItem, gs8valveTypeStrArray[i]);
	}
	DROPDOWN_SetSel(hItem, valveType);
}

/************************************************************************/
/* CallBack函数群                                                       */
/************************************************************************/
//主界面的回调函数
void mainCb(WM_MESSAGE* pMsg)
{
	int NCode, Id, keyId;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		mainFrameInit(hDlg);
		break;
	case WM_PAINT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode) {
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			GUI_EndDialog(hDlg, Id);
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		keyId = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		switch (keyId) {
		case GUI_KEY_ESCAPE:
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			if (keyId >= GUI_KEY_NUM1 && keyId <= GUI_KEY_NUM3)
				GUI_EndDialog(hDlg, keyId + (ID_BUTTON_0 - GUI_KEY_NUM0) - 1);//让按键"1"对应button0
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

void setMeterCb(WM_MESSAGE* pMsg)
{
	int NCode, Id, keyId;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode) {
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			GUI_EndDialog(hDlg, Id);
			break;
		default:
			break;
		}
		break;
	case WM_KEY: //按键消息
		keyId = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
		switch (keyId) {
		case GUI_KEY_ESCAPE:
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0)));
			break;
		default:
			if (keyId >= GUI_KEY_NUM1 && keyId <= GUI_KEY_NUM3)
				GUI_EndDialog(hDlg, keyId + (ID_BUTTON_0 - GUI_KEY_NUM0) - 1);//让按键"1"对应button0
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

void userRadioMeterId(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 meterAddr[2 * METER_ADDR_LEN + 1] = { 0 };
	flow_err_string_str flowErrStr;

	if (logic_radioMeterAddr(meterAddr, &flowErrStr)==ERROR) {
		GUI_MessageBox("\n广播读取表号失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
		EDIT_SetText(hItem, (const char*)meterAddr);

		hItem = WM_GetDialogItem(hDlg, ID_EDIT_1);
		EDIT_SetText(hItem, (const char*)flowErrStr.bigErr);
		hItem = WM_GetDialogItem(hDlg, ID_EDIT_2);
		EDIT_SetText(hItem, (const char*)flowErrStr.mid2Err);
		hItem = WM_GetDialogItem(hDlg, ID_EDIT_3);
		EDIT_SetText(hItem, (const char*)flowErrStr.mid1Err);
		hItem = WM_GetDialogItem(hDlg, ID_EDIT_4);
		EDIT_SetText(hItem, (const char*)flowErrStr.smallErr);
	}
	WM_SetFocus(hDlg);
}

void userModifyCoe(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 meterAddr[2 * METER_ADDR_LEN + 1] = { 0 };
	flow_err_string_str flowErrStr = { 0 };

	hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
	EDIT_GetText(hItem, (S8*)meterAddr, 20);
	if (isNumber(meterAddr , STRLEN(meterAddr)) == ERROR) {
		GUI_MessageBox("\n请在表地址输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		WM_SetFocus(hItem);
		return;
	}
	hItem = WM_GetDialogItem(hDlg, ID_EDIT_1);
	EDIT_GetText(hItem, (S8*)flowErrStr.bigErr, 20);
	if (isFloat(flowErrStr.bigErr, STRLEN(flowErrStr.bigErr)) == ERROR) {
		GUI_MessageBox("\n请在大流量点输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		WM_SetFocus(hItem);
		return;
	}
	hItem = WM_GetDialogItem(hDlg, ID_EDIT_2);
	EDIT_GetText(hItem, (S8*)flowErrStr.mid2Err, 20);
	if (isFloat(flowErrStr.mid2Err, STRLEN(flowErrStr.mid2Err)) == ERROR) {
		GUI_MessageBox("\n请在中二流量点输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		WM_SetFocus(hItem);
		return;
	}
	hItem = WM_GetDialogItem(hDlg, ID_EDIT_3);
	EDIT_GetText(hItem, (S8*)flowErrStr.mid1Err, 20);
	if (isFloat(flowErrStr.mid1Err, STRLEN(flowErrStr.mid1Err)) == ERROR) {
		GUI_MessageBox("\n请在中一流量点输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		WM_SetFocus(hItem);
		return;
	}
	hItem = WM_GetDialogItem(hDlg, ID_EDIT_4);
	EDIT_GetText(hItem, (S8*)flowErrStr.smallErr, 20);
	if (isFloat(flowErrStr.smallErr, STRLEN(flowErrStr.smallErr)) == ERROR) {
		GUI_MessageBox("\n请在小流量点输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		WM_SetFocus(hItem);
		return;
	}
	GUI_MessageBox("\n请将热表调整到温差状态\n", "温差", GUI_MESSAGEBOX_CF_MODAL);
	WM_SetFocus(hDlg);
	if (logic_modifyCoe(meterAddr, &flowErrStr) == ERROR) {
		GUI_MessageBox("\n修改误差失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n发送修改命令成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
	WM_SetFocus(hDlg);
}

void setMeterErrCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		setMeterErrInit(hDlg);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode) {
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case ID_BUTTON_0://广播读取表号
				userRadioMeterId(hDlg);
				break;
			case ID_BUTTON_1://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case ID_BUTTON_2://修改误差
				userModifyCoe(hDlg);
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
		case GUI_KEY_NUM1://广播读取表号
			userRadioMeterId(hDlg);
			break;
		case GUI_KEY_NUM2://退出
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://修改误差
			userModifyCoe(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, ID_BUTTON_0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, ID_BUTTON_0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void readValve(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8 valeAddr[2 * METER_ADDR_LEN + 1] = { 0 };
	U8 roomTemp[12] = { 0 };//房间温度
	U8 openTime[12] = { 0 };//阀开时间
	U8 isHexAddr = ADDR_BCD;

	hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
	EDIT_GetText(hItem, (S8*)valeAddr, 2 * METER_ADDR_LEN);
	hItem = WM_GetDialogItem(hDlg, ID_CHECKBOX_0);
	isHexAddr = CHECKBOX_GetState(hItem);
	if (isHexAddr== ADDR_BCD? \
		(isNumber(valeAddr, STRLEN(valeAddr)) == ERROR):\
		(isHex(valeAddr, STRLEN(valeAddr)) == ERROR)) {
		GUI_MessageBox("\n请在阀门号输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	if (logic_readValve(valeAddr, roomTemp, openTime) == ERROR) {
		GUI_MessageBox("\n读取阀门数据失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		hItem = WM_GetDialogItem(hDlg, ID_EDIT_1);
		EDIT_SetText(hItem, (const S8*)roomTemp);
		hItem = WM_GetDialogItem(hDlg, ID_EDIT_2);
		EDIT_SetText(hItem, (const S8*)openTime);
	}
	WM_SetFocus(hDlg);
}

void operateValve(WM_HWIN hDlg, U8 openClose)
{
	WM_HWIN hItem;
	U8 valeAddr[2 * METER_ADDR_LEN + 1] = { 0 };
	U8 isHexAddr = ADDR_BCD;
	U8 hint[256] = { 0 };
	U8 err = NO_ERR;

	hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
	EDIT_GetText(hItem, (S8*)valeAddr, 2 * METER_ADDR_LEN);
	hItem = WM_GetDialogItem(hDlg, ID_CHECKBOX_0);
	isHexAddr = CHECKBOX_GetState(hItem);
	if (isHexAddr == ADDR_BCD ? \
		(isNumber(valeAddr, STRLEN(valeAddr)) == ERROR) : \
		(isHex(valeAddr, STRLEN(valeAddr)) == ERROR)) {
		GUI_MessageBox("\n请在阀门号输入数字\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	err = logic_operValve(valeAddr, openClose);
	Lib_sprintf((S8*)hint, "\n%s阀门%s\n", openClose == OPEN_VALVE ? "打开" : "关闭", err == ERROR ? "失败" : "成功");
	GUI_MessageBox((const S8*)hint, err == ERROR ? "失败" : "成功", GUI_MESSAGEBOX_CF_MODAL);
	WM_SetFocus(hDlg);
}

void openValve(WM_HWIN hDlg)
{
	operateValve(hDlg, OPEN_VALVE);
}

void closeValve(WM_HWIN hDlg)
{
	operateValve(hDlg, CLOSE_VALVE);
}

void setValveCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;

	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		setValveInit(hDlg);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode) {
		case WM_NOTIFICATION_RELEASED: //触摸屏消息
			switch (Id) {
			case ID_BUTTON_0://关阀
				closeValve(hDlg);
				break;
			case ID_BUTTON_1://开阀
				openValve(hDlg);
				break;
			case ID_BUTTON_2://读取阀门信息
				readValve(hDlg);
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
		case GUI_KEY_NUM1://关阀
			closeValve(hDlg);
			break;
		case GUI_KEY_NUM2://开阀
			openValve(hDlg);
			break;
		case GUI_KEY_NUM3://读取阀门信息
			readValve(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, ID_BUTTON_0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, ID_BUTTON_0)));
			break;
		default:
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

U8 getUartMode(em_databit_idx data, em_parity_idx parity, em_stop_idx stop)
{
	U8 i = 0;
	U8	modeCnt = sizeof(gUart_mode_strMap) / sizeof(uart_mode_str);
	for (i = 0; i < modeCnt; i++) {
		if (gUart_mode_strMap[i].databit == data  &&\
			gUart_mode_strMap[i].parity == parity &&\
			gUart_mode_strMap[i].stop == stop) {
			return (i + 1);
		}
	}
	return ERROR;
}

void userSaveConfig(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	U8  device;
	U32 baud;
	em_databit_idx	data;
	em_parity_idx	parity;
	em_stop_idx		stop;
	U8  mode;
	U8	meterType;
	U8	valveType;

	//设备
	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_0);
	device = gu8deviceIntArray[DROPDOWN_GetSel(hItem)];
	//波特率
	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_1);
	baud = gu32baudIntArray[DROPDOWN_GetSel(hItem)];
	//数据位
	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_2);
	data = (em_databit_idx)DROPDOWN_GetSel(hItem);
	//校验位
	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_3);
	parity = (em_parity_idx)DROPDOWN_GetSel(hItem);
	//停止位
	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_4);
	stop = (em_stop_idx)DROPDOWN_GetSel(hItem);
	mode = getUartMode(data, parity, stop);
	if (mode == ERROR) {
		GUI_MessageBox("\n此串口设置本系统不支持\n", "错误", GUI_MESSAGEBOX_CF_MODAL);
		WM_SetFocus(hDlg);
		return;
	}

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_5);
	meterType= (em_mtype_idx)DROPDOWN_GetSel(hItem);

	hItem = WM_GetDialogItem(hDlg, ID_DROPDOWN_6);
	valveType = (em_vtype_idx)DROPDOWN_GetSel(hItem);

	if (logic_saveConfig(device, baud, mode, meterType, valveType) == ERROR) {
		GUI_MessageBox("\n保存设置失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n保存设置成功\n", "成功", GUI_MESSAGEBOX_CF_MODAL);
	}
	WM_SetFocus(hDlg);
}

void deviceConfigCb(WM_MESSAGE* pMsg)
{
	int NCode, Id;
	WM_HWIN hDlg;
	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		deviceConfigInit(hDlg);
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
			case ID_BUTTON_0://退出
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case ID_BUTTON_1://保存配置
				userSaveConfig(hDlg);
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
		case GUI_KEY_NUM2://保存配置
			userSaveConfig(hDlg);
			break;
		case GUI_KEY_ENTER:
			break;
		case GUI_KEY_UP:
			WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem(hDlg, ID_BUTTON_0)));
			break;
		case GUI_KEY_DOWN:
			WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem(hDlg, ID_BUTTON_0)));
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

void setMeterErr()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetSetMeterErr, GUI_COUNTOF(widgetSetMeterErr), &setMeterErrCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void setMeterAddr()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetSetMeterAddr, GUI_COUNTOF(widgetSetMeterAddr), &setMeterAddrCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void setMeterTime()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetSetMeterTime, GUI_COUNTOF(widgetSetMeterTime), &setMeterTimeCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void setMeter()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetMeterRelate, GUI_COUNTOF(widgetMeterRelate), &setMeterCb, WM_HBKWIN, 0, 0);
		switch (iRet) {
		case ID_BUTTON_0:
			setMeterErr();
			break;
		case ID_BUTTON_1:
			setMeterAddr();
			break;
		case ID_BUTTON_2:
			setMeterTime();
			break;
		case WM_USER_EXIT:
			return;
		default:
			break;
		}
	}
}

void setValve()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetSetValve, GUI_COUNTOF(widgetSetValve), &setValveCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void configDevice()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(widgetDeviceConfig, GUI_COUNTOF(widgetDeviceConfig), &deviceConfigCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void maingui(void)
{
	int iRet;

	while (1) {
		iRet = GUI_ExecDialogBox(widgetMainFrame, GUI_COUNTOF(widgetMainFrame), &mainCb, WM_HBKWIN, 0, 0);
		switch (iRet) {
		case ID_BUTTON_0:
			setMeter();
			break;
		case ID_BUTTON_1:
			setValve();
			break;
		case ID_BUTTON_2:
			configDevice();
			break;
		case -1:
			return;
		default:
			break;
		}
	}
}
