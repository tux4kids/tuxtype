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

#define to_upper(c) (((c) >= 'a' && (c) <= 'z') ? (c) -32 : (c))
#define COL2RGB( col ) SDL_MapRGB( screen->format, col->r, col->g, col->b )

#define FNLEN	200

#define RES_X	640
#define RES_Y	480
#define BPP	16	

#ifdef WIN32 
#undef DATA_PREFIX
#define DATA_PREFIX ".//"
#endif

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define MAX_SPRITE_FRAMES 30

#include <string.h>
#include <wchar.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>

#include "../config.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#endif //  __GLOBALS_H__

#define next_frame(SPRITE) if ((SPRITE)->num_frames) (SPRITE)->cur = (((SPRITE)->cur)+1) % (SPRITE)->num_frames;
#define rewind(SPRITE) (SPRITE)->cur = 0;

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

typedef struct {
    char lang[FNLEN];
    char path[FNLEN];
    char window[FNLEN];
    int sfx_volume;
    int mus_volume;
    int menu_music;
} settings;

typedef struct {
	SDL_Surface *frame[MAX_SPRITE_FRAMES];
	SDL_Surface *default_img;
	int num_frames;
	int cur;
} sprite;

#define _(str) gettext (str)
#define gettext_noop(str) (str)

/* LOGGING works as such:
 *
 * - Use LOG if you want to output a string LOG( "Hello World");
 *   
 * - Use DOUT if you want to output a value of a variable and the
 *   name of the variable gives enough context:
 *   DOUT( specialCode );  would add to stderr: "specialCode = 1\n" or
 *   whatever value specialCode had
 *   
 * - Use DEBUGCODE if you need to do something more complicated like
 *   DEBUGCODE { fprintf(stderr, "examining letter %d\n", x); }
 *   since DOUT(x) "x = 1\n" gives little information since x is used
 *   all over the place!
 */

#define LOG( str ) if (debugOn) fprintf( stderr, str );
#define DEBUGCODE if (debugOn) 
#define DOUT(x) if (debugOn) fprintf(stderr, "%s = %d\n", #x, x);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif

#define menu_font	"AndikaDesRevG.ttf"    /*"AndikaDesRevA.ttf"  GenAI102.ttf or "DejaVuSans-Bold.ttf"  or "FreeSansBold.ttf" */ 	/* was menu.ttf */
#define menu_font_size	20

#define ttf_font	"AndikaDesRevG.ttf" /*AndikaDesRevA.ttf"  GenAI102.ttf or "DejaVuSans-Bold.ttf" or "FreeSansBold.ttf" */  	/* was letters.ttf */
#define ttf_font_size	20

/* Limits on word list size, word length, and on the number of distinct characters */
/* that can be present within a word list: */
#define MAX_NUM_WORDS   500
#define MAX_WORD_SIZE   8
#define MAX_UNICODES 1024

#define WAIT_MS				2500
#define	FRAMES_PER_SEC	                50
#define FULL_CIRCLE		        140

/* Menu Prototypes */
enum Game_Type { 
	QUIT_GAME, CASCADE, OPTIONS, LESSONS,
	INSTRUCT_CASCADE, CASCADE1, CASCADE2, CASCADE3, CASCADE4,
	INSTRUCT_LASER,    LASER1,    LASER2,    LASER3,    LASER4,
	FREETYPE, ASDF, ALL, MAIN, SET_LANGUAGE, PROJECT_INFO, EDIT_WORDLIST,
	LEVEL1, LEVEL2, LEVEL3, LEVEL4, LASER, INSTRUCT, NOT_CODED, NONE};

/* Title sequence constants */
#define PRE_ANIM_FRAMES			10
#define PRE_FRAME_MULT			3
#define MENU_SEP			20

/* paths */

#define IMG_REGULAR  0x01
#define IMG_COLORKEY 0x02
#define IMG_ALPHA    0x04
#define IMG_MODES    0x07

#define IMG_NOT_REQUIRED 0x10
#define IMG_NO_THEME     0x20

//Game difficulty levels
enum { EASY, MEDIUM, HARD, INSANE, INF_PRACT };
#define NUM_LEVELS		        4

extern SDL_Surface *screen;
extern TTF_Font  *font;
extern SDL_Event  event;

extern SDL_Color black;
extern SDL_Color gray;
extern SDL_Color dark_blue;
extern SDL_Color red;
extern SDL_Color white;
extern SDL_Color yellow;

extern SDL_Surface *bkg;
extern SDL_Surface *letters[255];

extern unsigned char ALPHABET[256];
extern unsigned char KEYMAP[256];
extern unsigned char FINGER[256][10];
extern int ALPHABET_SIZE;
extern wchar_t unicode_chars_used[MAX_UNICODES];

//global vars
extern int speed_up;
extern int show_tux4kids;
extern int debugOn;
extern int o_lives;
extern int sound_vol;
extern int hidden; // Read the README file in the image directory for info on this ;)

/* Alternative language/word/image/sound theming */
extern unsigned char realPath[2][FNLEN];
extern char themeName[FNLEN];
extern int useEnglish;

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

extern Mix_Chunk      *sound[NUM_WAVES];
extern Mix_Music      *music;
extern int sys_sound;

#define MUSIC_FADE_OUT_MS	80

enum {
    WIPE_BLINDS_VERT,
    WIPE_BLINDS_HORIZ,
    WIPE_BLINDS_BOX,
    RANDOM_WIPE,

    NUM_WIPES
};


/* An individual item in the list of cached unicode characters that are rendered at   */
/* the start of each game. The 'max_y' is stored so that the characters can be lined  */
/* up properly even if their heights are different.                                   */
typedef struct uni_glyph {
  wchar_t unicode_value;
  SDL_Surface* white_glyph;
  SDL_Surface* red_glyph;
  int max_y;
} uni_glyph;
