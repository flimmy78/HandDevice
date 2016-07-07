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

//集中器程序主界面
static const GUI_WIDGET_CREATE_INFO mainFrame[] =
{
    { FRAMEWIN_CreateIndirect,  "集中器程序主界面", GUI_ID_USER,     	0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "参数设置及控制",   GUI_ID_BUTTON0,     40,10,150,60,0},
    { BUTTON_CreateIndirect,    "信息查询及编辑",   GUI_ID_BUTTON1,     40,90,150,60,0},
    { BUTTON_CreateIndirect,    "系统设置",         GUI_ID_BUTTON2,     40,170,150,60,0}
};

//参数设置及控制界面
static const GUI_WIDGET_CREATE_INFO setParaAndControl[] =
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
static const GUI_WIDGET_CREATE_INFO setTime[] =
{
	{ FRAMEWIN_CreateIndirect,  "校时",		CONFIG_CTIME_FRAME_IDX,		0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 },
	{ BUTTON_CreateIndirect,    "集中器号",	GUI_ID_BUTTON0,             10,26,80,30,0 },
	{ EDIT_CreateIndirect,      "",			GUI_ID_EDIT0,               108,26,110,30,0 },
	{ BUTTON_CreateIndirect,    "退出",		GUI_ID_BUTTON1,             10,221,80,30,0 },
	{ BUTTON_CreateIndirect,    "校时",		GUI_ID_BUTTON2,             138,221,80,30,0 }
};

//基础信息下发
static const GUI_WIDGET_CREATE_INFO baseInfoIssue[] =
{
    { FRAMEWIN_CreateIndirect,  "基础信息下发",	CONFIG_BASE_INFO_FRAME_IDX, 0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "集中器号",		GUI_ID_BUTTON0,             10,26,80,30,0},
    { EDIT_CreateIndirect,      "",				GUI_ID_EDIT0,               108,26,110,30,0},
    { BUTTON_CreateIndirect,    "退出",			GUI_ID_BUTTON1,             10,221,80,30,0},
    { BUTTON_CreateIndirect,    "下发",			GUI_ID_BUTTON2,             138,221,80,30,0}
};


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
	if (isNumber(lu8InputBuf, strlen((const char*)lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	suppplementTo12(lu8InputBuf);
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
	if (isNumber(lu8InputBuf, strlen((const char*)lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n请输入数字!\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	suppplementTo12(lu8InputBuf);

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

void setTimeConfig()
{
	int iRet;
	while (1) {
		iRet = GUI_ExecDialogBox(setTime, GUI_COUNTOF(setTime), &setTimeCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
	}
}

void baseinfoIssue()
{
    int iRet;
    while(1){
        iRet = GUI_ExecDialogBox(baseInfoIssue, GUI_COUNTOF(baseInfoIssue), &baseInfoIssueCb, WM_HBKWIN, 0, 0);
		if (iRet == WM_USER_EXIT)
			return;
    }
}

int dispConfig(){
    int iRet;
    while(1){
        iRet = GUI_ExecDialogBox(setParaAndControl, GUI_COUNTOF(setParaAndControl), &setParaAndControlCb, WM_HBKWIN, 0, 0);
        switch(iRet){
        case GUI_ID_BUTTON0://校时
			setTimeConfig();
            break;
        case GUI_ID_BUTTON1://抄表定时
            break;
        case GUI_ID_BUTTON2://修改集中器号
            break;
        case GUI_ID_BUTTON3://GPRS参数
            break;
        case GUI_ID_BUTTON4://补抄设置
            break;
        case GUI_ID_BUTTON5://基础信息下发
            baseinfoIssue();
            break;
        case GUI_ID_BUTTON6://基础信息修改
            break;
        case GUI_ID_BUTTON7://重启集中器
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
        iRet=GUI_ExecDialogBox(mainFrame, GUI_COUNTOF(mainFrame), &mainCb, WM_HBKWIN, 0, 0);//创建并执行对话框
        switch(iRet){
        case GUI_ID_BUTTON0:
            dispConfig();
			break;
        case GUI_ID_BUTTON1:
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

