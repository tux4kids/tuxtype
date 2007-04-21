/***************************************************************************
                          theme.c 
 -  description: theme related code
                             -------------------
    begin                : Jan 6 2003
    copyright            : (C) 2003 by Jesse Andrews et al
    email                : jdandr2@tux4kids.net
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

#define NUM_PATHS 4

const char PATHS[NUM_PATHS][FNLEN] = {
	"./data",
	"/usr/share/"PACKAGE"/data",
	"/usr/local/share/"PACKAGE"/data",
	DATA_PREFIX"/share/"PACKAGE"/data"
};

SDL_Surface *letters[255] = { NULL };
unsigned char ALPHABET[256];
unsigned char KEYMAP[256];
unsigned char FINGER[256][10];
int ALPHABET_SIZE;
unsigned char realPath[2][FNLEN];
char themeName[FNLEN];
int useEnglish;


#define MAX_LANGUAGES 100

/* setup_theme: this function is in charge of setting up
 * the paths that the loaders use.  It will set a variable
 * numPaths = 1 or 2.  1 if we are just using the default,
 * 2 if there are two themes.  Then it will edit the varible
 * Paths[].  It will always put the theme path first, then
 * the default path
 */
void setupTheme( char *dirName ) {
	static struct stat dirStats;
	int i;
	int found = 0;
	useEnglish=1; // default is to use English if we cannot find theme

	for (i=0; i<NUM_PATHS && !found; i++) {

		DEBUGCODE{
		fprintf(stderr, "setupTheme(): checking for: %s\n", PATHS[i]);
		}

		dirStats.st_mode = 0;
		stat( PATHS[i], &dirStats );
		if (S_IFDIR & dirStats.st_mode) {
			strncpy( realPath[1], PATHS[i], FNLEN-1);
			strncpy( themeName, "", FNLEN-1 );
			found = 1; /* so quit looking */

			DEBUGCODE{
			fprintf(stderr, "path '%s' found, copy to realPath[1]\n", PATHS[i]);
			}
		}
		else {
			DEBUGCODE{
			fprintf(stderr, "path '%s' not found.\n", PATHS[i]);
			}
		}

	}

	if (dirName != NULL) {
		char fullDir[FNLEN];

		/* find the path to the theme */
		sprintf( fullDir, "%s/themes/%s", realPath[1], dirName );
		dirStats.st_mode = 0;
		stat( fullDir, &dirStats );
		if (S_IFDIR & dirStats.st_mode) {
			useEnglish=0;
			strncpy( realPath[0], fullDir, FNLEN-1 );
			strncpy( themeName, dirName, FNLEN-1 );
		}
	}
	DEBUGCODE
	{
		fprintf(stderr, "Leaving setupTheme():\n");
		if (dirName != NULL)
			fprintf(stderr, "realPath[0] = %s\n", realPath[0]);
		fprintf(stderr, "realPath[1] = %s\n", realPath[1]);
	}
	
}

