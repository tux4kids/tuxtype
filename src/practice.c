/**************************************************************************
practice.c 
-  description: practice module
-------------------
begin                : Friday Jan 25, 2003
copyright            : (C) 2003 by Jesse Andrews
email                : jdandr2@uky.edu

Revised extensively: 2007
David Bruce <dbruce@tampabay.rr.com>
Revised extensively: 2008
Sreyas Kurumanghat <k.sreyas@gmail.com>
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

//static SDL_Surface* bg = NULL;
static SDL_Surface* hands = NULL;
static SDL_Surface* hand_shift[3] = {NULL};
static SDL_Surface* keyboard = NULL;
static SDL_Surface* keypress1 = NULL;
static SDL_Surface* keypress2 = NULL;
static SDL_Surface* hand[11] = {NULL};
static SDL_Rect hand_loc, letter_loc,keyboard_loc;
static TTF_Font* font = NULL;

static wchar_t phrase[255][FNLEN];

static Mix_Chunk* wrong = NULL;

/*local function prototypes: */
static int get_phrase(const wchar_t* phr);
static int practice_load_media(void);
static void practice_unload_media(void);
static void print_at(const wchar_t* pphrase, int wrap, int x, int y);
static void show(unsigned char t);
SDL_Surface* GetKeypress1(int index);
SDL_Surface* GetKeypress2(int index);
SDL_Surface* GetWrongKeypress(int index);


/************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)             */
/*                                                                      */
/************************************************************************/


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
      cursor = 0,
      wrap_pt = 0,
      z = 0,
      total = 0,
      state = 0;
  int keytimes[100] = {0};
  int next_line = 0;

  /* Locations to draw strings: */
  SDL_Rect user_text_rect;
  SDL_Rect keytime_rect;
  SDL_Rect totaltime_rect;
  SDL_Rect congrats_rect;
  SDL_Rect mydest;

  char keytime_str[20],
       totaltime_str[20];
  SDL_Surface* srfc = NULL;
  SDL_Surface* tmpsurf = NULL;


  if (!practice_load_media())
  {
    fprintf(stderr, "Phrases() - practice_load_media() failed, returning.\n");
    return 0;
  }

  SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);
  SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
  SDL_BlitSurface(hands, NULL, screen, &hand_loc);
  SDL_Flip(screen);

  /* FIXME get_phrase() does the blitting of the phrase to the screen - this */
  /* is utterly perverse!                                                    */
  wrap_pt = get_phrase(pphrase);

  if (!wcsncmp(phrase[0], (wchar_t*)"", 1))
    wcsncpy(pphrase, phrase[0], 80);

  srfc = GetWhiteGlyph(GetLastKey());

  if (!srfc)
  {
    fprintf(stderr, "Phrases() - GetWhiteGlyph(GetLastKey()) not defined - bailing out.\n");
    return 0;
  }

  /* FIXME Do these locations even need w and h???????? */
  user_text_rect.x = 40;
  user_text_rect.y = 100;
//  user_text_rect.w = srfc->w;
//  user_text_rect.h = srfc->h;

  keytime_rect.x = 50;
  keytime_rect.y = screen->h - 80;
//  keytime_rect.w = srfc->w;
//  keytime_rect.h = srfc->h;

  totaltime_rect.x = screen->w - 160;
  totaltime_rect.y = screen->h - 80;
