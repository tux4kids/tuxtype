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

#define MAX_PHRASES 256
#define MAX_PHRASE_LENGTH 256
#define MAX_WRAP_LINES 10

/* "Local globals" for practice.c */
static SDL_Surface* hands = NULL;
static SDL_Surface* hand_shift[3] = {NULL};
static SDL_Surface* keyboard = NULL;
static SDL_Surface* keypress1 = NULL;
static SDL_Surface* keypress2 = NULL;
static SDL_Surface* hand[11] = {NULL};
static TTF_Font* font = NULL;

static wchar_t phrases[MAX_PHRASES][MAX_PHRASE_LENGTH];
static Mix_Chunk* wrong = NULL;

/* Locations for blitting  */
static int phrase_draw_width = 0; /* How wide before text needs wrapping */
static SDL_Rect phr_text_rect;
static SDL_Rect user_text_rect;
static SDL_Rect keytime_rect;
static SDL_Rect totaltime_rect;
static SDL_Rect congrats_rect;
static SDL_Rect mydest;
static SDL_Rect hand_loc;
static SDL_Rect letter_loc;
static SDL_Rect keyboard_loc;

/*local function prototypes: */
static int load_phrases(const char* phrase_file);
static int num_phrases = 0;
static int find_next_wrap(const wchar_t* wstr, const TTF_Font* font, int width);
static int get_phrase(const wchar_t* phr);
static void recalc_positions(void);
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