void chooseTheme( void ) {
	SDL_Surface *titles[MAX_LANGUAGES];
	SDL_Surface *select[MAX_LANGUAGES];
	SDL_Surface *left, *right;
	SDL_Rect leftRect, rightRect;
	SDL_Surface *world, *map, *photo;
	SDL_Rect worldRect, photoRect;
	SDL_Rect titleRects[8];
	int stop = 0;
	int loc = 0;
	int old_loc = 1;

	int themes = 1;
	int i;
	unsigned char fn[FNLEN];
	unsigned char themeNames[MAX_LANGUAGES][FNLEN];
	unsigned char themePaths[MAX_LANGUAGES][FNLEN];

	int old_useEnglish;
	char old_realPath[FNLEN];

	DIR *themesDir;
	struct dirent *themesFile;
	struct stat fileStats;

	old_useEnglish = useEnglish;
	strncpy( old_realPath, realPath[0], FNLEN-1 );

	sprintf( fn, "%s/themes/", realPath[1]);
	themesDir = opendir(fn);

	do {
		themesFile = readdir(themesDir);
		if (!themesFile)
			break;

		/* we ignore any hidden file and CVS */

		if (themesFile->d_name[0] == '.') 
			continue;

		if (strcmp("CVS", themesFile->d_name)==0)
			continue;

		/* check to see if it is a directory */
		sprintf( fn, "%s/themes/%s", realPath[1], themesFile->d_name);

		fileStats.st_mode=0;
		stat( fn, &fileStats );

		if (S_IFDIR & fileStats.st_mode) {
		    /* HACK: we should get the names from file :) */
		    strncpy( themeNames[themes], themesFile->d_name, FNLEN-1);
		    strncpy( themePaths[themes++], themesFile->d_name, FNLEN-1 );
		}
	} while (1);

	closedir(themesDir);

	useEnglish = 1;
        // HACK: is font empty now???
	font = LoadFont( ttf_font, ttf_font_size );

	titles[0] = black_outline( "english", font, &white );
	select[0] = black_outline( "english", font, &yellow);
	for (i = 1; i<themes; i++) {
		titles[i] = black_outline( themeNames[i], font, &white );
		select[i] = black_outline( themeNames[i], font, &yellow);
	}

	world = LoadImage("world.png", IMG_ALPHA);
	worldRect.x = 480 - (world->w/2);
	worldRect.w = world->w;
	worldRect.y = 10;
	worldRect.h = world->h;

	

	TTF_CloseFont(font);

	useEnglish = old_useEnglish;

	bkg = LoadImage("main_bkg.jpg", IMG_REGULAR);

	left = LoadImage("left.png", IMG_ALPHA);
	leftRect.w = left->w; leftRect.h = left->h;
	leftRect.x = 160 - 80 - (leftRect.w/2); leftRect.y = 430;

	right = LoadImage("right.png", IMG_ALPHA);
	rightRect.w = right->w; rightRect.h = right->h;
	rightRect.x = 160 + 80 - (rightRect.w/2); rightRect.y = 430;

	/* set initial rect sizes */
	titleRects[0].y = 30;
	titleRects[0].w = titleRects[0].h = titleRects[0].x = 0;
	for (i = 1; i<8; i++) {
		titleRects[i].y = titleRects[i-1].y + 50;
		titleRects[i].w = titleRects[i].h = titleRects[i].x = 0;
	}
	

	while (!stop) {
		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_MOUSEMOTION: 
					for (i=0; (i<8) && (loc-(loc%8)+i<themes); i++)
						if (inRect( titleRects[i], event.motion.x, event.motion.y )) {
							loc = loc-(loc%8)+i;
							break;
						}
					
					break;
				case SDL_MOUSEBUTTONDOWN: 
					if (inRect( leftRect, event.button.x, event.button.y )) 
						if (loc-(loc%8)-8 >= 0) {
							loc=loc-(loc%8)-8;
							break;
						}
					if (inRect( rightRect, event.button.x, event.button.y )) 
						if (loc-(loc%8)+8 < themes) {
							loc=loc-(loc%8)+8;
							break;
						}
					for (i=0; (i<8) && (loc-(loc%8)+i<themes); i++) 
						if (inRect(titleRects[i], event.button.x, event.button.y)) {
							loc = loc-(loc%8)+i;
							if (loc) {
								/* --- set theme --- */
								setupTheme(themePaths[loc]);
							} else {
								/* --- english --- */
								setupTheme(NULL);
							}
							stop = 1;
							break;
						}
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) { 
						useEnglish = old_useEnglish;
						strncpy( realPath[0], old_realPath, FNLEN-1 );
						stop = 1; 
						break; 
					}
					if (event.key.keysym.sym == SDLK_RETURN) { 
						if (loc) {
							/* --- set theme --- */
							setupTheme(themePaths[loc]);
						} else {
							/* --- english --- */
							setupTheme(NULL);
						}
						stop = 1;
						break;
					}

					if ((event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_PAGEUP)) {
						if (loc-(loc%8)-8 >= 0) 
							loc=loc-(loc%8)-8;
					}

					if ((event.key.keysym.sym == SDLK_RIGHT) || (event.key.keysym.sym == SDLK_PAGEDOWN)) {
						if (loc-(loc%8)+8 < themes)
							loc=(loc-(loc%8)+8);
					}


					if (event.key.keysym.sym == SDLK_UP) {
						if (loc > 0)
							loc--;
					}

					if (event.key.keysym.sym == SDLK_DOWN) {
						if (loc+1<themes)
							loc++;
					}
			}

		if (old_loc != loc) {
			int start;

			SDL_BlitSurface( bkg, NULL, screen, NULL );

			SDL_BlitSurface( world, NULL, screen, &worldRect );

		        if (loc) setupTheme(themePaths[loc]); else setupTheme(NULL);

			map = LoadImage( "map.png", IMG_ALPHA|IMG_NOT_REQUIRED );
			if (map) {
				SDL_BlitSurface( map, NULL, screen, &worldRect );
				SDL_FreeSurface( map );
			}

			photo = LoadImage( "photo.png", IMG_ALPHA|IMG_NOT_REQUIRED );
			if (photo) {
				photoRect.x = 480 - (photo->w/2);
				photoRect.y = 250;
				photoRect.w = photo->w;
				photoRect.h = photo->h;
				SDL_BlitSurface( photo, NULL, screen, &photoRect );
				SDL_FreeSurface( photo );
			}

			start = loc - (loc % 8);
			for (i = start; i<MIN(start+8,themes); i++) {
				titleRects[i%8].x = 160 - (titles[i]->w/2);
				if (i == loc)
					SDL_BlitSurface(select[loc], NULL, screen, &titleRects[i%8]);
				else
					SDL_BlitSurface(titles[i], NULL, screen, &titleRects[i%8]);
			}

			/* --- draw buttons --- */

			if (start>0) 
				SDL_BlitSurface( left, NULL, screen, &leftRect );

			if (start+8<themes) 
				SDL_BlitSurface( right, NULL, screen, &rightRect );

			SDL_UpdateRect(screen, 0, 0, 0 ,0);
		}
		SDL_Delay(40);
		old_loc = loc;
	}

	/* --- clear graphics before quiting --- */ 

	for (i = 0; i<themes; i++) {
		SDL_FreeSurface(titles[i]);
		SDL_FreeSurface(select[i]);
	}

	SDL_FreeSurface(world);
	SDL_FreeSurface(bkg);
	SDL_FreeSurface(left);
	SDL_FreeSurface(right);
}
