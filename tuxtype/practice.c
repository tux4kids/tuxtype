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

Mix_Chunk *wrong;

/*local function prototypes: */
void print_at( char *pphrase, int wrap, int x, int y );

void practice_load_media(void) {
	int i;	
	unsigned char fn[FNLEN];
	unsigned char let[5];

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

	bkg = LoadImage("main_bkg.png", IMG_ALPHA);

	font = LoadFont( ttf_font, 32 );

	wrong = LoadSound("tock.wav");

	let[1]=0;
	for (i=1; i<255; i++)
		if (ALPHABET[i]) {
			let[0]=i;
			letters[i] = black_outline(let, font, &white); 
		}

	LOG("DONE - Loading practice media\n");
	TTF_CloseFont(font);

}

void practice_unload_media(void) {
	int i;
	SDL_FreeSurface(bkg);
	SDL_FreeSurface(hands);
	//TTF_CloseFont(font);

	for (i=0; i<10; i++) 
		SDL_FreeSurface(hand[i]);

	for (i=1; i<255; i++) 
		if (ALPHABET[i]) 
			SDL_FreeSurface(letters[i]);
	Mix_FreeChunk(wrong);
}

void show(unsigned char t) {
	SDL_Rect dst;
	dst.x = 320 - (letters[(int)t]->w/2);
	dst.y = 100;
	dst.w = letters[(int)t]->w;
	dst.h = letters[(int)t]->h;
	SDL_BlitSurface(letters[(int)t], NULL, screen, &dst);
}

int get_phrase(char *phr){
	int pc=0,
	    pw[256] = { 0 },
	    wp=0,
	    i=0,
	    c=0,
	    z=0;
	char fn[FNLEN];

	/* If we didn't receive a phrase get the first one from the file...*/
	
	if (strncmp("", phr, 40)==0){
	FILE *pf;
	/* set the phrases directory/file */
	#ifdef WIN32
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
				if (letters[(int)phrase[c][i]] == NULL)
					printf("no letter defined in keyboard.lst\n");
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
				for (i=0;i<strlen(phrase[c]);i++){
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
	return(wp);
}

void print_at( char *pphrase, int wrap, int x, int y ) {
	int z=0;
	letter_loc.x = x;
	letter_loc.y = y;
	letter_loc.w = letters[65]->w;
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


int Phrases( char *pphrase ) {

	/* TODO 
	 * 
	 * 
	 * 
	 */

	Uint32 start=0,a=0;
	int 	quit=0,
		i=0,
		c=0,
		wp=0,
		z=0,
		total=0,
		state=0;
	int key[100];
	SDL_Rect dst, dst2, dst3, dst4,dst5;
	char keytime[FNLEN],
	     totaltime[FNLEN];

	practice_load_media();
	SDL_BlitSurface(bkg, NULL, screen, NULL);
	SDL_BlitSurface(hands, NULL, screen, &hand_loc);
	SDL_Flip(screen);

	wp = get_phrase(pphrase);
	if (!strncmp(phrase[0], "", 1))
		strncpy(pphrase, phrase[0], 80);

 	dst.x = 320 - (letters[65]->w/2);	dst.y = 100;	dst.w = letters[65]->w;	dst.h = letters[65]->h;
 	dst2.x = 50;				dst2.y = 400;	dst2.w = letters[65]->w;	dst2.h = letters[65]->h;
 	dst3.x = 50;				dst3.y = 400;	dst3.w = 160;			dst3.h = 50;
 	dst4.x = 480;				dst4.y = 400;	dst4.w = 240;			dst4.h = 50;
 	dst5.x = 480;				dst5.y = 400;	dst5.w = 240;			dst5.h = 50;
	dst.x = 40;

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
					if (FINGER[(int)pphrase[c]][i]){
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
				if (FINGER[(int)pphrase[c]][i])
					SDL_BlitSurface(hand[i], NULL, screen, &hand_loc);
			state = 11;
			break;
		default:
			state -= 2; // this is to make the flashing slower
		}

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
						if (ALPHABET[KEYMAP[event.key.keysym.unicode]] && pphrase[c]==(char)event.key.keysym.unicode){
						state=0;
						dst2.x=40;
						dst4.x=480;
						SDL_BlitSurface(bkg, &dst3, screen, &dst2);
						SDL_BlitSurface(bkg, &dst5, screen, &dst4);
						SDL_Flip(screen);
						SDL_BlitSurface(letters[KEYMAP[event.key.keysym.unicode]], NULL, screen, &dst);
						for (z=0;z<strlen(keytime);z++){
 							SDL_BlitSurface(letters[(int)keytime[z]], NULL, screen, &dst2);
							dst2.x = dst2.x + letters[(int)keytime[z]]->w-2;
						}
						for (z=0;z<strlen(totaltime);z++){
							SDL_BlitSurface(letters[(int)totaltime[z]], NULL, screen, &dst4);
							dst4.x = dst4.x + letters[(int)totaltime[z]]->w-2;
						}
						dst.x = (dst.x + letters[KEYMAP[event.key.keysym.unicode]]->w) - 5;
						if (c==(strlen(pphrase)-1)){
							print_at("Great!",6 ,275 ,200);
							SDL_Flip(screen);
							SDL_Delay(2500);
							quit=1;
						}
						if (c==wp){
						c++;
						dst.x=40;
						dst.y=142;
						}
						c++;
						} else {
							if ( event.key.keysym.sym != SDLK_RSHIFT && event.key.keysym.sym != SDLK_LSHIFT )
								playsound( wrong );
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

