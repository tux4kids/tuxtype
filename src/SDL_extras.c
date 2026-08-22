/*
   SDL_extras.c:

   Headers for wrapper and utility functions for use with the
   SDL libraries.
   
   Copyright 2007, 2008, 2009, 2010.
   Authors: David Bruce, Tim Holy.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   SDL_extras.c is part of Tux Typing, a.k.a "tuxtype".

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

/* For TransWipe(): WIPE_BLINDS_VERT/HORIZ/BOX, RANDOM_WIPE, NUM_WIPES
   come from t4k_common.h's WipeStyle enum (same values/order). */

#include <math.h>

#include "convert_utf.h"
#include "SDL_extras.h"
#include "globals.h"
#include "pixels.h"
//Just need funcs.h for CurrentBkgd()
#include "funcs.h"

/**************************************************************************/
/*                                                                        */
/*        Begin graphics utility functions                                */
/*                                                                        */
/* These functions provide handy ways to do various types of drawing      */
/* without troubling the rest of tuxtype with messy mathematical details. */
/**************************************************************************/


/* DrawButton() creates and draws a translucent button with */
/* rounded ends.  All colors and alpha values are supported.*/
void DrawButton(SDL_Rect* target_rect,
                int radius,
                Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  /* NOTE - we use a 32-bit temp surface even if we have a 16-bit */
  /* screen - it gets converted during blitting.                  */
  SDL_Surface* tmp_surf = SDL_CreateSurface(target_rect->w,
                                             target_rect->h,
                                             SDL_PIXELFORMAT_RGBA32);
  Uint32 color = SDL_MapRGBA(SDL_GetPixelFormatDetails(tmp_surf->format), NULL, r, g, b, a);
  SDL_FillSurfaceRect(tmp_surf, NULL, color);
  RoundCorners(tmp_surf, radius);

  SDL_BlitSurface(tmp_surf, NULL, screen, target_rect);
  SDL_DestroySurface(tmp_surf);
}



void RoundCorners(SDL_Surface* s, Uint16 radius)
{
  int y = 0;
  int x_dist, y_dist;
  Uint32* p = NULL;
  Uint32 alpha_mask;
  int bytes_per_pix;

  const SDL_PixelFormatDetails* fmt;

  if (!s)
    return;
  if (!SDL_LockSurface(s))
    return;

  fmt = SDL_GetPixelFormatDetails(s->format);
  bytes_per_pix = fmt->bytes_per_pixel;
  if (bytes_per_pix != 4)
    return;

  /* radius cannot be more than half of width or height: */
  if (radius > (s->w)/2)
    radius = (s->w)/2;
  if (radius > (s->h)/2)
    radius = (s->h)/2;


  alpha_mask = fmt->Amask;

  /* Now round off corners: */
  /* upper left:            */
  for (y = 0; y < radius; y++)
  {
    p = (Uint32*)(s->pixels + (y * s->pitch));
    x_dist = radius;
    y_dist = radius - y;

    while (((x_dist * x_dist) + (y_dist * y_dist)) > (radius * radius))
    {
      /* (make pixel (x,y) transparent) */
      *p = *p & ~alpha_mask;
      p++;
      x_dist--;
    }
  }

  /* upper right:            */
  for (y = 0; y < radius; y++)
  {
    /* start at end of top row: */
    p = (Uint32*)(s->pixels + ((y + 1) * s->pitch) - bytes_per_pix);

    x_dist = radius;
    y_dist = radius - y;

    while (((x_dist * x_dist) + (y_dist * y_dist)) > (radius * radius))
    {
      /* (make pixel (x,y) transparent) */
      *p = *p & ~alpha_mask;
      p--;
      x_dist--;
    }
  }

  /* bottom left:            */
  for (y = (s->h - 1); y > (s->h - radius); y--)
  {
    /* start at beginning of bottom row */
    p = (Uint32*)(s->pixels + (y * s->pitch));
    x_dist = radius;
    y_dist = y - (s->h - radius);

    while (((x_dist * x_dist) + (y_dist * y_dist)) > (radius * radius))
    {
      /* (make pixel (x,y) transparent) */
      *p = *p & ~alpha_mask;
      p++;
      x_dist--;
    }
  }

  /* bottom right:            */
  for (y = (s->h - 1); y > (s->h - radius); y--)
  {
    /* start at end of bottom row */
    p = (Uint32*)(s->pixels + ((y + 1) * s->pitch) - bytes_per_pix);
    x_dist = radius;
    y_dist = y - (s->h - radius);

    while (((x_dist * x_dist) + (y_dist * y_dist)) > (radius * radius))
    {
      /* (make pixel (x,y) transparent) */
      *p = *p & ~alpha_mask;
      p--;
      x_dist--;
    }
  }
  SDL_UnlockSurface(s);
}


