/*
   playgame.h:

   
   Typedefs and #defines used in playgame.c

   Copyright 2000, 2007, 2008, 2009, 2010.
   Authors: Sam Hart, Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   playgame.h is part of Tux Typing, a.k.a "tuxtype".

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


//NOTE: it looks like all this stuff is only used in playgame.c, so
//why do we have it in a header file??



#ifndef __PLAYGAME_H__
#define __PLAYGAME_H__

#include "config.h"
#include "SDL_extras.h"

//definitions
#define FISHY_FONT_SIZE 32
#define LABEL_FONT_SIZE 20
//space between each status bar element in cascade
#define GRAPHIC_SPACE 4

#define NUM_NUMS 10	//"Mmmmm....Num-Nums...."

#define CONGRATS_FRAMES	2	//number of frames for congrats animation
#define OH_NO_FRAMES 1	//number of frames for "oh no!" losing animation
#define MAX_END_FRAMES_BETWEEN_LEVELS 350	//number of frames between levels
#define MAX_END_FRAMES_WIN_GAME 750	//number of frames at ending animation
#define MAX_END_FRAMES_GAMEOVER 400	//number of frames at game over
#define END_FRAME_DY 2	//rise speed of ending animation text
#define WIN_GAME_XAMP 4	//the xamplitide of winning game text circular motion
#define WIN_GAME_YAMP 4	//the yamplitude of winning game text circular motion
#define WIN_GAME_ANGLE_MULT 4	//the angle multiplier for winning game text circ motion

//tux frame lengths (number of frames each cell of animation frames on-screen)
#define WALK_ONSCREEN	10
#define RUN_ONSCREEN	8
#define STAND_ONSCREEN	15
#define GULP_ONSCREEN	10
//tux default walking & running speeds
#define WALKING_SPEED	5
//#define WALKING_SPEED         1
#define MIN_RUNNING_SPEED		4 * WALKING_SPEED
#define MAX_RUNNING_SPEED		10 * WALKING_SPEED
/*-- These last two are for future features --*/
#define WIN_ONSCREEN		1
#define YIPE_ONSCREEN		2

//fishy defs
#define MAX_FISHIES_EASY	15
#define MAX_FISHIES_MEDIUM	30
#define MAX_FISHIES_HARD	40
#define MAX_FISHIES_DIGITS	2

#define FISHY_ONSCREEN	10
#define SPLAT_FRAMES	1
#define SPLAT_ONSCREEN	4
#define DEFAULT_SPEED	1

#define EASY_SPEED_FACTOR       1.3
#define MEDIUM_SPEED_FACTOR     3
#define HARD_SPEED_FACTOR       5

#define FISH_SPACING_EASY    90
#define FISH_SPACING_MEDIUM  60
#define FISH_SPACING_HARD    30

#define LIVES_INIT_EASY      5
#define LIVES_INIT_MEDIUM    7
#define LIVES_INIT_HARD      9


/* The default alpha for slow moving words,
 note that 128 is optimized for speed! */
#define DEFAULT_ALPHA 128

typedef struct {
    int frames;			//number of frames to display this object
    SDL_Surface *image;
} object;			//normal (non-RL) object

struct fishypoo {
    int alive;
    int can_eat;
    wchar_t* word;
    double x, y;
    int    w;
    size_t len;
    int    splat_time;
    double dy;
} fish_object[MAX_FISHIES_HARD + 1];

struct fishypoo null_fishy;

enum {
	TUX_WALKING,
	TUX_STANDING,
	TUX_RUNNING,
	TUX_GULPING,
	TUX_WINNING,
	TUX_YIPING,
	TUX_NUM_STATES
};

static char* tux_sprite_fns[TUX_NUM_STATES] = {
	"tux/walk",
	"tux/stand",
	"tux/run",
	"tux/gulp",
	"tux/win",
	"tux/yipe"
};

#define RIGHT 0
#define LEFT 1

struct tuxguy {
    int facing;
    double x, y;
    double dx, dy;
    sprite *spr[TUX_NUM_STATES][2];
    int endx, endy;
    int state;
    int wordlen;
    wchar_t word[MAX_WORD_SIZE + 1];
} tux_object;

struct splatter {
    double x, y;
    int alive;
} splat_object[MAX_FISHIES_HARD + 1];

struct splatter null_splat;

//Structure for passing address of data to threded function 
struct tts_announcer_cascade_data_struct
{
	int *address_of_fishies;
	int *address_of_tts_pause;
};



#endif
