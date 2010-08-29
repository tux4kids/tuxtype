/*
   snow.c:

   Variables and functions for SNOW feature

   Copyright 2003, 2004, 2010.
   Authors: Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   snow.c is part of Tux Typing, a.k.a "tuxtype".

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



#include <stdlib.h>
#include "SDL.h"

/* while it looks like you can just
 * change the number of planes, flakes,
 * etc and it will work all right, it is not
 * true.  Do not fiddle with number unless you
 * change the rest of the code to be the same!
 */

#define NUM_PLANES 3

extern SDL_Surface *screen;
SDL_Surface* bkg;

Uint16 snow_color;

int wind = 0;
int SNOW_on = 0;

int NUM_FLAKES = 300;
int PER_PLANE = 100; // should equal NUM_FLAKES/NUM_PLANES

/* to get the real x location divide by 8 (>>3)
 */
struct {
	int x;
	int y;
} flake[300];

/* used for the SDL_UpdateRects */
SDL_Rect SNOW_rects[1000];

/* pass add_wind +/- 1 --- it just adds a fraction in that direction! */
void add_wind( int dir ) {

	/* don't let the wind blow too hard */
	if ( wind * dir > 16 )
		return;
	
	if (dir > 0)
		wind++;
	else
		wind--;
}

/* ### INTERNAL FUNCTION ### 
 * move_snow: updates the location of the flakes
 * based upon the wind and different "planes"
 * of the flakes
 */
void move_snow(void) {
	int i;
	static int slowframe=1;
	int ws=wind;

	slowframe = !slowframe;
	/* even though we only move the "slowest" every other frame,
	 * the code is still updating them every single frame!!!
	 * the SNOW_Rect data structure will need to be updated to fix this!
	 * slowframe just cycles between updating the slowest and not!
	 */
	if (slowframe) {
		for (i = 0; i<PER_PLANE; i++) {
			flake[i].x += ws;
			flake[i].y ++;
		}
	}

	for (i = PER_PLANE; i<PER_PLANE<<1; i++) {
		flake[i].x += ws;
		flake[i].y += 1;
	}

	ws<<=1;
	for (i = PER_PLANE<<1; i<NUM_FLAKES; i++) {
		flake[i].x += ws;
		flake[i].y += 2;
	} 

	for (i = 0; i<NUM_FLAKES; i++) {
		/* if a flake hits the bottom, move to top randomly */
		if (flake[i].y >= 478) {
			flake[i].y = 0;
			flake[i].x = (int)(8*639.0*rand()/(RAND_MAX+1.0));
		}

		/* if a flake hists the edge, move to the other edge randomly */
		if (flake[i].x >= 638<<3) {
			flake[i].x = 0;
			flake[i].y = (int)(478.0*rand()/(RAND_MAX+1.0));
		} else if (flake[i].x < 0) {
			flake[i].x = 8*637;
			flake[i].y = (int)(478.0*rand()/(RAND_MAX+1.0));
		}

		/* the first NUM_FLAKES are for erasing the last frame's FLAKES */
		SNOW_rects[i].x = SNOW_rects[NUM_FLAKES+i].x;
		SNOW_rects[i].y = SNOW_rects[NUM_FLAKES+i].y;
		/* the next NUM_FLAKES are for bliting the new positions this frame */
		SNOW_rects[NUM_FLAKES+i].x = flake[i].x>>3;
		SNOW_rects[NUM_FLAKES+i].y = flake[i].y;
	}
}

/* SNOW_draw: updates the screen surface
 * with the new flake pixels
 * note: y<<9 + y<<7 is short for y*640
 */
void SNOW_draw(void) {
	int i;
	Uint16 *to;

	/* we only draw if we are on and haven't toggled (see SNOW_Erase for more info */
	if (SNOW_on!=1)
		return;

	for (i=0; i<NUM_FLAKES; i++) {
		to = screen->pixels;
		to += (SNOW_rects[NUM_FLAKES+i].y<<9) + (SNOW_rects[NUM_FLAKES+i].y<<7) + SNOW_rects[NUM_FLAKES+i].x;
		*to = snow_color;
		to += 1;
		*to = snow_color;
		to += 640;
		*to = snow_color;
		to -= 1;
		*to = snow_color;
	}
}