/*  --------  This is the main function for the 'Practice' activity. ------ */
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
      prev_wrap = 0,
      z = 0,
      total = 0,
      state = 0;
  int cur_phrase = 0;
  int keytimes[MAX_PHRASE_LENGTH] = {0};
  int next_line = 0;
  char keytime_str[20],
       totaltime_str[20];
  SDL_Surface* tmpsurf = NULL;

  /* Load all needed graphics, strings, sounds.... */
  if (!practice_load_media())
  {
    fprintf(stderr, "Phrases() - practice_load_media() failed, returning.\n");
    return 0;
  }

  /* Set up positions for blitting: */
  recalc_positions();

  start = SDL_GetTicks();


  /* Begin main event loop for "Practice" activity:  -------- */
  do
  {
    switch (state)
    {
      /* state = 0 means the phrase has changed and we need to reset      */
      /* all the variables related to progress of this phrase; we also    */
      /* need a complete redraw (state = 1)                               */
      case 0:
       /* reset other variables related to progress within phrase: */
        for (i = 0; i < MAX_PHRASE_LENGTH; i++)
          keytimes[i] = 0;
        keytime_str[0] = '\0';
        totaltime_str[0] = '\0';
        total = 0;
        cursor = 0;
        wrap_pt = 0;
        prev_wrap = 0;

        /* No 'break;' so we drop through to do case 1 as well : */

      /* state == 1 means complete redraw needed                          */
      /* If we do this without the case 0 resets, it means the wrap point */
      /* has changed and the text needs redrawing, or that the screen     */
      /* size has changed:                                                */
      case 1:
        /* Draw bkgd before we start */
        /* NOTE the keyboard and hands will get drawn when we drop through to case 2: */
        SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);
//        SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
//        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        /* Draw the phrase to be typed up to the next wrapping point: */
        wrap_pt = find_next_wrap(&phrases[cur_phrase][prev_wrap],
                                  font, phrase_draw_width);
        tmpsurf = BlackOutline_w(&phrases[cur_phrase][prev_wrap],
                                  font, &white, wrap_pt + 1);
        if (tmpsurf)
        {
          SDL_BlitSurface(tmpsurf, NULL, screen, &phr_text_rect);
          SDL_FreeSurface(tmpsurf);
          tmpsurf = NULL;
        }
        /* Draw the text the player has typed so far: */
        tmpsurf = BlackOutline_w(&phrases[cur_phrase][prev_wrap],
                                  font, &white,
                                  cursor - prev_wrap);
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
//        SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);

      /* state == 2 means user has pressed key, either correct or incorrect */
      case 2:
        start = SDL_GetTicks();
        SDL_BlitSurface(CurrentBkgd(), &hand_loc, screen, &hand_loc);
        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
        /* Update entire screen */
        SDL_UpdateRect(screen, 0, 0, 0, 0);
 
        state = 3;
        break;


      case 3: /* Wind up here the next time through the loop: */
              /* If no typing for 0.5 sec, display hint:      */
        if (SDL_GetTicks() - start > 500) 
        {
          /* Show finger hint, if available. Note that GetFinger() */
          /* returns negative values on error and never returns a  */
          /* value greater than 9.                                 */
          int key = GetIndex(phrases[cur_phrase][cursor]);
          int fing = GetFinger(key);
          int shift = GetShift(key);
          keypress1 = GetKeypress1(key);
          keypress2 = GetKeypress2(key);

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

          SDL_BlitSurface(CurrentBkgd(), &hand_loc, screen, &hand_loc);
          SDL_BlitSurface(hands, NULL, screen, &hand_loc);
          if (fing >= 0) 
            SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
          SDL_BlitSurface(hand_shift[shift], NULL, screen, &hand_loc);
          SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
          SDL_BlitSurface(keypress2, NULL, screen, &keyboard_loc);
          SDL_FreeSurface(keypress1);
          SDL_FreeSurface(keypress2);
          state = 4;
        }
        break;

      case 4:
        /* After 0.75 sec, hint starts flashing (alternating states 5 and 6 with delays) */
        if (SDL_GetTicks() - start > 750)
        {
          state = 5;
        }
        break;

      case 5:
        SDL_BlitSurface(CurrentBkgd(), &hand_loc, screen, &hand_loc);
        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
        state = 14;
        break;

      case 6:
      {
        int key = GetIndex(phrases[cur_phrase][cursor]);
        int fing = GetFinger(key);
        int shift = GetShift(key);
        keypress1 = GetKeypress1(key);
        keypress2 = GetKeypress2(key);

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

        SDL_BlitSurface(CurrentBkgd(), &hand_loc, screen, &hand_loc);
        SDL_BlitSurface(hands, NULL, screen, &hand_loc);
        if (fing >= 0) 
          SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
        SDL_BlitSurface(hand_shift[shift], NULL, screen, &hand_loc);
        SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
        SDL_BlitSurface(keypress2, NULL, screen, &keyboard_loc);
        SDL_FreeSurface(keypress1);
        SDL_FreeSurface(keypress2);
        state = 13;
        break;
      }

      default:
        state -= 2; // this is to make the flashing slower

    }  /*  ----------- End of switch(state) statement-------------- */

    /* This blits "Next letter %c" onto the screen - confusing! */
    next_letter(phrases[cur_phrase], cursor);

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

          case  SDLK_F10:
            SwitchScreenMode();
            recalc_positions();
            state = 1;
            break;

          case  SDLK_DOWN: //practice next phase in list
            if (cur_phrase < num_phrases - 1)
            {
              cur_phrase++;
              state = 0;
            }
            break;

          case  SDLK_UP: //practice previous phase in list
            if (cur_phrase > 0)
            {
              cur_phrase--;
              state = 0;
            }
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

        /* If state has changed as direct result of keypress (e.g. F10), leave */
        /* poll event loop so we don't treat it as a simple 'wrong' key: */
        if (state == 0
         || state == 1)
          continue; 

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
        if (phrases[cur_phrase][cursor] == event.key.keysym.unicode)
        {
          cursor++;

 //         state = 2;

          if (cursor >= prev_wrap + wrap_pt + 2) /* wrap onto next line */
          {
            /* This will cause the next line of the phrase to be drawn: */
            prev_wrap = prev_wrap + wrap_pt + 2;
            state = 1;

/*            user_text_rect.x = 40;
            user_text_rect.y = user_text_rect.y + user_text_rect.h;
            mydest.y = user_text_rect.y;
            mydest.h = screen->h - mydest.y;
            next_line = 1;*/
          }
          else
            state = 2;

          /* Redraw everything below any "completed" lines of input text, */
          /* except we don't want to redraw keyboard to avoid flicker:    */

          tmpsurf = BlackOutline_w(&phrases[cur_phrase][prev_wrap],
                                   font, &white,
                                   cursor - prev_wrap);

          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &user_text_rect, screen, &user_text_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &user_text_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }


          /* Draw strings for time displays: */
          tmpsurf = BlackOutline(keytime_str, font, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &keytime_rect, screen, &keytime_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &keytime_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;	
          }

          tmpsurf = BlackOutline(totaltime_str, font, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &totaltime_rect, screen, &totaltime_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &totaltime_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          if (cursor == wcslen(phrases[cur_phrase]))
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
            SDL_Delay(1200);
  //          next_line = 0;
  //          quit = 2;
            /* Go on to next phrase, or back to first one if all done */
            if (cur_phrase < num_phrases)
              cur_phrase++;
            else
              cur_phrase = 0;

            state = 0;
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
          state = 2;

          if (event.key.keysym.sym != SDLK_RSHIFT
           && event.key.keysym.sym != SDLK_LSHIFT)
            PlaySound(wrong);
        }
        
      } /* End of "if(event.type == SDL_KEYDOWN)" block  --*/

    }  /* ----- End of SDL_PollEvent() loop -------------- */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
