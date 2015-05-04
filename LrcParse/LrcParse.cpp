/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : parse lrc files.
 * E-Mail : whitefirer@gmail.com
 */

#include "stdafx.h"
#include "LrcParse.h"
#include "TextEncode.h"

static PLRCAR InsertLrcAr(PLRCAR pLrcAr, char *pKeyStart, char *pValueStart)
{
	if(NULL == pLrcAr)
	{
		pLrcAr = (PLRCAR)malloc(sizeof(LRCAR));
		pLrcAr->key = pKeyStart;
		pLrcAr->value = pValueStart;
		pLrcAr->next = NULL;
	}
	else
	{
		PLRCAR p = pLrcAr, h = pLrcAr;
		while(p) 
		{
			h = p;
			p = p->next;
		}

		p = (PLRCAR)malloc(sizeof(LRCAR));
		p->key = pKeyStart;
		p->value = pValueStart;
		p->next = NULL;
		h->next = p;
	}

	return pLrcAr;
}

static PLRCTIME InsertLrcTime(PLRCTIME pLrcTime, unsigned long time)
{

	if(NULL == pLrcTime)
	{
		pLrcTime = (PLRCTIME)malloc(sizeof(LRCTIME));
		pLrcTime->time = time;
		pLrcTime->next = NULL;
	}
	else
	{
		PLRCTIME p = pLrcTime;
		PLRCTIME h = p;
		while(p) 
		{
			h = p;
			p = p->next;
		}
		p = (PLRCTIME)malloc(sizeof(LRCTIME));
		p->time = time;
		p->next = NULL;
		h->next = p;
	}
	return pLrcTime;
}

static PLRCTEXT InsertLrcText(PLRCTEXT pLrcText, PLRCTIME pLrcTime, char *pTextStart)
{
	if(NULL == pLrcTime)
		return pLrcText;

	if(NULL == pLrcText)
	{
		pLrcText = (PLRCTEXT)malloc(sizeof(LRCTEXT));
		pLrcText->time = pLrcTime->time;
		pLrcText->Text = pTextStart;
		pLrcText->next = NULL;

		return pLrcText;
	}

	PLRCTIME pLT = pLrcTime;
	while(pLT)
	{
		PLRCTEXT p = pLrcText;
		PLRCTEXT h = p;

		while(p)
		{
			if(pLT->time < p->time)
				break;
			h = p;
			p = p->next;
		}

		PLRCTEXT t = (PLRCTEXT)malloc(sizeof(LRCTEXT));
		t->time = pLT->time;
		t->Text = pTextStart;
		t->next = p;
		h->next = t;
		pLT = pLT->next;
	}

	return pLrcText;
}

static PLRCTIME FreeLrcAr(PLRCAR pLrcAr)
{
	PLRCAR p = pLrcAr;

	while(NULL != p)
	{
		PLRCAR h = p;
		p = p->next;
		free(h);
	}

	return NULL;
}

static PLRCTIME FreeLrcTime(PLRCTIME pLrcTime)
{
	PLRCTIME p = pLrcTime;

	while(NULL != p)
	{
		PLRCTIME h = p;
		p = p->next;
		free(h);
	}

	return NULL;
}

static PLRCTEXT FreeLrcText(PLRCTEXT pLrcText)
{
	PLRCTEXT p = pLrcText;

	while(NULL != p)
	{
		PLRCTEXT h = p;
		p = p->next;
		free(h);
	}

	return NULL;
}

int LrcParse(PLRC pLrc)
{
	PLRCAR pLrcAr = pLrc->ArHead;
	PLRCTEXT pLrcText = pLrc->LrcTextHead;

	PLRCTIME pLrcTime = NULL;
	char *p = pLrc->Buffer;
	char *pTimeStart = NULL;
	char *pMinStart = NULL;
	char *pSecStart = NULL;
	char *pTextStart = NULL;
	char *pKeyStart = NULL;
	char *pValueStart = NULL;
	enum _State
	{
		STAT_NONE = 0,
		STAT_GROUP,
		STAT_TIME,
		STAT_KEY,
		STAT_VALUE,
		STAT_TEXT
	}state = STAT_NONE;

	for(p = pLrc->Buffer; *p != '\0'; p++)
	{
		switch(state)
		{
		case STAT_NONE:
			if('[' == *p)
			{
				state = STAT_GROUP;
			}
			break;
		case STAT_GROUP:
			if((*p >= '0') && (*p <= '9'))
			{
				state = STAT_TIME;
				pTimeStart = p;
			}
			else 
			{
				state = STAT_KEY;
				pKeyStart = p;
			}
			break;
		case STAT_KEY:
			if(':' == *p)
			{
				*p = '\0';
				state = STAT_VALUE;
				pValueStart = p + 1;
			}
			break;
		case STAT_VALUE:
			if(']' == *p)
			{
				*p = '\0';
				state = STAT_NONE;
				//printf("%s, %s\n", pKeyStart, pValueStart);
				//Add attribute
				pLrcAr = InsertLrcAr(pLrcAr, pKeyStart, pValueStart);
				pKeyStart = NULL;
				pValueStart = NULL;
			}
			else if(('\n' == *p) || ('\r' == *p))
				state = STAT_NONE;
			break;
		case STAT_TIME:
			if(':' == *p)
			{
				*p = '\0';
				pMinStart = pTimeStart;
				pSecStart = p + 1;
			}
			else if((']' == *p))
			{
				*p = '\0';
				state = STAT_NONE;
				pTextStart = p + 1;
				if(pMinStart && pSecStart)
				{
					//printf("Time:%s:%s\t", pMinStart, pSecStart);
					unsigned long t = abs(atol(pMinStart)) * 60 * 1000 + abs(atof(pSecStart) * 1000);
					// Add time to TimeList (NO ODRE)
					pLrcTime = InsertLrcTime(pLrcTime, t);
				}
				if(('[' != *pTextStart))
				{	
					state = STAT_TEXT;
					if((('\n' == *pTextStart) || ('\0' == *pTextStart)))
						pTextStart = "......";
				}
				//else printf("\n");
				
				pMinStart = NULL;
				pSecStart = NULL;
			}
			else if(('\n' == *p) || ('\r' == *p))
				state = STAT_NONE;
			break;
		case STAT_TEXT:
			if(('\n' == *p) || ('\0' == *p))
			{
				*p = '\0';
				state = STAT_NONE;
				PLRCTIME plt = pLrcTime;
				
				pLrcText = InsertLrcText(pLrcText, pLrcTime, pTextStart);
				//printf("Text:%s\t\n", pTextStart);
				pLrcTime = FreeLrcTime(pLrcTime);
				
				pTextStart = NULL;
			}
			break;
		default: 
			fputs("Is there anything wrong?", stderr); 
			break;
		}
	}

	pLrc->ArHead = pLrcAr;
	pLrc->LrcTextHead = pLrcText;

	return 0;
}

