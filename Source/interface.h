#ifndef INTERFACE_H
#define INTERFACE_H

#include "basedef.h"

#define CL998_LCD_XLEN  240//����998�ֳֻ���Ļ���, �ο�"�ֳֻ�����(��)\1���ĵ�\C_��Ʒ��������\�����Ʒ����.pdf"
#define CL998_LCD_YLEN  320//����998�ֳֻ���Ļ�߶�, �ο�"�ֳֻ�����(��)\1���ĵ�\C_��Ʒ��������\�����Ʒ����.pdf"

#define CONFIG_FRAME_IDX                    (GUI_ID_USER+0x10)//������������ƽ���������
#define CONFIG_CTIME_FRAME_IDX              (GUI_ID_USER+0x11)//Уʱ����ı��
#define CONFIG_TNODE_FRAME_IDX              (GUI_ID_USER+0x12)//����ʱ����ı��
#define CONFIG_MODIFY_GATEWAYNO_FRAME_IDX   (GUI_ID_USER+0x13)//�޸ļ������Ž���ı��
#define CONFIG_GPRS_PARASET_FRAME_IDX       (GUI_ID_USER+0x14)//GPRS��������ı��
#define CONFIG_REREAD_PARASET_FRAME_IDX     (GUI_ID_USER+0x15)//�������ý���ı��
#define CONFIG_BASE_INFO_FRAME_IDX          (GUI_ID_USER+0x16)//������Ϣ�·�����ı��
#define CONFIG_BASE_INFO_MODIFY_FRAME_IDX   (GUI_ID_USER+0x17)//������Ϣ�޸Ľ���ı��
#define CONFIG_REBOOT_GATEWAY_FRAME_IDX     (GUI_ID_USER+0x18)//��������������ı��

#define INFOQ_EDIT_FRAME_IDX            	(GUI_ID_USER+0x20)//��Ϣ��ѯ���༭����ı��
#define INFOQ_READ_IMDT_FRAME_IDX       	(GUI_ID_USER+0x21)//�����������ı��
#define INFOQ_HISDATA_FRAME_IDX         	(GUI_ID_USER+0x22)//��ʷ���ݽ���ı��
#define INFOQ_READ_BASEINFO_FRAME_IDX   	(GUI_ID_USER+0x23)//��ȡ������Ϣ����ı��
#define INFOQ_SIGLE_READ_FRAME_IDX      	(GUI_ID_USER+0x24)//��������ı��
#define INFOQ_VER_FRAME_IDX             	(GUI_ID_USER+0x25)//����汾��ѯ����ı��
#define INFOQ_HISDATA_SHEET_IDX				(GUI_ID_USER+0x26)//��ѯʧ��/�ɹ��굥����ı��

#define COM_CONFIG_FRAME_IDX  				(GUI_ID_USER+0x30)//�ֳֻ��������ý���ı��

#define EDIT_MODIFY_1INFO_ROWID				GUI_ID_USER+0x101//������
#define EDIT_MODIFY_1INFO_METERADDR			GUI_ID_USER+0x102//�Ǳ�(������,ˮ���)��ַ
#define EDIT_MODIFY_1INFO_VENDORID			GUI_ID_USER+0x103//���̴���
#define EDIT_MODIFY_1INFO_PROTOVER			GUI_ID_USER+0x104//�Ǳ�Э��汾��
#define EDIT_MODIFY_1INFO_METERTYPE			GUI_ID_USER+0x105//�Ǳ�����
#define EDIT_MODIFY_1INFO_CHANNEL			GUI_ID_USER+0x106//ͨ����
#define EDIT_MODIFY_1INFO_VALVEPROTOVER		GUI_ID_USER+0x107//����Э��汾��
#define EDIT_MODIFY_1INFO_VALVEADDR			GUI_ID_USER+0x108//���ص�ַ
#define EDIT_MODIFY_1INFO_CONTROLPANELADDR	GUI_ID_USER+0x109//�¿�����ַ
#define EDIT_MODIFY_1INFO_BUILDID			GUI_ID_USER+0x10A//¥����
#define EDIT_MODIFY_1INFO_UNITID			GUI_ID_USER+0x10B//��Ԫ��
#define EDIT_MODIFY_1INFO_ROOMID			GUI_ID_USER+0x10C//�����

#define TEXT_MODIFY_1INFO_ROWID				GUI_ID_USER+0x111//������
#define TEXT_MODIFY_1INFO_METERADDR			GUI_ID_USER+0x112//�Ǳ�(������,ˮ���)��ַ
#define TEXT_MODIFY_1INFO_VENDORID			GUI_ID_USER+0x113//���̴���
#define TEXT_MODIFY_1INFO_PROTOVER			GUI_ID_USER+0x114//�Ǳ�Э��汾��
#define TEXT_MODIFY_1INFO_METERTYPE			GUI_ID_USER+0x115//�Ǳ�����
#define TEXT_MODIFY_1INFO_CHANNEL			GUI_ID_USER+0x116//ͨ����
#define TEXT_MODIFY_1INFO_VALVEPROTOVER		GUI_ID_USER+0x117//����Э��汾��
#define TEXT_MODIFY_1INFO_VALVEADDR			GUI_ID_USER+0x118//���ص�ַ
#define TEXT_MODIFY_1INFO_CONTROLPANELADDR	GUI_ID_USER+0x119//�¿�����ַ
#define TEXT_MODIFY_1INFO_BUILDID			GUI_ID_USER+0x11A//¥����
#define TEXT_MODIFY_1INFO_UNITID			GUI_ID_USER+0x11B//��Ԫ��
#define TEXT_MODIFY_1INFO_ROOMID			GUI_ID_USER+0x11C//�����


#define LISTVIEW_COL_ITEM	0//�޸ĵ��л�����Ϣʱ, ��ͷ������
#define LISTVIEW_COL_VALUE	1//�޸ĵ��л�����Ϣʱ, ֵ������

extern int maingui(void);

#endif//INTERFACE_H
