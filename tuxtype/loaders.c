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

/* Local function prototypes: */
int max(int n1, int n2);
SDL_Surface* flip(SDL_Surface *in, int x, int y);

/* Returns 1 if valid file, 2 if valid dir, 0 if neither: */
int CheckFile(const char* file)
{
  FILE* fp = NULL;
  DIR* dp = NULL;

  if (!file)
  {
    fprintf(stderr, "CheckFile(): invalid char* argument!");
    return;
  }

  DEBUGCODE {fprintf(stderr, "CheckFile() - checking: %s\n", file);} 

  dp = opendir(file);
  if (dp)
  {
    LOG("Opened successfully as DIR\n");

    closedir(dp);
    return 2;
  }

  fp = fopen(file, "r");
  if (fp)
  {
    LOG("Opened successfully as FILE\n");
    fclose(fp);
    return 1;
  }

  LOG("Unable to open as either FILE or DIR\n");
  return 0;
}


/* FIXME need to read language's font name, if needed - e.g. Russian. */
/* also should have return value reflect success or failure.     */
void LoadLang(void)
{
  char fn[FNLEN];

  /* we only need to load a lang.po file if we
   * are actually using a theme, so this is a little
   * different than the other loaders 
   */ 

  if (settings.use_english)
  {
    /* We need to set the locale to something supporting UTF-8: */
    if (!setlocale(LC_CTYPE, "en_US.UTF-8"))
      fprintf(stderr, "Cannot support UTF-8, ASCII-only words will be used\n");
    return;
  }

  /* --- create full path to the lang.po file --- */
  sprintf( fn, "%s/lang.po", settings.theme_data_path);

  /* FIXME should have program try to setlocale() to lang-specific locale -  */
  /* for now, at least get a default UTF-8 encoding set: */
  if (!setlocale(LC_CTYPE, "en_US.UTF-8"))
    fprintf(stderr, "Cannot support UTF-8, ASCII-only words will be used\n");

  /* This function confusingly returns 0 if successful! */
  if (0 != Load_PO_File( fn ))  /* Meaning it failed! */
  {
    /* failed to find a lang.po file, clear gettext & return */
    return;
  }
}

