/***************************************************************************
                          alphabet.c 
 -  description: Init SDL
                             -------------------
    begin                : Jan 6 2003
    copyright            : (C) 2003 by Jesse Andrews
    email                : jdandr2@tux4kids.net
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



/* Needed to convert UTF-8 under Windows because we don't have glibc: */
#include "ConvertUTF.h"
#include "iconv_string.h"

#include "globals.h"
#include "funcs.h"
#include "SDL_extras.h"



/* An individual item in the list of cached unicode characters that are rendered at   */
/* the start of each game.                                                            */
typedef struct uni_glyph {
  wchar_t unicode_value;
  SDL_Surface* white_glyph;
  SDL_Surface* red_glyph;
} uni_glyph;

/* These are the arrays for the red and white letters: */
static uni_glyph char_glyphs[MAX_UNICODES] = {0, NULL, NULL};

/* An individual item in the list of unicode characters in the keyboard setup.   */
/* Basically, just the Unicode value for the key and the finger used to type it. */
/*typedef struct keymap {
	char keyname[5];
	int shift;
} keymap;*/
typedef struct kbd_char {
  wchar_t unicode_value;
  char finger;
	//keymap key;
	char keyname[5];
	int shift;
	char latin_char;
} kbd_char;

/* List with one entry for each typable character in keyboard setup - has the */
/* Unicode value of the key and the associated fingering.                     */
static kbd_char keyboard_list[MAX_UNICODES] = {0, -1,0,0,-1};



static TTF_Font* font = NULL;

/* Used for word list functions (see below): */
static int num_words;
static wchar_t word_list[MAX_NUM_WORDS][MAX_WORD_SIZE + 1];
static wchar_t char_list[MAX_UNICODES];  // List of distinct letters in word list
static int num_chars_used = 0;       // Number of different letters in word list



/* Local function prototypes: */
static void gen_char_list(void);
static int add_char(wchar_t uc);
static void set_letters(unsigned char* t);
static void show_letters(void);
static void clear_keyboard(void);
static int unicode_in_key_list(wchar_t uni_char);
int check_needed_unicodes_str(const wchar_t* s);
int map_keys(wchar_t wide_char,kbd_char* keyboard_entry);



/*****************************************************/
/*                                                   */
/*          "Public" Functions                       */
/*                                                   */
/*****************************************************/