/**********************
 Flip:
   input: a SDL_Surface, x, y
   output: a copy of the SDL_Surface flipped via rules:

     if x is a nonzero value, then flip horizontally
     if y is a nonzero value, then flip vertically

     note: you can have it flip both
**********************/
SDL_Surface* Flip( SDL_Surface *in, int x, int y ) {
        SDL_Surface *out;
        SDL_Rect from_rect, to_rect;
        bool had_colorkey;
        Uint32  colorkey=0;
        SDL_BlendMode blend_mode;

        /* --- grab the settings for the incoming pixmap --- */

        had_colorkey = SDL_SurfaceHasColorKey(in);
        if (had_colorkey)
                SDL_GetSurfaceColorKey(in, &colorkey);
        SDL_GetSurfaceBlendMode(in, &blend_mode);

        /* --- temporarily disable colorkey & blending so the blits below
           copy pixels verbatim instead of keying/blending them --- */

        if (had_colorkey)
                SDL_SetSurfaceColorKey(in, false, 0);
        SDL_SetSurfaceBlendMode(in, SDL_BLENDMODE_NONE);

        /* --- create our new surface --- */

        out = SDL_CreateSurface(in->w, in->h, SDL_PIXELFORMAT_RGBA32);

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

        /* --- restore colorkey & blending on in, and set up out the same --- */

        if (had_colorkey) {
                SDL_SetSurfaceColorKey(in, true, colorkey);
                SDL_SetSurfaceColorKey(out, true, colorkey);
        }
        SDL_SetSurfaceBlendMode(in, blend_mode);
        SDL_SetSurfaceBlendMode(out, blend_mode);

        return out;
}

/* Blend two surfaces together. The third argument is between 0.0 and
   1.0, and represents the weight assigned to the first surface.  If
   the pointer to the second surface is NULL, this performs fading.

   Currently this works only with RGBA images, but this is largely to
   make the (fast) pointer arithmetic work out; it could be easily
   generalized to other image types. */
SDL_Surface* Blend(SDL_Surface* S1, SDL_Surface* S2, float gamma)
{
  const SDL_PixelFormatDetails *fmt1, *fmt2;
  Uint8 r1, r2, g1, g2, b1, b2, a1, a2;
  SDL_Surface *tmpS, *ret;
  Uint32 *cpix1, *epix1, *cpix2, *epix2;
  float gamflip;

  if (!S1)
    return NULL;

  fmt1 = fmt2 = NULL;
  tmpS = ret = NULL;

  gamflip = 1.0 - gamma;
  if (gamma < 0 || gamflip < 0)
  {
    perror("gamma must be between 0 and 1");
    exit(0);
  }

  fmt1 = SDL_GetPixelFormatDetails(S1->format);

  if (fmt1 && fmt1->bits_per_pixel != 32)
  {
    perror("This works only with RGBA images");
    return S1;
  }
  if (S2 != NULL)
  {
    fmt2 = SDL_GetPixelFormatDetails(S2->format);
    if (fmt2->bits_per_pixel != 32)
    {
      perror("This works only with RGBA images");
      return S1;
    }
    // Check that both images have the same width dimension
    if (S1->w != S2->w)
    {
      printf("S1->w %d, S2->w %d;  S1->h %d, S2->h %d\n",
             S1->w, S2->w, S1->h, S2->h);
      printf("Both images must have the same width dimensions\n");
      return S1;
    }
  }

  tmpS = SDL_ConvertSurface(S1, S1->format);
  if (tmpS == NULL)
  {
    perror("SDL_ConvertSurface() failed");
    return S1; 
  }
  if (!SDL_LockSurface(tmpS))
  {
    perror("SDL_LockSurface() failed");
    return S1;
  }

  // We're going to go through the pixels in reverse order, to start
  // from the bottom of each image. That way, we can blend things that
  // are not of the same height and have them align at the bottom.
  // So the "ending pixel" (epix) will be before the first pixel, and
  // the current pixel (cpix) will be the last pixel.
  epix1 = (Uint32*) tmpS->pixels - 1;
  cpix1 = epix1 + tmpS->w * tmpS->h;
  if (S2 != NULL
      && SDL_LockSurface(S2))
  {
    epix2 = (Uint32*) S2->pixels - 1;
    cpix2 = epix2 + S2->w * S2->h;
  }
  else
  {
    epix2 = epix1;
    cpix2 = cpix1;
  }

  for (; cpix1 > epix1; cpix1--, cpix2--)
  {
    SDL_GetRGBA(*cpix1, fmt1, NULL, &r1, &g1, &b1, &a1);
    a1 = gamma * a1;
    if (S2 != NULL && cpix2 > epix2)
    {
      SDL_GetRGBA(*cpix2, fmt2, NULL, &r2, &g2, &b2, &a2);
      r1 = gamma * r1 + gamflip * r2;
      g1 = gamma * g1 + gamflip * g2;
      b1 = gamma * b1 + gamflip * b2;
      a1 += gamflip * a2;
    }
    *cpix1 = SDL_MapRGBA(fmt1, NULL, r1, g1, b1, a1);
  }

  SDL_UnlockSurface(tmpS);

  if (S2 != NULL)
    SDL_UnlockSurface(S2);

  SDL_SetSurfaceBlendMode(tmpS, SDL_BLENDMODE_BLEND);
  ret = tmpS;

  return ret;
}