//    SDL_Flip(screen);
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

  /* load practice phrases from file */
  num_phrases = load_phrases("phrases.txt");

  /* load needed SDL_Surfaces: */
  LoadBothBkgds("main_bkg.png");
  hands = LoadImage("hands/hands.png", IMG_ALPHA);
  hand_shift[0] = LoadImage("hands/none.png", IMG_ALPHA);
  hand_shift[1] = LoadImage("hands/lshift.png", IMG_ALPHA);
  hand_shift[2] = LoadImage("hands/rshift.png", IMG_ALPHA);
  keyboard = LoadImage("keyboard/keyboard.png", IMG_ALPHA);

  for (i = 0; i < 10; i++)
  {
    sprintf(fn, "hands/%d.png", i);
    hand[i] = LoadImage(fn, IMG_ALPHA);
    if (!hand[i])
      load_failed = 1;
  }  /* load needed sounds: */

  wrong = LoadSound("tock.wav");

  /* load needed font: */
  font = LoadFont(settings.theme_font_name, 30);


  /* Get out if anything failed to load: */
  if (load_failed
    ||!num_phrases
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


  /* Now render letters for glyphs in alphabet: */
  /* FIXME do we need this? */
  RenderLetters(font);
  //TTF_CloseFont(font);  /* Don't need it after rendering done */
  //font = NULL;
  GenerateKeyboard(keyboard);

  LOG("DONE - Loading practice media\n");
  DEBUGCODE { printf("Leaving practice_load_media\n"); }
  return 1;
}


static void recalc_positions(void)
{
  /* we can't just use phr_text_rect.w because SDL_BlitSurface() clobbers it: */
  phrase_draw_width = screen->w - 80;

  phr_text_rect.x = 40;
  phr_text_rect.y = 10;

  user_text_rect.x = 40;
  user_text_rect.y = 100;
  user_text_rect.w = screen->w;
  user_text_rect.h = 30;    /* FIXME ideally should calculate from font height */

  keytime_rect.x = 50;
  keytime_rect.y = screen->h - 80;
  keytime_rect.w = 100;
  keytime_rect.h = 30;

  totaltime_rect.x = screen->w - 160;
  totaltime_rect.y = screen->h - 80;
  totaltime_rect.w = 100;
  totaltime_rect.h = 30;

  congrats_rect.x = screen->w/2;
  congrats_rect.y = 200;//screen->h - 80;

  /* This is just a rectangle to redraw everything from the user's text on down: */
  mydest.x = 0;
  mydest.y = user_text_rect.y;
  mydest.w = screen->w;
  mydest.h = screen->h-mydest.y;

  hand_loc.x = (screen->w/2) - (hand[0]->w/2);
  hand_loc.y = screen->h - (hand[0]->h);
  hand_loc.w = (hand[0]->w);
  hand_loc.h = (hand[0]->h);

  /****     Position of keyboard image       */
  keyboard_loc.x = screen->w/2 -keyboard->w/2; 
  keyboard_loc.y = screen->h/2;
  keyboard_loc.w = screen->w/8;
  keyboard_loc.h = screen->h/8;

}

