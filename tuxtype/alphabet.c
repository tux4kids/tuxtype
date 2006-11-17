/***************************************************************************
                          alphabet.c 
 -  description: Init SDL
                             -------------------
    begin                : Jan 6 2003
    copyright            : (C) 2003 by Jesse Andrews
    email                : jdandr2@tux4kids.net
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

/* the colors we use throughout the game */

SDL_Color black;
SDL_Color gray;
SDL_Color dark_blue;
SDL_Color red;
SDL_Color white;
SDL_Color yellow;

/* Used for word list functions (see below): */
static int WORD_qty;
unsigned char WORDS[MAX_NUM_WORDS][MAX_WORD_SIZE+1];

/* --- setup the alphabet --- */
void set_letters(unsigned char *t) {
	int i;

	ALPHABET_SIZE = 0;
	for (i=0; i<256; i++)
		ALPHABET[i]=0;

	for (i=0; i<strlen(t); i++)
		if (t[i]!=' ') {
			ALPHABET[(int)t[i]]=1;
			ALPHABET_SIZE++;
		}
}

void clear_keyboard( void ) {
	int i,j;

	ALPHABET_SIZE = 0;
	for (i=0; i<256; i++) {
		ALPHABET[i]=0;
		for (j=0; j<10; j++)
			FINGER[i][j]=0;
		KEYMAP[i]=i;
	}
}

void LoadKeyboard( void ) {
	unsigned char fn[FNLEN];
	int l;

	clear_keyboard();

	for (l=useEnglish; l<2; l++) {
		sprintf( fn , "%s/keyboard.lst", realPath[l]);
		if ( checkFile(fn) ) {
			unsigned char str[255];
			FILE *f;
			int i,j;

			f = fopen( fn, "r" );

			if (f == NULL)
				continue;

			do {
				fscanf( f, "%[^\n]\n", str);
				if (strlen(str) > 3) {

					/* format is: FINGER(s)|Char(s) Upper/Lower */

					/* advance past the fingers */

					for (i=0; i<strlen(str) && str[i] != '|'; i++);

					i++; // pass the '|'
					j = i; 
					ALPHABET[(int)str[j]] = 1;  // first character is default

					for (i++; i<strlen(str); i++)
						KEYMAP[(int)str[i]] = str[j];

					/* set the fingers for this letter */

					for (i=0; i<j-1; i++)
						if (str[i]>='0' && str[i]<='9')
							FINGER[str[j]][(int)(str[i]-'0')]=1;

					ALPHABET_SIZE++;
				}

			} while (!feof(f));

			fclose(f);

			return;
		}
	}

	fprintf( stderr, "Error finding file for keyboard setup!\n" );
}

SDL_Surface* black_outline(unsigned char *t, TTF_Font *font, SDL_Color *c) {
	SDL_Surface *out, *tmp, *tmp2;
	SDL_Rect dstrect;

	/* --- create the blocky black "outline" of the text --- */
        
        DEBUGCODE { fprintf( stderr, "black_outline of \"%s\"\n", t ); }

	tmp = TTF_RenderText_Shaded(font, t, black,black);
	tmp2 = SDL_CreateRGBSurface(SDL_SWSURFACE, (tmp->w)+5, (tmp->h)+5, BPP, rmask, gmask, bmask, amask);
	out = SDL_DisplayFormatAlpha(tmp2);
	SDL_FreeSurface(tmp2);

	dstrect.w = tmp->w;
	dstrect.h = tmp->h;

        for (dstrect.x = 1; dstrect.x < 4; dstrect.x++)
            for (dstrect.y = 1; dstrect.y < 4; dstrect.y++)
                SDL_BlitSurface( tmp, NULL, out, &dstrect );

	SDL_FreeSurface(tmp);

	/* --- Put the color version of the text on top! --- */

	tmp = TTF_RenderText_Blended(font, t, *c);

	dstrect.x = dstrect.y = 2;

	SDL_BlitSurface(tmp, NULL, out, &dstrect);

	SDL_FreeSurface(tmp);

	/* --- Convert to the screen format for quicker blits --- */

	tmp = SDL_DisplayFormatAlpha(out);
	SDL_FreeSurface(out);

	return tmp;
}

void show_letters( void ) {
	int i, l=0;
	SDL_Surface *abit;
	SDL_Rect dst;
	int stop = 0;
	unsigned char t[255];

	for (i=0; i<256; i++)
		if (ALPHABET[i])
			t[l++]=i;

	t[l] = 0;

	abit = black_outline(t, font, &white);

	dst.x = 320 - (abit->w / 2);
	dst.y = 275;
	dst.w = abit->w;
	dst.h = abit->h;

	SDL_BlitSurface(abit, NULL, screen, &dst);

	SDL_FreeSurface(abit);

	abit = black_outline("Alphabet Set To:", font, &white);
	dst.x = 320 - (abit->w / 2);
	dst.y = 200;
	dst.w = abit->w;
	dst.h = abit->h;

	SDL_BlitSurface(abit, NULL, screen, &dst);

	SDL_UpdateRect(screen, 0, 0, 0 ,0);

	while (!stop) 
		while (SDL_PollEvent(&event)) 
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
				case SDL_KEYDOWN:
				case SDL_MOUSEBUTTONDOWN:
					stop = 1;
			}

	SDL_FreeSurface(abit);
}

