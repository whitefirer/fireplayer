/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : Draw Matrix(char rain) on the consle.
 * E-Mail : whitefirer@gmail.com
 */

#include "stdafx.h"
#include "Matrix.h"
#include "Consle.h"
#include <time.h>
#include <stdlib.h>
#include <windows.h>

int g_width = getmaxx();
int g_height = 20;

int g_w = getmaxx(), g_h = 14;
int g_cw, g_ch;

struct chars 
{
    int x;
    int y;
    char c;
};

struct charrain 
{
    struct chars clist[300];
    int nchar;
}g_cr;

void InitMatrix() 
{
    int i;

   srand(GetTickCount());
    g_cw = g_width / g_w, g_ch = g_height / g_h;
    g_cr.nchar = g_w / 10;
    for(i = 0; i < g_cr.nchar; ++i) 
	{
        g_cr.clist[i].x = rand() % g_w;
        g_cr.clist[i].y = rand() % g_h;
    }
}

void DrawMatrix(int update) 
{
    int i, j;
	int h = g_h;
	static int lenth =  10;
	static int newflag = 0;
	//int k = rand() % g_cr.nchar + 1;

    for(i = 0; i < g_cr.nchar - 1; ++i) 
	{	
		update = i % (rand() % g_cr.nchar + 1);

        if(!update) 
		{
			if((g_cr.clist[i].x < 16) || (g_cr.clist[i].x > 60))
				h = g_h;
			else
			{
				h = g_h - 5;
			}
			int ys = g_cr.clist[i].y - lenth;
			if(ys < 0) 
				ys = 0;
			gotoxy(g_cr.clist[i].x, ys);
			putchar(' ');
			g_cr.clist[i + 2].c = g_cr.clist[i + 1].c;
            g_cr.clist[i + 1].c = g_cr.clist[i].c;
            g_cr.clist[i].c = rand() % (128 - 34) + 33;
			
			textcolor(GREEN);
            gotoxy(g_cr.clist[i].x, g_cr.clist[i].y);
			putchar(g_cr.clist[i].c);

			g_cr.clist[i].y++;

            if(g_cr.clist[i].y - rand() % 4 > h) 
			{
				for(j = lenth; j >= 0; --j)
				{
					int y = g_cr.clist[i].y - j;
					if(y < 0) 
						y = 0;
					gotoxy(g_cr.clist[i].x, y);
					putchar(' ');
				}
                g_cr.clist[i].x = rand() % g_w;
                g_cr.clist[i].y = rand() % 5;
				lenth = h + rand() % 5;
				
				newflag = 1;
            }
       

			textcolor(LIGHTGREEN);
			gotoxy(g_cr.clist[i].x, g_cr.clist[i].y);
			putchar(g_cr.clist[i+1].c);
			Sleep(10);
			textcolor(WHITE);
			gotoxy(g_cr.clist[i].x, g_cr.clist[i].y+1);
			putchar(g_cr.clist[i+2].c);		
		}
		
		//gotoxy(0, 0);
		//printf("%lld", get_highfeq_time_ls());
		Sleep(1);
		//Sleep(1);
    }
}

void seth(int h)
{
	g_h = h;
}