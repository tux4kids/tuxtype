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
  IMG_TUX_HELMET1,
  IMG_TUX_HELMET2,
  IMG_TUX_HELMET3,
  IMG_CITY_BLUE,
  IMG_CITY_BLUE_EXPL1,
  IMG_CITY_BLUE_EXPL2,
  IMG_CITY_BLUE_EXPL3,
  IMG_CITY_BLUE_EXPL4,
  IMG_CITY_BLUE_EXPL5,
  IMG_CITY_BLUE_DEAD,
  IMG_CITY_GREEN,
  IMG_CITY_GREEN_EXPL1,
  IMG_CITY_GREEN_EXPL2,
  IMG_CITY_GREEN_EXPL3,
  IMG_CITY_GREEN_EXPL4,
  IMG_CITY_GREEN_EXPL5,
  IMG_CITY_GREEN_DEAD,
  IMG_CITY_ORANGE,
  IMG_CITY_ORANGE_EXPL1,
  IMG_CITY_ORANGE_EXPL2,
  IMG_CITY_ORANGE_EXPL3,
  IMG_CITY_ORANGE_EXPL4,
  IMG_CITY_ORANGE_EXPL5,
  IMG_CITY_ORANGE_DEAD,
  IMG_CITY_RED,
  IMG_CITY_RED_EXPL1,
  IMG_CITY_RED_EXPL2,
  IMG_CITY_RED_EXPL3,
  IMG_CITY_RED_EXPL4,
  IMG_CITY_RED_EXPL5,
  IMG_CITY_RED_DEAD,
  IMG_COMET1,
  IMG_COMET2,
  IMG_COMET3,
  IMG_COMETEX1,
  IMG_COMETEX2,
  IMG_CONSOLE,
  IMG_TUX_CONSOLE1,
  IMG_TUX_CONSOLE2,
  IMG_TUX_CONSOLE3,
  IMG_TUX_CONSOLE4,
  IMG_TUX_RELAX1,
  IMG_TUX_RELAX2,
  IMG_TUX_DRAT,
  IMG_TUX_YIPE,
  IMG_TUX_YAY1,
  IMG_TUX_YAY2,
  IMG_TUX_YES1,
  IMG_TUX_YES2,
  IMG_TUX_SIT,
  IMG_TUX_FIST1,
  IMG_TUX_FIST2,
  IMG_WAVE,
  IMG_SCORE,
  IMG_NUMBERS,
  IMG_GAMEOVER,
  NUM_IMAGES
};


static char* image_filenames[NUM_IMAGES] = {
  "status/tux_helmet1.png",
  "status/tux_helmet2.png",
  "status/tux_helmet3.png", 
  "cities/city-blue.png",
  "cities/csplode-blue-1.png",
  "cities/csplode-blue-2.png",
  "cities/csplode-blue-3.png",
  "cities/csplode-blue-4.png",
  "cities/csplode-blue-5.png",
  "cities/cdead-blue.png",
  "cities/city-green.png",
  "cities/csplode-green-1.png",
  "cities/csplode-green-2.png",
  "cities/csplode-green-3.png",
  "cities/csplode-green-4.png",
  "cities/csplode-green-5.png",
  "cities/cdead-green.png",
  "cities/city-orange.png",
  "cities/csplode-orange-1.png",
  "cities/csplode-orange-2.png",
  "cities/csplode-orange-3.png",
  "cities/csplode-orange-4.png",
  "cities/csplode-orange-5.png",
  "cities/cdead-orange.png",
  "cities/city-red.png",
  "cities/csplode-red-1.png",
  "cities/csplode-red-2.png",
  "cities/csplode-red-3.png",
  "cities/csplode-red-4.png",
  "cities/csplode-red-5.png",
  "cities/cdead-red.png",
  "comets/comet1.png",
  "comets/comet2.png",
  "comets/comet3.png",
  "comets/cometex1.png",
  "comets/cometex2.png",
  "tux/console.png",
  "tux/tux-console1.png",
  "tux/tux-console2.png",
  "tux/tux-console3.png",
  "tux/tux-console4.png",
  "tux/tux-relax1.png",
  "tux/tux-relax2.png",
  "tux/tux-drat.png",
  "tux/tux-yipe.png",
  "tux/tux-yay1.png",
  "tux/tux-yay2.png",
  "tux/tux-yes1.png",
  "tux/tux-yes2.png",
  "tux/tux-sit.png",
  "tux/tux-fist0.png",
  "tux/tux-fist1.png",
  "status/wave.png",
  "status/score.png",
  "status/numbers.png",
  "status/gameover.png"
};

enum {
  SND_POP,
  SND_LASER,
  SND_BUZZ,
  SND_ALARM,
  SND_SHIELDSDOWN,
  SND_EXPLOSION,
  SND_TOCK,
  NUM_SOUNDS
};


static char* sound_filenames[NUM_SOUNDS] = {
  "pop.wav",
  "laser.wav",
  "buzz.wav",
  "alarm.wav",
  "shieldsdown.wav",
  "explosion.wav",
  "tock.wav"
};


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
