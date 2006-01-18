/***************************************************************************
                          main.c 
 -  description: main tux type file
                             -------------------
    begin                : Tue May  2 13:25:06 MST 2000
    copyright            : (C) 2000 by Sam Hart
    email                : hart@geekcomix.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "funcs.h"

/*************************
        FreeMem : Free the
        used memory in the
        game (wrapper func)
**************************/
void FreeMem(int verbose)
{// free.... your.... mind...
    FreeGraphics(verbose);
    //FreeGame(verbose);
}

/**************************
        RunGame : Run the
        game (main loop)
***************************/
void RunGame(int verbose)
{//run the game
    int             still_playing = YES;
    int             game_type;

    if (verbose == YES) {
        fprintf(stderr, "RunGame():\n");
        fprintf(stderr, "-Begin playing\n");
    }

    while (still_playing == YES) {
        if (verbose == YES)
            fprintf(stderr, "-Calling Title Screen\n");

        game_type = TitleScreen(verbose);

        if (verbose == YES)
            fprintf(stderr, "-Title Screen returned: %i\n", game_type);

        if (game_type != DONE) {
            if (game_type == KCASCADE1)
                still_playing = PlayCascade(EASY, verbose);
            if (game_type == KCASCADE2)
                still_playing = PlayCascade(MEDIUM, verbose);
            if (game_type == KCASCADE3)
                still_playing = PlayCascade(HARD, verbose);
            if (game_type == WCASCADE1)
                still_playing = PlayWCascade(EASY, verbose);
            if (game_type == WCASCADE2)
                still_playing = PlayWCascade(MEDIUM, verbose);
            if (game_type == WCASCADE3)
                still_playing = PlayWCascade(HARD, verbose);
            if (game_type == PKCASCADE)
                still_playing = PracticeKCascade(verbose);
            if (game_type == PWCASCADE)
                still_playing = PracticeWCascade(verbose);
            if (game_type == PFREETYPE)
                still_playing = PlayCascade(INF_PRACT, verbose);
        } else
            still_playing = NO;
    }

    if (verbose == YES) {
        fprintf(stderr, "RunGame():END\n");
    }
}

