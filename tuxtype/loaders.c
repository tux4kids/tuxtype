/***************************************************************************
 -  file: loaders.c
 -  description: Functions to load multimedia for Tux Typing
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

/* check to see if file exists, if so return true */
int checkFile( const char *file ) {
	static struct stat fileStats;

	fileStats.st_mode = 0;

	stat( file, &fileStats );
		
	return (S_IFREG & fileStats.st_mode);
}

void LoadLang( void ) {
	char fn[FNLEN];

	/* we only need to load a lang.po file if we
	 * are actually using a theme, so this is a little
	 * different than the other loaders 
	 */ 

	if (useEnglish) {
		/* clear translations and return! */
		return;
	}

	/* --- create full path to the lang.po file --- */

	sprintf( fn, "%s/lang.po", realPath[0]);
	if (load_trans( fn )) {
		/* failed to find a lang.po file, clear gettext & return */
		return;
	}
}

int max( int n1, int n2 ) {
	return (n1 > n2 ? n1 : n2);
}

/**********************
 Flip:
   input: a SDL_Surface, x, y
   output: a copy of the SDL_Surface flipped via rules:

     if x is a positive value, then flip horizontally
     if y is a positive value, then flip vertically

     note: you can have it flip both
**********************/
SDL_Surface *flip( SDL_Surface *in, int x, int y ) {
	SDL_Surface *out, *tmp;
	SDL_Rect from_rect, to_rect;
	Uint32	flags;
	Uint32  colorkey=0;

	/* --- grab the settings for the incoming pixmap --- */

	SDL_LockSurface(in);
	flags = in->flags;

	/* --- change in's flags so ignore colorkey & alpha --- */

	if (flags & SDL_SRCCOLORKEY) {
		in->flags &= ~SDL_SRCCOLORKEY;
		colorkey = in->format->colorkey;
	}
	if (flags & SDL_SRCALPHA) {
		in->flags &= ~SDL_SRCALPHA;
	}

	SDL_UnlockSurface(in);

	/* --- create our new surface --- */

	out = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		in->w, in->h, 32, rmask, gmask, bmask, amask);

	/* --- flip horizontally if requested --- */

	if (x) {
		from_rect.h = to_rect.h = in->h;
		from_rect.w = to_rect.w = 1;
		from_rect.y = to_rect.y = 0;
		from_rect.x = 0;
		to_rect.x = in->w - 1;

		do {
			SDL_BlitSurface(in, &from_rect, out, &to_rect);
			from_rect.x++;
			to_rect.x--;
		} while (to_rect.x >= 0);
	}

	/* --- flip vertically if requested --- */

	if (y) {
		from_rect.h = to_rect.h = 1;
		from_rect.w = to_rect.w = in->w;
		from_rect.x = to_rect.x = 0;
		from_rect.y = 0;
		to_rect.y = in->h - 1;

		do {
			SDL_BlitSurface(in, &from_rect, out, &to_rect);
			from_rect.y++;
			to_rect.y--;
		} while (to_rect.y >= 0);
	}

	/* --- restore colorkey & alpha on in and setup out the same --- */

	SDL_LockSurface(in);

	if (flags & SDL_SRCCOLORKEY) {
		in->flags |= SDL_SRCCOLORKEY;
		in->format->colorkey = colorkey;
		tmp = SDL_DisplayFormat(out);
		SDL_FreeSurface(out);
		out = tmp;
		out->flags |= SDL_SRCCOLORKEY;
		out->format->colorkey = colorkey;
	} else if (flags & SDL_SRCALPHA) {
		in->flags |= SDL_SRCALPHA;
		tmp = SDL_DisplayFormatAlpha(out);
		SDL_FreeSurface(out);
		out = tmp;
	} else {
		tmp = SDL_DisplayFormat(out);
		SDL_FreeSurface(out);
		out = tmp;
	}

	SDL_UnlockSurface(in);

	return out;
}

TTF_Font *LoadFont( char *fontfile, int fontsize ) {
	TTF_Font *loadedFont;
	char fn[FNLEN];
	int i;

	/* try to find font first in theme dir, then in default */
	for (i=useEnglish; i<2; i++) {
		sprintf( fn, "%s/fonts/%s", realPath[i], fontfile );
		if ( checkFile(fn) ) {
			/* try to load the font, if successful, return font*/

			loadedFont = TTF_OpenFont( fn, fontsize );

			if (loadedFont != NULL)
				return loadedFont;
		}
	}

	fprintf(stderr, "FATAL ERROR: couldn't load font: %s\n", fontfile);
	exit(1);

	return NULL;
}

