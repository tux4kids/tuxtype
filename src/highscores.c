#include "globals.h"
#include "funcs.h"
#include "profiles.c"

//tracks laser scores, and wpm stats
typedef struct highscores{ //structure for global highscores
  	char *highscore1_name;
  	int ghighscore1;
  	char *highscore2_name;
  	int ghighscore2;
  	char *highscore3_name;
  	int ghighscore3;
  	char *highscore4_name;
  	int ghighscore4;
  	char *highscore5_name;
  	int ghighscore5;
  	char *highscore6_name;
  	int ghighscore6;
  	char *highscore7_name;
  	int ghighscore7;
  	char *highscore8_name;
  	int ghighscore8;
  	char *wpm_fastest_name;
  	int wpm_fastest;
  	char *wpm_average_name;
  	int wpm_average;
  	char *wpm_slowest_name;
  	int wpm_slowest;
  } highscores;
  
  static SDL_Surface* bg = NULL;
  static TTF_Font* font = NULL;
  
typedef struct highscore_entry{
	int score;
	char name[32];
}highscore_entry;

	highscore_entry scoresdisplay[11]; 
	highscore_entry personalScores[12];
  	extern highscores hscores;

int load_highscores_filename(const char*);
int load_highscores_fp(FILE*);
void loadGlobalScores(void);
void loadPersonalScores(void);
void highscores_load_media(void);
void highscores_unload_media(void);
 
void saveHighScores()//called before exiting game, no need to write until then.
{
	char fn[FNLEN];
	FILE* highscores_file;
		
		#ifdef WIN32
			_mkdir( "userdata" ); 
			snprintf( fn, FNLEN-1, "userdata/highscores.txt" );
		#else
			snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
			mkdir( fn, 0755 );
			snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/highscores.txt", getenv("HOME") );
		#endif
			
		highscores_file = fopen(fn, "w");
		
		if (highscores_file == NULL) 
		{
				printf("Save_Highscores: Highscores file cannot be created!\n");
				return;
		}
		
		fprintf( highscores_file, "HighScore1=%d\tname1=%s\n", hscores.ghighscore1, &hscores.highscore1_name);
		fprintf( highscores_file, "HighScore2=%d\tname2=%s\n", hscores.ghighscore1, &hscores.highscore2_name);
		fprintf( highscores_file, "HighScore3=%d\tname3=%s\n", hscores.ghighscore1, &hscores.highscore3_name);
		fprintf( highscores_file, "HighScore4=%d\tname4=%s\n", hscores.ghighscore1, &hscores.highscore4_name);
		fprintf( highscores_file, "HighScore5=%d\tname5=%s\n", hscores.ghighscore1, &hscores.highscore5_name);
		fprintf( highscores_file, "HighScore6=%d\tname6=%s\n", hscores.ghighscore1, &hscores.highscore6_name);
		fprintf( highscores_file, "HighScore7=%d\tname7=%s\n", hscores.ghighscore1, &hscores.highscore7_name);
		fprintf( highscores_file, "HighScore8=%d\tname8=%s\n", hscores.ghighscore1, &hscores.highscore8_name);
		fprintf( highscores_file, "Fastest_WPM=%d\twname1=%s\n", hscores.wpm_fastest, &hscores.wpm_fastest_name);
		fprintf( highscores_file, "Highest_average_WPM=%d\twname2=%s\n", hscores.wpm_average, &hscores.wpm_average_name);
		fprintf( highscores_file, "Slowest_WPM=%d\twname3=%s\n", hscores.wpm_slowest, &hscores.wpm_slowest_name);
		
		fclose(highscores_file);
}

