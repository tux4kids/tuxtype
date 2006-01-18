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

Mix_Chunk      *sound[NUM_WAVES];
Mix_Music      *music;

void playsound(Mix_Chunk *snd) {
	if (!sys_sound) return;

	Mix_PlayChannel(-1, snd, 0);
}

Mix_Music *defaultMusic = NULL; // holds music for audioMusicLoad/unload

/* audioMusicLoad attempts to load and play the music file 
 * Note: loops == -1 means forever
 */
void audioMusicLoad( char *musicFilename, int loops ) {
	if (!sys_sound) return;

	audioMusicUnload(); // make sure defaultMusic is clear

	defaultMusic = LoadMusic( musicFilename );
	Mix_PlayMusic( defaultMusic, loops );
}

/* audioMusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void audioMusicUnload( void ) {
	if (!sys_sound) return;

	if ( defaultMusic )
		Mix_FreeMusic( defaultMusic );

	defaultMusic=NULL;
}

/* audioMusicPlay attempts to play the passed music data. 
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void audioMusicPlay( Mix_Music *musicData, int loops ) { 
	if (!sys_sound) return;

	audioMusicUnload();	
	Mix_PlayMusic( musicData, loops );
}