/********************
  main : init stuff
*********************/
int main(int argc, char *argv[])
{
    Uint32          video_flags,
                    lib_flags;
    int i;
/*    char * home; */

    int verbose = NO;       //default is not verbose (it can slow things down)
    sys_sound = YES;        //default using system sounds
    speed_up = NO;          //run at normal speed
    scroll = YES;           //default with scrolling items
    use_alpha = NO;        //default NOT using alpha blended words
    hidden = NO;
    use_theme = NO;         //default to not using theme
    memset(alt_theme_path, 0, FNLEN);

    // init flags
#ifdef WIN32
    video_flags = (SDL_HWSURFACE);
#else
    video_flags = (SDL_FULLSCREEN | SDL_HWPALETTE);
#endif

    //video_flags = (SDL_FULLSCREEN | SDL_HWPALETTE); // | SDL_HWSURFACE | SDL_DOUBLEBUF);
    //lib_flags = (SDL_INIT_AUDIO | SDL_INIT_VIDEO);

    // check command line args
    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if ((strcmp(argv[i], "-h") == 0) |
                (strcmp(argv[i], "--help") ==
                 0) | (strcmp(argv[i], "-help") == 0)) {
                fprintf(stderr, "\nUsage:\n tuxtype [OPTION]...");
                fprintf(stderr, "\n\nOptions:\n\n\t-h, --help, -help");
                fprintf(stderr, "\n\t\tPrints this help message");
                fprintf(stderr, "\n\n\t-f, --fullscreen");
                fprintf(stderr,
                        "\n\t\tSelects fullscreen display (default)");
                fprintf(stderr, "\n\n\t-w, --window");
                fprintf(stderr,
                        "\n\t\tSelects windowed display (not fullscreen)");
                fprintf(stderr, "\n\n\t-s, --sound");
                fprintf(stderr, "\n\t\tAllow in-game sounds (default)");
                fprintf(stderr, "\n\n\t-ns, --nosound");
                fprintf(stderr, "\n\t\tDisables in-game sounds");
                fprintf(stderr, "\n\n\t-t {THEME}, --theme {THEME}");
                fprintf(stderr,
                        "\n\t\tUse theme named {THEME}, if it exists");
                fprintf(stderr, "\n\n\t-sp, --speed");
                fprintf(stderr,
                        "\n\t\tSpeed up gameplay (for use on slower");
                fprintf(stderr, "\n\t\tmachines)");
                fprintf(stderr, "\n\n\t-a, --alpha");
                fprintf(stderr,
                        "\n\t\tUse transparencies to emphasize faster falling");
                fprintf(stderr, "\n\t\twords (will decrease performance)");
                fprintf(stderr, "\n\n\t-st, --static");
                fprintf(stderr,
                        "\n\t\tStatic (non-scrolling) setting (helps on systems");
                fprintf(stderr, "\n\t\twhere mouse disappears)");
                fprintf(stderr, "\n\n\t-v, --version");
                fprintf(stderr, "\n\t\tDisplay version number and exit");
                fprintf(stderr, "\n\n\t-vb, --verbose");
                fprintf(stderr,
                        "\n\t\tSelect verbose output (for debugging)");
                fprintf(stderr,
                        "\n\t\t(Warning! Verbose is /very/ verbose!)\n\n");
                exit(0);
            }
            if ((strcmp(argv[i], "-v") == 0) |
                (strcmp(argv[i], "--version") == 0)) {
                fprintf(stderr, "\n%s, Version %s\n", PACKAGE, VERSION);
                fprintf(stderr,
                        "Copyright (C) Sam Hart <hart@geekcomix.com>, under the GPL\n");
                fprintf(stderr,
                        "-See COPYING file for more info... Thx ;)\n\n");
                exit(0);
            }
            if ((strcmp(argv[i], "-f") == 0) |
                (strcmp(argv[i], "--fullscreen") == 0)) {
                video_flags = (SDL_FULLSCREEN | SDL_HWPALETTE); // | SDL_HWSURFACE | SDL_DOUBLEBUF);
            }
            if ((strcmp(argv[i], "-w") == 0) |
                (strcmp(argv[i], "--window") == 0)) {
                video_flags = (SDL_HWPALETTE);  // | SDL_DOUBLEBUF);
            }
            if ((strcmp(argv[i], "-sp") == 0) |
                (strcmp(argv[i], "--speed") == 0)) {
                speed_up = YES;
            }
            if ((strcmp(argv[i], "-st") == 0) |
                (strcmp(argv[i], "--static") == 0)) {
                scroll = NO;
            }
            if ((strcmp(argv[i], "-s") == 0) |
                (strcmp(argv[i], "--sound") == 0)) {
                lib_flags = (SDL_INIT_AUDIO | SDL_INIT_VIDEO);
                sys_sound = YES;
            }
            if ((strcmp(argv[i], "-ns") == 0) |
                (strcmp(argv[i], "--nosound") == 0)) {
                lib_flags = (SDL_INIT_VIDEO);
                sys_sound = NO;
            }
            if ((strcmp(argv[i], "-vb") == 0) |
                (strcmp(argv[i], "--verbose") == 0)) {
                verbose = YES;
            }
            if ((strcmp(argv[i], "--hidden") == 0) |
                (strcmp(argv[i], "-hidden") == 0)) {
                hidden = YES;
            }
            if ((strcmp(argv[i], "--alpha") == 0) |
                (strcmp(argv[i], "-a") == 0)) {
                use_alpha = YES;
            }
            if ((strcmp(argv[i], "-t") == 0) |
                (strcmp(argv[i], "--theme") == 0)) {
                use_theme = YES;
                sprintf(alt_theme_path, "%s", argv[++i]);
            }
        }
    }

    if (verbose == YES)
        fprintf(stderr, "\n%s, version %s BEGIN\n", PACKAGE, VERSION);

    //=*begin the game*/
    LibInit(lib_flags, verbose);
    GraphicsInit(video_flags, verbose);

    /*
     * Open the audio device if used 
     */
    if (sys_sound == YES) {
        if (Mix_OpenAudio(11025, AUDIO_U8, 1, 512) < 0) {
            fprintf(stderr,
                    "Warning: Couldn't set 11025 Hz 8-bit audio\n- Reason: %s\n",
                    SDL_GetError());
            sys_sound = NO;
        }
    }

    RunGame(verbose);
    FreeMem(verbose);

    if (verbose == YES)
        fprintf(stderr, "---GAME DONE, EXIT---- Thank you.\n");
    return EXIT_SUCCESS;
}