/* --- get a letter --- */
unsigned char get_letter(void) {
	static int last = -1; // we don't want to return same letter twice in a row
	int letter;
	do {
		letter = rand() % 255;
	} while ((letter == last && ALPHABET_SIZE > 1) || ALPHABET[letter] == 0);

	last = letter;

	return letter;
}

/******************************************************************************
*                           WORD FILE & DATA STRUCTURE                        *
******************************************************************************/



/* WORDS_init: clears the number of words
 */
void WORDS_init( void ) {
	WORD_qty = 0;
}

/* WORDS_use_alphabet: setups the WORDS so that it really
 * returns a LETTER when WORDS_get() is called
 */
void WORDS_use_alphabet( void ) {
	int i;

	LOG("Entering WORDS_use_alphabet()\n");

	WORD_qty = 0;
	/* This totally mucks up i18n abilities :( */
	for (i=65; i<90; i++) 
	{
		if (ALPHABET[i]) {
			WORDS[WORD_qty][0] = (unsigned char)i;
			WORDS[WORD_qty][1] = '\0';
			WORD_qty++;

			DEBUGCODE { fprintf(stderr, "Adding %c\n", (unsigned char)i); }
		}
	}
	/* Make sure list is terminated with null character */
	WORDS[WORD_qty][0] = '\0';

	DOUT(WORD_qty);
	LOG("Leaving WORDS_use_alphabet()\n");
}

/* WORDS_get: returns a random word that wasn't returned
 * the previous time (unless there is only 1 word!!!)
 */
unsigned char* WORDS_get( void ) {
	static int last_choice = -1;
	int choice;

	LOG("Entering WORDS_get()\n");
	DEBUGCODE { fprintf(stderr, "WORD_qty is: %d\n", WORD_qty); }

	/* Now count list to make sure WORD_qty is correct: */

	WORD_qty = 0;
	while (WORDS[WORD_qty][0] != '\0')
	{
	  WORD_qty++;
	}

	DEBUGCODE { fprintf(stderr, "After count, WORD_qty is: %d\n", WORD_qty); }

        if (0 == WORD_qty)
	{
	  LOG("No words in list\n");
          return NULL;
	}

        if (WORD_qty > MAX_NUM_WORDS)
	{
	  LOG("Error: WORD_qty greater than array size\n");
          return NULL;
	}

        if (WORD_qty < 0)
	{
	  LOG("Error: WORD_qty negative\n");
          return NULL;
	}

	do {
		choice = (rand() % WORD_qty);
	} while ((choice == last_choice) || (WORD_qty < 2));

	last_choice = choice;

	DEBUGCODE { fprintf(stderr, "Selected word is: %s\n", WORDS[choice]); }
	return WORDS[choice];
}



/* WORDS_use: adds the words from a given wordlist
 * it ignores any words too long or that has bad
 * character (such as #)
 */
void WORDS_use( char *wordFn ) {
	int j;
	unsigned char temp_word[FNLEN];
	FILE *wordFile=NULL;

	DEBUGCODE { fprintf(stderr, "Entering WORDS_use() for file: %s\n", wordFn); }

	WORD_qty = 0;

	/* --- open the file --- */

	wordFile = fopen( wordFn, "r" );

	if ( wordFile == NULL ) {
		fprintf(stderr, "ERROR: could not load wordlist: %s\n", wordFn );
		fprintf(stderr, "Using ALPHABET instead\n");
		WORDS_use_alphabet( );
		return;
	}

	
	/* --- load words for this curlevel ---  */


	DEBUGCODE { fprintf(stderr, "WORD FILE OPENNED @ %s\n", wordFn); }

	/* ignore the title */
	fscanf( wordFile, "%[^\n]\n", temp_word);

	while (!feof(wordFile) && (WORD_qty < MAX_NUM_WORDS)) {
		fscanf( wordFile, "%[^\n]\n", temp_word);

		for (j = 0; j < strlen(temp_word); j++)
			if (temp_word[j] == '\n' || temp_word[j] == '\r')
				temp_word[j] = '\0';

		/* --- check its size, if too big, ignore it --- */

		if (strlen(temp_word) >= 1 && strlen(temp_word) <= MAX_WORD_SIZE) {

			/* --- add word --- */
			if (WORD_qty < MAX_NUM_WORDS)
			{
				strcpy( WORDS[WORD_qty], temp_word );
				WORD_qty++;
			}
		}
	}
        
	/* Make sure list is terminated with null character */
	WORDS[WORD_qty][0] = '\0';

	DOUT(WORD_qty);

	if (WORD_qty == 0)
		WORDS_use_alphabet( );

	fclose(wordFile);

	LOG("Leaving WORDS_use()\n");
}
