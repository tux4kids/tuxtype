/*
   pause.c:
   

   Supplies pause screen feature.
   
   Copyright 2003, 2010.
   Authors: Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   pause.c is part of Tux Typing, a.k.a "tuxtype".

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

enum {
	SEL_SFX = 0,
	SEL_MUSIC,
	SEL_SPEECH_RATE,
	NUM_SELECTIONS
};

static Mix_Chunk *pause_sfx = NULL;
static SDL_Surface *up = NULL, *down = NULL, *left = NULL, *right = NULL;
static SDL_Surface *pause_bkg = NULL;
static SDL_Rect rectUp, rectDown, rectLeft, rectRight;
/* rects for the speech-rate row +/- buttons and bar hit-test */
static SDL_Rect rectSrateUp, rectSrateDown_btn, rectSrateDown;
const int pause_font_size1 = 24;
const int pause_font_size2 = 36;


/* Local function prototypes: */
static void draw_vols(int sfx, int mus, int tts_rate, int selected_opt);
static void pause_draw(int tts_rate, int selected_opt);
static void pause_load_media(void);
static void pause_unload_media(void);

// QUESTION: For usability sake, should escape return to the game
//           and the user have to choose to quit the game, or ???
/**********************
Pause : Pause the game
***********************/
int Pause(void)
{
	int paused = 1;
	int sfx_volume=0;
	int old_sfx_volume;
	int mus_volume=0;
	int old_mus_volume;
	int tts_rate=0;
	int old_tts_rate;
	int selected_opt = SEL_SFX;
	int mousePressed = 0;
	int quit=0;
	int tocks=0;  // used for keeping track of when a tock has happened
	SDL_Event event;

	LOG( "Entering Pause()\n" );

	pause_load_media();
	/* --- stop all sounds, play pause noise --- */

	if (settings.sys_sound) {
 		Mix_Pause(-1);
		Mix_PlayChannel(-1, pause_sfx, 0);
		sfx_volume = Mix_Volume(-1, -1);  // get sfx volume w/o changing it
		mus_volume = Mix_VolumeMusic(-1); // get mus volume w/o changing it
	}
	tts_rate = settings.tts_rate; /* current speech rate level (0-4) */
	if (tts_rate < 0) tts_rate = 0;
	if (tts_rate > 4) tts_rate = 4;

	/* --- show the pause screen --- */

	SDL_ShowCursor(1);

	// Darken the screen...
	DarkenScreen(1); 
	if (pause_bkg) {
		SDL_FreeSurface(pause_bkg);
	}
	pause_bkg = SDL_DuplicateSurface(screen);
	if (pause_bkg) {
		SDL_SetSurfaceBlendMode(pause_bkg, SDL_BLENDMODE_NONE);
	}

	pause_draw(tts_rate, selected_opt);

	if (settings.sys_sound) {
		draw_vols(sfx_volume, mus_volume, tts_rate, selected_opt);
	}

	T4K_PresentScreen();

	SDL_EnableKeyRepeat( 1, 20 );

	/* --- wait for space, click, or exit --- */

	while (paused) {
		old_sfx_volume = sfx_volume;
		old_mus_volume = mus_volume;
		old_tts_rate   = tts_rate;
		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_EVENT_QUIT: 
					exit(0);
					break;
				case SDL_EVENT_KEY_UP:
					if (settings.sys_sound && 
					   ((event.key.key == SDLK_RIGHT) ||
					    (event.key.key == SDLK_LEFT))) 
					    	tocks = 0;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (event.key.key == SDLK_SPACE) 
						paused = 0;
					if (event.key.key == SDLK_ESCAPE) {
						paused = 0;
						quit = 1;
					}
					if (event.key.key == SDLK_TAB) {
						if (SDL_GetModState() & SDL_KMOD_SHIFT)
							selected_opt = (selected_opt + NUM_SELECTIONS - 1) % NUM_SELECTIONS;
						else
							selected_opt = (selected_opt + 1) % NUM_SELECTIONS;

						if (settings.sys_sound) {
							if (selected_opt == SEL_SFX)
								T4K_Tts_say(DEFAULT_VALUE, DEFAULT_VALUE, INTERRUPT, _("Sound effects volume"));
							else if (selected_opt == SEL_MUSIC)
								T4K_Tts_say(DEFAULT_VALUE, DEFAULT_VALUE, INTERRUPT, _("Music volume"));
							else if (selected_opt == SEL_SPEECH_RATE)
								T4K_Tts_say(DEFAULT_VALUE, DEFAULT_VALUE, INTERRUPT, _("Speech rate %s"), get_speech_rate_label(tts_rate));
						}
						pause_draw(tts_rate, selected_opt);
						if (settings.sys_sound)
							draw_vols(sfx_volume, mus_volume, tts_rate, selected_opt);
						T4K_PresentScreen();
					}
					if (settings.sys_sound) { 
						/* Adjust whichever item is currently focused via Tab */
						if (selected_opt == SEL_SFX) {
							if (event.key.key == SDLK_RIGHT || event.key.key == SDLK_UP) 
								sfx_volume += 4;
							if (event.key.key == SDLK_LEFT || event.key.key == SDLK_DOWN) 
								sfx_volume -= 4;
						} else if (selected_opt == SEL_MUSIC) {
							if (event.key.key == SDLK_RIGHT || event.key.key == SDLK_UP) 
								mus_volume += 4;
							if (event.key.key == SDLK_LEFT || event.key.key == SDLK_DOWN) 
								mus_volume -= 4;
						} else if (selected_opt == SEL_SPEECH_RATE) {
							if (event.key.key == SDLK_RIGHT || event.key.key == SDLK_UP || event.key.key == SDLK_PAGEUP) 
								tts_rate++;
							if (event.key.key == SDLK_LEFT || event.key.key == SDLK_DOWN || event.key.key == SDLK_PAGEDOWN) 
								tts_rate--;
						}
						/* Dedicated PageUp/PageDown can always adjust speech rate */
						if (selected_opt != SEL_SPEECH_RATE) {
							if (event.key.key == SDLK_PAGEUP)
								tts_rate++;
							if (event.key.key == SDLK_PAGEDOWN)
								tts_rate--;
						}
					}
					if (event.key.key == SDLK_F5)
						ToggleTTS();
					if (event.key.key == SDLK_F9)
						ToggleBraille();
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					mousePressed = 1;
					tocks = 0;
					break;
				case SDL_EVENT_MOUSE_BUTTON_UP:
					mousePressed = 0;
					break;
			}
		if (settings.sys_sound && mousePressed) {
			int x, y;

			SDL_GetMouseState(&x, &y);
			/* check to see if they clicked on a button */

			if (inRect(rectUp, x, y)) {
				mus_volume += 4;
				selected_opt = SEL_MUSIC;
			} else if (inRect(rectDown, x, y)) {
				mus_volume -= 4;
				selected_opt = SEL_MUSIC;
			} else if (inRect(rectRight, x, y)) {
				sfx_volume += 4;
				selected_opt = SEL_SFX;
			} else if (inRect(rectLeft, x, y)) {
				sfx_volume -= 4;
				selected_opt = SEL_SFX;
			} else {

				/* check to see if they clicked a bar */

				if ((x > rectLeft.x + rectLeft.w) && (x < rectRight.x)) {
					if ((y >= rectLeft.y) && (y <= rectLeft.y + rectLeft.h)) {
						sfx_volume = 4+(int)(128.0 * ((x - rectLeft.x - rectLeft.w - 1.0) / (rectRight.x - rectLeft.x - rectLeft.w - 2.0)));
						selected_opt = SEL_SFX;
					}
					if ((y >= rectDown.y) && (y <= rectDown.y + rectDown.h)) {
						mus_volume = 4+(int)(128.0 * ((x - rectLeft.x - rectLeft.w - 1.0) / (rectRight.x - rectLeft.x - rectLeft.w - 2.0)));
						selected_opt = SEL_MUSIC;
					}

					/* Speech-rate bar: spans the same x-range, mapped to 5 discrete levels */
					if ((y >= rectSrateDown.y) && (y <= rectSrateDown.y + rectSrateDown.h)) {
						float ratio = (float)(x - (rectLeft.x + rectLeft.w + 1)) / (float)(rectRight.x - rectLeft.x - rectLeft.w - 2);
						if (ratio < 0.0f) ratio = 0.0f;
						if (ratio > 1.0f) ratio = 1.0f;
						tts_rate = (int)(ratio * 5.0f);
						if (tts_rate > 4) tts_rate = 4;
						selected_opt = SEL_SPEECH_RATE;
					}
				}
				/* +/- arrow buttons for speech rate */
				if (inRect(rectSrateUp, x, y)) {
					tts_rate++;
					selected_opt = SEL_SPEECH_RATE;
				}
				if (inRect(rectSrateDown_btn, x, y)) {
					tts_rate--;
					selected_opt = SEL_SPEECH_RATE;
				}
			}
		}

		if (settings.sys_sound) {

			if (sfx_volume > MIX_MAX_VOLUME)
				sfx_volume = MIX_MAX_VOLUME;
			if (sfx_volume < 0)
				sfx_volume = 0;
			if (mus_volume > MIX_MAX_VOLUME)
				mus_volume = MIX_MAX_VOLUME;
			if (mus_volume < 0)
				mus_volume = 0;
			if (tts_rate > 4) tts_rate = 4;
			if (tts_rate < 0) tts_rate = 0;

			if ((mus_volume != old_mus_volume) || 
			    (sfx_volume != old_sfx_volume)) {

				if (mus_volume != old_mus_volume)
					Mix_VolumeMusic(mus_volume);

				if (sfx_volume != old_sfx_volume) {
					Mix_Volume(-1,sfx_volume);
					if (tocks%4==0)
						Mix_PlayChannel(-1, pause_sfx, 0);
					tocks++;
			    }

				pause_draw(tts_rate, selected_opt);
				draw_vols(sfx_volume, mus_volume, tts_rate, selected_opt);
				settings.mus_volume=mus_volume;
				settings.sfx_volume=sfx_volume;
				T4K_PresentScreen();
			}

			if (tts_rate != old_tts_rate) {
				set_speech_rate(tts_rate);
				pause_draw(tts_rate, selected_opt);
				draw_vols(sfx_volume, mus_volume, tts_rate, selected_opt);
				/* play preview at newly selected rate */
				T4K_Tts_say(get_speech_rate_raw(tts_rate), DEFAULT_VALUE, INTERRUPT, _("Speech rate %s"), get_speech_rate_label(tts_rate));
				T4K_PresentScreen();
			}
		}

		SDL_Delay(33);
	}

	/* --- Return to previous state --- */

	SDL_EnableKeyRepeat( 0, SDL_DEFAULT_REPEAT_INTERVAL );

	SDL_ShowCursor(0);

	if (settings.sys_sound) {
		Mix_PlayChannel(-1, pause_sfx, 0);
		Mix_Resume(-1);
	}

	pause_unload_media();

	LOG( "Leaving Pause()\n" );

	return (quit);
}


