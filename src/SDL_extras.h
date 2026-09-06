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

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

typedef MIX_Audio Mix_Chunk;
typedef MIX_Audio Mix_Music;

#ifdef SDL_FreeSurface
#undef SDL_FreeSurface
#endif
#define SDL_FreeSurface SDL_DestroySurface

#ifdef SDL_FillRect
#undef SDL_FillRect
#endif
#define SDL_FillRect SDL_FillSurfaceRect

#ifdef SDL_EVENT_QUIT
#undef SDL_EVENT_QUIT
#endif
#define SDL_EVENT_QUIT SDL_EVENT_QUIT

#ifdef SDL_EVENT_KEY_DOWN
#undef SDL_EVENT_KEY_DOWN
#endif
#define SDL_EVENT_KEY_DOWN SDL_EVENT_KEY_DOWN

#ifdef SDL_EVENT_KEY_UP
#undef SDL_EVENT_KEY_UP
#endif
#define SDL_EVENT_KEY_UP SDL_EVENT_KEY_UP

#ifdef SDL_EVENT_MOUSE_BUTTON_DOWN
#undef SDL_EVENT_MOUSE_BUTTON_DOWN
#endif
#define SDL_EVENT_MOUSE_BUTTON_DOWN SDL_EVENT_MOUSE_BUTTON_DOWN

#ifdef SDL_EVENT_MOUSE_BUTTON_UP
#undef SDL_EVENT_MOUSE_BUTTON_UP
#endif
#define SDL_EVENT_MOUSE_BUTTON_UP SDL_EVENT_MOUSE_BUTTON_UP

#ifdef SDL_EVENT_MOUSE_MOTION
#undef SDL_EVENT_MOUSE_MOTION
#endif
#define SDL_EVENT_MOUSE_MOTION SDL_EVENT_MOUSE_MOTION

typedef struct SDL_keysym {
    SDL_Keycode sym;
    SDL_Keymod mod;
    Uint16 unicode;
    Uint8 scancode;
} SDL_keysym;

#ifndef SDLK_NUMLOCK
#define SDLK_NUMLOCK SDLK_NUMLOCKCLEAR
#endif
#ifndef SDLK_SCROLLOCK
#define SDLK_SCROLLOCK SDLK_SCROLLLOCK
#endif
#ifndef SDLK_LSUPER
#define SDLK_LSUPER SDLK_LGUI
#endif
#ifndef SDLK_RSUPER
#define SDLK_RSUPER SDLK_RGUI
#endif
#ifndef SDLK_COMPOSE
#define SDLK_COMPOSE SDLK_APPLICATION
#endif

#ifdef SDLK_BACKQUOTE
#undef SDLK_BACKQUOTE
#endif
#define SDLK_BACKQUOTE SDLK_GRAVE

#ifdef SDLK_QUOTE
#undef SDLK_QUOTE
#endif
#define SDLK_QUOTE SDLK_APOSTROPHE

#ifdef SDLK_a
#undef SDLK_a
#undef SDLK_b
#undef SDLK_c
#undef SDLK_d
#undef SDLK_e
#undef SDLK_f
#undef SDLK_g
#undef SDLK_h
#undef SDLK_i
#undef SDLK_j
#undef SDLK_k
#undef SDLK_l
#undef SDLK_m
#undef SDLK_n
#undef SDLK_o
#undef SDLK_p
#undef SDLK_q
#undef SDLK_r
#undef SDLK_s
#undef SDLK_t
#undef SDLK_u
#undef SDLK_v
#undef SDLK_w
#undef SDLK_x
#undef SDLK_y
#undef SDLK_z
#endif

#define SDLK_a SDLK_A
#define SDLK_b SDLK_B
#define SDLK_c SDLK_C
#define SDLK_d SDLK_D
#define SDLK_e SDLK_E
#define SDLK_f SDLK_F
#define SDLK_g SDLK_G
#define SDLK_h SDLK_H
#define SDLK_i SDLK_I
#define SDLK_j SDLK_J
#define SDLK_k SDLK_K
#define SDLK_l SDLK_L
#define SDLK_m SDLK_M
#define SDLK_n SDLK_N
#define SDLK_o SDLK_O
#define SDLK_p SDLK_P
#define SDLK_q SDLK_Q
#define SDLK_r SDLK_R
#define SDLK_s SDLK_S
#define SDLK_t SDLK_T
#define SDLK_u SDLK_U
#define SDLK_v SDLK_V
#define SDLK_w SDLK_W
#define SDLK_x SDLK_X
#define SDLK_y SDLK_Y
#define SDLK_z SDLK_Z


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

