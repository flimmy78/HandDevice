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
	{ FRAMEWIN_CreateIndirect, "������Ϣ�޸�",	CONFIG_BASE_INFO_MODIFY_FRAME_IDX,	0, 0, CL998_LCD_XLEN, CL998_LCD_YLEN, 0, 0 },
	{ BUTTON_CreateIndirect, "��������",			GUI_ID_BUTTON0,						9, 6, 80, 20, 0, 0 },
	{ TEXT_CreateIndirect, "������",				GUI_ID_TEXT0,						15, 38, 71, 20, 0, 0 },
	{ EDIT_CreateIndirect, "",					GUI_ID_EDIT0,						124, 7, 80, 20, 0, 0 },
	{ EDIT_CreateIndirect, "",					GUI_ID_EDIT1,						124, 32, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "��ѯ",				GUI_ID_BUTTON1,						12, 60, 80, 20, 0, 0 },
	{ LISTVIEW_CreateIndirect, "",				GUI_ID_LISTVIEW0,					9, 86, 210, 192, 0, 0 },
	{ BUTTON_CreateIndirect, "�޸�",				GUI_ID_BUTTON2,						138, 281, 80, 20, 0, 0 },
	{ BUTTON_CreateIndirect, "�˳�",				GUI_ID_BUTTON3,						9, 279, 80, 20, 0, 0 }
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

//�Ի�����Ϣ�·������ʼ��
static void ModifyOneInfoInit(WM_HWIN hDlg)
{
	WM_HWIN hListView;

	hListView = WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0);
	LISTVIEW_AddColumn(hListView, 60, "item", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hListView, 130, "value", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_SetGridVis(hListView, 1);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_AddRow(hListView, NULL);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_rowId, "������");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_meterAddr, "���ַ");	
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_vendorId, "���̴���");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_protoVer, "Э��汾");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_meterType, "�Ǳ�����");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_channel, "ͨ����");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_valveProtoVer, "����Э��汾");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_valveAddr, "���ص�ַ");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_controlPanelAddr, "�¿�����ַ");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_buildId, "¥����");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_unitId, "��Ԫ��");
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_ITEM, minfo_field_roomId, "�����");
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
	if (isNumber(lu8InputBuf, STRLEN(lu8InputBuf)) == ERROR) {
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
	if (isNumber(lu8InputBuf, STRLEN(lu8InputBuf)) == ERROR) {
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}

	suppplementTo12(lu8InputBuf);

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

void setListView(WM_HWIN hListView, db_meterinfo_ptr pInfo)
{
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_rowId, (const char*)pInfo->rowId);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_meterAddr, (const char*)pInfo->meterAddr);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_vendorId, (const char*)pInfo->vendorId);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_protoVer, (const char*)pInfo->protoVer);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_meterType, (const char*)pInfo->meterType);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_channel, (const char*)pInfo->channel);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_valveProtoVer, (const char*)pInfo->valveProtoVer);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_valveAddr, (const char*)pInfo->valveAddr);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_controlPanelAddr, (const char*)pInfo->controlPanelAddr);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_buildId, (const char*)pInfo->buildId);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_unitId, (const char*)pInfo->unitId);
	LISTVIEW_SetItemText(hListView, LISTVIEW_COL_VALUE, minfo_field_roomId, (const char*)pInfo->roomId);
}