/* FIXME would be better to get keymap from system somehow (SDL? X11?) - */
/* all this does now is fiddle with the ALPHABET and FINGER arrays */
int LoadKeyboard(void)
{
  unsigned char fn[FNLEN];
  int found = 0;

  clear_keyboard();

  /* First look for keyboard.lst in theme path, if desired: */
  if (!settings.use_english)
  {
    sprintf(fn , "%s/keyboard.lst", settings.theme_data_path);
    if (CheckFile(fn))
    {
      found = 1;
    }
  }

  /* Now look in default path if desired or needed: */
  if (!found)
  {
    sprintf(fn , "%s/keyboard.lst", settings.default_data_path);
    if (CheckFile(fn))
    {
      found = 1;
    }
  }

  if (!found)
  {
    fprintf(stderr, "LoadKeyboard(): Error finding file for keyboard setup!\n");
    return 0;
  }
  
  /* fn should now contain valid path to keyboard.lst: */
  DEBUGCODE{fprintf(stderr, "fn = %s\n", fn);}

  {
    unsigned char str[255];
    wchar_t wide_str[255];

    FILE* f;
    int i = 0, j = 0, k = 0;

    f = fopen( fn, "r" );

    if (f == NULL)
    {
      LOG("LoadKeyboard() - could not open keyboard.lst\n");
      return 0;
    }


    do
    {
      fscanf( f, "%[^\n]\n", str);
      /* Convert to wcs from UTF-8, if needed; */
      //mbstowcs(wide_str, str, strlen(str) + 1);
      ConvertFromUTF8(wide_str, str);

      /* Line must have 3 chars (if more, rest are ignored) */
      /* Format is: FINGER|Char  e.g   "3|d"                */
      /* wide_str[0] == finger used to type char            */
      /* wide_str[1] =='|'
      /* wide_str[2] == Unicode value of character          */

      /* FIXME - this might be a good place to use a    */
      /* hash table to avoid n^2 performance problems.  */
      /* Some sanity checks:  */
      if ((wcslen(wide_str) >=3)
       && (wcstol(&wide_str[0], NULL, 0) >=0)   /* These lines just make sure the */
       && (wcstol(&wide_str[0], NULL, 0) < 10)  /* finger is between 0 and 10     */
       && (wide_str[1] == '|')
       && (k < MAX_UNICODES)
       && !unicode_in_key_list(wide_str[2])) /* Make sure char not already added */
      {
        DEBUGCODE
        {
          fprintf(stderr, "Adding key: Unicode char = '%C'\tUnicode value = %d\tfinger = %d\n",
                  wide_str[2], wide_str[2], wcstol(&wide_str[0], NULL, 0)); 
	}

        /* Just plug values into array: */
        keyboard_list[k].unicode_value = wide_str[2];
        keyboard_list[k].finger = wcstol(&wide_str[0], NULL, 0);
	if (wcslen(wide_str) <5)
	{
		if(!settings.use_english)
		{
			map_keys(-1,&keyboard_list[k]);
			keyboard_list[k].latin_char=-1;
			keyboard_list[k].finger = wcstol(&wide_str[0], NULL, 0);
		}
		else
		{
			map_keys(wide_str[2],&keyboard_list[k]);
			keyboard_list[k].latin_char=wide_str[2];
		}
	}
	else
	{
		map_keys(wide_str[4],&keyboard_list[k]);
		keyboard_list[k].latin_char=wide_str[4];
	}
        k++;
      }
	else
	{
		if(wcslen(wide_str)==1)
		{
			if(!settings.use_english)
			{
				keyboard_list[k].unicode_value = wide_str[0];
				map_keys(-1,&keyboard_list[k]);
				keyboard_list[k].latin_char=-1;
			}
			else
			{
				keyboard_list[k].unicode_value = wide_str[0];
				map_keys(-1,&keyboard_list[k]);
				keyboard_list[k].latin_char=wide_str[0];
			}
			k++;
		}
		
	}
			
    } while (!feof(f));


    fclose(f);
    LOG("Leaving LoadKeyboard()\n");
    return 1;
  }
}

/* Returns the finger hint(0-9) associated with a given Unicode value */
/* in the keyboard_list:                                              */
/* Returns -1 if somehow no finger associated with a Unicode value    */
/* in the list (shouldn't happen).                                    */
/* Returns -2 if Unicode value not in list.                           */
int GetFinger(int i)
{
  if (i == -1)
  {
    fprintf(stderr, "GetFinger() - Unicode char '%C' not found in list.\n",i);
    return -2;
  }

  if ((keyboard_list[i].finger < 0)
   || (keyboard_list[i].finger > 9))
  {
    fprintf(stderr, "GetFinger() - Unicode char '%C' has no valid finger.\n",i);
    return -1;
  }  

  return (int)keyboard_list[i].finger; /* Keep compiler from complaining */
}

int GetShift(int i)
{
	return keyboard_list[i].shift;
}

int GetIndex(wchar_t uni_char)
{
  int i = 0;

  while ((i < MAX_UNICODES)
     &&  (keyboard_list[i].unicode_value != uni_char))
  {
    i++;
  }

  if (i == MAX_UNICODES)
  {
    fprintf(stderr, "GetIndex - Unicode char '%C' not found in list.\n", uni_char);
    return -1;
  }

  return i;
}

void GetKeyPos(int index, char *buf)
{
	sprintf(buf,"keyboard/keyboard_%s.png", keyboard_list[index].keyname);	
}


void GetWrongKeyPos(int index, char *buf)
{
	sprintf(buf,"keyboard/keyboardN_%s.png", keyboard_list[index].keyname);
}


void GetKeyShift(int index, char *buf)
{
	if(keyboard_list[index].shift==0)
		sprintf(buf,"keyboard/keyboard_None.png");
		else
		 	if(keyboard_list[index].shift==1)
				sprintf(buf,"keyboard/keyboard_D00.png");
				else
							sprintf(buf,"keyboard/keyboard_D12.png", settings.default_data_path);			
}