static void pause_load_media(void) {
	if (settings.sys_sound) 
		pause_sfx = LoadSound( "tock.wav" );
	up = LoadImage("up.png", IMG_ALPHA);
	if (up) { rectUp.w = up->w; rectUp.h = up->h; }
	down = LoadImage("down.png", IMG_ALPHA);
	if (down) { rectDown.w = down->w; rectDown.h = down->h; }
	left = LoadImage("left.png", IMG_ALPHA);
	if (left) { rectLeft.w = left->w; rectLeft.h = left->h; }
	right = LoadImage("right.png", IMG_ALPHA);
	if (right) { rectRight.w = right->w; rectRight.h = right->h; }

//	f1 = LoadFont(settings.theme_font_name, 24);
//	f2 = LoadFont(settings.theme_font_name, 36);
}

static void pause_unload_media(void) {
	if (settings.sys_sound)
	{
		Mix_FreeChunk(pause_sfx);
		pause_sfx = NULL;
	}
	if (pause_bkg)
	{
		SDL_FreeSurface(pause_bkg);
		pause_bkg = NULL;
	}
	SDL_FreeSurface(up);
	SDL_FreeSurface(down);
	SDL_FreeSurface(left);
	SDL_FreeSurface(right);
	up = down = left = right = NULL;
}



