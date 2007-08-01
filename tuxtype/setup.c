/***************************************************************************
                          setup.c 
 -  description: Init SDL
                             -------------------
    begin                : Thu May 4 2000
    copyright            : (C) 2000 by Sam Hart
                         : (C) 2003 by Jesse Andrews
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

//global vars

//int hidden; // Read the README file in the image directory for info on this ;)

/***************************
	GraphicsInit: Initializes the graphic system
****************************/
void GraphicsInit(Uint32 video_flags)
{
	LOG( "GraphicsInit - Initialize graphic system\n" );

	DEBUGCODE {
		fprintf(stderr, "-SDL Setting VidMode to %ix%ix%i\n", RES_X, RES_Y, BPP);
	}

	/* NOTE fullscreen vs. windowed is indicated by video_flags */
	screen = SDL_SetVideoMode(RES_X, RES_Y, BPP, video_flags);

	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n", RES_X, RES_Y, SDL_GetError());
		exit(2);
	}


	LOG( "SDL_SetClipRect(screen, NULL):\n" );

	SDL_SetClipRect(screen, NULL); // Let's set the appropriate clip rect  -- JA: is neccessary???  

	LOG( "SDL_ShowCursor(0):\n" );

	SDL_ShowCursor(0); // no cursor please

	LOG( "SDL_WM_SetCaption(\"Tux Typing\", PACKAGE);\n" );

	SDL_WM_SetCaption("Tux Typing", "tuxtype"); // Set window manager stuff

	/* --- setup color we use --- */
	black.r       = 0x00; black.g       = 0x00; black.b       = 0x00;
        gray.r        = 0x80; gray.g        = 0x80; gray.b        = 0x80;
	dark_blue.r   = 0x00; dark_blue.g   = 0x00; dark_blue.b   = 0x60; 
	red.r         = 0xff; red.g         = 0x00; red.b         = 0x00;
	white.r       = 0xff; white.g       = 0xff; white.b       = 0xff;
	yellow.r      = 0xff; yellow.g      = 0xff; yellow.b      = 0x00; 

	InitEngine();

	DEBUGCODE {
		fprintf(stderr, "-SDL VidMode successfully set to %ix%ix%i\n", RES_X, RES_Y, BPP);
	}

	LOG( "GraphicsInit():END\n" );
}

/****************************
	LibInit : Init the SDL
	library
*****************************/
void LibInit(Uint32 lib_flags)
{
	LOG( "LibInit():\n-About to init SDL Library\n" );

	if (SDL_Init(lib_flags) < 0) 
		if (settings.sys_sound) {
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				fprintf(stderr, "Couldn't initialize SDL: %s\n",
				SDL_GetError());
				exit(2);
			} else {
				LOG( "Couldn't initialize SDL Sound\n" );
				settings.sys_sound = 0;
			}
		}


	atexit(SDL_Quit); // fire and forget... 

	LOG( "-SDL Library init'd successfully\n" );

	if (settings.sys_sound) 
		if (Mix_OpenAudio( 22050, AUDIO_S16, 1, 2048) < 0) {
			fprintf( stderr, "Warning: couldn't set 22050 Hz 8-bit audio\n - Reasons: %s\n", SDL_GetError());
			settings.sys_sound=0;
		}

	LOG( "-about to init SDL_ttf\n" );

	if (TTF_Init() < 0) {
		fprintf( stderr, "Couldn't initialize SDL_ttf\n" );
		exit(2);
	}

	atexit(TTF_Quit);

	SDL_EnableKeyRepeat( 0, SDL_DEFAULT_REPEAT_INTERVAL );
	/* Need this to get Unicode values from keysyms: */
	SDL_EnableUNICODE(1);

	LOG( "LibInit():END\n" );
}

/* Load the settings from a file... make sure to update SaveSettings if you change
 *  what can be saved/loaded 
 */
