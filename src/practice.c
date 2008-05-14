/**************************************************************************
practice.c 
-  description: practice module
-------------------
begin                : Friday Jan 25, 2003
copyright            : (C) 2003 by Jesse Andrews
email                : jdandr2@uky.edu

Revised extensively: 2007
David Bruce <dbruce@tampabay.rr.com>
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "globals.h"
#include "funcs.h"

static SDL_Surface* bg = NULL;
static SDL_Surface* hands = NULL; 
static SDL_Surface* hand[11] = {NULL};
static SDL_Rect hand_loc, letter_loc;
static TTF_Font* font = NULL;
static wchar_t phrase[255][FNLEN];

static Mix_Chunk* wrong = NULL;

/*local function prototypes: */
static int get_phrase(const wchar_t* phr);
static int practice_load_media(void);
static void practice_unload_media(void);
static void print_at(const wchar_t* pphrase, int wrap, int x, int y);
static void show(unsigned char t);


/************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)             */
/*                                                                      */
/************************************************************************/


/* FIXME this is not UTF-8/Unicode compatible */
int Phrases(wchar_t* pphrase )
{

  /* TODO 
  * 
  * 
  * 
  */

  /* FIXME make variable names more descriptive */
  Uint32 start = 0, a = 0;
  int quit = 0,
      i = 0,
      c = 0,
      wp = 0,
      z = 0,
      total = 0,
      state = 0;
  int key[100] = {0};
  SDL_Rect dst, dst2, dst3, dst4, dst5;
  char keytime[20],
       totaltime[20];
  SDL_Surface* srfc = NULL;


  
  if (!practice_load_media())
  {
    fprintf(stderr, "Phrases() - practice_load_media() failed, returning.\n");
    return 0;
  }

  SDL_BlitSurface(bg, NULL, screen, NULL);
  SDL_BlitSurface(hands, NULL, screen, &hand_loc);
  SDL_Flip(screen);

  wp = get_phrase(pphrase);

  if (!wcsncmp(phrase[0], (wchar_t*)"", 1))
    wcsncpy(pphrase, phrase[0], 80);

  srfc = GetWhiteGlyph(65);

  if (!srfc)
  {
    fprintf(stderr, "Phrases() - GetWhiteGlyph(65) not defined - bailing out.\n");
    return 0;
  }

  dst.x = 320 - (srfc->w/2);
  dst.y = 100;
  dst.w = srfc->w;
  dst.h = srfc->h;

  dst2.x = 50;
  dst2.y = 400;
  dst2.w = srfc->w;
  dst2.h = srfc->h;

  dst3.x = 50;
  dst3.y = 400;
  dst3.w = 160;
  dst3.h = 50;

  dst4.x = 480;
  dst4.y = 400;
  dst4.w = 240;
  dst4.h = 50;

  dst5.x = 480;
  dst5.y = 400;
  dst5.w = 240;
  dst5.h = 50;

  dst.x = 40;

  start = SDL_GetTicks();

  do
  {
    switch (state)
    {
      case 0:
        start = SDL_GetTicks();
        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        state = 1;
        break;

      case 1:
        if (SDL_GetTicks() - start > 500)
        {
          /* Show finger hint, if available. Note that GetFinger() */
          /* returns negative values on error and never returns a  */
          /* value greater than 9.                                 */
          int fing = GetFinger(pphrase[c]);
          if (fing >= 0) 
            SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
          state = 2;
        }
        break;

      case 2:
        if (state == 2 && SDL_GetTicks() - start > 750)
        {
          state = 3;
        }
        break;

      case 3:
       SDL_BlitSurface(hands, NULL, screen, &hand_loc);
       state = 12;
       break;  

      case 4:
        {
          int fing = GetFinger(pphrase[c]);
          if (fing >= 0) 
            SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
          state = 11;
          break;
        }

      default:
        state -= 2; // this is to make the flashing slower
    }
	next_letter(pphrase, c);

    while  (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        a = SDL_GetTicks();
        key[c] = a - start;
        total += key[c];
        sprintf(keytime, "%.2f", (float) key[c] / 1000);
        sprintf(totaltime, "%.2f", (float) total / 1000);
        start = a;

        if (event.key.keysym.sym == SDLK_ESCAPE)
          quit = 1;

        if (event.key.keysym.sym == SDLK_DOWN) 
        {
          //practice next phase in list
          //a=a;
        }
        else
        {
          if (pphrase[c]==event.key.keysym.unicode)
          {
            state = 0;
            dst2.x = 40;
            dst4.x = 480;
            SDL_BlitSurface(bg, &dst3, screen, &dst2);
            SDL_BlitSurface(bg, &dst5, screen, &dst4);
            SDL_Flip(screen);

            srfc = GetWhiteGlyph(event.key.keysym.unicode);
            if (srfc)
            { 
              SDL_BlitSurface(srfc, NULL, screen, &dst);
              dst.x = (dst.x + srfc->w) - 5;
            }
 
            for (z = 0; z < strlen(keytime); z++)
            {
              srfc = GetWhiteGlyph((int)keytime[z]);
              if (srfc)
              {
                SDL_BlitSurface(srfc, NULL, screen, &dst2);
                dst2.x = dst2.x + srfc->w - 2;
              }
            }

            for (z = 0;z < strlen(totaltime); z++)
            {
              srfc = GetWhiteGlyph((int)totaltime[z]);
              if (srfc)
              {
                SDL_BlitSurface(srfc, NULL, screen, &dst4);
                dst4.x = dst4.x + srfc->w - 2;
              }
            }


            if (c == (wcslen(pphrase) - 1))
            {
		//print_string_at(_("Great!"), 275, 200);
              wchar_t buf[10];
              ConvertFromUTF8(buf, _("Great!"));
              print_at(buf,6 ,275 ,200);
              SDL_Flip(screen);
              SDL_Delay(2500);
              quit = 1;
            }

            if (c == wp)
            {
              c++;
              dst.x = 40;
              dst.y = 142;
            }

            c++;
          }
          else
          {
            if (event.key.keysym.sym != SDLK_RSHIFT
             && event.key.keysym.sym != SDLK_LSHIFT)
             PlaySound(wrong);
          }
        }
      }
    }
    SDL_Flip(screen);
    SDL_Delay(30);

  }while (!quit);

  practice_unload_media();

  return 1;
}



