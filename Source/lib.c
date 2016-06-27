/***************************************************
**	ģ�鹦�ܣ�	���ú���
**	ģ������:	lib.c
**	���ߣ�		�α���
****************************************************
*/
#include "basedef.h"
#include "lib.h"
#include "user.h"


//��ȡϵͳʱ��
U8 readSysTime(sys_time_ptr pTime)
{
	RtcGetDate(&(pTime->u8year), &(pTime->u8month), &(pTime->u8day));
	RtcGetTime(&(pTime->u8hour), &(pTime->u8minute), &(pTime->u8second));
	pTime->u8year	= HEX_TO_BCD(pTime->u8year);
	pTime->u8month	= HEX_TO_BCD(pTime->u8month);
	pTime->u8day		= HEX_TO_BCD(pTime->u8day);
	pTime->u8hour	= HEX_TO_BCD(pTime->u8hour);
	pTime->u8minute = HEX_TO_BCD(pTime->u8minute);
	pTime->u8second = HEX_TO_BCD(pTime->u8second);

	return 0;
}

//��Ϣͷ����Ϣ��У��
U8 countCheck(U8 *_data, U16 _len)
{
	U8 cs = 0;
	U16 i;
	for (i = 0;i < _len;i++, _data++)
		cs += *_data;

	return ((~cs) + 1);
}
