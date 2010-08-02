/*
  input_methods.h - renamed from Tux Paint's im.h and very lightly modified
  by David Bruce <davidstuartbruce@gmail.com> for use in Tux Typing and other
  Tux4Kids programs - 2009-2010.  Adaptation for Tux Typing assisted by Mark
  K. Kim.  Revised version licensed under GPLv2 or later as described below.
  
  The upstream "pristine" version of this file can be obtained from
  http://www.tux4kids.org
*/

/*
  im.h

  Input method handling
  Copyright (c) 2007 by Mark K. Kim and others

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  (See COPYING.txt)

  $Id: im.h,v 1.3 2007/05/06 04:00:20 vindaci Exp $
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
