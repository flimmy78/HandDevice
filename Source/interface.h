#ifndef INTERFACE_H
#define INTERFACE_H

#define CL998_LCD_XLEN  240//创伦998手持机屏幕宽度, 参考"手持机光盘(新)\1、文档\C_产品宣传资料\三款产品参数.pdf"
#define CL998_LCD_YLEN  320//创伦998手持机屏幕高度, 参考"手持机光盘(新)\1、文档\C_产品宣传资料\三款产品参数.pdf"

#define BASE_FRAME_IDX  1000//主界面的基础编号

#define CONFIG_FRAME_IDX                    1010//参数设置与控制界面基础编号
#define CONFIG_CTIME_FRAME_IDX              1011//校时界面的编号
#define CONFIG_TNODE_FRAME_IDX              1012//抄表定时界面的编号
#define CONFIG_MODIFY_GATEWAYNO_FRAME_IDX   1013//修改集中器号界面的编号
#define CONFIG_GPRS_PARASET_FRAME_IDX       1014//GPRS参数界面的编号
#define CONFIG_REREAD_PARASET_FRAME_IDX     1015//补抄设置界面的编号
#define CONFIG_BASE_INFO_FRAME_IDX          1016//基础信息下发界面的编号
#define CONFIG_BASE_INFO_MODIFY_FRAME_IDX   1017//基础信息修改界面的编号
#define CONFIG_REBOOT_GATEWAY_FRAME_IDX     1018//重启集中器界面的编号


#define INFOQ_EDIT_FRAME_IDX            1020//信息查询及编辑界面的编号
#define INFOQ_READ_IMDT_FRAME_IDX       1021//立即抄表界面的编号
#define INFOQ_HISDATA_FRAME_IDX         1022//历史数据界面的编号
#define INFOQ_READ_BASEINFO_FRAME_IDX   1023//读取基础信息界面的编号
#define INFOQ_SIGLE_READ_FRAME_IDX      1024//单抄界面的编号
#define INFOQ_VER_FRAME_IDX             1025//程序版本查询界面的编号

#define COM_CONFIG_FRAME_IDX  1030//手持机基本设置界面的编号


int maingui(void);

#endif//INTERFACE_H
