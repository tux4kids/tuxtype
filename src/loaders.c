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
#include "SDL_extras.h"
#include "mysetenv.h"

static SDL_Surface* win_bkgd = NULL;
static SDL_Surface* fullscr_bkgd = NULL;

/* Local function prototypes: */
static int max(int n1, int n2);
//static SDL_Surface* flip(SDL_Surface *in, int x, int y);

/* Returns 1 if valid file, 2 if valid dir, 0 if neither: */
int CheckFile(const char* file)
{
  FILE* fp = NULL;
  DIR* dp = NULL;

  if (!file)
  {
    fprintf(stderr, "CheckFile(): invalid char* argument!");
    return -1;
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


/* FIXME not sure we need to call *textdomain() functions again here  */
/* FIXME need to read language's font name, if needed - e.g. Russian. */
/* also should have return value reflect success or failure.     */
void LoadLang(void)
{
  char *s1, *s2, *s3, *s4;
  char buf [30];
  s1 = setlocale(LC_ALL, settings.theme_locale_name);
  s2 = bindtextdomain(PACKAGE, TUXLOCALE);
  s3 = bind_textdomain_codeset(PACKAGE, "UTF-8");
  s4 = textdomain(PACKAGE);

  DEBUGCODE
  {
    fprintf(stderr, "PACKAGE = %s\n", PACKAGE);
    fprintf(stderr, "TUXLOCALE = %s\n", TUXLOCALE);
    fprintf(stderr, "setlocale(LC_ALL, %s) returned: %s\n", settings.theme_locale_name, s1);
    fprintf(stderr, "bindtextdomain(PACKAGE, TUXLOCALE) returned: %s\n", s2);
    fprintf(stderr, "bind_textdomain_codeset(PACKAGE, \"UTF-8\") returned: %s\n", s3);
    fprintf(stderr, "textdomain(PACKAGE) returned: %s\n", s4);
    fprintf(stderr, "gettext(\"Fish\"): %s\n\n", gettext("Fish"));
//    fprintf(stderr, "After gettext() call\n");
  }

  /* Also set LANG and LANGUAGE as fallbacks because setlocale() unreliable */
  /* on some Windows versions, AFAICT                                       */
  snprintf(buf, 30, "%s", settings.theme_locale_name);
  buf[5] = '\0';  //en_US" rather than "en_US.utf8"
  DEBUGCODE { fprintf(stderr, "buf is: %s\n", buf); }

  if (my_setenv("LANG", buf) == -1)
  {
    fprintf(stderr, "Warning - could not set LANG to %s\n\n", buf);
  }

  if (my_setenv("LANGUAGE", buf) == -1)
  {
    fprintf(stderr, "Warning - could not set LANGUAGE to %s\n\n", buf);
  }

  return;
}


int max(int n1, int n2)
{
  return (n1 > n2 ? n1 : n2);
}



/* FIXME need code to search for font paths on different platforms */
TTF_Font* LoadFont(const char* font_name, int font_size )
{
  TTF_Font* loaded_font = NULL;
  char fn[FNLEN];

  /* try to find font in default data dir: */
  sprintf(fn, "%s/fonts/%s", settings.default_data_path, font_name );

  DEBUGCODE { fprintf(stderr, "LoadFont(): looking for %s using data paths\n", fn); }

  /* try to load the font, if successful, return font*/
  loaded_font = TTF_OpenFont(fn, font_size);
  if (loaded_font != NULL)
    return loaded_font;
		

  /* HACK hard-coded for Debian (and current exact font names): */ 

  if (strncmp(font_name, "AndikaDesRevG.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-sil-andika/AndikaDesRevG.ttf");
  else if (strncmp(font_name, "DoulosSILR.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-sil-doulos/DoulosSILR.ttf");
  else if (strncmp(font_name, "Kedage-n.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-kannada-fonts/Kedage-n.ttf");
  else if (strncmp(font_name, "lohit_bn.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-bengali-fonts/lohit_bn.ttf");
  else if (strncmp(font_name, "lohit_gu.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-indic-fonts-core/lohit_gu.ttf");
  else if (strncmp(font_name, "lohit_hi.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-indic-fonts-core/lohit_hi.ttf");
  else if (strncmp(font_name, "lohit_pa.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-indic-fonts-core/lohit_pa.ttf");
  else if (strncmp(font_name, "lohit_ta.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-indic-fonts-core/lohit_ta.ttf");
  else if (strncmp(font_name, "Rachana_w01.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-malayalam-fonts/Rachana_w01.ttf");
  else if (strncmp(font_name, "utkal.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-indic-fonts-core/utkal.ttf");
  else if (strncmp(font_name, "Vemana.ttf", FNLEN ) == 0)
    sprintf(fn, "/usr/share/fonts/truetype/ttf-indic-fonts-core/Vemena.ttf");



  DEBUGCODE { fprintf(stderr, "LoadFont(): looking for %s\n in OS' font path\n", fn); }

  /* try to load the font, if successful, return font*/
  loaded_font = TTF_OpenFont(fn, font_size);
  if (loaded_font != NULL)
    return loaded_font;

  /* We could not find desired font. If we were looking for something other  */
  /* than default (Andika) font, print warning and try to load default font: */
  if (strncmp(font_name, DEFAULT_FONT_NAME, FNLEN ) != 0)
  {
    fprintf(stderr, "Warning - could not load desired font: %s\n", font_name);
    fprintf(stderr, "Trying to load default instead: %s\n", DEFAULT_FONT_NAME);
    return LoadFont(DEFAULT_FONT_NAME, font_size);
  }
  else  /* Default failed also - bummer! */
  {
    fprintf(stderr, "LoadFont(): Error - couldn't load either selected or default font\n");
    return NULL;
  }
}
/***********************
	LoadImage : Load an image and set transparent if requested
************************/
SDL_Surface* LoadImage(const char* datafile, int mode)
{
  SDL_Surface* tmp_pic = NULL, *final_pic = NULL;
  char fn[FNLEN];

  /* Look for image under theme path if desired: */
  if (!settings.use_english && !(mode & IMG_NO_THEME))
  {
    sprintf(fn, "%s/images/%s", settings.theme_data_path, datafile);

    tmp_pic = IMG_Load(fn);
    if (tmp_pic != NULL)
      {}
    else
      DEBUGCODE { fprintf(stderr, "Warning: graphics file %s could not be loaded\n", fn);}
  }

  /* If we don't have a valid image yet, try the default path: */
  if (!tmp_pic)
  {
    sprintf(fn, "%s/images/%s", settings.default_data_path, datafile);

    tmp_pic = IMG_Load(fn);
    if (tmp_pic != NULL)
      {}
    else
      DEBUGCODE { fprintf(stderr, "Warning: graphics file %s could not be loaded\n", fn);}
  }

  /* NOTE changed this so we just return NULL instead of exiting - DSB   */
  /* Couldn't load image - action depends on whether image is essential: */
  if (!tmp_pic)
  {
    { 
      DEBUGCODE { fprintf(stderr, "Warning - could not load graphics file %s\n", datafile);}
      return NULL;
    }
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

//  LOG( "LoadImage(): Done\n" );

  return (final_pic);
}



/**********************
LoadBothBkgds() : loads two scaled images: one for the user's native 
resolution and one for 640x480 fullscreen. 
Returns: the number of images that were scaled
**********************/
int LoadBothBkgds(const char* datafile)
{
  int ret = 0;
  SDL_Surface* orig = NULL;
  
  //Avoid memory leak in case something else already loaded:
  FreeBothBkgds();

  LOG("Entering LoadBothBkgds()\n");

  orig = LoadImage(datafile, IMG_REGULAR);

  DEBUGCODE
  {
     printf("Scaling %dx%d to: %dx%d, %dx%d\n", 
           orig->w, orig->h, RES_X, RES_Y, fs_res_x, fs_res_y);
  }

  if (orig->w == RES_X && orig->h == RES_Y)
  {
    win_bkgd = orig;
  }
  else
  {
    win_bkgd = zoom(orig, RES_X, RES_Y);
    ++ret;
  }
  
  if (orig->w == fs_res_x && orig->h == fs_res_y)
  {
    fullscr_bkgd = orig;
  }
  else
  {
    fullscr_bkgd = zoom(orig, fs_res_x, fs_res_y);
    ++ret;
  }
  
  if (ret == 2) //orig won't be used at all
    SDL_FreeSurface(orig);
    
  DEBUGCODE
  {
    printf("%d images scaled\nLeaving LoadBothBkgds()\n", ret);
  }
  return ret;
}


SDL_Surface* CurrentBkgd(void)
{
  if (!screen)
    return NULL;
  if (screen->flags & SDL_FULLSCREEN)
    return fullscr_bkgd;
  else
    return win_bkgd;
}

void FreeBothBkgds(void)
{
  if (win_bkgd)
    SDL_FreeSurface(win_bkgd);
  win_bkgd = NULL;

  if (fullscr_bkgd)
    SDL_FreeSurface(fullscr_bkgd);
  fullscr_bkgd = NULL;
}


sprite* FlipSprite(sprite* in, int X, int Y ) {
	sprite* out;

	out = malloc(sizeof(sprite));
	if (in->default_img != NULL)
		out->default_img = Flip( in->default_img, X, Y );
	else
		out->default_img = NULL;
	for ( out->num_frames=0; out->num_frames<in->num_frames; out->num_frames++ )
		out->frame[out->num_frames] = Flip( in->frame[out->num_frames], X, Y );
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

void FreeSprite(sprite* gfx )
{
  int x;

  if (!gfx)
    return;
 
  for (x = 0; x < gfx->num_frames; x++)
  {
    if (gfx->frame[x])
      SDL_FreeSurface(gfx->frame[x]);
  }
  if (gfx->default_img)
    SDL_FreeSurface(gfx->default_img);
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
  // We never want to get here...
  return tempChunk;
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
  // We never want to get here...
  return temp_music;
}
