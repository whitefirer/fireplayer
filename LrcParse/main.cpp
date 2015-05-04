/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : main pf this project.
 * E-Mail : whitefirer@gmail.com
 */

#include "stdafx.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <windows.h>
#include "LrcParse.h"
#include "LrcShow.h"
#include <stdarg.h>

int g_argc = 0;
char **g_argv = NULL;

static DWORD WINAPI Play(LPVOID lpVoid)
{
	int result = 0;
	int argc = g_argc;
	char **argv = g_argv;

	for(int i = 1; i < argc; i++)
	{
		time_t t = 0;
		LRC Lrc = {NULL, NULL, NULL};

		char Drive[255], Dir[255], FileName[526], Ext[255];

		_splitpath(argv[i], Drive, Dir, FileName, Ext);// 并不是很好
		//printf("argv[%d]:%s\n", g_i, argv[g_i]);
			
		for(int k = 0; ('\0' != Ext[k]) && (k < 255); k++)
		{
			Ext[k] = tolower(Ext[k]);
		}

		if(strcmp(".lrc", Ext))
		{
			printf("this is not a lrc file.\n");
			if(strcmp(".mp3", Ext))
			{
				printf("this is not a mp3 file.\n");
				continue;
			}
			else
			{
				char lrcpath[255];
				_makepath(lrcpath, Drive, Dir, FileName, ".lrc");
				if(CreateLrcFromFile(&Lrc, lrcpath))
				{
					printf("can't find lrc file.\n");
					Lrc.LrcTextHead = NULL;
					Lrc.ArHead = NULL;
					Lrc.Buffer = NULL;
				}
				result = LrcShow(Lrc, argv[i]);
				DestroyLrc(Lrc);
			}
		}
			
		else
		{
			t = GetTickCount();
			if(CreateLrcFromFile(&Lrc, argv[i]))
			{
				printf("can't create lrc from this file.\n");
				getchar();
				continue;
			}
			t = GetTickCount() - t;

			//DisplayLrc(Lrc);
			//printf("%fs\nPress any key to play the lrc", t / 1000.0);
			//getchar();

			char mp3path[255];
			_makepath(mp3path, Drive, Dir, FileName, ".mp3");
			//puts(mp3path);

			result = LrcShow(Lrc, mp3path);
			DestroyLrc(Lrc);
		}
		if(EXIT == result)
		{
			break;
		}
		else if(NEXT == result)
		{
			if(i >= (argc - 1))
				i = 0;
			continue;
		}
		else if(FRONT == result)
		{
			i -= 2;
			if(i <= 1)
			{
				i = argc - 2;
			}
		}
	}
	return 0;
}
int main(int argc, char* argv[])
{
	HANDLE hThread = INVALID_HANDLE_VALUE;

	if(argc == 1)
	{
		printf("no input file!\n");
		getchar();
		return 0;
	}
	g_argc = argc;
	g_argv = argv;	

	hThread = CreateThread(NULL, 0,	Play ,NULL, CREATE_SUSPENDED, NULL);
	if(hThread == NULL)
	{
		printf("error!\n");
		return 0;
	}
	//SetThreadPriority(GetCurrentThread(), 0);
	SetThreadAffinityMask(hThread, 0x00000001);    //CPU1 指定线程只在CPU1上执行

	ResumeThread(hThread);

	DWORD dw = WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	//getchar();

	return 0;
}