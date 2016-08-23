/***************************************************
**	模块功能:	主函数
**	模块名字:	application.c
**	作者：		宋宝善
****************************************************
*/

#include "limits.h"
#include "GUI.h"
#include "EDIT.h"
#include "user.h"
#include "db.h"
#include "interface.h"

sUART *gpu;//指向串口的指针global_pointer_uart

int main(void) {
	sUART comConfig;

	/*
	**	打开Mbus串口的动作要放在main函数中才可以抄数,
	**	而不能放在logic_sendAndRead()中.
	**	原因不明.
	*/
	db_readAllConfig();
	db_getCongfig(config_com_para, (U8*)&comConfig);
	gpu = UartOpen(comConfig.baud, comConfig.mode, comConfig.device);//打开串口
	if (!gpu) {
		GUI_MessageBox("\n打开串口失败\n", "失败", GUI_MESSAGEBOX_CF_MODAL);
		return ERROR;
	}
	LcdClear();
	maingui();
	UartClose(gpu);
	return 0;
}
