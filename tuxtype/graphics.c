/***************************************************************************
                          graphics.c 
 -  description: graphics items
                             -------------------
    begin                : Thu May 4 2000
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
#include "graphics.h"
#include "funcs.h"

Mix_Chunk      *sound[NUM_WAVES];
Mix_Music      *music;

/*************************
	WaitFrame : Wait for
	the next frame.
**************************/
void WaitFrame(void)
{ //supposed to wait for next frame (not sure if it works, lifted from SDL demo aliens)
    static Uint32   next_tick = 0;
    Uint32          this_tick;

    /*
     * Wait for the next frame 
     */
    this_tick = SDL_GetTicks();
    if (this_tick < next_tick) {
        SDL_Delay(next_tick - this_tick);
    }

    next_tick = this_tick + (1000 / FRAMES_PER_SEC);
}

/***************************
	LoadSound : Load a
	sound/music patch from a
	file.
****************************/
Mix_Chunk      *LoadSound(char *datafile, int verbose)
{ //load a sound from datafile
    Mix_Chunk      *temp;
    char            filename[FNLEN];
    int             i = 0;
    char            DATAPATH[FNLEN];
    int             local_use_theme = YES;

    strcpy(DATAPATH, PATH[0]);

    if (use_theme == YES) {
        sprintf(filename, "%s/sounds/%s", alt_theme_path, datafile);
    } else {
        sprintf(filename, "%s/sounds/%s", DATAPATH, datafile);
    }

    if (verbose == YES)
        fprintf(stderr, "LOADSOUND: Trying %s\n", filename);

    temp = Mix_LoadWAV(filename);

    while (temp == NULL) {
        strcpy(DATAPATH, PATH[i]);
        if (use_theme == YES && local_use_theme == YES) {
            sprintf(filename, "%s/%s/sounds/%s", DATAPATH, alt_theme_path,
                    datafile);
        } else {
            sprintf(filename, "%s/sounds/%s", DATAPATH, datafile);
        }

        if (verbose == YES)
            fprintf(stderr, "LOADSOUND: Trying %s\n", filename);

        temp = Mix_LoadWAV(filename);
        i++;

        if (i >= PATHNUM && sys_sound == YES) {
            if (local_use_theme == YES && use_theme == YES) {
                i = 0;
                local_use_theme = NO;
            } else {
                fprintf(stderr, "Couldn't load %s: %s\n", filename,
                        Mix_GetError());
                exit(2);
            }
        }
    }

    if (verbose == YES)
        fprintf(stderr, "LOADSOUND: Done\n");

    return (temp);
}

/************************
	LoadMusic : Load
	music from a datafile
*************************/
void LoadMusic(char *datafile, int verbose)
{ //Load the music from datafile
    char            filename[FNLEN];
    int             i = 0;
    char            DATAPATH[FNLEN];
    int             local_use_theme = YES;

    strcpy(DATAPATH, PATH[0]);

    if (use_theme == YES) {
        sprintf(filename, "%s/sounds/%s", alt_theme_path, datafile);
    } else {
        sprintf(filename, "%s/sounds/%s", DATAPATH, datafile);
    }

    if (verbose == YES)
        fprintf(stderr, "LOADMUSIC: Trying %s\n", filename);

    music = Mix_LoadMUS(filename);

    while (music == NULL) {
        strcpy(DATAPATH, PATH[i]);

        if (use_theme == YES && local_use_theme == YES) {
            sprintf(filename, "%s/%s/sounds/%s", DATAPATH, alt_theme_path,
                    datafile);
        } else {
            sprintf(filename, "%s/sounds/%s", DATAPATH, datafile);
        }

        if (verbose == YES)
            fprintf(stderr, "LOADMUSIC: Trying %s\n", filename);

        music = Mix_LoadMUS(filename);
        i++;

        if (i >= PATHNUM && sys_sound == YES) {
            if (local_use_theme == YES && use_theme == YES) {
                i = 0;
                local_use_theme = NO;
            } else {
                fprintf(stderr, "Couldn't load %s: %s\n", filename,
                        Mix_GetError());
                exit(2);
            }
        }
    }


    if (verbose == YES)
        fprintf(stderr, "LOADMUSIC: Done\n");
}

