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

/* Simply stick all the default values into the settings struct: */
void Opts_Initialize(void)
{
  strncpy(settings.theme_font_name, DEFAULT_FONT_NAME, FNLEN);
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
  settings.hidden = DEFAULT_HIDDEN; 
}