static void practice_unload_media(void)
{
  int i;

  FreeBothBkgds();
  FreeLetters(); 

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


/* Looks for phrases.txt in theme, then in default if not found, */
/* loads it into phrases[][] array.  Returns number of phrases   */
/* successfully loaded.                                          */
static int load_phrases(const char* phrase_file)
{
  int found = 0;
  int num_phrases = 0;
  char buf[MAX_PHRASE_LENGTH];
  FILE* fp;
  char fn[FNLEN];

  /* If using theme, look there first: */
  if (!settings.use_english)
  {
    sprintf(fn , "%s/%s", settings.theme_data_path, phrase_file);
    if (CheckFile(fn))
      found = 1;
  }

  /* Now checking English: */
  if (!found)
  {
    sprintf(fn , "%s/%s", settings.default_data_path, phrase_file);
    if (CheckFile(fn))
      found = 1;
  }

  if (!found)
  {
    fprintf(stderr, "Could not find phrases file '%s' - cannot do Practice\n",
                   phrase_file);
    return 0;
  }

  DEBUGCODE { printf("load_phrases(): phrases file is '%s'\n", fn ); }

  /* We know it will open OK because we already ran CheckFile() on it */ 
  fp = fopen(fn, "r");

  /* So now copy each line into phrases array: */
  /* NOTE we need to convert to wchar_t so just fscanf won't work! */
  while (!feof(fp) && num_phrases <= MAX_PHRASES) 
  {
    fscanf(fp, "%[^\n]\n", buf);
    ConvertFromUTF8(phrases[num_phrases], buf);
    DEBUGCODE {printf("phrase %d:\t%S\n", num_phrases, phrases[num_phrases]);}
    num_phrases++;
  }

  if (num_phrases > MAX_PHRASES)
  {
    fprintf(stderr, "File contains more than MAX_PHRASES - stopping\n");
    num_phrases = MAX_PHRASES;
  }

  fclose(fp);

  DOUT(num_phrases);

  return num_phrases;
}



/* Returns index relative to wstr of last char to be printed before break.  */
/* (i.e. end of last full word that fits within 'width'                     */
static int find_next_wrap(const wchar_t* wstr, const TTF_Font* font, int width)
{
  wchar_t buf[MAX_PHRASE_LENGTH];
  unsigned char UTF8buf[MAX_PHRASE_LENGTH];

  int word_end = -1;
  int prev_word_end = -1;

  int i = 0;
  int phr_length = 0;
  int test_w = 0;      /* The width in pixels of the SDL-rendered string */

  settings.debug_on = 1;
  LOG("Entering find__next_wrap\n");

  /* Make sure args OK: */
  if (!wstr)
  {
    fprintf(stderr, "find_next_wrap() - error - invalid string argument\n");
    return -1;
  }

  if (!font)
  {
    fprintf(stderr, "find_next_wrap() - error - invalid font argument\n");
    return -1;
  }

  DOUT(width);
  DEBUGCODE{ fprintf(stderr, "wstr = %S\n", wstr);}
  
  phr_length = wcslen(wstr);

  DOUT(phr_length);
  /* Using 'MAX_PHRASE_LENGTH - 1' will make sure our copied string is   */
  /* null-terminated, even if it didn't fit.                             */

  if (phr_length > (MAX_PHRASE_LENGTH - 1))
  {
    fprintf(stderr, "find_next_wrap() - error - phrase exceeds MAX_PHRASE_LENGTH\n");
    return -1;
  }

  /* The function will eventually return from within the loop */
  while(1)
  {
    /* Find next either next space or end of string to check width */
    for( ; 
        i < phr_length  &&  wstr[i] != ' ';
        i++);

    DOUT(i);

    /* If exited because space found, back up one so we are at last char in word: */
    if (wstr[i] == ' ')
      word_end = i - 1;
    else
      word_end = i;

    /* See if we have exceeded the width */
    /* Copy string into buf and null terminate after point to be checked: */
    wcsncpy(buf, wstr, MAX_PHRASE_LENGTH);
    buf[word_end + 1] = '\0';
    DEBUGCODE{ fprintf(stderr, "buf = %S\n", buf);}

    /* Need to convert to UTF8 because couldn't get UNICODE version to work: */
    ConvertToUTF8(buf, UTF8buf);
    /*  Now check width of string: */
    if (-1 == TTF_SizeUTF8(font, UTF8buf, &test_w, NULL))
    {
      /* An error occurred: */
      return -1;
    }

    DOUT(test_w);
    DOUT(width);
    /* If we've gone past the width, the previous space was the wrap point, */
    /* whether or not we are at the end of the string:                      */
    if (test_w > width)
    {
      DEBUGCODE
      {
        fprintf(stderr, "width exceeded, returning end of previous word as wrap point\n");
        fprintf(stderr, "prev_word_end is %d\n", prev_word_end); 
      }
      settings.debug_on = 0;
      return prev_word_end; 
    }
    else
    {
      if (i >= phr_length)
      {
        DEBUGCODE
        {
          fprintf(stderr, "width not exceeded, returning because end of string reached\n");
          fprintf(stderr, "word_end is %d\n", word_end); 
        }
        /* We reached the end of the phrase without exceeding the width, */
        /* so just return our current position: */ 
        settings.debug_on = 0;
        return word_end;
      }
      else
      {
        prev_word_end = word_end;
        i++;
      }
    }
  }
}


static int get_phrase(const wchar_t* phr)
{
  int pc = 0;  // 'phrase count' (?)
  int phr_widths[MAX_PHRASES] = { 0 };
  int wrap_pt = 0, i = 0, c = 0, z = 0;
  char fn[FNLEN];
  int old_debug_on = settings.debug_on;
  settings.debug_on = 1;

  LOG("Entering get_phrase()\n");

  /* If we didn't receive a phrase get the first one from the file...*/
  if (wcsncmp((wchar_t*)"", phr, 40) == 0)
  {
    FILE* phrase_file;
    /* set the phrases directory/file */
    /* FIXME I think the phrases should be under data or the theme */
#ifdef WIN32
    snprintf(fn, FNLEN - 1, "userdata/phrases.txt");
#else
    snprintf(fn, FNLEN - 1, (const char*)"%s/.tuxtype/phrases.txt", getenv("HOME"));
#endif

    DEBUGCODE { printf("get_phrases(): phrases file is '%s'\n", fn ); }
    LOG("get_phrases(): trying to open phrases file\n");
    phrase_file = fopen( fn, "r" );
    if (phrase_file == NULL) 
      return(wrap_pt);  /* why not just 'return 0;' ??? */

    /* So now copy each line into phrases array: */
    while (!feof(phrase_file) && pc < 256) 
    {
      fscanf( phrase_file, "%[^\n]\n", phrases[pc] );
      pc++;
      DEBUGCODE {printf("%s", phrases[pc]);}
    }
    if (pc == MAX_PHRASES)
      LOG("File contains more than MAX_PHRASES - stopping\n");

    fclose(phrase_file);
    pc--;
  } 
  else
  {
    LOG("get_phrase() in else clause\n");
    pc = 1;
    wcsncpy(phrases[0], phr, MAX_PHRASE_LENGTH); 
  }

  /* FIXME this seems to be broken - phr_widths[] has not yet been calculated! */
  //Find wrapping point
  for (c = 0; c <= pc; c++)
  {
    if (phr_widths[c] < 50)  // If the phrase is less than 598 pixels wide
    {
      if (c == 0)
      {
        wrap_pt = wcslen(phrases[c]);
        print_at(phrases[0], wrap_pt, 40, 10);
      }
    }
    else
    {
      z = 0;
      wrap_pt = 0;

      for (i = 0; i < wcslen(phrases[c]); i++)
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
        if (wcsncmp((wchar_t*)" ", &phrases[c][i], 1) == 0)
        {
          wrap_pt = i-1;
          break;
        }
      }

      if (c == 0)
      {
        LOG("about to call print_at() near bottom\n");
        print_at(phrases[0], wrap_pt, 40, 10);
      }
    }
  }

  LOG("Leaving get_phrase()\n");

  settings.debug_on = old_debug_on;

  return(wrap_pt);
}

