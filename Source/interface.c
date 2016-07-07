/***************************************************
**	ģ�鹦��:	�����û����潻��
**	ģ������:	interface.c
**	���ߣ�		�α���
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

//����������������
static const GUI_WIDGET_CREATE_INFO mainFrame[] =
{
    { FRAMEWIN_CreateIndirect,  "����������������", GUI_ID_USER,     	0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "�������ü�����",   GUI_ID_BUTTON0,     40,10,150,60,0},
    { BUTTON_CreateIndirect,    "��Ϣ��ѯ���༭",   GUI_ID_BUTTON1,     40,90,150,60,0},
    { BUTTON_CreateIndirect,    "ϵͳ����",         GUI_ID_BUTTON2,     40,170,150,60,0}
};

//�������ü����ƽ���
static const GUI_WIDGET_CREATE_INFO setParaAndControl[] =
{
    { FRAMEWIN_CreateIndirect,  "�������ü�����",   CONFIG_FRAME_IDX,   0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "Уʱ",             GUI_ID_BUTTON0,     5,5,105,60,0},
    { BUTTON_CreateIndirect,    "����ʱ",         GUI_ID_BUTTON1,     130,5,105,60,0},
    { BUTTON_CreateIndirect,    "�޸ļ�������",     GUI_ID_BUTTON2,     5,75,105,60,0},
    { BUTTON_CreateIndirect,    "GPRS����",         GUI_ID_BUTTON3,     130,75,105,60,0},
    { BUTTON_CreateIndirect,    "��������",         GUI_ID_BUTTON4,     5,145,105,60,0},
    { BUTTON_CreateIndirect,    "������Ϣ",         GUI_ID_BUTTON5,     130,145,105,60,0},
    { BUTTON_CreateIndirect,    "������Ϣ�޸�",     GUI_ID_BUTTON6,     5,215,105,60,0},
    { BUTTON_CreateIndirect,    "����������",       GUI_ID_BUTTON7,     130,215,105,60,0}
};

//��������Уʱ
static const GUI_WIDGET_CREATE_INFO setTime[] =
{
	{ FRAMEWIN_CreateIndirect,  "Уʱ",		CONFIG_CTIME_FRAME_IDX,		0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 },
	{ BUTTON_CreateIndirect,    "��������",	GUI_ID_BUTTON0,             10,26,80,30,0 },
	{ EDIT_CreateIndirect,      "",			GUI_ID_EDIT0,               108,26,110,30,0 },
	{ BUTTON_CreateIndirect,    "�˳�",		GUI_ID_BUTTON1,             10,221,80,30,0 },
	{ BUTTON_CreateIndirect,    "Уʱ",		GUI_ID_BUTTON2,             138,221,80,30,0 }
};

//������Ϣ�·�
static const GUI_WIDGET_CREATE_INFO baseInfoIssue[] =
{
    { FRAMEWIN_CreateIndirect,  "������Ϣ�·�",	CONFIG_BASE_INFO_FRAME_IDX, 0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "��������",		GUI_ID_BUTTON0,             10,26,80,30,0},
    { EDIT_CreateIndirect,      "",				GUI_ID_EDIT0,               108,26,110,30,0},
    { BUTTON_CreateIndirect,    "�˳�",			GUI_ID_BUTTON1,             10,221,80,30,0},
    { BUTTON_CreateIndirect,    "�·�",			GUI_ID_BUTTON2,             138,221,80,30,0}
};


//�Լ�����������������ʼ��
static void mainFrameInit( WM_HWIN hDlg )
{
    WM_HWIN pObj;
    pObj = WM_GetDialogItem( hDlg, GUI_ID_TEXT0);
    TEXT_SetTextAlign(pObj,GUI_TA_CENTER);
    TEXT_SetFont(pObj,GUI_GetFontAddress(24));
  	WM_MakeModal( hDlg );                         //���öԻ���Ϊ��ռģʽ
  	WM_SetFocus( hDlg );                          //��ϵͳ���㶨λ���Ի�����
}

//�Բ�����������ƽ����ʼ��
static void setParaAndControlInit(WM_HWIN hDlg)
{
    
}

//�Լ�����Уʱ�����ʼ��
static void setTimeInit(WM_HWIN hDlg)
{
	WM_HWIN hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	EDIT_SetMaxLen(hObj, 2 * GATEWAY_OADD_LEN);
}
//�Ի�����Ϣ�·������ʼ��
static void baseInfoIssueInit(WM_HWIN hDlg)
{
    
}

//������Ļص�����
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
        case WM_NOTIFICATION_RELEASED: //��������Ϣ
            GUI_EndDialog(hDlg, Id);
            break;
        default:
            break;
        }
    break;
    case WM_KEY: //������Ϣ
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
                GUI_EndDialog( hDlg, keyId+(GUI_ID_BUTTON0-GUI_KEY_NUM0)-1);//�ð���"1"��Ӧbutton0
            break;
        }
        break;
    default:
        WM_DefaultProc( pMsg );
        break;
    }
}

//������������ƽ���Ļص�����
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
        case WM_NOTIFICATION_RELEASED: //��������Ϣ
            GUI_EndDialog(hDlg, Id); 
            break;
        default:
            break;
        }
    break;
    case WM_KEY: //������Ϣ
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
                GUI_EndDialog( hDlg, keyId+(GUI_ID_BUTTON0-GUI_KEY_NUM0)-1);//�ð���"1"��Ӧbutton0
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
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	suppplementTo12(lu8InputBuf);
	inverseStrToBCD(lu8InputBuf, 2 * GATEWAY_OADD_LEN, lu8gatewayId, GATEWAY_OADD_LEN);

	if (logic_setTime(lu8gatewayId) == NO_ERR) {
		GUI_MessageBox("\n����ʱ��ɹ�!\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n����ʱ��ʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
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
		case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id)	{
			case GUI_ID_BUTTON0://radio read gateway's Id
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://Exit
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://�·�������ʱ��
				userSetTime(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			default:
				break;
			}
			break;
		}
		break;
	case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://radio read gateway's Id
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM3://�·�������ʱ��
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
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	suppplementTo12(lu8InputBuf);

	if (logic_issueMeterInfo(lu8InputBuf) == NO_ERR) {
		GUI_MessageBox("\n�·����ַ�ɹ�!\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n�·����ַʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
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
        case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id) {
			case GUI_ID_BUTTON0://radio read gateway's Id
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://Exit
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://�·��Ǳ������Ϣ
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
    case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://radio read gateway's Id
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM3://�·�������ʱ��
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
        case GUI_ID_BUTTON0://Уʱ
			setTimeConfig();
            break;
        case GUI_ID_BUTTON1://����ʱ
            break;
        case GUI_ID_BUTTON2://�޸ļ�������
            break;
        case GUI_ID_BUTTON3://GPRS����
            break;
        case GUI_ID_BUTTON4://��������
            break;
        case GUI_ID_BUTTON5://������Ϣ�·�
            baseinfoIssue();
            break;
        case GUI_ID_BUTTON6://������Ϣ�޸�
            break;
        case GUI_ID_BUTTON7://����������
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
        iRet=GUI_ExecDialogBox(mainFrame, GUI_COUNTOF(mainFrame), &mainCb, WM_HBKWIN, 0, 0);//������ִ�жԻ���
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

