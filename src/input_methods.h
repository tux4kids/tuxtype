/*
   input_methods.h - renamed from Tux Paint's im.h and very lightly modified
   by David Bruce <davidstuartbruce@gmail.com> for use in Tux Typing and other
   Tux4Kids programs - 2009-2010.  Adaptation for Tux Typing assisted by Mark
   K. Kim.  Revised version licensed under GPLv3 or later as described below.
  
   The upstream "pristine" version of this file can be obtained from
   http://www.tuxpaint.org

   Input method handling
   Copyright 2007, 2009, 2010 by Mark K. Kim, David Bruce and others.
   
   For adapted version in Tux Math, Tux Typing, and t4k_common library:   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   input_methods.h is part of Tux Typing, a.k.a "tuxtype".

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




//#ifndef TUXPAINT_IM_H
//#define TUXPAINT_IM_H
#ifndef INPUT_METHODS_H
#define INPUT_METHODS_H

#include "SDL.h"
//#include "i18n.h"


/* ***************************************************************************
* TYPES
*/

typedef struct IM_DATA {
  int lang;             /* Language used in sequence translation */
  wchar_t s[16];        /* Characters that should be displayed */
  const char* tip_text; /* Tip text, read-only please */

  /* For use by language-specific im_event_<lang> calls. PRIVATE! */
  wchar_t buf[8];       /* Buffered characters */
  int redraw;           /* Redraw this many characters next time */
  int request;          /* Event request */
} IM_DATA;


/* ***************************************************************************
* FUNCTIONS
*/

void im_init(IM_DATA* im, int lang);      /* Initialize IM */
void im_fullreset(IM_DATA* im);           /* Full Reset IM */
void im_softreset(IM_DATA* im);           /* Soft Reset IM */
void im_free(IM_DATA* im);                /* Free IM resources */

int im_read(IM_DATA* im, SDL_keysym ks);



#endif /* TUXPAINT_IM_H */

/* vim:ts=8
*/