typedef SDL_Keycode SDLKey;

static inline bool T4K_ShowCursor(int toggle) {
    if (toggle) return SDL_ShowCursor();
    else return SDL_HideCursor();
}
#ifdef SDL_ShowCursor
#undef SDL_ShowCursor
#endif
#define SDL_ShowCursor(toggle) T4K_ShowCursor(toggle)

static inline SDL_MouseButtonFlags T4K_GetMouseStateInt(int *x, int *y) {
    float fx = 0, fy = 0;
    SDL_MouseButtonFlags res = SDL_GetMouseState(&fx, &fy);
    if (x) *x = (int)fx;
    if (y) *y = (int)fy;
    return res;
}
#ifdef SDL_GetMouseState
#undef SDL_GetMouseState
#endif
#define SDL_GetMouseState(x, y) T4K_GetMouseStateInt((int*)(x), (int*)(y))

#ifndef SDL_WarpMouse
#define SDL_WarpMouse(x, y) SDL_WarpMouseInWindow(T4K_GetWindow(), (float)(x), (float)(y))
#endif

#ifndef SDL_EnableKeyRepeat
#define SDL_EnableKeyRepeat(delay, interval) (void)0
#endif
#ifndef SDL_DEFAULT_REPEAT_INTERVAL
#define SDL_DEFAULT_REPEAT_INTERVAL 30
#endif
#ifndef SDL_ENABLE
#define SDL_ENABLE 1
#endif
#ifndef SDL_DISABLE
#define SDL_DISABLE 0
#endif
#ifndef SDL_EnableUNICODE
#define SDL_EnableUNICODE(enable) (void)0
#endif
#ifdef SDL_LowerBlit
#undef SDL_LowerBlit
#endif
#define SDL_LowerBlit SDL_BlitSurfaceUnchecked

#ifdef TTF_RenderUTF8_Blended
#undef TTF_RenderUTF8_Blended
#endif
#define TTF_RenderUTF8_Blended(font, text, fg) TTF_RenderText_Blended(font, text, 0, fg)

#ifndef Mix_PlayingMusic
#define Mix_PlayingMusic T4K_IsPlayingMusic
#endif
#ifndef Mix_HaltMusic
#define Mix_HaltMusic T4K_AudioMusicUnload
#endif
#ifndef Mix_FadeOutMusic
#define Mix_FadeOutMusic(ms) T4K_AudioMusicUnload()
#endif
#ifndef Mix_Playing
#define Mix_Playing(ch) 0
#endif
#ifndef Mix_HaltChannel
#define Mix_HaltChannel(ch) T4K_AudioHaltChannel(ch)
#endif
#ifndef Mix_Pause
#define Mix_Pause(ch) (void)(ch)
#endif
#ifndef Mix_Resume
#define Mix_Resume(ch) (void)(ch)
#endif
#ifndef Mix_PlayChannel
#define Mix_PlayChannel(ch, chunk, loops) T4K_PlaySoundLoop(chunk, loops)
#endif
#ifndef Mix_FreeChunk
#define Mix_FreeChunk(chunk) (void)(chunk)
#endif
#ifndef MIX_MAX_VOLUME
#define MIX_MAX_VOLUME 128
#endif

static inline const SDL_PixelFormatDetails* T4K_GetDetails(const void* fmt) {
    if ((uintptr_t)fmt < 0x10000000) {
        return SDL_GetPixelFormatDetails((SDL_PixelFormat)(uintptr_t)fmt);
    }
    return (const SDL_PixelFormatDetails*)fmt;
}

#ifdef SDL_MapRGB
#undef SDL_MapRGB
#endif
#define SDL_MapRGB(fmt, r, g, b) SDL_MapRGB(T4K_GetDetails((const void*)(uintptr_t)(fmt)), NULL, (r), (g), (b))

