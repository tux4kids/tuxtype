/*
   laser.h:

   Headers for "Comet Zap" typing game adapted from "Tux, of Math
   Command"  (http://tux4kids.alioth.debian.org)
   
   Copyright 2000, 2003, 2008, 2010.
   Authors: Bill Kendrick, Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   laser.h is part of Tux Typing, a.k.a "tuxtype".

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



#ifndef LASER_H
#define LASER_H

#define MAX_COMETS 30
#define NUM_CITIES 9   /* MUST BE AN EVEN NUMBER! */

#define NUM_BKGDS 5
#define MAX_CITY_COLORS 4

typedef struct comet_type {
  int alive;
  int expl;
  int city;
  int x, y;
  int shootable;
  struct comet_type* next;
  wchar_t ch;
  wchar_t* word;
  int pos;
} comet_type;

typedef struct city_type {
  int alive, expl, shields;
  int x;
} city_type;

typedef struct laser_type {
  int alive;
  int x1, y1;
  int x2, y2;
} laser_type;

enum {
  MUS_GAME,
  MUS_GAME2,
  MUS_GAME3,
  NUM_MUSICS
};

static char * music_filenames[NUM_MUSICS] = {
  "game.mod",
  "game2.mod",
  "game3.mod"
};

#endif
