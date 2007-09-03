/**************************************************************************
practice.c 
-  description: practice module
-------------------
begin                : Friday Jan 25, 2003
copyright            : (C) 2003 by Jesse Andrews
email                : jdandr2@uky.edu
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
static char phrase[255][FNLEN];

static Mix_Chunk* wrong = NULL;

/*local function prototypes: */
static int get_phrase(const char* phr);
static int practice_load_media(void);
static void practice_unload_media(void);
static void print_at(const char* pphrase, int wrap, int x, int y);
static void show(unsigned char t);


/************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)             */
/*                                                                      */
/************************************************************************/


/* FIXME this is not UTF-8/Unicode compatible */
int Phrases(char* pphrase )
{

  /* TODO 
  * 
  * 
  * 
  */

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
  char keytime[FNLEN],
       totaltime[FNLEN];
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

  if (!strncmp(phrase[0], "", 1))
    strncpy(pphrase, phrase[0], 80);

  if (!letters[65])
  {
    fprintf(stderr, "Phrases() - letters[65] not defined - bailing out.\n");
    return 0;
  }

  dst.x = 320 - (letters[65]->w/2);
  dst.y = 100;
  dst.w = letters[65]->w;
  dst.h = letters[65]->h;

  dst2.x = 50;
  dst2.y = 400;
  dst2.w = letters[65]->w;
  dst2.h = letters[65]->h;

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
          for (i = 0; i < 10; i++)
          { 
            if (((int)pphrase[c] >= 0) /* Prevent bounds violation */
              &&((int)pphrase[c] < 256) 
              && FINGER[(int)pphrase[c]][i])
            {
              SDL_BlitSurface(hand[i], NULL, screen, &hand_loc);
            }
          }
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
        for (i = 0; i < 10; i++)
        {
          if (((int)pphrase[c] >= 0) /* Prevent bounds violation */
            &&((int)pphrase[c] < 256) /* This can't actually occur for type char */
            && FINGER[(int)pphrase[c]][i])
          {
            SDL_BlitSurface(hand[i], NULL, screen, &hand_loc);
          }
        }
        state = 11;
        break;

      default:
      state -= 2; // this is to make the flashing slower
    }


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
          /* Keep from segfaulting on unicode chars beyond 255 */
          /* until practice mode is fixed:                     */
          if (event.key.keysym.unicode > 255)
          {
            fprintf(stderr, "Practice mode cannot handle this unicode char\n");
            continue;
          }

          if (pphrase[c]==(char)event.key.keysym.unicode)
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


            if (c == (strlen(pphrase) - 1))
            {
              print_at("Great!",6 ,275 ,200);
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


/* FIXME use RenderLetters(), etc */
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
    ||!wrong)
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
  font = LoadFont(settings.theme_font_name, 32 );

  /* FIXME below problem with i18n: */
  let[1]=0;
  for (i=1; i<255; i++)
    /* until we fix or get rid of ALPHABET[], just render the whole range: */
    if (ALPHABET[i])
    {
      let[0] = i;
      letters[i] = BlackOutline(let, font, &white); 
    }

  TTF_CloseFont(font);
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
	for (i = 1; i < 255; i++) 
		if (ALPHABET[i])
                { 
		  SDL_FreeSurface(letters[i]);
                  letters[i] = NULL;
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



static int get_phrase(const char* phr)
{
  int pc = 0;  // 'phrase count' (?)
  int pw[256] = { 0 };
  int wp = 0, i = 0, c = 0, z = 0;
  char fn[FNLEN];

  /* If we didn't receive a phrase get the first one from the file...*/
  if (strncmp("", phr, 40) == 0)
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
    strncpy(phrase[0], phr, 80);
  }


  /* Need to generate glyphs for all the needed Unicode chars: */

  ResetCharList();
  /* 'A' (i.e. 65) always has to go into list because width used for layout */
  /* HACK also need chars for "Great!" because of congrats message - this   */
  /* obviously is not a general solution. Numerals also needed for timers.  */
  {
    char* let = "AGreat!0123456789.";
    GenCharListFromString(let);
  }


  /* Scan through all the phrases and put needed chars into list: */
  for (c = 0; c <= pc; c++)
    GenCharListFromString(phrase[c]);

  /* Now render letters for glyphs in list: */
  font = LoadFont(settings.theme_font_name, 32 );
  if (!font)
  {
    fprintf(stderr, "get_phrase() - could not load font\n");
    return 0;
  }

  RenderLetters(font);

  TTF_CloseFont(font);
  font = NULL;


  //Calculate and record pixel width of phrases
  {
    SDL_Surface* let = NULL;
    for (c = 0; c <= pc; c++)
    {
      for(i = 0; i < strlen(phrase[c]); i++)
      {
        let = GetWhiteGlyph((int)phrase[c][i]);
        if (let)  
          pw[c]+= let->w - 5;
        else
        {
          fprintf(stderr, "get_phrase() - needed glyph not in letters[]\n");
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
        wp = strlen(phrase[c]);
        print_at(phrase[0], wp, 40, 10);
      }
    }
    else
    {
      z = 0;
      wp = 0;

      for (i = 0; i < strlen(phrase[c]); i++)
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
        if (strncmp(" ", &phrase[c][i], 1) == 0)
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



static void print_at(const char *pphrase, int wrap, int x, int y)
{
  int z = 0;
  SDL_Surface* surf = NULL;
  letter_loc.x = x;
  letter_loc.y = y;
  letter_loc.w = GetWhiteGlyph(65)->w;
  letter_loc.h = GetWhiteGlyph(65)->h;

  LOG("Entering print_at()\n");

  if (wrap >= strlen(pphrase)) // I think this means it fits on a single line
  {
    for (z = 0; z <strlen(pphrase); z++)
    {
      surf = GetWhiteGlyph((wchar_t)pphrase[z]);
      if (surf)
      {
        DEBUGCODE{printf("surf not NULL for %c\n", pphrase[z]);}
        SDL_BlitSurface(surf, NULL, screen, &letter_loc);
        letter_loc.x = (letter_loc.x + surf->w) - 5;
      }
      else
      {
        fprintf(stderr, "print_at(): needed glyph for %c not found\n",
                pphrase[z]);
      }
    }
  }
  else  /* Another line required - code only seems to support 1 or 2 lines! */
  {
    for (z = 0; z <= wrap; z++) 
    {
      surf = GetWhiteGlyph((wchar_t)pphrase[z]);
      if (surf)
      {
        DEBUGCODE{printf("surf not NULL for %c\n", pphrase[z]);}
        SDL_BlitSurface(surf, NULL, screen, &letter_loc);
        letter_loc.x = (letter_loc.x + surf->w) - 5;      }
      else
      {
        fprintf(stderr, "print_at(): needed glyph for %c not found\n",
                pphrase[z]);
      }
    }

    /* Move 'cursor' back to left and down one line: */
    letter_loc.x = 40;
    // - (letter_loc.h/4) to account for free space at top and bottom of rendered letters
    letter_loc.y = letter_loc.y + letter_loc.h - (letter_loc.h/4);

    for (z = wrap + 2; z <strlen(pphrase); z++)
    {
      surf = GetWhiteGlyph((wchar_t)pphrase[z]);
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

