/*
   main.c:
   
   main() function for Tux Typing

   Copyright 2000, 2003, 2007, 2008, 2009, 2010.
   Authors: Sam Hart, Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   main.c is part of Tux Typing, a.k.a "tuxtype".

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

SDL_Surface* screen;
SDL_Event  event;




/********************
  main : init stuff
*********************/
int main(int argc, char *argv[])
{
  Uint32 lib_flags = 0;
  int i;

  srand(time(NULL));

  //FIXME we should inspect return values and bail if these functions fail:
  Opts_Initialize(); // First, initialize settings with hard-coded defaults 
  // This sets settings.default_data_path to the default theme file path:
  if(!SetupPaths(NULL))
  {  
    fprintf(stderr, "Error - SetupPaths() failed, exiting program.\n");
    return 0;
  }  
  LoadSettings();    // Second, read saved any saved settings

  // Third, check command line args as these should override saved settings
  if (argc > 1) /* FIXME this could go into something like HandleCommandArgs() */
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
        fprintf(stderr, "Copyright 2000-2010 Sam Hart, Jesse Andrews, David Bruce, and the Tux4Kids team.\n");
        fprintf(stderr, "Tux4Kids website: http://tux4kids.alioth.debian.org\n");
        fprintf(stderr, "This software is licensed under the GNU General Public License, Version 3 or later, and compatible media licenses.\n");
        fprintf(stderr, "See COPYING file for licensing details.\n\n");
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

  //Now initialize locale/gettext system.
  //This is done after reading settings just so we can print
  //if the "-d" debug flag is set.
  {
    char *s1, *s2, *s3, *s4;

    s1 = setlocale(LC_ALL, "");
    s2 = bindtextdomain(PACKAGE, TUXLOCALE);
    s3 = bind_textdomain_codeset(PACKAGE, "UTF-8");
    s4 = textdomain(PACKAGE);

    DEBUGCODE
    {
      fprintf(stderr, "PACKAGE = %s\n", PACKAGE);
      fprintf(stderr, "TUXLOCALE = %s\n", TUXLOCALE);
      fprintf(stderr, "setlocale(LC_ALL, \"\") returned: %s\n", s1);
      fprintf(stderr, "bindtextdomain(PACKAGE, TUXLOCALE) returned: %s\n", s2);
      fprintf(stderr, "bind_textdomain_codeset(PACKAGE, \"UTF-8\") returned: %s\n", s3);
      fprintf(stderr, "textdomain(PACKAGE) returned: %s\n", s4);
      fprintf(stderr, "gettext(\"Fish\"): %s\n\n", gettext("Fish"));
      fprintf(stderr, "After gettext() call\n");
    }
  }


  lib_flags = SDL_INIT_VIDEO;

  lib_flags |= SDL_INIT_AUDIO;

  LibInit(lib_flags); /* calls SDL_Init(), TTF_Init(), some other settings */
  GraphicsInit(); /* calls SDL_SetVideoMode(), a few others     */

  if (settings.sys_sound)
  {
    Mix_VolumeMusic(settings.mus_volume);
    Mix_Volume(-1, settings.sfx_volume);
  }

  /* FIXME: we should check config files/environment variables like LANG! */
  /* NOTE what should we do if LANG is something without a theme - should */
  /* we then default to English?                                          */

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
