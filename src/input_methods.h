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


/* Language enum copied from Tux Paint's i18n.h (same authors) to */
/* avoid need to separately include that file:                    */
/* Possible languages: */

enum
{
  LANG_AF,			/* Afrikaans */
  LANG_AR,			/* Arabic */
  LANG_AST,			/* Asturian */
  LANG_AZ,			/* Azerbaijani */
  LANG_BE,			/* Belarusian */
  LANG_BG,			/* Bulgarian */
  LANG_BO,			/* Tibetan */
  LANG_BR,			/* Breton */
  LANG_CA,			/* Catalan */
  LANG_CS,			/* Czech */
  LANG_CY,			/* Welsh */
  LANG_DA,			/* Danish */
  LANG_DE,			/* German */
  LANG_EL,			/* Greek */
  LANG_EN,			/* English (American) (DEFAULT) */
  LANG_EN_AU,			/* English (Australian) */
  LANG_EN_CA,			/* English (Canadian) */
  LANG_EN_GB,			/* English (British) */
  LANG_EN_ZA,			/* English (South African) */
  LANG_EO,			/* Esperanto */
  LANG_ES,			/* Spanish */
  LANG_ES_MX,			/* Spanish (Mexican) */
  LANG_ET,			/* Estonian */
  LANG_EU,			/* Basque */
  LANG_FI,			/* Finnish */
  LANG_FO,			/* Faroese */
  LANG_FR,			/* French */
  LANG_GA,			/* Irish Gaelic */
  LANG_GD,			/* Scottish Gaelic */
  LANG_GL,			/* Galician */
  LANG_GR,			/* Gronings */
  LANG_GU,			/* Gujarati */
  LANG_HE,			/* Hebrew */
  LANG_HI,			/* Hindi */
  LANG_HR,			/* Croatian */
  LANG_HU,			/* Hungarian */
  LANG_I_KLINGON_ROMANIZED,	/* Klingon (Romanized) */
  LANG_ID,			/* Indonesian */
  LANG_IS,			/* Icelandic */
  LANG_IT,			/* Italian */
  LANG_JA,			/* Japanese */
  LANG_KA,			/* Georgian */
  LANG_KM,			/* Khmer */
  LANG_KO,			/* Korean */
  LANG_KU,			/* Kurdish */
  LANG_LT,			/* Lithuanian */
  LANG_LV,			/* Latvian */
  LANG_MK,			/* Macedonian */
  LANG_MS,			/* Malay */
  LANG_NB,			/* Norwegian Bokmal */
  LANG_NL,			/* Dutch */
  LANG_NN,			/* Norwegian Nynorsk */
  LANG_NR,                      /* Ndebele */
  LANG_OC,			/* Occitan */
  LANG_OJ,			/* Ojibway */
  LANG_PL,			/* Polish */
  LANG_PT_BR,			/* Portuguese (Brazilian) */
  LANG_PT_PT,			/* Portuguese (Portugal) */
  LANG_RO,			/* Romanian */
  LANG_RU,			/* Russian */
  LANG_RW,			/* Kinyarwanda */
  LANG_SHS,			/* Shuswap */
  LANG_SK,			/* Slovak */
  LANG_SL,			/* Slovenian */
  LANG_SON,                     /* Songhay */
  LANG_SQ,			/* Albanian */
  LANG_SR,			/* Serbian */
  LANG_SV,			/* Swedish */
  LANG_SW,			/* Swahili */
  LANG_TA,			/* Tamil */
  LANG_TE,                      /* Telugu */
  LANG_TH,			/* Thai */
  LANG_TL,			/* Tagalog */
  LANG_TR,			/* Turkish */
  LANG_TWI,			/* Twi */
  LANG_UK,			/* Ukrainian */
  LANG_VE,			/* Venda */
  LANG_VI,			/* Vietnamese */
  LANG_WA,			/* Walloon */
  LANF_WO,                      /* Wolof */
  LANG_XH,			/* Xhosa */
  LANG_ZH_CN,			/* Chinese (Simplified) */
  LANG_ZH_TW,			/* Chinese (Traditional) */
  LANG_ZAM,			/* Zapotec (Miahuatlan) */
  NUM_LANGS
};
#endif /* TUXPAINT_IM_H */

/* vim:ts=8
*/
