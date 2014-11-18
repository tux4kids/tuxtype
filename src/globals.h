/*
   globals.h:

   Global variables, #defines, #includes, and prototypes.

   Copyright 2000, 2003, 2007, 2008, 2009, 2010.
   Authors: Sam Hart, Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   globals.h is part of Tux Typing, a.k.a "tuxtype".

Tux Typing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tux Typing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//TTS Parameters
#define DEFAULT_VALUE 30
#define INTERRUPT 0
#define APPEND 1

#ifndef GLOBALS_H
#define GLOBALS_H

// Autoheader-derived defs in here:
#include "config.h"

// C library includes:
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>

// SDL includes:
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
/* NOTE only SDL_extras.c/.h now knows about SDL_ttf or SDL_Pango. */

// Translation stuff: 
#include "gettext.h"
#include <locale.h>
#include <iconv.h>
#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)


/* FIXME don't think this is needed (done elsewhere) */
#ifdef WIN32 
#undef DATA_PREFIX
#define DATA_PREFIX "./data"
#endif

#ifdef WIN32
#define TUXLOCALE "./locale"
#else
#define TUXLOCALE LOCALEDIR
#endif

// FIXME if we really need these, make them into functions rather than
// "evil" macros
#define to_upper(c) (((c) >= 'a' && (c) <= 'z') ? (c) -32 : (c))
#define COL2RGB( col ) SDL_MapRGB( screen->format, col->r, col->g, col->b )
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

//TTS Thread
SDL_Thread *tts_thread;

//TTS Word announcer Thread
SDL_Thread *tts_announcer_thread;

#define FNLEN	256


/* (renamed from 'settings' to match tuxmath) */
typedef struct game_option_type{
  char default_data_path[FNLEN];  // for static read-only data
  char theme_data_path[FNLEN];    // read-only data for current theme
  char var_data_path[FNLEN];      // for modifiable shared data (custom word lists, etc.)
  char user_settings_path[FNLEN];  // per-user settings (under /home)
  char global_settings_path[FNLEN]; // settings for all users (under /etc)
  char theme_name[FNLEN];
  char lang[FNLEN];
  char theme_font_name[FNLEN];
  char theme_locale_name[FNLEN];
  int use_english;
  int fullscreen;
  int sys_sound;
  int braille;
  int tts;
  int tts_volume;
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

//Declared in t4k_global.h
extern int text_to_speech_status;

struct braille_dict
{
	wchar_t key[100];
	wchar_t value_begin[100];
	wchar_t value_middle[100];
	wchar_t value_end[100];
}braille_key_value_map[100];


/* Default values for game_option_type struct */
/* They can be changed in the struct to other values at run-time */
#define DEFAULT_FONT_NAME       "AndikaDesRevG.ttf"
#define DEFAULT_GAME_FONT       "AndikaDesRevG.ttf" 
#define DEFAULT_MENU_FONT_SIZE	20
#define GAME_FONT_SIZE	20
#define DEFAULT_LOCALE	        "en_US.UTF-8"
#define DEFAULT_USE_ENGLISH 1
#define DEFAULT_FULLSCREEN 1
#define DEFAULT_BRAILLE 0
#define DEFAULT_TTS 0
#define DEFAULT_TTS_VOLUME 100
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


/* Goal is to have all global settings here */
/* FIXME get rid of as much global data as possible, esp. pointers */
extern game_option_type settings;
extern SDL_Surface* screen;
extern SDL_Event  event;

/* these will store the resolution used by the OS when we start, so we */
/* can run fullscreen at the user's preferred resolution:              */
extern int fs_res_x;
extern int fs_res_y;



/* #defines for run-time debugging output:
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




/* Various preprocessor constants: ------------------------- */


#define RES_X	640
#define RES_Y	480
#define BPP	32

/* Limits on word list size, word length, and on the number of distinct characters */
/* that can be present within a word list: */
#define MAX_NUM_WORDS   500
#define MAX_WORD_SIZE   30
#define MAX_WORD_LISTS  100
#define MAX_UNICODES    1024

#define WAIT_MS		2500
#define	FRAMES_PER_SEC	15
#define FULL_CIRCLE	140


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

#define MUSIC_FADE_OUT_MS	80

/* Menu Prototypes */
enum Game_Type { 
  QUIT_GAME, CASCADE, OPTIONS, LESSONS,
  INSTRUCT_CASCADE, CASCADE1, CASCADE2, CASCADE3, CASCADE4,
  INSTRUCT_LASER, LASER1, LASER2, LASER3, LASER4,
  PHRASE_TYPING, ASDF, ALL, MAIN, SET_LANGUAGE, PROJECT_INFO, NOT_CODED,
  LEVEL1, LEVEL2, LEVEL3, LEVEL4, LASER, INSTRUCT, EDIT_WORDLIST, NONE};

//Game difficulty levels
enum { EASY, MEDIUM, HARD, INSANE, INF_PRACT };
#define NUM_LEVELS  4

enum 
{
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


/* For TransWipe(): */
enum
{
  WIPE_BLINDS_VERT,
  WIPE_BLINDS_HORIZ,
  WIPE_BLINDS_BOX,
  RANDOM_WIPE,
  NUM_WIPES
};

#endif