void displayScores() //loads global high scores and individual high scores to display
{
	int finished = 0;
	SDL_Surface* score_surfs[11] = {NULL};
	SDL_Surface* personal_surfs[12] = {NULL};
	
	char score_strings[11][42] = {{'\0'}};
	char personal_strings[12][10] = {{'\0'}};

	SDL_Rect score_rects[11];
	SDL_Rect personal_rects[12];
	SDL_Rect table_bg1, table_bg2; //1 is global, 2 is personal

	const int max_width = 300;
	int score_table_y = 100;
	
	SDL_Surface* srfc = NULL;
	SDL_Rect text_rect;

	TTF_Font* title_font = LoadFont(DEFAULT_GAME_FONT, 32);
	TTF_Font* player_font = LoadFont(DEFAULT_GAME_FONT, 14);
	
	if (!player_font || !title_font)
	{
		return;
	}
	
	highscores_load_media();
	  
	while (!finished)
	{
		start = SDL_GetTicks();

		/* Check for user events: */
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					finished = 1;
				}
	        }
		}
		loadGlobalScores();
		loadPersonalScores();
		if (bg)
		{
			SDL_BlitSurface(bg, NULL, screen, NULL );
		}
		table_bg1.x = 10;
		table_bg1.y = 5;
		table_bg1.w = max_width;
		table_bg1.h = screen->h - 10;
		
		table-bg2.x = 330
		table_bg2.y = 5;
		table bg2.w = max_width;
		table_bg2.h = screen->h - 10;
		
		srfc = BlackOutline(_("Hall Of Fame"), title_font, &yellow);
		if (srfc)
		{
			/* Now blit text and free surface: */
			SDL_BlitSurface(srfc, NULL, screen, &text_rect);
			SDL_FreeSurface(srfc);
			srfc = NULL;
		}
		if (title_font)
		{
			srfc = BlackOutline(_("Hall of Fame"), title_font, &white);
		}
		if (srfc)
		{
			text_rect.x = (screen->w)/2 - (srfc->w)/2; 
			text_rect.y += text_rect.h;
			text_rect.w = srfc->w;
			text_rect.h = srfc->h;
			SDL_BlitSurface(srfc, NULL, screen, &text_rect);
			SDL_FreeSurface(srfc);
			srfc = NULL;
			score_table_y = text_rect.y + text_rect.h;
		}
		for (i = 0; i < 11; i++)
		{
			/* Get data for global entries: */
			sprintf(score_strings[i],
					"%d.	 %s	 %d",
					i + 1,   /* Add one to get common-language place number */
					scoresdisplay[i].score,
					scoresdisplay[i].name));

			/* Clear out old surfaces and update: */

			score_surfs[i] = BlackOutline(score_strings[i], player_font, &white);

			/* Get out if BlackOutline() fails: */
			if (!score_surfs[i])
			{
		          continue;
			}
		         
			/* Set up entries in vertical column: */
			if (0 == i)
			{
				score_rects[i].y = score_table_y;
			}
			else
			{
		          score_rects[i].y = personal_rects[i - 1].y + personal_rects[i - 1].h;
			}
			score_rects[i].x = 20;
			score_rects[i].h = score_surfs[i]->h;
			score_rects[i].w = max_width;

			SDL_BlitSurface(score_surfs[i], NULL, screen, &score_rects[i]);
			SDL_FreeSurface(score_surfs[i]);
			score_surfs[i] = NULL;
		}
		for (i = 0; i < 12; i++)
		{
			/* Get data for personal entries: */
			sprintf(personal_strings[i],
					"%d.	 %d",
					i + 1,   /* Add one to get common-language place number */
					personalScores[i].score);

			/* Clear out old surfaces and update: */

			personal_surfs[i] = BlackOutline(score_strings[i], player_font, &white);

			/* Get out if BlackOutline() fails: */
			if (!personal_surfs[i])
			{
				continue;
			}
					         
			/* Set up entries in vertical column: */
			if (0 == i)
			{
				personal_rects[i].y = score_table_y;
			}
			else
			{
				personal_rects[i].y = personal_rects[i - 1].y + personal_rects[i - 1].h;
			}
			personal_rects[i].x = 340;
			personal_rects[i].h = score_surfs[i]->h;
			personal_rects[i].w = max_width;

			SDL_BlitSurface(score_surfs[i], NULL, screen, &personal_rects[i]);
			SDL_FreeSurface(score_surfs[i]);
			score_surfs[i] = NULL;
		}
		/* Update screen: */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	while ((SDL_GetTicks() - start) < 33)
	{
		SDL_Delay(20);
	}
	frame++;
	// End of while (!finished) loop
	TTF_CloseFont(title_font);
	TTF_CloseFont(player_font);
	title_font = player_font = NULL;
	highscore_unload_media();
}
void load_highscores()
{
	char fn[FNLEN];

	#ifdef WIN32
	snprintf(fn, FNLEN - 1, "userdata/highscores.txt");
  	LOG("WIN32 defined\n");
	#else
  	snprintf(fn, FNLEN - 1, (const char*)"%s/.tuxtype/highscores.txt", getenv("HOME"));
  	LOG("WIN32 not defined\n");
	#endif
		
  	load_highscores_filename(fn);
	
}