void LoadSettings( void ) {
	
	char fn[FNLEN];
	char setting[FNLEN];
	char value[FNLEN];
	FILE *settingsFile;
	
	/* set the settings directory/file */

	#ifdef WIN32
		snprintf( fn, FNLEN-1, "userdata/settings.txt" );
		LOG("WIN32 defined\n");
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/settings.txt", getenv("HOME") );
		LOG("WIN32 not defined\n");
	#endif

	DEBUGCODE { printf("LoadSettings: settings file is '%s'\n", fn ); }
	
	LOG("LoadSettings: trying to open settings file\n");
	
	settingsFile = fopen( fn, "r" );

	/* FIXME should set complete default profile if file not found */
	if (settingsFile == NULL) {
		printf("LoadSettings: Settings file does not exist! settings not loaded\n");
		settings.mus_volume = 100;
		settings.sfx_volume = 100;
		return;
	}
	
	/* we load all the settings here */
	
	while (!feof(settingsFile)) {
		fscanf( settingsFile, "%[^=]=%[^\n]\n", setting, value );
	
		DEBUGCODE { printf( "%s = %s", setting, value ); }
		
		if (strncmp( setting, "lang", FNLEN ) == 0 ) {
			DEBUGCODE { printf("LoadSettings: Setting language to %s", value); }
			strncpy(settings.lang, value, FNLEN-1 );
			settings.lang[FNLEN-1]=0;
			SetupTheme(value);
		}
		if (strncmp( setting, "o_lives", FNLEN ) == 0 ) {
			DEBUGCODE { printf("LoadSettings: Setting lives to %s", value); }
			settings.o_lives = atoi(value);
		}
		if (strncmp( setting, "mus_volume", FNLEN ) == 0 ) {
			DEBUGCODE { printf("LoadSettings: Setting music volume to %s", value); }
			settings.mus_volume = atoi(value);
		}
		if (strncmp( setting, "sfx_volume", FNLEN ) == 0 ) {
			DEBUGCODE { printf("LoadSettings: Setting effects volume to %s", value); }
			settings.sfx_volume = atoi(value);
		}
		if (strncmp( setting, "menu_music", FNLEN ) == 0 ) {
			DEBUGCODE { printf("LoadSettings: Setting menu music to %s", value); }
			settings.menu_music = atoi(value);
		}
		if (strncmp( setting, "fullscreen", FNLEN ) == 0 ) {
			settings.fullscreen = atoi(value);
		}
	}
	
	fclose( settingsFile );

}

/* Save the settings from a file... make sure to update LoadSettings if you change
 *  what can be saved/loaded 
 */
void SaveSettings(void)
{
	char fn[FNLEN];
	FILE* settingsFile;
	
	/* set the settings directory/file */

	#ifdef WIN32
		_mkdir( "userdata" );  // just in case try to create save location
		snprintf( fn, FNLEN-1, "userdata/settings.txt" );
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
		mkdir( fn, 0755 ); // just in case try to create save location
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/settings.txt", getenv("HOME") );
	#endif


	DEBUGCODE { printf("SaveSettings: settings file is '%s'\n", fn ); }
	
	LOG("SaveSettings: trying to open settings file\n");
	
	settingsFile = fopen( fn, "w" );

	if (settingsFile == NULL) {
		printf("SaveSettings: Settings file cannot be created!\n");
		return;
	}
	
	/* Save all the settings here! */
	if (strncmp( themeName, "", FNLEN) != 0)
		fprintf( settingsFile, "lang=%s\n", themeName );
	if (settings.o_lives > 9)
		fprintf( settingsFile, "o_lives=%d\n", settings.o_lives );

	fprintf( settingsFile, "mus_volume=%d\n", settings.mus_volume );
	fprintf( settingsFile, "sfx_volume=%d\n", settings.sfx_volume );
	fprintf( settingsFile, "menu_music=%d\n", settings.menu_music );
	fprintf( settingsFile, "fullscreen=%d\n", settings.fullscreen);


// 	if (screen->flags & SDL_FULLSCREEN){
// 		fprintf( settingsFile, "fullscreen=%s\n", "1");
// 	} else {
// 		fprintf( settingsFile, "fullscreen=%s\n", "0");
// 	}
	fclose( settingsFile );
}
