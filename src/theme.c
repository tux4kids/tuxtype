/*
   theme.c:

   Theme- (mainly language-) related code.
   
   Copyright 2003, 2007, 2008, 2009, 2010.
   Authors: Jesse Andrews, David Bruce, Mobin Mohan.
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   theme.c is part of Tux Typing, a.k.a "tuxtype".

Tux Typing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tux Typing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "globals.h"
#include "funcs.h"
#include "SDL_extras.h"


#define MAX_LANGUAGES 100



void ChooseTheme(void)
{
  SDL_Surface* titles[MAX_LANGUAGES] = {NULL};
  SDL_Surface* select[MAX_LANGUAGES] = {NULL};
  SDL_Surface* left = NULL;
  SDL_Surface* right = NULL;
  SDL_Surface* world = NULL;
  SDL_Surface* map = NULL;
  SDL_Surface* photo = NULL;
  SDL_Rect leftRect, rightRect;
  SDL_Rect worldRect, photoRect;
  SDL_Rect titleRects[8];

  int stop = 0;
  int loc = 0;
  int old_loc = 1;

  int themes = 1;
  int i;
  char fn[FNLEN];
  char themeNames[MAX_LANGUAGES][FNLEN];
  char themePaths[MAX_LANGUAGES][FNLEN];

  int old_use_english;
  char old_theme_path[FNLEN];

  DIR* themesDir = NULL;
  struct dirent* themesFile = NULL;

  /* save previous settings in case we back out: */
  old_use_english = settings.use_english;
  strncpy(old_theme_path, settings.theme_data_path, FNLEN - 1);

  sprintf(fn, "%s/themes/", settings.default_data_path);
  themesDir = opendir(fn);

  if (!themesDir)
  {
    fprintf(stderr, "ChooseTheme() - cannot open themes directory!");
    return;
  }

  do  /* Iterate until readdir() returns NULL: */
  {
    themesFile = readdir(themesDir);
    if (!themesFile)
      break;

    /* we ignore any hidden file and CVS */
    if (themesFile->d_name[0] == '.') 
      continue;
    if (strcmp("CVS", themesFile->d_name)==0)
      continue;

    /* check to see if it is a directory */
    sprintf(fn, "%s/themes/%s", settings.default_data_path, themesFile->d_name);

    /* CheckFile() returns 2 if dir, 1 if file, 0 if neither: */
    if (CheckFile(fn) == 2)
    {
      /* HACK: we should get the names from file :) */
      strncpy( themeNames[themes], themesFile->d_name, FNLEN-1);
      /* Make sure theme name is capitalized: */
      themeNames[themes][0] = toupper(themeNames[themes][0]);
      strncpy( themePaths[themes++], themesFile->d_name, FNLEN-1 );
    }
  } while (1);

  closedir(themesDir);

  settings.use_english = 1;

  titles[0] = BlackOutline( "English", DEFAULT_MENU_FONT_SIZE, &white );
  select[0] = BlackOutline( "English", DEFAULT_MENU_FONT_SIZE, &yellow);
  for (i = 1; i < themes; i++)
  {
    titles[i] = BlackOutline( themeNames[i], DEFAULT_MENU_FONT_SIZE, &white );
    select[i] = BlackOutline( themeNames[i], DEFAULT_MENU_FONT_SIZE, &yellow);
  }

  LoadBothBkgds("main_bkg.png");

  world = LoadImage("world.png", IMG_ALPHA);
  left = LoadImage("left.png", IMG_ALPHA);
  right = LoadImage("right.png", IMG_ALPHA);

  if (!world || !left || !right || !CurrentBkgd())
  {
    fprintf(stderr, "ChooseTheme() - could not load needed image.\n");
    return;
  }

  worldRect.x = screen->w - world->w;
  worldRect.w = world->w;
  worldRect.y = 10;
  worldRect.h = world->h;

  leftRect.w = left->w;
  leftRect.h = left->h;
  leftRect.x = 160 - 80 - (leftRect.w/2);
  leftRect.y = 430;

  rightRect.w = right->w;
  rightRect.h = right->h;
  rightRect.x = 160 + 80 - (rightRect.w/2); 
  rightRect.y = 430;

  /* set initial rect sizes */
  titleRects[0].y = 30;
  titleRects[0].w = titleRects[0].h = titleRects[0].x = 0;
  for (i = 1; i < 8; i++)
  {
    titleRects[i].y = titleRects[i - 1].y + 50;
    titleRects[i].w = titleRects[i].h = titleRects[i].x = 0;
  }

  settings.use_english = old_use_english;

  while (!stop)
  {
    while (SDL_PollEvent(&event)) 
      switch (event.type)
      {
        case SDL_QUIT:
        exit(0);
        break;

        case SDL_MOUSEMOTION:
          for (i = 0; (i < 8) && (loc - (loc%8) + i < themes); i++)
            if (inRect( titleRects[i], event.motion.x, event.motion.y ))
            {
              loc = loc-(loc%8)+i;
              break;
            }

          break;

        case SDL_MOUSEBUTTONDOWN: 
          if (inRect( leftRect, event.button.x, event.button.y )) 
            if (loc-(loc%8)-8 >= 0)
            {
              loc=loc-(loc%8)-8;
              break;
            }

          if (inRect( rightRect, event.button.x, event.button.y )) 
            if (loc-(loc%8)+8 < themes)
            {
              loc=loc-(loc%8)+8;
              break;
            }

          for (i=0; (i<8) && (loc-(loc%8)+i<themes); i++) 
            if (inRect(titleRects[i], event.button.x, event.button.y))
            {
              loc = loc-(loc%8)+i;
              if (loc)
              {
                /* --- set theme --- */
                SetupPaths(themePaths[loc]);
              }
              else
              {
                /* --- english --- */
                SetupPaths(NULL);
              }

              stop = 1;
              break;
            }
          break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            settings.use_english = old_use_english;
            strncpy(settings.theme_data_path, old_theme_path, FNLEN - 1);
            stop = 1; 
            break; 
          }

          if (event.key.keysym.sym == SDLK_RETURN)
          { 
            if (loc)
            {
              /* --- set theme --- */
              SetupPaths(themePaths[loc]);
            }
            else
            {
              /* --- English --- */
              SetupPaths(NULL);
            }

            stop = 1;
            break;
          }

          if ((event.key.keysym.sym == SDLK_LEFT)
           || (event.key.keysym.sym == SDLK_PAGEUP))
          {
            if (loc-(loc%8)-8 >= 0) 
              loc=loc-(loc%8)-8;
          }

          if ((event.key.keysym.sym == SDLK_RIGHT)
           || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc-(loc%8)+8 < themes)
              loc=(loc-(loc%8)+8);
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
            if (loc+1<themes)
              loc++;
          }
        }

    if (old_loc != loc)
    {
      int start;

      SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );
      SDL_BlitSurface( world, NULL, screen, &worldRect );

      if (loc)
        SetupPaths(themePaths[loc]);
      else
        SetupPaths(NULL);

      map = LoadImage( "map.png", IMG_ALPHA|IMG_NOT_REQUIRED );
      if (map)
      {
        SDL_BlitSurface( map, NULL, screen, &worldRect );
        SDL_FreeSurface( map );
      }

      photo = LoadImage( "photo.png", IMG_ALPHA|IMG_NOT_REQUIRED );
      if (photo)
      {
        photoRect.x = 480 - (photo->w/2);
        photoRect.y = 250;
        photoRect.w = photo->w;
        photoRect.h = photo->h;
        SDL_BlitSurface( photo, NULL, screen, &photoRect );
        SDL_FreeSurface( photo );
      }

      start = loc - (loc % 8);

      for (i = start; i<MIN(start+8,themes); i++)
      {
        titleRects[i%8].x = 160 - (titles[i]->w/2);

        if (i == loc){
          SDL_BlitSurface(select[loc], NULL, screen, &titleRects[i%8]);
		  if (loc != 0)
			T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",themeNames[loc]);
		  else
		    T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"English");
	     }
	     else
	     SDL_BlitSurface(titles[i], NULL, screen, &titleRects[i%8]);
      }

      /* --- draw buttons --- */
      if (start>0) 
        SDL_BlitSurface( left, NULL, screen, &leftRect );

      if (start+8<themes) 
        SDL_BlitSurface( right, NULL, screen, &rightRect );

      SDL_UpdateRect(screen, 0, 0, 0 ,0);
    }
    SDL_Delay(40);
    old_loc = loc;
  }

  /* --- clear graphics before quitting --- */ 

  for (i = 0; i<themes; i++)
  {
    SDL_FreeSurface(titles[i]);
    SDL_FreeSurface(select[i]);
  }

  SDL_FreeSurface(world);
  SDL_FreeSurface(left);
  SDL_FreeSurface(right);

}