/***********************
	LoadImage : Load an image and set transparent if requested
************************/
SDL_Surface *LoadImage( char *datafile, int mode )
{
	int i;
	SDL_Surface  *tmp_pic=NULL, *final_pic=NULL;
	char         fn[FNLEN];

	DEBUGCODE { fprintf(stderr, "LoadImage: loading %s\n", datafile ); }

	/* truth table for start of loop, since we only use theme on those conditions!
              useEng    IMG_NO_THEME    i
                 0           0          0
                 0           1          1
                 1           0          1
                 1           1          1
	 */

	for (i = (useEnglish || (mode & IMG_NO_THEME)); i<2; i++) {

		sprintf( fn, "%s/images/%s", realPath[i], datafile );

		if ( checkFile( fn ) ) {
			tmp_pic = IMG_Load( fn );
			if (tmp_pic != NULL)
				break; 
			else
				fprintf(stderr, "Warning: graphics file %s is corrupt\n", fn);
		}
	}

	if (tmp_pic == NULL) {
		if (mode & IMG_NOT_REQUIRED) 
			return NULL;

		fprintf(stderr, "ERROR could not load required graphics file %s\n", datafile);
		exit(1);
	}

	/* finally setup the image to the proper format */

	switch (mode & IMG_MODES) {
		case IMG_REGULAR:
			final_pic = SDL_DisplayFormat(tmp_pic);
			SDL_FreeSurface(tmp_pic);
			break;
		case IMG_ALPHA:
			final_pic = SDL_DisplayFormatAlpha(tmp_pic);
			SDL_FreeSurface(tmp_pic);
			break;
		case IMG_COLORKEY:
			SDL_LockSurface(tmp_pic);
			SDL_SetColorKey(tmp_pic, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(tmp_pic->format, 255, 255, 0));
			final_pic = SDL_DisplayFormat(tmp_pic);
			SDL_FreeSurface(tmp_pic);
			break;
	}

	LOG( "LOADIMAGE: Done\n" );

	return (final_pic);
}

sprite* FlipSprite( sprite *in, int X, int Y ) {
	sprite *out;

	out = malloc(sizeof(sprite));
	if (in->default_img != NULL)
		out->default_img = flip( in->default_img, X, Y );
	else
		out->default_img = NULL;
	for ( out->num_frames=0; out->num_frames<in->num_frames; out->num_frames++ )
		out->frame[out->num_frames] = flip( in->frame[out->num_frames], X, Y );
	out->cur = 0;
	return out;
}

sprite* LoadSprite( char* name, int MODE ) {
	sprite *new_sprite;
	char fn[FNLEN];
	int x;

	/* JA --- HACK check out what has changed with new code */

	new_sprite = malloc(sizeof(sprite));

	sprintf(fn, "%sd.png", name);
	new_sprite->default_img = LoadImage( fn, MODE|IMG_NOT_REQUIRED );
	for (x = 0; x < MAX_SPRITE_FRAMES; x++) {
		sprintf(fn, "%s%d.png", name, x);
		new_sprite->frame[x] = LoadImage( fn, MODE|IMG_NOT_REQUIRED );
		if ( new_sprite->frame[x] == NULL ) {
			new_sprite->cur = 0;
			new_sprite->num_frames = x;
			break;
		}
	}

	DEBUGCODE {
		fprintf( stderr, "loading sprite %s - contains %d frames\n",
		        name, new_sprite->num_frames );
	}

	return new_sprite;
}

void FreeSprite( sprite *gfx ) {
	int x;
	for (x = 0; x < gfx->num_frames; x++)
		SDL_FreeSurface( gfx->frame[x] );
	SDL_FreeSurface( gfx->default_img );
	free(gfx);
}

/***************************
	LoadSound : Load a sound/music patch from a file.
****************************/
Mix_Chunk      *LoadSound( char *datafile )
{ 
	Mix_Chunk *tempChunk=NULL;
	char fn[FNLEN];
	int i;

	for (i = useEnglish; i<2; i++) {
		sprintf(fn , "%s/sounds/%s", realPath[i], datafile);
		if ( checkFile(fn) ) {
			tempChunk = Mix_LoadWAV(fn);
			if (tempChunk)
				return tempChunk;
		}
	}

	/* didn't find anything... fail peacefully */

	return NULL;
}

/************************
	LoadMusic : Load
	music from a datafile
*************************/
Mix_Music *LoadMusic(char *datafile )
{ 
	char            fn[FNLEN];
	Mix_Music	*tempMusic;
	int i;

	for (i = useEnglish; i<2; i++) {
		sprintf( fn , "%s/sounds/%s", realPath[i], datafile );
		if ( checkFile(fn) ) {
			tempMusic = Mix_LoadMUS(fn);
			if (tempMusic)
				return tempMusic;
		}
	}

	/* didn't find anything... fail peacefully */

	return NULL;
}