int inRect( SDL_Rect r, int x, int y)
{
  if ((x < r.x)
   || (y < r.y)
   || (x > r.x + r.w)
   || (y > r.y + r.h))
    return 0;
  return 1;
}


/* Darkens the screen by a factor of 2^bits */
void DarkenScreen(Uint8 bits)
{
#if BPP == 32
  Uint32* p;
#elif BPP == 16
  Uint16* p;
#else
  return;
#endif

  const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(screen->format);
  Uint32 rm = fmt->Rmask;
  Uint32 gm = fmt->Gmask;
  Uint32 bm = fmt->Bmask;
  int x, y;

  /* (realistically, 1 and 2 are the only useful values) */
  if (bits > 8)
    return;

  p = screen->pixels;

  for (y = 0; y < screen->h; y++)
  {
    for (x = 0; x < screen->w; x++)
    {
      *p = (((*p&rm)>>bits)&rm)
         | (((*p&gm)>>bits)&gm)
         | (((*p&bm)>>bits)&bm);
      p++;
    }
  }
}


void SwitchScreenMode(void)
{
  /* t4kcommon owns the window/screen-surface lifecycle and already
     caches both the windowed and fullscreen resolutions internally
     (set up by GraphicsInit()'s initial T4K_SetScreenMode() call). */
  T4K_SwitchScreenMode();
  screen = T4K_GetScreen();

  if (screen == NULL)
  {
    fprintf(stderr,
            "\nError: I could not switch screen mode.\n"
            "The Simple DirectMedia error that occured was:\n"
            "%s\n\n",
            SDL_GetError());
  }
  else
  {
    T4K_UpdateRect(screen, NULL);
  }

}


int WaitForKeypress(void)
{
  SDL_Event evt;
  while (1)
    while (SDL_PollEvent(&evt) )
      if (evt.type == SDL_EVENT_KEY_DOWN)
        return evt.key.key;
      else SDL_Delay(50);
}
/* Swiped shamelessly from TuxPaint
   Based on code from: http://www.codeproject.com/cs/media/imageprocessing4.asp
   copyright 2002 Christian Graus */

SDL_Surface* zoom(SDL_Surface* src, int new_w, int new_h)
{
  SDL_Surface* s;

  /* These function pointers will point to the appropriate */
  /* putpixel() and getpixel() variants to be used in the  */
  /* current colorspace:                                   */
  void (*putpixel) (SDL_Surface*, int, int, Uint32);
  Uint32(*getpixel) (SDL_Surface*, int, int);

  float xscale, yscale;
  int x, y;
  int floor_x, ceil_x,
        floor_y, ceil_y;
  float fraction_x, fraction_y,
        one_minus_x, one_minus_y;
  float n1, n2;
  Uint8 r1, g1, b1, a1;
  Uint8 r2, g2, b2, a2;
  Uint8 r3, g3, b3, a3;
  Uint8 r4, g4, b4, a4;
  Uint8 r, g, b, a;

  const SDL_PixelFormatDetails* src_fmt = SDL_GetPixelFormatDetails(src->format);

  /* Create surface for zoom: */

  s = SDL_CreateSurface(new_w, new_h, src->format);

  if (s == NULL)
  {
    fprintf(stderr, "\nError: Can't build zoom surface\n"
            "The Simple DirectMedia Layer error that occurred was:\n"
            "%s\n\n", SDL_GetError());
    return NULL;
//    cleanup();
//    exit(1);
  }


  /* Now assign function pointers to correct functions based */
  /* on data format of original and zoomed surfaces:         */
  getpixel = getpixels[src_fmt->bytes_per_pixel];
  putpixel = putpixels[src_fmt->bytes_per_pixel];

  SDL_LockSurface(src);
  SDL_LockSurface(s);

  xscale = (float) src->w / (float) new_w;
  yscale = (float) src->h / (float) new_h;

  for (x = 0; x < new_w; x++)
  {
    for (y = 0; y < new_h; y++)
    {
      /* Here we calculate the new RGBA values for each pixel */
      /* using a "weighted average" of the four pixels in the */
      /* corresponding location in the orginal surface:       */

      /* figure out which original pixels to use in the calc: */
      floor_x = floor((float) x * xscale);
      ceil_x = floor_x + 1;
      if (ceil_x >= src->w)
        ceil_x = floor_x;

      floor_y = floor((float) y * yscale);
      ceil_y = floor_y + 1;
      if (ceil_y >= src->h)
        ceil_y = floor_y;

      fraction_x = x * xscale - floor_x;
      fraction_y = y * yscale - floor_y;

      one_minus_x = 1.0 - fraction_x;
      one_minus_y = 1.0 - fraction_y;

      /* Grab their values:  */
      SDL_GetRGBA(getpixel(src, floor_x, floor_y), src_fmt, NULL,
                  &r1, &g1, &b1, &a1);
      SDL_GetRGBA(getpixel(src, ceil_x,  floor_y), src_fmt, NULL,
                  &r2, &g2, &b2, &a2);
      SDL_GetRGBA(getpixel(src, floor_x, ceil_y),  src_fmt, NULL,
                  &r3, &g3, &b3, &a3);
      SDL_GetRGBA(getpixel(src, ceil_x,  ceil_y),  src_fmt, NULL,
                  &r4, &g4, &b4, &a4);

      /* Create the weighted averages: */
      n1 = (one_minus_x * r1 + fraction_x * r2);
      n2 = (one_minus_x * r3 + fraction_x * r4);
      r = (one_minus_y * n1 + fraction_y * n2);

      n1 = (one_minus_x * g1 + fraction_x * g2);
      n2 = (one_minus_x * g3 + fraction_x * g4);
      g = (one_minus_y * n1 + fraction_y * n2);

      n1 = (one_minus_x * b1 + fraction_x * b2);
      n2 = (one_minus_x * b3 + fraction_x * b4);
      b = (one_minus_y * n1 + fraction_y * n2);

      n1 = (one_minus_x * a1 + fraction_x * a2);
      n2 = (one_minus_x * a3 + fraction_x * a4);
      a = (one_minus_y * n1 + fraction_y * n2);

      /* and put them into our new surface: */
      putpixel(s, x, y, SDL_MapRGBA(src_fmt, NULL, r, g, b, a));

    }
  }

  SDL_UnlockSurface(s);
  SDL_UnlockSurface(src);

  return s;
}



