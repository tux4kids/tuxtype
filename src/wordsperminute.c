//Words per minute mode

//currently only has an english file..
//only calculates wpm if they finish the set, otherwise ignore
#include "globals.h"
#include "funcs.h"

typedef struct lines{
	wchar_t words[][]; 
	int wcount; /* number of words in line */
	int finished; /* chars that have to be typed to finish line */
}lines;

	lines typeline;
	lines previewline;
	lines templine;
 
static SDL_Surface* bg = NULL;
static letter_loc;
static TTF_Font* font = NULL;
int charsfinished;
static Mix_Chunk* wrong = NULL;

lines getTypeLine(void);
lines getPreviewLine(void);
void copyToTempLine(lines);
static int wpm_load_media(void);
static void wpm_unload_media(void);
static void print_at(const wchar_t* pphrase, int wrap, int x, int y);
void displayTypeLine(lines, int);
void displayPreviewLine(lines, int);

int wpm()
{
	Uint32 time = 30000, elapsed; /* 30 seconds */
  	int quit = 0,
  	i = 0,
  	count = 0,
  	wp = 0,
  	z = 0;
  	SDL_Rect dst;
  	SDL_Surface* srfc = NULL;

  	if (!wpm_load_media())
  	{
  		fprintf(stderr, "wpm() - wpm_load_media() failed, returning.\n");
  		return 0;
  	}

  	SDL_BlitSurface(bg, NULL, screen, NULL);
  	SDL_Flip(screen);

  	srfc = GetWhiteGlyph(65);

  	if (!srfc)
  	{
  		fprintf(stderr, "wpm() - GetWhiteGlyph(65) not defined - bailing out.\n");
  		return 0;
  	}
  	
  	dst.x = 40;
  	dst.y = 10;
  	dst.w = srfc->w;
  	dst.h = srfc->h;
  	
  	getTypeLine();
  	getPreviewLine();
  	displayTypeLine(typeline, 1);
  	displayPreviewLine(previewline, 1);

  	copyToTempLine(typeline); //copy active to temp
  	do
  	{
  		while  (SDL_PollEvent(&event))
  		{
  			if (event.type == SDL_KEYDOWN)
  			{
  				if (event.key.keysym.sym == SDLK_ESCAPE)
  				{
  					quit = 1;
  				}
  				else
  				{
  					if (templine.words[c]==(wchar_t)event.key.keysym.unicode)
  					{
  						srfc = GetRedGlyph(event.key.keysym.unicode);
  						if (srfc)
  						{
  							SDL_BlitSurface(srfc, NULL, screen, &dst);
  							dst.x = (dst.x + srfc->w) - 5;
  						}
  						charsfinished++;
  						c++;
  						templine.finished--;
  					}
  					else
  					{
  						if (event.key.keysym.sym != SDLK_RSHIFT
  								&& event.key.keysym.sym != SDLK_LSHIFT)
  						{
  							PlaySound(wrong);
  						}
  					}
  				}
  			}
  		}
  		if(templine.finished <= 0)
  		{
  			displayTypeLine(templine, 0); //erase top line
  			copyToTempFile(previewline); //copy preview to temp
  			displayTypeLine(tempfile, 1); //move preview to active
  			displayPreviewLine(previewline, 0) //erase bottom
  			getPreviewLine(); //get new bottom
  			displayPreviewLine(previewline, 1) //display bottom
  		}
  		SDL_Flip(screen);
  		SDL_Delay(30);
  		elapsed = time - SDL_GetTicks();
  	}
  	while (!quit && elapsed > 0);
  	if (elapsed)
  	{
  		updateWPM(charsfinished);
  		checkWPM(charsfinished);
  	}
  wpm_unload_media();

  return 1;
}

static int wpm_load_media(void)
{
  int i;
  unsigned char fn[FNLEN];
  unsigned char let[5];

  LOG("Loading wpm media\n");


  bg = LoadImage("main_bkg.png", IMG_ALPHA);
  wrong = LoadSound("tock.wav");
  font = LoadFont(settings.theme_font_name, 30);

  /* Get out if anything failed to load: */
  if (!bg
    ||!wrong
    ||!font)
  {
    fprintf(stderr, "wpm_load_media() - failed to load needed media \n");
    wpm_unload_media();
    return 0;
  }

  /* Now render letters for glyphs in alphabet: */
  RenderLetters(font);
  TTF_CloseFont(font);  /* Don't need it after rendering done */
  font = NULL;

  LOG("DONE - Loading wpm media\n");
  return 1;
}

