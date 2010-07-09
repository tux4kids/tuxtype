//
// C Interface: SDL_extras
//
// Description: A few handy functions for using SDL graphics.
//
//
// Author: David Bruce,,, <davidstuartbruce@gmail.com>, (C) 2007-9
//
// Copyright: See COPYING file that comes with this distribution
// (briefly, GPL v3 or later).
//

#ifndef SDL_EXTRAS_H
#define SDL_EXTRAS_H

// Need this so the #ifdef HAVE_LIBSDL_PANGO will work:
#include "../config.h"

#include "SDL.h"

#if HAVE_LIBT4K_COMMON
# include <t4k_common.h>
#else
# if SDL_BYTEORDER == SDL_BIG_ENDIAN
# define rmask 0xff000000
# define gmask 0x00ff0000
# define bmask 0x0000ff00
# define amask 0x000000ff
# else
# define rmask 0x000000ff
# define gmask 0x0000ff00
# define bmask 0x00ff0000
# define amask 0xff000000
# endif

typedef struct {
  SDL_Surface* frame[MAX_SPRITE_FRAMES];
  SDL_Surface* default_img;
  int num_frames;
  int cur;
} sprite;

/* "Public" function prototypes: */
void RoundCorners(SDL_Surface* s, Uint16 radius);
SDL_Surface* Flip(SDL_Surface *in, int x, int y);
int  inRect(SDL_Rect r, int x, int y);
void DarkenScreen(Uint8 bits);
void SwitchScreenMode(void);
int WaitForKeypress(void);
SDL_Surface* Blend(SDL_Surface *S1, SDL_Surface *S2, float gamma);
SDL_Surface* zoom(SDL_Surface * src, int new_w, int new_h);
int TransWipe(const SDL_Surface* newbkg, int type, int segments, int duration);

/*Text rendering functions: */
int Setup_SDL_Text(void);
void Cleanup_SDL_Text(void);
SDL_Surface* BlackOutline(const char* t, int font_size, const SDL_Color* c);
SDL_Surface* BlackOutline_w(const wchar_t* t, int font_size, const SDL_Color* c, int length);
//SDL_Surface* SimpleTextWithOffset(const char *t, int size, SDL_Color* col, int *glyph_offset);

/* Blit queue functions: */
void InitBlitQueue(void);
void ResetBlitQueue(void);
int AddRect(SDL_Rect* src, SDL_Rect* dst);
int DrawSprite(sprite* gfx, int x, int y);
int EraseSprite(sprite* img, int x, int y);
int EraseObject(SDL_Surface* surf, int x, int y);
void UpdateScreen(int* frame);
#endif //HAVE_LIBT4K_COMMON





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


#endif
