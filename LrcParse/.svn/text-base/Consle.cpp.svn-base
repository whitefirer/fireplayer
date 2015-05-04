/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : some fuctions for console.
 * E-Mail : whitefirer@gmail.com
 */

#include "stdafx.h"
#include <windows.h>
#include "Consle.h"

static void console_cls() 
{// 清屏
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {
        0, 0
    };

    BOOL   bSuccess;
    DWORD  cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD  dwConSize;

    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    bSuccess = FillConsoleOutputCharacter(
        hConsole,
        ' ',
        dwConSize,
        coordScreen,
        &cCharsWritten
        );
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    bSuccess = FillConsoleOutputAttribute(
        hConsole,
        csbi.wAttributes,
        dwConSize,
        coordScreen,
        &cCharsWritten
        );
    bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
}

void clrscr(void)
{
    console_cls();
}

void gotoxy(int x, int y) 
{// 设置文本输出位置
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT)x, (SHORT)y};

    SetConsoleCursorPosition(hOut, pos);
}

void textcolor(int color) 
{// 设置文本输出颜色
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hOut, (WORD)color);
}

void textattr(int attr) 
{
    textcolor(attr);
}

short getmaxx()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	// COORD coordScreen = GetLargestConsoleWindowSize(hConsole);
   GetConsoleScreenBufferInfo(hConsole, &csbi);

	return csbi.dwSize.X;
}

short getmaxy()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//CONSOLE_SCREEN_BUFFER_INFO csbi
	COORD coordScreen = GetLargestConsoleWindowSize(hConsole);
    //GetConsoleScreenBufferInfo(hConsole, &csbi);

	return coordScreen.Y;
}