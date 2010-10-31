/**************************************************************************
practice.c 
-  description: practice module
-------------------
begin                : Friday Jan 25, 2003
copyright            : (C) 2003 by Jesse Andrews
email                : jdandr2@uky.edu

Revised extensively: 2007 and 2008
David Bruce <davidstuartbruce@gmail.com>
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
#include "SDL_extras.h"
#include "convert_utf.h"

#define MAX_PHRASES 256
#define MAX_PHRASE_LENGTH 256
#define MAX_WRAP_LINES 10
#define TEXT_HEIGHT 28
#define SPRITE_FRAME_TIME 200
/* "Local globals" for practice.c */
static int fontsize = 0;
static int medfontsize = 0;
static int bigfontsize = 0;

/* Surfaces for things we want to pre-render: */
static SDL_Surface* hands = NULL;
static SDL_Surface* hand_shift[3] = {NULL};
static SDL_Surface* keyboard = NULL;
static SDL_Surface* keypress1 = NULL;
static SDL_Surface* keypress2 = NULL;
static SDL_Surface* hand[11] = {NULL};
static sprite* tux_stand = NULL;
static sprite* tux_win = NULL;
static SDL_Surface* time_label_srfc = NULL;
static SDL_Surface* chars_label_srfc = NULL;
static SDL_Surface* cpm_label_srfc = NULL;
static SDL_Surface* wpm_label_srfc = NULL;
static SDL_Surface* errors_label_srfc = NULL;
static SDL_Surface* accuracy_label_srfc = NULL;


static wchar_t phrases[MAX_PHRASES][MAX_PHRASE_LENGTH];
static Mix_Chunk* wrong = NULL;
static Mix_Chunk* cheer = NULL;


static int phrase_draw_width = 0; /* How wide before text needs wrapping */
static int num_phrases = 0;


/* Locations for blitting  */

/* Three main areas within window: */
static SDL_Rect left_pane;
static SDL_Rect top_pane;
static SDL_Rect bottom_pane;

/* Locations within left pane: */
static SDL_Rect tux_loc;
static SDL_Rect time_label;
static SDL_Rect time_rect;
static SDL_Rect chars_typed_label;
static SDL_Rect chars_typed_rect;
static SDL_Rect cpm_label;
static SDL_Rect cpm_rect;
static SDL_Rect wpm_label;
static SDL_Rect wpm_rect;
static SDL_Rect errors_label;
static SDL_Rect errors_rect;
static SDL_Rect accuracy_label;
static SDL_Rect accuracy_rect;

/* Locations within top pane: */
static SDL_Rect phr_text_rect;
static SDL_Rect user_text_rect;

/* Locations within bottom pane: */
static SDL_Rect hand_loc;
static SDL_Rect nextletter_rect;
//static SDL_Rect letter_loc;
static SDL_Rect keyboard_loc;


/*local function prototypes: */
static int load_phrases(const char* phrase_file);
static int find_next_wrap(const wchar_t* wstr, int font_size, int width);
static void recalc_positions(void);
static void calc_font_sizes(void);
static int create_labels(void);
static void display_next_letter(const wchar_t* str, Uint16 index);
static int practice_load_media(void);
static void practice_unload_media(void);
//static void show(char t);
SDL_Surface* GetKeypress1(int index);
SDL_Surface* GetKeypress2(int index);
SDL_Surface* GetWrongKeypress(int index);
static void print_load_results(void);

/************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)             */
/*                                                                      */
/************************************************************************/