/******************************************/
/*                                        */
/*       Local ("private") functions      */
/*                                        */
/******************************************/



static void pause_draw(int tts_rate, int selected_opt)
{
  SDL_Rect s;
  SDL_Surface* t = NULL;
  SDL_Color white  = {255, 255, 255, 255};
  SDL_Color yellow = {255, 255, 0, 255};

  LOG("Entering pause_draw()\n");

  if (pause_bkg) {
    SDL_SetSurfaceBlendMode(pause_bkg, SDL_BLENDMODE_NONE);
    SDL_BlitSurface(pause_bkg, NULL, screen, NULL);
  }

  /* Well-spaced layout avoiding overlaps */
  rectLeft.y = rectRight.y = screen->h/2 - 120;
  rectDown.y = rectUp.y    = screen->h/2 - 45;
  rectSrateDown_btn.y = rectSrateUp.y = screen->h/2 + 30;
  rectSrateDown.y                     = screen->h/2 + 30;

  rectLeft.x = rectDown.x = screen->w/2 - (7*16) - rectLeft.w - 4;
  rectRight.x = rectUp.x  = screen->w/2 + (7*16) + 4;

  rectSrateDown_btn.w = rectLeft.w;
  rectSrateDown_btn.h = rectLeft.h;
  rectSrateUp.w       = rectLeft.w;
  rectSrateUp.h       = rectLeft.h;
  rectSrateDown.w     = rectLeft.w;
  rectSrateDown.h     = rectLeft.h;

  rectSrateDown_btn.x = rectLeft.x;
  rectSrateUp.x       = rectRight.x;
  rectSrateDown.x     = rectLeft.x;

  /* Blit arrows showing how to adjust each option: */
  if (settings.sys_sound)
  {
    if (left)  SDL_BlitSurface(left,  NULL, screen, &rectLeft);
    if (right) SDL_BlitSurface(right, NULL, screen, &rectRight);
    if (down)  SDL_BlitSurface(down,  NULL, screen, &rectDown);
    if (up)    SDL_BlitSurface(up,    NULL, screen, &rectUp);
    /* left/right arrows for the speech-rate row */
    if (left)  SDL_BlitSurface(left,  NULL, screen, &rectSrateDown_btn);
    if (right) SDL_BlitSurface(right, NULL, screen, &rectSrateUp);
  }

  if (settings.sys_sound)
  {
    const SDL_Color* sfx_col = (selected_opt == SEL_SFX) ? &yellow : &white;
    const SDL_Color* mus_col = (selected_opt == SEL_MUSIC) ? &yellow : &white;
    const SDL_Color* srate_col = (selected_opt == SEL_SPEECH_RATE) ? &yellow : &white;

    t = BlackOutline(_("Sound Effects Volume"), pause_font_size1, sfx_col);
    if (t)
    {	
      s.y = rectLeft.y - 35;
      s.x = screen->w/2 - t->w/2;
      SDL_BlitSurface(t, NULL, screen, &s);
      SDL_FreeSurface(t);
    }

    t = BlackOutline(gettext("Music Volume"), pause_font_size1, mus_col);
    if (t)
    {
      s.y = rectDown.y - 35;
      s.x = screen->w/2 - t->w/2;
      SDL_BlitSurface(t, NULL, screen, &s);
      SDL_FreeSurface(t);
    }

    char srate_buf[64];
    snprintf(srate_buf, sizeof(srate_buf), "%s: %s", _("Speech Rate"), get_speech_rate_label(tts_rate));
    t = BlackOutline(srate_buf, pause_font_size1, srate_col);
    if (t)
    {
      s.y = rectSrateDown.y - 35;
      s.x = screen->w/2 - t->w/2;
      SDL_BlitSurface(t, NULL, screen, &s);
      SDL_FreeSurface(t);
    }
  }
  else  /* No sound: */
  {
    t = BlackOutline(gettext("Sound & Music Disabled"), pause_font_size1, &white);
    if (t)
    {
      s.y = screen->h/2 - 80;
      s.x = screen->w/2 - t->w/2;
      SDL_BlitSurface(t, NULL, screen, &s);
      SDL_FreeSurface(t);
    }
  }

  t = BlackOutline(gettext("Paused!"), pause_font_size2, &white);
  if (t)
  {
	s.y = screen->h/2 - 195;
	s.x = screen->w/2 - t->w/2;
	SDL_BlitSurface(t, NULL, screen, &s);
	SDL_FreeSurface(t);
  }

  t = BlackOutline(gettext("Tab: Select  |  Arrows: Adjust"), pause_font_size1, &white);
  if (t)
  {
    s.y = screen->h/2 + 95;
    s.x = screen->w/2 - t->w/2;
    SDL_BlitSurface(t, NULL, screen, &s);
    SDL_FreeSurface(t);
  }

  t = BlackOutline(gettext("Press escape again to return to menu"), pause_font_size1, &white);
  if (t)
  {
    s.y = screen->h/2 + 130;
    s.x = screen->w/2 - t->w/2;
    SDL_BlitSurface(t, NULL, screen, &s);
    SDL_FreeSurface(t);
  }

  t = BlackOutline(gettext("Press space bar to return to game"), pause_font_size1, &white);
  if (t)
  {
    s.y = screen->h/2 + 165;
    s.x = screen->w/2 - t->w/2;
    SDL_BlitSurface(t, NULL, screen, &s);
    SDL_FreeSurface(t);
  }

  LOG("Leaving pause_draw()\n");
}


