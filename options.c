/***************************************************************************
 -  file: options.c
 -  description: Functions supporting settings struct for Tux Typing
                             -------------------
    begin                : Wed July 11 2007
    copyright            : (C) 2000 by David Bruce
                           dbruce@tampabay.rr.com

    project email        : tuxmath-devel@sourceforge.net
    website              : http://tux4kids.alioth.debian.org

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "globals.h"
#include "funcs.h"

/* Local function prototypes: */

/* Global (for now) settings struct: */
game_option_type settings;
profiles activeProfile; //added globally since will be required globally
highscores hscores;

/* Simply stick all the default values into the settings struct: */
void Opts_Initialize(void)
{
  strncpy(settings.theme_font_name, DEFAULT_MENU_FONT, FNLEN);
  strncpy(settings.theme_locale_name, DEFAULT_LOCALE, FNLEN);
  settings.use_english = DEFAULT_USE_ENGLISH;
  settings.fullscreen = DEFAULT_FULLSCREEN;
  settings.sys_sound = DEFAULT_SYS_SOUND;
  settings.sfx_volume = DEFAULT_SFX_VOLUME;
  settings.mus_volume = DEFAULT_MUS_VOLUME;
  settings.menu_music = DEFAULT_MENU_MUSIC;
  settings.menu_sound = DEFAULT_MENU_SOUND;
  settings.speed_up = DEFAULT_SPEED_UP;
  settings.show_tux4kids = DEFAULT_SHOW_TUX4KIDS;
  settings.debug_on = DEFAULT_DEBUG_ON;
  settings.o_lives = DEFAULT_O_LIVES;
  settings.sound_vol = DEFAULT_SOUND_VOL;
  settings.numProfiles = DEFAULT_PROFILES; //no profiles made yet, so default, which is 0
  settings.hidden = DEFAULT_HIDDEN; 
}

void profile_Initialize(void) //first profile shouldn't have any scores...
{
	activeProfile.highscore1 	= 	0;
	activeProfile.highscore2 	= 	0;
	activeProfile.highscore3 	= 	0;
	activeProfile.highscore4 	= 	0;
	activeProfile.highscore5 	= 	0;
	activeProfile.highscore6 	= 	0;
	activeProfile.highscore7 	= 	0;
	activeProfile.highscore8 	= 	0;
	activeProfile.wpm_highest 	= 	0;
	activeProfile.wpm_average 	= 	0;
	activeProfile.wpm_slowest 	= 	0;
	activeProfile.wpm_taken	 	=	0;
}
void hScores_Initialize(void)
{
	hscores.highscore1_name 	= DEFAULT_GNAME;
	hscores.ghighscore1 		= DEFAULT_GHIGHSCORE1;
	hscores.highscore2_name 	= DEFAULT_GNAME;
	hscores.ghighscore2 		= DEFAULT_GHIGHSCORE2;
	hscores.highscore3_name 	= DEFAULT_GNAME;
	hscores.ghighscore3 		= DEFAULT_GHIGHSCORE3;
	hscores.highscore4_name 	= DEFAULT_GNAME;
	hscores.ghighscore4 		= DEFAULT_GHIGHSCORE4;
	hscores.highscore5_name 	= DEFAULT_GNAME;
	hscores.ghighscore5 		= DEFAULT_GHIGHSCORE5;
	hscores.highscore6_name 	= DEFAULT_GNAME;
	hscores.ghighscore6 		= DEFAULT_GHIGHSCORE6;
	hscores.highscore7_name 	= DEFAULT_GNAME;
	hscores.ghighscore7 		= DEFAULT_GHIGHSCORE7;
	hscores.highscore8_name 	= DEFAULT_GNAME;
	hscores.ghighscore8 		= DEFAULT_GHIGHSCORE8;
	hscores.wpm_fastest_name 	= DEFAULT_GNAME;
	hscores.wpm_fastest 		= DEFAULT_GWPM_FASTEST;
	hscores.wpm_average_name 	= DEFAULT_GNAME;
	hscores.wpm_average 		= DEFAULT_GWPM_AVERAGE;
	hscores.wpm_slowest_name 	= DEFAULT_GNAME;
	hscores.wpm_slowest 		= DEFAULT_GWPM_SLOWEST;
}