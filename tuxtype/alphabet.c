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

/* Needed to convert UTF-8 under Windows because we don't have glibc: */
#include "ConvertUTF.h"  

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
static int num_words;
wchar_t word_list[MAX_NUM_WORDS][MAX_WORD_SIZE + 1];
wchar_t char_list[MAX_UNICODES];  // List of distinct letters in list
static int num_chars_used = 0;       // Number of different letters in word list

/* These are the arrays for the red and white letters: */
uni_glyph char_glyphs[MAX_UNICODES];

/* Local function prototypes: */
static void gen_char_list(void);
static int add_char(wchar_t uc);
static void print_keymap(void);
static void set_letters(unsigned char* t);
static void show_letters(void);
static void clear_keyboard(void);
static int convert_from_UTF8(wchar_t* wide_word, const char* UTF8_word);

/*****************************************************/
/*                                                   */
/*          "Public" Functions                       */
/*                                                   */
/*****************************************************/



void LoadKeyboard( void ) {
	unsigned char fn[FNLEN];
	int l;

	clear_keyboard();

	for (l=useEnglish; l<2; l++) {
		sprintf( fn , "%s/keyboard.lst", realPath[l]);
		if (CheckFile(fn)) {
			unsigned char str[255];
			wchar_t wide_str[255];

			FILE *f;
			int i,j;

			f = fopen( fn, "r" );

			/* Should never fail as we just did the same thing in CheckFile(): */
			if (f == NULL)
				continue;

			do {
				fscanf( f, "%[^\n]\n", str);
                                /* Convert to wcs from UTF-8, if needed; */
                                mbstowcs(wide_str, str, strlen(str) + 1);

				if (wcslen(wide_str) > 3) {

					/* format is: FINGER(s)|Char(s) Upper/Lower */

					/* advance past the fingers */

					for (i=0; i<wcslen(wide_str) && wide_str[i] != '|'; i++);

					i++; // pass the '|'
					j = i; 
					ALPHABET[(int)wide_str[j]] = 1;  // first character is default

					for (i++; i<wcslen(wide_str); i++)
						KEYMAP[(int)wide_str[i]] = wide_str[j];

					/* set the fingers for this letter */

					for (i=0; i<j-1; i++)
						if (wide_str[i]>='0' && wide_str[i]<='9')
							FINGER[wide_str[j]][(int)(wide_str[i]-'0')]=1;

					ALPHABET_SIZE++;
				}

			} while (!feof(f));

			fclose(f);

			DEBUGCODE
			{
			  fprintf(stderr, "printing keymap for %s\n", fn);
        		  print_keymap();
			}

			return;
		}
	}
	fprintf( stderr, "Error finding file for keyboard setup!\n" );
}


SDL_Surface* BlackOutline(const unsigned char *t, TTF_Font *font, const SDL_Color *c)
{
  SDL_Surface* out = NULL;
  SDL_Surface* black_letters = NULL;
  SDL_Surface* white_letters = NULL;
  SDL_Surface* bg = NULL;
  SDL_Rect dstrect;
  Uint32 color_key;

  if (!t || !font || !c)
  {
    fprintf(stderr, "BlackOutline(): invalid ptr parameter, returning.");
    return NULL;
  }

  black_letters = TTF_RenderUTF8_Blended(font, t, black);

  if (!black_letters)
  {
    fprintf (stderr, "Warning - BlackOutline() could not create image for %s\n", t);
    return NULL;
  }

  bg = SDL_CreateRGBSurface(SDL_SWSURFACE,
                            (black_letters->w) + 5,
                            (black_letters->h) + 5,
                             32,
                             rmask, gmask, bmask, amask);
  /* Use color key for eventual transparency: */
  color_key = SDL_MapRGB(bg->format, 10, 10, 10);
  SDL_FillRect(bg, NULL, color_key);

  /* Now draw black outline/shadow 2 pixels on each side: */
  dstrect.w = black_letters->w;
  dstrect.h = black_letters->h;

  /* NOTE: can make the "shadow" more or less pronounced by */
  /* changing the parameters of these loops.                */
  for (dstrect.x = 1; dstrect.x < 4; dstrect.x++)
    for (dstrect.y = 1; dstrect.y < 3; dstrect.y++)
      SDL_BlitSurface(black_letters , NULL, bg, &dstrect );

  SDL_FreeSurface(black_letters);

  /* --- Put the color version of the text on top! --- */
  white_letters = TTF_RenderUTF8_Blended(font, t, *c);
  dstrect.x = 1;
  dstrect.y = 1;
  SDL_BlitSurface(white_letters, NULL, bg, &dstrect);
  SDL_FreeSurface(white_letters);

  /* --- Convert to the screen format for quicker blits --- */
  SDL_SetColorKey(bg, SDL_SRCCOLORKEY|SDL_RLEACCEL, color_key);
  out = SDL_DisplayFormatAlpha(bg);
  SDL_FreeSurface(bg);

  return out;
}