/* FIXME what if rectLeft and rectDown not initialized? - should be args */
static void draw_vols(int sfx, int mus, int tts_rate, int selected_opt)
{
  static const int srate_segs[5] = { 6, 13, 19, 26, 32 };
  SDL_Rect s, m, r;
  int i;

  if (tts_rate < 0) tts_rate = 0;
  if (tts_rate > 4) tts_rate = 4;

  s.y = rectLeft.y; 
  m.y = rectDown.y;
  r.y = rectSrateDown.y;  /* speech-rate bar row */
  m.w = s.w = r.w = 5;
  s.x = rectLeft.x + rectLeft.w + 5;
  m.x = rectDown.x + rectDown.w + 5;
  r.x = rectSrateDown_btn.x + rectSrateDown_btn.w + 5;
  m.h = s.h = r.h = 40;

  for (i = 1; i<=32; i++)
  {
    if (sfx >= i * 4) {
      if (selected_opt == SEL_SFX)
        SDL_FillRect(screen, &s, SDL_MapSurfaceRGB(screen, 50, 150, 255));
      else
        SDL_FillRect(screen, &s, SDL_MapSurfaceRGB(screen, 0, 0, 127 + sfx));
    } else {
      SDL_FillRect(screen, &s, SDL_MapSurfaceRGB(screen, 0, 0, 0));
    }

    if (mus >= i * 4) {
      if (selected_opt == SEL_MUSIC)
        SDL_FillRect(screen, &m, SDL_MapSurfaceRGB(screen, 50, 150, 255));
      else
        SDL_FillRect(screen, &m, SDL_MapSurfaceRGB(screen, 0, 0, 127 + mus));
    } else {
      SDL_FillRect(screen, &m, SDL_MapSurfaceRGB(screen, 0, 0, 0));
    }

    /* speech rate bar: 5 discrete levels (0.5, 0.75, 1, 1.25, 1.5) */
    if (i <= srate_segs[tts_rate]) {
      if (selected_opt == SEL_SPEECH_RATE)
        SDL_FillRect(screen, &r, SDL_MapSurfaceRGB(screen, 50, 255, 100));
      else
        SDL_FillRect(screen, &r, SDL_MapSurfaceRGB(screen, 0, 180 + tts_rate * 15, 0));
    } else {
      SDL_FillRect(screen, &r, SDL_MapSurfaceRGB(screen, 0, 0, 0));
    }

    r.x = m.x = s.x += 7;
  }
}
