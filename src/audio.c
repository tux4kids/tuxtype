/*
   audio.c:

   Code for audio-related functions

   Copyright 2000, 2003, 2010.
   Authors: Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   audio.c is part of Tux Typing, a.k.a "tuxtype".

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




#include "globals.h"
#include "funcs.h"

static Mix_Music* defaultMusic = NULL; // holds music for audioMusicLoad/unload

// play sound once
void PlaySound(Mix_Chunk* snd)
{
  PlaySoundLoop(snd, 0);
}

// play sound with optional repeats, or -1 for infinite
void PlaySoundLoop(Mix_Chunk* snd, int loops)
{
  if(!snd)
    return;
  if (!settings.sys_sound)
    return;

  Mix_PlayChannel(-1, snd, loops);
}

// halt a channel or -1 for all
void audioHaltChannel(int channel)
{
    Mix_HaltChannel(channel);
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
