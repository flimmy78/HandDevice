/***************************************************
**	ģ�鹦�ܣ�	���ú���
**	ģ������:	lib.c
**	���ߣ�		�α���
****************************************************
*/
#include "basedef.h"
#include "lib.h"
#include "user.h"

static const U8 spaces[] = { ' ', '\t', '\n', '\r', '\0' };

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

//У��, �ۼӺ�ȡ����һ
U8 countCheck(U8 *data, U16 len)
{
	U8 cs = 0;
	U16 i;
	for (i = 0;i < len;i++, data++)
		cs += *data;

	return ((~cs) + 1);
}

/*
**	�ж��ַ��Ƿ��ǿո�.
**	@s:	�ַ�
*/
U8 isSpace(U8 s)
{
	U16 i = 0;
	U16 arrayLen = sizeof(spaces);
	for (i = 0;i < arrayLen;i++)
		if(s == spaces[i])
			return IS_SPACE;
	return NOT_SPACE;
}

/*
**	ȥ���ַ����еĿո�.
**	@s:		�ַ���
**	@len:	�ַ�������
*/
U8 trimSpace(U8* s, U16 len)
{
	U8* p = s;
	U8	buf[256];
	U16 i=0 ,j=0;
	for (i = 0, j = 0;i < len; p++, i++) {
		if(!isSpace(*p)) {
			buf[j] = *p;
			j++;
		}
	}
	memset(s, 0x00, len);
	memcpy(s, buf, j+1);
	return NO_ERR;
}

/*
**	����ַ����Ƿ�������.
**	@s:		�ַ���
**	@len:	�ַ�������
*/
U8 isNumber(U8* s, U16 len)
{
	U16 i = 0;
	for (i = 0;i < len; s++, i++) {
		if (!isdigit(*s)) {
			return ERROR;
		}
	}

	return NO_ERR;
}

/*
**	����������ַ�����BCD��.
**	@s:		Դ�ַ���
**	@sLen:	Դ�ַ�������
**	@t:		Ŀ���ַ���
**	@tLen:	Ŀ���ַ�������
*/
U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen)
{
	U16 i = 0;

	if (sLen % 2)
		return ERROR;

	for (i = 0;i < sLen/2;i++, s += 2) {//convert ascii to hex each two chars
		t[i] = (ASCII_TO_HEX(s[0]) << 4 | ASCII_TO_HEX(s[1]));
	}

	inserseArray(t, i);
	return NO_ERR;
}

void inserseArray(U8* buf, U16 bufSize)
{
	U16 i = 0;
	for (i = 0;i < bufSize / 2; i++) {//swap two symmetric elements
		buf[i] = buf[i] ^ buf[bufSize - i - 1];
		buf[bufSize - i - 1] = buf[i] ^ buf[bufSize - i - 1];
		buf[i] = buf[i] ^ buf[bufSize - i - 1];
	}
}
