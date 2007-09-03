/***************************************************************************
                          setup.c 
 -  description: Init SDL
                             -------------------
    begin                : Thu May 4 2000
    copyright            : (C) 2000 by Sam Hart
                         : (C) 2003 by Jesse Andrews
    email                : tuxtype-dev@tux4kids.net
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

#define NUM_PATHS 4

const char PATHS[NUM_PATHS][FNLEN] = 
{
  "./data",
  "/usr/share/"PACKAGE"/data",
  "/usr/local/share/"PACKAGE"/data",
  DATA_PREFIX"/share/"PACKAGE"/data"
};


/* Local function prototypes: */
static int load_settings_fp(FILE* fp);
static int load_settings_filename(const char* fn);

/***************************
	GraphicsInit: Initializes the graphic system
****************************/
void GraphicsInit(Uint32 video_flags)
{
	LOG( "GraphicsInit - Initialize graphic system\n" );

	DEBUGCODE {
		fprintf(stderr, "-SDL Setting VidMode to %ix%ix%i\n", RES_X, RES_Y, BPP);
	}

	/* NOTE fullscreen vs. windowed is indicated by video_flags */
	screen = SDL_SetVideoMode(RES_X, RES_Y, BPP, video_flags);

	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n", RES_X, RES_Y, SDL_GetError());
		exit(2);
	}


	LOG( "SDL_SetClipRect(screen, NULL):\n" );

	SDL_SetClipRect(screen, NULL); // Let's set the appropriate clip rect  -- JA: is neccessary???  

	LOG( "SDL_ShowCursor(0):\n" );

	SDL_ShowCursor(0); // no cursor please

	LOG( "SDL_WM_SetCaption(\"Tux Typing\", PACKAGE);\n" );

	SDL_WM_SetCaption("Tux Typing", "tuxtype"); // Set window manager stuff

	/* --- setup color we use --- */
	black.r       = 0x00; black.g       = 0x00; black.b       = 0x00;
        gray.r        = 0x80; gray.g        = 0x80; gray.b        = 0x80;
	dark_blue.r   = 0x00; dark_blue.g   = 0x00; dark_blue.b   = 0x60; 
	red.r         = 0xff; red.g         = 0x00; red.b         = 0x00;
	white.r       = 0xff; white.g       = 0xff; white.b       = 0xff;
	yellow.r      = 0xff; yellow.g      = 0xff; yellow.b      = 0x00; 

	InitEngine();

	DEBUGCODE {
		fprintf(stderr, "-SDL VidMode successfully set to %ix%ix%i\n", RES_X, RES_Y, BPP);
	}

	LOG( "GraphicsInit():END\n" );
}

/****************************
	LibInit : Init the SDL
	library
*****************************/
void LibInit(Uint32 lib_flags)
{
	LOG( "LibInit():\n-About to init SDL Library\n" );

	if (SDL_Init(lib_flags) < 0) 
		/* FIXME this looks wrong - if no sys_sound, we don't init video??? */
		if (settings.sys_sound) {
			if (SDL_Init(SDL_INIT_VIDEO) < 0) {
				fprintf(stderr, "Couldn't initialize SDL: %s\n",
				SDL_GetError());
				exit(2);
			} else {
				LOG( "Couldn't initialize SDL Sound\n" );
				settings.sys_sound = 0;
			}
		}


//	atexit(SDL_Quit); // fire and forget... 

	LOG( "-SDL Library init'd successfully\n" );

	/* FIXME should read settings before we do this: */ 
	if (settings.sys_sound)
        { 
          if (Mix_OpenAudio(22050, AUDIO_S16, 1, 2048) == -1)
          {
            fprintf( stderr, "Warning: couldn't set 22050 Hz 8-bit audio\n - Reasons: %s\n", SDL_GetError());
            settings.sys_sound=0;
          }
          else
            LOG("Mix_OpenAudio() successful\n");
        }

	LOG( "-about to init SDL_ttf\n" );

	if (TTF_Init() < 0) {
		fprintf( stderr, "Couldn't initialize SDL_ttf\n" );
		exit(2);
	}

//	atexit(TTF_Quit);

	SDL_EnableKeyRepeat( 0, SDL_DEFAULT_REPEAT_INTERVAL );
	/* Need this to get Unicode values from keysyms: */
	SDL_EnableUNICODE(1);

	LOG( "LibInit():END\n" );
}

/* Load the settings from a file... make sure to update SaveSettings if you change
 *  what can be saved/loaded 
 */
