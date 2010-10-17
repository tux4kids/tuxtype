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

static Mix_Chunk *pause_sfx = NULL;
static SDL_Surface *up = NULL, *down = NULL, *left = NULL, *right = NULL;
static SDL_Rect rectUp, rectDown, rectLeft, rectRight;
const int pause_font_size1 = 24;
const int pause_font_size2 = 36;


/* Local function prototypes: */
static void draw_vols(int sfx, int mus);
static void pause_draw(void);
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

	/* --- show the pause screen --- */

	SDL_ShowCursor(1);

	// Darken the screen...
	DarkenScreen(1); 

	pause_draw();

	if (settings.sys_sound) {
		draw_vols(sfx_volume, mus_volume);
	}

	SDL_Flip(screen);

	SDL_EnableKeyRepeat( 1, 20 );

	/* --- wait for space, click, or exit --- */

	while (paused) {
		old_sfx_volume = sfx_volume;
		old_mus_volume = mus_volume;
		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_QUIT: 
					exit(0);
					break;
				case SDL_KEYUP:
					if (settings.sys_sound && 
					   ((event.key.keysym.sym == SDLK_RIGHT) ||
					    (event.key.keysym.sym == SDLK_LEFT))) 
					    	tocks = 0;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_SPACE) 
						paused = 0;
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						paused = 0;
						quit = 1;
					}
					if (settings.sys_sound) { 
						if (event.key.keysym.sym == SDLK_RIGHT) 
							sfx_volume += 4;
						if (event.key.keysym.sym == SDLK_LEFT) 
							sfx_volume -= 4;
						if (event.key.keysym.sym == SDLK_UP) 
							mus_volume += 4;
						if (event.key.keysym.sym == SDLK_DOWN) 
							mus_volume -= 4;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					mousePressed = 1;
					tocks = 0;
					break;
				case SDL_MOUSEBUTTONUP:
					mousePressed = 0;
					break;

					break;
			}
		if (settings.sys_sound && mousePressed) {
			int x, y;

			SDL_GetMouseState(&x, &y);
			/* check to see if they clicked on a button */

			if (inRect(rectUp, x, y)) {
				mus_volume += 4;
			} else if (inRect(rectDown, x, y)) {
				mus_volume -= 4;
			} else if (inRect(rectRight, x, y)) {
				sfx_volume += 4;
			} else if (inRect(rectLeft, x, y)) {
				sfx_volume -= 4;
			} else {

				/* check to see if they clicked a bar */

				if ((x > rectLeft.x + rectLeft.w) && (x < rectRight.x)) {
					if ((y >= rectLeft.y) && (y <= rectLeft.y + rectLeft.h)) {
						sfx_volume = 4+(int)(128.0 * ((x - rectLeft.x - rectLeft.w - 1.0) / (rectRight.x - rectLeft.x - rectLeft.w - 2.0)));
					}
					if ((y >= rectDown.y) && (y <= rectDown.y + rectDown.h)) {
						mus_volume = 4+(int)(128.0 * ((x - rectLeft.x - rectLeft.w - 1.0) / (rectRight.x - rectLeft.x - rectLeft.w - 2.0)));
					}

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

				draw_vols(sfx_volume, mus_volume);
				settings.mus_volume=mus_volume;
				settings.sfx_volume=sfx_volume;
				SDL_Flip(screen);
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
	rectUp.w = up->w; rectUp.h = up->h;

	down = LoadImage("down.png", IMG_ALPHA);
	rectDown.w = down->w; rectDown.h = down->h;

	left = LoadImage("left.png", IMG_ALPHA);
	rectLeft.w = left->w; rectLeft.h = left->h;

	right = LoadImage("right.png", IMG_ALPHA);
	rectRight.w = right->w; rectRight.h = right->h;

//	f1 = LoadFont(settings.theme_font_name, 24);
//	f2 = LoadFont(settings.theme_font_name, 36);
}

static void pause_unload_media(void) {
	if (settings.sys_sound)
        {
	  Mix_FreeChunk(pause_sfx);
	  pause_sfx = NULL;
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



static void pause_draw(void)
{
  SDL_Rect s;
  SDL_Surface* t = NULL;

  LOG("Entering pause_draw()\n");

  rectLeft.y = rectRight.y = screen->h/2 - 40;
  rectDown.y = rectUp.y = screen->h/2 + 60;

  rectLeft.x = rectDown.x = screen->w/2 - (7*16) - rectLeft.w - 4;
  rectRight.x = rectUp.x  = screen->w/2 + (7*16) + 4;

  /* Avoid segfault if any needed SDL_Surfaces missing: */
  if (settings.sys_sound
    && left && right && down && up)
  {
    SDL_BlitSurface(left, NULL, screen, &rectLeft);
    SDL_BlitSurface(right, NULL, screen, &rectRight);
    SDL_BlitSurface(down, NULL, screen, &rectDown);
    SDL_BlitSurface(up, NULL, screen, &rectUp);
  }

  if (settings.sys_sound)
  {
    t = BlackOutline(_("Sound Effects Volume"), pause_font_size1, &white);
    if (t)
    {	
      s.y = screen->h/2 - 80;
      s.x = screen->w/2 - t->w/2;
      SDL_BlitSurface(t, NULL, screen, &s);
      SDL_FreeSurface(t);
    }

    t = BlackOutline(gettext("Music Volume"), pause_font_size1, &white);
    if (t)
    {
      s.y = screen->h/2 + 20;
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
	s.y = screen->h/2 - 180; //60;
	s.x = screen->w/2 - t->w/2;
	SDL_BlitSurface(t, NULL, screen, &s);
	SDL_FreeSurface(t);
  }

  t = BlackOutline(gettext("Press escape again to return to menu"), pause_font_size1, &white);
  if (t)
  {
    s.y = screen->h/2 + 160;
    s.x = screen->w/2 - t->w/2;
    SDL_BlitSurface(t, NULL, screen, &s);
    SDL_FreeSurface(t);
  }

  t = BlackOutline(gettext("Press space bar to return to game"), pause_font_size1, &white);
  if (t)
  {
    s.y = screen->h/2 + 200;
    s.x = screen->w/2 - t->w/2;
    SDL_BlitSurface(t, NULL, screen, &s);
    SDL_FreeSurface(t);
  }

  LOG("Leaving pause_draw()\n");
}


/* FIXME what if rectLeft and rectDown not initialized? - should be args */
static void draw_vols(int sfx, int mus)
{
  SDL_Rect s,m;
  int i;

  s.y = rectLeft.y; 
  m.y = rectDown.y;
  m.w = s.w = 5;
  s.x = rectLeft.x + rectLeft.w + 5;
  m.x = rectDown.x + rectDown.w + 5;
  m.h = s.h = 40;

  for (i = 1; i<=32; i++)
  {
    if (sfx >= i * 4)
      SDL_FillRect(screen, &s, SDL_MapRGB(screen->format, 0, 0, 127 + sfx));
    else
      SDL_FillRect(screen, &s, SDL_MapRGB(screen->format, 0, 0, 0));

    if (mus >= i * 4)
      SDL_FillRect(screen, &m, SDL_MapRGB(screen->format, 0, 0, 127 + mus));
    else
      SDL_FillRect(screen, &m, SDL_MapRGB(screen->format, 0, 0, 0));

    m.x = s.x += 7;
  }
}