/************************************************************************/
/*                                                                      */ 
/*       "Private" functions (local to practice.c)                      */
/*                                                                      */
/************************************************************************/


static int practice_load_media(void)
{
  int i;	
  unsigned char fn[FNLEN];
  unsigned char let[5];
  int load_failed = 0;

  LOG("Loading practice media\n");


  hands = LoadImage("hands/hands.png", IMG_ALPHA);
  bg = LoadImage("main_bkg.png", IMG_ALPHA);
  wrong = LoadSound("tock.wav");
  font = LoadFont(settings.theme_font_name, 30);

  for (i = 0; i < 10; i++)
  {
    sprintf(fn, "hands/%d.png", i);
    hand[i] = LoadImage(fn, IMG_ALPHA);
    if (!hand[i])
      load_failed = 1;
  }

  /* Get out if anything failed to load: */
  if (load_failed
    ||!hands
    ||!bg
    ||!wrong
    ||!font)
  {
    fprintf(stderr, "practice_load_media() - failed to load needed media \n");
    practice_unload_media;
    return 0;
  }

  /* Should be safe from here on out: */
  hand_loc.x = (screen->w/2) - (hand[0]->w/2);
  hand_loc.y = screen->h - (hand[0]->h);
  hand_loc.w = (hand[0]->w);
  hand_loc.h = (hand[0]->h);

  /* Now render letters for glyphs in alphabet: */
  RenderLetters(font);
  TTF_CloseFont(font);  /* Don't need it after rendering done */
  font = NULL;

  LOG("DONE - Loading practice media\n");
  return 1;
}



