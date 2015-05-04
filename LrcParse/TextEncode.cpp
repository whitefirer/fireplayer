/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : TextEncode chardetect and chang.
 * E-Mail : whitefirer@gmail.com
 */

#include "stdafx.h"
#include "TextEncode.h"
#include <malloc.h>
#include <stdlib.h>

char* UCS2BEToASCII(WCHAR *src, size_t len)
{
	char *p = (char*)src;
	for(int i = 0; i < len; i+=2)
	{
		p[i] ^= p[i+1];
		p[i+1]  ^= p[i];
		p[i] ^= p[i+1];
	}

	int ByteCount = WideCharToMultiByte(CP_ACP , 0, src, -1, 0, 0, "\0", 0);
	if(ByteCount <= 0)
	{
		return NULL;
	}

	char *pszRet = (char*)malloc(sizeof(WCHAR) * ByteCount);
	ByteCount = WideCharToMultiByte(CP_ACP, 0, src, -1, pszRet, ByteCount, "\0", 0);
	if(ByteCount > 0)
		pszRet[ByteCount] = 0; 

	return pszRet;
}

char* UCS2LEToASCII(WCHAR *src)
{
	int ByteCount = WideCharToMultiByte(CP_ACP , 0, src, -1, 0, 0, "\0", 0);
	if(ByteCount <= 0)
	{
		return NULL;
	}
	
	char *pszRet = (char*)malloc(sizeof(WCHAR) * ByteCount);

	ByteCount = WideCharToMultiByte(CP_ACP, 0, src, -1, pszRet, ByteCount, "\0", 0);
	if(ByteCount > 0)
		pszRet[ByteCount] = 0; 
	
	return pszRet;
}

void UTF8ToASCII(char *src, char *dst, int len)
{
	int ret = 0;
	WCHAR* strA;
	int ByteCount = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);

	if(ByteCount <= 0)
	{
		return;
	}
	strA = (WCHAR*)malloc(ByteCount * sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0 , src, -1, strA, ByteCount);

	ByteCount = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
	
	if(len < ByteCount)
	{
		free(dst);
		dst = (char *)malloc(ByteCount * sizeof(WCHAR));	
	}

	ret = WideCharToMultiByte(CP_ACP, 0, strA, -1, dst, ByteCount, NULL, NULL);
	dst[ByteCount] = 0;

	free(strA);
}

void ASCIIToUTF8(char *src, char *dst, int len)
{
	int ret = 0;
	WCHAR* strA;
	int ByteCount = MultiByteToWideChar (CP_ACP, 0, src, -1, NULL, NULL);

	if(ByteCount <= 0)
	{
		 return;
	}
	strA = (WCHAR*)malloc(ByteCount * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, src, -1, strA, ByteCount);
	ByteCount = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
	if(len >= ByteCount)
	{
		ret = WideCharToMultiByte (CP_UTF8, 0, strA, -1, dst, ByteCount, NULL, NULL);
		dst[ByteCount] = 0;
	}
	if(ret <= 0)
	{
		free(strA);
		return;
	}

	free(strA);
}

EncodeType CheckTextEncode(char *src, int len)
{
	//bool bUTF8 = true;
	int i = 0;
	bool BOMflag = false;

	if(!src || (len < 2))
		return ASCII;

	unsigned char *pVal8 = (unsigned char *)src;
	unsigned short *pVal16 = (unsigned short *)src;

	/*for(int i = 0; i < (len - 2); i = i + 2)
    {
		if(0x0000 == *pVal16++)
			return BINARY;
    }*/
	pVal16 = (unsigned short *)src;

    if (*pVal16 == 0xFEFF)
	{
		return UCS2_LE;
	}
	else if(*pVal16 == 0xFFFE)
	{
		return UCS2_BE;
	}
	// Ìø¹ýUTF8 BOM
	if((pVal8[0] == 0xEF) && (pVal8[1] == 0xBB) && (pVal8[2] == 0xBF))
	{
		pVal8 += 3;
		BOMflag = true;
	}
	if((pVal8[0] == 0xBB) && (pVal8[1] == 0xEF) && (pVal8[2] == 0xBF))
	{
		pVal8 += 3;
		BOMflag = true;
	}

	for (i = 3; i < len; ++i)
	{
		if((*pVal8 == 0xC0) || (*pVal8 == 0xC1) || (*pVal8 >= 0xF5))
		{
			return ASCII;
		}
		if((*pVal8 & 0xE0)==0xC0)
		{
			pVal8++;
			i++;
			if(*pVal8 == 0)
				break;
			if((*pVal8 & 0xC0)!=0x80)
			{
				return ASCII;
			}
		}
		if((*pVal8 & 0xF0) == 0xE0)
		{
			pVal8++;
			i++;
			if(*pVal8 == 0)
				break;
			if((*pVal8 & 0xC0) != 0x80)
			{
				return ASCII;
			}
			pVal8++;
			i++;
			if(*pVal8 == 0)
				break;
			if((*pVal8 & 0xC0) != 0x80)
			{
				return ASCII;
			}
		}
		if ((*pVal8 & 0xF8) == 0xF0)
		{
			pVal8++;
			i++;
			if (*pVal8 == 0)
				break;
			if ((*pVal8 & 0xC0)!=0x80)
			{
				return ASCII;
			}
			pVal8++;
			i++;
			if (*pVal8 == 0)
				break;
			if ((*pVal8 & 0xC0) != 0x80)
			{
				return ASCII;
			}
			pVal8++;
			i++;
			if (*pVal8 == 0)
				break;
			if ((*pVal8 & 0xC0) != 0x80)
			{
				return ASCII;
			}
		}

		pVal8++;
		i++;
	}

	if(i == (len - 3))
		return ASCII;
	if(BOMflag)
		return UTF8;

	return UTF8_NOBOM;
}