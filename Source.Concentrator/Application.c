/***************************************************
**	模块功能:	主函数
**	模块名字:	application.c
**	作者：		宋宝善
****************************************************
*/
#include "user.h"
#include "db.h"
#include "interface.h"

/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : WIDGET_Edit.c
Purpose     : Example demonstrating the use of a EDIT widget
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "EDIT.h"
#include "db.h"

int main(void) {
	db_readAllConfig();
	LcdClear();
	maingui();
	return 0;
}
