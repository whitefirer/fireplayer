/*
 * Designer: whitefirer
 * Language: C/C++
 * Compiler: VS2005
 * Date : 20111024
 * Discription : display lrc and play the music
 * E-Mail : whitefirer@gmail.com
 */

/*
 * You can use the keys to control the player.
 */

#ifndef __LRC_SHOW_H__
#define __LRC_SHOW_H__
#include "LrcParse.h"

#define EXIT 0 // exit the program
#define	FRONT (-1) // back to the front lrc
#define	NEXT 1 // forward to the next lrc

int LrcShow(LRC Lrc, char *mp3path); // show the lrc or play the mp3 what if they exist
#endif;