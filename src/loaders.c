/*
   loaders.c:
   
   Functions to load multimedia for Tux Typing.

   Copyright 2000, 2003, 2007, 2008, 2009, 2010.
   Authors: Sam Hart, Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   loaders.c is part of Tux Typing, a.k.a "tuxtype".

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


#ifdef HAVE_RSVG
/***********************
    SVG related functions
************************/
#include<librsvg/rsvg.h>
#include<librsvg/rsvg-cairo.h>

/* Load an SVG file and resize it to given dimensions.
   if width or height is set to 0 no resizing is applied
   (partly based on TuxPaint's SVG loading function) */
SDL_Surface* LoadSVGOfDimensions(char* filename, int width, int height)
{
  cairo_surface_t* temp_surf;
  cairo_t* context;
  RsvgHandle* file_handle;
  RsvgDimensionData dimensions;
  SDL_Surface* dest;
  float scale_x;
  float scale_y;
  int bpp = 32;
  Uint32 Rmask, Gmask, Bmask, Amask;

  DEBUGCODE{
    fprintf(stderr, "LoadSVGOfDimensions(): looking for %s\n", filename);
  }

  rsvg_init();

  file_handle = rsvg_handle_new_from_file(filename, NULL);
  if(file_handle == NULL)
  {
    DEBUGCODE{
      fprintf(stderr, "LoadSVGOfDimensions(): file %s not found\n", filename);
    }
    rsvg_term();
    return NULL;
  }

  rsvg_handle_get_dimensions(file_handle, &dimensions);
  DEBUGCODE{
    fprintf(stderr, "SVG is %d x %d\n", dimensions.width, dimensions.height);
  }

  if(width <= 0 || height <= 0)
  {
    width = dimensions.width;
    height = dimensions.height;
    scale_x = 1.0;
    scale_y = 1.0;
  }
  else
  {
    scale_x = (float)width / dimensions.width;
    scale_y = (float)height / dimensions.height;
  }

  /* FIXME: We assume that our bpp = 32 */

  /* rmask, gmask, bmask, amask defined in SDL_extras.h do not work !
     are those (taken from TuxPaint) dependent on endianness ? */
  Rmask = 0x00ff0000;
  Gmask = 0x0000ff00;
  Bmask = 0x000000ff;
  Amask = 0xff000000;

  dest = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
        width, height, bpp, Rmask, Gmask, Bmask, Amask);

  SDL_LockSurface(dest);
  temp_surf = cairo_image_surface_create_for_data(dest->pixels,
        CAIRO_FORMAT_ARGB32, dest->w, dest->h, dest->pitch);

  context = cairo_create(temp_surf);
  if(cairo_status(context) != CAIRO_STATUS_SUCCESS)
  {
    DEBUGCODE{
      fprintf(stderr, "LoadSVGOfDimensions(): error rendering SVG from %s\n", filename);
    }
    g_object_unref(file_handle);
    cairo_surface_destroy(temp_surf);
    rsvg_term();
    return NULL;
  }

  cairo_scale(context, scale_x, scale_y);
  rsvg_handle_render_cairo(file_handle, context);

  SDL_UnlockSurface(dest);

  g_object_unref(file_handle);
  cairo_surface_destroy(temp_surf);
  cairo_destroy(context);
  rsvg_term();

  return dest;
}

#endif

/***********************
        LoadImageFromFile : Simply load an image from given file
        or its SVG equivalent (if present). Return NULL if loading failed.
************************/
SDL_Surface* LoadImageFromFile(char *datafile)
{
  SDL_Surface* tmp_pic = NULL;

#ifdef HAVE_RSVG
  char svgfn[PATH_MAX];
#endif

  DEBUGCODE{
    fprintf(stderr, "LoadImageFromFile(): looking in %s\n", datafile);
  }

#ifdef HAVE_RSVG
  /* This is just an ugly workaround to test SVG
     before any scaling routines are implemented */

  /* change extension into .svg */
  char* dotpos = strrchr(datafile, '.');
  if (dotpos) //will be NULL if '.' not found:
  {
    strncpy(svgfn, datafile, dotpos - datafile);
    svgfn[dotpos - datafile] = '\0';
    strcat(svgfn, ".svg");

    /* try to load an SVG equivalent */
    tmp_pic = LoadSVGOfDimensions(svgfn, 0, 0);
  }
#endif

  if(tmp_pic == NULL)
    /* Try to load image with SDL_image: */
    tmp_pic = IMG_Load(datafile);

  return tmp_pic;
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

    tmp_pic = LoadImageFromFile(fn);
    if (tmp_pic != NULL)
      {}
    else
      DEBUGCODE { fprintf(stderr, "Warning: graphics file %s could not be loaded\n", fn);}
  }

  /* If we don't have a valid image yet, try the default path: */
  if (!tmp_pic)
  {
    sprintf(fn, "%s/images/%s", settings.default_data_path, datafile);

    tmp_pic = LoadImageFromFile(fn);
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
    if (tempChunk)
      return tempChunk;
  }
  // We never want to get here...
  fprintf(stderr, "LoadSound() - could not load %s\n", datafile);
  return NULL;
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