/*************************************************/
/* TransWipe: Performs various wipes to new bkgs */
/*************************************************/
/*
 * Given a wipe request type, and any variables
 * that wipe requires, will perform a wipe from
 * the current screen image to a new one.
 */
int TransWipe(const SDL_Surface* newbkg, int type, int segments, int duration)
{
  int i, j, x1, x2, y1, y2;
  int step1, step2, step3, step4;
  int frame;
  SDL_Rect src;
  SDL_Rect dst;

  LOG("->TransWipe(): START\n");

  /* Input validation: ----------------------- */
  if (!newbkg)
  {
    fprintf(stderr, "TransWipe() - 'newbkg' arg invalid!\n");
    return 0;
  }

  /* FIXME should support scaling here - DSB */
  if(newbkg->w != screen->w || newbkg->h != screen->h)
  {
    fprintf(stderr, "TransWipe() - wrong size newbkg* arg");
    return 0;
  }

  /* segments is num of divisions */
  /* duration is how many frames animation should take */
      
  if(segments < 1)
    segments = 1;
  if(duration < 1)
    duration = 1;

  /* Pick a card, any card...            */
  while(type == RANDOM_WIPE)
    type = rand() % NUM_WIPES;


  ResetBlitQueue();
  frame = 0;

  switch(type)
  {
    case WIPE_BLINDS_VERT:
    {
      LOG("--+ Doing 'WIPE_BLINDS_VERT'\n");

      step1 = screen->w/segments;
      step2 = step1/duration;

      for(i = 0; i <= duration; i++)
      {
        for(j = 0; j <= segments; j++)
        {
          x1 = step1 * (j - 0.5) - i * step2 + 1;
          x2 = step1 * (j - 0.5) + i * step2 + 1;
          src.x = x1;
          src.y = 0;
          src.w = step2;
          src.h = screen->h;
          dst.x = x2;
          dst.y = 0;
          dst.w = step2;
          dst.h = screen->h;
          SDL_BlitSurface(newbkg, &src, screen, &src);
          SDL_BlitSurface(newbkg, &dst, screen, &dst);
          AddRect(&src, &src);
          AddRect(&dst, &dst);
        }
        UpdateScreen(&frame);
      }

      src.x = 0;
      src.y = 0;
      src.w = screen->w;
      src.h = screen->h;
      SDL_BlitSurface(newbkg, NULL, screen, &src);
      T4K_UpdateRect(screen, NULL);

      break;
    } 

    case WIPE_BLINDS_HORIZ:
    {
      LOG("--+ Doing 'WIPE_BLINDS_HORIZ'\n");

      step1 = screen->h / segments;
      step2 = step1 / duration;

      for(i = 0; i <= duration; i++)
      {
        for(j = 0; j <= segments; j++)
        {
          y1 = step1 * (j - 0.5) - i * step2 + 1;
          y2 = step1 * (j - 0.5) + i * step2 + 1;
          src.x = 0;
          src.y = y1;
          src.w = screen->w;
          src.h = step2;
          dst.x = 0;
          dst.y = y2;
          dst.w = screen->w;
          dst.h = step2;
          SDL_BlitSurface(newbkg, &src, screen, &src);
          SDL_BlitSurface(newbkg, &dst, screen, &dst);
          AddRect(&src, &src);
          AddRect(&dst, &dst);
        }
        UpdateScreen(&frame);
      }

      src.x = 0;
      src.y = 0;
      src.w = screen->w;
      src.h = screen->h;
      SDL_BlitSurface(newbkg, NULL, screen, &src);
      T4K_UpdateRect(screen, NULL);

      break;
    }

    case WIPE_BLINDS_BOX:
    {
      LOG("--+ Doing 'WIPE_BLINDS_BOX'\n");

      step1 = screen->w/segments;
      step2 = step1/duration;
      step3 = screen->h/segments;
      step4 = step1/duration;

      for(i = 0; i <= duration; i++)
      {
        for(j = 0; j <= segments; j++)
        {
          x1 = step1 * (j - 0.5) - i * step2 + 1;
          x2 = step1 * (j - 0.5) + i * step2 + 1;
          src.x = x1;
          src.y = 0;
          src.w = step2;
          src.h = screen->h;
          dst.x = x2;
          dst.y = 0;
          dst.w = step2;
          dst.h = screen->h;
          SDL_BlitSurface(newbkg, &src, screen, &src);
          SDL_BlitSurface(newbkg, &dst, screen, &dst);
          AddRect(&src, &src);
          AddRect(&dst, &dst);
          y1 = step3 * (j - 0.5) - i * step4 + 1;
          y2 = step3 * (j - 0.5) + i * step4 + 1;
          src.x = 0;
          src.y = y1;
          src.w = screen->w;
          src.h = step4;
          dst.x = 0;
          dst.y = y2;
          dst.w = screen->w;
          dst.h = step4;
          SDL_BlitSurface(newbkg, &src, screen, &src);
          SDL_BlitSurface(newbkg, &dst, screen, &dst);
          AddRect(&src, &src);
          AddRect(&dst, &dst);
        }
        UpdateScreen(&frame);
      }

      src.x = 0;
      src.y = 0;
      src.w = screen->w;
      src.h = screen->h;
      SDL_BlitSurface(newbkg, NULL, screen, &src);
      T4K_UpdateRect(screen, NULL);

      break;
    }
    default:
      break;
  }
  return 1;
}