void LoadSettings(void)
{
  char fn[FNLEN];
// 	char setting[FNLEN];
// 	char value[FNLEN];
//	FILE *settingsFile;
	
  /* set the settings directory/file */

#ifdef WIN32
  snprintf(fn, FNLEN - 1, "userdata/settings.txt");
  LOG("WIN32 defined\n");
#else
  snprintf(fn, FNLEN - 1, (const char*)"%s/.tuxtype/settings.txt", getenv("HOME"));
  LOG("WIN32 not defined\n");
#endif

  DEBUGCODE { printf("LoadSettings: settings file is '%s'\n", fn ); }

  LOG("LoadSettings: trying to open settings file\n");

  load_settings_filename(fn);
}



/* Load the settings if given the complete pathname to the settings file.  Returns 1 if
   able to call load_settings_fp() successfully on named file.
 */
static int load_settings_filename(const char* fn)
{
  FILE* fp = fopen(fn, "r");

  if (!fp)
  {
    fprintf(stderr, "load_settings_filename(): no theme-specific settings found\n");
    return;
  }
	
  if (!load_settings_fp(fp))
  {
    fprintf(stderr, "No settings in settings file.\n");
    fclose(fp); /* still need to close fp */
    return 0;
  }

  /* Success! */
  fclose(fp);
  return 1;
}


/* Load the settings if given an open FILE* pointer to the settings file.  Returns 1 if
   at least one setting value found, 0 otherwise. It does not close the FILE*.
 */
static int load_settings_fp(FILE* fp)
{
  char setting[FNLEN]; /* these don't really need to be 'FNLEN' long */
  char value[FNLEN];
  int setting_found = 0;

  if (!fp)
  {
    fprintf(stderr, "load_settings_fp() - FILE* parameter NULL\n");
    return 0;
  }

  /* we load all the settings here */
  while (!feof(fp))
  {
    fscanf(fp, "%[^=]=%[^\n]\n", setting, value );

    DEBUGCODE {fprintf(stderr, "%s = %s", setting, value );}

    if (strncmp( setting, "lang", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting language to %s", value);}
      strncpy(settings.lang, value, FNLEN - 1);
      setting_found = 1;
      SetupPaths(value); /* Does this really belong here? */ 
    }
    else if (strncmp( setting, "o_lives", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting lives to %s", value);}
      settings.o_lives = atoi(value);
      setting_found = 1;
   }
    else if (strncmp( setting, "mus_volume", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting music volume to %s", value);}
      settings.mus_volume = atoi(value);
      setting_found = 1;
    }
    else if (strncmp(setting, "sfx_volume", FNLEN) == 0)
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting effects volume to %s", value);}
      settings.sfx_volume = atoi(value);
      setting_found = 1;
    }
    else if (strncmp(setting, "menu_music", FNLEN) == 0)
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting menu music to %s", value);}
      settings.menu_music = atoi(value);
      setting_found = 1;
    }
    else if (strncmp( setting, "fullscreen", FNLEN ) == 0 )
    {
      settings.fullscreen = atoi(value);
      setting_found = 1;
    }
    else if (strncmp( setting, "theme_font_name", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "load_settings_fp(): Setting theme font to %s", value);}
      strncpy(settings.theme_font_name, value, FNLEN - 1);
      setting_found = 1;
    }
    else
      DEBUGCODE {fprintf(stderr, "load_settings_fp(): unrecognized string: %s", value);}

  }


  if (setting_found)
    return 1;
  else
  {
    fprintf(stderr, "load_settings_fp() - no settings in file - empty or corrupt?\n");
    return 0;
  }
}



/* Save the settings from a file... make sure to update LoadSettings if you change
 *  what can be saved/loaded 
 */
void SaveSettings(void)
{
	char fn[FNLEN];
	FILE* settingsFile;
	
	/* set the settings directory/file */

	#ifdef WIN32
		_mkdir( "userdata" );  // just in case try to create save location
		snprintf( fn, FNLEN-1, "userdata/settings.txt" );
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
		mkdir( fn, 0755 ); // just in case try to create save location
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/settings.txt", getenv("HOME") );
	#endif


	DEBUGCODE { printf("SaveSettings: settings file is '%s'\n", fn ); }
	
	LOG("SaveSettings: trying to open settings file\n");
	
	settingsFile = fopen( fn, "w" );

	if (settingsFile == NULL) {
		printf("SaveSettings: Settings file cannot be created!\n");
		return;
	}
	
	/* Save all the settings here! */
	if (strncmp(settings.theme_name, "", FNLEN) != 0)
		fprintf( settingsFile, "lang=%s\n", settings.theme_name );
	if (settings.o_lives > 9)
		fprintf( settingsFile, "o_lives=%d\n", settings.o_lives );

	fprintf( settingsFile, "mus_volume=%d\n", settings.mus_volume );
	fprintf( settingsFile, "sfx_volume=%d\n", settings.sfx_volume );
	fprintf( settingsFile, "menu_music=%d\n", settings.menu_music );
	fprintf( settingsFile, "fullscreen=%d\n", settings.fullscreen);


// 	if (screen->flags & SDL_FULLSCREEN){
// 		fprintf( settingsFile, "fullscreen=%s\n", "1");
// 	} else {
// 		fprintf( settingsFile, "fullscreen=%s\n", "0");
// 	}
	fclose(settingsFile);
}