/* This version takes a single wide character and renders it with the */
/* Unicode glyph versions of the SDL_ttf functions:                         */
SDL_Surface* BlackOutline_wchar(wchar_t t, TTF_Font *font, const SDL_Color *c)
{
  SDL_Surface* out = NULL;
  SDL_Surface* black_letters = NULL;
  SDL_Surface* white_letters = NULL;
  SDL_Surface* bg = NULL;
  SDL_Rect dstrect;
  Uint32 color_key;

  if (!font || !c)
  {
    fprintf(stderr, "BlackOutline_wchar(): invalid ptr parameter, returning.");
    return NULL;
  }

  black_letters = TTF_RenderGlyph_Blended(font, t, black);

  if (!black_letters)
  {
    fprintf (stderr, "Warning - BlackOutline_wchar() could not create image for %lc\n", t);
    return NULL;
  }

  bg = SDL_CreateRGBSurface(SDL_SWSURFACE,
                            (black_letters->w) + 5,
                            (black_letters->h) + 5,
                             32,
                             rmask, gmask, bmask, amask);
  /* Use color key for eventual transparency: */
  color_key = SDL_MapRGB(bg->format, 10, 10, 10);
  SDL_FillRect(bg, NULL, color_key);

  /* Now draw black outline/shadow 2 pixels on each side: */
  dstrect.w = black_letters->w;
  dstrect.h = black_letters->h;

  /* NOTE: can make the "shadow" more or less pronounced by */
  /* changing the parameters of these loops.                */
  for (dstrect.x = 1; dstrect.x < 4; dstrect.x++)
    for (dstrect.y = 1; dstrect.y < 3; dstrect.y++)
      SDL_BlitSurface(black_letters , NULL, bg, &dstrect );

  SDL_FreeSurface(black_letters);

  /* --- Put the color version of the text on top! --- */
  white_letters = TTF_RenderGlyph_Blended(font, t, *c);
  dstrect.x = 1;
  dstrect.y = 1;
  SDL_BlitSurface(white_letters, NULL, bg, &dstrect);
  SDL_FreeSurface(white_letters);

  /* --- Convert to the screen format for quicker blits --- */
  SDL_SetColorKey(bg, SDL_SRCCOLORKEY|SDL_RLEACCEL, color_key);
  out = SDL_DisplayFormatAlpha(bg);
  SDL_FreeSurface(bg);

  return out;
}


