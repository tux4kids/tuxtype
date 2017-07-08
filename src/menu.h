/*
   menu.h:

   Functions responsible for loading, parsing and displaying game menu.
   (interface)

   Copyright 2009, 2010, 2011.
Authors: Boleslaw Kulbabinski <bkulbabinski@gmail.com>
Brendan Luchen
David Bruce
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org


menu.h is part of "Tux, of Math Command", a.k.a. "tuxmath".

Tuxmath is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tuxmath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/




#ifndef MENU_H
#define MENU_H

//#include "globals.h"

#include "SDL.h"
/* titlescreen & menu frame rate */
#define MAX_FPS                    30
/* number of "real" frames per one sprite frame */
#define SPRITE_FRAME_DELAY         6




/* these are all menu choices that are available in tuxmath.
   By using a define we can create both an enum and
   a string array without writing these names twice */
#define ACTIVITIES \
    X( RUN_QUIT ),\
X( RUN_CASCADE_LEVEL1 ),\
X( RUN_CASCADE_LEVEL2 ),\
X( RUN_CASCADE_LEVEL3 ),\
X( RUN_INSTRUCT ),\
X( RUN_LASER_LEVEL1 ),\
X( RUN_LASER_LEVEL2 ),\
X( RUN_LASER_LEVEL3 ),\
X( RUN_LASER_LEVEL4 ),\
X( RUN_LESSONS ),\
X( RUN_PHRASE_TYPING ),\
X( RUN_SET_LANGUAGE ),\
X( RUN_EDIT_WORDLIST ),\
X( RUN_SET_BRAILLE_KEYS ),\
X( RUN_PROJECT_INFO ),\
X( N_OF_ACTIVITIES )  /* this one has to be the last one */



/* create enum */
#define X(name) name
enum { ACTIVITIES };
#undef X

/* we may use a few separate menu trees */
typedef enum {
    MENU_MAIN,
    N_OF_MENUS
} MenuType;

/* used also by highscore.c */
extern SDL_Rect menu_rect, stop_rect, prev_rect, next_rect;
extern SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;


/* global functions */
void LoadMenus(void);
int RunLoginMenu(void);
void RunMainMenu(void);

#endif // MENU_H