static void wpm_unload_media(void)
{
	int i;
	SDL_FreeSurface(bg);
	bg = NULL;
	Mix_FreeChunk(wrong);
	wrong = NULL;
}
void getTypeLine()
{
	int count = 0;
	int linewidth = 0; //keep lines to 598 pixels
	typeline.finished = 0 ;
	typeline.wcount = 0;
	do
	{
		typeline.words[typeline.wcount] = strcat(GetWord(), " ");
		typeline.wcount++;
		SDL_Surface* let = NULL;
		for (count = 0; count <= typeline.wcount; count++)
		{							
			i = 0;
			while( i < wcslen(typeline.words[count]) && linewidth < 598)
			{
				let = GetWhiteGlyph((int)typeline.words[count][i]);
				if (let)
				{
					linewidth+= let->w - 5;
					typeline.finished++;
				}	
				else
				{
					fprintf(stderr, "getTypeLine() - needed glyph not available\n");
					return;
				}
				i++;
			}
		}
	}
	while(linewidth < 598);
	typeline.words[count] = NULL;
	typeline.wcount--;
}
void getPreviewLine()
{
	int count = 0;
	int linewidth = 0; //keep it to a line 598 pixels
	previewline.wcount = 0;
	previewline.finished = 0;
	do
	{
		previewline.words[previewline.wcount] = strcat(GetWord(), " ");
		previewline.wcount++;
		SDL_Surface* let = NULL;
		for (count = 0; count <= previewline.wcount; count++)
		{							
			i = 0;
			while( i < wcslen(previewline.words[count]) && linewidth < 598)
			{
				let = GetWhiteGlyph((int)previewline.words[count][i]);
				if (let)
				{
					linewidth+= let->w - 5;
					previewline.finished++;
				}	
				else
				{
					fprintf(stderr, "getTypeLine() - needed glyph not available\n");
					return;
				}
				i++;
			}
		}
	}
	while(linewidth < 598);
	previewline.words[count] = NULL;
	previewline.wcount--;
}
void displayTypeLine(lines tline, int switcher)
{
	int i = 0;
	int count;
	SDL_Surface* surf = NULL;
	letter_loc.x = 40;
	letter_loc.y = 10;
	letter_loc.w = GetWhiteGlyph(65)->w;
	letter_loc.h = GetWhiteGlyph(65)->h;
	
	if(switcher)
	{
		for (count = 0; count < wcslen(tline.words[count]); count++)
		{
			for(i = 0; i < wcslen(tline.words[count][i]); i++)
			{
				surf = GetWhiteGlyph(tline.words[count][i]);
				if (surf)
				{
					SDL_BlitSurface(surf, NULL, screen, &letter_loc);
					letter_loc.x = (letter_loc.x + surf->w) - 5;
				}
				else
				{
					fprintf(stderr, "print_at(): needed glyph for %C not found\n",
							pphrase[z]);
				}
			}
		}
	}
	else //cleanup because old
	{
		for (count = 0; count < wcslen(tline.words[count]); count++)
		{
			for(i = 0; i < wcslen(tline.words[count][i]); i++)
			{
				surf = GetWhiteGlyph(tline.words[count][i]);
				if (surf)
				{
					letter_loc.x = (letter_loc.x + surf->w) - 5;
					SDL_FreeSurface(surf);
				}
			}
		}
	}
}
void displayPreviewLine(lines pline, int switcher)
{	
	int i = 0;
	int count;
	SDL_Surface* surf = NULL;
	letter_loc.x = 40
	letter_loc.w = GetWhiteGlyph(65)->w;
	letter_loc.h = GetWhiteGlyph(65)->h;
	letter_loc.y = 10 + letter_loc.h -(letter_loc.h/4);
	if(switcher)
	{
		for (count = 0; count < wcslen(pline.words[count]); count++)
		{
			for(i = 0; i < wcslen(pline.words[count][i]); i++)
			{
				surf = GetWhiteGlyph(pline.words[count][i]);
				if (surf)
				{
					SDL_BlitSurface(surf, NULL, screen, &letter_loc);
					letter_loc.x = (letter_loc.x + surf->w) - 5;
				}
				else
				{
					fprintf(stderr, "print_at(): needed glyph for %C not found\n",
							pphrase[z]);
				}
			}
		}
	}
	else //cleanup because old
	{
		for (count = 0; count < wcslen(pline.words[count]); count++)
		{
			for(i = 0; i < wcslen(pline.words[count][i]); i++)
			{
				surf = GetWhiteGlyph(pline.words[count][i]);
				if (surf)
				{
					letter_loc.x = (letter_loc.x + surf->w) - 5;
					SDL_FreeSurface(surf);
				}
			}
		}
	}
}
void copyToTemp(lines x)
{
	int count = 0;
	templine.wcount = 0;
	templine.finished = 0;
	do
	{
		templine.words[templine.wcount] = x.word[count];
		templine.wcount++;
		SDL_Surface* let = NULL;
		for (count = 0; count <= x.wcount; count++)
		{							
			i = 0;
			while( i < wcslen(x.words[count]))
			{
				let = GetWhiteGlyph((int)x.words[count][i]);
				if (let)
				{
					templine.finished++;
				}	
				else
				{
					fprintf(stderr, "getTypeLine() - needed glyph not available\n");
					return;
				}
				i++;
			}
		}
	}
}