void queryOneInfo(WM_HWIN hObjGatewayId, WM_HWIN hObjMeterId, WM_HWIN hObjListview)
{
	U8 lu8GatewayId[EDIT_MAX_LEN] = { 0 };
	U8 lu8MeterId[EDIT_MAX_LEN] = { 0 };
	db_meterinfo_str oneMeterInfoStr;

	EDIT_GetText(hObjGatewayId, (char*)lu8GatewayId, EDIT_MAX_LEN);
	EDIT_GetText(hObjMeterId, (char*)lu8MeterId, EDIT_MAX_LEN);

	trimSpace(lu8GatewayId, EDIT_MAX_LEN);
	if (isNumber(lu8GatewayId, STRLEN(lu8GatewayId)) == ERROR) {
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	trimSpace(lu8MeterId, EDIT_MAX_LEN);
	if (isNumber(lu8MeterId, STRLEN(lu8MeterId)) == ERROR) {
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	if (STRLEN(lu8GatewayId) > DB_MINFO_LEN_GATEWAYID) {
			GUI_MessageBox("\n�������Ź���!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
			return;
	}

	if (logic_queryOneMeterInfo(lu8GatewayId, Lib_atoi((char*)lu8MeterId), &oneMeterInfoStr) == NO_ERR) {
		setListView(hObjListview, &oneMeterInfoStr);
	}
	else {
		GUI_MessageBox("\n��ѯ��������Ϣʧ��!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
	}
}

U8 readListView(WM_HWIN hObjListview, db_meterinfo_ptr pInfo)
{
	S8 l8buf[EDIT_MAX_LEN] = { 0 };
	S8 msgBuf[256] = { 0 };
	U16 lu16strLen;

	LISTVIEW_TO_DBSTR(rowId, DB_MINFO_LEN_ROWID + 1, minfo_field_rowId, "������")
	LISTVIEW_TO_DBSTR(meterAddr, DB_MINFO_LEN_METERADDR, minfo_field_meterAddr, "���ַ")
	LISTVIEW_TO_DBSTR(vendorId, DB_MINFO_LEN_VENDORID + 1, minfo_field_vendorId, "���̴���")
	LISTVIEW_TO_DBSTR(protoVer, DB_MINFO_LEN_PROTOVER + 1, minfo_field_protoVer, "�Ǳ�Э��汾��")
	LISTVIEW_TO_DBSTR(meterType, DB_MINFO_LEN_METERTYPE, minfo_field_meterType, "�Ǳ�����")
	LISTVIEW_TO_DBSTR(channel, DB_MINFO_LEN_CHANNEL + 1, minfo_field_channel, "ͨ����")
	LISTVIEW_TO_DBSTR(valveProtoVer, DB_MINFO_LEN_VALVEPROTOVER + 1, minfo_field_valveProtoVer, "����Э��汾��")
	LISTVIEW_TO_DBSTR(valveAddr, DB_MINFO_LEN_VALVEADDR, minfo_field_valveAddr, "���ص�ַ")
	LISTVIEW_TO_DBSTR(controlPanelAddr, DB_MINFO_LEN_CTLPANELADDR, minfo_field_controlPanelAddr, "�¿�����ַ")
	LISTVIEW_TO_DBSTR(buildId, DB_MINFO_LEN_BUILDID + 1, minfo_field_buildId, "¥����")
	LISTVIEW_TO_DBSTR(unitId, DB_MINFO_LEN_UNITID + 1, minfo_field_unitId, "��Ԫ��")
	LISTVIEW_TO_DBSTR(roomId, DB_MINFO_LEN_ROOMID + 1, minfo_field_roomId, "�����")
	return NO_ERR;
}

void userIssueOneInfo(WM_HWIN hObjGatewayId, WM_HWIN hObjListview)
{
	U8 lu8GatewayId[EDIT_MAX_LEN] = { 0 };
	db_meterinfo_str oneMeterInfoStr;

	EDIT_GetText(hObjGatewayId, (char*)lu8GatewayId, EDIT_MAX_LEN);
	trimSpace(lu8GatewayId, EDIT_MAX_LEN);
	if (isNumber(lu8GatewayId, STRLEN(lu8GatewayId)) == ERROR) {
		GUI_MessageBox("\n����������!\n", "ʧ��", GUI_MESSAGEBOX_CF_MODAL);
		return;
	}
	suppplementTo12(lu8GatewayId);

	if (readListView(hObjListview, &oneMeterInfoStr) == ERROR) {
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
				queryOneInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0), \
					WM_GetDialogItem(hDlg, GUI_ID_EDIT1), WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0));
				break;
			case GUI_ID_BUTTON2://�·������Ǳ������Ϣ
				userIssueOneInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0), WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0));
				break;
			case GUI_ID_BUTTON3://�˳�
				GUI_EndDialog(hDlg, WM_USER_EXIT);
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
			queryOneInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0), \
				WM_GetDialogItem(hDlg, GUI_ID_EDIT1), WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0));
			break;
		case GUI_KEY_NUM3://�·������Ǳ������Ϣ
			userIssueOneInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0), WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0));
			break;
		case GUI_KEY_NUM4://�˳�
			GUI_EndDialog(hDlg, WM_USER_EXIT);
			break;
		case GUI_KEY_ENTER:
			userIssueOneInfo(WM_GetDialogItem(hDlg, GUI_ID_EDIT0), WM_GetDialogItem(hDlg, GUI_ID_LISTVIEW0));
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

int dispConfig(){
    int iRet;
    while(1){
        iRet = GUI_ExecDialogBox(widgetSetParaAndControl, GUI_COUNTOF(widgetSetParaAndControl), &setParaAndControlCb, WM_HBKWIN, 0, 0);
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
			modifyOneInfo();
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

