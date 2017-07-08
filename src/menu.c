/*
   menu.c

   Functions responsible for loading, parsing and displaying game menu.

   Copyright 2009, 2010, 2011.
Authors:  Boleslaw Kulbabinski, David Bruce, Brendan Luchen.
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

menu.c is part of "Tux, of Math Command", a.k.a. "tuxmath".

Tuxmath is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tuxmath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */



#include "menu.h"
#include "titlescreen.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* create string array of activities' names */
#define X(name) #name
static char* activities[] = { ACTIVITIES };
#undef X

/* actions available while viewing the menu */
//enum { NONE, CLICK, PAGEUP, PAGEDOWN, STOP_ESC, RESIZED };

/* stop button, left and right arrow positions do not
   depend on currently displayed menu */
//SDL_Rect menu_rect, stop_rect, prev_rect, next_rect;
//SDL_Surface *stop_button, *prev_arrow, *next_arrow, *prev_gray, *next_gray;

/*TODO: move these constants into a config file (maybe together with
  titlescreen paths and rects ? ) */
//const float menu_pos[4] = {0.38, 0.23, 0.55, 0.72};
//const float stop_pos[4] = {0.94, 0.0, 0.06, 0.06};
//const float prev_pos[4] = {0.87, 0.93, 0.06, 0.06};
//const float next_pos[4] = {0.94, 0.93, 0.06, 0.06};
//const char* stop_path = "status/stop.svg";
//const char* prev_path = "status/left.svg";
//const char* next_path = "status/right.svg";
//const char* prev_gray_path = "status/left_gray.svg";
//const char* next_gray_path = "status/right_gray.svg";
//const float button_gap = 0.2, text_h_gap = 0.4, text_w_gap = 0.5, button_radius = 0.27;
//const int min_font_size = 8, default_font_size = 20, max_font_size = 33;

/* menu title rect */
SDL_Rect menu_title_rect;

/* buffer size used when reading attributes or names */
const int buf_size = 128;

static int chooseWordlist(void);


/* local functions */

int             run_menu(MenuType which, bool return_choice);
int             handle_activity(int act, int param);

/* convenience wrapper for T4K_RunMenu */
int run_menu(MenuType which, bool return_choice)
{    
    return T4K_RunMenu(
            which,
            return_choice,
            &DrawTitleScreen, 
            &HandleTitleScreenEvents, 
            &HandleTitleScreenAnimations, 
            &handle_activity); 
}

/*
   handlers for specific game activities
   */

/* return QUIT if user decided to quit the application while running an activity
   return 0 otherwise */