/*  --------  This is the main function for the 'Practice' activity. ------ */
int Phrases(wchar_t* pphrase )
{
  /* FIXME make variable names more descriptive */
  Uint32 start = 0, a = 0, tuxtime = 0;
  int quit = 0,
      i = 0,
      cursor = 0,
      wrap_pt = 0,
      prev_wrap = 0,
      total = 0,
      state = 0;
  int once_only = 0;
  int correct_chars = 0;
  int wrong_chars = 0;
  float accuracy = 0;
  int cur_phrase = 0;
  int keytimes[MAX_PHRASE_LENGTH] = {0};
  char time_str[20];
  char chars_typed_str[20];
  char cpm_str[20];
  char wpm_str[20];
  char errors_str[20];
  char accuracy_str[20];
  SDL_Surface* tmpsurf = NULL;

  /* Load all needed graphics, strings, sounds.... */
  if (!practice_load_media())
  {
    fprintf(stderr, "Phrases() - practice_load_media() failed, returning.\n\n");
    return 0;
  }

  /* If we got a phrase string arg, use it, otherwise we */
  /* load practice phrases from  the default file:       */
  if (pphrase != NULL
    && wcslen(pphrase) > 0)
  {
    wcsncpy(phrases[0], pphrase, MAX_PHRASE_LENGTH);
    num_phrases = 1;
    once_only = 1;
  }
  else
  {
    num_phrases = load_phrases("phrases.txt");
  }
  /* Set up positions for blitting: */
  recalc_positions();

  start = tuxtime = SDL_GetTicks();


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
        time_str[0] = '\0';
        chars_typed_str[0] = '\0';
        cpm_str[0] = '\0';
        wpm_str[0] = '\0';
        errors_str[0] = '\0';
        accuracy_str[0] = '\0';
        total = 0;
        cursor = 0;
        wrap_pt = 0;
        prev_wrap = 0;
        correct_chars = 0;
        wrong_chars = 0;
        /* No 'break;' so we drop through to do case 1 as well : */

      /* state == 1 means complete redraw needed                          */
      /* If we do this without the case 0 resets, it means the wrap point */
      /* has changed and the text needs redrawing, or that the screen     */
      /* size has changed:                                                */
      case 1:
        /* Draw bkgd before we start */
        /* NOTE the keyboard and hands will get drawn when we drop through to case 2: */
        SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);
        /* Note - the validity of all these surfaces is tested */
        /* in Practice_Load_Media(), so we should be safe.     */

        /* Draw Tux:   */
        SDL_BlitSurface(tux_stand->frame[tux_stand->cur], NULL, screen, &tux_loc);
        /* Draw all the labels for the typing stats: */
        SDL_BlitSurface(time_label_srfc, NULL, screen, &time_label);
        SDL_BlitSurface(chars_label_srfc, NULL, screen, &chars_typed_label);
        SDL_BlitSurface(cpm_label_srfc, NULL, screen, &cpm_label);
        SDL_BlitSurface(wpm_label_srfc, NULL, screen, &wpm_label);
        SDL_BlitSurface(errors_label_srfc, NULL, screen, &errors_label);
        SDL_BlitSurface(accuracy_label_srfc, NULL, screen, &accuracy_label);


        /* Find wrapping point: */
        wrap_pt = find_next_wrap(&phrases[cur_phrase][prev_wrap],
                                  medfontsize, phrase_draw_width);

        /* Draw the phrase to be typed up to the next wrapping point: */
        DEBUGCODE
        {
          wchar_t buf[200];
          wcsncpy(buf, &phrases[cur_phrase][prev_wrap], wrap_pt + 1); 
          buf[wrap_pt + 1]= '\0';
          fprintf(stderr, "wrap_pt = %d\t cursor = %d\t prev_wrap = %d\n",
                          wrap_pt, cursor, prev_wrap);
          fprintf(stderr, "Phrase to be typed is: %S\n", buf);
        }

        tmpsurf = BlackOutline_w(&phrases[cur_phrase][prev_wrap],
                                  medfontsize, &white, wrap_pt + 1);

        if (tmpsurf)
        {
          SDL_BlitSurface(tmpsurf, NULL, screen, &phr_text_rect);
          SDL_FreeSurface(tmpsurf);
          tmpsurf = NULL;
        }

        /* Draw the text the player has typed so far: */

        tmpsurf = BlackOutline_w(&phrases[cur_phrase][prev_wrap],
                                  medfontsize, &white,
                                  cursor - prev_wrap);

        DEBUGCODE
        {
          wchar_t buf[200];
          wcsncpy(buf, &phrases[cur_phrase][prev_wrap], cursor - prev_wrap); 
          buf[cursor - prev_wrap]= '\0';
          fprintf(stderr, "wrap_pt = %d\t cursor = %d\t prev_wrap = %d\n",
                          wrap_pt, cursor, prev_wrap);
          fprintf(stderr, "Text typed so far is: %S\n", buf);
        }

        if (tmpsurf)
        {
          SDL_BlitSurface(tmpsurf, NULL, screen, &user_text_rect);
          SDL_FreeSurface(tmpsurf);
          tmpsurf = NULL;
        }

        /* Update timer: */
        tmpsurf = BlackOutline(time_str, fontsize, &white);
        if (tmpsurf)
        {
          SDL_BlitSurface(tmpsurf, NULL, screen, &time_rect);
          SDL_FreeSurface(tmpsurf);
          tmpsurf = NULL;
        }

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

          SDL_BlitSurface(CurrentBkgd(), &hand_loc, screen, &hand_loc);
          SDL_BlitSurface(hands, NULL, screen, &hand_loc);

          if (fing >= 0) 
            SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);
          SDL_BlitSurface(hand_shift[shift], NULL, screen, &hand_loc);

          if (keypress1)
          {
            SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
            SDL_FreeSurface(keypress1);
            keypress1 = NULL;
          }

          if (keypress2)
          {
            SDL_BlitSurface(keypress2, NULL, screen, &keyboard_loc);
            SDL_FreeSurface(keypress2);
            keypress2 = NULL;
          }     

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
 
        SDL_BlitSurface(CurrentBkgd(), &hand_loc, screen, &hand_loc);
        SDL_BlitSurface(hands, NULL, screen, &hand_loc);

        if (fing >= 0) 
          SDL_BlitSurface(hand[fing], NULL, screen, &hand_loc);

        SDL_BlitSurface(hand_shift[shift], NULL, screen, &hand_loc);

        if (keypress1)
        {
          SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
          SDL_FreeSurface(keypress1);
          keypress1 = NULL;
        }

        if (keypress2)
        {
          SDL_BlitSurface(keypress2, NULL, screen, &keyboard_loc);
          SDL_FreeSurface(keypress2);
          keypress2 = NULL;
        }

        state = 13;
        break;
      }

      default:
        state -= 2; // this is to make the flashing slower

    }  /*  ----------- End of switch(state) statement-------------- */

    /* This blits the next character onto the screen in a large font: */
    display_next_letter(phrases[cur_phrase], cursor);

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
            if (Pause() == 1)
               quit = 1;
            // continue loop and/or redraw screen
            state = 1;
            break;

          case  SDLK_F10:
            SwitchScreenMode();
            recalc_positions();
            create_labels();
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
          /* ignore other keys: */
          default: break;
        }

        /* If state has changed as direct result of keypress (e.g. F10), leave */
        /* poll event loop so we don't treat it as a simple 'wrong' key: */
        if (state == 0
         || state == 1)
          continue; 

        /* Change to uppercase if shift used */
        if(shift_pressed)
          tmp=toupper(tmp);

        if ( key != -1 ) 
          updatekeylist(key,tmp);

        /* Record elapsed time for this keypress and update running total: */
        a = SDL_GetTicks();
        keytimes[cursor] = a - start;
        start = a;

        total += keytimes[cursor];
        sprintf(time_str, "%.2f %s", (float) total / 1000, N_("sec"));
        sprintf(chars_typed_str, "%d", correct_chars);
        sprintf(cpm_str, "%.1f", (float) correct_chars /((float)total/60000));
        sprintf(wpm_str, "%.1f", (float) ((float) correct_chars/5) /((float) total/60000));
        sprintf(errors_str, "%d", wrong_chars);

        /* Calculate accuracy, avoiding divide-by-zero error: */
        if (correct_chars + wrong_chars == 0)
          accuracy = 1;
        else
          accuracy = (float)correct_chars/((float) (correct_chars + wrong_chars));
        sprintf(accuracy_str, "%.1f%%", accuracy * 100); 


        /****************************************************/
        /*  ---------- If user typed correct character, handle it: --------------- */
        if (phrases[cur_phrase][cursor] == event.key.keysym.unicode)
        {
          cursor++;
          correct_chars++;

          /* Handle wrapping if we are at the end of the current display. */
          /* NOTE now also checking for space at end of line so we wrap   */
          /* automatically without waiting for user to type invisible     */
          /* space                                                        */
          if (cursor >= prev_wrap + wrap_pt + 2) /* wrap onto next line */
          {
            /* This will cause the next line of the phrase to be drawn: */
            prev_wrap = prev_wrap + wrap_pt + 2;
            state = 1;
          }
          else if ((cursor == prev_wrap + wrap_pt + 1) /* skip terminal space */
               && (phrases[cur_phrase][cursor] == ' '))
          {
            /* Need to increment cursor another time in this case: */
            cursor ++;
            prev_wrap = prev_wrap + wrap_pt + 2;
            state = 1;
          }
          else
            state = 2;

          /* Redraw everything below any "completed" lines of input text, */
          /* except we don't want to redraw keyboard to avoid flicker:    */
          tmpsurf = BlackOutline_w(&phrases[cur_phrase][prev_wrap],
                                   medfontsize, &white,
                                   cursor - prev_wrap);

          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &user_text_rect, screen, &user_text_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &user_text_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }


          tmpsurf = BlackOutline(time_str, fontsize, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &time_rect, screen, &time_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &time_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          tmpsurf = BlackOutline(chars_typed_str, fontsize, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &chars_typed_rect, screen, &chars_typed_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &chars_typed_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          tmpsurf = BlackOutline(cpm_str, fontsize, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &cpm_rect, screen, &cpm_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &cpm_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          tmpsurf = BlackOutline(wpm_str, fontsize, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &wpm_rect, screen, &wpm_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &wpm_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          tmpsurf = BlackOutline(errors_str, fontsize, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &errors_rect, screen, &errors_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &errors_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          tmpsurf = BlackOutline(accuracy_str, fontsize, &white);
          if (tmpsurf)
          {
            SDL_BlitSurface(CurrentBkgd(), &accuracy_rect, screen, &accuracy_rect);
            SDL_BlitSurface(tmpsurf, NULL, screen, &accuracy_rect);
            SDL_FreeSurface(tmpsurf);
            tmpsurf = NULL;
          }

          SDL_Flip(screen);

          /* If player has completed phrase, celebrate! */
          if (cursor == wcslen(phrases[cur_phrase]))
          {
            /* Draw Tux celebrating: */
            {
              int done = 0;

              PlaySound(cheer);

              while (!done)
              {
                while (SDL_PollEvent(&event))
                {
                  if ((event.type == SDL_KEYDOWN)
                    ||(event.type == SDL_MOUSEBUTTONDOWN))
                    done = 1;
                }

                /* Draw Tux: */
                SDL_BlitSurface(CurrentBkgd(), &tux_loc, screen, &tux_loc);
                if (tux_win && tux_win->frame[tux_win->cur])
                  SDL_BlitSurface(tux_win->frame[tux_win->cur], NULL, screen, &tux_loc);
                SDL_UpdateRect(screen, tux_loc.x, tux_loc.y, tux_loc.w, tux_loc.h);
                NEXT_FRAME(tux_win);
                SDL_Delay(200);
              }
            }

            /* if we are just doing a single phrase passed as arg to function, */
            /* we stop here:                                                   */
            if (once_only)
              quit = 1;
            else  /* means we are using the phrases from the file */
            {
              /* Go on to next phrase, or back to first one if all done */
              if (cur_phrase < num_phrases)
                cur_phrase++;
              else
                cur_phrase = 0;
            }
            state = 0;
          }
        }
        else  /* -------- handle incorrect key press: -------------*/
        {
          // int key = GetIndex((wchar_t)event.key.keysym.unicode);
          if ( key != -1 ) 
          {
            keypress1= GetWrongKeypress(key);
          
            if (keypress1) // avoid segfault if NULL
            {
              SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
              SDL_FreeSurface(keypress1);
            }
          }
          state = 2;

          /* Don't count shift keys as wrong: */
          if (event.key.keysym.sym != SDLK_RSHIFT
           && event.key.keysym.sym != SDLK_LSHIFT)
          {
            /* Also, don't count spacebar as wrong on first char */
            /* after wrap because we automatically skip it above */
            if((cursor != prev_wrap) 
             ||(event.key.keysym.sym != SDLK_SPACE))
            {
              wrong_chars++;
              PlaySound(wrong);
            }
          }
        }
        
      } /* End of "if(event.type == SDL_KEYDOWN)" block  --*/

    }  /* ----- End of SDL_PollEvent() loop -------------- */

    /* Draw next tux frame if appropriate: */
    if ((SDL_GetTicks() - tuxtime) > SPRITE_FRAME_TIME)
    {
      tuxtime = SDL_GetTicks();

      SDL_BlitSurface(CurrentBkgd(), &tux_loc, screen, &tux_loc);

      if (tux_stand && tux_stand->frame[tux_stand->cur])
        SDL_BlitSurface(tux_stand->frame[tux_stand->cur], NULL, screen, &tux_loc);
      NEXT_FRAME(tux_stand);
    }

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

/* Select appropriate font sizes based on the screen we're working with: */
static void calc_font_sizes(void)
{
  fontsize = (screen->h)/28;
  medfontsize = fontsize * 1.5;
  bigfontsize = fontsize * 4;
}

static int practice_load_media(void)
{
  int i;	
  char fn[FNLEN];
  int load_failed = 0;
  int labels_ok = 0;

  DEBUGCODE { printf("Entering practice_load_media\n"); }


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
  }

  /* load tux sprites: */
  tux_win = LoadSprite("tux/win", IMG_ALPHA);
  tux_stand = LoadSprite("tux/stand", IMG_ALPHA);
  /* load needed sounds: */
  wrong = LoadSound("buzz.wav");
  cheer = LoadSound("cheer.wav");

  /* load needed fonts: */
  calc_font_sizes();

  /* create labels: */
  labels_ok = create_labels();

  /* Get out if anything failed to load (except sounds): */
  if (load_failed
    ||!hands
    ||!CurrentBkgd()
    ||!tux_win
    ||!tux_stand
    ||!keyboard
    ||!hand_shift[0]
    ||!hand_shift[1]
    ||!hand_shift[2]
    ||!labels_ok)
  {
    fprintf(stderr, "practice_load_media() - failed to load needed media \n");
    print_load_results();
    return 0;
  }

  /* Now render letters for glyphs in alphabet: */
  /* This is used for keyboard graphic */
  RenderLetters(fontsize);
  GenerateKeyboard(keyboard);

  LOG("DONE - Loading practice media\n");
  DEBUGCODE { printf("Leaving practice_load_media\n"); }
  return 1;
}



static void print_load_results(void)
{
  LOG("\npractice - print_load_results:\n");
 /* Get out if anything failed to load: */
  if (!CurrentBkgd())
    { LOG("CurrentBkgd() did not load\n");}
  if (!hands)
    { LOG("hands did not load\n");}
  if (!tux_win)
    { LOG("tux_win did not load\n");}
  if (!tux_stand)
    { LOG("tux_stand did not load\n");}
  if (!keyboard)
    { LOG("keyboard did not load\n");}
  if (!hand_shift[0])
    { LOG("hand_shift[0] did not load\n");}
  if (!hand_shift[1])
    { LOG("hand_shift[1] did not load\n");}
  if (!hand_shift[2])
    { LOG("hand_shift[2] did not load\n");}
  if (!time_label_srfc)
    { LOG("time_label_srfc did not load\n");}
  if (!chars_label_srfc)
    { LOG("chars_label_srfc did not load\n");}
  if (!cpm_label_srfc)
    { LOG("cpm_label_srfc did not load\n");}
  if (!wpm_label_srfc)
    { LOG("wpm_label_srfc did not load\n");}
  if (!time_label_srfc)
    { LOG("time_label_srfc did not load\n");}
  if (!errors_label_srfc)
    { LOG("errors_label_srfc did not load\n");}
  if (!accuracy_label_srfc)
    { LOG("accuracy_label_srfc did not load\n");}

  LOG("End print_load_results()\n\n");
}

static void recalc_positions(void)
{
  int text_height;
  calc_font_sizes();
  text_height = fontsize * 1.5;

  DEBUGCODE
  {
    fprintf(stderr, "Entering recalc_positions(), screen is %d x %d\n", screen->w, screen->h); 
  }

  if (!keyboard
    ||!tux_win
    ||!tux_win->frame[0]
    ||!hand[0])
  {
    fprintf(stderr, "recalc_positions() - needed ptr invalid - returning\n");
  } 

  /* Screen is divided into three areas or 'panes' : */
  /*
        *************************************
        *        *                          *
        * left   *           top            *
        *        *                          *
        *        ****************************
        *        *                          *
        *        *                          *
        *        *          bottom          *
        *        *                          *
        *        *                          *
        *************************************
  */

  left_pane.x = 0;
  left_pane.y = 0;
  left_pane.w = screen->w * 0.25;
  left_pane.h = screen->h;

  top_pane.x = screen->w * 0.25;
  top_pane.y = 0;
  top_pane.w = screen->w * 0.75;
  top_pane.h = screen->h * 0.4;

  bottom_pane.x = screen->w * 0.25;
  bottom_pane.y = screen->h * 0.4;
  bottom_pane.w = screen->w * 0.75;
  bottom_pane.h = screen->h * 0.6;

  /* Set up all the locations within the left pane: */
  tux_loc.x = left_pane.x + 5;
  tux_loc.y = left_pane.y + 5;
  tux_loc.w = tux_stand->frame[0]->w;
  tux_loc.h = tux_stand->frame[0]->h;

  time_label.x = left_pane.x + 5;
  time_label.y = tux_loc.y + tux_loc.h;
  time_label.w = left_pane.w - 5;
  time_label.h = text_height;

  time_rect.x = left_pane.x + 5;
  time_rect.y = time_label.y + time_label.h;
  time_rect.w = left_pane.w - 5;
  time_rect.h = text_height;

  chars_typed_label.x = left_pane.x + 5;
  chars_typed_label.y = time_rect.y + time_rect.h;
  chars_typed_label.w = left_pane.w - 5;
  chars_typed_label.h = text_height;

  chars_typed_rect.x = left_pane.x + 5;
  chars_typed_rect.y = chars_typed_label.y + chars_typed_label.h;
  chars_typed_rect.w = left_pane.w - 5;
  chars_typed_rect.h = text_height;

  cpm_label.x = left_pane.x + 5;
  cpm_label.y = chars_typed_rect.y + chars_typed_rect.h;
  cpm_label.w = left_pane.w - 5;
  cpm_label.h = text_height;

  cpm_rect.x = left_pane.x + 5;
  cpm_rect.y = cpm_label.y + cpm_label.h;
  cpm_rect.w = left_pane.w - 5;
  cpm_rect.h = text_height;

  wpm_label.x = left_pane.x + 5;
  wpm_label.y = cpm_rect.y + cpm_rect.h;
  wpm_label.w = left_pane.w - 5;
  wpm_label.h = text_height;

  wpm_rect.x = left_pane.x + 5;
  wpm_rect.y = wpm_label.y + wpm_label.h;
  wpm_rect.w = left_pane.w - 5;
  wpm_rect.h = text_height;

  errors_label.x = left_pane.x + 5;
  errors_label.y = wpm_rect.y + wpm_rect.h;
  errors_label.w = left_pane.w - 5;
  errors_label.h = text_height;

  errors_rect.x = left_pane.x + 5;
  errors_rect.y = errors_label.y + errors_label.h;
  errors_rect.w = left_pane.w - 5;
  errors_rect.h = text_height;

  accuracy_label.x = left_pane.x + 5;
  accuracy_label.y = errors_rect.y + errors_rect.h;
  accuracy_label.w = left_pane.w - 5;
  accuracy_label.h = text_height;

  accuracy_rect.x = left_pane.x + 5;
  accuracy_rect.y = accuracy_label.y + accuracy_label.h;
  accuracy_rect.w = left_pane.w - 5;
  accuracy_rect.h = text_height;

  /* Set up all the locations within the top pane: */
  phr_text_rect.x = top_pane.x + 5;
  phr_text_rect.y = top_pane.y + top_pane.h * 0.3;
  phr_text_rect.w = top_pane.w - 5;
  phr_text_rect.h = medfontsize;

  /* we can't just use phr_text_rect.w to calc wrap */
  /* because SDL_BlitSurface() clobbers it: */
  phrase_draw_width = phr_text_rect.w;

  user_text_rect.x = top_pane.x + 5;
  user_text_rect.y = top_pane.y + top_pane.h * 0.6;
  user_text_rect.w = top_pane.w - 5;
  user_text_rect.h = medfontsize * 1.5;

  /* Set up all the locations within the bottom pane: */
  keyboard_loc.x = bottom_pane.x + bottom_pane.w/4 - keyboard->w/4;
  keyboard_loc.y = bottom_pane.y + 5;
  keyboard_loc.w = keyboard->w;
  keyboard_loc.h = keyboard->h;

  hand_loc.x = keyboard_loc.x;
  hand_loc.y = keyboard_loc.y + keyboard_loc.h + 20;
  hand_loc.w = (hand[0]->w);
  hand_loc.h = (hand[0]->h);


  nextletter_rect.x = keyboard_loc.x + keyboard_loc.w - 80;
  nextletter_rect.y = keyboard_loc.y + keyboard_loc.h;
  nextletter_rect.w = bigfontsize * 1.5;
  nextletter_rect.h = bigfontsize * 1.5;

}


static void practice_unload_media(void)
{
  int i;

  FreeBothBkgds();
  FreeLetters(); 

  if (time_label_srfc)
    SDL_FreeSurface(time_label_srfc);
  time_label_srfc = NULL;

  if (chars_label_srfc)
    SDL_FreeSurface(chars_label_srfc);
  chars_label_srfc = NULL;

  if (cpm_label_srfc)
    SDL_FreeSurface(cpm_label_srfc);
  cpm_label_srfc = NULL;

  if (wpm_label_srfc)
    SDL_FreeSurface(wpm_label_srfc);
  wpm_label_srfc = NULL;

  if (errors_label_srfc)
    SDL_FreeSurface(errors_label_srfc);
  errors_label_srfc = NULL;

  if (accuracy_label_srfc)
    SDL_FreeSurface(accuracy_label_srfc);
  accuracy_label_srfc = NULL;

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

  for (i = 0; i < 10; i++) 
  {
    if (hand[i])
      SDL_FreeSurface(hand[i]);
    hand[i] = NULL;
  }

  if (tux_stand)
  {
    FreeSprite(tux_stand);
    tux_stand = NULL;
  }

  if (tux_win)
  {
    FreeSprite(tux_win);
    tux_win = NULL;
  }

  if (cheer)
    Mix_FreeChunk(cheer);
  cheer = NULL;

  if (wrong)
    Mix_FreeChunk(wrong);
  wrong = NULL;
}


// /* looks like dead code: */
// static void show(char t)
// {
//   SDL_Rect dst;
//   SDL_Surface* s = NULL;
// 
//   s = GetWhiteGlyph((int)t);
//   if (!s)
//     return; 
// 
//   dst.x = 320 - (s->w/2);
//   dst.y = 100;
//   dst.w = s->w;
//   dst.h = s->h;
//   SDL_BlitSurface(s, NULL, screen, &dst);
// }


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
    /* Similar check to above but compiler complains unless we */
    /* inspect return value of fscanf():                       */
    if (EOF != fscanf(fp, "%[^\n]\n", buf))
    {
      ConvertFromUTF8(phrases[num_phrases], buf, MAX_PHRASE_LENGTH);
      DEBUGCODE {printf("phrase %d:\t%S\n", num_phrases, phrases[num_phrases]);}
      num_phrases++;
    }
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
static int find_next_wrap(const wchar_t* wstr, int font_size, int width)
{
  wchar_t buf[MAX_PHRASE_LENGTH];
  char UTF8buf[MAX_PHRASE_LENGTH];
  SDL_Surface* s = NULL;
  int word_end = -1;
  int prev_word_end = -1;

  int i = 0;
  int phr_length = 0;
  int test_w = 0;      /* The width in pixels of the SDL-rendered string */
  /* FIXME get rid of this once overhaul done: */

  LOG("Entering find__next_wrap\n");

  /* Make sure args OK: */
  if (!wstr)
  {
    fprintf(stderr, "find_next_wrap() - error - invalid string argument\n");
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
    ConvertToUTF8(buf, UTF8buf, MAX_PHRASE_LENGTH);
    /*  Now check width of string: */
    s = SimpleText(UTF8buf, font_size, &white);
    if (!s)
    {
      /* An error occurred: */
      return -1;
    }

    test_w = s->w;
    SDL_FreeSurface(s);
    s = NULL;

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
        fprintf(stderr, "leaving find_next_wrap()\n");
      }
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


/* FIXME this isn't very safe because index could be out of allocated string, */
/* and there a very good way to test for this within this function.           */
/* Displays the next letter to be typed in a large font */
static void display_next_letter(const wchar_t *str, Uint16 index)
{
  wchar_t ltr[2];
  SDL_Surface* s = NULL;

  if (!str || (index >= MAX_PHRASE_LENGTH))
    return;

  ltr[0] = str[index];
  ltr[1] = '\0';

  s = BlackOutline_w(ltr, bigfontsize, &white, 1);

  if (s)
  {
    SDL_BlitSurface(CurrentBkgd(), &nextletter_rect, screen, &nextletter_rect);
    SDL_BlitSurface(s, NULL, screen, &nextletter_rect);
    SDL_FreeSurface(s);
    s = NULL;
  }
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
	GetKeyShift(index, buf);
	return (LoadImage(buf, IMG_ALPHA));
}

static int create_labels(void)
{
  if (time_label_srfc)
    SDL_FreeSurface(time_label_srfc); 
  time_label_srfc = BlackOutline(_("Time"), fontsize, &yellow);

  if (chars_label_srfc)
    SDL_FreeSurface(chars_label_srfc); 
  chars_label_srfc = BlackOutline(_("Chars"), fontsize, &yellow);

  if (cpm_label_srfc)
    SDL_FreeSurface(cpm_label_srfc);
  cpm_label_srfc = BlackOutline(_("CPM"), fontsize, &yellow);

  if (wpm_label_srfc)
    SDL_FreeSurface(wpm_label_srfc); 
  wpm_label_srfc = BlackOutline(_("WPM"), fontsize, &yellow);

  if (errors_label_srfc)
    SDL_FreeSurface(errors_label_srfc); 
  errors_label_srfc = BlackOutline(_("Errors"), fontsize, &yellow);

  if (accuracy_label_srfc)
    SDL_FreeSurface(accuracy_label_srfc); 
  accuracy_label_srfc = BlackOutline(_("Accuracy"), fontsize, &yellow);

  if (time_label_srfc
   && chars_label_srfc
   && cpm_label_srfc
   && wpm_label_srfc
   && errors_label_srfc
   && accuracy_label_srfc)
    return 1;
  else
    return 0;
}