/* Check for default (English) and theme data paths and update settings struct. */
/* Returns 0 if default data path not found, 1 if successfully located.         */
/* If theme not found, still returns 1 but settings changed to use English.     */
/* TODO should have this function set up the user and global settings paths.    */
/* TODO settings should be re-loaded when theme changes.                        */

int SetupPaths(const char* theme_dir)
{
  int i;
  settings.use_english = 1; // default is to use English if we cannot find theme

  /* First find default data path: */
  for (i = 0; i < NUM_PATHS; i++)
  {

    DEBUGCODE
    {
      fprintf(stderr, "SetupPaths(): checking for '%s' as default data path\n", PATHS[i]);
    }

    if (CheckFile(PATHS[i]))
    {
      strncpy(settings.default_data_path, PATHS[i], FNLEN - 1);

      DEBUGCODE
      {
        fprintf(stderr, "path '%s' found, copy to settings.default_data_path\n", PATHS[i]);
      }
      break;
    }
    else
    {
      DEBUGCODE
      {
        fprintf(stderr, "path '%s' not found.\n", PATHS[i]);
      }
    }
  }

  /* If we didn't find a data path, print error msg and get out: */
  if (i >= NUM_PATHS) /* (shouldn't actually ever be > NUM_PATHS) */
  {
    fprintf(stderr, "SetupPaths(): Error - could not find data path.\n");
    return 0;
  }


  /* Now look for theme directory: */
  if (theme_dir != NULL)
  {
    char full_theme_path[FNLEN];
    char theme_settings_path[FNLEN];

    sprintf(full_theme_path, "%s/themes/%s", settings.default_data_path, theme_dir);

    DEBUGCODE
    {
      fprintf(stderr, "SetupPaths(): checking for '%s' as theme path\n", full_theme_path);
    }

    if (CheckFile(full_theme_path)) /* Theme found - set it up! */
    {
      settings.use_english = 0;
      strncpy(settings.theme_data_path, full_theme_path, FNLEN - 1);
      DEBUGCODE
      {
        fprintf(stderr, "settings.theme_data_path is: %s\n", settings.theme_data_path);
      }
 
      strncpy(settings.theme_name, theme_dir, FNLEN - 1);
      /* (Need to do this in case we are changing from a theme with */
      /* a special font to a theme that uses the default, but lacks */
      /* an explicit statement to use the default(                  */
      strncpy(settings.theme_font_name, DEFAULT_MENU_FONT, FNLEN);

      /* Load fontname or any other theme-specific settings: */
      sprintf(theme_settings_path, "%s/settings.txt", full_theme_path);

      DEBUGCODE
      {
        fprintf(stderr, "theme_settings_path is: %s\n", theme_settings_path);
      }

      load_settings_filename(theme_settings_path);
    }
    else /* Theme not found! */
    {
      settings.use_english = 1; // default is to use English if we cannot find theme
      strcpy(settings.theme_name, "");
      strncpy(settings.theme_font_name, DEFAULT_MENU_FONT, FNLEN);
      fprintf(stderr, "SetupPaths(): could not find '%s'\n", full_theme_path);
    }
  }
  else /* No theme name passed as arg so just use English: */
  {
    settings.use_english = 1; // default is to use English if we cannot find theme
    strcpy(settings.theme_name, "");
  }


  DEBUGCODE
  {
    fprintf(stderr, "Leaving SetupPaths():\n");
    fprintf(stderr, "default_data_path: '%s'\n", settings.default_data_path);
    fprintf(stderr, "theme_data_path: '%s'\n\n", settings.theme_data_path);
  }
  return 1;	
}


void Cleanup(void)
{
  SDL_FreeSurface(screen);
  screen = NULL;

  SDL_Quit();
  TTF_Quit();
}
