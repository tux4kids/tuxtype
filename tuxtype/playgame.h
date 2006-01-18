/***************************************************************************
                          playgame.h
  description:
                             -------------------
    begin                : Fri May 12 2000
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

#ifndef __PLAYGAME_H__
#define __PLAYGAME_H__

//definitions
#define RIGHT 0
#define LEFT 1
#define RIGHT_LEFT 2
#define NUM_BACKGROUNDS	4
#define GRAPHIC_SPACE 4 //space between each status bar element

#define NUM_NUMS 10	//"Mmmmm....Num-Nums...."
#define NUM_LETTERS 26
#define NOT_A_LETTER NUM_LETTERS + 2

//word cascade specific defs
#define NUM_WORDS 200
#define MAX_WORD_SIZE 7		//no words more than 7 chars

#define CONGRATS_FRAMES	2	//number of frames for congrats animation
//#define CONGRATS_ONSCREEN 4
#define OH_NO_FRAMES 1	//number of frames for "oh no!" losing animation
//#define OH_NO_ONSCREEN        4
#define FRAME_MAX 80	//maximum number of counter frames before looping frame counter to 0
#define MAX_END_FRAMES_BETWEEN_LEVELS 350	//number of frames between levels
#define MAX_END_FRAMES_WIN_GAME 750	//number of frames at ending animation
#define MAX_END_FRAMES_GAMEOVER 400	//number of frames at game over
#define END_FRAME_DY 2	//rise speed of ending animation text
#define WIN_GAME_XAMP 4	//the xamplitide of winning game text circular motion
#define WIN_GAME_YAMP 4	//the yamplitude of winning game text circular motion
#define WIN_GAME_ANGLE_MULT 4	//the angle multiplier for winning game te xt circ motion

//tux defs
#define WALK_FRAMES 4	//number of walking frames
#define STAND_FRAMES 2	//number of standing frames
#define RUN_FRAMES 2	//number of running frames
#define GULP_FRAMES 4	//number of gulping frames
#define WIN_FRAMES 1	//number of winning frames
#define YIPE_FRAMES 1	//number of hurting frames
//tux frame types
#define WALKING				1
#define STANDING			2
#define RUNNING				3
#define GULPING				4
#define WINNING				6
#define YIPING				7
//tux frame lengths (number of frames each cell of animation frames on-screen)
#define WALK_ONSCREEN	10
#define RUN_ONSCREEN	8
#define STAND_ONSCREEN	15
#define GULP_ONSCREEN	10
//tux default walking & running speeds
#define WALKING_SPEED	1.5
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
#define FISHY_FRAMES	4
#define FISHY_ONSCREEN	10
#define SPLAT_FRAMES	1
#define SPLAT_ONSCREEN	4
//#define DEFAULT_SPEED                 .6
#define DEFAULT_SPEED	.3

//fishy math - EASY
#define MAX_AMPLITUDE_EASY      	1
#define MAX_PHASE_EASY			1
#define MAX_ANGLE_EASY			1
#define MAX_SPEED_VARIANCE_EASY	        1
#define FISH_SPACING_EASY		90
#define EASY_LIVES_INIT	        	5
//fishy math - MEDYUM
#define MAX_AMPLITUDE_MEDIUM	        2
#define MAX_PHASE_MEDIUM		5
#define MAX_ANGLE_MEDIUM		.1
#define MAX_SPEED_VARIANCE_MEDIUM	1
#define FISH_SPACING_MEDIUM	        40
#define MEDIUM_LIVES_INIT		7
//fishy math - HARD
#define MAX_AMPLITUDE_HARD	3
#define MAX_PHASE_HARD		5
#define MAX_ANGLE_HARD		.1
#define MAX_SPEED_VARIANCE_HARD	2
#define FISH_SPACING_HARD	30
#define HARD_LIVES_INIT		9

#define MAX_UPDATES	3*(20+MAX_FISHIES_HARD)	//20 to account for text updates (I hope)

/* The default alpha for slow moving words,
 note that 128 is optimized for speed! */
#define DEFAULT_ALPHA 128

//game play structures
typedef struct {
    int frames;			//number of frames to display this object
    SDL_Surface *image[RIGHT_LEFT];
} rl_object;			//right-left object

typedef struct {
    int frames;			//number of frames to display this object
    SDL_Surface *image;
} object;			//normal (non-RL) object

struct fishypoo {
    int alive;
    int endframe;
    int curframe;
    int letter;
    double x, y;
    double x0;
    double dy;
    double x_amplitude;
    double x_phase;
    double x_angle_mult;
} fish_object[MAX_FISHIES_HARD + 1];

struct fishypoo null_fishy;

struct wordypoo {
    int alive;
    int eaten;
    int endframe;
    int curframe;
    char *word;
    double x, y;
    double x0;
    double dy;
    double x_amplitude;
    double x_phase;
    double x_angle_mult;
} word_object[MAX_FISHIES_HARD + 1];

struct wordypoo null_word;

struct tuxguy {
    int facing;
    double x, y;
    double dx, dy;
    int endx, endy;
    int endframe;
    int frametype;
    int curframe;
    int letter;
    char word[MAX_WORD_SIZE + 1];
} tux_object;

struct splatter {
    double x, y;
    int alive;
    int curframe;
    int endframe;
} splat_object[MAX_FISHIES_HARD + 1];

struct splatter null_splat;

#define DIRLEN		2
const char DIR[RIGHT_LEFT][DIRLEN] = { "r", "l" };
#define TOT_BAD_CHARS 8
const int BAD_CHARS[TOT_BAD_CHARS] =
    { '#', '!', '@', '$', '%', '^', '&', '*' };

//Practice definitions
#define PKCASCADE_TUT_SCREENS		6
#define PRACT_TUX_Y			10
//KCASCADE, FIRST FRAME
#define DEFAULT_TUT1_FISH_SPEED .5
#define TUT1_ENDFRAME		500
//KCASCADE. SECOND FRAME
#define DEFAULT_TUT2_FISH_SPEED	.35
#define TUX2_DX1		1
#define TUX2_ENDX_GOTO1	        10
//KCASCADE. THIRD FRAME
#define DEFAULT_TUT3_FISH_SPEED	.8
#define TUX3_DX1		1
#define TUX3_ENDX_GOTO1		10
#define FISH3_MAX_Y		10
//KCASCADE. FOURTH FRAME
#define DEFAULT_TUT4_FISH_SPEED	.5
#define TUX4_DX1		1.5
#define FISH4_MAX_Y		10

const char WORD_PATH[PATHNUM][FNLEN] =
    { DATA_PREFIX "/tuxtype" , ".", ".." };

/*const char WORD_PATH[PATHNUM][FNLEN] =
    { ".", "data", "/usr/local/share/tuxtype", "/usr/share/tuxtype",
	"../data", "/usr/share/pixmaps/tuxtype" }; */

#endif				//KCASCADE. FOURTH FRAMEsplay this object6s --*/ __PLAYGAME_H__
