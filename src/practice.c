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
	int next_line=0;
  int key[100] = {0};
  SDL_Rect dst, dst2, dst3, dst4, dst5;
  char keytime[20],
       totaltime[20];
  SDL_Surface* tmp = NULL;
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
	SDL_FreeSurface(srfc);
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
/******************************************************/
		if (pphrase[c]==event.key.keysym.unicode){

						state=0;
						dst2.x=40;
						dst4.x=480;
						SDL_BlitSurface(bg, &dst3, screen, &dst2);
						SDL_BlitSurface(bg, &dst5, screen, &dst4);
						SDL_Flip(screen);


						tmp = BlackOutline_w(pphrase+((wp)*next_line), &white, (c)%wp+1);
						SDL_BlitSurface(tmp, NULL, screen, &dst);
						SDL_FreeSurface(tmp);
						

/*
						print_at(keytime,wcslen(keytime),dst2.x,dst2.y);			
						print_at(totaltime,wcslen(keytime),dst4.x,dst4.y);					

*/
						tmp=BlackOutline_c(keytime, &white);
						SDL_BlitSurface(tmp, NULL, screen, &dst2);
						SDL_FreeSurface(tmp);
						
						tmp=BlackOutline_c(totaltime, &white);
						SDL_BlitSurface(tmp, NULL, screen, &dst4);
						SDL_FreeSurface(tmp);


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
							next_line=1;
						}
						c++;
					} else {
							if ( event.key.keysym.sym != SDLK_RSHIFT && event.key.keysym.sym != SDLK_LSHIFT )
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



static int get_phrase(const wchar_t* wphrase)
{	int pc=0,
	    pw[256] = { 0 },
	    wp=0,
	    i=0,
//	    c=0
	    z=0;
//	char fn[FNLEN];

	/* If we didn't receive a phrase get the first one from the file...*/
	
//	if (strncmp("", phr, 40)==0){
//	FILE *pf;
//	/* set the phrases directory/file */
/*	#ifdef WIN32
		snprintf( fn, FNLEN-1, "userdata/phrases.txt" );
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/phrases.txt", getenv("HOME") );
	#endif

	DEBUGCODE { printf("get_phrases(): phrases file is '%s'\n", fn ); }
	LOG("get_phrases(): trying to open phrases file\n");
	pf = fopen( fn, "r" );
	if (pf == NULL) 
		return(wp);
	while (!feof(pf)) {
		fscanf( pf, "%[^\n]\n", phrase[pc] );
		pc++;
		DEBUGCODE { printf( "%s", phrase[pc] ); }
	}
	fclose( pf );
	pc--;
	} else {
		pc=1;
		strncpy(phrase[0], phr, 80);
	}

	//Calculate and record pixel width of phrases
 		for (c=0;c<=pc;c++){
 			for(i=0; i<strlen(phrase[c]); i++){
				if (letters[(int)phrase[c][i]] == NULL){
					LOG("no letter defined in keyboard.lst\n");
				}
				else
 					pw[c]+= letters[(int)phrase[c][i]]->w-5;
 			}
 		}

	//Find wrapping point
	for ( c=0; c<=pc; c++ ){
			if (pw[c]<598){
				if ( c==0 ){
					wp=strlen(phrase[c]);
					print_at( phrase[0], wp, 40, 10 );
				}
			}else{
				z=0;
				wp=0;
				for (i=0;i<strlen(phrase[c]);i++)
				{
					z += letters[(int)phrase[c][i]]->w-5;
					if (wp == 0 && z > 598){
					wp = i-1;
					break;
					}
				}
				for (i=wp;i>=0;i--){
					if ( strncmp( " ", &phrase[c][i], 1 ) == 0 ){
					wp=i-1;
					break;
					}
				}
				if ( c==0 ){
				print_at( phrase[0], wp, 40, 10 );
				}
			}
	}
	LOG("Leaving get_phrase()\n");
	return(wp);
}
*/




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


static void next_letter(wchar_t *t, int c)
{
	int i;
	wchar_t buf[30];
        i=ConvertFromUTF8(buf, _("Next letter :    "));
	buf[i]=t[c];
	buf[i+1]=0;
        print_at(buf,wcslen(buf),230 ,400);

}
