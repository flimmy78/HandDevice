/***************************************************
**	模块功能:	主函数
**	模块名字:	application.c
**	作者：		宋宝善
****************************************************
*/
#include "user.h"
#include "db.h"
#include "interface.h"

int main(void){
//	KeyWait();

	db_readAllConfig();
    LcdClear();
    maingui();
    return 0;
}
