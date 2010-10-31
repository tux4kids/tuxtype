/*
   setup.c:

   Initialization of SDL and other libraries.
   
   Copyright 2000, 2003, 2007, 2008, 2009, 2010.
   Authors: Sam Hart, Jesse Andrews, David Bruce.
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   setup.c is part of Tux Typing, a.k.a "tuxtype".

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


int fs_res_x = 0;
int fs_res_y = 0;

/* Local function prototypes: */
static void seticon(void);
static int load_settings_fp(FILE* fp);
static int load_settings_filename(const char* fn);

/***************************
	GraphicsInit: Initializes the graphic system
****************************/
void GraphicsInit(void)
{
  const SDL_VideoInfo* video_info = SDL_GetVideoInfo();
  Uint32 surface_mode = 0;

  DEBUGCODE
  { fprintf(stderr, "Entering GraphicsInit()\n"); };

  //Set application's icon:
  seticon();
  //Set caption:
  SDL_WM_SetCaption("Tux Typing", "TuxType");

  if (video_info->hw_available)
  {
    surface_mode = SDL_HWSURFACE;
    LOG("HW mode\n");
  }
  else
  {
    surface_mode = SDL_SWSURFACE;
    LOG("SW mode\n");
  }

  // Determine the current resolution: this will be used as the
  // fullscreen resolution, if the user wants fullscreen.
  DEBUGCODE
  {
    fprintf(stderr, "Current resolution: w %d, h %d.\n", 
            video_info->current_w, video_info->current_h);
  }

  /* For fullscreen, we try to use current resolution from OS: */
  
  fs_res_x = video_info->current_w;
  fs_res_y = video_info->current_h;

  if (settings.fullscreen == 1)
  {
    screen = SDL_SetVideoMode(fs_res_x, fs_res_y, BPP, SDL_FULLSCREEN | surface_mode);
    if (screen == NULL)
    {
      fprintf(stderr,
            "\nWarning: I could not open the display in fullscreen mode.\n"
            "The Simple DirectMedia error that occured was:\n"
            "%s\n\n", SDL_GetError());
      settings.fullscreen = 0;
    }
  }

  /* Either fullscreen not requested, or couldn't get fullscreen in SDL: */
  if (settings.fullscreen == 0)
  {
    screen = SDL_SetVideoMode(RES_X, RES_Y, BPP, surface_mode);
  }

  /* Failed to get a usable screen - must bail out! */
  if (screen == NULL)
  {
    fprintf(stderr,
          "\nError: I could not open the display.\n"
          "The Simple DirectMedia error that occured was:\n"
          "%s\n\n", SDL_GetError());
    exit(2);
  }

  InitBlitQueue();



  DEBUGCODE 
  {
    video_info = SDL_GetVideoInfo();
    fprintf(stderr, "-SDL VidMode successfully set to %ix%ix%i\n",
            video_info->current_w,
            video_info->current_h,
            video_info->vfmt->BitsPerPixel);
  }

	LOG( "GraphicsInit():END\n" );
}

/****************************
	LibInit : Init the SDL
	library
*****************************/
/* NOTE lib_flags is *always* SDL_INIT_VIDEO|SDL_INIT_AUDIO - maybe we */
/* should just simplify all this:                                      */
void LibInit(Uint32 lib_flags)
{
  LOG( "LibInit():\n-About to init SDL Library\n" );

  /* Initialize video: */
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Couldn't initialize SDL: %s\n",
    SDL_GetError());
    exit(2);
  }
  /* Initialize audio if desired: */
  if (settings.sys_sound)
  {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
      fprintf(stderr, "Couldn't initialize SDL Sound: %s\n",
              SDL_GetError());
      settings.sys_sound = 0;
    }
    else
      LOG("SDL_InitSubSystem(SDL_INIT_AUDIO) succeeded\n");
  }

