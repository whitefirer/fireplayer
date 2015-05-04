/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : parse lrc files.
 * E-Mail : whitefirer@gmail.com
 */

#ifndef _LRC_PARSE_H_
#define _LRC_PARSE_H_
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct tagLrcTime
{
	unsigned long time;
	struct tagLrcTime *next;
}*PLRCTIME, LRCTIME;

typedef struct tagLrcText
{
	 unsigned long time;
	 char *Text;
	 struct tagLrcText *next;
}*PLRCTEXT, LRCTEXT; // as a tree or a list?

typedef struct tagLrcAr
{
	char *key;
	char *value;
	struct tagLrcAr *next;
}*PLRCAR, LRCAR; // as a list?

typedef struct tagLrc
{
	PLRCAR ArHead;
	char *Buffer;
	PLRCTEXT LrcTextHead;
}*PLRC, LRC; // Lrc

int CreateLrcFromFile(PLRC pLrc, char *FilePath);
int CreateLrc(PLRC pLrc, char *Buffer);
void DestroyLrc(LRC Lrc); // You should do it at last.
void DisplayLrc(const LRC Lrc);
void DisplayLrcAr(const PLRCAR pLrcAr);
void DisplayLrcText(const PLRCTEXT pLrcText);
#endif