int handle_activity(int act, int param)
{
    DEBUGMSG(debug_menu, "entering handle_activity()\n");
    DEBUGMSG(debug_menu, "act: %d\n", act);

    T4K_OnResolutionSwitch(NULL); //in case an activity forgets to register its own resolution switch handler, prevent insanity

    switch(act)
    {
        case RUN_CASCADE_LEVEL1:
            DEBUGMSG(debug_menu, "activity: RUN_CASCADE_LEVEL1\n");
            if (chooseWordlist())
				PlayCascade( EASY );
            break;

        case RUN_CASCADE_LEVEL2:
            DEBUGMSG(debug_menu, "activity: RUN_CASCADE_LEVEL2\n");
            if (chooseWordlist())
				PlayCascade( MEDIUM );
            break;

        case RUN_CASCADE_LEVEL3:
            DEBUGMSG(debug_menu, "activity: RUN_CASCADE_LEVEL3\n");
            if (chooseWordlist())
				PlayCascade( HARD );
            break;

        case RUN_INSTRUCT:
            DEBUGMSG(debug_menu, "activity: RUN_INSTRUCT\n");
            InstructCascade();
            break;

        case RUN_LASER_LEVEL1:
            DEBUGMSG(debug_menu, "activity: RUN_LASER_LEVEL1\n");
            if (chooseWordlist())
				PlayLaserGame( EASY );
            break;

        case RUN_LASER_LEVEL2:
            DEBUGMSG(debug_menu, "activity: RUN_LASER_LEVEL2\n");
            if (chooseWordlist())
				PlayLaserGame( MEDIUM );
            break;

        case RUN_LASER_LEVEL3:
            DEBUGMSG(debug_menu, "activity: RUN_LASER_LEVEL3\n");
            if (chooseWordlist())
				PlayLaserGame( HARD );
            break;

        case RUN_LASER_LEVEL4:
            DEBUGMSG(debug_menu, "activity: RUN_LASER_LEVEL4\n");
            if (chooseWordlist())
				PlayLaserGame( INSANE );
            break;

        case RUN_LESSONS:
            DEBUGMSG(debug_menu, "activity: RUN_LESSONS\n");
            XMLLesson();
            break;

        case RUN_PHRASE_TYPING:
            DEBUGMSG(debug_menu, "activity: RUN_PHRASE_TYPING\n");
			Phrases(NULL); 
            break;

        case RUN_SET_LANGUAGE:
            DEBUGMSG(debug_menu, "activity: RUN_SET_LANGUAGE\n");
			//unload_media();
			ChooseTheme();
			LoadLang();
			LoadKeyboard();
			//load_media();
            break;


        case RUN_EDIT_WORDLIST:
            DEBUGMSG(debug_menu, "activity: RUN_EDIT_WORDLIST\n");
            ChooseListToEdit();
            break;

        case RUN_SET_BRAILLE_KEYS:
            DEBUGMSG(debug_menu, "activity: RUN_SET_BRAILLE_KEYS\n");
            break;

        case RUN_PROJECT_INFO:
            DEBUGMSG(debug_menu, "activity: RUN_PROJECT_INFO\n");
            ProjectInfo();
            break;

        case RUN_QUIT:
            return QUIT;
    }

    //re-register resolution switcher
    T4K_OnResolutionSwitch(&HandleTitleScreenResSwitch);
    //redraw if necessary
    RenderTitleScreen();

    if (settings.menu_music) //Turn menu music back on
        T4K_AudioMusicLoad( "tuxi.ogg", T4K_AUDIO_LOOP_FOREVER );

    return 0;
}



/* load menu trees from disk and prerender them */
void LoadMenus(void)
{
    T4K_SetMenuSpritePrefix("sprites");
    T4K_SetActivitiesList(N_OF_ACTIVITIES, activities);
    /* main menu */
    T4K_LoadMenu(MENU_MAIN, "main_menu.xml");

    //NOTE level_menu.xml doesn't exist, and as it's not being used I'm skipping the load for now -Cheez
    /* difficulty menu */
    //  T4K_LoadMenu(MENU_DIFFICULTY, "level_menu.xml");
    T4K_SetMenuFontSize(MF_BESTFIT, 0);
    T4K_PrerenderAll();
}


/* run main menu. If this function ends it means that tuxmath is going to quit */
void RunMainMenu(void)
{
    int i;
    //  char* lltitle = "Lesson List"; //lesson list menu title
    char* icon_names[num_lessons];


    /* lessons menu */

    //for(i = 0; i < num_lessons; i++)
   // {
    //    icon_names[i] = (lesson_list_goldstars[i] ? "goldstar" : "no_goldstar");
    //}

    //T4K_CreateOneLevelMenu(MENU_LESSONS, num_lessons, lesson_list_titles, icon_names, NULL, "Back");

    //T4K_PrerenderMenu(MENU_LESSONS);

    run_menu(MENU_MAIN, false);
    DEBUGMSG(debug_menu, "Leaving RunMainMenu()\n");
}

/* returns 0 if user pressed escape ...
 *         1 if word list was set correctly
 */