/***********************
	LoadImage : Load an
	image and set
	transparent if
	requested
************************/
SDL_Surface    *LoadImage(char *datafile, int transparent, int verbose)
{ //Load an image and set transparent (note, must use rgb = 255, 255, 0 as transparent color)
    SDL_Surface    *pic,
                   *pic2;
    char            filename[FNLEN];
    int             i = 0;
    char            DATAPATH[FNLEN];
    int             local_use_theme = YES;

    /*verbose = YES; for forced debugging ;-)*/

    strcpy(DATAPATH, PATH[0]);

    if (use_theme == YES) {
        sprintf(filename, "%s/images/%s", alt_theme_path, datafile);
    } else {
        sprintf(filename, "%s/images/%s", DATAPATH, datafile);
    }

    if (verbose == YES)
        fprintf(stderr, "LOADIMAGE: Trying %s\n", filename);

    pic = IMG_Load(filename);

    if (verbose == YES)
        fprintf(stderr, "LOADIMAGE: testing for existance\n");

    while (pic == NULL) {
        strcpy(DATAPATH, PATH[i]);

        if (use_theme == YES && local_use_theme == YES) {
            sprintf(filename, "%s/%s/images/%s", DATAPATH, alt_theme_path,
                    datafile);
        } else {
            sprintf(filename, "%s/images/%s", DATAPATH, datafile);
        }

        if (verbose == YES)
            fprintf(stderr, "LOADIMAGE: Trying %s\n", filename);

        pic = IMG_Load(filename);
        i++;

        if (i >= PATHNUM) {
            if (local_use_theme == YES && use_theme == YES) {
                i = 0;
                local_use_theme = NO;
            } else {
                fprintf(stderr, "Couldn't load %s: %s\n", filename,
                        Mix_GetError());
                exit(2);
            }
        }
    }

    if (transparent) {
        ///* Assuming 8-bit BMP image */
        //SDL_SetColorKey(pic, (SDL_SRCCOLORKEY|SDL_RLEACCEL),
        //                              *(Uint8 *)pic->pixels);
        SDL_SetColorKey(pic, (SDL_SRCCOLORKEY | SDL_RLEACCEL),
                        SDL_MapRGB(pic->format, 255, 255, 0));
        SDL_SetColorKey(pic, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(pic->format, 254, 254, 4));
        //kludge because gimp is giving me headaches! perhaps figue out what the problem is and fix this later
        /* Note, 2001-07-12 : S. Hart: We can easily fix this by changing all the images to alpha images
           and convert them to alpha display with SDL_DisplayFormatAlpha(pic) bellow */
    }
    pic2 = SDL_DisplayFormat(pic);
    SDL_FreeSurface(pic);

    if (verbose == YES)
        fprintf(stderr, "LOADIMAGE: Done\n");

    return (pic2);
}

/*********************
	StandbyScreen:
	Display the Standby
	screen.... duh
***********************/
void StandbyScreen()
{ //what to do when standing by ;)
    SDL_Surface    *standby;
    SDL_Rect        dest;
    char            filename[FNLEN];
    int             reset_theme = NO;

    strcpy(filename, "standby.png");

    reset_theme = use_theme;
    use_theme = NO; /* Sorry, I want the digital monkey logo not to be themed ;-) */
    standby = LoadImage(filename, 0, NO);
    use_theme = reset_theme;

    dest.x = ((screen->w) / 2) - (standby->w) / 2;
    dest.y = ((screen->h) / 2) - (standby->h) / 2;
    dest.w = standby->w;
    dest.h = standby->h;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(standby, NULL, screen, &dest);
    SDL_UpdateRect(screen, 0, 0, 0, 0);

    /*
     * Wait a while- give screen chance to come up (if fullscreen) 
     */
    SDL_Delay(WAIT_MS);

    SDL_FreeSurface(standby);

}