int max(int n1, int n2)
{
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
SDL_Surface* flip(SDL_Surface* in, int x, int y ) {
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
		in->w, in->h, 32, RMASK, GMASK, BMASK, AMASK);

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


/* FIXME need code to search for font paths on different platforms */
TTF_Font* LoadFont(const char* font_name, int font_size )
{
  TTF_Font* loaded_font = NULL;
  char fn[FNLEN];
  int i;

  /* try to find font in default data dir: */
  sprintf(fn, "%s/fonts/%s", settings.default_data_path, font_name );

  DEBUGCODE { fprintf(stderr, "LoadFont(): looking for %s using data paths\n", fn); }

  /* try to load the font, if successful, return font*/
 loaded_font = TTF_OpenFont(fn, font_size);
 if (loaded_font != NULL)
   return loaded_font;
		

  /* HACK hard-coded for Debian once Andika is included: */ 
  sprintf(fn, "%s/%s", "/usr/share/fonts/truetype/ttf-andika/", font_name);
  DEBUGCODE { fprintf(stderr, "LoadFont(): looking for %s\n in OS' font path\n", fn); }

  /* try to load the font, if successful, return font*/
  loaded_font = TTF_OpenFont(fn, font_size);
  if (loaded_font != NULL)
    return loaded_font;

  fprintf(stderr, "LoadFont(): Error - couldn't load font: %s\n", font_name);
  return NULL;
}

/***********************
	LoadImage : Load an image and set transparent if requested
************************/
SDL_Surface* LoadImage(const char* datafile, int mode)
{
  int oldDebug;  //so we can turn off debug output for this func only
  SDL_Surface* tmp_pic = NULL, *final_pic = NULL;
  char fn[FNLEN];

  oldDebug = settings.debug_on;  // suppress output for now
  settings.debug_on = 0;

  DEBUGCODE { fprintf(stderr, "LoadImage: loading %s\n", datafile ); }

  /* Look for image under theme path if desired: */
  if (!settings.use_english && !(mode & IMG_NO_THEME))
  {
    sprintf(fn, "%s/images/%s", settings.theme_data_path, datafile);
    DEBUGCODE { fprintf(stderr, "LoadImage: looking in %s\n", fn); }

    tmp_pic = IMG_Load(fn);
    if (tmp_pic != NULL)
      DEBUGCODE { fprintf(stderr, "Graphics file %s successfully loaded\n", fn);}
    else
      DEBUGCODE { fprintf(stderr, "Warning: graphics file %s could not be loaded\n", fn);}
  }

  /* If we don't have a valid image yet, try the default path: */
  if (!tmp_pic)
  {
    sprintf(fn, "%s/images/%s", settings.default_data_path, datafile);
    DEBUGCODE { fprintf(stderr, "LoadImage: looking in %s\n", fn); }

    tmp_pic = IMG_Load(fn);
    if (tmp_pic != NULL)
      DEBUGCODE { fprintf(stderr, "Graphics file %s successfully loaded\n", fn);}
    else
      DEBUGCODE { fprintf(stderr, "Warning: graphics file %s could not be loaded\n", fn);}
  }

  /* Couldn't load image - action depends on whether image is essential: */
  if (!tmp_pic)
  {
    if (mode & IMG_NOT_REQUIRED)
    { 
      settings.debug_on = oldDebug;
      return NULL;
    }

    fprintf(stderr, "ERROR could not load required graphics file %s\n", datafile);
    exit(1);
  }


  /* If we get to here, success - setup the image in the proper format: */

  switch (mode & IMG_MODES)
  {
    case IMG_REGULAR:
    { 
      final_pic = SDL_DisplayFormat(tmp_pic);
      SDL_FreeSurface(tmp_pic);
      break;
    }

    case IMG_ALPHA:
    {
      final_pic = SDL_DisplayFormatAlpha(tmp_pic);
      SDL_FreeSurface(tmp_pic);
      break;
    }

    case IMG_COLORKEY:
    {
      SDL_LockSurface(tmp_pic);
      SDL_SetColorKey(tmp_pic,
                      (SDL_SRCCOLORKEY | SDL_RLEACCEL),
                      SDL_MapRGB(tmp_pic->format, 255, 255, 0));
      final_pic = SDL_DisplayFormat(tmp_pic);
      SDL_FreeSurface(tmp_pic);
      break;
    }

    default:
    {
      LOG ("Image mode not recognized\n");
    }
  }

  LOG( "LoadImage(): Done\n" );

  settings.debug_on = oldDebug;

  return (final_pic);
}

sprite* FlipSprite(sprite* in, int X, int Y ) {
	sprite* out;

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

sprite* LoadSprite(const char* name, int MODE ) {
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

void FreeSprite(sprite *gfx ) {
	int x;
	for (x = 0; x < gfx->num_frames; x++)
		SDL_FreeSurface( gfx->frame[x] );
	SDL_FreeSurface( gfx->default_img );
	free(gfx);
}

/***************************
	LoadSound : Load a sound/music patch from a file.
****************************/
Mix_Chunk* LoadSound(const char* datafile )
{ 
  Mix_Chunk* tempChunk = NULL;
  char fn[FNLEN];

  /* First look under theme path if desired: */
  if (!settings.use_english)
  {
    sprintf(fn , "%s/sounds/%s", settings.theme_data_path, datafile);
    tempChunk = Mix_LoadWAV(fn);
    if (tempChunk)
      return tempChunk;
  }

  /* If nothing loaded yet, try default path: */
  if (!tempChunk)
  {
    sprintf(fn , "%s/sounds/%s", settings.default_data_path, datafile);
    tempChunk = Mix_LoadWAV(fn);
    return tempChunk;
  }
}


/************************
	LoadMusic : Load
	music from a datafile
*************************/
Mix_Music* LoadMusic(const char* datafile )
{ 
  Mix_Music* temp_music = NULL;
  char fn[FNLEN];

  /* First look under theme path if desired: */
  if (!settings.use_english)
  {
    sprintf(fn , "%s/sounds/%s", settings.theme_data_path, datafile);
    temp_music = Mix_LoadMUS(fn);
    if (temp_music)
      return temp_music;
  }

  /* If nothing loaded yet, try default path: */
  if (!temp_music)
  {
    sprintf(fn , "%s/sounds/%s", settings.default_data_path, datafile);
    temp_music = Mix_LoadMUS(fn);
    return temp_music;
  }
}
