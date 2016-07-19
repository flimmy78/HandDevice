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

/************************************************************************/
/* widget����Ⱥ                                                         */
/************************************************************************/
//����������������
static const GUI_WIDGET_CREATE_INFO widgetMainFrame[] =
{
    { FRAMEWIN_CreateIndirect,  "����������������", GUI_ID_USER,     	0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "�������ü�����",   GUI_ID_BUTTON0,     40,10,150,60,0},
    { BUTTON_CreateIndirect,    "��Ϣ��ѯ���༭",   GUI_ID_BUTTON1,     40,90,150,60,0},
    { BUTTON_CreateIndirect,    "ϵͳ����",         GUI_ID_BUTTON2,     40,170,150,60,0}
};

//�������ü����ƽ���
static const GUI_WIDGET_CREATE_INFO widgetSetParaAndControl[] =
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
static const GUI_WIDGET_CREATE_INFO widgetSetTime[] =
{
	{ FRAMEWIN_CreateIndirect,  "Уʱ",		CONFIG_CTIME_FRAME_IDX,		0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 },
	{ BUTTON_CreateIndirect,    "��������",	GUI_ID_BUTTON0,             10,26,80,30,0 },
	{ EDIT_CreateIndirect,      "",			GUI_ID_EDIT0,               108,26,110,30,0 },
	{ BUTTON_CreateIndirect,    "�˳�",		GUI_ID_BUTTON1,             10,221,80,30,0 },
	{ BUTTON_CreateIndirect,    "Уʱ",		GUI_ID_BUTTON2,             138,221,80,30,0 }
};

//������Ϣ�·�
static const GUI_WIDGET_CREATE_INFO widgetBaseInfoIssue[] =
{
    { FRAMEWIN_CreateIndirect,  "������Ϣ�·�",	CONFIG_BASE_INFO_FRAME_IDX, 0,0,CL998_LCD_XLEN,CL998_LCD_YLEN,0,0 }, 
    { BUTTON_CreateIndirect,    "��������",		GUI_ID_BUTTON0,             10,26,80,30,0},
    { EDIT_CreateIndirect,      "",				GUI_ID_EDIT0,               108,26,110,30,0},
    { BUTTON_CreateIndirect,    "�˳�",			GUI_ID_BUTTON1,             10,221,80,30,0},
    { BUTTON_CreateIndirect,    "�·�",			GUI_ID_BUTTON2,             138,221,80,30,0}
};

