/*
   pixels.h

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

   pixels.h is part of Tux Typing, a.k.a "tuxtype".

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




#ifndef PIXELS_H
#define PIXELS_H

#include "SDL.h"

void putpixel8(SDL_Surface * surface, int x, int y, Uint32 pixel);
void putpixel16(SDL_Surface * surface, int x, int y, Uint32 pixel);
void putpixel24(SDL_Surface * surface, int x, int y, Uint32 pixel);
void putpixel32(SDL_Surface * surface, int x, int y, Uint32 pixel);

extern void (*putpixels[]) (SDL_Surface *, int, int, Uint32);

Uint32 getpixel8(SDL_Surface * surface, int x, int y);
Uint32 getpixel16(SDL_Surface * surface, int x, int y);
Uint32 getpixel24(SDL_Surface * surface, int x, int y);
Uint32 getpixel32(SDL_Surface * surface, int x, int y);

extern Uint32(*getpixels[]) (SDL_Surface *, int, int);

#endif