/* FIXME dead code but could be useful*/
static void show_letters(void)
{
	int i, l = 0;
	SDL_Surface* abit;
	SDL_Rect dst;
	int stop = 0;
	unsigned char t[255];

	for (i=0; i<256; i++)
		if (ALPHABET[i])
			t[l++]=i;

	t[l] = 0;

	abit = BlackOutline(t, font, &white);

	dst.x = 320 - (abit->w / 2);
	dst.y = 275;
	dst.w = abit->w;
	dst.h = abit->h;

	SDL_BlitSurface(abit, NULL, screen, &dst);

	SDL_FreeSurface(abit);

	abit = BlackOutline("Alphabet Set To:", font, &white);
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


/* FIXME won't handle Unicode chars beyond 255
/* --- get a letter --- */
wchar_t GetLetter(void)
{
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



/* ClearWordList: clears the number of words
 */
void ClearWordList(void)
{
  int i;
  for (i = 0; i < num_words; i++)
  {
    word_list[i][0] = '\0';
  }
  num_words = 0;
}

/* UseAlphabet(): setups the word_list so that it really
 * returns a LETTER when GetWord() is called
 */
void UseAlphabet(void)
{
	int i;

	LOG("Entering UseAlphabet()\n");

	num_words = 0;
	/* This totally mucks up i18n abilities :( */
	for (i=65; i<90; i++) 
	{
		if (ALPHABET[i]) {
			word_list[num_words][0] = (unsigned char)i;
			word_list[num_words][1] = '\0';
			num_words++;

			DEBUGCODE { fprintf(stderr, "Adding %c\n", (unsigned char)i); }
		}
	}
	/* Make sure list is terminated with null character */
	word_list[num_words][0] = '\0';

	/* Make list of all unicode characters used in word list: */
	gen_char_list();

	DOUT(num_words);
	LOG("Leaving UseAlphabet()\n");
}

/* GetWord: returns a random word that wasn't returned
 * the previous time (unless there is only 1 word!!!)
 */
wchar_t* GetWord(void)
{
	static int last_choice = -1;
	int choice;

	LOG("Entering GetWord()\n");
	DEBUGCODE { fprintf(stderr, "num_words is: %d\n", num_words); }

	/* Now count list to make sure num_words is correct: */

	num_words = 0;
	while (word_list[num_words][0] != '\0')
	{
	  num_words++;
	}

	DEBUGCODE { fprintf(stderr, "After count, num_words is: %d\n", num_words); }

        if (0 == num_words)
	{
	  LOG("No words in list\n");
          return NULL;
	}

        if (num_words > MAX_NUM_WORDS)
	{
	  LOG("Error: num_words greater than array size\n");
          return NULL;
	}

        if (num_words < 0)
	{
	  LOG("Error: num_words negative\n");
          return NULL;
	}

	do {
		choice = (rand() % num_words);
	} while ((choice == last_choice) || (num_words < 2));

	last_choice = choice;

	/* NOTE need %S rather than %s because of wide characters */
	DEBUGCODE { fprintf(stderr, "Selected word is: %S\n", word_list[choice]); }

	return word_list[choice];
}



/* GenerateWordList(): adds the words from a given wordlist
 * it ignores any words too long or that has bad
 * character (such as #)
 */

void GenerateWordList(const char* wordFn)
{
  int j;
  unsigned char temp_word[FNLEN];
  wchar_t temp_wide_word[FNLEN];
  size_t length;

  FILE* wordFile=NULL;

  DEBUGCODE { fprintf(stderr, "Entering GenerateWordList() for file: %s\n", wordFn); }

  num_words = 0;

  /* --- open the file --- */

  wordFile = fopen( wordFn, "r" );

  if ( wordFile == NULL )
  {
    fprintf(stderr, "ERROR: could not load wordlist: %s\n", wordFn );
    fprintf(stderr, "Using ALPHABET instead\n");
    UseAlphabet( );
    return;
  }


  /* --- load words from file named as argument: */

  DEBUGCODE { fprintf(stderr, "WORD FILE OPENNED @ %s\n", wordFn); }

  /* ignore the title (i.e. first line) */
  fscanf( wordFile, "%[^\n]\n", temp_word);

  while (!feof(wordFile) && (num_words < MAX_NUM_WORDS))
  {
    fscanf( wordFile, "%[^\n]\n", temp_word);
    DEBUGCODE {fprintf(stderr, "temp_word = %s\n", temp_word);}

    for (j = 0; j < strlen(temp_word); j++)
    {
      if (temp_word[j] == '\n' || temp_word[j] == '\r')
        temp_word[j] = '\0';
    }

    /* Convert from UTF-8 to wcs and make sure word is usable: */
    /* NOTE need to add one to length arg so terminating '\0' gets added: */
    //length = mbstowcs(temp_wide_word, temp_word, strlen(temp_word) + 1);

    length = convert_from_UTF8(temp_wide_word, temp_word);
    DOUT(length);

    if (length == -1)  /* Means invalid UTF-8 sequence or conversion failed */
    {
      fprintf(stderr, "Word '%s' not added - invalid UTF-8 sequence!\n", temp_word);
      continue;
    }

    if (length == 0)
    {
      fprintf(stderr, "Word '%ls' not added - length is zero\n", temp_wide_word);
      continue;
    }

    if (length > MAX_WORD_SIZE)
    {
      fprintf(stderr, "Word '%s' not added - exceeds %d characters\n",
              temp_word, MAX_WORD_SIZE);
      continue;
    }
;
    if (num_words >= MAX_NUM_WORDS)
    {
      fprintf(stderr, "Word '%s' not added - list has reached max of %d characters\n",
              temp_word, MAX_NUM_WORDS);
      continue;
    }

    /* If we make it to here, OK to add word: */
    /* NOTE we have to add one to the length argument */
    /* to include the terminating null.  */
    //mbstowcs(word_list[num_words], temp_word, strlen(temp_word) + 1);
    DEBUGCODE
    {
      fprintf(stderr, "Adding word: %ls\n", temp_wide_word);
    }

    wcsncpy(word_list[num_words], temp_wide_word, strlen(temp_word) + 1);
    num_words++;
  }
        
  /* Make sure list is terminated with null character */
  word_list[num_words][0] = '\0';

  DOUT(num_words);

  if (num_words == 0)
    UseAlphabet( );

  fclose(wordFile);

  /* Make list of all unicode characters used in word list: */
  gen_char_list();

  LOG("Leaving GenerateWordList()\n");
}







/* Now that the words are stored internally as wchars, we use the */
/* Unicode glyph version of black_outline():                      */
int RenderLetters(const TTF_Font* letter_font)
{
  wchar_t t;
  int i;
  int maxy;

  if (!letter_font)
  {
    fprintf(stderr, "RenderLetters() - invalid TTF_Font* argument!\n");
    return 0;
  }

  /* The following will supercede the old code: */
  i = num_chars_used = 0;
  while (char_list[i] != '\0')
  {
    t = char_list[i];

    if(TTF_GlyphMetrics(font, t, NULL , NULL, NULL,
                        &maxy, NULL) == -1)
    {
      fprintf(stderr, "Could not get glyph metrics for %lc", t);
      i++;
      continue;
    }

    DEBUGCODE
    {
      fprintf(stderr, "Creating SDL_Surface for list element %d, char = %lc\n", i, t);
    }

    char_glyphs[i].unicode_value = t;
    char_glyphs[i].white_glyph = BlackOutline_wchar(t, font, &white);
    char_glyphs[i].red_glyph = BlackOutline_wchar(t, font, &red);
    char_glyphs[i].max_y = maxy;

    i++;
    num_chars_used++;
  }
  /* Remember how many letters we added: */
  return num_chars_used;
}


void FreeLetters(void)
{
  int i;

  for (i = 0; i < num_chars_used; i++)
  {
    SDL_FreeSurface(char_glyphs[i].white_glyph);
    SDL_FreeSurface(char_glyphs[i].red_glyph);
  } 
}


SDL_Surface* GetWhiteGlyph(wchar_t t)
{
  int i;

  for (i = 0;
       (char_glyphs[i].unicode_value != t) && (i <= num_chars_used);
       i++)
  {}

  /* Now return appropriate pointer: */
  if (i > num_chars_used)
  {
    /* Didn't find character: */
    fprintf(stderr, "Could not find glyph for unicode character %lc\n", t);
    return NULL;
  }
  
  /* Return corresponding surface for blitting: */
  return char_glyphs[i].white_glyph;
}



SDL_Surface* GetRedGlyph(wchar_t t)
{
  int i;

  for (i = 0;
       char_glyphs[i].unicode_value != t && i <= num_chars_used;
       i++)
  {}

  /* Now return appropriate pointer: */
  if (i > num_chars_used)
  {
    /* Didn't find character: */
    fprintf(stderr, "Could not find glyph for unicode character %lc\n", t);
    return NULL;
  }
  
  /* Return corresponding surface for blitting: */
  return char_glyphs[i].red_glyph;
}


/* Since SDL drawing just uses the upper left corner, but text needs to be drawn relative to */
/* the glyph origin (i.e. the lower left corner for a character that doesn't go below        */
/* the baseline), we need to convert them - basically just subtracting the max_y, which is   */
/* the glyph's height above the baseline.                                                    */
/*  So - 'x' and 'y' before the function should be the coords where the *origin* is supposed */
/* to be, and after the function they will contain the correct coords for blitting of the    */
/* glypg. OK?                                                                                */
int GetGlyphCoords(wchar_t t, int* x, int* y)
{
  int i;

  for (i = 0;
       char_glyphs[i].unicode_value != t && i <= num_chars_used;
       i++)
  {}

  if (i > num_chars_used)
  {
    /* Didn't find character: */
    fprintf(stderr, "Could not find glyph for unicode character %lc\n", t);
    return 0;
  }
  
  /* Set "upper left" coordinates for blitting (currently, don't need to */
  /* do anything to x):                                                  */
  *y -= char_glyphs[i].max_y;
  return 1;
}

/****************************************************/
/*                                                  */
/*       Local ("private") functions:               */
/*                                                  */
/****************************************************/


/* Creates a list of distinct Unicode characters in */
/* word_list[][] (so the program knows what         */
/* needs to be rendered for the games)              */
static void gen_char_list(void)
{
  int i, j;
  i = j = 0;
  char_list[0] = '\0';

  while (word_list[i][0] != '\0' && i < MAX_NUM_WORDS) 
  {
    j = 0;

    while (word_list[i][j]!= '\0' && j < MAX_WORD_SIZE)
    {
      add_char(word_list[i][j]);
      j++;
    }

    i++;
  }

  DEBUGCODE
  {
    fprintf(stderr, "char_list = %S\n", char_list);
  }
}



/* FIXME this function is currently dead code */
/* --- setup the alphabet --- */
static void set_letters(unsigned char *t) {
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


/* For debugging purposes: */
static void print_keymap(void)
{
  int i;

  for(i = 0; i < 256; i++)
  {
    fprintf(stderr, "i = %d\t(int)KEYMAP[i] = %d\tKEYMAP[i] = %lc\t",
            i, KEYMAP[i], KEYMAP[i]); 
    if(isupper(i) && !islower(i))
      fprintf(stderr, "Upper\n");
    if(!isupper(i) && islower(i))
      fprintf(stderr, "Lower\n");
    if(isupper(i) && islower(i))
      fprintf(stderr, "Both\n");
    if(!isupper(i) && !islower(i))
      fprintf(stderr, "Neither\n");
  }
}


/* Checks to see if the argument is already in the list and adds    */
/* it if necessary.  Returns 1 if char added, 0 if already in list, */
/* -1 if list already up to maximum size:                           */

static int add_char(wchar_t uc)
{
  int i = 0;
  while ((char_list[i] != uc)
      && (char_list[i] != '\0')
      && (i < MAX_UNICODES - 1))          //Because 1 need for null terminator
  {
    i++;
  }

  /* unicode already in list: */
  if (char_list[i] == uc)
  {
    DEBUGCODE{ fprintf(stderr,
                       "Unicode value: %d\tcharacter %lc already in list\n",
                        uc, uc);}
    return 0;
  }

  if (char_list[i] == '\0')
  {
    DEBUGCODE{ fprintf(stderr, "Adding unicode value: %d\tcharacter %lc\n", uc, uc);}
    char_list[i] = uc;
    char_list[i + 1] = '\0';
    return 1;
  }

  if (i == MAX_UNICODES - 1)            //Because 1 need for null terminator
  {
    LOG ("Unable to add unicode - list at max capacity");
    return -1;
  }
}


static void clear_keyboard( void ) {
	int i,j;

	ALPHABET_SIZE = 0;
	for (i=0; i<256; i++) {
		ALPHABET[i]=0;
		for (j=0; j<10; j++)
			FINGER[i][j]=0;
		KEYMAP[i]=i;
	}
}
/* This function just tidies up all the ptr args needed for      */
/* ConvertUTF8toUTF32() from Unicode, Inc. into a neat wrapper.  */
/* It returns -1 on error, otherwise returns the length of the   */
/* converted, null-terminated wchar_t* string now stored in the  */
/* location of the 'wide_word' pointer.                          */
static int convert_from_UTF8(wchar_t* wide_word, const char* UTF8_word)
{
  int i = 0;
  ConversionResult result;
  UTF8 temp_UTF8[FNLEN];
  UTF32 temp_UTF32[FNLEN];

  const UTF8* UTF8_Start = temp_UTF8;
  const UTF8* UTF8_End = &temp_UTF8[FNLEN-1];
  UTF32* UTF32_Start = temp_UTF32;
  UTF32* UTF32_End = &temp_UTF32[FNLEN-1];

  strncpy(temp_UTF8, UTF8_word, FNLEN);

  ConvertUTF8toUTF32(&UTF8_Start, UTF8_End,
                     &UTF32_Start, UTF32_End, 0);

  wide_word[0] = '\0';

  while ((i < FNLEN) && (temp_UTF32[i] != '\0'))
  {
    wide_word[i] = temp_UTF32[i];
    i++; 
  }

  if (i >= FNLEN)
  {
    fprintf(stderr, "convert_from_UTF8(): buffer overflow\n");
    return -1;
  }
  else  //need terminating null:
  {
    wide_word[i] = '\0';
  }

  DEBUGCODE {fprintf(stderr, "wide_word = %ls\n", wide_word);}

  return wcslen(wide_word);
}