static void print_at(const wchar_t *pphrase, int wrap, int x, int y)
{
  int z = 0;
  SDL_Surface* tmp;
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
	//font = LoadFont(settings.theme_font_name, 30);
  DEBUGCODE
  {
    printf("\n\n\nEntering print_at with : %S\n",pphrase);
    printf("wrap = %d\t wsclen() = %d\n", wrap, wcslen(pphrase));
  }


  if (wrap == wcslen(pphrase))
  {
    LOG("Wrap not needed\n");

    tmp = BlackOutline_w(pphrase, font, &white, wrap);
    if (tmp)
    {
      SDL_BlitSurface(tmp, NULL, screen, &dst);
      SDL_FreeSurface(tmp);
      tmp = NULL;
    }
  }
  else
  {
    LOG("Line length exceeded - wrap required\n");

    tmp = BlackOutline_w(pphrase, font, &white, wrap + 1);
    if (tmp)
    {
      SDL_BlitSurface(tmp, NULL, screen, &dst);
      dst.y += tmp->h;  // move "cursor" down for next line
      SDL_FreeSurface(tmp);
      tmp = NULL;
    }

    tmp = BlackOutline_w(pphrase+wrap+1, font, &white, wcslen(pphrase));
    if (tmp)
    {
      SDL_BlitSurface(tmp, NULL, screen, &dst);
      SDL_FreeSurface(tmp);
    }
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