/* SNOW_erase: updates the screen surface
 * by replacing the previous flakes with
 * the background image (set via SNOW_setBkg).
 * This should be run before bliting any
 * graphics to the screen so you don't over-
 * write good images with background!
 *
 * note: y<<9 + y<<7 is short for y*640
 */
void SNOW_erase(void) {
	int i;
	Uint16 *from, *to;

	if (!SNOW_on)
		return;

	/* SNOW_on isn't a boolean variable, it is really
	 * holding the state:
	 *  0 means no snow
	 *  1 means snow
	 *  2 means the user has requested snowing to stop
	 *          we cannot just stop snowing since we
	 *          need to erase the previous frames snow
	 *  3 means have erased the snow during the previous
	 *          frame and can go back to waiting for
	 *          snow to be turned on
	 */
	if (SNOW_on>1) {
		if (SNOW_on == 3)
			SNOW_on = 0;
		else
			SNOW_on++;
	}

	for (i=0; i<NUM_FLAKES; i++) {
		to = screen->pixels;
		from = bkg->pixels;
		to += (SNOW_rects[i].y<<9) + (SNOW_rects[i].y<<7) + SNOW_rects[i].x;
		from += (SNOW_rects[i].y<<9) + (SNOW_rects[i].y<<7) + SNOW_rects[i].x;
		*to = *from;
		to += 1;
		from += 1;
		*to = *from;
		to += 640;
		from += 640;
		*to = *from;
		to -= 1;
		from -= 1;
		*to = *from;
	}
}

/* SNOW_setBkg: we need to have the  background
 * to erase snow flakes every frame
 */
void SNOW_setBkg( SDL_Surface *img ) {
	bkg = img;
}

/* SNOW_add: this adds the other rectangles that we need 
 * to update on the screen to the list and returns
 * the total number of items on the complete list
 * (flakes & regular stuff)
 */
int SNOW_add( SDL_Rect *rs, int num ){
	int i;
	for (i=0; i<num; i++) {
		SNOW_rects[(NUM_FLAKES<<1)+i].x = rs[i].x;
		SNOW_rects[(NUM_FLAKES<<1)+i].y = rs[i].y;
		SNOW_rects[(NUM_FLAKES<<1)+i].w = rs[i].w;
		SNOW_rects[(NUM_FLAKES<<1)+i].h = rs[i].h;
	}
	return num + (NUM_FLAKES<<1);
}

/* SNOW_init: initing the snow "library" envoles
 * seting up the color for the snow, the inital
 * locations for the flakes and the size of the
 * snow rectangles
 */
void SNOW_init( void ) {

	int i;

	snow_color = SDL_MapRGB( screen->format, 255, 255, 255 );
	for (i=0; i<NUM_FLAKES; i++) {
		flake[i].x = (int)(8*638.0*rand()/(RAND_MAX+1.0));
		flake[i].y = (int)(480.0*rand()/(RAND_MAX+1.0));
	}
	for (i=0; i<2*NUM_FLAKES; i++) {
		SNOW_rects[i].w = 2;
		SNOW_rects[i].h = 2;
	}
}

/* SNOW_toggle: toggles the snow on and off.
 * see SNOW_erase for a discription of what
 * SNOW_on means
 */
void SNOW_toggle( void ) {
	SNOW_on ++;
}

/* SNOW_update: update the wind with
 * a 10% prob of blowing further left
 * a 10% prob of blowing further right
 * and 80% of not changing.
 * AFTER updating the wind, we move all
 * the snow
 */
void SNOW_update( void ) {
	int rand_tmp = (int)(10.0*rand()/(RAND_MAX+1.0));

	if (!SNOW_on)
		return;

	if (rand_tmp == 0)
		add_wind(-1);
	else if (rand_tmp == 1)
		add_wind(+1);
	
	move_snow();
}

