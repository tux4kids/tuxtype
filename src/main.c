/***************************************************************************
                          main.c 
 -  description: main tux type file
                             -------------------
    begin                : Tue May  2 13:25:06 MST 2000
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

SDL_Surface* screen;

SDL_Event  event;

/* the colors we use throughout the game */
SDL_Color black;
SDL_Color gray;
SDL_Color dark_blue;
SDL_Color red;
SDL_Color white;
SDL_Color yellow;


/********************
  main : init stuff
*********************/
int main(int argc, char *argv[])
{
  Uint32 lib_flags = 0;
  int i;

  /* Initialize settings with hard-coded defaults: */ 
  Opts_Initialize();

  srand(time(NULL));

  // This sets settings.default_data_path to the default theme file path:
  SetupPaths(NULL);

  LoadSettings();
  DEBUGCODE { printf("Window setting from config file is: %d\n", settings.fullscreen);}

  lib_flags = SDL_INIT_VIDEO;

  /* FIXME this could go into something like HandleCommandArgs() */
  // check command line args
  if (argc > 1)
  { 
    for (i = 1; i < argc; i++)
    {
      if (  (strcmp(argv[i], "-h") == 0)
         || (strcmp(argv[i], "--help") == 0)
         || (strcmp(argv[i], "-help") == 0))
      {
        fprintf(stderr, "\nUsage:\n tuxtype [OPTION]...");
        fprintf(stderr, "\n\nOptions:\n\n\t-h, --help, -help");
        fprintf(stderr, "\n\t\tPrints this help message");
        fprintf(stderr, "\n\n\t-f, --fullscreen");
        fprintf(stderr, "\n\t\tSelects fullscreen display (default)");
        fprintf(stderr, "\n\n\t-w, --window");
        fprintf(stderr, "\n\t\tSelects windowed display (not fullscreen)");
        fprintf(stderr, "\n\n\t-s, --sound");
        fprintf(stderr, "\n\t\tAllow in-game sounds (default)");
        fprintf(stderr, "\n\n\t-ns, --nosound");
        fprintf(stderr, "\n\t\tDisables in-game sounds");
        fprintf(stderr, "\n\n\t-t {THEME}, --theme {THEME}");
        fprintf(stderr, "\n\t\tUse theme named {THEME}, if it exists");
        fprintf(stderr, "\n\n\t-sp, --speed");
        fprintf(stderr, "\n\t\tSpeed up gameplay (for use on slower");
        fprintf(stderr, "\n\t\tmachines)");
        fprintf(stderr, "\n\n\t-d, --debug");
        fprintf(stderr, "\n\t\tEnable debug mode (output)\n");
        fprintf(stderr, "\n\n\t-v, --version");
        fprintf(stderr, "\n\t\tDisplay version number and exit\n");
        exit(0);
      }

      if (  (strcmp(argv[i], "-v") == 0)
         || (strcmp(argv[i], "--version") == 0))
      {
        fprintf(stderr, "\n%s, Version %s\n", PACKAGE, VERSION);
        fprintf(stderr, "Copyright (C) Sam Hart <hart@geekcomix.com>, under the GPL\n");
        fprintf(stderr, "-See COPYING file for more info... Thx ;)\n\n");
        exit(0);
      }

      if (  (strcmp(argv[i], "-f") == 0)
         || (strcmp(argv[i], "--fullscreen") == 0))
        settings.fullscreen = 1;

      if (  (strcmp(argv[i], "-w") == 0)
         || (strcmp(argv[i], "--window") == 0))
        settings.fullscreen = 0;

      if (  (strcmp(argv[i], "-sp") == 0)
         || (strcmp(argv[i], "--speed") == 0))
        settings.speed_up = 1;

      if (  (strcmp(argv[i], "-d") == 0)
         || (strcmp(argv[i], "--debug") == 0))
        settings.debug_on = 1;

      if (  (strcmp(argv[i], "-s") == 0)
         || (strcmp(argv[i], "--sound") == 0))
        settings.sys_sound = 1;

      if (  (strcmp(argv[i], "-ns") == 0)
         || (strcmp(argv[i], "--nosound") == 0))
        settings.sys_sound = 0;

      if (  (strcmp(argv[i], "--hidden") == 0)
         || (strcmp(argv[i], "-hidden") == 0))
        settings.hidden = 1;

      if (  (strcmp(argv[i], "-t") == 0)
         || (strcmp(argv[i], "--theme") == 0))
        SetupPaths(argv[++i]);
    }
  }

  DEBUGCODE
  {
    fprintf(stderr, "\n%s, version %s BEGIN\n", PACKAGE, VERSION);
  }

  lib_flags |= SDL_INIT_AUDIO;

  LibInit(lib_flags); /* calls SDL_Init(), TTF_Init(), some other settings */
  GraphicsInit(); /* calls SDL_SetVideoMode(), a few others     */

  #ifdef HAVE_LIBSDL_PANGO
  init_SDLPango_Context();
  #endif

  if (settings.sys_sound)
  {
    Mix_VolumeMusic(settings.mus_volume);
    Mix_Volume(-1, settings.sfx_volume);
  }

  /* FIXME: we should check config files/environment variables like LANG! */
  LoadLang();
  LoadKeyboard();

  /* Now actually play the game: */
  TitleScreen();

  SaveSettings();

  /* Release heap: */
  Cleanup();

  LOG( "---GAME DONE, EXIT---- Thank you.\n" );

  return EXIT_SUCCESS;
}