#ifdef SDL_MapRGBA
#undef SDL_MapRGBA
#endif
#define SDL_MapRGBA(fmt, r, g, b, a) SDL_MapRGBA(T4K_GetDetails((const void*)(uintptr_t)(fmt)), NULL, (r), (g), (b), (a))

#ifdef SDL_GetRGB
#undef SDL_GetRGB
#endif
#define SDL_GetRGB(pixel, fmt, r, g, b) SDL_GetRGB((pixel), T4K_GetDetails((const void*)(uintptr_t)(fmt)), NULL, (r), (g), (b))

#ifdef SDL_GetRGBA
#undef SDL_GetRGBA
#endif
#define SDL_GetRGBA(pixel, fmt, r, g, b, a) SDL_GetRGBA((pixel), T4K_GetDetails((const void*)(uintptr_t)(fmt)), NULL, (r), (g), (b), (a))

#ifdef SDL_GetColorKey
#undef SDL_GetColorKey
#endif
#define SDL_GetColorKey SDL_GetSurfaceColorKey

#ifdef SDL_SetColorKey
#undef SDL_SetColorKey
#endif
#define SDL_SetColorKey SDL_SetSurfaceColorKey

#ifndef Mix_LoadWAV
#define Mix_LoadWAV(fn) MIX_LoadAudio(T4K_GetAudioMixer(), fn, true)
#endif
#ifndef Mix_LoadMUS
#define Mix_LoadMUS(fn) MIX_LoadAudio(T4K_GetAudioMixer(), fn, true)
#endif
int Mix_VolumeMusic(int vol);
int Mix_Volume(int channel, int vol);

#ifndef SDL_DisplayFormat
#define SDL_DisplayFormat(surf) SDL_ConvertSurface(surf, SDL_PIXELFORMAT_RGBA8888)
#endif
#ifndef SDL_DisplayFormatAlpha
#define SDL_DisplayFormatAlpha(surf) SDL_ConvertSurface(surf, SDL_PIXELFORMAT_RGBA8888)
#endif
#ifndef SDL_CreateRGBSurface
#define SDL_CreateRGBSurface(flags, w, h, depth, rmask, gmask, bmask, amask) SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888)
#endif

#ifdef SDL_SetColorKey
#undef SDL_SetColorKey
#endif
#define SDL_SetColorKey(surf, flag, key) SDL_SetSurfaceColorKey(surf, true, key)

#ifndef SDL_SRCCOLORKEY
#define SDL_SRCCOLORKEY 1
#endif
#ifndef SDL_RLEACCEL
#define SDL_RLEACCEL 1
#endif

#ifndef T4K_Tts_wait
#define T4K_Tts_wait() SDL_Delay(50)
#endif

/* FIXME get rid of these 'evil' macros */
#ifndef NEXT_FRAME
#define NEXT_FRAME(SPRITE) if ((SPRITE)->num_frames) (SPRITE)->cur = (((SPRITE)->cur)+1) % (SPRITE)->num_frames;
#endif
#ifndef REWIND
#define REWIND(SPRITE) (SPRITE)->cur = 0;
#endif
#ifndef ERASE_MARGIN
#define ERASE_MARGIN 5
#endif

#ifndef TUX4KIDS_COMMON_H
/* the colors we use throughout the game */
static const SDL_Color black 		= {0x00, 0x00, 0x00, 0x00};
static const SDL_Color gray 		= {0x80, 0x80, 0x80, 0x00};
static const SDL_Color dark_blue	= {0x00, 0x00, 0x60, 0x00};
static const SDL_Color red 		= {0xff, 0x00, 0x00, 0x00};
static const SDL_Color white 		= {0xff, 0xff, 0xff, 0x00};
static const SDL_Color yellow 		= {0xff, 0xff, 0x00, 0x00};

#define MAX_SPRITE_FRAMES 30

typedef struct {
  SDL_Surface* frame[MAX_SPRITE_FRAMES];
  SDL_Surface* default_img;
  int num_frames;
  int cur;
} sprite;
#endif


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
int TransWipe(SDL_Surface* newbkg, int type, int segments, int duration);

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
