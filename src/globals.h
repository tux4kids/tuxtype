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

#define FNLEN	256

#define RES_X	640
#define RES_Y	480
#define BPP	32

#ifdef WIN32 
#undef DATA_PREFIX
#define DATA_PREFIX "./data"
#endif

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define MAX_SPRITE_FRAMES 30


#include "config.h"

// Translation stuff (now works for Mac and Win too!): 
#include "gettext.h"
#include <locale.h>
#include <iconv.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

#ifdef WIN32
#define TUXLOCALE "./locale"
#else
#define TUXLOCALE LOCALEDIR
#endif

#include <string.h>
#include <wchar.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
//#include <SDL/SDL_mixer.h>
//#include <SDL/SDL_ttf.h>

//#include "i18n.h"
//#include <libintl.h>
//#define _(String) gettext(String)
//#define gettext_noop(String) (String)



#endif //  __GLOBALS_H__

/* FIXME get rid of these 'evil' macros */
#define NEXT_FRAME(SPRITE) if ((SPRITE)->num_frames) (SPRITE)->cur = (((SPRITE)->cur)+1) % (SPRITE)->num_frames;
#define REWIND(SPRITE) (SPRITE)->cur = 0;

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

/* Goal is to have all global settings here */
/* (renamed from 'settings' to match tuxmath) */
typedef struct game_option_type{
  char default_data_path[FNLEN];
  char theme_data_path[FNLEN];
  char user_settings_path[FNLEN];
  char global_settings_path[FNLEN];
  char theme_name[FNLEN];
  char lang[FNLEN];
  char theme_font_name[FNLEN];
  char theme_locale_name[FNLEN];
  int use_english;
  int fullscreen;
  int sys_sound;
  int sfx_volume;
  int mus_volume;
  int menu_music;
  int menu_sound;
  int speed_up;
  int show_tux4kids;
  int debug_on;
  int o_lives;
  int sound_vol;
  int hidden; // Read the README file in the image directory for info on this ;)
} game_option_type;

/* Default values for game_option_type struct */
/* They can be changed in the struct to other values at run-time */
#define DEFAULT_FONT_NAME       "AndikaDesRevG.ttf"
#define DEFAULT_GAME_FONT       "AndikaDesRevG.ttf" 
#define DEFAULT_MENU_FONT_SIZE	20
#define GAME_FONT_SIZE	20
#define DEFAULT_LOCALE	        "en_US.UTF-8"
#define DEFAULT_USE_ENGLISH 1
#define DEFAULT_FULLSCREEN 1
#define DEFAULT_SYS_SOUND 1
#define DEFAULT_SFX_VOLUME 100
#define DEFAULT_MUS_VOLUME 100
#define DEFAULT_MENU_MUSIC 1
#define DEFAULT_MENU_SOUND 1
#define DEFAULT_SPEED_UP 0
#define DEFAULT_SHOW_TUX4KIDS 1
#define DEFAULT_DEBUG_ON 0
#define DEFAULT_O_LIVES 0
#define DEFAULT_SOUND_VOL 100
#define DEFAULT_HIDDEN 0



typedef struct {
  SDL_Surface* frame[MAX_SPRITE_FRAMES];
  SDL_Surface* default_img;
  int num_frames;
  int cur;
} sprite;



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

#define LOG( str ) if (settings.debug_on) fprintf( stderr, str );
#define DEBUGCODE if (settings.debug_on) 
#define DOUT(x) if (settings.debug_on) fprintf(stderr, "%s = %d\n", #x, x);

// #if SDL_BYTEORDER == SDL_BIG_ENDIAN
// #define RMASK 0xff000000
// #define GMASK 0x00ff0000
// #define BMASK 0x0000ff00
// #define AMASK 0x000000ff
// #else
// #define RMASK 0x000000ff
// #define GMASK 0x0000ff00
// #define BMASK 0x00ff0000
// #define AMASK 0xff000000
// #endif




/* Limits on word list size, word length, and on the number of distinct characters */
/* that can be present within a word list: */
#define MAX_NUM_WORDS   500
#define MAX_WORD_SIZE   8
#define MAX_UNICODES    1024

#define WAIT_MS		2500
#define	FRAMES_PER_SEC	15
#define FULL_CIRCLE	140

/* Menu Prototypes */
enum Game_Type { 
  QUIT_GAME, CASCADE, OPTIONS, LESSONS,
  INSTRUCT_CASCADE, CASCADE1, CASCADE2, CASCADE3, CASCADE4,
  INSTRUCT_LASER, LASER1, LASER2, LASER3, LASER4,
  PHRASE_TYPING, ASDF, ALL, MAIN, SET_LANGUAGE, PROJECT_INFO, NOT_CODED,
  LEVEL1, LEVEL2, LEVEL3, LEVEL4, LASER, INSTRUCT, EDIT_WORDLIST, NONE};

/* Title sequence constants */
#define PRE_ANIM_FRAMES	 10
#define PRE_FRAME_MULT	 3
#define MENU_SEP	 20


#define IMG_REGULAR  0x01
#define IMG_COLORKEY 0x02
#define IMG_ALPHA    0x04
#define IMG_MODES    0x07

#define IMG_NOT_REQUIRED 0x10
#define IMG_NO_THEME     0x20

/* Values for menu button drawing: */
#define REG_RGBA 16,16,96,96
#define SEL_RGBA 16,16,128,128

//Game difficulty levels
enum { EASY, MEDIUM, HARD, INSANE, INF_PRACT };
#define NUM_LEVELS  4

extern game_option_type settings;

/* FIXME get rid of as much global data as possible, esp. pointers */

extern SDL_Surface* screen;

/* these will store the resolution used by the OS when we start, so we */
/* can run fullscreen at the user's preferred resolution:              */
extern int fs_res_x;
extern int fs_res_y;

//extern TTF_Font* font;
extern SDL_Event  event;

extern SDL_Color black;
extern SDL_Color gray;
extern SDL_Color dark_blue;
extern SDL_Color red;
extern SDL_Color white;
extern SDL_Color yellow;

extern SDL_Surface* letters[255]; /* Will be going away */

/* These need some work to support Unicode & i18n: */
extern wchar_t ALPHABET[256];
extern int ALPHABET_SIZE;


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

#define MUSIC_FADE_OUT_MS	80

enum {
    WIPE_BLINDS_VERT,
    WIPE_BLINDS_HORIZ,
    WIPE_BLINDS_BOX,
    RANDOM_WIPE,

    NUM_WIPES
};