/*************************
	FreeGraphics: Free all
	the allocated graphics
**************************/
void FreeGraphics(int verbose)
{ // free allocd graphics
    if (verbose == YES) {
        fprintf(stderr, "FreeGraphics():\n");
        fprintf(stderr, "-Freeing screen\n");
    }
    SDL_FreeSurface(screen);
    if (verbose == YES)
        fprintf(stderr, "-Screen freed\n");

    if (verbose == YES) {
        fprintf(stderr, "FreeGraphics():END\n");
    }
}

/*************************
	TitleScreen: Display
	the title screen
**************************/
int TitleScreen(int verbose)
{ // display title screen, get input
    SDL_Surface    *background;
    SDL_Surface    *temp,
                   *temp2;
    SDL_Surface    *title,
                   *cursor;
    SDL_Surface    *Tux[MAX_TUX_ANIM_FRAMES + 1];
    SDL_Surface    *menu[TITLE_MENU_ITEMS + 1][TITLE_MENU_DEPTH + 1];
    SDL_Rect        dest,
                    destd,
                    tempdest,
                    CursRect;
    SDL_Rect        Tuxdest,
                    Titledest;
    SDL_Rect        menudest[TITLE_MENU_ITEMS + 1];
    SDL_Rect        orig_menudest[TITLE_MENU_ITEMS + 1];
    char            filename[FNLEN];
    int             back_w,
                    back_h;
    int             i,
                    j,
                    tux_frame;
    int             x = 0;
    int             y = 0;
    int             dirx = 1;
    int             diry = 1;
    int             done = NO;
    int             menu_opt = MAIN;
    int             sub_menu = DONE;
    int             menu_depth = 1;
    int             last = 0;
    int             key_menu = 5;
    int             old_key_menu = 0;
    Uint8           state;
    Uint8          *keys;

    //setup menu items
    menu_item[1][1] = KCASCADE;
    menu_item[2][1] = WCASCADE;
    menu_item[3][1] = PRACTICE;
    menu_item[4][1] = DONE;

    menu_item[1][2] = LEVEL1;
    menu_item[2][2] = LEVEL2;
    menu_item[3][2] = LEVEL3;
    menu_item[4][2] = MAIN;

    menu_item[1][3] = PKCASCADE;
    menu_item[2][3] = PWCASCADE;
    menu_item[3][3] = PFREETYPE;
    menu_item[4][3] = MAIN;
    //done setup menu items (like to move in globals eventually)

    if (verbose == YES) {
        fprintf(stderr, "->TitleScreen():\n");
        fprintf(stderr, "->>Loading graphics\n");
    }
    //LOAD MAIN TITLE SCREEN IMAGES
    if (verbose == YES)
        fprintf(stderr, "->>checking for images\n");

    //strcpy(filename, "title1.png");
    sprintf(filename, "title1.png");

    title = LoadImage(filename, 1, verbose);

    if (title == NULL) {
        fprintf(stderr, "!!Error Allocating Image Files!\n");
        exit(2);
    }

    if (verbose == YES) {
        fprintf(stderr, "->>Images found\n");
        fprintf(stderr, "->>Loading bkg image\n");
    }
    //strcpy(filename, "main_bkg.png");
    sprintf(filename, "main_bkg.png");
    temp = LoadImage(filename, 0, verbose);
    if (temp == NULL) {
        fprintf(stderr, "!!Error Allocating Image Files!\n");
        exit(2);
    }

    for (i = 1; i <= MAX_TUX_ANIM_FRAMES; i++) {
        //load tux animation
        if (verbose == YES)
            fprintf(stderr, "->>Loading tux anim frame %i\n", i);
        sprintf(filename, "tux%i.png", i);
        if (verbose == YES)
            fprintf(stderr, "->>Loading tux anim frame %s\n", filename);
        Tux[i] = LoadImage(filename, 1, verbose);
        if (Tux[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    for (i = 1; i <= TITLE_MENU_ITEMS; i++)
        for (j = 1; j <= TITLE_MENU_DEPTH; j++) {
            //load menu items
            if (verbose == YES)
                fprintf(stderr, "->>Loading menu item %ix%i\n", j, i);
            sprintf(filename, "menu%i_%i.png", j, i);
            if (verbose == YES)
                fprintf(stderr, "->>Loading menu item %s\n", filename);
            menu[i][j] = LoadImage(filename, 1, verbose);
            if (menu[i][j] == NULL) {
                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
        }

    if (verbose == YES)
        fprintf(stderr, "->>Loading cursor\n");

    if (hidden == YES)
        sprintf(filename, "cursorh.png");
    else
        sprintf(filename, "cursorn.png");

    cursor = LoadImage(filename, 1, verbose);
    if (cursor == NULL) {
        fprintf(stderr, "!!Error Loading Cursor file!\n");
        exit(2);
    }
    CursRect.w = cursor->w;
    CursRect.h = cursor->h;

    if (sys_sound == YES) {
        if (verbose == YES)
            fprintf(stderr, "->>Loading title music\n");

        if (hidden == YES)
            sprintf(filename, "funkbrk.it");
        else
            sprintf(filename, "funkbrk.it");

        LoadMusic(filename, verbose);
    }

    if (verbose == YES)
        fprintf(stderr, "->>Title images loaded, setting up background\n");

    //DONE LOADING TITLE IMAGES

    //SETUP TITLE SCREEN BACKGROUND
    back_w = temp->w * BACK_MULT + 1;
    back_h = temp->h * BACK_MULT + 1;
    temp2 = SDL_CreateRGBSurface(0, back_w, back_h, BACK_BPP, 0, 0, 0, 0);
    background = SDL_ConvertSurface(temp2, screen->format, 0);
    SDL_FreeSurface(temp2);

    for (x = 0; x <= background->w; x += temp->w) {
        for (y = 0; y <= background->h; y += temp->h) {
            dest.x = x;
            dest.y = y;
            if ((dest.x + temp->w) >= (background->w))
                dest.w = (background->w) - dest.x - 1;
            else
                dest.w = temp->w;

            if ((dest.y + temp->h) >= (background->h))
                dest.h = (background->w) - dest.y - 1;
            else
                dest.h = temp->h;

            SDL_BlitSurface(temp, NULL, background, &dest);
        }
    }

    SDL_FreeSurface(temp);

    // I don't recall why this was here... but it places
    // an unfortunate delay at the title screen
    // just commenting it out because I /may/ have stuck this
    // in for a very valid reason!

    /*
     * Wait a while- give screen chance to come up (if fullscreen) 
     */
    //SDL_Delay(WAIT_MS);

    if (verbose == YES)
        fprintf(stderr,
                "->Background set up successfully, starting init anim\n");

    dest.x = 0;
    dest.y = 0;
    dest.w = screen->w;
    dest.h = screen->h;

    destd = dest;

    Tuxdest.x = 0;
    Tuxdest.y = screen->h - (Tux[1]->h / PRE_ANIM_FRAMES);
    Tuxdest.w = Tux[1]->w - (Tux[1]->w / PRE_ANIM_FRAMES);
    Tuxdest.h = Tux[1]->h / PRE_ANIM_FRAMES;



    Titledest.x = screen->w - ((TITLE_MULT * title->w) / PRE_ANIM_FRAMES);
    Titledest.y = (title->h) / 2;
    Titledest.w = (TITLE_MULT * title->w) / PRE_ANIM_FRAMES;
    Titledest.h = title->h;

    menudest[0].y = Titledest.y + (Titledest.h) / 2 + MENU_SEP;

    for (i = 1; i <= TITLE_MENU_ITEMS; i++) {
        menudest[i].x = ((screen->w) / 2) - (menu[i][1]->w) / 2;
        menudest[i].y = menudest[i - 1].y + MENU_SEP + menu[i][1]->h;
        menudest[i].w = menu[i][1]->w;
        menudest[i].h = menu[i][1]->h;
        orig_menudest[i] = menudest[i];
    }

    i = 0 - PRE_ANIM_FRAMES * PRE_FRAME_MULT;   // for first animation

    //for some reason, doesn't place mouse cursor where I want it
    //in full-screen.... but goofs SDL up enough to allow me to poll
    //the mouse's init position at 0,0.... which works fine to prevent

    //inital jerkiness of whatever menu item is at or near center of
    //screen!! Thus, fulfills its poip-pus! ;)
    SDL_WarpMouse(0, 0);

    //no cursor please
    SDL_ShowCursor(0);

    if (sys_sound == YES) {
        //Fade in music
        Mix_FadeInMusic(music, 1, FADE_IN_TITLE_MUSIC);
    }

    do {
        SDL_BlitSurface(background, &dest, screen, &destd);
        i++;
        if (i == 0 && verbose == YES)
            fprintf(stderr, "->Begin Tux Anim Loop\n");

        if (scroll != NO) {
            dest.x += XSTEP * dirx;
            dest.y += YSTEP * diry;

            if (dest.x <= 0) {
                dest.x = 0;
                dirx *= -1;
            } else if (dest.x >= ((background->w) - (screen->w))) {
                dest.x = (background->w) - (screen->w);
                dirx *= -1;
            }

            if (dest.y <= 0) {
                dest.y = 0;
                diry *= -1;
            } else if (dest.y >= ((background->h) - (screen->h))) {
                dest.y = (background->h) - (screen->h);
                diry *= -1;
            }
        }

        if (sys_sound == YES) {
            //play the title music
            if (!Mix_PlayingMusic()) {
                Mix_PlayMusic(music, 1);
            }
        }
        //This PollEvent section was placed here largely to clean out
        //the event list between menu level changes (preventing unwanted double clicks)
        if (SDL_PollEvent(&event) > 0) {
            switch (event.type) {
            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        /*
                         * This quit event signals the closing of the window 
                         */
                        if (verbose == YES)
                            fprintf(stderr,
                                    "->>Escape key rec'd from WM\n");
                        menu_opt = DONE;
                        done = YES;
                        i = FULL_CIRCLE + 1;
                    }
                    break;
                }
            case SDL_QUIT:
                {               /* This quit event signals the closing of the window */
                    if (verbose == YES)
                        fprintf(stderr, "->>Signal Quit rec'd from WM\n");
                    menu_opt = DONE;
                    done = YES;
                    i = FULL_CIRCLE + 1;
                    break;
                }
            case SDL_ACTIVEEVENT:
                {               /* For future usage */
                    if (verbose == YES)
                        fprintf(stderr, "->>App Focus has changed\n");
                }
            }
        }

        keys = SDL_GetKeyState(NULL);

        old_key_menu = key_menu;

        if (keys[SDLK_UP] == SDL_PRESSED) {
            if (last != 1) {
                key_menu--;
                last = 1;
            }
        } else if (keys[SDLK_DOWN] == SDL_PRESSED) {
            if (last != 2) {
                key_menu++;
                last = 2;
            }
        } else
            last = 0;

        if (key_menu < 1)
            key_menu = 4;
        if (key_menu > 4)
            key_menu = 1;

        if (old_key_menu != key_menu)
            SDL_WarpMouse(orig_menudest[key_menu].x +
                          (menudest[key_menu].w / 2),
                          orig_menudest[key_menu].y +
                          (2 * menudest[key_menu].h / 3));

        if (keys[SDLK_RETURN] == SDL_PRESSED) {
            while (keys[SDLK_RETURN] == SDL_PRESSED) {
                SDL_PollEvent(&event);
                keys = SDL_GetKeyState(NULL);
            }

            state = SDL_GetMouseState(&x, &y);
            if ((x >= orig_menudest[key_menu].x) &&
                (x <= (orig_menudest[key_menu].x + menudest[key_menu].w))
                && (y >= orig_menudest[key_menu].y)
                && (y <=
                    (orig_menudest[key_menu].y + menudest[key_menu].h))) {
                menu_opt = menu_item[key_menu][menu_depth];
                fprintf(stderr, "");    //WHAT THE ? This fixes segfault
                key_menu = 5;   // intential to cause update..
                old_key_menu = 0;
                i = FULL_CIRCLE + 1;
            }
        }


        if (i <= 0) {
            //do animations
            if (verbose == YES)
                fprintf(stderr, "->Doing beginning animations\n");

            tempdest.x = 0;
            tempdest.y = 0;
            tempdest.w = Tuxdest.w;
            tempdest.h = Tuxdest.h;
            SDL_BlitSurface(Tux[1], &tempdest, screen, &Tuxdest);
            tempdest.w = Titledest.w;
            tempdest.h = Titledest.h;
            SDL_BlitSurface(title, &tempdest, screen, &Titledest);

            Tuxdest.y -= Tux[1]->h / (PRE_ANIM_FRAMES * PRE_FRAME_MULT);
            Tuxdest.w += Tux[1]->w / (PRE_ANIM_FRAMES * PRE_FRAME_MULT);
            Tuxdest.h += Tux[1]->h / (PRE_ANIM_FRAMES * PRE_FRAME_MULT);

            if (Tuxdest.w >= Tux[1]->w || Tuxdest.h >= Tux[1]->h) {
                Tuxdest.w = Tux[1]->w;
                Tuxdest.h = Tux[1]->h;
            }

            Titledest.x -=
                (TITLE_MULT * title->w) / (PRE_ANIM_FRAMES *
                                           PRE_FRAME_MULT);
            Titledest.w +=
                (TITLE_MULT * title->w) / (PRE_ANIM_FRAMES *
                                           PRE_FRAME_MULT);
            if (Titledest.w >= title->w)
                Titledest.w = title->w;

        } else if (i <= FULL_CIRCLE) {
            if (i >= FULL_CIRCLE)
                i = 1;
            Tuxdest.y = screen->h - Tux[1]->h;
            done = NO;

            //ANIMATE TUX
            if (i >= TUX1 && i <= TUX2)
                tux_frame = 2;
            else if (i > TUX2 && i <= TUX3)
                tux_frame = 3;
            else if (i > TUX3 && i <= TUX4)
                tux_frame = 4;
            else if (i > TUX4 && i <= TUX5)
                tux_frame = 3;
            else if (i > TUX5 && i <= TUX6)
                tux_frame = 2;
            else
                tux_frame = 1;
            SDL_BlitSurface(Tux[tux_frame], NULL, screen, &Tuxdest);

            //place title
            SDL_BlitSurface(title, NULL, screen, &Titledest);

            state = SDL_GetMouseState(&x, &y);
            for (j = 1; j <= TITLE_MENU_ITEMS; j++) {
                //check to see if cursor inside for animation
                if ((x >= orig_menudest[j].x
                     && x <= (orig_menudest[j].x + menudest[j].w))
                    && (y >= orig_menudest[j].y
                        && y <= (orig_menudest[j].y + menudest[j].h))) {
                    key_menu = j;
                    menudest[j].x =
                        orig_menudest[j].x +
                        (MENU_GYRO_AMP * sin(i * MENU_GYRO_MULT));
                    menudest[j].y =
                        orig_menudest[j].y +
                        (MENU_GYRO_AMP * cos(i * MENU_GYRO_MULT));
                } else {
                    menudest[j].x = orig_menudest[j].x;
                    menudest[j].y = orig_menudest[j].y;
                }
                tempdest = menudest[j];
                tempdest.h = menu[j][menu_depth]->h;
                tempdest.w = menu[j][menu_depth]->w;
                SDL_BlitSurface(menu[j][menu_depth], NULL, screen,
                                &tempdest);
            }

            //check for mouse buttone depression
            if (state == SDL_BUTTON(1) || state == SDL_BUTTON(2)
                || state == SDL_BUTTON(3)) {    // ehh, we're easy... whatever mouse button you wanna through at me is fine
                for (j = 1; j <= TITLE_MENU_ITEMS; j++) {
                    if ((x >= orig_menudest[j].x
                         && x <= (orig_menudest[j].x + menudest[j].w))
                        && (y >= orig_menudest[j].y
                            && y <=
                            (orig_menudest[j].y + menudest[j].h))) {
                        menu_opt = menu_item[j][menu_depth];
                        i = FULL_CIRCLE + 1;
                        if (verbose == YES)
                            fprintf(stderr, "->>BUTTON CLICK\n");
                    }
                }
            }
        } else {
            if (verbose == YES)
                fprintf(stderr, "MENU SELECTED, PROCESSING\n");
            if (i >= FULL_CIRCLE + SLING_FRAMES) {
                i = 1;
                if (verbose == YES)
                    fprintf(stderr, "PROCESSING END!\n");
            }
            if (menu_opt == DONE) {
                done = YES;
            } else if (menu_opt == PRACTICE) {
                menu_depth = PRACTICE_SUBMENU;
                if (verbose == YES)
                    fprintf(stderr, "PRACTICE SUBMENU\n");
            } else if (menu_opt == KCASCADE) {
                menu_depth = KCASCADE_SUBMENU;
                sub_menu = KCASCADE;
                if (verbose == YES)
                    fprintf(stderr, "KCASCADE SUBMENU\n");
            } else if (menu_opt == WCASCADE) {
                menu_depth = WCASCADE_SUBMENU;
                sub_menu = WCASCADE;
                if (verbose == YES)
                    fprintf(stderr, "WCASCADE SUBMENU\n");
            } else if (menu_opt == MAIN) {
                menu_depth = ROOTMENU;
                if (verbose == YES)
                    fprintf(stderr, "ROOT MENU\n");
            } else if (menu_opt == LEVEL1) {
                done = YES;
                if (sub_menu == KCASCADE) {
                    menu_opt = KCASCADE1;
                } else {
                    menu_opt = WCASCADE1;
                }
            } else if (menu_opt == LEVEL2) {
                done = YES;
                if (sub_menu == KCASCADE) {
                    menu_opt = KCASCADE2;
                } else {
                    menu_opt = WCASCADE2;
                }
            } else if (menu_opt == LEVEL3) {
                done = YES;
                if (sub_menu == KCASCADE) {
                    menu_opt = KCASCADE3;
                } else {
                    menu_opt = WCASCADE3;
                }
            } else if (menu_opt == PKCASCADE) {
                done = YES;
            } else if (menu_opt == PFREETYPE) {
                done = YES;
            } else if (menu_opt == PWCASCADE) {
                done = YES;
            }

            menudest[0].y = Titledest.y + (Titledest.h) / 2 + MENU_SEP;
            for (j = 1; j <= TITLE_MENU_ITEMS; j++) {
                menudest[j].x =
                    ((screen->w) / 2) - (menu[j][menu_depth]->w) / 2;
                menudest[j].y =
                    menudest[j - 1].y + MENU_SEP + menu[j][menu_depth]->h;
                menudest[j].w = menu[j][menu_depth]->w;
                menudest[j].h = menu[j][menu_depth]->h;
                orig_menudest[j] = menudest[j];
            }

            //place tux                   
            SDL_BlitSurface(Tux[tux_frame], NULL, screen, &Tuxdest);

            //place title
            SDL_BlitSurface(title, NULL, screen, &Titledest);
        }

        //PLACE CURSOR
        if (x < 0)
            x = 0;
        if (x > (screen->w) - (cursor->w) - 1)
            x = (screen->w) - (cursor->w) - 1;

        if (y < 0)
            y = 0;
        if (y > (screen->h) - (cursor->h) - 1)
            y = (screen->h) - (cursor->h) - 1;

        CursRect.x = x;
        CursRect.y = y;
        SDL_BlitSurface(cursor, NULL, screen, &CursRect);
        //SDL_Flip(screen);
        if (speed_up == NO)
            WaitFrame();

        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
    while (done == NO);

    if (verbose == YES)
        fprintf(stderr, "->>Freeing title screen images\n");

    SDL_FreeSurface(background);
    SDL_FreeSurface(title);

    for (i = 1; i <= MAX_TUX_ANIM_FRAMES; i++) {
        SDL_FreeSurface(Tux[i]);
    }

    for (i = 1; i <= TITLE_MENU_ITEMS; i++)
        for (j = 1; j <= TITLE_MENU_DEPTH; j++) {
            SDL_FreeSurface(menu[i][j]);
        }

    SDL_FreeSurface(cursor);

    if (sys_sound == YES) {
        //stop the music
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }

    if (verbose == YES)
        fprintf(stderr, "->TitleScreen():END\n");
    return menu_opt;
}                               //note to myself: font size in menu used was 38 pixels, GIMP logo "Comic Book", using "Blue_Green" gradient

/***************************
	GraphicsInit: Initializes
	the graphic system
****************************/
void GraphicsInit(Uint32 video_flags, int verbose)
{
    /*
     * Open the display device 
     */
    if (verbose == YES) {
        fprintf(stderr, "GraphicsInit():\n");
        fprintf(stderr, "-SDL Setting VidMode to %ix%ix%i\n", RES_X, RES_Y,
                BPP);
    }

    screen = SDL_SetVideoMode(RES_X, RES_Y, BPP, video_flags);
    if (screen == NULL) {
        fprintf(stderr, "Couldn't set %ix%i video mode: %s\n", RES_X,
                RES_Y, SDL_GetError());
        exit(2);
    }

    /*
     * Let's set the appropriate clip rect 
     */
    SDL_SetClipRect(screen, NULL);

    /*
     * no cursor please 
     */
    SDL_ShowCursor(0);

    StandbyScreen();

    /*
     * Set window manager stuff: 
     */
    SDL_WM_SetCaption("Tux Typing", PACKAGE);

    if (verbose == YES) {
        fprintf(stderr, "-SDL VidMode successfully set to %ix%ix%i\n",
                RES_X, RES_Y, BPP);
        fprintf(stderr, "GraphicsInit():END\n");
    }
}

/****************************
	LibInit : Init the SDL
	library
*****************************/
void LibInit(Uint32 lib_flags, int verbose)
{
    /*
     * Inizialize the SDL library 
     */
    if (verbose == YES) {
        fprintf(stderr, "LibInit():\n");
        fprintf(stderr, "-About to init SDL Library\n");
    }

    if (SDL_Init(lib_flags) < 0) {
        // First let's try without sound if sound is set
        if (sys_sound == YES) {
            lib_flags = (SDL_INIT_VIDEO);
            sys_sound = NO;
        }
        if (SDL_Init(lib_flags) < 0) {
            fprintf(stderr, "Couldn't initialize SDL: %s\n",
                    SDL_GetError());
            exit(2);
        }
    }

    if (verbose == YES)
        fprintf(stderr, "-SDL Library init'd successfully\n");

    /*
     * fire and forget... 
     */
    atexit(SDL_Quit);

    if (verbose == YES) {
        fprintf(stderr, "-SDL_Quit updated\n");
        fprintf(stderr, "LibInit():END\n");
    }
    SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
}