// atexit(SDL_Quit); // fire and forget... 

  LOG( "-SDL Library init'd successfully\n" );

  DEBUGCODE
  { fprintf(stderr, "settings.sys_sound = %d\n", settings.sys_sound); }

  /* FIXME should read settings before we do this: */ 
  if (settings.sys_sound) //can be turned off with "--nosound" runtime flag
  {
    int initted = 1;

    /* For SDL_mixer 1.2.10 and later, we must call Mix_Init() before any */
    /* other SDL_mixer functions. We can see what types of audio files    */
    /* are supported at this time (ogg and mod are required):             */

#ifdef HAVE_MIX_INIT
    int flags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_FLAC;
    initted = Mix_Init(flags);

    /* Just give warnings if MP3 or FLAC not supported: */
    if((initted & MIX_INIT_MP3) != MIX_INIT_MP3)
      LOG("NOTE - MP3 playback not supported\n");
    if((initted & MIX_INIT_FLAC) != MIX_INIT_FLAC)
      LOG("NOTE - MP3 playback not supported\n");

    /* We must have Ogg and Mod support to have sound: */
    if((initted & (MIX_INIT_OGG | MIX_INIT_MOD)) != (MIX_INIT_OGG | MIX_INIT_MOD))
    {
      fprintf(stderr, "Mix_Init: Failed to init required ogg and mod support!\n");
      fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
      settings.sys_sound = 0;
      initted = 0;
    }
    else
      LOG("Mix_Init() succeeded\n");
#endif

    DOUT(initted);

    /* If Mix_Init() succeeded (or wasn't required), set audio parameters: */
    if(initted)
    {
      LOG("About to call Mix_OpenAudio():\n");
//    if (Mix_OpenAudio(22050, AUDIO_S16, 1, 2048) == -1)
      if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048) ==
		      -1)
      {
        fprintf(stderr, "Warning: Mix_OpenAudio() failed\n - Reasons: %s\n", SDL_GetError());
        settings.sys_sound = 0;
      }
      else
        LOG("Mix_OpenAudio() successful\n");
    }
  }

  LOG( "-about to init SDL text library (SDL_ttf or SDL_Pango\n" );

  if (!Setup_SDL_Text())
  {
    fprintf( stderr, "Couldn't initialize desired SDL text libary\n" );
    exit(2);
  }