/************************************************************************/
/*                                                                      */
/*        Begin blit queue support                                      */
/*                                                                      */
/* This code (modified from Sam Lantinga's "Alien" example program)     */
/* implements a blit queue to perform screen updates in a more          */
/* optimized fashion.                                                   */
/************************************************************************/

//With fullscreen, we need more updates - 180 wasn't enough
#define MAX_UPDATES 512

/* --- Data Structure for Dirty Blitting --- */
static SDL_Rect srcupdate[MAX_UPDATES];
static SDL_Rect dstupdate[MAX_UPDATES];
static int numupdates = 0; // tracks how many blits to be done

struct blit {
    SDL_Surface* src;
    SDL_Rect* srcrect;
    SDL_Rect* dstrect;
    unsigned char type;
} blits[MAX_UPDATES];



/***********************
 InitBlitQueue()
 ***********************/
void InitBlitQueue(void)
{
  int i;

  /* --- Set up the update rectangle pointers --- */
  for (i = 0; i < MAX_UPDATES; ++i)
  {
    blits[i].srcrect = &srcupdate[i];
    blits[i].dstrect = &dstupdate[i];
  }
  numupdates = 0;
}


/**************************
ResetBlitQueue(): just set the number
of pending updates to zero
***************************/
void ResetBlitQueue(void)
{
  numupdates = 0;
}


