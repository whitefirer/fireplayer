/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : textencoding chardetect and chang.
 * E-Mail : whitefirer@gmail.com
 */

#ifndef _TEXT_ENCODE_H
#define _TEXT_ENCODE_H
#include <windows.h>

enum EncodeType{
	BINARY = 0,
	ASCII,
	UTF8_NOBOM,
	UTF8,
	UCS2_LE, // UTF16
	UCS2_BE
};

void UTF8ToASCII(char *src, char *dst, int len); // utf8->gb
void ASCIIToUTF8(char *src, char *dst, int len); // gb->utf8
char* UCS2LEToASCII(WCHAR *src);
char* UCS2BEToASCII(WCHAR *src, size_t len);
EncodeType CheckTextEncode(char* src, int len);
#endif