//������Ϣ�޸�
static const GUI_WIDGET_CREATE_INFO widgetModifyMeterInfo[] = {
	{ FRAMEWIN_CreateIndirect, "������Ϣ�޸�", CONFIG_BASE_INFO_MODIFY_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 95, 4, 66, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 95, 32, 68, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�", GUI_ID_BUTTON2, 4, 265, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�·�", GUI_ID_BUTTON3, 141, 265, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "������", GUI_ID_BUTTON0, 4, 3, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "������", GUI_ID_TEXT0, 4, 34, 71, 17, 0, 0 },
	{ BUTTON_CreateIndirect, "��ѯ", GUI_ID_BUTTON1, 172, 5, 48, 45, 0, 0 },

	{ TEXT_CreateIndirect, "������", TEXT_MODIFY_1INFO_ROWID, 4, 67, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_ROWID, 65, 66, 53, 20, 0, 0 },
	{ TEXT_CreateIndirect, "���", TEXT_MODIFY_1INFO_METERADDR, 4, 93, 46, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_METERADDR, 65, 90, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "����", TEXT_MODIFY_1INFO_VENDORID, 4, 115, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VENDORID, 65, 114, 33, 20, 0, 0 },
	{ TEXT_CreateIndirect, "�汾", TEXT_MODIFY_1INFO_PROTOVER, 128, 65, 42, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_PROTOVER, 181, 64, 30, 20, 0, 0 },
	{ TEXT_CreateIndirect, "������", TEXT_MODIFY_1INFO_METERTYPE, 109, 114, 50, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_METERTYPE, 180, 113, 29, 20, 0, 0 },
	{ TEXT_CreateIndirect, "ͨ��", TEXT_MODIFY_1INFO_CHANNEL, 4, 141, 49, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_CHANNEL, 65, 140, 34, 20, 0, 0 },
	{ TEXT_CreateIndirect, "���ذ汾", TEXT_MODIFY_1INFO_VALVEPROTOVER, 113, 140, 65, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VALVEPROTOVER, 180, 140, 30, 20, 0, 0 },
	{ TEXT_CreateIndirect, "����", TEXT_MODIFY_1INFO_VALVEADDR, 4, 166, 54, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_VALVEADDR, 65, 164, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "���", TEXT_MODIFY_1INFO_CONTROLPANELADDR, 4, 188, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_CONTROLPANELADDR, 65, 190, 145, 20, 0, 0 },
	{ TEXT_CreateIndirect, "¥��", TEXT_MODIFY_1INFO_BUILDID, 4, 211, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_BUILDID, 65, 212, 42, 20, 0, 0 },
	{ TEXT_CreateIndirect, "��Ԫ", TEXT_MODIFY_1INFO_UNITID, 119, 214, 36, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_UNITID, 162, 214, 48, 20, 0, 0 },
	{ TEXT_CreateIndirect, "����", TEXT_MODIFY_1INFO_ROOMID, 4, 238, 42, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", EDIT_MODIFY_1INFO_ROOMID, 65, 235, 53, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetTimeNodes[] = {
	{ FRAMEWIN_CreateIndirect, "����ʱ���", CONFIG_TNODE_FRAME_IDX, 0, 1, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ TEXT_CreateIndirect, "��ʼʱ��", GUI_ID_TEXT0, 15, 18, 47, 20, 0, 0 },
	{ TEXT_CreateIndirect, "����", GUI_ID_TEXT1, 13, 42, 48, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 77, 15, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 77, 40, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "����", GUI_ID_BUTTON0, 172, 15, 49, 42, 0, 0 },
	{ MULTIEDIT_CreateIndirect, "", GUI_ID_MULTIEDIT0, 14, 73, 200, 121, 0, 0 },
	{ BUTTON_CreateIndirect, "������", GUI_ID_BUTTON1, 15, 220, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 126, 220, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�", GUI_ID_BUTTON2, 10, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�·�", GUI_ID_BUTTON3, 126, 260, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetModifyGatewayId[] = {
	{ FRAMEWIN_CreateIndirect, "�޸ļ��������", CONFIG_MODIFY_GATEWAYNO_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "ԭ���", GUI_ID_BUTTON0, 10, 30, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 125, 31, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 125, 80, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�", GUI_ID_BUTTON1, 9, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�޸�", GUI_ID_BUTTON2, 146, 260, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "�ֱ��", GUI_ID_TEXT0, 10, 82, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetModifyGPRS[] = {
	{ FRAMEWIN_CreateIndirect, "�޸�GPRS����", CONFIG_GPRS_PARASET_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "��������", GUI_ID_BUTTON0, 15, 12, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "IP", GUI_ID_TEXT0, 30, 45, 50, 20, 0, 0 },
	{ TEXT_CreateIndirect, "�˿ں�", GUI_ID_TEXT1, 15, 75, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "APN", GUI_ID_TEXT2, 15, 105, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "��ȡ", GUI_ID_BUTTON1, 15, 159, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�", GUI_ID_BUTTON2, 15, 260, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�޸�", GUI_ID_BUTTON3, 129, 260, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 12, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT1, 80, 45, 130, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT2, 130, 75, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT3, 130, 105, 80, 20, 0, 0 }
};

static const GUI_WIDGET_CREATE_INFO widgetReboot[] = {
	{ FRAMEWIN_CreateIndirect, "����������", CONFIG_REBOOT_GATEWAY_FRAME_IDX, 0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "��������", GUI_ID_BUTTON0, 15, 20, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "", GUI_ID_EDIT0, 130, 20, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�", GUI_ID_BUTTON1, 13, 245, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "����", GUI_ID_BUTTON2, 130, 245, 80, 20, 0, 0 }
};

/************************************************************************/
/* Init����Ⱥ                                                           */
/************************************************************************/
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

//���޸Ļ�����Ϣ�����ʼ��
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
	hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip��ַ
	EDIT_SetMaxLen(hItem, 20);
}

static void rebootInit(WM_HWIN hDlg){}
/************************************************************************/
/* CallBack����Ⱥ                                                       */
/************************************************************************/
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
	if (isNumber(lu8InputBuf, STRLEN(lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	supplementTo12(lu8InputBuf);
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
	if (isNumber(lu8InputBuf, STRLEN(lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	supplementTo12(lu8InputBuf);

	if (logic_issueMeterInfo(lu8InputBuf) == NO_ERR) {
		//GUI_MessageBox("\n�·����ַ�ɹ�!\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		//GUI_MessageBox("\n�·����ַʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
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
		GUI_MessageBox("\n����������������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (STRLEN(lu8GatewayId) > DB_MINFO_LEN_GATEWAYID) {
		GUI_MessageBox("\n�������Ź���!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(lu8GatewayId);

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
	EDIT_GetText(hObj, (char*)lu8MeterId, EDIT_MAX_LEN);
	trimSpace(lu8MeterId, STRLEN(lu8MeterId));
	if (isNumber(lu8MeterId, STRLEN(lu8MeterId)) == ERROR) {
		GUI_MessageBox("\n����������������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	if (logic_queryOneMeterInfo(lu8GatewayId, Lib_atoi((char*)lu8MeterId), &oneMeterInfoStr) == NO_ERR) {
		setListView(hDlg, &oneMeterInfoStr);
	}
	else {
		GUI_MessageBox("\n��ѯ��������Ϣʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
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
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(lu8GatewayId);

	if (readListView(hDlg, &oneMeterInfoStr) == ERROR) {
		return;
	}
	
	if (logic_issueOneMeterInfo(lu8GatewayId, &oneMeterInfoStr) == NO_ERR) {
		GUI_MessageBox("\n�޸ĵ������ݳɹ�!\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
	}
	else {
		GUI_MessageBox("\n�޸ĵ�������ʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
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
		case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id) {
			case GUI_ID_BUTTON0://�㲥��ȡ��������
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://��DBF��ѯ��Ӧ������Ļ�����Ϣ
				queryOneInfo(hDlg);
				break;
			case GUI_ID_BUTTON2://�˳�
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://�·������Ǳ������Ϣ
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
	case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://�㲥��ȡ��������
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;		
		case GUI_KEY_NUM2://��DBF��ѯ��Ӧ������Ļ�����Ϣ
			queryOneInfo(hDlg);
			break;
		case GUI_KEY_NUM3://�·������Ǳ������Ϣ
			userIssueOneInfo(hDlg);
			break;
		case GUI_KEY_NUM4://�˳�
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
		GUI_MessageBox("\n�������ʱ���ʽ����!\n", "����", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	hEdit = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
	EDIT_GetText(hEdit, (char*)nodes, 5);
	if (isNumber(nodes, STRLEN(nodes)) == ERROR) {
		GUI_MessageBox("\nʱ������, ����������!\n", "����", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (Lib_atoi((const char*)nodes) > 24) {
		GUI_MessageBox("\nʱ����������С��24!\n", "����", GUI_MESSAGEBOX_CF_MODAL);
	}
	if (logic_genTimeNodesStr(bufTimeNodes, 1024, startTime, Lib_atoi((const char*)nodes)) == ERROR) {
		GUI_MessageBox("\n����ʱ���ʧ��\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
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
		GUI_MessageBox("\n�·�����ʱ���ʧ��\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n�·�����ʱ���ɹ�\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
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
		case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id) {
			case GUI_ID_BUTTON0://���ɳ���ʱ���
				userGenTimeNodes(hDlg);
				break;
			case GUI_ID_BUTTON1://�㲥��ȡ��������
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
				break;
			case GUI_ID_BUTTON2://�˳�
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://�·�����ʱ���
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
	case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://���ɳ���ʱ���
			userGenTimeNodes(hDlg);
			break;
		case GUI_KEY_NUM2://�㲥��ȡ��������
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
			break;
		case GUI_KEY_NUM3://�˳�
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM4://�·�����ʱ���
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
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	supplementTo12(originalId);
	supplementTo12(targetId);
	if (logic_modifyGatewayId(originalId, targetId) == ERROR) {
		GUI_MessageBox("\n�޸ļ�������ʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n�޸ļ������ųɹ�!\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
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
		case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id) {
			case GUI_ID_BUTTON0://�㲥����������
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://�˳�
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://�޸ļ�������
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
	case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://�㲥����������
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
			break;
		case GUI_KEY_NUM2://�˳�
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://�޸ļ�������
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

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);//������id
	EDIT_GetText(hObj, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR) {
		GUI_MessageBox("\n����������\n", "����", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	supplementTo12(gatewayId);
	if (logic_readGPRSParam(gatewayId, apnId, serverIp, port) == ERROR) {
		GUI_MessageBox("\n��ȡ����\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip��ַ
		EDIT_SetText(hObj, (const char*)serverIp);
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);//�˿ں�
		EDIT_SetText(hObj, (const char*)port);
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT3);//apn���
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

	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);//������id
	EDIT_GetText(hObj, (char*)gatewayId, 2 * GATEWAY_OADD_LEN);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip��ַ
	EDIT_GetText(hObj, (char*)serverIp, 20);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT2);//�˿ں�
	EDIT_GetText(hObj, (char*)port, 5);
	hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT3);//apn���
	EDIT_GetText(hObj, (char*)apnId, 3);
	if (isNumber(gatewayId, STRLEN(gatewayId)) == ERROR \
		|| isNumber(port, STRLEN(port)) == ERROR \
		|| isNumber(apnId, STRLEN(apnId)) == ERROR) {
		GUI_MessageBox("\n����������\n", "����", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (IpLegal(serverIp, STRLEN(serverIp)) == ERROR) {
		GUI_MessageBox("\nIP��ַ�Ƿ�\n", "�Ƿ�", GUI_MESSAGEBOX_CF_MODAL);
		hObj = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);//ip��ַ
		EDIT_SetText(hObj, "");
		return;
	}
	supplementTo12(gatewayId);
	if(logic_modifyGPRSParam(gatewayId, apnId, serverIp, port) == ERROR)
		GUI_MessageBox("\n�޸�GPRS����ʧ��\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
	else
		GUI_MessageBox("\n�޸�GPRS�����ɹ�\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
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
		case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id) {
			case GUI_ID_BUTTON0://�㲥����������
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://��ѯ���в���
				userQueryGPRS(hDlg);
				break;
			case GUI_ID_BUTTON2://�˳�
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON3://�޸Ĳ���
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
	case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://�㲥����������
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT2));
			break;
		case GUI_KEY_NUM2://��ѯ���в���
			userQueryGPRS(hDlg);
			break;
		case GUI_KEY_NUM3://�˳�
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM4://�޸Ĳ���
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
	supplementTo12(gatewayId);
	if (logic_reboot(gatewayId) == ERROR) {
		GUI_MessageBox("\n����������ʧ��\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
	} else {
		GUI_MessageBox("\n3�������������\n", "�ɹ�", GUI_MESSAGEBOX_CF_MODAL);
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
		case WM_NOTIFICATION_RELEASED: //��������Ϣ
			switch (Id) {
			case GUI_ID_BUTTON0://�㲥����������
				radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
				break;
			case GUI_ID_BUTTON1://�˳�
				GUI_EndDialog(hDlg, WM_USER_EXIT);
				break;
			case GUI_ID_BUTTON2://����
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
	case WM_KEY: //������Ϣ
		switch (((WM_KEY_INFO *)(pMsg->Data.p))->Key) {
		case GUI_KEY_ESCAPE://Exit
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM1://�㲥����������
			radioReadGatewayId(WM_GetDialogItem(hDlg, GUI_ID_EDIT0));
			break;
		case GUI_KEY_NUM2://�˳�
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_NUM3://����
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

/************************************************************************/
/* �������溯��Ⱥ                                                       */
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

int dispConfig(){
    int iRet;
    while(1){
        iRet = GUI_ExecDialogBox(widgetSetParaAndControl, GUI_COUNTOF(widgetSetParaAndControl), &setParaAndControlCb, WM_HBKWIN, 0, 0);
        switch(iRet){
        case GUI_ID_BUTTON0://Уʱ
			setTimeConfig();
            break;
        case GUI_ID_BUTTON1://����ʱ
			setTimeNodes();
            break;
        case GUI_ID_BUTTON2://�޸ļ�������
			modifyGatewayId();
            break;
        case GUI_ID_BUTTON3://GPRS����
			modifyGPRS();
            break;
        case GUI_ID_BUTTON4://��������
            break;
        case GUI_ID_BUTTON5://������Ϣ�·�
            baseinfoIssue();
            break;
        case GUI_ID_BUTTON6://������Ϣ�޸�
			modifyOneInfo();
            break;
        case GUI_ID_BUTTON7://����������
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

int maingui( void ){
    int iRet;

	while (1) {
        iRet=GUI_ExecDialogBox(widgetMainFrame, GUI_COUNTOF(widgetMainFrame), &mainCb, WM_HBKWIN, 0, 0);//������ִ�жԻ���
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

