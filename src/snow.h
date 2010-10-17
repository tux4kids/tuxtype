/*
   snow.h:

   Variables and functions for SNOW feature

   Copyright 2003, 2004, 2010.
   Authors: Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   snow.h is part of Tux Typing, a.k.a "tuxtype".

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



extern int SNOW_on;
extern SDL_Rect SNOW_rects[2000];
extern int SNOW_add( SDL_Rect *rs, int num );
extern void SNOW_toggle( void );
extern void SNOW_draw( void );
extern void SNOW_erase( void );
extern void SNOW_setBkg( SDL_Surface *img );
extern void SNOW_init( void );
extern void SNOW_update( void );