int CreateLrc(PLRC pLrc, char *Buffer)
{
	if(NULL == Buffer)
		return 1;

	pLrc->ArHead = NULL;
	pLrc->LrcTextHead = NULL;
	pLrc->Buffer = Buffer;
	LrcParse(pLrc);

	return 0;
}

int CreateLrcFromFile(PLRC pLrc, char *FilePath)
{
	FILE *fp = NULL;

	if((fp = fopen(FilePath, "rb")) == NULL)
	{
		fputs("File error\n", stderr);
		return 1;
	}

	fseek (fp , 0, SEEK_END);
	long lSize = ftell(fp);
	//printf("FileSize: %ld\n", lSize);
	//getchar();
	rewind (fp);

	char *buf = NULL;
	if((buf = (char*)malloc(sizeof(char) * lSize)) == NULL)
	{
		fputs("Memory error\n", stderr); 
		fclose(fp);
		return 2;
	}
	memset(buf, 0, sizeof(char) * lSize); // ²»ÄÜÉÙ¡£


	if(!(fread(buf, 1, lSize, fp)))
	{
		fputs("Reading error\n", stderr);
		fclose(fp);
		return 3;
	}

	//printf("Characters(whith bank):%ld\n", lSize);
	//char *type[] = {"BINARY", "ASCII", "UTF8_NOBOM", "UTF8", "UCS2_LE", "UCS2_BE"};
	//printf("%s", type[CheckTextEncode(buf, lSize)]);
	//getchar();

	int Type = CheckTextEncode(buf, lSize);
	switch(Type)
	{
	case UTF8:
		buf += 3;
	case UTF8_NOBOM:
		{
			char *pDst = (char *)malloc(lSize * sizeof(char));
			//memset(buf, 0, lSize * sizeof(char) + 1);
			UTF8ToASCII(buf, pDst, lSize);

			free(buf);
			buf = pDst;
		}
		break;
	case UCS2_LE:
		{
			buf += 2;
			char *pDst = UCS2LEToASCII((WCHAR*)buf);

			free(buf);
			buf = pDst;
		}
		break;
	case UCS2_BE:
		{
			buf += 2;
			char *pDst = UCS2BEToASCII((WCHAR*)buf, lSize - 2);
			
			free(buf);
			buf = pDst;
		}
		break;
	default:break;
	}
	
	if(NULL == buf)
		return -1;
	if(lSize > 0)
	{
		//printf("comment:\n%s", buf);
		//getchar();
		pLrc->Buffer = buf;
		CreateLrc(pLrc, buf);
	}
	else
	{
		printf("this is an empty file!\n");
	}

	fclose(fp);

	return 0;
}

void DestroyLrc(LRC Lrc)
{
	FreeLrcAr(Lrc.ArHead);
	FreeLrcText(Lrc.LrcTextHead);
	if(Lrc.Buffer)
		free(Lrc.Buffer);
}

void DisplayLrcAr(const PLRCAR pLrcAr)
{
	PLRCAR p = pLrcAr;

	while(NULL != p)
	{
		PLRCAR h = p;
		p = p->next;
		printf("%s, %s\n", h->key, h->value);
	}
}

void DisplayLrcText(const PLRCTEXT pLrcText)
{
	PLRCTEXT p = pLrcText;

	while(NULL != p)
	{
		PLRCTEXT h = p;
		p = p->next;
		printf("Time(ms):%ld\tTime:%ld:%.2f\t%s\n", h->time, (h->time)/60000,  (h->time)%60000 / 1000.0, h->Text);
	}
}

void DisplayLrc(const LRC Lrc)
{
	DisplayLrcAr(Lrc.ArHead);
	printf("\n\n");
	DisplayLrcText(Lrc.LrcTextHead);
}