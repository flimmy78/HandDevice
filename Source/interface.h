#ifndef INTERFACE_H
#define INTERFACE_H

#define CL998_LCD_XLEN  240//����998�ֳֻ���Ļ���, �ο�"�ֳֻ�����(��)\1���ĵ�\C_��Ʒ��������\�����Ʒ����.pdf"
#define CL998_LCD_YLEN  320//����998�ֳֻ���Ļ�߶�, �ο�"�ֳֻ�����(��)\1���ĵ�\C_��Ʒ��������\�����Ʒ����.pdf"

#define BASE_FRAME_IDX  1000//������Ļ������

#define CONFIG_FRAME_IDX                    1010//������������ƽ���������
#define CONFIG_CTIME_FRAME_IDX              1011//Уʱ����ı��
#define CONFIG_TNODE_FRAME_IDX              1012//����ʱ����ı��
#define CONFIG_MODIFY_GATEWAYNO_FRAME_IDX   1013//�޸ļ������Ž���ı��
#define CONFIG_GPRS_PARASET_FRAME_IDX       1014//GPRS��������ı��
#define CONFIG_REREAD_PARASET_FRAME_IDX     1015//�������ý���ı��
#define CONFIG_BASE_INFO_FRAME_IDX          1016//������Ϣ�·�����ı��
#define CONFIG_BASE_INFO_MODIFY_FRAME_IDX   1017//������Ϣ�޸Ľ���ı��
#define CONFIG_REBOOT_GATEWAY_FRAME_IDX     1018//��������������ı��


#define INFOQ_EDIT_FRAME_IDX            1020//��Ϣ��ѯ���༭����ı��
#define INFOQ_READ_IMDT_FRAME_IDX       1021//�����������ı��
#define INFOQ_HISDATA_FRAME_IDX         1022//��ʷ���ݽ���ı��
#define INFOQ_READ_BASEINFO_FRAME_IDX   1023//��ȡ������Ϣ����ı��
#define INFOQ_SIGLE_READ_FRAME_IDX      1024//��������ı��
#define INFOQ_VER_FRAME_IDX             1025//����汾��ѯ����ı��

#define COM_CONFIG_FRAME_IDX  1030//�ֳֻ��������ý���ı��


int maingui(void);

#endif//INTERFACE_H
