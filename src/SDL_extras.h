//
// C Interface: SDL_extras
//
// Description: A few handy functions for using SDL graphics.
//
//
// Author: David Bruce,,, <dbruce@tampabay.rr.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
// (briefly, GPL v3 or later).
//

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

/* "Public" function prototypes: */
void DrawButton(SDL_Rect* target_rect, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void RoundCorners(SDL_Surface* s, Uint16 radius);
SDL_Surface* Flip(SDL_Surface *in, int x, int y);

int  inRect(SDL_Rect r, int x, int y);
void DarkenScreen(Uint8 bits);
void SwitchScreenMode(void);
int WaitForKeypress(void);
SDL_Surface* Blend(SDL_Surface *S1, SDL_Surface *S2,float gamma);
SDL_Surface* zoom(SDL_Surface * src, int new_w, int new_h);

/*Text rendering functions: */
int Setup_SDL_Text(void);
void Cleanup_SDL_Text(void);
SDL_Surface* BlackOutline(const char* t, int font_size, const SDL_Color* c);
SDL_Surface* BlackOutline_w(const wchar_t* t, int font_size, const SDL_Color* c, int length);
SDL_Surface* SimpleText(const char *t, int size, SDL_Color* col);
//SDL_Surface* SimpleTextWithOffset(const char *t, int size, SDL_Color* col, int *glyph_offset);

#endif
