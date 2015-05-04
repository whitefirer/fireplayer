/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : display lrc and play the music
 * E-Mail : whitefirer@gmail.com
 */

#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include "LrcParse.h"
#include "LrcShow.h"
#include "Consle.h"
#include "Matrix.h"
//#include <AFXCOM_.H> 
//#pragma comment(lib,"winmm.lib") 
#include <vfw.h>
#pragma comment(lib,"vfw32.lib")

void clearline(int line)
{
	gotoxy(0, line);
	for(int i = 0; i < getmaxx(); i++)
		putchar(' ');
}

int LrcShow(LRC Lrc, char *mp3path)
{
	PLRCTEXT pLrcText = Lrc.LrcTextHead;
	PLRCTEXT h = pLrcText;
	PLRCTEXT ph = h;
	HWND hAudio = NULL;
	FILE *fp = NULL;
	static int Au = 500;
	char Playmp3Flag = 0;
	char PauseFlag = 0;
	char KeyDownFlag = 1;
	char TempFlag = 0;
	long Mp3Lenth = 1;
	float Mp3Sec = 0;
	int Mp3Min = 0;
	short W = getmaxx();
	short H = 19;
	//long count = 0;
	COORD MousePos = {0, 0};

	SetConsoleTitle("LrcParse - By whitefirer");
	clrscr();
	DWORD oldmode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&oldmode);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), (oldmode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_MOUSE_INPUT);// 临时改成非快速编辑模式
	//DisplayLrcAr(Lrc.ArHead); 
	MCIWndRegisterClass();
	if(fp = fopen(mp3path, "rb"))
	{
		hAudio = MCIWndCreate(GetDesktopWindow(), NULL, MCIWNDOPENF_NEW | MCIWNDF_NOMENU, mp3path);
		fclose(fp);
	}
	
	if(hAudio != NULL)
	{
		ShowWindow(hAudio, SW_HIDE);
		if(!MCIWndPlay(hAudio))
		{
			MCIWndSetVolume(hAudio, Au);
			Mp3Lenth = MCIWndGetLength(hAudio);
			Mp3Min = Mp3Lenth / 60000.0;
			Mp3Sec = Mp3Lenth % 60000 / 1000.0;
			Playmp3Flag = 1;
		}
	}
	else
	{
		//printf("Can not play this mp3!But you can play the lrc.\n");
		//getchar();
		Playmp3Flag = 0;
	}

	time_t t = 0;
	time_t tt = t;
	if(Playmp3Flag)
	{
		tt = MCIWndGetPosition(hAudio);
	}
	else
	{
		t = GetTickCount();
	}
	if(pLrcText)
	{
		textcolor(LIGHTCYAN);
		gotoxy(W / 2 - strlen(pLrcText->Text) / 2, H);
		printf("%s", pLrcText->Text);
	}
	else if(!Playmp3Flag)
	{
		printf("Not a lrc file\n");
		Sleep(1000);
		return NEXT;
	}
	InitMatrix();
	while(1)
	{
		int i = 0;
		//delay_fps(60);
		//count++;

		while((pLrcText ? (pLrcText->time > (tt - t)) : 1) || (NULL == Lrc.Buffer))
		{
			if(Playmp3Flag)
			{
				tt = MCIWndGetPosition(hAudio);
			}
			else
			{
				tt = GetTickCount();
			}

			INPUT_RECORD InputRec;
			unsigned long EventNumber = 0;
			PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
			if(EventNumber)
			{
				switch(InputRec.EventType)
				{
				case KEY_EVENT:
					KeyDownFlag = 1;
					switch(InputRec.Event.KeyEvent.wVirtualKeyCode)
					{
					case VK_ESCAPE:
						{
							if(InputRec.Event.KeyEvent.bKeyDown)
							{
								if(Playmp3Flag)
									MCIWndDestroy(hAudio);
								ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
								// ReadConsoleInput是不能省的
								return EXIT;	
							}
						}
						break;
					if(!Playmp3Flag)
						break;
					case VK_UP:
						{	
							if(InputRec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED)
							{
								MCIWndPlayFromTo(hAudio, MCIWndGetStart(hAudio), MCIWndGetEnd(hAudio));
								PauseFlag = 0;
							}
							else //if(InputRec.Event.KeyEvent.bKeyDown)
							{
								Au = MCIWndGetVolume(hAudio) + 20;

								if(Au > 1000)
									Au = 1000;
								MCIWndSetVolume(hAudio, Au);
								PauseFlag = 0;
							}
						}
						break;
					case VK_DOWN:
						{
							if(InputRec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED)
							{
								TempFlag = 1;
								//KeyDownFlag = 0;
								//H = 18;
								//g_height = 22;
							}
							else //if(InputRec.Event.KeyEvent.bKeyDown)
							{
								Au = MCIWndGetVolume(hAudio) - 20;

								if(Au < 0)
									Au = 0;
								MCIWndSetVolume(hAudio, Au);
								PauseFlag = 0;
							}
						}
						break;
					case VK_LEFT:
						{
							if(InputRec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED)
							{
								long mpft = tt - 1000;

								if(mpft > Mp3Lenth)
									mpft = Mp3Lenth;
								MCIWndPlayFromTo(hAudio, mpft, Mp3Lenth);
								pLrcText = Lrc.LrcTextHead;
								h = pLrcText;
								PauseFlag = 0;
							}
							else if(InputRec.Event.KeyEvent.bKeyDown)
							{
								if(Playmp3Flag && hAudio)
									MCIWndDestroy(hAudio);
								ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
								return FRONT;
							}
						}
						break;
					case VK_RIGHT:
						{
							if(InputRec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED)
							{
								long mpft = tt + 1000;

								if(mpft < 1)
									mpft = 1;
								MCIWndPlayFromTo(hAudio, mpft, Mp3Lenth);	
								PauseFlag = 0;
							}
							else if(InputRec.Event.KeyEvent.bKeyDown)
							{
								if(Playmp3Flag)
									MCIWndDestroy(hAudio);
								ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
								return NEXT;
							}
						}
						break;
					case VK_SPACE:
						{
							if(InputRec.Event.KeyEvent.bKeyDown)
							{
								PauseFlag = (PauseFlag + 1) % 2;
							}
							break;
						}
					}
					break;
				case MOUSE_EVENT:
					{
						KeyDownFlag = 1;
						MousePos = InputRec.Event.MouseEvent.dwMousePosition;
						GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&oldmode);

						switch(InputRec.Event.MouseEvent.dwButtonState)
						{
							case FROM_LEFT_1ST_BUTTON_PRESSED:
								if((MousePos.Y == 21) && (MousePos.X > 43) && (MousePos.X < 65))
								{
									Au = (MousePos.X - 44) * 50;
									MCIWndSetVolume(hAudio, Au);
								}
								else if((MousePos.Y == 22) && (MousePos.X > 14) && (MousePos.X < 65))
								{
									long mpft = Mp3Lenth * ( MousePos.X - 15) / 50;
									MCIWndPlayFromTo(hAudio, mpft, Mp3Lenth);	
									PauseFlag = 0;
									pLrcText = Lrc.LrcTextHead;
								}
								else if((MousePos.Y > 20) && (MousePos.X < 21))
								{
									if(Playmp3Flag && hAudio)
										MCIWndDestroy(hAudio);
									ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
									return FRONT;
								}
								else if((MousePos.Y > 20) && (MousePos.X > 69))
								{
									if(Playmp3Flag && hAudio)
										MCIWndDestroy(hAudio);
									ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
									return NEXT;
								}
								else if(MousePos.Y < (H - 2))
								{
									PauseFlag = (PauseFlag + 1) % 2;
								}
								//else
								//{
								//	gotoxy(MousePos.X, MousePos.Y);
								//	putc('.', stdout);
								//}
							break;
						}

						
					}
					break;
				}

				if(1 == PauseFlag)
				{
					gotoxy(0, 0);
					printf("(%2d, %2d) %d 暂停，按空格键播放", MousePos.X, MousePos.Y, oldmode);
					MCIWndPause(hAudio);
				}
				else
				{
					gotoxy(0, 0);
					printf("(%2d, %2d) %d                   ", MousePos.X, MousePos.Y, oldmode);
					MCIWndResume(hAudio);
				}
				ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRec, 1, &EventNumber);
			}

			if((tt % 10) && KeyDownFlag)
			{
				textcolor(LIGHTGREEN);
				
				gotoxy(0, 21);
				printf("\tTime: %ld:", (tt - t) / 60000);
				printf("%.2f/%ld:%.2f", ((tt - t) % 60000) / 1000.0, Mp3Min, Mp3Sec);

				//if(Playmp3Flag)
				{
					printf("%s", "\t     音量: [");
					int GetAu = MCIWndGetVolume(hAudio);

					for(int i = 0; i < 21; i++)
					{
						if(i < GetAu / 50)
							putchar('=');

						else if(i == GetAu / 50)
							putchar('O');
						else putchar('-');
					}

					int Pos = (tt * 100) / Mp3Lenth;
					if(Pos < 0)
						Pos = 0;
					if(MousePos.Y > (H + 3))
						printf("]%d%%  \n<上一首 进度: [", GetAu / 10);
					else
						printf("]%d%%  \n\t进度: [", GetAu / 10);
					for(int i = 0; i < 50; i++)
					{
						if(i < Pos / 2)
							putchar('=');
						else if(i == Pos / 2)
							putchar('O');
						else putchar('-');
					}
					if(MousePos.Y > (H + 3))
						printf("]%d%% \t下一首>\n\tfireplayer111024 By: whitefirer E-mail: whitefirer@gmail.com  ", Pos);
					else 
						printf("]%d%%          \n\tfireplayer111024 By: whitefirer E-mail: whitefirer@gmail.com  ", Pos);
				}
			}
			//DrawMatrix(tt%8);
			//DrawMatrix(get_highfeq_time_ls()%10);
			if(0 == PauseFlag)
				DrawMatrix(GetTickCount()%10);
			else
				Sleep(getmaxx());
			long texttime = 0; 
			if(h)
			{
				if(pLrcText)
					texttime = (pLrcText->time - h->time) / (float)strlen(h->Text);
				else 
					texttime = (Mp3Lenth - h->time) / (float)strlen(h->Text);
				if((Mp3Lenth == 1) && !pLrcText)
					texttime = 200;

				if((tt - t) >= (h->time + texttime * i))
				{
					textcolor(YELLOW);
					
					if((unsigned char)*(h->Text + i) > 0x80) // 显示汉字(双字节)
					{
						gotoxy(W / 2 - strlen(h->Text) / 2 + i, H);
						//printf("%c%c", *(h->Text + i), *(h->Text + i + 1));
						putchar((unsigned char)*(h->Text + i));
						putchar((unsigned char)*(h->Text + i + 1));
						i = (i + 2) % (strlen(h->Text) + 1);
					}
					else // ASCII码
					{
						gotoxy(W / 2 - strlen(h->Text) / 2 + i, H);
						putchar(*(h->Text + i));
						i = (i + 1) % (strlen(h->Text) + 1);
					}
					//gotoxy(W - 1, H / 2 + 1);
				}
			}
			if(!TempFlag && (tt!= 0) && !(tt % 6000))
			{
				TempFlag = 1;
				clearline(21);
				clearline(22);
				clearline(23);
			}
			//else
			//{
				//clearline(17);
				//clearline(18);
				//clearline(19);
			//}
			//if(!h && !Playmp3Flag)
				//return EXIT;
			if(((Mp3Lenth == 1) && (NULL == pLrcText)) && ((NULL == h) && (NULL != h->Text) && (i == strlen(h->Text))))
				return NEXT;
			if((Playmp3Flag && (MCIWndGetPosition(hAudio) >= (Mp3Lenth - 1))))
				break;
		}
		clrscr();
		if(1 == KeyDownFlag)
		{
			H = 17;
			seth(16);
		}
		if(TempFlag)
		{
			KeyDownFlag = 0;
			TempFlag = 0;
		}

		if(!KeyDownFlag)
		{	
			H = 22;
			seth(19);
			
		}
		if(h != pLrcText)
		{
			textcolor(CYAN);
			gotoxy(W / 2 - strlen(h->Text) / 2, H - 1);
			printf("%s", h->Text);
		}
		
		if(pLrcText)
		{
			textcolor(LIGHTCYAN);
			gotoxy(W / 2 - strlen(pLrcText->Text) / 2, H);
			printf("%s", pLrcText->Text);
			
			if(pLrcText->next)
			{
				textcolor(CYAN);
				gotoxy(W / 2 - strlen(pLrcText->next->Text) / 2, H + 1);
				printf("%s", pLrcText->next->Text);
			}
			SetConsoleTitle(pLrcText->Text);

			ph = h;
			h = pLrcText;
			pLrcText = pLrcText->next;
		}
		if((Playmp3Flag && hAudio && (MCIWndGetPosition(hAudio) >= (Mp3Lenth - 1))))
			break;
	}
	while(Playmp3Flag && hAudio && (MCIWndGetPosition(hAudio) < Mp3Lenth))
	{
		Sleep(1);
	}
	if(hAudio)
		MCIWndDestroy(hAudio);

	return NEXT;
}
