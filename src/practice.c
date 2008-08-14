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

static SDL_Surface* bg = NULL;
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
  int next_line=0;
  SDL_Rect dst, dst2, dst4, mydest;
  char keytime[20],
       totaltime[20];
  SDL_Surface* srfc = NULL;
  SDL_Surface* tmpsurf = NULL;
  
  

  if (!practice_load_media())
  {
    fprintf(stderr, "Phrases() - practice_load_media() failed, returning.\n");
    return 0;
  }

  SDL_BlitSurface(bg, NULL, screen, NULL);
  SDL_BlitSurface(keyboard, NULL, screen, &keyboard_loc);
  SDL_BlitSurface(screen, NULL, bg, NULL);
  SDL_BlitSurface(hands, NULL, screen, &hand_loc);
  SDL_Flip(screen);

  wp = get_phrase(pphrase);

  if (!wcsncmp(phrase[0], (wchar_t*)"", 1))
    wcsncpy(pphrase, phrase[0], 80);

  srfc = GetWhiteGlyph(GetLastKey());

  if (!srfc)
  {
    fprintf(stderr, "Phrases() - GetWhiteGlyph(GetLastKey()) not defined - bailing out.\n");
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


  dst4.x = 480;
  dst4.y = 400;
  dst4.w = 240;
  dst4.h = 50;

  dst.x = 40;

  mydest.x = 0;
  mydest.y = dst.y;
  mydest.w = screen->w;
  mydest.h = screen->h-80;


  start = SDL_GetTicks();

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
          int key = GetIndex(pphrase[c]);
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
          int key = GetIndex(pphrase[c]);
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
/**************************************************/
		int key=GetIndex((wchar_t)event.key.keysym.unicode);
		char tmp=-1;
		switch(event.key.keysym.sym)
		{
			case  SDLK_BACKQUOTE:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='~';
				else
					tmp='`';
				break;
			case SDLK_COMMA:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='<';
				else
					tmp=',';
				break;
			case SDLK_MINUS:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='_';
				else
					tmp='-';
				break;
			case SDLK_PERIOD:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='>';
				else
					tmp='.';
				break;
			case SDLK_SLASH:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='?';
				else
					tmp='/';
				break;
			case SDLK_0:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp=')';
				else
					tmp='0';
				break;
			case SDLK_1:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='!';
				else
					tmp='1';
				break;
			case SDLK_2:
					if(event.key.keysym.mod&KMOD_SHIFT)
						tmp='@';
					else
						tmp='2';
				break;
			case SDLK_3:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='#';
				else
					tmp='3';
				break;
			case SDLK_4:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='$';
				else
					tmp='4';
				break;
			case SDLK_5:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='%';
				else
					tmp='5';
				break;
			case SDLK_6:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='^';
				else
					tmp='6';
				break;
			case SDLK_7:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='&';
				else
					tmp='7';
				break;
			case SDLK_8:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='*';
				else
					tmp='8';
				break;
			case SDLK_9:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='(';
				else
					tmp='9';
				break;
			case SDLK_SEMICOLON:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp=':';
				else
					tmp=';';
				break;
			case SDLK_EQUALS:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='+';
				else
					tmp='=';
				break;
			case SDLK_LEFTBRACKET:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='{';
				else
					tmp='[';
				break;
			case SDLK_BACKSLASH:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='|';
				else
					tmp='\\';
				break;
			case SDLK_RIGHTBRACKET:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='}';
				else
					tmp=']';
				break;
			case SDLK_QUOTE:
				if(event.key.keysym.mod&KMOD_SHIFT)
					tmp='"';
				else
					tmp='\'';
				break;
			case SDLK_a:tmp='a';
				break;
			case SDLK_b:tmp='b';
				break;
			case SDLK_c:tmp='c';
				break;
			case SDLK_d:tmp='d';
				break;
			case SDLK_e:tmp='e';
				break;
			case SDLK_f:tmp='f';
				break;
			case SDLK_g:tmp='g';
				break;
			case SDLK_h:tmp='h';
				break;
			case SDLK_i:tmp='i';
				break;
			case SDLK_j:tmp='j';
				break;
			case SDLK_k:tmp='k';
				break;
			case SDLK_l:tmp='l';
				break;
			case SDLK_m:tmp='m';
				break;
			case SDLK_n:tmp='n';
				break;
			case SDLK_o:tmp='o';
				break;
			case SDLK_p:tmp='p';
				break;
			case SDLK_q:tmp='q';
				break;
			case SDLK_r:tmp='r';
				break;
			case SDLK_s:tmp='s';
				break;
			case SDLK_t:tmp='t';
				break;
			case SDLK_u:tmp='u';
				break;
			case SDLK_v:tmp='v';
				break;
			case SDLK_w:tmp='w';
				break;
			case SDLK_x:tmp='x';
				break;
			case SDLK_y:tmp='y';
				break;
			case SDLK_z:tmp='z';
				break;
		}
		if(event.key.keysym.mod&KMOD_SHIFT)
			tmp=toupper(tmp);
		updatekeylist(key,tmp);
/****************************************************/
          if (pphrase[c]==event.key.keysym.unicode)
          {
		state = 0;
		dst2.x = 40;
		dst4.x = 480;
		SDL_BlitSurface(bg, &mydest, screen, &mydest);
		SDL_Flip(screen);
		tmpsurf = BlackOutline_w(pphrase+((wp)*next_line), &white, (c)%wp+1);
		SDL_BlitSurface(tmpsurf, NULL, screen, &dst);
		SDL_FreeSurface(tmpsurf);
		tmpsurf = NULL;
		tmpsurf=BlackOutline_c(keytime, &white);
		SDL_BlitSurface(tmpsurf, NULL, screen, &dst2);
		SDL_FreeSurface(tmpsurf);
		tmpsurf = NULL;	
		tmpsurf=BlackOutline_c(totaltime, &white);
		SDL_BlitSurface(tmpsurf, NULL, screen, &dst4);
		SDL_FreeSurface(tmpsurf);
		tmpsurf = NULL;
		if (c==(wcslen(pphrase)-1)){
				wchar_t buf[10];
				ConvertFromUTF8(buf, _("Great!"));
				print_at(buf, wcslen(buf), 275, 200);
				SDL_Flip(screen);
				SDL_Delay(2500);
				next_line=0;
				quit=1;
		}
		if (c==wp){
			//c++;
			dst.x=40;
			dst.y=140;
			mydest.y+=40;
			next_line=1;
		}
		c++;
	}
	else
	{
		int key = GetIndex((wchar_t)event.key.keysym.unicode);
		keypress1= GetWrongKeypress(key);
		SDL_BlitSurface(keypress1, NULL, screen, &keyboard_loc);
		SDL_FreeSurface(keypress1);
		state=0;
		
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
  savekeyboard();
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
  DEBUGCODE { printf("Entering practice_load_media\n"); }
  LOG("Loading practice media\n");


  hands = LoadImage("hands/hands.png", IMG_ALPHA);
	hand_shift[0] = LoadImage("hands/none.png", IMG_ALPHA);
	hand_shift[1] = LoadImage("hands/lshift.png", IMG_ALPHA);
	hand_shift[2] = LoadImage("hands/rshift.png", IMG_ALPHA);
	keyboard = LoadImage("keyboard/keyboard.png", IMG_ALPHA);
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
    ||!font
    ||!keyboard
    ||!hand_shift[0]
    ||!hand_shift[1]
    ||!hand_shift[2])
  {
    fprintf(stderr, "practice_load_media() - failed to load needed media \n");
    practice_unload_media();
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
  TTF_CloseFont(font);  /* Don't need it after rendering done */
  font = NULL;
  GenerateKeyboard(keyboard);

  LOG("DONE - Loading practice media\n");
  DEBUGCODE { printf("Leaving practice_load_media\n"); }
  return 1;
}



static void practice_unload_media(void)
{
	int i;
	DEBUGCODE { printf("Entering practice_unload_media\n"); }
	SDL_FreeSurface(bg);
        bg = NULL;
	SDL_FreeSurface(hands);
        hands = NULL;
	for(i=0;i<3;i++)
	{
		SDL_FreeSurface(hand_shift[i]);
	        hand_shift[i] = NULL;
	}
	SDL_FreeSurface(keyboard);
        keyboard = NULL;
	//TTF_CloseFont(font);
	for (i=0; i<10; i++) 
        {
          SDL_FreeSurface(hand[i]);
          hand[i] = NULL;
        }

	Mix_FreeChunk(wrong);
	wrong = NULL;
	DEBUGCODE { printf("Leaving practice_unload_media()\n"); }

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

static void print_at(const wchar_t *pphrase, int wrap, int x, int y)
{
	int z=0;
	SDL_Surface *tmp;
	letter_loc.x = x;
	letter_loc.y = y;
	//font = LoadFont(settings.theme_font_name, 30);
	DEBUGCODE { printf("\n\n\nEntering print_at with : %S\n",pphrase); }
	if ( wrap == wcslen(pphrase) ){
		tmp = BlackOutline_w(pphrase, &white, wrap);
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
	}else{
		tmp = BlackOutline_w(pphrase, &white, wrap+1);
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
		letter_loc.x = 40;
                // - (letter_loc.h/4) to account for free space at top and bottom of rendered letters
		//SDL_FreeSurface(tmp);
		letter_loc.y = letter_loc.y + letter_loc.h - (letter_loc.h/4);
		tmp = BlackOutline_w(pphrase+wrap+2, &white, wcslen(pphrase));
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
	}
	//TTF_CloseFont(font);
	// DEBUGCODE { exit(-1); }
	DEBUGCODE { printf("Leaving print_at \n\n\n"); }
}

static int get_phrase(const wchar_t* wphrase)
{	int pc=0,
	    pw[256] = { 0 },
	    wp=0,
	    i=0,
	    z=0;
//Calculate and record pixel width of phrases
	//Find wrapping point
	if (wcslen(wphrase)<50)
	{
		wp=wcslen(wphrase);
		print_at( wphrase, wp, 40, 10 );
		
	}
	else
	{
		z=0;
		wp=0;
		for (i=0;i<wcslen(wphrase);i++)
		{

			z++;
			if (wp == 0 && z > 50)
			{
				wp = i-1;
				break;
			}
		}
		for (i=wp;i>=0;i--){
			if (wphrase[i] == ' ')
			{
				wp=i-1;
				break;
			}
		}
		print_at( wphrase, wp, 40, 10 );
		
	}
	return(wp);
}


static void next_letter(wchar_t *t, int c)
{
	int i;
	wchar_t buf[30];
        i=ConvertFromUTF8(buf, _("Next letter "));
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