wchar_t GetLastKey()
{
  if(!num_chars_used)
    return -1;
  else
    return keyboard_list[num_chars_used - 1].unicode_value;
} 


int unicode_in_key_list(wchar_t uni_char)
{
  int i = 0;
  while ((i < MAX_UNICODES)
     &&  (keyboard_list[i].unicode_value != uni_char))
  {
    i++;
  }
  if (i == MAX_UNICODES)
    return 0;
  else
    return 1;
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



/* Returns a random Unicode char from the char_glyphs list: */
/* --- get a letter --- */
wchar_t GetRandLetter(void)
{
  static wchar_t last = -1; // we don't want to return same letter twice in a row
  wchar_t letter;
  int i = 0;

  if (!num_chars_used)
  {
    fprintf(stderr, "GetRandLetter() - no letters in list!\n");
    last = -1;
    return -1;
  }

  do
  {
    i = rand() % num_chars_used;
    letter = char_glyphs[i].unicode_value;
  } while (letter == last);

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

/* Now returns the number of words in the list, so if no words */
/* returns "false"                                             */
int GenerateWordList(const char* wordFn)
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
//    UseAlphabet( );
    return 0;
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

    length = ConvertFromUTF8(temp_wide_word, temp_word);
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

    if (num_words >= MAX_NUM_WORDS)
    {
      fprintf(stderr, "Word '%s' not added - list has reached max of %d characters\n",
              temp_word, MAX_NUM_WORDS);
      continue;
    }

    if (!check_needed_unicodes_str(temp_wide_word))
    {
      fprintf(stderr, "Word '%S' not added - contains Unicode chars not in keyboard list\n",
              temp_wide_word);
      continue;
    }

    /* If we make it to here, OK to add word: */
    /* NOTE we have to add one to the length argument */
    /* to include the terminating null.  */
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

//  if (num_words == 0)
//    UseAlphabet( );

  fclose(wordFile);

  /* Make list of all unicode characters used in word list: */
  /* (we use this to check to make sure all are "typable"); */
  gen_char_list();

  LOG("Leaving GenerateWordList()\n");

  return (num_words);
}






/* This version creates the letters using TTF_RenderUNICODE_Blended */
int RenderLetters(const TTF_Font* letter_font)
{
  Uint16 t[2];
  int i, j;  /* i is chars attempted, j is chars actually rendered. */

  if (!letter_font)
  {
    fprintf(stderr, "RenderLetters() - invalid TTF_Font* argument!\n");
    return 0;
  }

  i = j = num_chars_used = 0;

  t[1] = '\0';

  while (i < MAX_UNICODES)
  {
    t[0] = keyboard_list[i].unicode_value;

    if (t[0] != 0)
    {
      DEBUGCODE
      {
        fprintf(stderr, "Creating SDL_Surface for list element %d, char = '%lc', Unicode value = %d\n", i, *t, *t);
      }

      char_glyphs[j].unicode_value = t[0];
      char_glyphs[j].white_glyph = BlackOutline_Unicode(t, letter_font, &white);
      char_glyphs[j].red_glyph = BlackOutline_Unicode(t, letter_font, &red);

      j++;
      num_chars_used++;
    }
    i++;
  }

  return num_chars_used;
}



void FreeLetters(void)
{
  int i;

  for (i = 0; i < num_chars_used; i++)
  {
    SDL_FreeSurface(char_glyphs[i].white_glyph);
    SDL_FreeSurface(char_glyphs[i].red_glyph);
    char_glyphs[i].unicode_value = 0;
    char_glyphs[i].white_glyph = NULL;
    char_glyphs[i].red_glyph = NULL;
  } 
  /* List now empty: */
  num_chars_used = 0;
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
    fprintf(stderr, "Could not find glyph for Unicode char '%C', value = %d\n", t, t);
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


/* Checks to see if all of the glyphs needed by the word list have been     */
/* successfully rendered based on the Unicode values given in keyboard.lst. */
/* If not, then the list contains characters that will not display and (if  */
/* keyboard.lst is correct) cannot be typed. Most likely, this means that   */
/* keyboard.lst is not correct.
/* Returns 1 if all needed chars found, 0 otherwise.                        */
int CheckNeededGlyphs(void)
{
  int i = 0;

  while ((i < MAX_UNICODES)
      && (char_list[i] != '\0'))
  {
    if (!GetWhiteGlyph(char_list[i]))
    {
      fprintf(stderr, "\nCheckNeededGlyphs() - needed char '%C' (Unicode value = %d) not found.\n",
              char_list[i], char_list[i]);
      fprintf(stderr, "This probably means that the theme's 'keyboard.lst' file is incorrect or incomplete.\n");
      return 0;
    }
    i++;
  }
  LOG("CheckNeededGlyphs() - all chars found.\n");
  return 1;
}

int check_needed_unicodes_str(const wchar_t* s)
{
  int i = 0;

  while ((i < MAX_WORD_SIZE)
      && (s[i] != '\0'))
  {
    if (!unicode_in_key_list(s[i]))
    {
      fprintf(stderr, "\ncheck_needed_unicodes_str() - needed char '%C' (Unicode value = %d) not found.\n",
              s[i], s[i]);
      return 0;
    }
    i++;
  }
  return 1;
}

/* FIXME should be able to use iconv() for this one but have not */
/* gotten it to work properly - staying with Unicode Inc.        */
/* implementation for now.

/* This function just tidies up all the ptr args needed for      */
/* ConvertUTF8toUTF32() from Unicode, Inc. into a neat wrapper.  */
/* It returns -1 on error, otherwise returns the length of the   */
/* converted, null-terminated wchar_t* string now stored in the  */
/* location of the 'wide_word' pointer.                          */
int ConvertFromUTF8(wchar_t* wide_word, const unsigned char* UTF8_word)
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

  DEBUGCODE {fprintf(stderr, "wide_word = %ls\t", wide_word);}
  DEBUGCODE {fprintf(stderr, "wcslen(wide_word) = %d\n", wcslen(wide_word));}

  return wcslen(wide_word);
}


/* Now this uses GNU iconv and works correctly!   */
/* This probably could be simplified - not certain */
/* we have to copy into and out of the buffers     */

/******************To be used for savekeyboard*************/
/***Converts wchar_t string to char string*****************/
int ConvertToUTF8(wchar_t* wide_word, unsigned char* UTF8_word)
{
  int i = 0;
  UTF8 temp_UTF8[1024];
  wchar_t temp_wchar_t[1024];

  UTF8* UTF8_Start = &temp_UTF8[0];
  UTF8* UTF8_End = &temp_UTF8[1024 - 1];
  const wchar_t* wchar_t_Start = &temp_wchar_t[0];
  const wchar_t* wchar_t_End = &temp_wchar_t[1024 - 1];
  
  iconv_t conv_descr;
  size_t bytes_converted;
  size_t in_length = (size_t)1024;
  size_t out_length = (size_t)1024;
  wcsncpy(temp_wchar_t, wide_word, 1024);

  DEBUGCODE {fprintf(stderr, "ConvertToUTF8(): wide_word = %S\n", wide_word);}
  DEBUGCODE {fprintf(stderr, "ConvertToUTF8(): temp_wchar_t = %S\n", temp_wchar_t);}

  if (sizeof(wchar_t) == 2)  // Windows wchar_t is 16 bytes
  {
    conv_descr = iconv_open ("UTF-8", "UTF-16");
  }
  else                       // Other platforms, wchar_t 32 bytes
  {
    conv_descr = iconv_open ("UTF-8", "UTF-32");
  }

  bytes_converted = iconv(conv_descr,
                          &wchar_t_Start, &in_length,
                          &UTF8_Start, &out_length);
  iconv_close(conv_descr);

  UTF8_word[0] = 0;

  while ((i < 1024) && (temp_UTF8[i] != 0))
  {
    UTF8_word[i] = temp_UTF8[i];
    i++; 
  }

  if (i >= 1024)
  {
    fprintf(stderr, "ConvertToUTF8(): buffer overflow\n");
    return -1;
  }
  else  //need terminating null:
  {
    UTF8_word[i] = '\0';
  }

  DEBUGCODE {fprintf(stderr, "ConvertToUTF8(): UTF8_word = %s\n", UTF8_word);}
  DEBUGCODE {fprintf(stderr, "ConvertToUTF8(): temp_UTF8 = %s\n", temp_UTF8);}

  return strlen(UTF8_word);
}



/******************************************************************/
int map_keys(wchar_t wide_char,kbd_char* keyboard_entry)
{
	switch(wide_char)
	{
		case '`':strcpy(keyboard_entry->keyname,"A00");
			keyboard_entry->shift=0;
			keyboard_entry->finger=0;
			break;
		case '~':strcpy(keyboard_entry->keyname,"A00");
			keyboard_entry->shift=2;
			keyboard_entry->finger=0;
			break;
		case '1':strcpy(keyboard_entry->keyname,"A01");
			keyboard_entry->shift=0;
			keyboard_entry->finger=0;
			break;
		case '!':strcpy(keyboard_entry->keyname,"A01");
			keyboard_entry->shift=2;
			keyboard_entry->finger=0;
			break;
		case '2':strcpy(keyboard_entry->keyname,"A02");
			keyboard_entry->shift=0;
			keyboard_entry->finger=1;
			break;
		case '@':strcpy(keyboard_entry->keyname,"A02");
			keyboard_entry->shift=2;
			keyboard_entry->finger=1;
			break;
		case '3':strcpy(keyboard_entry->keyname,"A03");
			keyboard_entry->shift=0;
			keyboard_entry->finger=2;
			break;
		case '#':strcpy(keyboard_entry->keyname,"A03");
			keyboard_entry->shift=2;
			keyboard_entry->finger=2;
			break;
		case '4':strcpy(keyboard_entry->keyname,"A04");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case '$':strcpy(keyboard_entry->keyname,"A04");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case '5':strcpy(keyboard_entry->keyname,"A05");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case '%':strcpy(keyboard_entry->keyname,"A05");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case '6':strcpy(keyboard_entry->keyname,"A06");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case '^':strcpy(keyboard_entry->keyname,"A06");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case '7':strcpy(keyboard_entry->keyname,"A07");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case '&':strcpy(keyboard_entry->keyname,"A07");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case '8':strcpy(keyboard_entry->keyname,"A08");
			keyboard_entry->shift=0;
			keyboard_entry->finger=7;
			break;
		case '*':strcpy(keyboard_entry->keyname,"A08");
			keyboard_entry->shift=1;
			keyboard_entry->finger=7;
			break;
		case '9':strcpy(keyboard_entry->keyname,"A09");
			keyboard_entry->shift=0;
			keyboard_entry->finger=8;
			break;
		case '(':strcpy(keyboard_entry->keyname,"A09");
			keyboard_entry->shift=1;
			keyboard_entry->finger=8;
			break;
		case '0':strcpy(keyboard_entry->keyname,"A10");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case ')':strcpy(keyboard_entry->keyname,"A10");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case '-':strcpy(keyboard_entry->keyname,"A11");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '_':strcpy(keyboard_entry->keyname,"A11");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case '=':strcpy(keyboard_entry->keyname,"A12");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '+':strcpy(keyboard_entry->keyname,"A12");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case '\\':strcpy(keyboard_entry->keyname,"A13");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '|':strcpy(keyboard_entry->keyname,"A13");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case 'q':strcpy(keyboard_entry->keyname,"B01");
			keyboard_entry->shift=0;
			keyboard_entry->finger=0;
			break;
		case 'Q':strcpy(keyboard_entry->keyname,"B01");
			keyboard_entry->shift=2;
			keyboard_entry->finger=0;
			break;
		case 'w':strcpy(keyboard_entry->keyname,"B02");
			keyboard_entry->shift=0;
			keyboard_entry->finger=1;
			break;
		case 'W':strcpy(keyboard_entry->keyname,"B02");
			keyboard_entry->shift=2;
			keyboard_entry->finger=1;
			break;
		case 'e':strcpy(keyboard_entry->keyname,"B03");
			keyboard_entry->shift=0;
			keyboard_entry->finger=2;
			break;
		case 'E':strcpy(keyboard_entry->keyname,"B03");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case 'r':strcpy(keyboard_entry->keyname,"B04");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case 'R':strcpy(keyboard_entry->keyname,"B04");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case 't':strcpy(keyboard_entry->keyname,"B05");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case 'T':strcpy(keyboard_entry->keyname,"B05");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case 'y':strcpy(keyboard_entry->keyname,"B06");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case 'Y':strcpy(keyboard_entry->keyname,"B06");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case 'u':strcpy(keyboard_entry->keyname,"B07");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case 'U':strcpy(keyboard_entry->keyname,"B07");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case 'i':strcpy(keyboard_entry->keyname,"B08");
			keyboard_entry->shift=0;
			keyboard_entry->finger=7;
			break;
		case 'I':strcpy(keyboard_entry->keyname,"B08");
			keyboard_entry->shift=1;
			keyboard_entry->finger=7;
			break;
		case 'o':strcpy(keyboard_entry->keyname,"B09");
			keyboard_entry->shift=0;
			keyboard_entry->finger=8;
			break;
		case 'O':strcpy(keyboard_entry->keyname,"B09");
			keyboard_entry->shift=1;
			keyboard_entry->finger=8;
			break;
		case 'p':strcpy(keyboard_entry->keyname,"B10");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case 'P':strcpy(keyboard_entry->keyname,"B10");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case '[':strcpy(keyboard_entry->keyname,"B11");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '{':strcpy(keyboard_entry->keyname,"B11");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case ']':strcpy(keyboard_entry->keyname,"B12");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '}':strcpy(keyboard_entry->keyname,"B12");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case 'a':strcpy(keyboard_entry->keyname,"C01");
			keyboard_entry->shift=0;
			keyboard_entry->finger=0;
			break;
		case 'A':strcpy(keyboard_entry->keyname,"C01");
			keyboard_entry->shift=2;
			keyboard_entry->finger=0;
			break;
		case 's':strcpy(keyboard_entry->keyname,"C02");
			keyboard_entry->shift=0;
			keyboard_entry->finger=1;
			break;
		case 'S':strcpy(keyboard_entry->keyname,"C02");
			keyboard_entry->shift=2;
			keyboard_entry->finger=1;
			break;
		case 'd':strcpy(keyboard_entry->keyname,"C03");
			keyboard_entry->shift=0;
			keyboard_entry->finger=2;
			break;
		case 'D':strcpy(keyboard_entry->keyname,"C03");
			keyboard_entry->shift=2;
			keyboard_entry->finger=2;
			break;
		case 'f':strcpy(keyboard_entry->keyname,"C04");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case 'F':strcpy(keyboard_entry->keyname,"C04");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case 'g':strcpy(keyboard_entry->keyname,"C05");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case 'G':strcpy(keyboard_entry->keyname,"C05");
			keyboard_entry->shift=1;
			keyboard_entry->finger=3;
			break;
		case 'h':strcpy(keyboard_entry->keyname,"C06");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case 'H':strcpy(keyboard_entry->keyname,"C06");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case 'j':strcpy(keyboard_entry->keyname,"C07");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case 'J':strcpy(keyboard_entry->keyname,"C07");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case 'k':strcpy(keyboard_entry->keyname,"C08");
			keyboard_entry->shift=0;
			keyboard_entry->finger=7;
			break;
		case 'K':strcpy(keyboard_entry->keyname,"C08");
			keyboard_entry->shift=1;
			keyboard_entry->finger=7;
			break;
		case 'l':strcpy(keyboard_entry->keyname,"C09");
			keyboard_entry->shift=0;
			keyboard_entry->finger=8;
			break;
		case 'L':strcpy(keyboard_entry->keyname,"C09");
			keyboard_entry->shift=1;
			keyboard_entry->finger=8;
			break;
		case ';':strcpy(keyboard_entry->keyname,"C10");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case ':':strcpy(keyboard_entry->keyname,"C10");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;			
		case '\'':strcpy(keyboard_entry->keyname,"C11");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '"':strcpy(keyboard_entry->keyname,"C11");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case 'z':strcpy(keyboard_entry->keyname,"D01");
			keyboard_entry->shift=0;
			keyboard_entry->finger=0;
			break;
		case 'Z':strcpy(keyboard_entry->keyname,"D01");
			keyboard_entry->shift=2;
			keyboard_entry->finger=0;
			break;
		case 'x':strcpy(keyboard_entry->keyname,"D02");
			keyboard_entry->shift=0;
			keyboard_entry->finger=1;
			break;
		case 'X':strcpy(keyboard_entry->keyname,"D02");
			keyboard_entry->shift=2;
			keyboard_entry->finger=1;
			break;
		case 'c':strcpy(keyboard_entry->keyname,"D03");
			keyboard_entry->shift=0;
			keyboard_entry->finger=2;
			break;
		case 'C':strcpy(keyboard_entry->keyname,"D03");
			keyboard_entry->shift=2;
			keyboard_entry->finger=2;
			break;
		case 'v':strcpy(keyboard_entry->keyname,"D04");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case 'V':strcpy(keyboard_entry->keyname,"D04");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case 'b':strcpy(keyboard_entry->keyname,"D05");
			keyboard_entry->shift=0;
			keyboard_entry->finger=3;
			break;
		case 'B':strcpy(keyboard_entry->keyname,"D05");
			keyboard_entry->shift=2;
			keyboard_entry->finger=3;
			break;
		case 'n':strcpy(keyboard_entry->keyname,"D06");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case 'N':strcpy(keyboard_entry->keyname,"D06");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case 'm':strcpy(keyboard_entry->keyname,"D07");
			keyboard_entry->shift=0;
			keyboard_entry->finger=6;
			break;
		case 'M':strcpy(keyboard_entry->keyname,"D07");
			keyboard_entry->shift=1;
			keyboard_entry->finger=6;
			break;
		case ',':strcpy(keyboard_entry->keyname,"D08");
			keyboard_entry->shift=0;
			keyboard_entry->finger=7;
			break;
		case '<':strcpy(keyboard_entry->keyname,"D08");
			keyboard_entry->shift=1;
			keyboard_entry->finger=7;
			break;
		case '.':strcpy(keyboard_entry->keyname,"D09");
			keyboard_entry->shift=0;
			keyboard_entry->finger=8;
			break;
		case '>':strcpy(keyboard_entry->keyname,"D09");
			keyboard_entry->shift=1;
			keyboard_entry->finger=8;
			break;
		case '/':strcpy(keyboard_entry->keyname,"D10");
			keyboard_entry->shift=0;
			keyboard_entry->finger=9;
			break;
		case '?':strcpy(keyboard_entry->keyname,"D10");
			keyboard_entry->shift=1;
			keyboard_entry->finger=9;
			break;
		case ' ':strcpy(keyboard_entry->keyname,"E03");
			keyboard_entry->shift=0;
			keyboard_entry->finger=5;
			break;
		default:strcpy(keyboard_entry->keyname,"None");
			keyboard_entry->shift=0;
			keyboard_entry->finger=-1;
			break;
	}
  return 0;
}

void GenerateKeyboard(SDL_Surface* keyboard)
{
	SDL_Surface* tmp = NULL;
	SDL_Rect new;
	int i;
	int col;
	char row;
	int render=1;
	Uint16 t[2];
	TTF_Font* smallfont = NULL;
	DEBUGCODE { printf("Entering GenerateKeyboard\n"); }
	t[1] = '\0';
	smallfont = LoadFont(settings.theme_font_name, 15);
	if(!smallfont)
	{
		DEBUGCODE { printf("Error loading font\n"); }
		return;
	}
	for(i=0;i < num_chars_used;i++)
	{
		render=1;
		new.x=0;
		new.y=0;
		new.w=5;
		new.h=5;
		t[0]=keyboard_list[i].unicode_value;
		sscanf(keyboard_list[i].keyname,"%c%d",&row,&col);
		switch(row)
		{
			case 'A':new.y+=6;new.x+=13;break;
			case 'B':new.y+=36;new.x+=23;break;
			case 'C':new.y+=66;new.x+=33;break;
			case 'D':new.y+=96;new.x+=23;break;
			case 'E':new.y+126;break;
			default: render=0;break;
		}
		if(!render)
			continue;
		new.x+=31*col;
		if(keyboard_list[i].shift>0)
		{
					new.x-=9;
					if(new.y<9)
						new.y-=5;
					else
						new.y-=9;
		}
		DEBUGCODE { printf("Making %d : %C\n",i,keyboard_list[i].unicode_value); }
		//tmp=BlackOutline_Unicode(t, smallfont, &black);
		tmp=TTF_RenderUNICODE_Blended((TTF_Font*)smallfont, t, black);
		if(tmp==NULL)
		{
			DEBUGCODE { printf("Error Making %d : %C\n",i,keyboard_list[i].unicode_value); }
		}
		SDL_BlitSurface(tmp, NULL, keyboard, &new);
	}	
	TTF_CloseFont(smallfont);
	DEBUGCODE { printf("Leaving GenerateKeyboard\n"); }
}


void updatekeylist(int key,char ch)
{
	wchar_t;
	keyboard_list[key].latin_char=ch;
	wchar_t wtmp=ch;
	map_keys(wtmp,&keyboard_list[key]);
}


void savekeyboard(void)
{
	unsigned char fn[FNLEN];
	FILE *fp;
	int i;
	wchar_t tmp[2];
	char buf[FNLEN];
	tmp[1]=0;
	if(!settings.use_english)
		sprintf(fn , "%s/keyboard.lst", settings.theme_data_path);
	else
		sprintf(fn , "%s/keyboard.lst", settings.default_data_path);

	fp=fopen(fn,"w");
	if (fp == NULL)
	{
		LOG("savekeyboard() - could not open keyboard.lst\n");
		return 0;
	}
	for(i=0;i<num_chars_used;i++)
	{
		tmp[0]=keyboard_list[i].unicode_value;
		/**********fprintf(fp,"%d|%C\n",keyboard_list[i].finger,keyboard_list[i].unicode_value); doesnt work, so the unicode value is converted into a char string*/
		ConvertToUTF8(tmp, buf);
		if(keyboard_list[i].finger==-1)
		{
			fprintf(fp,"%s\n",buf);
		}
		else
		if(keyboard_list[i].latin_char==-1)
		{
				fprintf(fp,"%d|%s\n",keyboard_list[i].finger,buf);
		}
		else
		{
			fprintf(fp,"%d|%s|%c\n",keyboard_list[i].finger,buf,keyboard_list[i].latin_char);
		}
	}
	fclose(fp);
}
/****************************************************************/

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



void ResetCharList(void)
{
  char_list[0] = '\0';
}



/* Creates a list of distinct Unicode characters in       */
/* the argument string for subsequent rendering.          */
/* Like gen_char_list() but takes a string argument       */
/* instead of going through the currently selected        */
/* word list. Argument should be UTF-8                    */
/* Can be called multiple times on different strings      */
/* to accumulate entire repertoire - call ResetCharList() */
/* to start over                                          */
void GenCharListFromString(const unsigned char* UTF8_str)
{
  int i = 0;
  wchar_t wchar_buf[MAX_UNICODES];

  ConvertFromUTF8(wchar_buf, UTF8_str);

  /* FNLEN is max length of phrase (I think) */
  while (wchar_buf[i] != '\0' && i < FNLEN) 
  {
    add_char(wchar_buf[i]);
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



/* Checks to see if the argument is already in the list and adds    */
/* it if necessary.  Returns 1 if char added, 0 if already in list, */
/* -1 if list already up to maximum size:                           */
/* FIXME performance would be better with hashtable                 */
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
  // We never want to get here...
  return -1;
}



static void clear_keyboard(void)
{
  int i = 0;
  for (i = 0; i < MAX_UNICODES; i++)
  {
    keyboard_list[i].unicode_value = 0;
    keyboard_list[i].finger = -1;
  }
}