static int chooseWordlist(void)
{
  SDL_Surface* titles[MAX_WORD_LISTS] = {NULL};
  SDL_Surface* select[MAX_WORD_LISTS] = {NULL};
  SDL_Surface* left = NULL, *right = NULL;
  SDL_Rect leftRect, rightRect;
  SDL_Rect titleRects[8];
  int stop = 0;
  int loc = 0;
  int old_loc = 1;
  int lists = 0;
  int i;
  int result = 0;;
  char wordPath[FNLEN];
  char wordlistFile[MAX_WORD_LISTS][200];
  char wordlistName[MAX_WORD_LISTS][200];

  DIR* wordsDir = NULL;
  struct dirent* wordsFile = NULL;
  FILE* tempFile = NULL;

  LOG("Entering chooseWordlist():\n");

  /* find the directory to load wordlists from */

  /* First we add the bundled word lists in either NLS or default theme: */
  {
    if (!settings.use_english)  /* Using theme: */
    {
      sprintf(wordPath,"%s/words", settings.theme_data_path);
      if (!CheckFile(wordPath))
      {
        fprintf(stderr, "chooseWordList() - theme contains no wordlist dir \n");
        return 0;
      }
    }
    else  /* No user settings or theme selected - using English: */
    {
      sprintf(wordPath,"%s/words", settings.default_data_path);
      if (!CheckFile(wordPath))
      {
        fprintf(stderr, "chooseWordList() - data path contains no wordlist dir \n");
        return 0;
      }
    }
  }

  /* If we get to here, we know there is at least a wordlist directory */
  /* but not necessarily any valid files.                              */

  //DEBUGMSG { fprintf(stderr, "bundled wordPath is: %s\n", wordPath); }


  /* FIXME looks like a place for scandir() - or our own w32_scandir() */
  /* FIXME we also have this block of code repeated three times - DSB  */
  /* create a list of all the .txt files */
  wordsDir = opendir( wordPath );	
  do
  {
    wordsFile = readdir(wordsDir);
    if (!wordsFile)
      break; /* Loop continues until break occurs */

    //DEBUGMSG { fprintf(stderr, "wordsFile name is: %s\n", wordsFile->d_name); }

    /* must have at least .txt at the end */
    if (strlen(wordsFile->d_name) < 5)
      continue;

    if (strcmp(&wordsFile->d_name[strlen(wordsFile->d_name) -4 ],".txt"))
      continue;

    sprintf(wordlistFile[lists], "%s/%s", wordPath, wordsFile->d_name);

    /* load the name for the wordlist from the file ... (1st line) */
    tempFile = fopen( wordlistFile[lists], "r" );
    if (!tempFile)
      continue;

    result = fscanf(tempFile, "%[^\n]\n", wordlistName[lists]);
    if (result == EOF)
      continue;

    /* check to see if it has a \r at the end of it (dos format!) */
    if (wordlistName[lists][strlen(wordlistName[lists]) - 1] == '\r')
      wordlistName[lists][strlen(wordlistName[lists]) - 1] = '\0';

    lists++;

    fclose(tempFile);
  } while (1); /* Loop continues until break occurs */
  closedir(wordsDir);	

  
  
  /* Adding global custom wordlists ------------------------------------ */
 
  sprintf(wordPath,"%s/words", settings.var_data_path);
  if (!CheckFile(wordPath))
  {
    //DEBUGMSG { fprintf(stderr, "chooseWordList() -  path \"%s\" not found\n", wordPath); }
  }
  else
  {
    /* If we get to here, we know there is at least a wordlist directory */
    /* but not necessarily any valid files.                              */

    //DEBUGMSG { fprintf(stderr, "global custom wordPath is: %s\n", wordPath); }

    /* FIXME looks like a place for scandir() - or our own w32_scandir() */
    /* create a list of all the .txt files */
    wordsDir = opendir( wordPath );	
    do
    {
      wordsFile = readdir(wordsDir);
      if (!wordsFile)
        break; /* Loop continues until break occurs */

      //DEBUGMSG { fprintf(stderr, "wordsFile name is: %s\n", wordsFile->d_name); }

      /* must have at least .txt at the end */
      if (strlen(wordsFile->d_name) < 5)
        continue;

      if (strcmp(&wordsFile->d_name[strlen(wordsFile->d_name) -4 ],".txt"))
        continue;

      sprintf(wordlistFile[lists], "%s/%s", wordPath, wordsFile->d_name);

      /* load the name for the wordlist from the file ... (1st line) */
      tempFile = fopen( wordlistFile[lists], "r" );
      if (!tempFile)
        continue;

      result = fscanf(tempFile, "%[^\n]\n", wordlistName[lists]);
      if (result == EOF)
        continue;

      /* check to see if it has a \r at the end of it (dos format!) */
      if (wordlistName[lists][strlen(wordlistName[lists]) - 1] == '\r')
        wordlistName[lists][strlen(wordlistName[lists]) - 1] = '\0';

      lists++;

      fclose(tempFile);
    } while (1); /* Loop continues until break occurs */
    closedir(wordsDir);
  }
  
  /* Now add any lists in the user's personal settings path: ------------ */

  sprintf(wordPath,"%s/words", settings.user_settings_path);
  if (!CheckFile(wordPath))
  {
    //DEBUGMSG { fprintf(stderr, "chooseWordList() -  path \"%s\" not found\n", wordPath); }
  }
  else
  {
    /* If we get to here, we know there is at least a wordlist directory */
    /* but not necessarily any valid files.                              */

    //DEBUGMSG { fprintf(stderr, "user-specific wordPath is: %s\n", wordPath); }

    /* FIXME looks like a place for scandir() - or our own w32_scandir() */
    /* create a list of all the .txt files */
    wordsDir = opendir( wordPath );	
    do
    {
      wordsFile = readdir(wordsDir);
      if (!wordsFile)
        break; /* Loop continues until break occurs */

      //DEBUGMSG { fprintf(stderr, "wordsFile name is: %s\n", wordsFile->d_name); }

      /* must have at least .txt at the end */
      if (strlen(wordsFile->d_name) < 5)
        continue;

      if (strcmp(&wordsFile->d_name[strlen(wordsFile->d_name) -4 ],".txt"))
        continue;

      sprintf(wordlistFile[lists], "%s/%s", wordPath, wordsFile->d_name);

      /* load the name for the wordlist from the file ... (1st line) */
      tempFile = fopen( wordlistFile[lists], "r" );
      if (!tempFile)
        continue;

      result = fscanf(tempFile, "%[^\n]\n", wordlistName[lists]);
      if (result == EOF)
        continue;

      /* check to see if it has a \r at the end of it (dos format!) */
      if (wordlistName[lists][strlen(wordlistName[lists]) - 1] == '\r')
        wordlistName[lists][strlen(wordlistName[lists]) - 1] = '\0';

      lists++;

      fclose(tempFile);
    } while (1); /* Loop continues until break occurs */
    closedir(wordsDir);
  }

  //DEBUGMSG { fprintf(stderr, "Found %d .txt file(s) in words dir\n", lists); }

  
  /* Done scanning for word lists, now display them for user selection: */

  /* Render SDL_Surfaces for list entries: */
  for (i = 0; i < lists; i++)
  {
    titles[i] = BlackOutline( wordlistName[i], DEFAULT_MENU_FONT_SIZE, &white );
    select[i] = BlackOutline( wordlistName[i], DEFAULT_MENU_FONT_SIZE, &yellow);
  }

  left = LoadImage("left.png", IMG_ALPHA);
  right = LoadImage("right.png", IMG_ALPHA);
  LoadBothBkgds("title/menu_bkg.jpg");

  /* Get out if needed surface not loaded successfully: */
  if (!current_bkg() || !left || !right)
  {
    fprintf(stderr, "chooseWordList(): needed image not available\n");
  
    for (i = 0; i < lists; i++)
    {
      SDL_FreeSurface(titles[i]);
      SDL_FreeSurface(select[i]);
      titles[i] = select[i] = NULL;
    }

    SDL_FreeSurface(left);
    SDL_FreeSurface(right);
    left = right = NULL;

    return 0;
  }


  leftRect.w = left->w;
  leftRect.h = left->h;
  leftRect.x = screen->w/2 - 80 - (leftRect.w/2);
  leftRect.y = screen->h - 50;

  rightRect.w = right->w;
  rightRect.h = right->h;
  rightRect.x = screen->w/2 + 80 - (rightRect.w/2);
  rightRect.y = screen->h - 50;

  /* set initial rect sizes */
  titleRects[0].y = 30;
  titleRects[0].w = titleRects[0].h = titleRects[0].x = 0;

  for (i = 1; i < 8; i++)
  { 
    titleRects[i].y = titleRects[i - 1].y + 50;
    titleRects[i].w = titleRects[i].h = titleRects[i].x = 0;
  }

  /* Main event loop for this screen: */
  while (!stop)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          exit(0); /* FIXME may need to cleanup memory and exit more cleanly */
          break;

        case SDL_MOUSEMOTION:
          for (i=0; (i<8) && (loc-(loc%8)+i<lists); i++)
            if (inRect( titleRects[i], event.motion.x, event.motion.y ))
            {
              loc = loc-(loc%8)+i;
              break;
            }
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (inRect( leftRect, event.button.x, event.button.y ))
          {
            if (loc - (loc % 8) - 8 >= 0)
            {
              loc = loc - (loc % 8) - 8;
              break;
            }
          }

          if (inRect(rightRect, event.button.x, event.button.y))
          {
            if (loc - (loc % 8) + 8 < lists)
            {
              loc = loc - (loc % 8) + 8;
              break;
            }
          }

          for (i = 0; (i < 8) && (loc - (loc % 8) + i < lists); i++)
          {
            if (inRect(titleRects[i], event.button.x, event.button.y))
            {
              loc = loc - (loc % 8) + i;
              ClearWordList(); /* clear old selection */
              GenerateWordList(wordlistFile[loc]); 
              stop = 1;
              break;
            }
          }

          break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            stop = 2;
            break;
          }

          if (event.key.keysym.sym == SDLK_RETURN)
          {
            ClearWordList(); /* clear old selection */
            GenerateWordList(wordlistFile[loc]); 
            stop = 1;
            break;
          }

          if ((event.key.keysym.sym == SDLK_LEFT)
           || (event.key.keysym.sym == SDLK_PAGEUP))
          {
            if (loc - (loc % 8) - 8 >= 0)
              loc = loc - (loc % 8) - 8;
          }

          if ((event.key.keysym.sym == SDLK_RIGHT)
           || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc - (loc % 8) + 8 < lists)
              loc = (loc - (loc % 8) + 8);
          }

          if ((event.key.keysym.sym == SDLK_UP)
	     ||
	      (event.key.keysym.sym == SDLK_k))
          {
            if (loc > 0)
              loc--;
          }

          if ((event.key.keysym.sym == SDLK_DOWN)
	     ||
	      (event.key.keysym.sym == SDLK_j))
          {
            if (loc+1<lists)
              loc++;
          }
      }
    }

    /* Redraw if we have changed location: */
    if (old_loc != loc)
    {
      int start;

      SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );

      start = loc - (loc % 8);

      for (i = start; i< MIN(start + 8,lists); i++) 
      {
        titleRects[i % 8].x = screen->w/2 - (titles[i]->w/2);
        if (i == loc)
        {
			/* Draw selected text in yellow:  */
          SDL_BlitSurface(select[loc], NULL, screen, &titleRects[i%8]);
          
          /* --- Announce the selected word list */
          T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",wordlistName[loc]);
        }  
        else
        {
			/* Draw unselected text in white: */
            SDL_BlitSurface(titles[i], NULL, screen, &titleRects[i%8]);
		}
      }

      /* --- draw arrow buttons --- */
      if (start > 0)
        SDL_BlitSurface(left, NULL, screen, &leftRect);

      if (start + 8 < lists)
        SDL_BlitSurface(right, NULL, screen, &rightRect);

      SDL_UpdateRect(screen, 0, 0, 0 ,0);
    }

    SDL_Delay(40);
    old_loc = loc;
  }

  /* --- clear graphics before leaving function --- */ 
  for (i = 0; i < lists; i++)
  {
    SDL_FreeSurface(titles[i]);
    SDL_FreeSurface(select[i]);
    titles[i] = select[i] = NULL;
  }

  SDL_FreeSurface(left);
  SDL_FreeSurface(right);
  left = right = NULL; /* Maybe overkill - about to be destroyed anyway */

  //DEBUGMSG { fprintf( stderr, "Leaving chooseWordlist();\n" ); }

  if (stop == 2)
    return 0;

  return 1;
}

