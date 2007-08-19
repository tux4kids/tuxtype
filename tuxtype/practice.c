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

SDL_Surface *hands;
SDL_Surface *hand[11];
SDL_Rect hand_loc, letter_loc;
TTF_Font *font;
char phrase[255][FNLEN];

wchar_t wphrase[255][FNLEN];

Mix_Chunk *wrong;

/*local function prototypes: */
static int get_phrase(wchar_t* phr);
static void practice_load_media(void);
static void practice_unload_media(void);
static void print_at(wchar_t *pphrase, int wrap, int x, int y);
static void show(unsigned char t);

void create_letters(wchar_t* widestr);
static void print_string_at(const char *string,int x,int y);
static void next_letter(wchar_t *wphrase, int c);


//static int convert_from_UTF8(wchar_t*, const char*);

//************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)   convert_from_UTF8(wphrase, pphrase);          */
/*                                                                      */
/************************************************************************/

/* FIXME this file is not Unicode-compliant */

int Phrases(char* pphrase ) {

	/* TODO 
	 * 
	 * 
	 * 
	 */
	SDL_Surface *tmp;
	SDL_Surface *clear_text;
        wchar_t wphrase[128];	

	Uint32 start=0,a=0;
	int 	quit=0,
		i=0,
		c=0,
		wp=0,
		z=0,
		total=0,
		state=0;
	int key[100];
	SDL_Rect dst, dst2, dst3, dst4,dst5,clear_dst;
	char keytime[FNLEN],
	     totaltime[FNLEN];

	practice_load_media();

	clear_text = SDL_CreateRGBSurface(SDL_SWSURFACE, 700, 50, 32, rmask, gmask, bmask, amask);
	
	SDL_FillRect(clear_text, NULL, 0xff000000);

	SDL_BlitSurface(bkg, NULL, screen, NULL);
	SDL_BlitSurface(hands, NULL, screen, &hand_loc);
	SDL_Flip(screen);

	convert_from_UTF8(wphrase, pphrase);

	wp = get_phrase(wphrase);
	if (!strncmp(phrase[0], "", 1))
		strncpy(pphrase, phrase[0], 80);

/* 	dst.x = 320 - (letters[65]->w/2);	dst.y = 100;	dst.w = letters[65]->w;	dst.h = letters[65]->h;
 	dst2.x = 50;				dst2.y = 400;	dst2.w = letters[65]->w;	dst2.h = letters[65]->h;
 	dst3.x = 50;				dst3.y = 400;	dst3.w = 160;			dst3.h = 50;
 	dst4.x = 480;				dst4.y = 400;	dst4.w = 240;			dst4.h = 50;
 	dst5.x = 480;				dst5.y = 400;	dst5.w = 240;			dst5.h = 50;
	dst.x = 40;
*/


 	dst.x = 320 - 10;	dst.y = 100;	dst.w = 21;	dst.h = 21;
 	dst2.x = 50;				dst2.y = 400;	dst2.w = 21;	dst2.h = 21;
 	dst3.x = 50;				dst3.y = 400;	dst3.w = 160;			dst3.h = 50;
 	dst4.x = 480;				dst4.y = 400;	dst4.w = 240;			dst4.h = 50;
 	dst5.x = 480;				dst5.y = 400;	dst5.w = 240;			dst5.h = 50;
	dst.x = 40;
	clear_dst.x = dst.x-70;
	clear_dst.y = dst.y;
	clear_dst.w = 700;
	clear_dst.h=40;
	SDL_BlitSurface(clear_text, NULL, screen, &clear_dst);


	start = SDL_GetTicks();

	do {
		switch (state) {
		case 0:
			start = SDL_GetTicks();
			SDL_BlitSurface(hands, NULL, screen, &hand_loc);
			state = 1;
			break;
		case 1:
			if (SDL_GetTicks() - start > 500) {
				for (i=0; i<10; i++)
					if (FINGER[(int)wphrase[c]][i]){

						SDL_BlitSurface(hand[i], NULL, screen, &hand_loc);
					}
				state = 2;
			}
			break;
		case 2:
			if (state == 2 && SDL_GetTicks() - start > 750) {
				state = 3;
			}
			break;
		case 3:
			SDL_BlitSurface(hands, NULL, screen, &hand_loc);
			state = 12;
			break;  
		case 4:
			for (i=0; i<10; i++)
				if (FINGER[(int)wphrase[c]][i])

					SDL_BlitSurface(hand[i], NULL, screen, &hand_loc);
			state = 11;
			break;
		default:
			state -= 2; // this is to make the flashing slower
		}

		next_letter(wphrase, c);

		while  (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					a=SDL_GetTicks();
					key[c]=a-start;
					total += key[c];
					sprintf(keytime, "%.2f", (float) key[c] / 1000);
					sprintf(totaltime, "%.2f", (float) total / 1000);
					start = a;
					if (event.key.keysym.sym == SDLK_ESCAPE)
						quit=1;
					if (event.key.keysym.sym == SDLK_DOWN) {
						//practice next phase in list
						//a=a;
					} else {
						if (wphrase[c]==event.key.keysym.unicode){

						state=0;
						dst2.x=40;
						dst4.x=480;
						SDL_BlitSurface(bkg, &dst3, screen, &dst2);
						SDL_BlitSurface(bkg, &dst5, screen, &dst4);
						SDL_Flip(screen);



						if(c>wp)
						{
							
							dst.y=142;
							clear_dst.y = dst.y;							tmp = create_surface_wchar(wphrase+wp+2,font, &white, (c+2)%wp);
													}
						else
							tmp = create_surface_wchar(wphrase,font, &white, c+1);
						SDL_BlitSurface(clear_text, NULL, screen, &clear_dst);					SDL_BlitSurface(tmp, NULL, screen, &dst);
						SDL_FreeSurface(tmp);
						
						print_string_at(keytime,dst2.x,dst2.y);			
						print_string_at(keytime,dst4.x,dst4.y);					



						if (c==(wcslen(wphrase)-1)){
							print_string_at(_("Great!"),275 ,200);
							SDL_Flip(screen);
							SDL_Delay(2500);
							quit=1;
						}
						if (c==wp){
						//c++;
						dst.x=40;
						dst.y=142;
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
	SDL_FreeSurface(clear_text);
	practice_unload_media();
	return 1;
}



/************************************************************************/
/*                                                                      */ 
/*       "Private" functions (local to practice.c)                      */
/*                                                                      */
/************************************************************************/


/* FIXME use RenderLetters(), etc */
static void practice_load_media(void)
{
	int i;	
	unsigned char fn[FNLEN];
	//unsigned char let[5];

	LOG("Loading practice media\n");
	for (i=0; i<10; i++) {
		sprintf(fn, "hands/%d.png", i);
		hand[i] = LoadImage(fn, IMG_ALPHA);
	}
	hands = LoadImage("hands/hands.png", IMG_ALPHA);

	hand_loc.x = (screen->w/2) - (hand[0]->w/2);
	hand_loc.y = screen->h - (hand[0]->h);
	hand_loc.w = (hand[0]->w);
	hand_loc.h = (hand[0]->h);

	bkg = LoadImage("keyboard/key.png", IMG_ALPHA);

	font = LoadFont( ttf_font, 32 );

	wrong = LoadSound("tock.wav");

	//let[1]=0;

	/*for (i=1; i<255; i++)
	{
		if (ALPHABET[i])
		{
			LOG("letter in ALPHABET[]\n");
			let[0]=i;
			letters[i] = BlackOutline(let, font, &white); 
		}
		else
		{
			LOG("letter NOT in ALPHABET[]\n");
			letters[i] = NULL;
		}
	}
*/
	LOG("DONE - Loading practice media\n");
	TTF_CloseFont(font);

}



static void practice_unload_media(void)
{
	int i;
	SDL_FreeSurface(bkg);
	SDL_FreeSurface(hands);
	//TTF_CloseFont(font);

	for (i=0; i<10; i++) 
		SDL_FreeSurface(hand[i]);

/*	for (i=1; i<255; i++) 
		if (ALPHABET[i]) 
			SDL_FreeSurface(letters[i]);
*/	Mix_FreeChunk(wrong);
}



/*static void show(unsigned char t)
{
	SDL_Rect dst;
	dst.x = 320 - (letters[(int)t]->w/2);
	dst.y = 100;
	dst.w = letters[(int)t]->w;
	dst.h = letters[(int)t]->h;
	SDL_BlitSurface(letters[(int)t], NULL, screen, &dst);
}
*/


static int get_phrase(wchar_t* wphrase)
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


static void print_at(wchar_t *pphrase, int wrap, int x, int y)
{
	int z=0;
	SDL_Surface *tmp;
	letter_loc.x = x;
	letter_loc.y = y;
/*	letter_loc.w = letters[65]->w;
	letter_loc.h = letters[65]->h;
	if ( wrap == strlen(pphrase) ){
		for (z=0;z<strlen(pphrase);z++){
			SDL_BlitSurface(letters[(int)pphrase[z]], NULL, screen, &letter_loc);
			letter_loc.x = (letter_loc.x + letters[(int)pphrase[z]]->w)-5;
		}
	}else{
		for (z=0;z<=wrap;z++){
			SDL_BlitSurface(letters[(int)pphrase[z]], NULL, screen, &letter_loc);
			letter_loc.x = (letter_loc.x + letters[(int)pphrase[z]]->w)-5;
		}
		letter_loc.x = 40;
                // - (letter_loc.h/4) to account for free space at top and bottom of rendered letters
		letter_loc.y = letter_loc.y + letter_loc.h - (letter_loc.h/4);
		for (z=wrap+2;z<strlen(pphrase);z++){
			SDL_BlitSurface(letters[(int)pphrase[z]], NULL, screen, &letter_loc);
			letter_loc.x = (letter_loc.x + letters[(int)pphrase[z]]->w)-5;
		}
	}
}
*/

	font = LoadFont( ttf_font, 32 );
	DEBUGCODE { printf("\n\n\nEntering print_at with : %S\n",pphrase); }
	if ( wrap == wcslen(pphrase) ){
		tmp = create_surface_wchar(pphrase,font, &white, wrap);
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
	}else{
		tmp = create_surface_wchar(pphrase,font, &white, wrap+1);
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
		letter_loc.x = 40;
                // - (letter_loc.h/4) to account for free space at top and bottom of rendered letters
		letter_loc.y = letter_loc.y + letter_loc.h - (letter_loc.h/4);
		tmp = create_surface_wchar(pphrase+wrap+2,font, &white, wcslen(pphrase));
		letter_loc.w = tmp->w+5;
		letter_loc.h = tmp->h+5;
		SDL_BlitSurface(tmp, NULL, screen, &letter_loc);
		SDL_FreeSurface(tmp);
	}
	TTF_CloseFont(font);
	// DEBUGCODE { exit(-1); }
	DEBUGCODE { printf("Leaving print_at \n\n\n"); }
}



static void print_string_at(const char *string,int x,int y)
{
	wchar_t t[512];
	SDL_Surface *tmp;
	SDL_Rect dst;
	dst.x=x;
	dst.y=y;
	dst.h=21;
	dst.w=21;
	convert_from_UTF8(t, string);
	tmp = create_surface_wchar( t, font, &white, wcslen(t));
	SDL_BlitSurface(tmp, NULL, screen, &dst);
	SDL_FreeSurface(tmp);
	
}


static void next_letter(wchar_t *t, int c)
{
	//int i=21;
	SDL_Surface *tmp;
	SDL_Rect dst;
	print_string_at(_("Next letter:"), 230, 400);
	dst.x=320;
	dst.y=350;
	dst.h=100;
	dst.w=100;
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, 100, 100, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(tmp, NULL, screen, &dst);
	SDL_FreeSurface(tmp);
	dst.x=390;	
	dst.y=400;
	dst.h=21;
	dst.w=21;
	tmp = create_surface_wchar( t+c, font, &white,1);
	SDL_BlitSurface(tmp, NULL, screen, &dst);
	SDL_FreeSurface(tmp);
}