/******************************
AddRect : Don't actually blit a surface,
    but add a rect to be updated next
    update
*******************************/
int AddRect(SDL_Rect* src, SDL_Rect* dst)
{

  /*borrowed from SL's alien (and modified)*/
  struct blit* update;

  if(!src)
  {
    fprintf(stderr, "AddRect() - invalid 'src' arg!\n");
    return 0;
  }

  if(!dst)
  {
    fprintf(stderr, "AddRect() - invalid 'dst' arg!\n");
    return 0;
  }

  if(numupdates >= MAX_UPDATES)
  {
    fprintf(stderr, "Warning - MAX_UPDATES exceeded, cannot add blit to queue\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "AddRect() - 'update' ptr invalid!\n");
    return 0;
  }

  update->srcrect->x = src->x;
  update->srcrect->y = src->y;
  update->srcrect->w = src->w;
  update->srcrect->h = src->h;
  update->dstrect->x = dst->x;
  update->dstrect->y = dst->y;
  update->dstrect->w = dst->w;
  update->dstrect->h = dst->h;
  update->type = 'I';

  return 1;
}



int DrawSprite(sprite* gfx, int x, int y)
{
  LOG("Entering DrawSprite()\n");

  if (!gfx || !gfx->frame[gfx->cur])
  {
    fprintf(stderr, "DrawSprite() - 'gfx' arg invalid!\n");
    LOG("Leaving DrawSprite()\n");
    return 0;
  }

  LOG("Leaving DrawSprite()\n");

  return DrawObject(gfx->frame[gfx->cur], x, y);

}



/**********************
DrawObject : Draw an object at the specified
location. No respect to clipping!
*************************/
int DrawObject(SDL_Surface* surf, int x, int y)
{
  struct blit *update;

  LOG("Entering DrawObject()\n");

  if (!surf)
  {
    fprintf(stderr, "DrawObject() - invalid 'surf' arg!\n");
    return 0;
  }

  DOUT(numupdates);

  if(numupdates >= MAX_UPDATES)
  {
    fprintf(stderr, "Warning - MAX_UPDATES exceeded, cannot add blit to queue\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "DrawObject() - 'update' ptr invalid!\n");
    return 0;
  }

  update->src = surf;
  update->srcrect->x = 0;
  update->srcrect->y = 0;
  update->srcrect->w = surf->w;
  update->srcrect->h = surf->h;
  update->dstrect->x = x;
  update->dstrect->y = y;
  update->dstrect->w = surf->w;
  update->dstrect->h = surf->h;
  update->type = 'D';

  LOG("Leaving DrawObject()\n");

  return 1;
}



/************************
UpdateScreen : Update the screen and increment the frame num
***************************/
void UpdateScreen(int* frame)
{
  int i;

  LOG("Entering UpdateScreen()\n");
  DOUT(numupdates);

  /* -- First erase everything we need to -- */
  for (i = 0; i < numupdates; i++)
  {
    if (blits[i].type == 'E') 
    {
//       DEBUGCODE
//       {
//         fprintf(stderr, "Erasing blits[%d]\n", i);
//         fprintf(stderr, "srcrect->x = %d\t srcrect->y = %d\t srcrect->w = %d\t srcrect->h = %d\n",
//               blits[i].srcrect->x, blits[i].srcrect->y, blits[i].srcrect->w, blits[i].srcrect->h);
//         fprintf(stderr, "dstrect->x = %d\t dstrect->y = %d\t dstrect->w = %d\t dstrect->h = %d\n",
//               blits[i].dstrect->x, blits[i].dstrect->y, blits[i].dstrect->w, blits[i].dstrect->h);
//       }

      SDL_BlitSurfaceUnchecked(blits[i].src, blits[i].srcrect, screen, blits[i].dstrect);
    }
  }

  LOG("Done erasing\n");

//  SNOW_erase();

  /* -- then draw -- */ 
  for (i = 0; i < numupdates; i++)
  {
    if (blits[i].type == 'D') 
    {
//       DEBUGCODE
//       {
//         fprintf(stderr, "drawing blits[%d]\n", i);
//         fprintf(stderr, "srcrect->x = %d\t srcrect->y = %d\t srcrect->w = %d\t srcrect->h = %d\n",
//               blits[i].srcrect->x, blits[i].srcrect->y, blits[i].srcrect->w, blits[i].srcrect->h);
//         fprintf(stderr, "dstrect->x = %d\t dstrect->y = %d\t dstrect->w = %d\t dstrect->h = %d\n",
//               blits[i].dstrect->x, blits[i].dstrect->y, blits[i].dstrect->w, blits[i].dstrect->h);
//       } 

      SDL_BlitSurface(blits[i].src, blits[i].srcrect, screen, blits[i].dstrect);
    } 
  }

  LOG("Done drawing\n");

//  SNOW_draw();

  /* -- update the screen only where we need to! -- */
//  if (SNOW_on) 
//    SDL_UpdateRects(screen, SNOW_add( (SDL_Rect*)&dstupdate, numupdates ), SNOW_rects);
//  else 
    T4K_UpdateRect(screen, NULL);

  numupdates = 0;
  *frame = *frame + 1;

  LOG("Leaving UpdateScreen()\n");
}


/* basically puts in an order to overdraw sprite with corresponding */
/* rect of bkgd img                                                 */
int EraseSprite(sprite* img, int x, int y)
{
//  struct blit* update;

  LOG("Entering EraseSprite()\n");

  if( !img 
   || img->cur < 0
   || img->cur > MAX_SPRITE_FRAMES
   || !img->frame[img->cur])
  {
    fprintf(stderr, "EraseSprite() - invalid 'img' arg!\n");
    LOG("Leaving EraseSprite()\n");
    return 0;
  }

  LOG("Leaving EraseSprite()\n");

  return EraseObject(img->frame[img->cur], x, y);
}



/*************************
EraseObject : Erase an object from the screen
**************************/
int EraseObject(SDL_Surface* surf, int x, int y)
{
  struct blit* update = NULL;

  LOG("Entering EraseObject()\n");

  if(!surf)
  {
    fprintf(stderr, "EraseObject() - invalid 'surf' arg!\n");
    return 0;
  }

  if(numupdates >= MAX_UPDATES)
  {
    fprintf(stderr, "Warning - MAX_UPDATES exceeded, cannot add blit to queue\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "EraseObject() - 'update' ptr invalid!\n");
    return 0;
  }

  update->src = CurrentBkgd();

  /* take dimentsions from src surface: */
  update->srcrect->x = x;
  update->srcrect->y = y;
  update->srcrect->w = surf->w;
  update->srcrect->h = surf->h;

  /* NOTE this is needed because the letters may go beyond the size of */
  /* the fish, and we only erase the fish image before we redraw the   */
  /* fish followed by the letter - DSB                                 */
  /* add margin of a few pixels on each side: */
  update->srcrect->x -= ERASE_MARGIN;
  update->srcrect->y -= ERASE_MARGIN;
  update->srcrect->w += (ERASE_MARGIN * 2);
  update->srcrect->h += (ERASE_MARGIN * 2);


  /* Adjust srcrect so it doesn't go past bkgd: */
  if (update->srcrect->x < 0)
  {
    update->srcrect->w += update->srcrect->x; //so right edge stays correct
    update->srcrect->x = 0;
  }
  if (update->srcrect->y < 0)
  {
    update->srcrect->h += update->srcrect->y; //so bottom edge stays correct
    update->srcrect->y = 0;
  }

  if (update->srcrect->x + update->srcrect->w > CurrentBkgd()->w)
    update->srcrect->w = CurrentBkgd()->w - update->srcrect->x;
  if (update->srcrect->y + update->srcrect->h > CurrentBkgd()->h)
    update->srcrect->h = CurrentBkgd()->h - update->srcrect->y;


  update->dstrect->x = update->srcrect->x;
  update->dstrect->y = update->srcrect->y;
  update->dstrect->w = update->srcrect->w;
  update->dstrect->h = update->srcrect->h; 
  update->type = 'E';

  LOG("Leaving EraseObject()\n");

  return 1;
}





/************************************************************************/
/*                                                                      */
/*        Begin text drawing functions                                  */
/*                                                                      */
/* These functions support text drawing using SDL_ttf.                  */
/************************************************************************/


#define MAX_FONT_SIZE 72

/*-- file-scope variables and local file prototypes for SDL_ttf-based code: */
#include <SDL3_ttf/SDL_ttf.h>
/* We cache fonts here once loaded to improve performance: */
static TTF_Font* font_list[MAX_FONT_SIZE + 1] = {NULL};
static void free_font_list(void);
static TTF_Font* get_font(int size);
static TTF_Font* load_font(const char* font_name, int font_size);




/* "Public" functions called from other files: */


int Setup_SDL_Text(void)
{

  LOG("Setup_SDL_Text() - using SDL_ttf\n");

  if (!TTF_Init())
  {
    fprintf(stderr, "\nError: I could not initialize SDL_ttf\n");
    return 0;
  }

  return 1;
}


void Cleanup_SDL_Text(void)
{
  free_font_list();
  TTF_Quit();
}


/* BlackOutline() creates a surface containing text of the designated */
/* foreground color, surrounded by a black shadow, on a transparent    */
/* background.  The appearance can be tuned by adjusting the number of */
/* background copies and the offset where the foreground text is       */
/* finally written (see below).                                        */
//SDL_Surface* BlackOutline(const char *t, TTF_Font *font, SDL_Color *c)
SDL_Surface* BlackOutline(const char* t, int font_size, const SDL_Color* c)
{
  SDL_Surface* out = NULL;
  SDL_Surface* black_letters = NULL;
  SDL_Surface* white_letters = NULL;
  SDL_Surface* bg = NULL;
  SDL_Rect dstrect;
  Uint32 color_key;

/* Make sure everything is sane before we proceed: */
  TTF_Font* font = get_font(font_size);
  if (!font)
  {
    fprintf(stderr, "BlackOutline(): could not load needed font - returning.\n");
    return NULL;
  }

  if (!t || !c)
  {
    fprintf(stderr, "BlackOutline(): invalid ptr parameter, returning.\n");
    return NULL;
  }

  if (t[0] == '\0')
  {
    LOG("BlackOutline(): empty string, returning\n");
    return NULL;
  }

DEBUGCODE
{
  fprintf( stderr, "\nEntering BlackOutline(): \n");
  fprintf( stderr, "BlackOutline of \"%s\"\n", t );
}

  black_letters = TTF_RenderText_Blended(font, t, 0, black);

  if (!black_letters)
  {
    fprintf (stderr, "Warning - BlackOutline() could not create image for %s\n", t);
    return NULL;
  }

  bg = SDL_CreateSurface((black_letters->w) + 5,
                         (black_letters->h) + 5,
                         SDL_PIXELFORMAT_RGBA32);
  /* Use color key for eventual transparency: */
  color_key = SDL_MapRGB(SDL_GetPixelFormatDetails(bg->format), NULL, 01, 01, 01);
  SDL_FillSurfaceRect(bg, NULL, color_key);

  /* Now draw black outline/shadow 2 pixels on each side: */
  dstrect.w = black_letters->w;
  dstrect.h = black_letters->h;

  /* NOTE: can make the "shadow" more or less pronounced by */
  /* changing the parameters of these loops.                */
  for (dstrect.x = 1; dstrect.x < 4; dstrect.x++)
    for (dstrect.y = 1; dstrect.y < 3; dstrect.y++)
      SDL_BlitSurface(black_letters , NULL, bg, &dstrect );

  SDL_DestroySurface(black_letters);

  /* --- Put the color version of the text on top! --- */
  white_letters = TTF_RenderText_Blended(font, t, 0, *c);

  if (!white_letters)
  {
    fprintf (stderr, "Warning - BlackOutline() could not create image for %s\n", t);
    return NULL;
  }

  dstrect.x = 1;
  dstrect.y = 1;
  SDL_BlitSurface(white_letters, NULL, bg, &dstrect);
  SDL_DestroySurface(white_letters);

  /* --- Set up transparency key --- */
  SDL_SetSurfaceColorKey(bg, true, color_key);
  SDL_SetSurfaceRLE(bg, true);
  out = bg;

DEBUGCODE
  { fprintf( stderr, "\nLeaving BlackOutline(): \n"); }


  return out;
}



SDL_Surface* BlackOutline_w(const wchar_t* t, int font_size, const SDL_Color* c, int length)
{
  wchar_t wchar_tmp[1024];
  char tmp[1024];
  int i;

  // Safety checks:
  if (!t || !c)
  {
    fprintf(stderr, "BlackOutline_w(): invalid ptr parameter, returning.\n");
    return NULL;
  }

  if (t[0] == '\0')
  {
    fprintf(stderr, "BlackOutline_w(): empty string, returning\n");
    return NULL;
  }

  wcsncpy(wchar_tmp, t, length);
  wchar_tmp[length] = '\0';

  DEBUGCODE
  {
    fprintf(stderr, "In BlackOutline_w() - input wchar_t string is: %S\n", wchar_tmp);
  }

  i = ConvertToUTF8(wchar_tmp, tmp, 1024);
  //tmp[i] = 0;

  DEBUGCODE
  {
    fprintf(stderr, "In BlackOutline_w() - converted UTF8 string is: %s\n", tmp);
  }

  return BlackOutline(tmp, font_size, c);
}

/* This (fast) function just returns a non-outlined surf */
SDL_Surface* SimpleText(const char *t, int size, const SDL_Color* col)
{
  SDL_Surface* surf = NULL;

  if (!t||!col)
    return NULL;

  {
    TTF_Font* font = get_font(size);
    if (!font)
      return NULL;
    surf = TTF_RenderText_Blended(font, t, 0, *col);
  }

  return surf;
}


/*-----------------------------------------------------------*/
/* Local functions, callable only within SDL_extras, divided */
/* according with which text lib we are using:               */
/*-----------------------------------------------------------*/



/* Local functions when using SDL_ttf: */

static void free_font_list(void)
{
  int i;
  for(i = 0; i < MAX_FONT_SIZE; i++)
  {
    if(font_list[i])
    {
      TTF_CloseFont(font_list[i]);
      font_list[i] = NULL;
    }
  }
}



/* Loads and caches fonts in each size as they are requested: */
/* We use the font size as an array index, keeping each size  */
/* font in memory once loaded until cleanup.                  */
static TTF_Font* get_font(int size)
{
  static char prev_font_name[FNLEN];

  if (size < 0)
  {
    fprintf(stderr, "Error - requested font size %d is negative\n", size);
    return NULL;
  }

  if (size > MAX_FONT_SIZE)
  {
    fprintf(stderr, "Error - requested font size %d exceeds max = %d, resetting.\n",
            size, MAX_FONT_SIZE);
    size = MAX_FONT_SIZE;
  }

  /* If the font has changed, we need to wipe out the old ones: */
  if (0 != strncmp(prev_font_name, settings.theme_font_name, FNLEN))
  {
    free_font_list();
    strncpy(prev_font_name, settings.theme_font_name, sizeof(prev_font_name));
  }

  /* If we can't load the font, this will return NULL: */
  if(font_list[size] == NULL)
    font_list[size] = load_font(settings.theme_font_name, size);
  return font_list[size];
}


/* FIXME need code to search for font paths on different platforms */
static TTF_Font* load_font(const char* font_name, int font_size)
{
  TTF_Font* loaded_font = NULL;
  char fn[FNLEN];

  /* try to find font in default data dir: */
  sprintf(fn, "%s/fonts/%s", settings.default_data_path, font_name);

  DEBUGCODE { fprintf(stderr, "load_font(): looking for %s using bundled data paths\n", fn); }

  /* try to load the font, if successful, return font*/
  loaded_font = TTF_OpenFont(fn, font_size);
  if (loaded_font != NULL)
  {
    DEBUGCODE { fprintf(stderr, "load_font(): found bundled font: %s\n", fn); }
    return loaded_font;
  }

		

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



  DEBUGCODE { fprintf(stderr, "load_font(): looking for %s\n in OS' font path\n", fn); }

  /* try to load the font, if successful, return font*/
  loaded_font = TTF_OpenFont(fn, font_size);
  if (loaded_font != NULL)
  {
    DEBUGCODE { fprintf(stderr, "load_font(): found font in OS' location: %s\n", fn); }
    return loaded_font;
  }
  /* We could not find desired font. If we were looking for something other  */
  /* than default (Andika) font, print warning and try to load default font: */
  if (strncmp(font_name, DEFAULT_FONT_NAME, FNLEN ) != 0)
  {
    fprintf(stderr, "Warning - could not load desired font: %s\n", font_name);
    fprintf(stderr, "Trying to load default instead: %s\n", DEFAULT_FONT_NAME);
    return load_font(DEFAULT_FONT_NAME, font_size);
  }
  else  /* Default failed also - bummer! */
  {
    fprintf(stderr, "LoadFont(): Error - couldn't load either selected or default font\n");
    return NULL;
  }
}