static void practice_unload_media(void)
{
	int i;
	SDL_FreeSurface(bg);
        bg = NULL;
	SDL_FreeSurface(hands);
        hands = NULL;
	//TTF_CloseFont(font);

	for (i=0; i<10; i++) 
        {
          SDL_FreeSurface(hand[i]);
          hand[i] = NULL;
        }

	Mix_FreeChunk(wrong);
	wrong = NULL;
}


/* looks like dead code: */
static void show(unsigned char t)
{
	SDL_Rect dst;
        SDL_Surface* s = NULL;

        s= GetWhiteGlyph((int)t);
        if (!s)
          return; 
	dst.x = 320 - (s->w/2);
	dst.y = 100;
	dst.w = s->w;
	dst.h = s->h;
	SDL_BlitSurface(s, NULL, screen, &dst);
}



static int get_phrase(const wchar_t* phr)
{
  int pc = 0;  // 'phrase count' (?)
  int pw[256] = { 0 };
  int wp = 0, i = 0, c = 0, z = 0;
  char fn[FNLEN];

  /* If we didn't receive a phrase get the first one from the file...*/
  if (wcsncmp((wchar_t*)"", phr, 40) == 0)
  {
    FILE* pf; /*   "phrase file"   */
    /* set the phrases directory/file */
    /* FIXME I think the phrases should be under data or the theme */
#ifdef WIN32
    snprintf(fn, FNLEN - 1, "userdata/phrases.txt");
#else
    snprintf(fn, FNLEN - 1, (const char*)"%s/.tuxtype/phrases.txt", getenv("HOME"));
#endif

    DEBUGCODE { printf("get_phrases(): phrases file is '%s'\n", fn ); }
    LOG("get_phrases(): trying to open phrases file\n");
    pf = fopen( fn, "r" );
    if (pf == NULL) 
      return(wp);  /* why not just 'return 0;' ??? */

    /* So now copy each line into phrases array: */
    while (!feof(pf) && pc < 256) 
    {
      fscanf( pf, "%[^\n]\n", phrase[pc] );
      pc++;
      DEBUGCODE {printf("%s", phrase[pc]);}
    }
    if (pc == 256)
      LOG("File contains more than max allowed phrases - stopping\n");

    fclose(pf);
    pc--;
  } 
  else
  {
    pc = 1;
    wcsncpy(phrase[0], phr, 80);
  }

  /* FIXME maybe should verify that all chars in phrase are 
//   /* Need to generate glyphs for all the needed Unicode chars: */
// 
//   ResetCharList();
//   /* 'A' (i.e. 65) always has to go into list because width used for layout */
//   /* HACK also need chars for "Great!" because of congrats message - this   */
//   /* obviously is not a general solution. Numerals also needed for timers.  */
//   {
//     char* let = "AGreat!0123456789.";
//     GenCharListFromString(let);
//   }
// 
// 
//   /* Scan through all the phrases and put needed chars into list: */
//   for (c = 0; c <= pc; c++)
//     GenCharListFromString(phrase[c]);
// 
//   /* Now render letters for glyphs in list: */
//   font = LoadFont(settings.theme_font_name, 32 );
//   if (!font)
//   {
//     fprintf(stderr, "get_phrase() - could not load font\n");
//     return 0;
//   }
// 
//   RenderLetters(font);
// 
//   TTF_CloseFont(font);
//   font = NULL;


  //Calculate and record pixel width of phrases
  {
    SDL_Surface* let = NULL;
    for (c = 0; c <= pc; c++)
    {
      for(i = 0; i < wcslen(phrase[c]); i++)
      {
        let = GetWhiteGlyph((int)phrase[c][i]);
        if (let)  
          pw[c]+= let->w - 5;
        else
        {
          fprintf(stderr, "get_phrase() - needed glyph not available\n");
          return;
        }
      }
    }
  }

  //Find wrapping point
  for (c = 0; c <= pc; c++)
  {
    if (pw[c] < 598)  // If the phrase is less than 598 pixels wide
    {
      if (c == 0)
      {
        wp = wcslen(phrase[c]);
        print_at(phrase[0], wp, 40, 10);
      }
    }
    else
    {
      z = 0;
      wp = 0;

      for (i = 0; i < wcslen(phrase[c]); i++)
      {
        /* Should be safe (if no glyph, will have returned above) */
        z += GetWhiteGlyph((int)phrase[c][i])->w-5;
        if (wp == 0 && z > 598)
        {
          wp = i - 1;
          break;
        }
      }

      for (i = wp; i >= 0; i--)
      {
        if (wcsncmp((wchar_t*)" ", &phrase[c][i], 1) == 0)
        {
          wp = i-1;
          break;
        }
      }

      if (c == 0)
      {
        print_at(phrase[0], wp, 40, 10);
      }
    }
  }
  LOG("Leaving get_phrase()\n");
  return(wp);
}



