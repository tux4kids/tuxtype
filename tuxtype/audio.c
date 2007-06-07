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
/* NOTE these two don't really belong here - not used in this file */
Mix_Chunk* sound[NUM_WAVES];
Mix_Music* music;

Mix_Music* defaultMusic = NULL; // holds music for audioMusicLoad/unload

void PlaySound(Mix_Chunk* snd) {
	if (!sys_sound) return;

	Mix_PlayChannel(-1, snd, 0);
}

/* MusicLoad attempts to load and play the music file 
 * Note: loops == -1 means forever
 */
void MusicLoad(const char *musicFilename, int loops ) {
	if (!sys_sound) return;

	MusicUnload(); // make sure defaultMusic is clear

	defaultMusic = LoadMusic( musicFilename );
	Mix_PlayMusic( defaultMusic, loops );
}

/* MusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void MusicUnload( void ) {
	if (!sys_sound) return;

	if ( defaultMusic )
		Mix_FreeMusic( defaultMusic );

	defaultMusic = NULL;
}

/* audioMusicPlay attempts to play the passed music data. 
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void MusicPlay(Mix_Music* musicData, int loops)
{ 
  if (!sys_sound) return;
  /* Stop previous music before playing new one: */
  MusicUnload();	
  Mix_PlayMusic(musicData, loops);
}
