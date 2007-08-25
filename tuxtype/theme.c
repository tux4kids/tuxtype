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


SDL_Surface* letters[255] = {NULL}; //get rid of this
wchar_t ALPHABET[256];
unsigned char FINGER[256][10];
int ALPHABET_SIZE;


#define MAX_LANGUAGES 100



void ChooseTheme(void)
{
  SDL_Surface* titles[MAX_LANGUAGES] = {NULL};
  SDL_Surface* select[MAX_LANGUAGES] = {NULL};
  SDL_Surface* left = NULL, *right = NULL;
  SDL_Rect leftRect, rightRect;
  SDL_Surface* world = NULL, *map = NULL, *photo = NULL;
  SDL_Surface* bkg = NULL;
  TTF_Font* font = NULL;
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

  int old_use_english;
  char old_theme_path[FNLEN];

  DIR* themesDir = NULL;
  struct dirent* themesFile = NULL;

  /* save previous settings in case we back out: */
  old_use_english = settings.use_english;
  strncpy(old_theme_path, settings.theme_data_path, FNLEN - 1);

  sprintf(fn, "%s/themes/", settings.default_data_path);
  themesDir = opendir(fn);

  if (!themesDir)
  {
    fprintf(stderr, "ChooseTheme() - cannot open themes directory!");
    return;
  }

  do
  {
    themesFile = readdir(themesDir);
    if (!themesFile)
			break;

		/* we ignore any hidden file and CVS */

		if (themesFile->d_name[0] == '.') 
			continue;

		if (strcmp("CVS", themesFile->d_name)==0)
			continue;

		/* check to see if it is a directory */
		sprintf(fn, "%s/themes/%s", settings.default_data_path, themesFile->d_name);


		/* CheckFile() returns 2 if dir, 1 if file, 0 if neither: */
		if (CheckFile(fn) == 2) {
		    /* HACK: we should get the names from file :) */
		    strncpy( themeNames[themes], themesFile->d_name, FNLEN-1);
		    /* Make sure theme name is capitalized: */
                    themeNames[themes][0] = toupper(themeNames[themes][0]);
		    strncpy( themePaths[themes++], themesFile->d_name, FNLEN-1 );
		}
	} while (1);

	closedir(themesDir);

	settings.use_english = 1;
        // HACK: is font empty now???
	font = LoadFont(settings.theme_font_name, MENU_FONT_SIZE);

	titles[0] = BlackOutline( "English", font, &white );
	select[0] = BlackOutline( "English", font, &yellow);
	for (i = 1; i<themes; i++) {
		titles[i] = BlackOutline( themeNames[i], font, &white );
		select[i] = BlackOutline( themeNames[i], font, &yellow);
	}

	world = LoadImage("world.png", IMG_ALPHA);
	worldRect.x = 480 - (world->w/2);
	worldRect.w = world->w;
	worldRect.y = 10;
	worldRect.h = world->h;

	

	TTF_CloseFont(font);
        font = NULL;

	settings.use_english = old_use_english;

	bkg = LoadImage("main_bkg.png", IMG_REGULAR);

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
								SetupPaths(themePaths[loc]);
							} else {
								/* --- english --- */
								SetupPaths(NULL);
							}
							stop = 1;
							break;
						}
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) { 
						settings.use_english = old_use_english;
						strncpy(settings.theme_data_path, old_theme_path, FNLEN - 1);
						stop = 1; 
						break; 
					}
					if (event.key.keysym.sym == SDLK_RETURN) { 
						if (loc) {
							/* --- set theme --- */
							SetupPaths(themePaths[loc]);
						} else {
							/* --- english --- */
							SetupPaths(NULL);
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

		        if (loc) SetupPaths(themePaths[loc]); else SetupPaths(NULL);

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

  /* --- clear graphics before quitting --- */ 

  for (i = 0; i<themes; i++)
  {
    SDL_FreeSurface(titles[i]);
    SDL_FreeSurface(select[i]);
  }

  SDL_FreeSurface(world);
  SDL_FreeSurface(bkg);
  SDL_FreeSurface(left);
  SDL_FreeSurface(right);
  bkg = NULL;  /* the other pointers are going out of scope so we don't */
               /* have to worry about setting them to NULL              */
}
