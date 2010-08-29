/*
   SDL_extras.h:

   Headers for wrapper and utility functions for use with the
   SDL libraries.
   
   Copyright 2007, 2008, 2009, 2010.
   Authors: David Bruce, Tim Holy.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   SDL_extras.h is part of Tux Typing, a.k.a "tuxtype".

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



#ifndef SDL_EXTRAS_H
#define SDL_EXTRAS_H

// Need this so the #ifdef HAVE_LIBSDL_PANGO will work:
#include "../config.h"

#include "SDL.h"


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif

/* the colors we use throughout the game */
static const SDL_Color black 		= {0x00, 0x00, 0x00, 0x00};
static const SDL_Color gray 		= {0x80, 0x80, 0x80, 0x00};
static const SDL_Color dark_blue	= {0x00, 0x00, 0x60, 0x00};
static const SDL_Color red 		= {0xff, 0x00, 0x00, 0x00};
static const SDL_Color white 		= {0xff, 0xff, 0xff, 0x00};
static const SDL_Color yellow 		= {0xff, 0xff, 0x00, 0x00};



/* FIXME get rid of these 'evil' macros */
#define NEXT_FRAME(SPRITE) if ((SPRITE)->num_frames) (SPRITE)->cur = (((SPRITE)->cur)+1) % (SPRITE)->num_frames;
#define REWIND(SPRITE) (SPRITE)->cur = 0;
#define MAX_SPRITE_FRAMES 30
//extra bkgd border around "erased" images.
#define ERASE_MARGIN 5


typedef struct {
  SDL_Surface* frame[MAX_SPRITE_FRAMES];
  SDL_Surface* default_img;
  int num_frames;
  int cur;
} sprite;


/* "Public" function prototypes: */
void DrawButton(SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void RoundCorners(SDL_Surface* s, Uint16 radius);
SDL_Surface* Flip(SDL_Surface *in, int x, int y);
int  inRect(SDL_Rect r, int x, int y);
void DarkenScreen(Uint8 bits);
void SwitchScreenMode(void);
int WaitForKeypress(void);
SDL_Surface* Blend(SDL_Surface *S1, SDL_Surface *S2, float gamma);
SDL_Surface* zoom(SDL_Surface * src, int new_w, int new_h);
int TransWipe(const SDL_Surface* newbkg, int type, int segments, int duration);

/* Blit queue functions: */
void InitBlitQueue(void);
void ResetBlitQueue(void);
int AddRect(SDL_Rect* src, SDL_Rect* dst);
int DrawObject(SDL_Surface* surf, int x, int y);
int DrawSprite(sprite* gfx, int x, int y);
int EraseObject(SDL_Surface* surf, int x, int y);
int EraseSprite(sprite* img, int x, int y);
void UpdateScreen(int* frame);

/*Text rendering functions: */
int Setup_SDL_Text(void);
void Cleanup_SDL_Text(void);
SDL_Surface* BlackOutline(const char* t, int font_size, const SDL_Color* c);
SDL_Surface* BlackOutline_w(const wchar_t* t, int font_size, const SDL_Color* c, int length);
SDL_Surface* SimpleText(const char *t, int size, const SDL_Color* col);
//SDL_Surface* SimpleTextWithOffset(const char *t, int size, SDL_Color* col, int *glyph_offset);

#endif