//	atexit(TTF_Quit);

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
  // MDTTEMP: commented out the next line and added the line after
  // snprintf(fn, FNLEN - 1, "userdata/settings.txt");
  snprintf( fn, FNLEN-1, "%s/TuxType/settings.txt", getenv("APPDATA"));
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
    fprintf(stderr, "load_settings_filename(): no theme-specific settings found: %s\n",fn);
    return -1;
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
    if (EOF == fscanf(fp, "%[^=]=%[^\n]\n", setting, value))
      break;

    DEBUGCODE {fprintf(stderr, "%s = %s", setting, value );}
      //For now we are not reading or saving the language selection: 
      //MDTTEMP: uncommented the following 8 lines and joined the else to the if on line 259
    if (strncmp( setting, "lang", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting language to %s\n", value);}
      strncpy(settings.lang, value, FNLEN - 1);
      setting_found = 1;
      SetupPaths(value); /* Does this really belong here? */ 
    }
    else if (strncmp( setting, "o_lives", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting lives to %s\n", value);}
      settings.o_lives = atoi(value);
      setting_found = 1;
    }
    else if (strncmp( setting, "mus_volume", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting music volume to %s\n", value);}
      settings.mus_volume = atoi(value);
      setting_found = 1;
    }
    else if (strncmp(setting, "sfx_volume", FNLEN) == 0)
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting effects volume to %s\n", value);}
      settings.sfx_volume = atoi(value);
      setting_found = 1;
    }
    else if (strncmp(setting, "menu_music", FNLEN) == 0)
    {
      DEBUGCODE {fprintf(stderr, "LoadSettings: Setting menu music to %s\n", value);}
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
      DEBUGCODE {fprintf(stderr, "load_settings_fp(): Setting theme font to %s\n", value);}
      strncpy(settings.theme_font_name, value, FNLEN - 1);
      setting_found = 1;
    }
    else if (strncmp( setting, "theme_locale_name", FNLEN ) == 0 )
    {
      DEBUGCODE {fprintf(stderr, "load_settings_fp(): Setting theme locale to %s\n", value);}
      strncpy(settings.theme_locale_name, value, FNLEN - 1);
      setting_found = 1;
    }
    else
      DEBUGCODE {fprintf(stderr, "load_settings_fp(): unrecognized string: %s\n", value);}

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

	//#ifdef WIN32
    //    //MDTTEMP: Commented out the next line and added the next 2 lines after
	//	//_mkdir( "userdata" );  // just in case try to create save location
	//	snprintf( fn, FNLEN-1, (const char*)"%s/TuxType", getenv("APPDATA") );
	//	_mkdir( fn );  // just in case try to create save location
    //    // MDTTEMP: Commented out the next line and added the line after
	//	//snprintf( fn, FNLEN-1, "userdata/settings.txt" );
	//	snprintf( fn, FNLEN-1, "%s/TuxType/settings.txt", getenv("APPDATA"));
	//#else
	//	snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
	//	mkdir( fn, 0755 ); // just in case try to create save location
	//	snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/settings.txt", getenv("HOME") );
	//#endif
	
	// Use the settings.user_settings_path member
	snprintf( fn, FNLEN-1, (const char*)"%s/settings.txt", settings.user_settings_path );


	DEBUGCODE { printf("SaveSettings: settings file is '%s'\n", fn ); }
	
	LOG("SaveSettings: trying to open settings file\n");
	
	settingsFile = fopen( fn, "w" );

	if (settingsFile == NULL) {
		printf("SaveSettings: Settings file cannot be created!\n");
		return;
	}
	
	/* Save all the settings here! */
//NOTE for now, don't save theme because things get screwed up if the language
// doesn't match the LANG environmental variable - DSB
// MDTTEMP: uncommented the following 2 lines
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


/* NOTE Now default path is _always_ DATA_PREFIX - hackish workarounds removed. */
/* Check for default (English) and theme data paths and update settings struct. */
/* Returns 0 if default data path not found, 1 if successfully located.         */
/* If theme not found, still returns 1 but settings changed to use English.     */
/* TODO should have this function set up the user and global settings paths.    */
/* TODO settings should be re-loaded when theme changes.                        */

int SetupPaths(const char* theme_dir)
{
  DEBUGCODE
  {
    fprintf(stderr, "Entering SetupPaths()\n");
  }

  settings.use_english = 1; // default is to use English if we cannot find theme
  char fn[FNLEN];           // used later when setting settings.user_settings_path

  if (CheckFile(DATA_PREFIX))
  {
    strncpy(settings.default_data_path, DATA_PREFIX, FNLEN - 1);
    DEBUGCODE {fprintf(stderr, "path '%s' found, copy to settings.default_data_path\n", DATA_PREFIX);}
  }
  else
  {
    fprintf(stderr, "Error - DATA_PREFIX = '%s' not found!\n", DATA_PREFIX);
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
      strncpy(settings.theme_font_name, DEFAULT_FONT_NAME, FNLEN);
      
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
      strncpy(settings.theme_font_name, DEFAULT_FONT_NAME, FNLEN);
      strncpy(settings.theme_locale_name, DEFAULT_LOCALE, FNLEN);
      fprintf(stderr, "SetupPaths(): could not find '%s'\n", full_theme_path);
    }
  }
  else /* No theme name passed as arg so just use English: */
  {
    settings.use_english = 1; // default is to use English if we cannot find theme
    strcpy(settings.theme_name, "");
    strncpy(settings.theme_font_name, DEFAULT_FONT_NAME, FNLEN);
    strncpy(settings.theme_locale_name, DEFAULT_LOCALE, FNLEN);
  }



  /* Now check for VAR_PREFIX (for modifiable data shared by all users, */ 
  /* such as custom word lists, high scores, etc:                       */
  /* This will generally be /var/lib/tuxtype (distro-provided pkg)      */
  /* or /usr/local/etc/tuxtype (locally-built and installed pkg)        */
  if (CheckFile(VAR_PREFIX))
  {
    strncpy(settings.var_data_path, VAR_PREFIX, FNLEN - 1);
    DEBUGCODE {fprintf(stderr, "path '%s' found, copy to settings.var_data_path\n", VAR_PREFIX);}
  }
  else
  {
    fprintf(stderr, "Error - VAR_PREFIX = '%s' not found!\n", VAR_PREFIX);
    return 0;
  }

  /* Now check for CONF_PREFIX (for program wide settings that apply to all users). */ 
  /* This would typically be /etc/tuxtype if tuxtype is installed by a distro pkg,  */
  /* or /usr/local/etc/tuxtype if the package is built locally                      */
  if (CheckFile(CONF_PREFIX))
  {
    strncpy(settings.global_settings_path, CONF_PREFIX, FNLEN - 1);
    DEBUGCODE {fprintf(stderr, "path '%s' found, copy to settings.global_settings_path\n", CONF_PREFIX);}
  }
  else
  {
    fprintf(stderr, "Error - CONF_PREFIX = '%s' not found!\n", CONF_PREFIX);
    return 0;
  }


  /* Determine the user data path (for user specific settings)  this would normally be     */
  /* /home/user/.tuxtype for POSIX systems or Documents and Settings/user/Application Data */
  /* for windows systems                                                                   */
  #ifdef WIN32
    snprintf( fn, FNLEN-1, (const char*)"%s/TuxType", getenv("APPDATA") );
  #else
    snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
  #endif

  if (CheckFile(fn))
  {
    strncpy(settings.user_settings_path, fn, FNLEN - 1);
    DEBUGCODE {fprintf(stderr, "path '%s' found, copying to settings.user_settings_path\n", fn);}
  }
  else
  {
    fprintf(stderr, "Error - User Settings Path = '%s' not found, I will attempt to create it.\n", fn);
  #ifdef WIN32
     _mkdir( fn );
  #else
     mkdir( fn, 0755 );
  #endif
    if (CheckFile(fn))
    {
      strncpy(settings.user_settings_path, fn, FNLEN - 1);
      DEBUGCODE {fprintf(stderr, "path '%s' successfully created, copy to settings.user_settings_path\n", fn);}
    }
    else
    {
      //NOTE this shouldn't happen, right? We should always be able to make a directory
      //in our own home folder - DSB
      fprintf(stderr, "Error - Could not create: '%s', falling back to the global settings.\n", fn);
  #ifdef WIN32
      strncpy(settings.user_settings_path, "userdata", FNLEN - 1);
  #else
      // FIXME: in this fallback case if we are forced to use global settings, and /etc/tuxtype
      // contains those settings we can't write to it, so use /tmp - but then we have nothing to
      // read from, copy the global settings to temp first?  Or just forget about saving the settings
      // when we exit? - MDT
      strncpy(settings.user_settings_path, "/tmp", FNLEN - 1);
  #endif
    }
  }

DEBUGCODE
  {
    fprintf(stderr, "Leaving SetupPaths():\n");
    fprintf(stderr, "default_data_path: '%s'\n", settings.default_data_path);
    fprintf(stderr, "theme_data_path: '%s'\n\n", settings.theme_data_path);
    fprintf(stderr, "var_data_path: '%s'\n\n", settings.var_data_path);
    fprintf(stderr, "user_settings_path: '%s'\n\n", settings.user_settings_path);
    fprintf(stderr, "global_settings_path: '%s'\n\n", settings.global_settings_path);
  }

  return 1;	
}


/* Set the application's icon: */

static void seticon(void)
{
  SDL_Surface* icon;
  int colorkey;

  /* Load icon into a surface: */
  icon = IMG_Load(DATA_PREFIX "/images/icons/icon.png");
  if (icon == NULL)
  {
    fprintf(stderr,
            "\nWarning: I could not load the icon image: %s\n"
            "The Simple DirectMedia error that occured was:\n"
            "%s\n\n", DATA_PREFIX "/images/icons/icon.png", SDL_GetError());
    return;
  }

  /* Set up key for transparency: */
  colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
  SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);              

  SDL_WM_SetIcon(icon,NULL);

  SDL_FreeSurface(icon);
}


void Cleanup(void)
{
  SDL_FreeSurface(screen);
  screen = NULL;
  Cleanup_SDL_Text();
  SDL_Quit();
}
