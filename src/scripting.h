/*
   scripting.h:

   Headers for XML-based scripting code.
   
   Copyright 2003, 2004, 2007, 2008, 2009, 2010.
   Authors: Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   scripting.h is part of Tux Typing, a.k.a "tuxtype".

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

static const int DEFAULT_SCRIPT_FONT_SIZE = 24;

enum { itemTEXT, itemIMG, itemBKGD, itemWAV, itemPRAC, itemWFIN, itemWFCH };

/* linked list of elements for a page */
struct item {
        char type;		// text or img or wav enum type?
        char *data;		// holds text/location for file (sound/image)
        char *onclick;		// holds additional data
        char size;		// holds font size if applicable
        char align;             // holds 'L'eft, 'R'ight, 'C'enter for alignment
        char loop;		// holds if sound files loop
        int  goal;		// goal for practice session
	int  x,y;		// for absolute positioning
        SDL_Color *color;       // holds text color
        
        struct item *next; // the linked list part ... 
};

typedef struct item itemType;

/* linked list of pages for a lesson */
struct page {
    itemType *items;	// linked list of elements
    char *background;		// background image
    char *title;		// title of the page
    SDL_Color *bgcolor;		// background color
    SDL_Color *fgcolor;		// default text color
    
    struct page *next;         // the linked list part ...
    struct page *prev;         // the doubly-linked list part ...
};

typedef struct page pageType;

struct script {
    pageType *pages;		// linked list of pages
    char *title;		// title of lesson
    SDL_Color *bgcolor;         // default background color for all pages
    SDL_Color *fgcolor;         // default foreground color for all text
    char *background;		// default background image for all pages
}; 

typedef struct script scriptType;