static int load_highscores_filename(const char* fn)
{
	FILE* fp = fopen(fn, "r");
	
	if (!fp)
	{
		fprintf(stderr, "load_highscores_filename(): Incorrect pointer\n");
	    return;
	}
	
	if (!load_highscores_fp(fp))
	{
		fprintf(stderr, "Could not find any highscores.\n");
	    fclose(fp);
	    return 0;
	}
	
	fclose(fp);
	return 1;
}

static int load_highscores_fp(FILE* fp)
{
  char highscores[FNLEN];
  char value[FNLEN];
  int highscores_found = 0;

  if (!fp)
  {
    fprintf(stderr, "load_highscores_fp() - FILE* parameter NULL\n");
    return 0;
  }

  /* load all the highscores here */
  while (!feof(fp))
  {
    fscanf(fp, "%[^=]=%[^\n]\n", highscores, value );

    if (strncmp( highscores, "Highscore1", FNLEN ) == 0 )
    {
    	hscores.ghighscore1 = atoi(value);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "name1", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore1_name, value, FNLEN - 1);
    	highscores_found = 1;
   }
    else if (strncmp( highscores, "Highscore2", FNLEN ) == 0 )
    {
    	hscores.ghighscore2 = atoi(value);
    	highscores_found = 1;
    }
    else if (strncmp(highscores, "name2", FNLEN) == 0)
    {
    	strncpy(hscores.highscore2_name, value, FNLEN - 1);
    	highscores_found = 1;
    }
    else if (strncmp(highscores, "Highscore3", FNLEN) == 0)
    {
    	hscores.ghighscore3 = atoi(value);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "name3", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore3_name, value, FNLEN - 1);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "Highscore4", FNLEN ) == 0 )
    {
    	hscores.ghighscore4 = atoi(value);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "name4", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore4_name, value, FNLEN - 1);
    	highscores_found = 1;
	}
    else if (strncmp( highscores, "Highscore5", FNLEN ) == 0 )
    {
    	hscores.ghighscore5 = atoi(value);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "name5", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore5_name, value, FNLEN - 1);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "Highscore6", FNLEN ) == 0 )
    {
    	hscores.ghighscore6 = atoi(value);
        highscores_found = 1;
    }
    else if (strncmp( highscores, "name6", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore6_name, value, FNLEN - 1);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "Highscore7", FNLEN ) == 0 )
    {
    	hscores.ghighscore7 = atoi(value);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "name7", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore7_name, value, FNLEN - 1);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "Highscore8", FNLEN ) == 0 )
  	{
    	hscores.ghighscore8 = atoi(value);
    	highscores_found = 1;
  	}
    else if (strncmp( highscores, "name8", FNLEN ) == 0 )
    {
    	strncpy(hscores.highscore8_name, value, FNLEN - 1);
    	highscores_found = 1;
    }
    else if (strncmp( highscores, "Fastest_WPM", FNLEN ) == 0 )
 	{
    	hscores.wpm_fastest = atoi(value);
    	highscores_found = 1;
   	}
    else if (strncmp( highscores, "wname1", FNLEN ) == 0 )
   	{
    	strncpy(hscores.wpm_fastest_name, value, FNLEN - 1);
    	highscores_found = 1;
   	}
    else if (strncmp( highscores, "Highest_average_WPM", FNLEN ) == 0 )	
    {
	  	hscores.wpm_average = atoi(value);
	 	highscores_found = 1;
	}
    else if (strncmp( highscores, "wname2", FNLEN ) == 0 )
   	{
    	strncpy(hscores.wpm_average_name, value, FNLEN - 1);
    	highscores_found = 1;
   	}
    else if (strncmp( highscores, "Slowest_WPM", FNLEN ) == 0 )
  	{
      	 hscores.wpm_slowest = atoi(value);
      	 highscores_found = 1;
   	}
    else if (strncmp( highscores, "wname3", FNLEN ) == 0 )
   	{
    	strncpy(hscores.wpm_slowest_name, value, FNLEN - 1);
      	highscores_found = 1;
   	}
    else
    {
      DEBUGCODE {fprintf(stderr, "load_highscores_fp(): unrecognized string: %s", value);}
    }
    
  }

  if (highscores_found)
  {
    return 1;
  }
  else
  {
    fprintf(stderr, "load_highscores_fp() - no highscores in file - empty or corrupt?\n");
    return 0;
  }
}