//  totaltime_rect.w = 240;
//  totaltime_rect.h = 50;

  congrats_rect.x = screen->w/2;
  congrats_rect.y = 200;//screen->h - 80;

  /* This is just a rectangle to redraw everything from the user's text on down: */
  mydest.x = 0;
  mydest.y = user_text_rect.y;
  mydest.w = screen->w;
  mydest.h = screen->h-mydest.y;

  start = SDL_GetTicks();


  /* Begin main event loop for "Practice" activity:  -------- */
  do
  {
    switch (state)
    {
      case 0:
        start = SDL_GetTicks();
        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
        state = 1;
        break;

      case 1:
        if (SDL_GetTicks() - start > 500)
        {
          /* Show finger hint, if available. Note that GetFinger() */
          /* returns negative values on error and never returns a  */
          /* value greater than 9.                                 */
          int key = GetIndex(pphrase[cursor]);
          int fing = GetFinger(key);
          int shift = GetShift(key);
          keypress1= GetKeypress1(key);
          keypress2= GetKeypress2(key);

          if (!keypress1)
          {
            fprintf(stderr, "Phrases() - GetKeypress1 failed, returning.\n");
            return 0;
          }

          if(!keypress2)
          {
            fprintf(stderr, "Phrases() - GetKeypress2 failed, returning.\n");
            return 0;
          }

          SDL_BlitSurface(hands, NULL, screen, &hand_loc);
          if (fing >= 0) 
            SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
          SDL_BlitSurface(hand_shift[shift], NULL, screen, &hand_loc);
          SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
          SDL_BlitSurface(keypress2, NULL, screen, &keyboard_loc);
          SDL_FreeSurface(keypress1);
          SDL_FreeSurface(keypress2);
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
        SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
        state = 12;
        break;

      case 4:
      {
        int key = GetIndex(pphrase[cursor]);
        int fing = GetFinger(key);
        int shift = GetShift(key);
        keypress1= GetKeypress1(key);
        keypress2= GetKeypress2(key);

        if (!keypress1)
        {
          fprintf(stderr, "Phrases() - GetKeypress1 failed, returning.\n");
          return 0;
        }

        if(!keypress2)
        {
          fprintf(stderr, "Phrases() - GetKeypress2 failed, returning.\n");
          return 0;
        }

        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        if (fing >= 0) 
          SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
        SDL_BlitSurface(hand_shift[shift], NULL, screen, &hand_loc);
        SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
        SDL_BlitSurface(keypress2, NULL, screen, &keyboard_loc);
        SDL_FreeSurface(keypress1);
        SDL_FreeSurface(keypress2);
        state = 11;
        break;
      }

      default:
        state -= 2; // this is to make the flashing slower

    }  /*  ----------- End of switch(state) statement-------------- */

    /* This blits "Next letter %c" onto the screen - confusing! */
    next_letter(pphrase, cursor);

    while  (SDL_PollEvent(&event))
    {
      if (event.type == SDL_KEYDOWN)
      {
        int key = GetIndex((wchar_t)event.key.keysym.unicode);
        int shift_pressed = event.key.keysym.mod&KMOD_SHIFT;
        char tmp = -1;

        /* TODO I must be missing something - why aren't we just looking at */
        /* the event.key.keysym.unicode value instead of going through this */
        /* giant switch statement?                                          */

        switch(event.key.keysym.sym)
        {
          case  SDLK_ESCAPE:
            quit = 1;
            break;

          case  SDLK_DOWN: //practice next phase in list
            quit = 2;
            break;

          case  SDLK_BACKQUOTE:
            if(shift_pressed)
              tmp='~';
            else
              tmp='`';
            break;

          case SDLK_COMMA:
            if(shift_pressed)
              tmp='<';
            else
              tmp=',';
            break;

          case SDLK_MINUS:
            if(shift_pressed)
              tmp='_';
            else
              tmp='-';
            break;

          case SDLK_PERIOD:
            if(shift_pressed)
              tmp='>';
            else
              tmp='.';
            break;

          case SDLK_SLASH:
            if(shift_pressed)
              tmp='?';
            else
              tmp='/';
            break;

          case SDLK_0:
            if(shift_pressed)
              tmp=')';
            else
              tmp='0';
            break;

          case SDLK_1:
            if(shift_pressed)
              tmp='!';
            else
              tmp='1';
            break;

          case SDLK_2:
            if(shift_pressed)
              tmp='@';
            else
              tmp='2';
            break;

          case SDLK_3:
            if(shift_pressed)
              tmp='#';
            else
              tmp='3';
            break;

          case SDLK_4:
            if(shift_pressed)
              tmp='$';
            else
              tmp='4';
            break;

          case SDLK_5:
            if(shift_pressed)
              tmp='%';
            else
              tmp='5';
            break;

          case SDLK_6:
            if(shift_pressed)
              tmp='^';
            else
              tmp='6';
            break;

          case SDLK_7:
            if(shift_pressed)
              tmp='&';
            else
              tmp='7';
            break;

          case SDLK_8:
            if(shift_pressed)
              tmp='*';
            else
              tmp='8';
            break;

          case SDLK_9:
            if(shift_pressed)
              tmp='(';
            else
              tmp='9';
            break;

          case SDLK_SEMICOLON:
            if(shift_pressed)
              tmp=':';
            else
              tmp=';';
            break;

          case SDLK_EQUALS:
            if(shift_pressed)
              tmp='+';
            else
              tmp='=';
            break;

          case SDLK_LEFTBRACKET:
            if(shift_pressed)
              tmp='{';
            else
              tmp='[';
            break;

          case SDLK_BACKSLASH:
            if(shift_pressed)
              tmp='|';
            else
              tmp='\\';
            break;

          case SDLK_RIGHTBRACKET:
            if(shift_pressed)
              tmp='}';
            else
              tmp=']';
            break;

          case SDLK_QUOTE:
            if(shift_pressed)
              tmp='"';
            else
              tmp='\'';
            break;

          case SDLK_SPACE:  tmp=' ';  break;
          case SDLK_a:      tmp='a';  break;
          case SDLK_b:      tmp='b';  break;
          case SDLK_c:      tmp='c';  break;
          case SDLK_d:      tmp='d';  break;
          case SDLK_e:      tmp='e';  break;
          case SDLK_f:      tmp='f';  break;
          case SDLK_g:      tmp='g';  break;
          case SDLK_h:      tmp='h';  break;
          case SDLK_i:      tmp='i';  break;
          case SDLK_j:      tmp='j';  break;
          case SDLK_k:      tmp='k';  break;
          case SDLK_l:      tmp='l';  break;
          case SDLK_m:      tmp='m';  break;
          case SDLK_n:      tmp='n';  break;
          case SDLK_o:      tmp='o';  break;
          case SDLK_p:      tmp='p';  break;
          case SDLK_q:      tmp='q';  break;
          case SDLK_r:      tmp='r';  break;
          case SDLK_s:      tmp='s';  break;
          case SDLK_t:      tmp='t';  break;
          case SDLK_u:      tmp='u';  break;
          case SDLK_v:      tmp='v';  break;
          case SDLK_w:      tmp='w';  break;
          case SDLK_x:      tmp='x';  break;
          case SDLK_y:      tmp='y';  break;
          case SDLK_z:      tmp='z';  break;
        }

        /* Change to uppercase if shift used */
        if(shift_pressed)
          tmp=toupper(tmp);
        updatekeylist(key,tmp);

        /* Record elapsed time for this keypress and update running total: */
        a = SDL_GetTicks();
        keytimes[cursor] = a - start;
        total += keytimes[cursor];
        sprintf(keytime_str, "%.2f", (float) keytimes[cursor] / 1000);
        sprintf(totaltime_str, "%.2f", (float) total / 1000);
        start = a;


        /****************************************************/
        /*  ---------- If user typed correct character, handle it: --------------- */
        if (pphrase[cursor] == event.key.keysym.unicode)
        {
          cursor++;
          state = 0;

          if (cursor == wrap_pt + 2) /* wrap onto next line */
          {
            user_text_rect.x = 40;
            user_text_rect.y = user_text_rect.y + user_text_rect.h;
            mydest.y = user_text_rect.y;
            mydest.h = screen->h - mydest.y;
            next_line = 1;
          }

          /* Redraw everything below any "completed" lines of input text: */
          SDL_BlitSurface(CurrentBkgd(), &mydest, screen, &mydest);
          SDL_Flip(screen);

          if(!next_line)
            tmpsurf = BlackOutline_w(pphrase, font, &white, cursor);
          else
            tmpsurf = BlackOutline_w(pphrase + wrap_pt + 1,
                                     font, &white,
                                     cursor - (wrap_pt + 1));
          if (tmpsurf)
          {
            SDL_BlitSurface(tmpsurf, NULL, screen, &user_text_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }


          /* Draw strings for time displays: */
          tmpsurf = BlackOutline(keytime_str, font, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(tmpsurf, NULL, screen, &keytime_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;	
          }

          tmpsurf = BlackOutline(totaltime_str, font, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(tmpsurf, NULL, screen, &totaltime_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          if (cursor==(wcslen(pphrase)))
          {
            tmpsurf = BlackOutline(gettext("Great!"), font, &white);
            if (tmpsurf)
            {
              /* Center message on intended point: */
              int save_x = congrats_rect.x;
              congrats_rect.x -= tmpsurf->w/2;
              SDL_BlitSurface(tmpsurf, NULL, screen, &congrats_rect);
              SDL_FreeSurface(tmpsurf);
              tmpsurf = NULL;
              /* reset rect to prior value: */
              congrats_rect.x = save_x;
            }

            SDL_Flip(screen);
            SDL_Delay(2500);
            next_line = 0;
            quit = 2;
          }
        }
        else  /* -------- handle incorrect key press: -------------*/
        {
          int key = GetIndex((wchar_t)event.key.keysym.unicode);
          keypress1= GetWrongKeypress(key);

          if (keypress1) // avoid segfault if NULL
          {
            SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
            SDL_FreeSurface(keypress1);
          }
          state=0;

          if (event.key.keysym.sym != SDLK_RSHIFT
           && event.key.keysym.sym != SDLK_LSHIFT)
            PlaySound(wrong);
        }
        
      } /* End of "if(event.type == SDL_KEYDOWN)" block  --*/

    }  /* ----- End of SDL_PollEvent() loop -------------- */

    SDL_Flip(screen);
    SDL_Delay(30); /* FIXME should keep frame rate constant */

  }while (!quit);  /* ------- End of main event loop ------------- */

  savekeyboard();

  practice_unload_media();

  return quit;
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
  DEBUGCODE { printf("Entering practice_load_media\n"); }
  LOG("Loading practice media\n");

  LoadBothBkgds("main_bkg.png");

  hands = LoadImage("hands/hands.png", IMG_ALPHA);
  hand_shift[0] = LoadImage("hands/none.png", IMG_ALPHA);
  hand_shift[1] = LoadImage("hands/lshift.png", IMG_ALPHA);
  hand_shift[2] = LoadImage("hands/rshift.png", IMG_ALPHA);
  keyboard = LoadImage("keyboard/keyboard.png", IMG_ALPHA);
//  bg = LoadImage("main_bkg.png", IMG_ALPHA);
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
    ||!CurrentBkgd()
    ||!wrong
    ||!font
    ||!keyboard
    ||!hand_shift[0]
    ||!hand_shift[1]
    ||!hand_shift[2])
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

	/********Position of keyboard image*/
  keyboard_loc.x = screen->w/2 -keyboard->w/2; 
  keyboard_loc.y = screen->h/2;
  keyboard_loc.w = screen->w/8;
  keyboard_loc.h = screen->h/8;

  /* Now render letters for glyphs in alphabet: */
  RenderLetters(font);
  //TTF_CloseFont(font);  /* Don't need it after rendering done */
  //font = NULL;
  GenerateKeyboard(keyboard);

  LOG("DONE - Loading practice media\n");
  DEBUGCODE { printf("Leaving practice_load_media\n"); }
  return 1;
}



static void practice_unload_media(void)
{
  int i;

  FreeBothBkgds();  
//  SDL_FreeSurface(bg);
//  bg = NULL;

  if (hands)
    SDL_FreeSurface(hands);
  hands = NULL;

  for(i = 0; i < 3; i++)
  {
    if (hand_shift[i])
      SDL_FreeSurface(hand_shift[i]);
    hand_shift[i] = NULL;
  }

  if (keyboard)
    SDL_FreeSurface(keyboard);
  keyboard = NULL;

  if (font)
    TTF_CloseFont(font);
  font = NULL;

  for (i = 0; i < 10; i++) 
  {
    if (hand[i])
      SDL_FreeSurface(hand[i]);
    hand[i] = NULL;
  }

  if (wrong)
    Mix_FreeChunk(wrong);
  wrong = NULL;
}


/* looks like dead code: */
static void show(unsigned char t)
{
  SDL_Rect dst;
  SDL_Surface* s = NULL;

  s = GetWhiteGlyph((int)t);
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
  int wrap_pt = 0, i = 0, cursor = 0, z = 0;
  char fn[FNLEN];

  int old_debug_on = settings.debug_on;
  settings.debug_on = 1;

  LOG("Entering get_phrase()\n");

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
      return(wrap_pt);  /* why not just 'return 0;' ??? */

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

  //Find wrapping point
  for (cursor = 0; cursor <= pc; cursor++)
  {
    if (pw[cursor] < 50)  // If the phrase is less than 598 pixels wide
    {
      if (cursor == 0)
      {
        wrap_pt = wcslen(phrase[cursor]);
        print_at(phrase[0], wrap_pt, 40, 10);
      }
    }
    else
    {
      z = 0;
      wrap_pt = 0;

      for (i = 0; i < wcslen(phrase[cursor]); i++)
      {
        /* Should be safe (if no glyph, will have returned above) */
        z ++;
        if (wrap_pt == 0 && z > 50)
        {
          wrap_pt = i - 1;
          break;
        }
      }

      for (i = wrap_pt; i >= 0; i--)
      {
        if (wcsncmp((wchar_t*)" ", &phrase[cursor][i], 1) == 0)
        {
          wrap_pt = i-1;
          break;
        }
      }

      if (cursor == 0)
      {
        print_at(phrase[0], wrap_pt, 40, 10);
      }
    }
  }

  settings.debug_on = old_debug_on;

  LOG("Leaving get_phrase()\n");
  return(wrap_pt);
}

static void print_at(const wchar_t *pphrase, int wrap, int x, int y)
{
	int z=0;
	SDL_Surface *tmp;
	letter_loc.x = x;
	letter_loc.y = y;
	//font = LoadFont(settings.theme_font_name, 30);
	DEBUGCODE {
                    printf("\n\n\nEntering print_at with : %S\n",pphrase);
                    printf("wrap = %d\t wsclen() = %d\n", wrap, wcslen(pphrase));
                  }


	if ( wrap == wcslen(pphrase) ){
                LOG("Wrap not needed\n");

		tmp = BlackOutline_w(pphrase, font, &white, wrap);
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
	}else{
                LOG("Line length exceeded - wrap required\n");

		tmp = BlackOutline_w(pphrase, font, &white, wrap+1);
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
		letter_loc.x = 40;
                // - (letter_loc.h/4) to account for free space at top and bottom of rendered letters
		//SDL_FreeSurface(tmp);
		letter_loc.y = letter_loc.y + letter_loc.h - (letter_loc.h/4);
		tmp = BlackOutline_w(pphrase+wrap+1, font, &white, wcslen(pphrase));
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
	}
	//TTF_CloseFont(font);
	// DEBUGCODE { exit(-1); }
	DEBUGCODE { printf("Leaving print_at \n\n\n"); }
}
static void next_letter(wchar_t *t, int c)
{
	int i;
	wchar_t buf[30];
        i=ConvertFromUTF8(buf, gettext("Next letter "));
	buf[i]=t[c];
	buf[i+1]=0;
        print_at(buf,wcslen(buf),215 ,420);
}

SDL_Surface* GetKeypress1(int index)
{
	char buf[50];
	GetKeyPos(index,buf);
	return (LoadImage(buf, IMG_ALPHA));
}

SDL_Surface* GetWrongKeypress(int index)
{
	char buf[50];
	GetWrongKeyPos(index,buf);
	return (LoadImage(buf, IMG_ALPHA));
}

SDL_Surface* GetKeypress2(int index)
{
	char buf[50];
	GetKeyShift(index,buf);
	return (LoadImage(buf, IMG_ALPHA));
}
