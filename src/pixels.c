/*
   pixels.c

   For Tux Paint
   Pixel read/write functions

   bill@newbreedsoftware.com
   http://www.newbreedsoftware.com/tuxpaint/

   Brought to tuxmath/tuxtype, slight modified, and license updated to 
   "GPL3 or later" as detailed below.
   
   Copyright 2007, 2008, 2009, 2010.
   Copyright 2002-2006, 2007, 2008, 2009, 2010.
   Authors: Bill Kendrick and others from Tux Paint,
            David Bruce, Brendan Luchen.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   pixels.c is part of Tux Typing, a.k.a "tuxtype".

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



#include "pixels.h"
#include "compiler.h"
//#include "debug.h"

/* Draw a single pixel into the surface: */
void putpixel8(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  Uint8 *p;

  /* Assuming the X/Y values are within the bounds of this surface... */
  if (likely
      (likely((unsigned) x < (unsigned) surface->w)
       && likely((unsigned) y < (unsigned) surface->h)))
  {
    // Set a pointer to the exact location in memory of the pixel
    p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start: beginning of RAM */
                   (y * surface->pitch) +        /* Go down Y lines */
                   x);                /* Go in X pixels */


    /* Set the (correctly-sized) piece of data in the surface's RAM
     *          to the pixel value sent in: */

    *p = pixel;
  }
}

/* Draw a single pixel into the surface: */
void putpixel16(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  Uint8 *p;

  /* Assuming the X/Y values are within the bounds of this surface... */
  if (likely
      (likely((unsigned) x < (unsigned) surface->w)
       && likely((unsigned) y < (unsigned) surface->h)))
  {
    // Set a pointer to the exact location in memory of the pixel
    p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start: beginning of RAM */
                   (y * surface->pitch) +        /* Go down Y lines */
                   (x * 2));        /* Go in X pixels */


    /* Set the (correctly-sized) piece of data in the surface's RAM
     *          to the pixel value sent in: */

    *(Uint16 *) p = pixel;
  }
}

/* Draw a single pixel into the surface: */
void putpixel24(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  Uint8 *p;

  /* Assuming the X/Y values are within the bounds of this surface... */
  if (likely
      (likely((unsigned) x < (unsigned) surface->w)
       && likely((unsigned) y < (unsigned) surface->h)))
  {
    // Set a pointer to the exact location in memory of the pixel
    p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start: beginning of RAM */
                   (y * surface->pitch) +        /* Go down Y lines */
                   (x * 3));        /* Go in X pixels */


    /* Set the (correctly-sized) piece of data in the surface's RAM
     *          to the pixel value sent in: */

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    }
    else
    {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }

  }
}

/* Draw a single pixel into the surface: */
void putpixel32(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  Uint8 *p;

  /* Assuming the X/Y values are within the bounds of this surface... */
  if (likely
      (likely((unsigned) x < (unsigned) surface->w)
       && likely((unsigned) y < (unsigned) surface->h)))
  {
    // Set a pointer to the exact location in memory of the pixel
    p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start: beginning of RAM */
                   (y * surface->pitch) +        /* Go down Y lines */
                   (x * 4));        /* Go in X pixels */


    /* Set the (correctly-sized) piece of data in the surface's RAM
     *          to the pixel value sent in: */

    *(Uint32 *) p = pixel;        // 32-bit display
  }
}

/* Get a pixel: */
Uint32 getpixel8(SDL_Surface * surface, int x, int y)
{
  Uint8 *p;

  /* get the X/Y values within the bounds of this surface */
  if (unlikely((unsigned) x > (unsigned) surface->w - 1u))
    x = (x < 0) ? 0 : surface->w - 1;
  if (unlikely((unsigned) y > (unsigned) surface->h - 1u))
    y = (y < 0) ? 0 : surface->h - 1;

  /* Set a pointer to the exact location in memory of the pixel
     in question: */

  p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start at top of RAM */
                 (y * surface->pitch) +        /* Go down Y lines */
                 x);                /* Go in X pixels */


  /* Return the correctly-sized piece of data containing the
   * pixel's value (an 8-bit palette value, or a 16-, 24- or 32-bit
   * RGB value) */

  return (*p);
}

/* Get a pixel: */
Uint32 getpixel16(SDL_Surface * surface, int x, int y)
{
  Uint8 *p;

  /* get the X/Y values within the bounds of this surface */
  if (unlikely((unsigned) x > (unsigned) surface->w - 1u))
    x = (x < 0) ? 0 : surface->w - 1;
  if (unlikely((unsigned) y > (unsigned) surface->h - 1u))
    y = (y < 0) ? 0 : surface->h - 1;

  /* Set a pointer to the exact location in memory of the pixel
     in question: */

  p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start at top of RAM */
                 (y * surface->pitch) +        /* Go down Y lines */
                 (x * 2));        /* Go in X pixels */


  /* Return the correctly-sized piece of data containing the
   * pixel's value (an 8-bit palette value, or a 16-, 24- or 32-bit
   * RGB value) */

  return (*(Uint16 *) p);
}

/* Get a pixel: */
Uint32 getpixel24(SDL_Surface * surface, int x, int y)
{
  Uint8 *p;
  Uint32 pixel;

  /* get the X/Y values within the bounds of this surface */
  if (unlikely((unsigned) x > (unsigned) surface->w - 1u))
    x = (x < 0) ? 0 : surface->w - 1;
  if (unlikely((unsigned) y > (unsigned) surface->h - 1u))
    y = (y < 0) ? 0 : surface->h - 1;

  /* Set a pointer to the exact location in memory of the pixel
     in question: */

  p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start at top of RAM */
                 (y * surface->pitch) +        /* Go down Y lines */
                 (x * 3));        /* Go in X pixels */


  /* Return the correctly-sized piece of data containing the
   * pixel's value (an 8-bit palette value, or a 16-, 24- or 32-bit
   * RGB value) */

  /* Depending on the byte-order, it could be stored RGB or BGR! */

  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    pixel = p[0] << 16 | p[1] << 8 | p[2];
  else
    pixel = p[0] | p[1] << 8 | p[2] << 16;

  return pixel;
}

/* Get a pixel: */
Uint32 getpixel32(SDL_Surface * surface, int x, int y)
{
  Uint8 *p;

  /* get the X/Y values within the bounds of this surface */
  if (unlikely((unsigned) x > (unsigned) surface->w - 1u))
    x = (x < 0) ? 0 : surface->w - 1;
  if (unlikely((unsigned) y > (unsigned) surface->h - 1u))
    y = (y < 0) ? 0 : surface->h - 1;

  /* Set a pointer to the exact location in memory of the pixel
     in question: */

  p = (Uint8 *) (((Uint8 *) surface->pixels) +        /* Start at top of RAM */
                 (y * surface->pitch) +        /* Go down Y lines */
                 (x * 4));        /* Go in X pixels */


  /* Return the correctly-sized piece of data containing the
   * pixel's value (an 8-bit palette value, or a 16-, 24- or 32-bit
   * RGB value) */

  return *(Uint32 *) p;                // 32-bit display
}

void (*putpixels[]) (SDL_Surface *, int, int, Uint32) =
{
putpixel8, putpixel8, putpixel16, putpixel24, putpixel32};


Uint32(*getpixels[])(SDL_Surface *, int, int) =
{
getpixel8, getpixel8, getpixel16, getpixel24, getpixel32};
