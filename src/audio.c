/***************************************************************************
 -  file: audio.c
 -  description: this file contains audio related functions
                            -------------------
    begin                : Jan 22, 2003
    copyright            : Sam Hart, Jesse Andrews (C) 2003
    email                : tuxtype-dev@tux4kids.net
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

static Mix_Music* defaultMusic = NULL; // holds music for audioMusicLoad/unload


void PlaySound(Mix_Chunk* snd)
{
  if(!snd) return;
  if (!settings.sys_sound) return;

  Mix_PlayChannel(-1, snd, 0);
}

/* MusicLoad attempts to load and play the music file 
 * Note: loops == -1 means forever
 */
void MusicLoad(const char* musicFilename, int loops)
{
  Mix_Music* tmp_music = NULL;

  if (!settings.sys_sound) return;
  if (!musicFilename) return;

  tmp_music = LoadMusic(musicFilename);

  if (tmp_music)
  {
    MusicUnload(); //Unload previous defaultMusic
    defaultMusic = tmp_music;
    Mix_PlayMusic(defaultMusic, loops);
  }
}


/* MusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void MusicUnload(void)
{
  if (!settings.sys_sound) return;

  if (defaultMusic)
  {
    Mix_FreeMusic(defaultMusic);
    defaultMusic = NULL;
  }
}


/* audioMusicPlay attempts to play the passed music data. 
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void MusicPlay(Mix_Music* musicData, int loops)
{ 
  if (!settings.sys_sound) return;
  if (!musicData) return;

  /* Stop previous music before playing new one: */
  MusicUnload();	
  Mix_PlayMusic(musicData, loops);
}
