/***************************************************************************
                          globals.h
  description: global varis
                             -------------------
    begin                : Fri May 5 2000
    copyright            : (C) 2000 by Sam Hart
    email                : hart@geekcomix.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#define YES		100
#define NO		200

#define WAIT_MS				2500
#define	FRAMES_PER_SEC	                50
#define FULL_CIRCLE		        140
#define PAUSE_IMAGES	                2

/* Menu Prototypes */
#define DONE				0
#define KCASCADE			1
#define WCASCADE			2
#define PRACTICE			3

#define KCASCADE1			4
#define KCASCADE2			5
#define KCASCADE3			6

#define WCASCADE1			7
#define WCASCADE2			8
#define WCASCADE3			9

#define PKCASCADE			10
#define PWCASCADE			11
#define PFREETYPE			12

#define MAIN				13
#define LEVEL1				14
#define LEVEL2				15
#define LEVEL3				16

#define PRACTICE_SUBMENU	        3
#define KCASCADE_SUBMENU	        2
#define WCASCADE_SUBMENU	        2
#define ROOTMENU		        1

/* Title sequence constants */
#define MAX_TUX_ANIM_FRAMES		4
#define TITLE_MENU_ITEMS		4
#define TITLE_MENU_DEPTH		3
#define PRE_ANIM_FRAMES			10
#define PRE_FRAME_MULT			3
#define SLING_FRAMES			10
#define XSTEP				2
#define YSTEP				2
#define TITLE_MULT			2.5
#define MENU_GYRO_AMP			8
#define MENU_SEP			10
#define MENU_GYRO_MULT			.5
#define MENU_START			10

//actually, six frames of animation, first is stationary
//longest, then cycles up and down other six
#define TUX1				105
#define TUX2				110
#define TUX3				115
#define TUX4				120
#define TUX5				125
#define TUX6				130

//Title screen constants
#define BACK_MULT			10
#define BACK_BPP			8

/* paths */
#define FNLEN 	200
#define PATHNUM 3

//Game difficulty levels
#define EASY				0
#define MEDIUM				1
#define HARD				2
#define INF_PRACT                       3
#define NUM_LEVELS		        4

SDL_Surface *screen;
SDL_Event event;

//global varis
int sys_sound;
int speed_up;
int scroll;
int use_alpha;

int hidden; // Read the README file in the image directory for info on this ;)

/* Alternative language/word/image/sound theming */
char alt_theme_path[FNLEN];
int use_theme;

//Sound FX and Music datatypes
extern Mix_Music *music;

enum {
	WIN_WAV,
	BITE_WAV,
	LOSE_WAV,
	RUN_WAV,
	SPLAT_WAV,
	WINFINAL_WAV,
	EXCUSEME_WAV,
	PAUSE_WAV,
	NUM_WAVES
};

extern Mix_Chunk *sound[NUM_WAVES];

#define MUSIC_FADE_OUT_MS	80
#define FADE_IN_TITLE_MUSIC 1000
#endif // __GLOBALS_H__