void loadGlobalScores()
{
	scoresdisplay[0].score = hscores.ghighscore1;
	scoresdisplay[1].score = hscores.ghighscore2;
	scoresdisplay[2].score = hscores.ghighscore3;
	scoresdisplay[3].score = hscores.ghighscore4;
	scoresdisplay[4].score = hscores.ghighscore5;
	scoresdisplay[5].score = hscores.ghighscore6;
	scoresdisplay[6].score = hscores.ghighscore7;
	scoresdisplay[7].score = hscores.ghighscore8;
	scoresdisplay[8].score = hscores.wpm_fastest;
	scoresdisplay[9].score = hscores.wpm_average;
	scoresdisplay[10].score= hscores.wpm_slowest;
	
	scoresdisplay[0].name = hscores.highscore1_name;
	scoresdisplay[0].name = hscores.highscore2_name;
	scoresdisplay[0].name = hscores.highscore3_name;
	scoresdisplay[0].name = hscores.highscore4_name;
	scoresdisplay[0].name = hscores.highscore5_name;
	scoresdisplay[0].name = hscores.highscore6_name;
	scoresdisplay[0].name = hscores.highscore7_name;
	scoresdisplay[0].name = hscores.highscore8_name;
	scoresdisplay[0].name = hscores.wpm_fastest_name;
	scoresdisplay[0].name = hscores.wpm_average_name;
	scoresdisplay[0].name =hscores.wpm_slowest_name;
}

void loadPersonalScores() 
{
	
	personalScores[0].score = activeProfile.highscore1;
	personalScores[1].score = activeProfile.highscore2;
	personalScores[2].score = activeProfile.highscore3;
	personalScores[3].score = activeProfile.highscore4;
	personalScores[4].score = activeProfile.highscore5;
	personalScores[5].score = activeProfile.highscore6;
	personalScores[6].score = activeProfile.highscore7;
	personalScores[7].score = activeProfile.highscore8;
	personalScores[8].score = activeProfile.wpm_fastest;
	personalScores[9].score = activeProfile.wpm_average;
	personalScores[10].score = activeProfile.wpm_slowest;
	personalScores[11].core = activeProfile.wpm_taken;

}
void checkHighScores(int score) //checks to see if score qualifies as a high score, adds
{
	int temp, count = 1;
	int checkScore = score;
	do
	{
		if(checkScore > hscores.ghighscore(count))
		{
			temp = hscores.ghighscore(count);
			hscores.ghighscore(count) = checkScore;
			hscores.highscore(count)_name = activeProfile.profileName;
			checkScore = temp;
		}
	}
	while(checkScore < hscores.ghighscore(count) && count <=8);
}

void checkWPM(int wpm) //checks global wpm and updates if necessary
{
	if(wpm > hscores.wpm_fastest)
	{
		hscores.wpm_fastest_name = activeProfile.profileName;
		hscores.wpm_fastest = wpm;
	}
	if(wpm < hscores.wpm_slowest)
	{
		hscores.wpm_slowest_name = activeProfile.profileName;
		hscores.wpm_slowest = wpm;
	}
	if(activeProfile.wpm_average > hscores.wpm_average)
	{
		hscores.wpm_average_name = activeProfile.profileName;
		hscores.wpm_average = activeProfile.wpm_average;
	}
}

static void highscores_load_media()
{
	int load_failed = 0;

	bg = LoadImage("main_bkg.png", IMG_ALPHA);
	
	if(load_failed || !bg)
	{
		highscores_unload_media();
	}
}

static void highscores_unload_media()
{
	SDL_FreeSurface(bg);
	bg = NULL;
}