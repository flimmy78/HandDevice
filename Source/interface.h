#ifndef INTERFACE_H
#define INTERFACE_H

#include "basedef.h"

#define WM_USER_EXIT	-1
#define EDIT_MAX_LEN	128

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

#define COM_CONFIG_FRAME_IDX  				(GUI_ID_USER+0x30)//�ֳֻ��������ý���ı��


int maingui(void);

#endif//INTERFACE_H
