/***************************************************
**	ģ�鹦��:	������
**	ģ������:	application.c
**	���ߣ�		�α���
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