static void print_at(const wchar_t *pphrase, int wrap, int x, int y)
{
  int z = 0;
  SDL_Surface* surf = NULL;
  letter_loc.x = x;
  letter_loc.y = y;
  letter_loc.w = GetWhiteGlyph(65)->w;
  letter_loc.h = GetWhiteGlyph(65)->h;

  LOG("Entering print_at()\n");

  if (wrap >= wcslen(pphrase)) // I think this means it fits on a single line
  {
    for (z = 0; z <wcslen(pphrase); z++)
    {
      surf = GetWhiteGlyph(pphrase[z]);
      if (surf)
      {
        DEBUGCODE{printf("surf not NULL for %C\n", pphrase[z]);}
        SDL_BlitSurface(surf, NULL, screen, &letter_loc);
        letter_loc.x = (letter_loc.x + surf->w) - 5;
      }
      else
      {
        fprintf(stderr, "print_at(): needed glyph for %C not found\n",
                pphrase[z]);
      }
    }
  }
  else  /* Another line required - code only seems to support 1 or 2 lines! */
  {
    for (z = 0; z <= wrap; z++) 
    {
      surf = GetWhiteGlyph(pphrase[z]);
      if (surf)
      {
        DEBUGCODE{printf("surf not NULL for %C\n", pphrase[z]);}
        SDL_BlitSurface(surf, NULL, screen, &letter_loc);
        letter_loc.x = (letter_loc.x + surf->w) - 5;      }
      else
      {
        fprintf(stderr, "print_at(): needed glyph for %C not found\n",
                pphrase[z]);
      }
    }

    /* Move 'cursor' back to left and down one line: */
    letter_loc.x = 40;
    // - (letter_loc.h/4) to account for free space at top and bottom of rendered letters
    letter_loc.y = letter_loc.y + letter_loc.h - (letter_loc.h/4);

    for (z = wrap + 2; z <wcslen(pphrase); z++)
    {
      surf = GetWhiteGlyph(pphrase[z]);
      if (surf)
      {
        DEBUGCODE{printf("surf not NULL for %c\n", pphrase[z]);}
        SDL_BlitSurface(surf, NULL, screen, &letter_loc);
        letter_loc.x = (letter_loc.x + surf->w) - 5;
      }
      else
      {
        fprintf(stderr, "print_at(): needed glyph for %c not found",
                pphrase[z]);
      }
    }
  }
  LOG("Leaving print_at()\n");
}

static void next_letter(wchar_t *t, int c)
{
	int i;
	wchar_t buf[30];
        i=ConvertFromUTF8(buf, _("Next letter "));
	buf[i]=t[c];
	buf[i+1]=0;
        print_at(buf,wcslen(buf),230 ,400);
	/*dst.x=320;
	dst.y=350;
	dst.h=100;
	dst.w=100;
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, 100, 100, 32, RMASK, GMASK, BMASK, AMASK);
	SDL_BlitSurface(tmp, NULL, screen, &dst);
	SDL_FreeSurface(tmp);
	dst.x=390;	
	dst.y=400;
	dst.h=21;
	dst.w=21;
	tmp = create_surface_wchar( t+c, font, &white,1);
	SDL_BlitSurface(tmp, NULL, screen, &dst);
	SDL_FreeSurface(tmp);*/
}
