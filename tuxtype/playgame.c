/***************************************************************************
                          playgame.c 
 -  description: verious game play types
                             -------------------
    begin                : Fri May 5 2000
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "globals.h"
#include "funcs.h"
#include "playgame.h"

Mix_Chunk      *sound[NUM_WAVES];
Mix_Music      *music;

object          fishy[FISHY_FRAMES];
object          splat[SPLAT_FRAMES];
//object rocky[ROCKY_FRAMES]; // May wish to use this in future... dunno
// The concept was kewl... add falling rocks with letters on them... if
//player types these letters, tux eats the rocks, and it counts against
//him.... Nice concept... hard to impliment (does he die? do I need
//hit-points? how would I do this) left in cuz still kewl idea ;)
rl_object       tuxStand[STAND_FRAMES];
rl_object       tuxWalk[WALK_FRAMES];
rl_object       tuxRun[RUN_FRAMES];
rl_object       tuxGulp[GULP_FRAMES];
rl_object       tuxWin[WIN_FRAMES];
rl_object       tuxYipe[YIPE_FRAMES];

SDL_Surface    *level[NUM_LEVELS];
SDL_Surface    *curlev;
SDL_Surface    *number[NUM_NUMS];
int             number_max_w;
SDL_Surface    *letter[NUM_LETTERS];
SDL_Surface    *lives;
SDL_Surface    *fish;
SDL_Surface    *congrats[CONGRATS_FRAMES];
SDL_Surface    *ohno[OH_NO_FRAMES];
SDL_Surface    *pause_img[PAUSE_IMAGES];
int             total_pause_h;

SDL_Surface    *background;

/* Special alpha blitting versions of sprites */
SDL_Surface     *letter_alpha[NUM_LETTERS];
object          fishy_alpha[FISHY_FRAMES];

int             numupdates = 0;
//int oldnumupdates = 0;
SDL_Rect        srcupdate[MAX_UPDATES];
SDL_Rect        dstupdate[MAX_UPDATES];
//SDL_Rect olddstupdate[MAX_UPDATES];
struct blit {
    SDL_Surface    *src;
    SDL_Rect       *srcrect;
    SDL_Rect       *dstrect;
    char            type;
} blits[MAX_UPDATES];

int             tux_max_width;

/*************************
  ClearObjects : Clear and
  reset all objects to
  dead
*************************/
void ClearObjects(int verbose)
{
    //clear and reset all objects to dead ('cept tux... who is immortal ;)
    int             i;

    if (verbose == YES)
        fprintf(stderr, "CLEARING OBJECTS\n");

    for (i = 0; i < MAX_FISHIES_HARD + 1; i++) {
        fish_object[i] = null_fishy;
        splat_object[i] = null_splat;
        word_object[i] = null_word;
    }

    tux_object.facing = RIGHT;
    tux_object.x = tuxStand[0].image[RIGHT]->w / 2;
    tux_object.y = screen->h - tuxStand[0].image[RIGHT]->h - 1;
    tux_object.dx = 0;
    tux_object.dy = 0;
    tux_object.endx = tux_object.x;
    tux_object.endy = tux_object.y;
    tux_object.frametype = STANDING;
    tux_object.curframe = 0;
    tux_object.endframe = tuxStand[0].frames;
    tux_object.letter = NOT_A_LETTER;

    if (verbose == YES)
        fprintf(stderr, "OBJECTS CLEARED\n");
}

/**********************
  DrawObject : Draw an
  object at the specified
  location. No respect
  to clipping!
*************************/
void DrawObject(SDL_Surface * sprite, int x, int y)
{
    //borrowed from SL's alien (and modified)
    struct blit    *update;

    update = &blits[numupdates++];
    update->src = sprite;
    update->srcrect->x = 0;
    update->srcrect->y = 0;
    update->srcrect->w = sprite->w;
    update->srcrect->h = sprite->h;
    update->dstrect->x = x;
    update->dstrect->y = y;
    update->dstrect->w = sprite->w;
    update->dstrect->h = sprite->h;
    update->type = 'D';
}

/************************
  UpdateScreen : Update
  the screen and increment
  the frame num
***************************/
void UpdateScreen(int *frame)
{
    //borrowed from SL's alien
    int             i;

    for (i = 0; i < numupdates; i++)
        if (blits[i].type == 'E')
            SDL_LowerBlit(blits[i].src, blits[i].srcrect,
                          screen, blits[i].dstrect);

    for (i = 0; i < numupdates; i++)
        if (blits[i].type == 'D')
            SDL_BlitSurface(blits[i].src, blits[i].srcrect,
                            screen, blits[i].dstrect);

    for (i = 0; i < numupdates; i++)
        if (blits[i].type == 'R')
            SDL_LowerBlit(blits[i].src, blits[i].srcrect,
                          screen, blits[i].dstrect);

    SDL_UpdateRects(screen, numupdates, dstupdate);

    numupdates = 0;
    *frame = *frame + 1;        //next frame
    if (*frame > FRAME_MAX)
        *frame = 0;
}

/*************************
  RealEraseObject : This really erases an
  object from the screen after drawing is done...
**************************/
void RealEraseObject(SDL_Surface * sprite, int x, int y)
{
    //borrowed from SL's alien
    struct blit    *update;

    update = &blits[numupdates++];
    update->src = background;
    update->srcrect->x = x;
    update->srcrect->y = y;
    update->srcrect->w = sprite->w;
    update->srcrect->h = sprite->h;
    update->dstrect->x = x;
    update->dstrect->y = y;
    update->dstrect->w = sprite->w;
    update->dstrect->h = sprite->h;
    update->type = 'R';
}

/*************************
  EraseObject : Erase an
  object from the screen
**************************/
void EraseObject(SDL_Surface * sprite, int x, int y)
{
    //borrowed from SL's alien
    struct blit    *update;

    update = &blits[numupdates++];
    update->src = background;
    update->srcrect->x = x;
    update->srcrect->y = y;
    update->srcrect->w = sprite->w;
    update->srcrect->h = sprite->h;
    update->dstrect->x = x;
    update->dstrect->y = y;
    update->dstrect->w = sprite->w;
    update->dstrect->h = sprite->h;
    update->type = 'E';
}

/*********************
  LoadOthers : Load all
  other graphics
**********************/
void LoadOthers(int verbose)
{
    //load all other graphics
    int             i,
                    max_w = 0;
    char            filename[FNLEN];

    if (verbose == YES)
        fprintf(stderr, "=LoadOthers()\n");

    sprintf(filename, "level.png");
    curlev = LoadImage(filename, 1, verbose);
    if (curlev == NULL) {
        fprintf(stderr, "!!Error Allocating Image Files!\n");
        exit(2);
    }
    sprintf(filename, "lives.png");
    lives = LoadImage(filename, 1, verbose);
    if (lives == NULL) {
        fprintf(stderr, "!!Error Allocating Image Files!\n");
        exit(2);
    }
    sprintf(filename, "fish.png");
    fish = LoadImage(filename, 1, verbose);
    if (fish == NULL) {
        fprintf(stderr, "!!Error Allocating Image Files!\n");
        exit(2);
    }

    for (i = 0; i < NUM_LEVELS; i++) {
        sprintf(filename, "gm_%i.png", i + 1);
        level[i] = LoadImage(filename, 1, verbose);
        if (level[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    for (i = 0; i < NUM_NUMS; i++) {
        sprintf(filename, "num_%i.png", i);
        number[i] = LoadImage(filename, 1, verbose);
        if (number[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
        if (number[i]->w > max_w)
            max_w = number[i]->w;
    }
    number_max_w = max_w;

    for (i = 0; i < NUM_LETTERS; i++) {
        sprintf(filename, "let_%i.png", i + 1);
        letter[i] = LoadImage(filename, 1, verbose);
        letter_alpha[i] = LoadImage(filename, 1, verbose);
        SDL_SetAlpha(letter_alpha[i], (SDL_SRCALPHA | SDL_RLEACCEL), DEFAULT_ALPHA);
        if (letter[i] == NULL || letter_alpha[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    total_pause_h = 0;

    for (i = 0; i < PAUSE_IMAGES; i++) {
        sprintf(filename, "pause%i.png", i + 1);
        pause_img[i] = LoadImage(filename, 1, verbose);
        if (pause_img[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
        total_pause_h += pause_img[i]->h;
    }

    for (i = 0; i < CONGRATS_FRAMES; i++) {
        sprintf(filename, "congrats%i.png", i + 1);
        congrats[i] = LoadImage(filename, 1, verbose);
        if (congrats[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    for (i = 0; i < OH_NO_FRAMES; i++) {
        sprintf(filename, "ohno%i.png", i + 1);
        ohno[i] = LoadImage(filename, 1, verbose);
        if (congrats[i] == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    if (sys_sound == YES) {
        if (verbose == YES)
            fprintf(stderr, "=Loading Sound FX\n");

        sprintf(filename, "win.wav");
        sound[WIN_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "winfinal.wav");
        sound[WINFINAL_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "bite.wav");
        sound[BITE_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "lose.wav");
        sound[LOSE_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "run.wav");
        sound[RUN_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "splat.wav");
        sound[SPLAT_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "pause.wav");
        sound[PAUSE_WAV] = LoadSound(filename, verbose);

        sprintf(filename, "excuseme.wav");
        sound[EXCUSEME_WAV] = LoadSound(filename, verbose);

        if (verbose == YES)
            fprintf(stderr, "=Done Loading Sound FX\n");
    } else if (verbose == YES)
        fprintf(stderr, "=NO SOUND FX LOADED (not selected)\n");

    if(verbose == YES)
        fprintf(stderr, "=Setting NULL fish & splat & word\n");
    null_fishy.alive = NO;
    null_fishy.endframe = 0;
    null_fishy.curframe = 0;
    null_fishy.letter = 0;
    null_fishy.x = 0;
    null_fishy.y = 0;
    null_fishy.x0 = 0;
    null_fishy.dy = 0;
    null_fishy.x_amplitude = 0;
    null_fishy.x_phase = 0;
    null_fishy.x_angle_mult = 0;

    null_word.alive = NO;
    null_word.endframe = 0;
    null_word.curframe = 0;
    null_word.x = 0;
    null_word.y = 0;
    null_word.x0 = 0;
    null_word.dy = 0;
    null_word.x_amplitude = 0;
    null_word.x_phase = 0;
    null_word.x_angle_mult = 0;

    null_splat.x = 0;
    null_splat.y = 0;
    null_splat.endframe = 0;
    null_splat.curframe = 0;
    null_splat.alive = NO;

    if (verbose == YES)
        fprintf(stderr, "=LoadOthers() END\n");

}

/***************************
  LoadFishies : Load the fish
  animations and graphics
*****************************/
void LoadFishies(int verbose)
{
    //load the fishy animation
    int             i;
    char            filename[FNLEN];

    if (verbose == YES)
        fprintf(stderr, "=LoadFishies()\n");

    for (i = 0; i < FISHY_FRAMES; i++) {
        if (verbose == YES)
            fprintf(stderr, "=>Loading FISHY anim frame %i\n", i);
        sprintf(filename, "fishy%i.png", (i + 1));
        if (verbose == YES)
            fprintf(stderr, "=>Loading FISHY anim frame %s\n", filename);
        fishy[i].image = LoadImage(filename, 1, verbose);
        fishy_alpha[i].image = LoadImage(filename, 1, verbose);
        fishy[i].frames = FISHY_ONSCREEN;
        fishy_alpha[i].frames = FISHY_ONSCREEN;
        SDL_SetAlpha(fishy_alpha[i].image, (SDL_SRCALPHA | SDL_RLEACCEL), DEFAULT_ALPHA);
        if (fishy[i].image == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    for (i = 0; i < SPLAT_FRAMES; i++) {
        if (verbose == YES)
            fprintf(stderr, "=>Loading FISHY SPLAT frame %i\n", i);
        sprintf(filename, "splat%i.png", (i + 1));
        if (verbose == YES)
            fprintf(stderr, "=>Loading FISHY SPLAT frame %s\n", filename);
        splat[i].image = LoadImage(filename, 1, verbose);
        splat[i].frames = SPLAT_ONSCREEN;
        if (splat[i].image == NULL) {
            fprintf(stderr, "!!Error Allocating Image Files!\n");
            exit(2);
        }
    }

    for (i = 0; i < MAX_FISHIES_HARD; i++) {
        fish_object[i].alive = NO;
        splat_object[i].alive = NO;
    }

    if (verbose == YES)
        fprintf(stderr, "=LoadFishies(): END\n");
}

/******************************
  LoadTuxAnims : Load the Tux
  graphics and animations
*******************************/
void LoadTuxAnims(int verbose)
{
    //load the tux animations
    int             i,
                    j;
    char            filename[FNLEN];
    int             width,
                    height;     //temp width/height varis to determine max's

    if (verbose == YES)
        fprintf(stderr, "=LoadTuxAnims(): Loading Tux Animations\n");

    for (i = 0; i < STAND_FRAMES; i++) {
        //load standing tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux STAND anim frame %i, %s\n",
                        i, DIR[j]);
            sprintf(filename, "tstand%s%i.png", DIR[j], (i + 1));
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux STAND anim frame %s\n",
                        filename);
            tuxStand[i].image[j] = LoadImage(filename, 1, verbose);
            if (tuxStand[i].image[j] == NULL) {
                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
            width = tuxStand[i].image[j]->w;
            height = tuxStand[i].image[j]->h;
        }
        tuxStand[i].frames = STAND_ONSCREEN;
    }

    for (i = 0; i < WALK_FRAMES; i++) {
        //load walking tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux WALK anim frame %i, %s\n",
                        i, DIR[j]);
            sprintf(filename, "twalk%s%i.png", DIR[j], (i + 1));
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux WALK anim frame %s\n",
                        filename);
            tuxWalk[i].image[j] = LoadImage(filename, 1, verbose);
            if (tuxWalk[i].image[j] == NULL) {
                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
            if ((tuxWalk[i].image[j]->w) > width)
                width = tuxWalk[i].image[j]->w;
            if ((tuxWalk[i].image[j]->h) > height)
                height = tuxWalk[i].image[j]->h;
        }
        tuxWalk[i].frames = WALK_ONSCREEN;
    }

    for (i = 0; i < RUN_FRAMES; i++) {
        //load running tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux RUN anim frame %i, %s\n", i,
                        DIR[j]);
            sprintf(filename, "trun%s%i.png", DIR[j], (i + 1));
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux RUN anim frame %s\n",
                        filename);
            tuxRun[i].image[j] = LoadImage(filename, 1, verbose);
            if (tuxRun[i].image[j] == NULL) {
                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
            if ((tuxRun[i].image[j]->w) > width)
                width = tuxRun[i].image[j]->w;
            if ((tuxRun[i].image[j]->h) > height)
                height = tuxRun[i].image[j]->h;
        }
        tuxRun[i].frames = RUN_ONSCREEN;
    }

    for (i = 0; i < GULP_FRAMES; i++) {
        //load gulping tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux GULP anim frame %i, %s\n",
                        i, DIR[j]);
            sprintf(filename, "tgulp%s%i.png", DIR[j], (i + 1));
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux GULP anim frame %s\n",
                        filename);
            tuxGulp[i].image[j] = LoadImage(filename, 1, verbose);
            if (tuxGulp[i].image[j] == NULL) {
                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
            if ((tuxGulp[i].image[j]->w) > width)
                width = tuxGulp[i].image[j]->w;
            if ((tuxGulp[i].image[j]->h) > height)
                height = tuxGulp[i].image[j]->h;
        }
        tuxGulp[i].frames = GULP_ONSCREEN;
    }

    for (i = 0; i < WIN_FRAMES; i++) {
        //load winning tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux WIN anim frame %i, %s\n", i,
                        DIR[j]);
            sprintf(filename, "tohyeah%s%i.png", DIR[j], (i + 1));
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux WIN anim frame %s\n",
                        filename);
            tuxWin[i].image[j] = LoadImage(filename, 1, verbose);
            if (tuxWin[i].image[j] == NULL) {

                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
            if ((tuxWin[i].image[j]->w) > width)
                width = tuxWin[i].image[j]->w;
            if ((tuxWin[i].image[j]->h) > height)
                height = tuxWin[i].image[j]->h;
        }
        tuxWin[i].frames = WIN_ONSCREEN;
    }

    for (i = 0; i < YIPE_FRAMES; i++) {
        //load hurting tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux YIPE anim frame %i, %s\n",
                        i, DIR[j]);
            sprintf(filename, "tyipe%s%i.png", DIR[j], (i + 1));
            if (verbose == YES)
                fprintf(stderr, "=>Loading tux YIPE anim frame %s\n",
                        filename);
            tuxYipe[i].image[j] = LoadImage(filename, 1, verbose);
            if (tuxYipe[i].image[j] == NULL) {
                fprintf(stderr, "!!Error Allocating Image Files!\n");
                exit(2);
            }
            if ((tuxYipe[i].image[j]->w) > width)
                width = tuxYipe[i].image[j]->w;
            if ((tuxYipe[i].image[j]->h) > height)
                height = tuxYipe[i].image[j]->h;
        }
        tuxYipe[i].frames = YIPE_ONSCREEN;
    }

    tux_max_width = width + 1;

    if (verbose == YES)
        fprintf(stderr, "=>Setting up Tux - the object\n");

    tux_object.facing = RIGHT;
    tux_object.x = 1;
    tux_object.y = (screen->h) - (height + 1);
    tux_object.dx = 0;
    tux_object.dy = 0;
    tux_object.endframe = tuxStand[0].frames;
    tux_object.frametype = STANDING;
    tux_object.curframe = 0;

    if (verbose == YES)
        fprintf(stderr, "=LoadTuxAnims(): END\n");
}

/******************************
 DrawNumbrs : Draw numbers at
 a certain x,y. See "usage"
 bellow
*******************************/
void DrawNumbers(int num, int x, int y, int places)
{
    //usage:
    //      num    = number to draw onscreen
    //      x, y   = coords to place number (starting upper left)
    //      places = number of places to fit it into (i.e., if
    //                                       number = 5 and places = 2, would draw "05")
    //                                       if places = 0, then will simply display as
    //                                       many as necessary
    char            numnuts[FNLEN];
    int             needed_places,
                    i;
    int             uddernumber;

    sprintf(numnuts, "%d", num);
    i = 0;
    needed_places = strlen(numnuts);

    if (needed_places < FNLEN && needed_places <= places) {
        if (places > 0) {
            for (i = 1; i <= (places - needed_places); i++) {
                DrawObject(number[0], x, y);
                x += number[0]->w;
            }
        }
    }
    for (i = 0; i < needed_places; i++) {
        uddernumber = numnuts[i] - '0';

        DrawObject(number[uddernumber], x, y);
        x += number[uddernumber]->w;
    }
}

/*************************
  EraseNumbers: Erase numbers
  from the screen. See "usage"
*****************************/
void EraseNumbers(int num, int x, int y, int places)
{
    //usage:
    //      num    = number to draw onscreen
    //      x, y   = coords to place number (starting upper left)
    //      places = number of places to fit it into (i.e., if
    //                                       number = 5 and places = 2, would draw "05")
    //                                       if places = 0, then will simply display as
    //                                       many as necessary
    char            numnuts[FNLEN];
    int             needed_places,
                    i;
    int             uddernumber;

    sprintf(numnuts, "%d", num);
    i = 0;
    needed_places = strlen(numnuts);

    if (needed_places < FNLEN && needed_places <= places) {
        if (places > 0) {
            for (i = 1; i <= (places - needed_places); i++) {
                EraseObject(number[0], x, y);
                x += number[0]->w;
            }
        }
    }
    for (i = 0; i < needed_places; i++) {
        uddernumber = numnuts[i] - '0';
        EraseObject(number[uddernumber], x, y);
        x += number[uddernumber]->w;
    }
}

/**********************
  FreeGame : Free all
  the game elements
***********************/
//void FreeGame(int verbose)
void FreeGame(int verbose, int mus)
{
    // if mus == 1 there is no music, if mus == 0 stop the music also
    int             i,
                    j;

    if (verbose == YES)
        fprintf(stderr, "FreeGame():\n-Freeing Tux Animations\n");

    for (i = 0; i < STAND_FRAMES; i++) {
        //standing tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            SDL_FreeSurface(tuxStand[i].image[j]);
        }
    }

    for (i = 0; i < WALK_FRAMES; i++) {
        //walking tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            SDL_FreeSurface(tuxWalk[i].image[j]);
        }
    }

    for (i = 0; i < RUN_FRAMES; i++) {
        //running tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            SDL_FreeSurface(tuxRun[i].image[j]);
        }
    }

    for (i = 0; i < GULP_FRAMES; i++) {
        //gulping tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            SDL_FreeSurface(tuxGulp[i].image[j]);
        }
    }

    for (i = 0; i < WIN_FRAMES; i++) {
        //winning tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            SDL_FreeSurface(tuxWin[i].image[j]);
        }
    }

    for (i = 0; i < YIPE_FRAMES; i++) {
        //hurting tux
        for (j = 0; j < RIGHT_LEFT; j++) {
            SDL_FreeSurface(tuxYipe[i].image[j]);
        }
    }

    if (verbose == YES)
        fprintf(stderr, "-Freeing fishies\n");

    for (i = 0; i < FISHY_FRAMES; i++) {
        SDL_FreeSurface(fishy[i].image);
        SDL_FreeSurface(fishy_alpha[i].image);
    }

    for (i = 0; i < SPLAT_FRAMES; i++) {
        SDL_FreeSurface(splat[i].image);
    }

    if (verbose == YES)
        fprintf(stderr, "-Freeing other game graphics\n");

    SDL_FreeSurface(background);

    SDL_FreeSurface(lives);
    SDL_FreeSurface(fish);

    for (i = 0; i < NUM_LEVELS; i++)
        SDL_FreeSurface(level[i]);
    for (i = 0; i < NUM_NUMS; i++)
        SDL_FreeSurface(number[i]);
    for (i = 0; i < NUM_LETTERS; i++) {
        SDL_FreeSurface(letter[i]);
        SDL_FreeSurface(letter_alpha[i]);
    }
    for (i = 0; i < CONGRATS_FRAMES; i++)
        SDL_FreeSurface(congrats[i]);

    for (i = 0; i < OH_NO_FRAMES; i++)
        SDL_FreeSurface(ohno[i]);

    if (sys_sound == YES) {
        if (verbose == YES)
            fprintf(stderr, "-Freeing music and sound\n");

        /*
         * Free sounds 
         */
        //printf("trying to free musc\n");
        if (!mus)
            Mix_FreeMusic(music);

        //printf("FREED MUSIC\n");

        for (i = 0; i < NUM_WAVES; ++i) {
            Mix_FreeChunk(sound[i]);
            //printf("FREED CHUNK sound %i\n", i);
        }
    }

    if (verbose == YES)
        fprintf(stderr, "FreeGame(): END\n");
}

/***************************
  UpdateBackground : This
  function updates the entire
  background. Usefull when
  loading new backgrounds,
  or clearing game screen
****************************/
void UpdateBackground(int verbose)
{
    //update the background
    if (verbose == YES)
        fprintf(stderr,
                "-UpdateBackground(): Updating entire background\n");
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

/****************************
 SpawnFishies: Spawn the fishes
 in the key cascade game
*****************************/
void SpawnFishies(int diflevel, int *fishies, int *local_max_fishies,
                  int *frame, int verbose)
{
    //spawn fishes if avail space
    double          phase,
                    angle;
    int             spd,
                    framebay,
                    terminator,
                    i,
                    xamp,
                    fish_spacing;

    if (diflevel == EASY || diflevel == INF_PRACT) {
        xamp = MAX_AMPLITUDE_EASY;
        spd = MAX_SPEED_VARIANCE_EASY;
        phase = MAX_PHASE_EASY;
        angle = MAX_ANGLE_EASY;
        fish_spacing = FISH_SPACING_EASY;
    } else if (diflevel == MEDIUM) {
        xamp = MAX_AMPLITUDE_MEDIUM;
        spd = MAX_SPEED_VARIANCE_MEDIUM;
        phase = MAX_PHASE_MEDIUM;
        angle = MAX_ANGLE_MEDIUM;
        fish_spacing = FISH_SPACING_MEDIUM;
    } else {
        xamp = MAX_AMPLITUDE_HARD;
        spd = MAX_SPEED_VARIANCE_HARD;
        phase = MAX_PHASE_HARD;
        angle = MAX_ANGLE_HARD;
        fish_spacing = FISH_SPACING_HARD;
    }

    if (*fishies < *local_max_fishies) {
        if (*fishies == 0) {
            if (verbose == YES)
                fprintf(stderr, "=>Spawning first fishy\n");
            fish_object[*fishies].alive = YES;
            fish_object[*fishies].y = 1;
            fish_object[*fishies].x0 =
                (xamp / 2) + rand() % (screen->w -
                                       (fishy[0].image->w + (xamp / 2)));
            fish_object[*fishies].x = fish_object[*fishies].x0;
            fish_object[*fishies].dy =
                DEFAULT_SPEED * ((rand() % spd) + 1);
            fish_object[*fishies].x_amplitude = rand() % xamp;
            fish_object[*fishies].x_phase =
                (double) (phase * rand() / (RAND_MAX + 1.0));
            fish_object[*fishies].x_angle_mult =
                (double) (angle * rand() / (RAND_MAX + 1.0));
            fish_object[*fishies].letter = rand() % NUM_LETTERS;
            fish_object[*fishies].curframe = 0;
            framebay = fishy[0].frames + *frame;
            if (framebay > FRAME_MAX)
                framebay -= FRAME_MAX;
            fish_object[*fishies].endframe = framebay;
            *fishies = *fishies + 1;
        } else {
            terminator = 0;
            for (i = 0; i < *fishies; i++)
                if (fish_object[i].y < (fishy[0].image->h + fish_spacing))
                    terminator++;       //wait for fishies to move one full length before spawning another to ensure clarity

            if (terminator == 0) {
                if (verbose == YES)
                    fprintf(stderr, "=>Spawning fishy #%i\n", *fishies);
                fish_object[*fishies].alive = YES;
                fish_object[*fishies].y = 1;
                fish_object[*fishies].x0 =
                    (xamp / 2) + rand() % (screen->w -
                                           (fishy[0].image->w +
                                            (xamp / 2)));
                fish_object[*fishies].x = fish_object[*fishies].x0;
                fish_object[*fishies].dy =
                    DEFAULT_SPEED * ((rand() % spd) + 1);
                fish_object[*fishies].x_amplitude = rand() % xamp;
                fish_object[*fishies].x_phase =
                    (double) (phase * rand() / (RAND_MAX + 1.0));
                fish_object[*fishies].x_angle_mult =
                    (double) (angle * rand() / (RAND_MAX + 1.0));;
                fish_object[*fishies].letter = rand() % NUM_LETTERS;
                fish_object[*fishies].curframe = 0;
                framebay = fishy[0].frames + *frame;
                if (framebay > FRAME_MAX)
                    framebay -= FRAME_MAX;
                fish_object[*fishies].endframe = framebay;
                *fishies = *fishies + 1;
            }
        }
    }
}

/***************************
 CheckFishies : Check all
 the fishies and splats.
****************************/
void CheckFishies(int *fishies, int *splats)
{
    //sort the splats and fishies
    int             stemp = 0;
    int             ftemp = 0;
    int             tcap;
    int             i;
    int             j;
    int             k;

    struct fishypoo fish_temp[MAX_FISHIES_HARD];
    struct splatter splat_temp[MAX_FISHIES_HARD];

    //sort the fish_object[] and splat_object[] lists so that all alive

    //objects are first in arrays
    //This is rather inefficient and slow, but should be fine for the small ammount
    //of onscreen fishes we're working with
    tcap = *fishies;

    if (tcap < *splats)
        tcap = *splats;

    for (i = 0; i <= tcap; i++) {
        if (fish_object[i].alive == YES) {
            fish_temp[ftemp] = fish_object[i];
            ftemp++;
        }
        if (splat_object[i].alive == YES) {
            splat_temp[stemp] = splat_object[i];
            stemp++;
        }
    }

	j = ftemp;
	k = stemp;
	for (i = 0; i < MAX_FISHIES_HARD; i++) {
		if (fish_object[i].alive == NO) {
			fish_temp[j] = fish_object[i];
			j++;
		}
		if (splat_object[i].alive == NO) {
			splat_temp[k] = splat_object[i];
			k++;
		}
	}

    for (i = 0; i < MAX_FISHIES_HARD; i++) {
        fish_object[i] = fish_temp[i];
        splat_object[i] = splat_temp[i];
    }

    *fishies = ftemp;
    *splats = stemp;
}

/***************************
 AddSplat: A fish has died,
 add a splat where he used
 to be
****************************/
void AddSplat(int *splats, int x, int y, int *curlives, int *frame)
{
    //splat a fishy
    int             framepoo;
    int             temp;

    temp = *splats;
    framepoo = *frame + splat[0].frames;
    if (framepoo > FRAME_MAX)
        framepoo -= FRAME_MAX;

    splat_object[temp].x = x;
    splat_object[temp].y = y;
    splat_object[temp].endframe = framepoo;
    splat_object[temp].curframe = 0;
    splat_object[temp].alive = YES;
    *splats = *splats + 1;
    *curlives = *curlives - 1;
    if (*curlives <= 0)
        *curlives = 0;
}

/****************************
  MoveFishies : Display and
  move the fishies according
  to their settings
*****************************/
void MoveFishies(int *fishies, int *splats, int *lifes, int *frame)
{
    //display and move dem fishes around
    int             i;
    int             sx,
                    sy;
    for (i = 0; i < *fishies; i++) {
        if (fish_object[i].alive == YES) {      //only want living fish

            EraseObject(fishy[fish_object[i].curframe].image, fish_object[i].x, fish_object[i].y);      //erase fishy

            fish_object[i].y += fish_object[i].dy;
            fish_object[i].x =
                fish_object[i].x0 +
                (fish_object[i].x_amplitude) *
                sin((fish_object[i].x_angle_mult) * fish_object[i].y +
                    fish_object[i].x_phase);

            if (fish_object[i].y >=
                (screen->h) - fishy[fish_object[i].curframe].image->h -
                1) {
                fish_object[i].alive = NO;
                sx = fish_object[i].x +
                    ((fishy[fish_object[i].curframe].image->w) / 2) -
                    ((splat[0].image->w) / 2);
                if (sx < 0)
                    sx = 0;
                if (sx > (screen->w - splat[0].image->w))
                    sx = screen->w - splat[0].image->w - 1;
                sy = screen->h - splat[0].image->h - 1;
                AddSplat(splats, sx, sy, lifes, frame);

                //play splatting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
                }

            }

            DrawObject(fishy[fish_object[i].curframe].image, fish_object[i].x, fish_object[i].y);       //draw fishy
            DrawObject(letter[fish_object[i].letter], fish_object[i].x, fish_object[i].y);      //draw letter overlay

            if (*frame == fish_object[i].endframe) {
                fish_object[i].curframe++;
                if (fish_object[i].curframe >= FISHY_FRAMES)
                    fish_object[i].curframe = 0;
                fish_object[i].endframe =
                    *frame + fishy[fish_object[i].curframe].frames;
                if (fish_object[i].endframe > FRAME_MAX)
                    fish_object[i].endframe -= FRAME_MAX;
            }
        }
    }

    for (i = 0; i < *splats; i++) {
        if (splat_object[i].alive == YES) {
            if (*frame == splat_object[i].endframe) {
                splat_object[i].curframe++;
                if (splat_object[i].curframe >= SPLAT_FRAMES) {
                    splat_object[i].curframe = 0;
                    splat_object[i].alive = NO;
                    EraseObject(splat[splat_object[i].curframe].image,
                                splat_object[i].x, splat_object[i].y);
                }
                splat_object[i].endframe = *frame + splat[0].frames;
                if (splat_object[i].endframe > FRAME_MAX)
                    splat_object[i].endframe -= FRAME_MAX;
            } else
                DrawObject(splat[splat_object[i].curframe].image,
                           splat_object[i].x, splat_object[i].y);
        }
    }
}

/******************
 COnvertKeyPress :
 See bellow - still needs to be fixed, 04/2001
********************/
int ConvertKeyPress(Uint8 * keys)
{
    // convert the Uint8 SDL key press to something we can use in our array
    //This routine must be modified if ever internationalized
    //---- BTW, this is going to be very inefficient and ugly... to be cleaned up later
    if (keys[SDLK_a] == SDL_PRESSED)
        return *"a" - *"a";
    else if (keys[SDLK_b] == SDL_PRESSED)
        return *"b" - *"a";
    else if (keys[SDLK_c] == SDL_PRESSED)
        return *"c" - *"a";
    else if (keys[SDLK_d] == SDL_PRESSED)
        return *"d" - *"a";
    else if (keys[SDLK_e] == SDL_PRESSED)
        return *"e" - *"a";
    else if (keys[SDLK_f] == SDL_PRESSED)
        return *"f" - *"a";
    else if (keys[SDLK_g] == SDL_PRESSED)
        return *"g" - *"a";
    else if (keys[SDLK_h] == SDL_PRESSED)
        return *"h" - *"a";
    else if (keys[SDLK_i] == SDL_PRESSED)
        return *"i" - *"a";
    else if (keys[SDLK_j] == SDL_PRESSED)
        return *"j" - *"a";
    else if (keys[SDLK_k] == SDL_PRESSED)
        return *"k" - *"a";
    else if (keys[SDLK_l] == SDL_PRESSED)
        return *"l" - *"a";
    else if (keys[SDLK_m] == SDL_PRESSED)
        return *"m" - *"a";
    else if (keys[SDLK_n] == SDL_PRESSED)
        return *"n" - *"a";
    else if (keys[SDLK_o] == SDL_PRESSED)
        return *"o" - *"a";
    else if (keys[SDLK_p] == SDL_PRESSED)
        return *"p" - *"a";
    else if (keys[SDLK_q] == SDL_PRESSED)
        return *"q" - *"a";
    else if (keys[SDLK_r] == SDL_PRESSED)
        return *"r" - *"a";
    else if (keys[SDLK_s] == SDL_PRESSED)
        return *"s" - *"a";
    else if (keys[SDLK_t] == SDL_PRESSED)
        return *"t" - *"a";
    else if (keys[SDLK_u] == SDL_PRESSED)
        return *"u" - *"a";
    else if (keys[SDLK_v] == SDL_PRESSED)
        return *"v" - *"a";
    else if (keys[SDLK_w] == SDL_PRESSED)
        return *"w" - *"a";
    else if (keys[SDLK_x] == SDL_PRESSED)
        return *"x" - *"a";
    else if (keys[SDLK_y] == SDL_PRESSED)
        return *"y" - *"a";
    else if (keys[SDLK_z] == SDL_PRESSED)
        return *"z" - *"a";
    else
        return -1;
}

/****************************
 UpdateTux : Update Tux's
 conditions
*****************************/
void UpdateTux(int letter_pressed, int fishies, int frame)
{
    //update Tux's general state of affairs
    int             i;
    double          time_to_hit = 100 * screen->h;

    tux_object.letter = NOT_A_LETTER;   //if you press a key, you must press one avail!

    for (i = 0; i < fishies; i++) {
        if (fish_object[i].alive == YES) {      //just in case I screwed up elsewhere
            if (fish_object[i].letter == letter_pressed) {
                if (((screen->h - fish_object[i].y) / fish_object[i].dy) < time_to_hit) {       //go to nearest letter
                    time_to_hit =
                        ((screen->
                          h - fish_object[i].y -
                          tuxGulp[0].image[0]->h) / fish_object[i].dy);
                    tux_object.endx =
                        fish_object[i].x0 +
                        ((fishy[fish_object[i].curframe].image->w) / 2) -
                        ((tuxGulp[0].image[RIGHT]->w) / 2);

                    if (tux_object.endx < 0)
                        tux_object.endx = 0;
                    if (tux_object.endx >= (screen->w - tux_max_width))
                        tux_object.endx = screen->w - tux_max_width - 1;

                    tux_object.letter = letter_pressed;
                }
            }
        }
    }

    if (tux_object.letter != NOT_A_LETTER) {
        if (tux_object.endx >= tux_object.x)
            tux_object.facing = RIGHT;
        else
            tux_object.facing = LEFT;

        //the following is needed to fully erase a running tux if state is changed during run
        //this is needed because of the speeds at which tux can run...
        //if his walking speed is ever increased (constant definition in playgame.h), then something
        //similar may be required for walking....               
        if (tux_object.frametype == RUNNING)
            RealEraseObject(tuxRun[tux_object.curframe].
                            image[tux_object.facing], tux_object.x,
                            tux_object.y);

        if (time_to_hit > (abs(tux_object.endx - tux_object.x) / WALKING_SPEED)) {      //we have time to walk to eat fishy
            tux_object.dx = WALKING_SPEED;
            tux_object.frametype = WALKING;
            tux_object.curframe = 0;
            tux_object.endframe = tuxWalk[0].frames + frame;
            if (tux_object.endframe > FRAME_MAX)
                tux_object.endframe -= FRAME_MAX;

            //stop running sound (if playing)                                               
            if (sys_sound == YES)
                if (Mix_Playing(RUN_WAV)) {
                    Mix_HaltChannel(RUN_WAV);
                }

        } else {                //woah, nelly, we'z gots'ta run!
            tux_object.dx =
                abs(tux_object.endx - tux_object.x) / time_to_hit;
            if (tux_object.dx < MIN_RUNNING_SPEED)
                tux_object.dx = MIN_RUNNING_SPEED;
            else if (tux_object.dx > MAX_RUNNING_SPEED)
                tux_object.dx = MAX_RUNNING_SPEED;      //it is possible for fishy to be too far away
            tux_object.frametype = RUNNING;
            tux_object.curframe = 0;
            tux_object.endframe = tuxRun[0].frames + frame;
            if (tux_object.endframe > FRAME_MAX)
                tux_object.endframe -= FRAME_MAX;

            //play running sound
            if (sys_sound == YES) {
                Mix_PlayChannel(RUN_WAV, sound[RUN_WAV], 0);
            }
        }
    }
}

/*************************
 CheckCollision: Check
 for collisions between Tux
 and Fishies. If collided,
 perform appropriate action
***************************/
void CheckCollision(int fishies, int *fish_left, int frame, int verbose)
{
    //check for collision w/ tux and fishy
    int             i,
                    framesy;

    rl_object      *temp;

    if (tux_object.frametype == WALKING) {
        temp = tuxWalk;
        framesy = WALK_FRAMES;
    } else if (tux_object.frametype == STANDING) {
        temp = tuxStand;
        framesy = STAND_FRAMES;
    } else if (tux_object.frametype == RUNNING) {
        temp = tuxRun;
        framesy = RUN_FRAMES;
    } else if (tux_object.frametype == GULPING) {

        temp = tuxGulp;
        framesy = GULP_FRAMES;
    } else if (tux_object.frametype == WINNING) {
        temp = tuxWin;
        framesy = WIN_FRAMES;
    } else {
        temp = tuxYipe;
        framesy = YIPE_FRAMES;
    }

    for (i = 0; i < fishies; i++) {
        if ((fish_object[i].x >= tux_object.x
             && fish_object[i].x <=
             tux_object.x +
             temp[tux_object.curframe].image[tux_object.facing]->w)
            && (fish_object[i].y >=
                tux_object.y - fishy[fish_object[i].curframe].image->h)) {

            if (fish_object[i].letter == tux_object.letter) {   // then eat the fishy!
                if (verbose == YES)
                    fprintf(stderr,
                            "**EATING A FISHY** - in CheckCollision()\n");

                fish_object[i].alive = NO;

                EraseObject(fishy[fish_object[i].curframe].image, fish_object[i].x, fish_object[i].y);  //erase fishy

                if (tux_object.frametype == RUNNING)
                    RealEraseObject(tuxRun[tux_object.curframe].image[tux_object.facing], tux_object.x, tux_object.y);  //erase present tux, as well (prep for gulping)

                *fish_left = *fish_left - 1;
                tux_object.frametype = GULPING;
                tux_object.curframe = 0;
                tux_object.dx = 0;
                tux_object.endframe = tuxGulp[0].frames + frame;
                if (tux_object.endframe > FRAME_MAX)
                    tux_object.endframe -= FRAME_MAX;

                //play biting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(BITE_WAV, sound[BITE_WAV], 0);
                }

            } else if (tux_object.frametype == STANDING) {
                // tux will want to move out of the way if he's not eating the fishy
                if (verbose == YES)
                    fprintf(stderr,
                            "***EXCUSE ME!** - in CheckCollision()\n");

                tux_object.endx =
                    fish_object[i].x +
                    fishy[fish_object[i].curframe].image->w;
                tux_object.facing = RIGHT;
                if (tux_object.endx >= (screen->w - tux_max_width)) {
                    tux_object.endx = fish_object[i].x - tux_max_width;
                    tux_object.facing = LEFT;
                }
                tux_object.dx = WALKING_SPEED;
                tux_object.frametype = WALKING;
                tux_object.curframe = 0;
                tux_object.endframe = tuxWalk[0].frames + frame;
                if (tux_object.endframe > FRAME_MAX)
                    tux_object.endframe -= FRAME_MAX;

                //play excusing sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(EXCUSEME_WAV, sound[EXCUSEME_WAV], 0);
                }

            }
        }
    }
}

/***********************************
 MoveTux : Move Tux (duh!)
************************************/
void MoveTux(int frame, int verbose)
{
    //lets place and move Tux
    rl_object      *temp;       //not sure I like the speed of this approach, may monkey with in future
    int             dir;
    int             framesy;

    if (tux_object.facing == RIGHT)
        dir = 1;
    else
        dir = -1;

    if (tux_object.frametype == WALKING) {
        temp = tuxWalk;
        framesy = WALK_FRAMES;
    } else if (tux_object.frametype == STANDING) {
        temp = tuxStand;
        framesy = STAND_FRAMES;
    } else if (tux_object.frametype == RUNNING) {
        temp = tuxRun;
        framesy = RUN_FRAMES;
    } else if (tux_object.frametype == GULPING) {
        temp = tuxGulp;
        framesy = GULP_FRAMES;
    } else if (tux_object.frametype == WINNING) {
        temp = tuxWin;
        framesy = WIN_FRAMES;
    } else {
        temp = tuxYipe;
        framesy = YIPE_FRAMES;
    }

    EraseObject(temp[tux_object.curframe].image[tux_object.facing],
                tux_object.x, tux_object.y);

    //move tux (if moving)
    if (tux_object.dx != 0) {
        tux_object.x += dir * tux_object.dx;

        if (tux_object.x < 0)
            tux_object.x = 0;
        if (tux_object.x >= (screen->w - tux_max_width))
            tux_object.x = screen->w - tux_max_width - 1;
    }
    //cycle frames
    if (tux_object.frametype != GULPING) {
        if (frame == tux_object.endframe) {
            tux_object.curframe++;
            if (tux_object.curframe >= framesy)
                tux_object.curframe = 0;
            tux_object.endframe = temp[tux_object.curframe].frames + frame;
            if (tux_object.endframe > FRAME_MAX)
                tux_object.endframe -= FRAME_MAX;
        }
    } else {                    //we only cycle through gulp animation once per eat
        if (frame == tux_object.endframe) {
            tux_object.curframe++;
            if (tux_object.curframe < framesy) {
                tux_object.endframe =
                    temp[tux_object.curframe].frames + frame;
                if (tux_object.endframe > FRAME_MAX)
                    tux_object.endframe -= FRAME_MAX;
            } else {
                //EraseObject(temp[tux_object.curframe].image[tux_object.facing], tux_object.x, tux_object.y);
                tux_object.frametype = STANDING;
                tux_object.letter = NOT_A_LETTER;
                temp = tuxStand;
                tux_object.curframe = 0;
                tux_object.endframe =
                    temp[tux_object.curframe].frames + frame;
                if (tux_object.endframe > FRAME_MAX)
                    tux_object.endframe -= FRAME_MAX;
            }
        }
    }

    //if done with certain frames, then reset to standing
    if (tux_object.frametype == WALKING || tux_object.frametype == RUNNING) {
        if ((tux_object.facing == RIGHT && tux_object.x >= tux_object.endx)
            || (tux_object.facing == LEFT
                && tux_object.x <= tux_object.endx)) {

            //EraseObject(temp[tux_object.curframe].image[tux_object.facing], tux_object.x, tux_object.y);
            if (verbose == YES)
                fprintf(stderr,
                        "-In MoveTux(): returning tux to standing\n");
            tux_object.frametype = STANDING;
            tux_object.dx = 0;
            tux_object.x = tux_object.endx;
            temp = tuxStand;
            tux_object.curframe = 0;
            tux_object.endframe = temp[tux_object.curframe].frames + frame;
            if (tux_object.endframe > FRAME_MAX)
                tux_object.endframe -= FRAME_MAX;
        }
    }

    DrawObject(temp[tux_object.curframe].image[tux_object.facing],
               tux_object.x, tux_object.y);
}

/**********************
 Pause : Pause the game
***********************/
int Pause(int verbose)
{
    //pause the game
    int             j,
                    k,
                    f,
                    play_more;
    Uint8           state;
    SDL_Event       event;


    //stop all music and sounds, play pause noise
    if (verbose == YES)
        fprintf(stderr, "---GAME PAUSED---\n");

    if (sys_sound == YES) {
        Mix_PauseMusic();
        for (j = 0; j < NUM_WAVES; j++) {
            Mix_Pause(j);
        }
        Mix_PlayChannel(PAUSE_WAV, sound[PAUSE_WAV], 0);
    }
    //show mouse pointer
    SDL_ShowCursor(1);

    // We need to clear the screen of all the fish !!! (later)

    k = 0;
    for (j = 0; j < PAUSE_IMAGES; j++) {
        DrawObject(pause_img[j],
                   (screen->w / 2) - (pause_img[j]->w / 2),
                   (screen->h / 2) - (total_pause_h / 2) + k);
        k = pause_img[j]->h;
    }
    UpdateScreen(&f);

    k = NO;
    play_more = YES;
    while (k == NO) {
        state = SDL_GetMouseState(NULL, NULL);

        if (state == SDL_BUTTON(1) || state == SDL_BUTTON(2)
            || state == SDL_BUTTON(3))
            k = YES;
        else
            k = NO;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                k = YES;
                play_more = NO;
            }
            // ok, sdl registers an event when the user presses
            // the key and when they release it, so when I am
            // checking the keys, we need to make sure the space
            // is not from the space to pause, and then wait until
            // they release the key!!!
            if ((event.key.keysym.sym == SDLK_SPACE) &&
                (event.key.type != SDL_KEYUP)) {
                while (event.key.type != SDL_KEYUP)
                    SDL_PollEvent(&event);
                k = YES;
            }
        }
    }

    if (sys_sound == YES) {
        Mix_ResumeMusic();
        for (j = 0; j < NUM_WAVES; j++) {
            Mix_Resume(j);
        }
        Mix_PlayChannel(PAUSE_WAV, sound[PAUSE_WAV], 0);
    }
    //show mouse pointer
    SDL_ShowCursor(0);

    k = 0;
    for (j = 0; j < PAUSE_IMAGES; j++) {
        EraseObject(pause_img[j],
                    (screen->w / 2) - (pause_img[j]->w / 2),
                    (screen->h / 2) - (total_pause_h / 2) + k);
        k = pause_img[j]->h;
    }
    UpdateScreen(&f);

    if (verbose == YES)
        fprintf(stderr, "---GAME RESUMED---\n");

    return (play_more);
}

/*****************************
 PlayCascade : Play Key Cascade
 Sorry, I know this was a poor
 choice in function names!
 This is the main Key Cascade
 game loop
******************************/
int PlayCascade(int diflevel, int verbose)
{
    // play cascade
    char            filename[FNLEN];
    int             still_playing = YES;
    int             playing_level = YES;
    int             resume = YES;
    int             quitting = NO;
    int             curlevel = 0;
    int             i;
    int             curlives;
    int             oldlives,
                    oldfish_left;
    int             fish_left,
                    fishies = 0,
                    local_max_fishies;
    int             frame = 0;
    int             done_frames;        //, congrats_endframe, oh_no_endframe;
    int             splats = 0;
    int             letter_pressed;
    int             last_letter_pressed;
    SDL_Event       event;
    SDL_Surface    *temp_text[CONGRATS_FRAMES + OH_NO_FRAMES];
    SDL_Rect        text_rect;
    int             text_y_end;
    int             xamp,
                    yamp,
                    x_not,
                    y_not;
    rl_object      *temp_tux;
    int             temp_text_frames,
                    temp_tux_frames;
    int             temp_text_count,
                    temp_tux_count;
    Uint8          *keys;

    if (verbose == YES)
        fprintf(stderr, "->PlayCascade: level=%i\n", diflevel);

    srand(time(NULL));

    SDL_WarpMouse(screen->w / 2, screen->h / 2);

    //no cursor please
    SDL_ShowCursor(0);

    LoadTuxAnims(verbose);
    LoadFishies(verbose);
    LoadOthers(verbose);
    /*
     * Set up the update rectangle pointers 
     */
    for (i = 0; i < MAX_UPDATES; ++i) {
        blits[i].srcrect = &srcupdate[i];
        blits[i].dstrect = &dstupdate[i];
    }

    while (still_playing == YES) {
        //set up the number for fish based on diflevel
        if (diflevel == EASY) {
            fish_left = MAX_FISHIES_EASY;
            curlives = EASY_LIVES_INIT;
        } else if (diflevel == MEDIUM) {
            fish_left = MAX_FISHIES_MEDIUM;
            curlives = MEDIUM_LIVES_INIT;
        } else {
            fish_left = MAX_FISHIES_HARD;
            curlives = HARD_LIVES_INIT;
        }

        local_max_fishies = fish_left;

        if (diflevel == INF_PRACT) {
            sprintf(filename, "pract.png");
        } else {
            sprintf(filename, "kcas%i_%i.png", diflevel + 1, curlevel + 1);
        }

        /********************************************************
        * Special Hidden Code                                  *
        * See README file in images directory for info         *
        ********************************************************/

        if (hidden == YES && curlevel == 3) {
            sprintf(filename, "hidden.png");
        }

        /********************************************************/

        if (verbose == YES)
            fprintf(stderr, "->>Loading background: %s\n", filename);

        if (curlevel != 0)
            SDL_FreeSurface(background);

        background = LoadImage(filename, 0, verbose);

        UpdateBackground(verbose);

        //do any pre-game animation here
        ClearObjects(verbose);

        //load music file
        if (sys_sound == YES) {
            sprintf(filename, "kmus%i.wav", curlevel + 1);
            LoadMusic(filename, verbose);
        }

        if (verbose == YES)
            fprintf(stderr, "->>PLAYING THE GAME\n");

        i = 0;

        while (playing_level == YES) {
            /*
             * Poll input queue, run keyboard loop 
             */
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    resume = NO;
                    playing_level = NO;
                    still_playing = NO;
                    quitting = YES;
                }
            }

            keys = SDL_GetKeyState(NULL);

            if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
                resume = YES;
                playing_level = NO;
                still_playing = NO;
                quitting = YES;
            }

            if (keys[SDLK_PAUSE] == SDL_PRESSED || keys[SDLK_SPACE] == SDL_PRESSED) {   //pause the game
                if (Pause(verbose) == NO) {
                    playing_level = NO;
                    resume = NO;
                    still_playing = NO;
                    quitting = YES;

                }
            }

            MoveTux(frame, verbose);
            oldlives = curlives;
            oldfish_left = fish_left;
            SpawnFishies(diflevel, &fishies, &local_max_fishies, &frame,
                         verbose);
            last_letter_pressed = letter_pressed;

            letter_pressed = ConvertKeyPress(keys);

            //if (letter_pressed > -1) {
            if ((last_letter_pressed != letter_pressed) &&
                (letter_pressed > -1)) {
                if (verbose == YES)
                    fprintf(stderr, "-In PlayGame(): Key Just Pressed\n");
                UpdateTux(letter_pressed, fishies, frame);
            }


            CheckCollision(fishies, &fish_left, frame, verbose);
            MoveFishies(&fishies, &splats, &curlives, &frame);
            CheckFishies(&fishies, &splats);

            //update top score/info bar
            DrawObject(level[diflevel], 1, 1);
            if (diflevel != INF_PRACT) {
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel + 1,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);

                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                if (curlives <= 0) {
                    playing_level = NO;
                    still_playing = NO;
                }
            }

            if (diflevel == INF_PRACT)
                fish_left = 1;

            if (fish_left <= 0) {
                playing_level = NO;
                curlevel++;
                still_playing = YES;
            }
            UpdateScreen(&frame);
            if (speed_up == NO)
                WaitFrame();


            //play music if not already playing
            if (sys_sound == YES) {
                if (!Mix_PlayingMusic()) {
                    Mix_PlayMusic(music, 0);
                }
            }
        }

        if (verbose == YES)
            fprintf(stderr, "---In PlayGame(): Level Terminates\n");

        //stop music!
        if (sys_sound == YES)
            i = Mix_FadeOutMusic(MUSIC_FADE_OUT_MS);

        //oh_no_endframe = OH_NO_ONSCREEN;
        //congrats_endframe = CONGRATS_ONSCREEN;

        // I know very well what I'm about to do is cheesy, lazy, and dumb...
        // I know I need to go and move all this repeated code to a seperate
        // function... and this is something I'll do someday... but only
        // after I get the game running the way I want.....
        if (quitting == NO) {
            if (diflevel == INF_PRACT)

                curlives = 1;
            if (curlives <= 0) {
                //too bad here - game over
                done_frames = MAX_END_FRAMES_GAMEOVER;
                xamp = 0;
                yamp = 0;
                if (verbose == YES)
                    fprintf(stderr, "--->GAME OVER\n");

                //play losing sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(LOSE_WAV, sound[LOSE_WAV], 0);
                }
                //first, erase the fish
                for (i = 0; i < fishies; i++) {
                    if (fish_object[i].alive == YES) {  //only want living fish
                        EraseObject(fishy[fish_object[i].curframe].image, fish_object[i].x, fish_object[i].y);  //erase fishy
                    }
                }

                //now put splat on background to ensure it stays
                for (i = 0; i < splats; i++) {
                    if (splat_object[i].alive == YES) {
                        text_rect.x = splat_object[i].x;
                        text_rect.y = splat_object[i].y;
                        text_rect.w =
                            splat[splat_object[i].curframe].image->w;
                        text_rect.h =
                            splat[splat_object[i].curframe].image->h;
                        SDL_BlitSurface(splat[splat_object[i].curframe].
                                        image, NULL, background,
                                        &text_rect);
                    }
                }

                //redraw top bar
                DrawObject(level[diflevel], 1, 1);
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel + 1,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);
                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                //set up "OH NO!" animation and tux_yipe!
                for (i = 0; i < OH_NO_FRAMES; i++)
                    temp_text[i] = ohno[i];
                temp_text_frames = OH_NO_FRAMES;
                temp_tux = tuxYipe;
                temp_tux_frames = YIPE_FRAMES;
            } else if (curlevel >= 4) {
                //HEY WE WON!
                done_frames = MAX_END_FRAMES_WIN_GAME;
                still_playing = NO;
                xamp = WIN_GAME_XAMP;
                yamp = WIN_GAME_YAMP;
                if (verbose == YES)
                    fprintf(stderr, "--->WINNER!\n");

                //play winning finale sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(WINFINAL_WAV, sound[WINFINAL_WAV], 0);
                    Mix_PlayChannel(WIN_WAV, sound[WIN_WAV], 0);
                }
                //first, erase the fish
                for (i = 0; i < fishies; i++) {
                    if (fish_object[i].alive == YES) {  //only want living fish
                        EraseObject(fishy[fish_object[i].curframe].image, fish_object[i].x, fish_object[i].y);  //erase fishy
                    }
                }

                //now put any splats left on background to ensure they stay
                for (i = 0; i < splats; i++) {
                    if (splat_object[i].alive == YES) {
                        text_rect.x = splat_object[i].x;
                        text_rect.y = splat_object[i].y;
                        text_rect.w =
                            splat[splat_object[i].curframe].image->w;
                        text_rect.h =
                            splat[splat_object[i].curframe].image->h;
                        SDL_BlitSurface(splat[splat_object[i].curframe].
                                        image, NULL, background,
                                        &text_rect);
                    }
                }

                //redraw top bar
                DrawObject(level[diflevel], 1, 1);
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);
                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                //set up congrats
                for (i = 0; i < CONGRATS_FRAMES; i++)
                    temp_text[i] = congrats[i];
                temp_text_frames = CONGRATS_FRAMES;
                temp_tux = tuxWin;
                temp_tux_frames = WIN_FRAMES;
            } else {
                //moving on to next level
                done_frames = MAX_END_FRAMES_BETWEEN_LEVELS;
                playing_level = YES;
                xamp = 0;
                yamp = 0;
                if (verbose == YES)
                    fprintf(stderr, "--->NEXT LEVEL!\n");

                //play winning sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(WIN_WAV, sound[WIN_WAV], 0);
                }
                //first, erase the fish
                for (i = 0; i < fishies; i++) {
                    if (fish_object[i].alive == YES) {  //only want living fish
                        EraseObject(fishy[fish_object[i].curframe].image, fish_object[i].x, fish_object[i].y);  //erase fishy
                    }
                }

                //now put any splats left on background to ensure they stay
                for (i = 0; i < splats; i++) {
                    if (splat_object[i].alive == YES) {
                        text_rect.x = splat_object[i].x;
                        text_rect.y = splat_object[i].y;
                        text_rect.w =
                            splat[splat_object[i].curframe].image->w;
                        text_rect.h =
                            splat[splat_object[i].curframe].image->h;
                        SDL_BlitSurface(splat[splat_object[i].curframe].
                                        image, NULL, background,
                                        &text_rect);
                    }
                }

                //redraw top bar
                DrawObject(level[diflevel], 1, 1);
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);
                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                //set up congrats
                for (i = 0; i < CONGRATS_FRAMES; i++)
                    temp_text[i] = congrats[i];
                temp_text_frames = CONGRATS_FRAMES;
                temp_tux = tuxWin;
                temp_tux_frames = WIN_FRAMES;
            }

            i = 0;
            temp_text_count = 0;
            temp_tux_count = 0;
            text_y_end = (screen->h / 2) - (temp_text[0]->h / 2);
            text_rect.x = (screen->w / 2) - (temp_text[0]->w / 2);
            text_rect.y = screen->h - temp_text[0]->h - 1;
            x_not = text_rect.x;

            if (verbose == YES)
                fprintf(stderr, "--->Starting Ending Animation\n");

            while (i <= done_frames) {  // display ending animation!
                EraseObject(temp_tux[temp_tux_count].
                            image[tux_object.facing], tux_object.x,
                            tux_object.y);
                EraseObject(temp_text[temp_text_count], text_rect.x,
                            text_rect.y);
                EraseObject(temp_text[temp_text_count], text_rect.x,
                            text_rect.y - 1);

                temp_tux_count++;
                if (temp_tux_count >= temp_tux_frames)
                    temp_tux_count = 0;

                temp_text_count++;
                if (temp_text_count >= temp_text_frames)
                    temp_text_count = 0;

                text_rect.y -= END_FRAME_DY;
                y_not = text_rect.y;

                if (text_rect.y < text_y_end) {
                    y_not =
                        text_y_end + yamp * sin(i / WIN_GAME_ANGLE_MULT);
                    text_rect.y = text_y_end;
                    text_rect.x =
                        x_not + xamp * cos(i / WIN_GAME_ANGLE_MULT);
                }

                DrawObject(temp_tux[temp_tux_count].
                           image[tux_object.facing], tux_object.x,
                           tux_object.y);
                DrawObject(temp_text[temp_text_count], text_rect.x, y_not);

                UpdateScreen(&frame);
                if (speed_up == NO)
                    WaitFrame();
                i++;
            }
        }
    }

    if (verbose == YES)
        fprintf(stderr, "->Done with level... cleaning up\n");

    /*
     * Wait a while- give screen chance to come up (if fullscreen) 
     */
    SDL_Delay(WAIT_MS);

    FreeGame(verbose, 0);       //free the game stuff

    if (verbose == YES)
        fprintf(stderr, "->PlayCascade: END\n");

    return (resume);
}

/************************************
 PraciticeKCascade : The Key Cascade
 Tutorial
************************************/
int PracticeKCascade(int verbose)
{
    //Give a little tutorial on playing Key Cascade
    int             still_playing = YES,
                    exit_tut = NO;
    int             i,
                    tut_frame = 0,
                    watchdog;
    char            filename[FNLEN];
    int             fish_left = 1,
                    fishies = 0;
    int             frame = 0,
                    framebay;
    int             splats = 0;
    int             sx,
                    sy;
    SDL_Event       event;
    Uint8          *keys;
    SDL_Surface    *TutScreen[PKCASCADE_TUT_SCREENS];

    if (verbose == YES)
        fprintf(stderr, "->PractceKCascade(): START TUTORIAL\n");

    SDL_WarpMouse(screen->w / 2, screen->h / 2);

    //no cursor please
    SDL_ShowCursor(0);

    //we may not use everything we're about to load, but rather than
    //making seperate loading routines for practice, we'll just use these           
    LoadTuxAnims(verbose);
    LoadFishies(verbose);
    LoadOthers(verbose);
    /*
     * Set up the update rectangle pointers 
     */
    for (i = 0; i < MAX_UPDATES; ++i) {
        blits[i].srcrect = &srcupdate[i];
        blits[i].dstrect = &dstupdate[i];
    }

    sprintf(filename, "tutor.png");
    if (verbose == YES)
        fprintf(stderr, "->>Loading background: %s\n", filename);
    background = LoadImage(filename, 0, verbose);

    UpdateBackground(verbose);

    //clear everything
    ClearObjects(verbose);

    //load the Tutorial Screens
    for (i = 0; i < PKCASCADE_TUT_SCREENS; i++) {
        sprintf(filename, "tutor%i.png", i + 1);
        TutScreen[i] = LoadImage(filename, 1, verbose);
    }

    //BEGIN THE TUTORIAL - FIRST TUT-FRAME, INTRO AND TUX STANDING THERE
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 1\n");

    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    watchdog = 0;
    //place tux & fishy
    tux_object.x = (TutScreen[0]->w / 2) - tuxStand[0].image[RIGHT]->w / 2;
    tux_object.y =
        screen->h - tuxStand[0].image[RIGHT]->h - 1 - PRACT_TUX_Y;
    tux_object.dx = 0;
    tux_object.dy = 0;
    tux_object.endx = tux_object.x;
    tux_object.endy = tux_object.y;
    tux_object.letter = NOT_A_LETTER;

    fish_object[fishies].alive = YES;
    fish_object[fishies].y = 1;
    fish_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w / 2;
    fish_object[fishies].x = fish_object[fishies].x0;
    fish_object[fishies].dy = DEFAULT_TUT1_FISH_SPEED;

    fish_object[fishies].x_amplitude = 0;
    fish_object[fishies].x_phase = 0;
    fish_object[fishies].x_angle_mult = 0;
    fish_object[fishies].letter = NOT_A_LETTER;
    fish_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    fish_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        if (fishies > 0) {
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy

            fish_object[0].y += fish_object[0].dy;

            DrawObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);       //draw fishy

            if (frame == fish_object[0].endframe) {
                fish_object[0].curframe++;
                if (fish_object[0].curframe >= FISHY_FRAMES)
                    fish_object[0].curframe = 0;
                fish_object[0].endframe =
                    frame + fishy[fish_object[0].curframe].frames;
                if (fish_object[0].endframe > FRAME_MAX)
                    fish_object[0].endframe -= FRAME_MAX;
            }

            watchdog = 0;
        } else {
            watchdog++;
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy
        }

        CheckCollision(fishies, &fish_left, frame, verbose);
        CheckFishies(&fishies, &splats);

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }

    //SECOND TUT-FRAME, MOVE TUX, introduce real fishy w/ letter and eat it
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 2\n");

    EraseObject(TutScreen[tut_frame],
                screen->w - TutScreen[tut_frame]->w - 1, 0);
    watchdog = 0;
    tut_frame++;
    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    //place tux & fishy
    tux_object.dx = TUX2_DX1;
    tux_object.facing = LEFT;
    tux_object.frametype = WALKING;
    tux_object.endx = TUX2_ENDX_GOTO1;
    tux_object.letter = NOT_A_LETTER;

    fish_object[fishies].alive = YES;
    fish_object[fishies].y = 1;
    fish_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w / 2;
    fish_object[fishies].x = fish_object[fishies].x0;
    fish_object[fishies].dy = DEFAULT_TUT2_FISH_SPEED;
    fish_object[fishies].x_amplitude = 0;
    fish_object[fishies].x_phase = 0;
    fish_object[fishies].x_angle_mult = 0;
    fish_object[fishies].letter = 0;
    fish_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    fish_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        if (fishies > 0) {
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy

            fish_object[0].y += fish_object[0].dy;

            DrawObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);       //draw fishy
            DrawObject(letter[fish_object[0].letter], fish_object[0].x, fish_object[0].y);      //draw letter overlay


            if (frame == fish_object[0].endframe) {
                fish_object[0].curframe++;
                if (fish_object[0].curframe >= FISHY_FRAMES)
                    fish_object[0].curframe = 0;
                fish_object[0].endframe =
                    frame + fishy[fish_object[0].curframe].frames;
                if (fish_object[0].endframe > FRAME_MAX)
                    fish_object[0].endframe -= FRAME_MAX;
            }
            watchdog = 0;
        } else {
            watchdog++;
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy
        }

        if (tux_object.facing == LEFT
            && fish_object[0].y >=
            (screen->h / 2 -
             2 * fishy[fish_object[0].curframe].image->h)) {
            tux_object.facing = RIGHT;
            tux_object.frametype = WALKING;
            tux_object.curframe = 0;
            tux_object.dx = TUX2_DX1;
            tux_object.endx =
                (TutScreen[0]->w / 2) - tuxStand[0].image[RIGHT]->w / 2;
            tux_object.letter = 0;
            EraseObject(TutScreen[tut_frame],
                        screen->w - TutScreen[tut_frame]->w - 1, 0);
            watchdog = 0;
            tut_frame++;
            DrawObject(TutScreen[tut_frame],
                       screen->w - TutScreen[tut_frame]->w - 1, 0);
        }

        CheckCollision(fishies, &fish_left, frame, verbose);
        CheckFishies(&fishies, &splats);

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }

    //THIRD TUT-FRAME, fishy go boom
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 3\n");

    EraseObject(TutScreen[tut_frame],
                screen->w - TutScreen[tut_frame]->w - 1, 0);
    watchdog = 0;
    tut_frame++;
    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    //place tux & fishy
    fishies = 0;
    splats = 0;
    tux_object.dx = TUX3_DX1;
    tux_object.facing = LEFT;
    tux_object.frametype = WALKING;
    tux_object.endx = TUX3_ENDX_GOTO1;
    tux_object.letter = NOT_A_LETTER;

    fish_object[fishies].alive = YES;
    fish_object[fishies].y = 1;
    fish_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w / 2;
    fish_object[fishies].x = fish_object[fishies].x0;
    fish_object[fishies].dy = DEFAULT_TUT3_FISH_SPEED;
    fish_object[fishies].x_amplitude = 0;
    fish_object[fishies].x_phase = 0;
    fish_object[fishies].x_angle_mult = 0;
    fish_object[fishies].letter = 0;
    fish_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    fish_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }


        if (tux_object.dx == 0)
            tux_object.facing = RIGHT;

        if (fishies > 0) {
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy

            fish_object[0].y += fish_object[0].dy;

            if (fish_object[0].y >=
                (screen->h) - fishy[fish_object[0].curframe].image->h -
                FISH3_MAX_Y - 1) {
                fish_object[0].alive = NO;
                sx = fish_object[0].x +
                    ((fishy[fish_object[0].curframe].image->w) / 2) -
                    ((splat[0].image->w) / 2);
                if (sx < 0)
                    sx = 0;
                if (sx > (screen->w - splat[0].image->w))
                    sx = screen->w - splat[0].image->w - 1;
                sy = screen->h - splat[0].image->h - FISH3_MAX_Y - 1;
                AddSplat(&splats, sx, sy, &i, &frame);

                tux_object.frametype = YIPING;
                tux_object.curframe = 0;
                tux_object.facing = RIGHT;

                EraseObject(tuxStand[tux_object.curframe].image[LEFT],
                            tux_object.x, tux_object.y);
                DrawObject(tuxYipe[tux_object.curframe].image[RIGHT],
                           tux_object.x, tux_object.y);

                //play splatting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
                }
            } else
                MoveTux(frame, verbose);

            if (fish_object[0].alive == YES) {
                DrawObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);   //draw fishy
                DrawObject(letter[fish_object[0].letter], fish_object[0].x, fish_object[0].y);  //draw letter overlay

                if (frame == fish_object[0].endframe) {
                    fish_object[0].curframe++;
                    if (fish_object[0].curframe >= FISHY_FRAMES)
                        fish_object[0].curframe = 0;
                    fish_object[0].endframe =
                        frame + fishy[fish_object[0].curframe].frames;
                    if (fish_object[0].endframe > FRAME_MAX)
                        fish_object[0].endframe -= FRAME_MAX;
                }
            }
            watchdog = 0;
        } else
            watchdog++;

        CheckCollision(fishies, &fish_left, frame, verbose);
        CheckFishies(&fishies, &splats);

        if (splats > 0) {       //draw splat permanent!
            for (i = 0; i < splats; i++) {
                if (splat_object[i].alive == YES) {
                    DrawObject(splat[splat_object[i].curframe].image,
                               splat_object[i].x, splat_object[i].y);
                    if (frame == splat_object[i].endframe) {
                        splat_object[i].curframe++;
                        if (splat_object[i].curframe >= SPLAT_FRAMES) {
                            splat_object[i].curframe = SPLAT_FRAMES - 1;
                        }
                        splat_object[i].endframe = frame + splat[0].frames;
                        if (splat_object[i].endframe > FRAME_MAX)
                            splat_object[i].endframe -= FRAME_MAX;
                    }
                }
            }
        }

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }

    //FOURTH TUT-FRAME, Tux is so polite!
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 4\n");

    EraseObject(TutScreen[tut_frame],
                screen->w - TutScreen[tut_frame]->w - 1, 0);

    if (splats > 0)
        EraseObject(splat[splat_object[0].curframe].image,
                    splat_object[0].x, splat_object[0].y);

    watchdog = 0;
    tut_frame++;
    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    //place tux & fishy
    fishies = 0;
    splats = 0;
    splat_object[0].alive = NO;
    tux_object.dx = TUX4_DX1;
    tux_object.facing = RIGHT;
    tux_object.frametype = WALKING;
    tux_object.endx =
        (TutScreen[0]->w / 2) - tuxStand[0].image[RIGHT]->w / 2;
    tux_object.letter = NOT_A_LETTER;

    fish_object[fishies].alive = YES;
    fish_object[fishies].y = 1;
    fish_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w / 2;
    fish_object[fishies].x = fish_object[fishies].x0;
    fish_object[fishies].dy = DEFAULT_TUT4_FISH_SPEED;
    fish_object[fishies].x_amplitude = 0;
    fish_object[fishies].x_phase = 0;
    fish_object[fishies].x_angle_mult = 0;
    fish_object[fishies].letter = 1;
    fish_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    fish_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        if (fishies > 0) {
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy

            fish_object[0].y += fish_object[0].dy;

            if (fish_object[0].y >=
                (screen->h) - fishy[fish_object[0].curframe].image->h -
                FISH4_MAX_Y - 1) {
                fish_object[0].alive = NO;
                sx = fish_object[0].x +
                    ((fishy[fish_object[0].curframe].image->w) / 2) -
                    ((splat[0].image->w) / 2);
                if (sx < 0)
                    sx = 0;
                if (sx > (screen->w - splat[0].image->w))
                    sx = screen->w - splat[0].image->w - 1;
                sy = screen->h - splat[0].image->h - FISH4_MAX_Y - 1;
                AddSplat(&splats, sx, sy, &i, &frame);

                tux_object.frametype = YIPING;
                tux_object.curframe = 0;
                tux_object.facing = LEFT;

                EraseObject(tuxStand[tux_object.curframe].image[RIGHT],
                            tux_object.x, tux_object.y);
                DrawObject(tuxYipe[tux_object.curframe].image[LEFT],
                           tux_object.x, tux_object.y);

                EraseObject(TutScreen[tut_frame],
                            screen->w - TutScreen[tut_frame]->w - 1, 0);
                tut_frame++;
                DrawObject(TutScreen[tut_frame],
                           screen->w - TutScreen[tut_frame]->w - 1, 0);

                //play splatting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
                }

            }

            if (fish_object[0].alive == YES) {
                DrawObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);   //draw fishy
                DrawObject(letter[fish_object[0].letter], fish_object[0].x, fish_object[0].y);  //draw letter overlay

                if (frame == fish_object[0].endframe) {
                    fish_object[0].curframe++;
                    if (fish_object[0].curframe >= FISHY_FRAMES)
                        fish_object[0].curframe = 0;
                    fish_object[0].endframe =
                        frame + fishy[fish_object[0].curframe].frames;
                    if (fish_object[0].endframe > FRAME_MAX)
                        fish_object[0].endframe -= FRAME_MAX;
                }
            }
            watchdog = 0;
        } else {
            watchdog++;
        }


        CheckCollision(fishies, &fish_left, frame, verbose);
        CheckFishies(&fishies, &splats);

        if (splats > 0) {       //draw splat permanent!
            for (i = 0; i < splats; i++) {
                if (splat_object[i].alive == YES) {
                    DrawObject(splat[splat_object[i].curframe].image,
                               splat_object[i].x, splat_object[i].y);
                    if (frame == splat_object[i].endframe) {
                        splat_object[i].curframe++;
                        if (splat_object[i].curframe >= SPLAT_FRAMES) {
                            splat_object[i].curframe = SPLAT_FRAMES - 1;
                        }
                        splat_object[i].endframe = frame + splat[0].frames;
                        if (splat_object[i].endframe > FRAME_MAX)
                            splat_object[i].endframe -= FRAME_MAX;
                    }
                }
            }
        }

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }


    /*
     * Wait a while- give screen chance to come up (if fullscreen) 
     */
    //SDL_Delay(3 *WAIT_MS);

    FreeGame(verbose, 1);       //free the game stuff, no music

    for (i = 0; i < PKCASCADE_TUT_SCREENS; i++)
        SDL_FreeSurface(TutScreen[i]);

    if (verbose == YES)
        fprintf(stderr, "->PractceKCascade(): END TUTORIAL\n");

    numupdates = 0;

    return (still_playing);
}

/******************************************
  WordSpawnFishies -
   Will spawn new fishes for WordCascade if
   there is a need for more fishies
*******************************************/
void WordSpawnFishies(int diflevel, int *fishies, int *local_max_fishies,
                      int *frame, int verbose, char *a_word)
{
    double          phase,
                    angle;
    int             spd,
                    framebay,
                    terminator,
                    i,
                    xamp,
                    fish_spacing,
                    width;

    if (diflevel == EASY) {
        xamp = MAX_AMPLITUDE_EASY;
        spd = MAX_SPEED_VARIANCE_EASY;
        phase = MAX_PHASE_EASY;
        angle = MAX_ANGLE_EASY;
        fish_spacing = FISH_SPACING_EASY;
    } else if (diflevel == MEDIUM) {
        xamp = MAX_AMPLITUDE_MEDIUM;
        spd = MAX_SPEED_VARIANCE_MEDIUM;
        phase = MAX_PHASE_MEDIUM;
        angle = MAX_ANGLE_MEDIUM;
        fish_spacing = FISH_SPACING_MEDIUM;
    } else {
        xamp = MAX_AMPLITUDE_HARD;
        spd = MAX_SPEED_VARIANCE_HARD;
        phase = MAX_PHASE_HARD;
        angle = MAX_ANGLE_HARD;
        fish_spacing = FISH_SPACING_HARD;
    }

    //printf("-%s- with a length of %i\n", a_word, strlen(a_word));

    //printf("%i w/ total %i\n", *fishies, *local_max_fishies);

    if (*fishies < *local_max_fishies) {
        if (*fishies == 0) {

            if (verbose == YES)
                fprintf(stderr, "=>Spawning first fishy\n");

            word_object[*fishies].alive = YES;
            word_object[*fishies].y = 1;
            word_object[*fishies].dy =
                DEFAULT_SPEED * ((rand() % spd) + 1);
            word_object[*fishies].x_amplitude = rand() % xamp;
            word_object[*fishies].x_phase =
                (double) (phase * rand() / (RAND_MAX + 1.0));
            word_object[*fishies].x_angle_mult =
                (double) (angle * rand() / (RAND_MAX + 1.0));
            word_object[*fishies].word = a_word;
            word_object[*fishies].curframe = 0;
            word_object[*fishies].eaten = NO;

            width = strlen(a_word);

            word_object[*fishies].x0 =
                (xamp / 2) + rand() % (screen->w -
                                       (width * fishy[0].image->w +
                                        (xamp / 2)));
            word_object[*fishies].x = word_object[*fishies].x0;

            framebay = fishy[0].frames + *frame;
            if (framebay > FRAME_MAX)
                framebay -= FRAME_MAX;
            word_object[*fishies].endframe = framebay;
            *fishies = *fishies + 1;
        } else {
            terminator = 0;
            for (i = 0; i < *fishies; i++)
                if (word_object[i].y < (fishy[0].image->h + fish_spacing))
                    terminator++;       //wait for fishies to move one full length before spawning another to ensure clarity

            if (terminator == 0) {

                if (verbose == YES)
                    fprintf(stderr, "=>Spawning fishy #%i\n", *fishies);

                word_object[*fishies].alive = YES;
                word_object[*fishies].y = 1;
                word_object[*fishies].dy =
                    DEFAULT_SPEED * ((rand() % spd) + 1);
                word_object[*fishies].x_amplitude = rand() % xamp;
                word_object[*fishies].x_phase =
                    (double) (phase * rand() / (RAND_MAX + 1.0));
                word_object[*fishies].x_angle_mult =
                    (double) (angle * rand() / (RAND_MAX + 1.0));;
                word_object[*fishies].word = a_word;
                word_object[*fishies].curframe = 0;
                word_object[*fishies].eaten = NO;

                width = strlen(a_word);

                word_object[*fishies].x0 =
                    (xamp / 2) + rand() % (screen->w -
                                           (width * fishy[0].image->w +
                                            (xamp / 2)));
                word_object[*fishies].x = word_object[*fishies].x0;

                //printf("%i\n", word_object[*fishies].x);

                framebay = fishy[0].frames + *frame;
                if (framebay > FRAME_MAX)
                    framebay -= FRAME_MAX;
                word_object[*fishies].endframe = framebay;
                *fishies = *fishies + 1;
            }
        }
    }
}

/******************************************
  WordMoveFishies - Move them fishies
*******************************************/
void WordMoveFishies(int *fishies, int *splats, int *lifes, int *frame)
{
    int             i,
                    t,
                    j,
                    this_letter,
                    draw_alpha = NO,
                    dummy_lives = 0;
    int             sx,
                    sy;

    for (i = 0; i < *fishies; i++) {
        if (word_object[i].alive == YES) {      //only want living fish

            for (t = 0; t <= strlen(word_object[i].word) - 2; t++) {
                EraseObject(fishy[word_object[i].curframe].image, (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)), word_object[i].y);        //erase fishy
            }

            word_object[i].y += word_object[i].dy;
            word_object[i].x =
                word_object[i].x0 +
                (word_object[i].x_amplitude) *
                sin((word_object[i].x_angle_mult) * word_object[i].y +
                    word_object[i].x_phase);

            if (word_object[i].y >=
                (screen->h) - fishy[word_object[i].curframe].image->h -
                1) {
                word_object[i].alive = NO;
                for (t = 0; t <= strlen(word_object[i].word) - 2; t++) {
                    sx = (word_object[i].x +
                          (t * fishy[word_object[i].curframe].image->w)) +
                        ((fishy[word_object[i].curframe].image->w) / 2) -
                        ((splat[0].image->w) / 2);
                    if (sx < 0)
                        sx = 0;
                    if (sx > (screen->w - splat[0].image->w))
                        sx = screen->w - splat[0].image->w - 1;
                    sy = screen->h - splat[0].image->h - 1;

                    AddSplat(splats, sx, sy, &dummy_lives, frame);
                }

                *lifes = *lifes - 1;

                //play splatting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
                }

            }

            for (t = 0; t <= strlen(word_object[i].word) - 2; t++) {
                /* This is intended to be optimized for speed, and might not look pretty */
                if(use_alpha == YES) {
                    draw_alpha = NO;
                    for(j = *fishies; j > i; --j) {
                        if(word_object[j].alive == YES) {
                            if( ((word_object[j].y + fishy[word_object[j].curframe].image->h) >= word_object[i].y) &&
                             (word_object[j].y < (word_object[i].y + fishy[word_object[i].curframe].image->h)) )
                            {
//                                printf("Alpha fishy drawn!\n");
                                j = i;
                                draw_alpha = YES;
                            }
                        }
                    }
                }
                if(draw_alpha == YES) {
                    DrawObject(fishy_alpha[word_object[i].curframe].image,
                        (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)),
                        word_object[i].y); //draw fishy
                } else {
                    DrawObject(fishy[word_object[i].curframe].image,
                        (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)),
                        word_object[i].y); //draw fishy
//                    printf("Non-alpha, outside fishy drawn!\n");
                }

                if ((word_object[i].word[t] <= *"z") &
                    (word_object[i].word[t] >= *"a")) {
                    this_letter = word_object[i].word[t] - *"a";

                    if(draw_alpha == YES) {
                        DrawObject(letter_alpha[this_letter],
                            (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)),
                            word_object[i].y);      //draw letter overlay
                    } else {
                        DrawObject(letter[this_letter],
                            (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)),
                            word_object[i].y);      //draw letter overlay
                    }
                }
            }

            if (*frame == word_object[i].endframe) {
                word_object[i].curframe++;
                if (word_object[i].curframe >= FISHY_FRAMES)
                    word_object[i].curframe = 0;
                word_object[i].endframe =
                    *frame + fishy[word_object[i].curframe].frames;
                if (word_object[i].endframe > FRAME_MAX)
                    word_object[i].endframe -= FRAME_MAX;
            }
        }
    }

    for (i = 0; i < *splats; i++) {
        if (splat_object[i].alive == YES) {
            if (*frame == splat_object[i].endframe) {
                splat_object[i].curframe++;
                if (splat_object[i].curframe >= SPLAT_FRAMES) {
                    splat_object[i].curframe = 0;
                    splat_object[i].alive = NO;
                    EraseObject(splat[splat_object[i].curframe].image,
                                splat_object[i].x, splat_object[i].y);
                }
                splat_object[i].endframe = *frame + splat[0].frames;
                if (splat_object[i].endframe > FRAME_MAX)
                    splat_object[i].endframe -= FRAME_MAX;
            } else
                DrawObject(splat[splat_object[i].curframe].image,
                           splat_object[i].x, splat_object[i].y);
        }
    }
}

/************************************
  WordCovertKeyPress - see bellow
************************************/
char WordConvertKeyPress(Uint8 * keys)
{
    // convert the Uint8 SDL key press to something we can use in our array
    //This routine must be modified if ever internationalized
    //---- BTW, this is going to be very inefficient and ugly... to be cleaned up later
    if (keys[SDLK_a] == SDL_PRESSED)
        return *"a";
    else if (keys[SDLK_b] == SDL_PRESSED)
        return *"b";
    else if (keys[SDLK_c] == SDL_PRESSED)
        return *"c";
    else if (keys[SDLK_d] == SDL_PRESSED)
        return *"d";
    else if (keys[SDLK_e] == SDL_PRESSED)
        return *"e";
    else if (keys[SDLK_f] == SDL_PRESSED)
        return *"f";
    else if (keys[SDLK_g] == SDL_PRESSED)
        return *"g";
    else if (keys[SDLK_h] == SDL_PRESSED)
        return *"h";
    else if (keys[SDLK_i] == SDL_PRESSED)
        return *"i";
    else if (keys[SDLK_j] == SDL_PRESSED)
        return *"j";
    else if (keys[SDLK_k] == SDL_PRESSED)
        return *"k";
    else if (keys[SDLK_l] == SDL_PRESSED)
        return *"l";
    else if (keys[SDLK_m] == SDL_PRESSED)
        return *"m";
    else if (keys[SDLK_n] == SDL_PRESSED)
        return *"n";
    else if (keys[SDLK_o] == SDL_PRESSED)
        return *"o";
    else if (keys[SDLK_p] == SDL_PRESSED)
        return *"p";
    else if (keys[SDLK_q] == SDL_PRESSED)
        return *"q";
    else if (keys[SDLK_r] == SDL_PRESSED)
        return *"r";
    else if (keys[SDLK_s] == SDL_PRESSED)
        return *"s";
    else if (keys[SDLK_t] == SDL_PRESSED)
        return *"t";
    else if (keys[SDLK_u] == SDL_PRESSED)
        return *"u";
    else if (keys[SDLK_v] == SDL_PRESSED)
        return *"v";
    else if (keys[SDLK_w] == SDL_PRESSED)
        return *"w";
    else if (keys[SDLK_x] == SDL_PRESSED)
        return *"x";
    else if (keys[SDLK_y] == SDL_PRESSED)
        return *"y";
    else if (keys[SDLK_z] == SDL_PRESSED)
        return *"z";
    else
        return *" ";
}


/******************************************
  WordUpdateTux -
    Update and move tux as dictated in the
    Word Cascade Section of the game
******************************************/
void WordUpdateTux(char *letter_pressed, int fishies, int frame)
{
    int             i,
                    termin;
    double          time_to_hit = 100 * screen->h;

    //If the word length in Tux's queue is too long,
    //we must cycle it back down (too bad I didn't do
    //this game in Perl with the Perl/SDL bindings..
    //cuz in Perl, this would be a piece of cake ;)

    for (i = 0; i < MAX_WORD_SIZE; i++) {
        tux_object.word[i] = tux_object.word[i + 1];
    }

    tux_object.word[MAX_WORD_SIZE] = letter_pressed[0];

    termin = NOT_A_LETTER;

    for (i = 0; i < fishies; i++) {
        if (word_object[i].alive == YES) {      //just in case I screwed up elsewhere
            //printf("Comparing \"%s\" to %s w/ \"%s\"\n",  tux_object.word, word_object[i].word, &tux_object.word[strlen(tux_object.word) - strlen(word_object[i].word) + 1]);
            //printf("%i\n", strncmp(word_object[i].word, &tux_object.word[strlen(tux_object.word) - strlen(word_object[i].word) + 1], strlen(word_object[i].word) - 1));
            if (strncmp
                (word_object[i].word,
                 &tux_object.word[MAX_WORD_SIZE + 1 -	/* tux_object.word is defined as char word[MAX_WORD_SIZE + 1], strlen will count until it hits a NULL */
                                  strlen(word_object[i].word) + 1],
                 strlen(word_object[i].word) - 1) == 0) {
                //printf("Words Match! %s to %s\n", tux_object.word, word_object[i].word);
                if (((screen->h - word_object[i].y) / word_object[i].dy) < time_to_hit) {       //go to nearest letter
                    //time_to_hit = (fish_object[i].dy / (screen->h - fish_object[i].y));
                    //time_to_hit = ((screen->h - fish_object[i].y - fishy[fish_object[i].curframe].image->h) / fish_object[i].dy);
                    time_to_hit =
                        ((screen->
                          h - word_object[i].y -
                          tuxGulp[0].image[0]->h) / word_object[i].dy);
                    //fprintf(stderr, "time to hit : %f\n\n", time_to_hit);
                    tux_object.endx =
                        word_object[i].x0 +
                        ((fishy[word_object[i].curframe].image->w) / 2) -
                        ((tuxGulp[0].image[RIGHT]->w) / 2);

                    if (tux_object.endx < 0)
                        tux_object.endx = 0;
                    if (tux_object.endx >= (screen->w - tux_max_width))
                        tux_object.endx = screen->w - tux_max_width - 1;

                    termin = NOT_A_LETTER + 1;
                    word_object[i].eaten = YES;
                }

            }
        }
    }
    if (termin != NOT_A_LETTER) {
        if (tux_object.endx >= tux_object.x)
            tux_object.facing = RIGHT;
        else
            tux_object.facing = LEFT;

        //the following is needed to fully erase a running tux if state is changed during run
        //this is needed because of the speeds at which tux can run...
        //if his walking speed is ever increased (constant definition in playgame.h), then something
        //similar may be required for walking....               
        if (tux_object.frametype == RUNNING)
            RealEraseObject(tuxRun[tux_object.curframe].
                            image[tux_object.facing], tux_object.x,
                            tux_object.y);

        if (time_to_hit > (abs(tux_object.endx - tux_object.x) / WALKING_SPEED)) {      //we have time to walk to eat fishy
            tux_object.dx = WALKING_SPEED;
            tux_object.frametype = WALKING;
            tux_object.curframe = 0;
            tux_object.endframe = tuxWalk[0].frames + frame;
            if (tux_object.endframe > FRAME_MAX)
                tux_object.endframe -= FRAME_MAX;

            //stop running sound (if playing)                                               

            if (sys_sound == YES)
                if (Mix_Playing(RUN_WAV)) {
                    Mix_HaltChannel(RUN_WAV);
                }

        } else {                //woah, nelly, we'z gots'ta run!
            tux_object.dx =
                abs(tux_object.endx - tux_object.x) / time_to_hit;
            //fprintf(stderr, "%f\t%i\n", tux_object.dx, abs(tux_object.endx - tux_object.x));
            if (tux_object.dx < MIN_RUNNING_SPEED)
                tux_object.dx = MIN_RUNNING_SPEED;
            else if (tux_object.dx > MAX_RUNNING_SPEED)
                tux_object.dx = MAX_RUNNING_SPEED;      //it is possible for fishy to be too far away
            tux_object.frametype = RUNNING;
            //fprintf(stderr, "after : %f\n", tux_object.dx);
            tux_object.curframe = 0;
            tux_object.endframe = tuxRun[0].frames + frame;
            if (tux_object.endframe > FRAME_MAX)
                tux_object.endframe -= FRAME_MAX;

            //play running sound
            if (sys_sound == YES) {
                Mix_PlayChannel(RUN_WAV, sound[RUN_WAV], 0);
            }
        }
    }

}

/********************************
 WordCheckCollision : Check
 collisions between Tux and Word
 objects (groups of fishies).
 Different from CheckCollisions,
 which only dealt with single
 letter fishies. If collide,
 perform appropriate acction
*********************************/
void WordCheckCollision(int fishies, int *fish_left, int frame,
                        int verbose)
{
    int             i,
                    framesy,
                    t;

    rl_object      *temp;

    if (tux_object.frametype == WALKING) {
        temp = tuxWalk;
        framesy = WALK_FRAMES;
    } else if (tux_object.frametype == STANDING) {
        temp = tuxStand;

        framesy = STAND_FRAMES;
    } else if (tux_object.frametype == RUNNING) {
        temp = tuxRun;
        framesy = RUN_FRAMES;
    } else if (tux_object.frametype == GULPING) {
        temp = tuxGulp;
        framesy = GULP_FRAMES;
    } else if (tux_object.frametype == WINNING) {
        temp = tuxWin;
        framesy = WIN_FRAMES;
    } else {
        temp = tuxYipe;
        framesy = YIPE_FRAMES;
    }

    for (i = 0; i < fishies; i++) {
        if ((word_object[i].x >= tux_object.x
             && word_object[i].x <=
             tux_object.x +
             temp[tux_object.curframe].image[tux_object.facing]->w)
            && (word_object[i].y >=
                tux_object.y - fishy[word_object[i].curframe].image->h)) {

            if (word_object[i].eaten == YES) {  // then eat the fishy!
                if (verbose == YES)
                    fprintf(stderr,
                            "**EATING A FISHY** - in CheckCollision()\n");

                word_object[i].alive = NO;
                word_object[i].eaten = NO;

                for (t = 0; t <= strlen(word_object[i].word) - 2; t++) {
                    // I don't think I fixed it this way in the keycascade, but
                    // it seems to work, and its late and I have to get up early ;)
                    RealEraseObject(fishy[word_object[i].curframe].image, (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)), word_object[i].y);        //erase fishy
                }

                if (tux_object.frametype == RUNNING)
                    RealEraseObject(tuxRun[tux_object.curframe].image[tux_object.facing], tux_object.x, tux_object.y);  //erase present tux, as well (prep for gulping)

                *fish_left = *fish_left - 1;
                tux_object.frametype = GULPING;
                tux_object.curframe = 0;
                tux_object.dx = 0;
                tux_object.endframe = tuxGulp[0].frames + frame;
                if (tux_object.endframe > FRAME_MAX)
                    tux_object.endframe -= FRAME_MAX;

                //play biting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(BITE_WAV, sound[BITE_WAV], 0);
                }

            }
            // This doesn't work, to either fix in future or remove altogether! // else if(tux_object.frametype == STANDING) {
            // tux will want to move out of the way if he's not eating the fishy
            //if(verbose == YES)
            //fprintf(stderr, "***EXCUSE ME!** - in CheckCollision()\n");

            //tux_object.endx = fish_object[i].x + (fishy[word_object[i].curframe].image->w) * (strlen(word_object[i].word)-1);
            //tux_object.facing = RIGHT;
            //if(tux_object.endx >= (screen->w - tux_max_width)) {
            //tux_object.endx = word_object[i].x - tux_max_width;
            //tux_object.facing = LEFT;
            //}
            //tux_object.dx = WALKING_SPEED;
            //tux_object.frametype = WALKING;
            //tux_object.curframe = 0;
            //tux_object.endframe = tuxWalk[0].frames + frame;
            //if(tux_object.endframe > FRAME_MAX)
            //tux_object.endframe -= FRAME_MAX;

            //play excusing sound
            //if(sys_sound == YES) {
            //Mix_PlayChannel(EXCUSEME_WAV, sound[EXCUSEME_WAV], 0);
            //}

            //}
        }
    }
}

/******************************************
  WordCheckFishies -
    Check on the fishies and clear away the
    dead ones (mu-wah-hah-hah-haaaahhhh...)
*******************************************/
void WordCheckFishies(int *fishies, int *splats)
{
    //sort the splats and fishies
    int             stemp = 0;
    int             ftemp = 0;
    int             tcap;
    int             i;

    struct wordypoo fish_temp[MAX_FISHIES_HARD];
    struct splatter splat_temp[MAX_FISHIES_HARD];

    //sort the fish_object[] and splat_object[] lists so that all alive
    //objects are first in arrays
    //This is rather inefficient and slow, but should be fine for the small ammount
    //of onscreen fishes we're working with
    tcap = *fishies;

    if (tcap < *splats)
        tcap = *splats;

    for (i = 0; i <= tcap; i++) {
        if (word_object[i].alive == YES) {
            fish_temp[ftemp] = word_object[i];
            ftemp++;
        }
        if (splat_object[i].alive == YES) {
            splat_temp[stemp] = splat_object[i];
            stemp++;
        }
    }
    for (i = ftemp; i < MAX_FISHIES_HARD; i++) {
        fish_temp[i].alive = NO;
    }
    for (i = stemp; i < MAX_FISHIES_HARD; i++) {
        splat_temp[i].alive = NO;
    }

    for (i = 0; i < MAX_FISHIES_HARD; i++) {

        word_object[i] = fish_temp[i];
        splat_object[i] = splat_temp[i];
    }

    *fishies = ftemp;
    *splats = stemp;
}

/******************************************
  PlayWCascade -
   Play WordCascade. Actual game loop
*******************************************/
int PlayWCascade(int diflevel, int verbose)
{
    // play word cascade
    FILE           *fi;
    char            filename[FNLEN];
    char            words[NUM_WORDS][MAX_WORD_SIZE];
    char            temp[FNLEN],
                    temp_word[MAX_WORD_SIZE];
    int             wordnum = 0;
    int             numwords = 0;
    int             still_playing = YES;
    int             playing_level = YES;
    int             resume = YES;
    int             quitting = NO;
    int             curlevel = 0;
    int             t,
                    i,
                    j,
                    notword = NO;
    int             curlives;
    int             oldlives,
                    oldfish_left;
    int             fish_left,
                    fishies = 0,
                    local_max_fishies;
    int             frame = 0;
    int             done_frames;        //, congrats_endframe, oh_no_endframe;
    int             splats = 0;
    char            letter_pressed,
                    old_letter_pressed;
    SDL_Event       event;
    SDL_Surface    *temp_text[CONGRATS_FRAMES + OH_NO_FRAMES];
    SDL_Rect        text_rect;
    int             text_y_end;
    int             xamp,
                    yamp,
                    x_not,
                    y_not;
    rl_object      *temp_tux;
    int             temp_text_frames,
                    temp_tux_frames;
    int             temp_text_count,
                    temp_tux_count;
    Uint8          *keys;
    char            DATAPATH[FNLEN];
    int             local_use_theme = YES;

    if (verbose == YES)
        fprintf(stderr, "->PlayWCascade: level=%i\n", diflevel);

    srand(time(NULL));
    for (i = 0; i <= MAX_WORD_SIZE; i++) {
        tux_object.word[i] = *" ";
    }

    SDL_WarpMouse(screen->w / 2, screen->h / 2);

    //no cursor please
    SDL_ShowCursor(0);

    LoadTuxAnims(verbose);
    LoadFishies(verbose);
    LoadOthers(verbose);
    /*
     * Set up the update rectangle pointers 
     */
    for (i = 0; i < MAX_UPDATES; ++i) {
        blits[i].srcrect = &srcupdate[i];
        blits[i].dstrect = &dstupdate[i];
    }

    while (still_playing == YES) {
        //set up the number for fish based on diflevel
        if (diflevel == EASY) {
            fish_left = MAX_FISHIES_EASY;
            curlives = EASY_LIVES_INIT;
        } else if (diflevel == MEDIUM) {
            fish_left = MAX_FISHIES_MEDIUM;
            curlives = MEDIUM_LIVES_INIT;
        } else {
            fish_left = MAX_FISHIES_HARD;
            curlives = HARD_LIVES_INIT;
        }

        local_max_fishies = fish_left;

        sprintf(filename, "wcas%i_%i.png", diflevel + 1, curlevel + 1);

        /********************************************************
        * Special Hidden Code                                  *
        * See README file in images directory for info         *
        ********************************************************/

        if (hidden == YES && curlevel == 3) {
            sprintf(filename, "hidden.png");
        }

        /********************************************************/

        if (verbose == YES)
            fprintf(stderr, "->>Loading background: %s\n", filename);
        background = LoadImage(filename, 0, verbose);

        UpdateBackground(verbose);

        //do any pre-game animation here
        ClearObjects(verbose);

        //load music file
        if (sys_sound == YES) {
            sprintf(filename, "kmus%i.wav", curlevel + 1);
            LoadMusic(filename, verbose);
        }

        i = 0;

        if (verbose == YES)
            fprintf(stderr, "->>Loading word file #%i\n", curlevel + 1);

        //load words for this curlevel
        strcpy(DATAPATH, WORD_PATH[0]);

        if (use_theme == YES) {
            sprintf(filename, "%s/data/words%i.txt", alt_theme_path,
                    curlevel + 1);
            i = -1;             /* This is necessary in case the word file is not there */
        } else {
            sprintf(filename, "%s/data/words%i.txt", DATAPATH,
                    curlevel + 1);
        }

        if (verbose == YES)
            fprintf(stderr, "OPEN_WORD_FILE: Trying %s\n", filename);

        /*
         * Try opening the file: 
         */

        fi = fopen(filename, "r");

        while (fi == NULL) {
            i++;
            if (i >= PATHNUM) {
                if (local_use_theme == YES && use_theme == YES) {
                    local_use_theme = NO;
                    i = 0;
                } else {
                    fprintf(stderr, "***ERROR LOADING WORD FILE!****\n");
                    exit(2);
                }
            }

            strcpy(DATAPATH, WORD_PATH[i]);
            if (use_theme == YES && local_use_theme == YES) {
                sprintf(filename, "%s/%s/data/words%i.txt", DATAPATH,
                        alt_theme_path, curlevel + 1);
            } else {
                sprintf(filename, "%s/data/words%i.txt", DATAPATH,
                        curlevel + 1);
            }

            if (verbose == YES)
                fprintf(stderr, "OPEN_WORD_FILE: Trying %s\n", filename);

            fi = fopen(filename, "r");
        }

        if (verbose == YES)
            fprintf(stderr, "WORD FILE OPENNED @ %s\n", filename);

        if (fi != NULL) {
            do {
                fgets(temp, sizeof(temp), fi);

                if (verbose == YES)
                    fprintf(stderr, "%i: %s\n", i, temp);

                if (strstr(temp_word, "#") == NULL) {   //# is comment
                    notword = NO;
                    if (strlen(temp) > 1 && strlen(temp) < MAX_WORD_SIZE) {
                        //make sure we have something
                        //make sure not a comment or contains invalid characters
                        if (notword == NO) {
                            for (j = 0; j < TOT_BAD_CHARS; j++) {
                                if (strchr(temp, BAD_CHARS[j]) != NULL) {
                                    //we have an invalid char, make line a comment
                                    notword = YES;
                                }
                            }
                            if (wordnum <= NUM_WORDS) {
                                strcpy(words[wordnum], temp);
                                numwords = wordnum;
                                wordnum++;
                            } else
                                fprintf(stderr,
                                        "Error in word file #%i on line %i\n-Too many words in file... Word count exceeds NUM_WORDS (%i)\n",
                                        curlevel + 1, i, NUM_WORDS);
                        } else
                            fprintf(stderr,
                                    "Error in word file #%i on line %i\n-Remember to use # to denote comment lines\n",
                                    curlevel + 1, i);
                    }
                }
                i++;
            }
            while (!feof(fi));
        }

        fclose(fi);

        if (verbose == YES)
            fprintf(stderr, "->>PLAYING THE GAME\n");

        i = 0;

        while (playing_level == YES) {
            /*
             * Poll input queue, run keyboard loop 
             */
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    resume = NO;
                    playing_level = NO;
                    still_playing = NO;
                    quitting = YES;
                }
            }

            keys = SDL_GetKeyState(NULL);

            if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
                resume = YES;
                playing_level = NO;
                still_playing = NO;
                quitting = YES;
            }

            if (keys[SDLK_PAUSE] == SDL_PRESSED || keys[SDLK_SPACE] == SDL_PRESSED) {   //pause the game
                if (Pause(verbose) == NO) {
                    playing_level = NO;
                    resume = NO;
                    still_playing = NO;
                    quitting = YES;
                }
            }

            oldlives = curlives;
            oldfish_left = fish_left;

            WordSpawnFishies(diflevel, &fishies, &local_max_fishies,
                             &frame, verbose, words[rand() % numwords]);

            old_letter_pressed = letter_pressed;

            letter_pressed = WordConvertKeyPress(keys);

            if (letter_pressed != *" ") {
                if (old_letter_pressed != letter_pressed) {
                    // We have to have a pause between keys
                    //luckily computers are so fast the user SHOULDN'T
                    //notice this
                    if (verbose == YES)
                        fprintf(stderr,
                                "-In PlayGame(): Key Just Pressed\n");
                    WordUpdateTux(&letter_pressed, fishies, frame);
                }
            }
            MoveTux(frame, verbose);

            WordMoveFishies(&fishies, &splats, &curlives, &frame);
            WordCheckFishies(&fishies, &splats);

            WordCheckCollision(fishies, &fish_left, frame, verbose);

            //update top score/info bar
            DrawObject(level[diflevel], 1, 1);
            DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w, 1);

            DrawNumbers(curlevel + 1,
                        1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                        curlev->w, 1, 0);
            DrawObject(lives,
                       (screen->w) - (1 + lives->w + fish->w +
                                      ((MAX_FISHIES_DIGITS + 1) * 2 *
                                       number_max_w) + GRAPHIC_SPACE), 1);

            if (oldlives != curlives) {
                EraseNumbers(oldlives,
                             (screen->w) - (1 + fish->w +
                                            ((MAX_FISHIES_DIGITS + 1) * 2 *
                                             number_max_w) +
                                            GRAPHIC_SPACE), 1, 0);
                oldlives = curlives;
            }

            DrawNumbers(curlives,
                        (screen->w) - (1 + fish->w +
                                       ((MAX_FISHIES_DIGITS + 1) * 2 *
                                        number_max_w) + GRAPHIC_SPACE), 1,
                        0);
            DrawObject(fish,
                       (screen->w) - (1 + fish->w +
                                      (MAX_FISHIES_DIGITS * number_max_w)),
                       1);

            if (oldfish_left != fish_left) {
                EraseNumbers(oldfish_left,
                             (screen->w) - (1 +
                                            (MAX_FISHIES_DIGITS *
                                             number_max_w)), 1,
                             MAX_FISHIES_DIGITS);
                oldfish_left = fish_left;
            }

            DrawNumbers(fish_left,
                        (screen->w) - (1 +
                                       (MAX_FISHIES_DIGITS *
                                        number[4]->w)), 1,
                        MAX_FISHIES_DIGITS);

            if (curlives <= 0) {
                playing_level = NO;
                still_playing = NO;
            }

            if (fish_left <= 0) {
                playing_level = NO;
                curlevel++;
                still_playing = YES;
            }

            UpdateScreen(&frame);
            if (speed_up == NO)
                WaitFrame();


            //play music if not already playing
            if (sys_sound == YES) {
                if (!Mix_PlayingMusic()) {
                    Mix_PlayMusic(music, 0);
                }
            }
        }

        if (verbose == YES)
            fprintf(stderr, "---In PlayGame(): Level Terminates\n");

        //stop music!
        if (sys_sound == YES)
            i = Mix_FadeOutMusic(MUSIC_FADE_OUT_MS);

        // I know very well what I'm about to do is cheesy, lazy, and dumb...
        // I know I need to go and move all this repeated code to a seperate
        // function... and this is something I'll do someday... but only
        // after I get the game running the way I want.....
        if (quitting == NO) {
            if (curlives <= 0) {
                //too bad here - game over
                done_frames = MAX_END_FRAMES_GAMEOVER;
                xamp = 0;
                yamp = 0;
                if (verbose == YES)
                    fprintf(stderr, "--->GAME OVER\n");

                //play losing sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(LOSE_WAV, sound[LOSE_WAV], 0);
                }
                //first, erase the fish
                for (i = 0; i < fishies; i++) {
                    if (word_object[i].alive == YES) {  //only want living fish
                        for (t = 0; t <= strlen(word_object[i].word) - 2;
                             t++) {
                            EraseObject(fishy[word_object[i].curframe].image, (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)), word_object[i].y);    //erase fishy
                        }
                    }
                }

                //now put splat on background to ensure it stays
                for (i = 0; i < splats; i++) {
                    if (splat_object[i].alive == YES) {
                        text_rect.x = splat_object[i].x;
                        text_rect.y = splat_object[i].y;
                        text_rect.w =
                            splat[splat_object[i].curframe].image->w;
                        text_rect.h =
                            splat[splat_object[i].curframe].image->h;
                        SDL_BlitSurface(splat[splat_object[i].curframe].
                                        image, NULL, background,
                                        &text_rect);
                    }
                }

                //redraw top bar
                DrawObject(level[diflevel], 1, 1);
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel + 1,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);
                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                //set up "OH NO!" animation and tux_yipe!
                for (i = 0; i < OH_NO_FRAMES; i++)
                    temp_text[i] = ohno[i];
                temp_text_frames = OH_NO_FRAMES;
                temp_tux = tuxYipe;
                temp_tux_frames = YIPE_FRAMES;
            } else if (curlevel >= 4) {
                //HEY WE WON!
                done_frames = MAX_END_FRAMES_WIN_GAME;
                still_playing = NO;
                xamp = WIN_GAME_XAMP;
                yamp = WIN_GAME_YAMP;
                if (verbose == YES)
                    fprintf(stderr, "--->WINNER!\n");

                //play winning finale sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(WINFINAL_WAV, sound[WINFINAL_WAV], 0);
                    Mix_PlayChannel(WIN_WAV, sound[WIN_WAV], 0);
                }
                //first, erase the fish
                for (i = 0; i < fishies; i++) {
                    if (word_object[i].alive == YES) {  //only want living fish
                        for (t = 0; t <= strlen(word_object[i].word) - 2;
                             t++) {
                            EraseObject(fishy[word_object[i].curframe].image, (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)), word_object[i].y);    //erase fishy
                        }
                    }
                }

                //now put any splats left on background to ensure they stay
                for (i = 0; i < splats; i++) {
                    if (splat_object[i].alive == YES) {
                        text_rect.x = splat_object[i].x;
                        text_rect.y = splat_object[i].y;
                        text_rect.w =
                            splat[splat_object[i].curframe].image->w;
                        text_rect.h =
                            splat[splat_object[i].curframe].image->h;
                        SDL_BlitSurface(splat[splat_object[i].curframe].
                                        image, NULL, background,
                                        &text_rect);
                    }
                }

                //redraw top bar
                DrawObject(level[diflevel], 1, 1);
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);
                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                //set up congrats
                for (i = 0; i < CONGRATS_FRAMES; i++)
                    temp_text[i] = congrats[i];
                temp_text_frames = CONGRATS_FRAMES;
                temp_tux = tuxWin;
                temp_tux_frames = WIN_FRAMES;
            } else {
                //moving on to next level
                done_frames = MAX_END_FRAMES_BETWEEN_LEVELS;
                playing_level = YES;
                xamp = 0;
                yamp = 0;
                if (verbose == YES)
                    fprintf(stderr, "--->NEXT LEVEL!\n");

                //play winning sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(WIN_WAV, sound[WIN_WAV], 0);
                }
                //first, erase the fish
                for (i = 0; i < fishies; i++) {
                    if (word_object[i].alive == YES) {  //only want living fish
                        for (t = 0; t <= strlen(word_object[i].word) - 2;
                             t++) {
                            EraseObject(fishy[word_object[i].curframe].image, (word_object[i].x + (t * fishy[word_object[i].curframe].image->w)), word_object[i].y);    //erase fishy
                        }
                    }
                }

                //now put any splats left on background to ensure they stay
                for (i = 0; i < splats; i++) {
                    if (splat_object[i].alive == YES) {
                        text_rect.x = splat_object[i].x;
                        text_rect.y = splat_object[i].y;
                        text_rect.w =
                            splat[splat_object[i].curframe].image->w;
                        text_rect.h =
                            splat[splat_object[i].curframe].image->h;
                        SDL_BlitSurface(splat[splat_object[i].curframe].
                                        image, NULL, background,
                                        &text_rect);
                    }
                }

                //redraw top bar
                DrawObject(level[diflevel], 1, 1);
                DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w,
                           1);
                DrawNumbers(curlevel,
                            1 + 2 * GRAPHIC_SPACE + level[diflevel]->w +
                            curlev->w, 1, 0);
                DrawObject(lives,
                           (screen->w) - (1 + lives->w + fish->w +
                                          ((MAX_FISHIES_DIGITS + 1) * 2 *
                                           number_max_w) + GRAPHIC_SPACE),
                           1);

                if (oldlives != curlives) {
                    EraseNumbers(oldlives,
                                 (screen->w) - (1 + fish->w +
                                                ((MAX_FISHIES_DIGITS + 1) *
                                                 2 * number_max_w) +
                                                GRAPHIC_SPACE), 1, 0);
                    oldlives = curlives;
                }

                DrawNumbers(curlives,
                            (screen->w) - (1 + fish->w +
                                           ((MAX_FISHIES_DIGITS + 1) * 2 *
                                            number_max_w) + GRAPHIC_SPACE),
                            1, 0);
                DrawObject(fish,
                           (screen->w) - (1 + fish->w +
                                          (MAX_FISHIES_DIGITS *
                                           number_max_w)), 1);

                if (oldfish_left != fish_left) {
                    EraseNumbers(oldfish_left,
                                 (screen->w) - (1 +
                                                (MAX_FISHIES_DIGITS *
                                                 number_max_w)), 1,
                                 MAX_FISHIES_DIGITS);
                    oldfish_left = fish_left;
                }

                DrawNumbers(fish_left,
                            (screen->w) - (1 +
                                           (MAX_FISHIES_DIGITS *
                                            number[4]->w)), 1,
                            MAX_FISHIES_DIGITS);

                //set up congrats
                for (i = 0; i < CONGRATS_FRAMES; i++)
                    temp_text[i] = congrats[i];
                temp_text_frames = CONGRATS_FRAMES;
                temp_tux = tuxWin;
                temp_tux_frames = WIN_FRAMES;
            }

            i = 0;
            temp_text_count = 0;
            temp_tux_count = 0;
            text_y_end = (screen->h / 2) - (temp_text[0]->h / 2);
            text_rect.x = (screen->w / 2) - (temp_text[0]->w / 2);
            text_rect.y = screen->h - temp_text[0]->h - 1;
            x_not = text_rect.x;

            if (verbose == YES)
                fprintf(stderr, "--->Starting Ending Animation\n");

            while (i <= done_frames) {  // display ending animation!
                EraseObject(temp_tux[temp_tux_count].
                            image[tux_object.facing], tux_object.x,
                            tux_object.y);
                EraseObject(temp_text[temp_text_count], text_rect.x,
                            text_rect.y);
                EraseObject(temp_text[temp_text_count], text_rect.x,
                            text_rect.y - 1);

                temp_tux_count++;
                if (temp_tux_count >= temp_tux_frames)
                    temp_tux_count = 0;

                temp_text_count++;
                if (temp_text_count >= temp_text_frames)
                    temp_text_count = 0;

                text_rect.y -= END_FRAME_DY;
                y_not = text_rect.y;

                if (text_rect.y < text_y_end) {
                    y_not =
                        text_y_end + yamp * sin(i / WIN_GAME_ANGLE_MULT);
                    text_rect.y = text_y_end;
                    text_rect.x =
                        x_not + xamp * cos(i / WIN_GAME_ANGLE_MULT);
                }

                DrawObject(temp_tux[temp_tux_count].
                           image[tux_object.facing], tux_object.x,
                           tux_object.y);
                DrawObject(temp_text[temp_text_count], text_rect.x, y_not);

                UpdateScreen(&frame);
                if (speed_up == NO)
                    WaitFrame();
                i++;
            }
        }
    }

    if (verbose == YES)
        fprintf(stderr, "->Done with level... cleaning up\n");

    /*
     * Wait a while- give screen chance to come up (if fullscreen) 
     */
    SDL_Delay(WAIT_MS);

    FreeGame(verbose, 0);       //free the game stuff

    return (resume);
}

/*****************************
/* PracticeWCascade -
 * The Word Cascade Tutorial
******************************/
int PracticeWCascade(int verbose)
{
    //Give a little tutorial on playing Word Cascade
    int             still_playing = YES,
                    exit_tut = NO;
    int             i,
                    tut_frame = 0,
                    watchdog,
                    t;
    char            filename[FNLEN];
    int             fish_left = 1,
                    fishies = 0,
                    lifes = 100;
    int             frame = 0,
                    framebay;
    int             splats = 0;
    int             sx,
                    sy;
    SDL_Event       event;
    Uint8          *keys;
    SDL_Surface    *TutScreen[PKCASCADE_TUT_SCREENS];

    if (verbose == YES)
        fprintf(stderr, "->PractceWCascade(): START TUTORIAL\n");

    SDL_WarpMouse(screen->w / 2, screen->h / 2);

    //no cursor please
    SDL_ShowCursor(0);

    //we may not use everything we're about to load, but rather than
    //making seperate loading routines for practice, we'll just use these
    LoadTuxAnims(verbose);
    LoadFishies(verbose);
    LoadOthers(verbose);
    /*
     * Set up the update rectangle pointers 
     */
    for (i = 0; i < MAX_UPDATES; ++i) {
        blits[i].srcrect = &srcupdate[i];
        blits[i].dstrect = &dstupdate[i];
    }

    sprintf(filename, "tutor.png");
    if (verbose == YES)
        fprintf(stderr, "->>Loading background: %s\n", filename);
    background = LoadImage(filename, 0, verbose);

    UpdateBackground(verbose);

    //clear everything
    ClearObjects(verbose);

    //load the Tutorial Screens
    for (i = 0; i < PKCASCADE_TUT_SCREENS; i++) {
        sprintf(filename, "wtutor%i.png", i + 1);
        TutScreen[i] = LoadImage(filename, 1, verbose);
    }

    //BEGIN THE TUTORIAL - FIRST TUT-FRAME, INTRO AND TUX STANDING THERE
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 1\n");

    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    watchdog = 0;
    //place tux & fishy
    tux_object.x = (TutScreen[0]->w / 2) - tuxStand[0].image[RIGHT]->w / 2;
    tux_object.y =
        screen->h - tuxStand[0].image[RIGHT]->h - 1 - PRACT_TUX_Y;
    tux_object.dx = 0;
    tux_object.dy = 0;
    tux_object.endx = tux_object.x;
    tux_object.endy = tux_object.y;
    tux_object.letter = NOT_A_LETTER;

    fish_object[fishies].alive = YES;
    fish_object[fishies].y = 1;
    fish_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w / 2;
    fish_object[fishies].x = fish_object[fishies].x0;
    fish_object[fishies].dy = DEFAULT_TUT1_FISH_SPEED;
    fish_object[fishies].x_amplitude = 0;
    fish_object[fishies].x_phase = 0;
    fish_object[fishies].x_angle_mult = 0;
    fish_object[fishies].letter = NOT_A_LETTER;
    fish_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    fish_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        if (fishies > 0) {
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy

            fish_object[0].y += fish_object[0].dy;

            DrawObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);       //draw fishy

            if (frame == fish_object[0].endframe) {
                fish_object[0].curframe++;
                if (fish_object[0].curframe >= FISHY_FRAMES)
                    fish_object[0].curframe = 0;
                fish_object[0].endframe =
                    frame + fishy[fish_object[0].curframe].frames;
                if (fish_object[0].endframe > FRAME_MAX)
                    fish_object[0].endframe -= FRAME_MAX;
            }
            watchdog = 0;
        } else {
            watchdog++;
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy
        }

        CheckCollision(fishies, &fish_left, frame, verbose);
        CheckFishies(&fishies, &splats);

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }

    //SECOND TUT-FRAME, MOVE TUX, introduce real fishy w/ letter and eat it
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 2\n");

    EraseObject(TutScreen[tut_frame],
                screen->w - TutScreen[tut_frame]->w - 1, 0);
    watchdog = 0;
    tut_frame++;
    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    //place tux & fishy
    tux_object.dx = TUX2_DX1;
    tux_object.facing = LEFT;
    tux_object.frametype = WALKING;
    tux_object.endx = TUX2_ENDX_GOTO1;
    tux_object.word[0] = *"c" - *"a";
    tux_object.word[1] = *"a" - *"a";
    tux_object.word[2] = *"t" - *"a";

    word_object[fishies].alive = YES;
    word_object[fishies].y = 1;
    word_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w * 1.5;
    word_object[fishies].x = fish_object[fishies].x0;
    word_object[fishies].dy = DEFAULT_TUT2_FISH_SPEED;
    word_object[fishies].x_amplitude = 0;
    word_object[fishies].x_phase = 0;
    word_object[fishies].x_angle_mult = 0;
    //word_object[fishies].letter = 0;
    word_object[fishies].word = "cat "; //DEFAULT_PRACT_WORD;
    word_object[fishies].curframe = 0;
    word_object[fishies].eaten = YES;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    word_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        // THIS IS WHERE I AM!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (fishies > 0) {
            //EraseObject(fishy[fish_object[0].curframe].image,
            //fish_object[0].x, fish_object[0].y);  //erase fishy

            WordMoveFishies(&fishies, &splats, &lifes, &frame);

            word_object[0].y += word_object[0].dy;

            //DrawObject(fishy[fish_object[0].curframe].image,
            //fish_object[0].x, fish_object[0].y);   //draw fishy
            //DrawObject(letter[fish_object[0].letter],
            //fish_object[0].x, fish_object[0].y);   //draw letter overlay


            if (frame == word_object[0].endframe) {
                word_object[0].curframe++;
                if (word_object[0].curframe >= FISHY_FRAMES)
                    word_object[0].curframe = 0;
                word_object[0].endframe =
                    frame + fishy[word_object[0].curframe].frames;
                if (word_object[0].endframe > FRAME_MAX)
                    word_object[0].endframe -= FRAME_MAX;
            }
            watchdog = 0;
        } else
            watchdog++;

        if (tux_object.facing == LEFT
            && word_object[0].y >=
            (screen->h / 2 -
             2 * fishy[word_object[0].curframe].image->h)) {
            tux_object.facing = RIGHT;
            tux_object.frametype = WALKING;
            tux_object.curframe = 0;
            tux_object.dx = TUX2_DX1;
            tux_object.endx = word_object[0].x;
            //(TutScreen[0]->w / 2) - tuxStand[0].image[RIGHT]->w / 2;
            tux_object.letter = 0;
            EraseObject(TutScreen[tut_frame],
                        screen->w - TutScreen[tut_frame]->w - 1, 0);
            watchdog = 0;
            tut_frame++;
            DrawObject(TutScreen[tut_frame],
                       screen->w - TutScreen[tut_frame]->w - 1, 0);
        }

        WordCheckCollision(fishies, &fish_left, frame, verbose);
        WordCheckFishies(&fishies, &splats);

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }

    //THIRD TUT-FRAME, fishy go boom
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 3\n");

    EraseObject(TutScreen[tut_frame],
                screen->w - TutScreen[tut_frame]->w - 1, 0);
    watchdog = 0;
    tut_frame++;
    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    //place tux & fishy
    fishies = 0;
    splats = 0;
    tux_object.dx = TUX3_DX1;
    tux_object.facing = LEFT;
    tux_object.frametype = WALKING;
    tux_object.endx = TUX3_ENDX_GOTO1;
    tux_object.letter = NOT_A_LETTER;

    word_object[fishies].alive = YES;
    word_object[fishies].y = 1;
    word_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w * 1.5;
    word_object[fishies].x = word_object[fishies].x0;
    word_object[fishies].dy = DEFAULT_TUT3_FISH_SPEED;
    word_object[fishies].x_amplitude = 0;
    word_object[fishies].x_phase = 0;
    word_object[fishies].x_angle_mult = 0;
    //word_object[fishies].letter = 0;
    word_object[fishies].word = "cat "; //DEFAULT_PRACT_WORD;
    word_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    word_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        if (tux_object.dx == 0)
            tux_object.facing = RIGHT;

        if (fishies > 0) {
            //EraseObject(fishy[fish_object[0].curframe].image,
            //      fish_object[0].x, fish_object[0].y);    //erase fishy

            //fish_object[0].y += fish_object[0].dy;

            WordMoveFishies(&fishies, &splats, &lifes, &frame);

            word_object[0].y += word_object[0].dy;

            if (word_object[0].y >=
                (screen->h) - fishy[word_object[0].curframe].image->h -
                FISH3_MAX_Y - 1) {
                word_object[0].alive = NO;
                //sx =
                //   word_object[0].x +
                //   ((fishy[word_object[0].curframe].image->w) / 2) -
                //   ((splat[0].image->w) / 2);
                //if (sx < 0)
                //  sx = 0;
                //if (sx > (screen->w - splat[0].image->w))
                //  sx = screen->w - splat[0].image->w - 1;
                //sy = screen->h - splat[0].image->h - FISH3_MAX_Y - 1;
                for (t = 0; t <= strlen(word_object[0].word) - 2; t++) {
                    sx = (word_object[0].x +
                          (t * fishy[word_object[0].curframe].image->w)) +
                        ((fishy[word_object[0].curframe].image->w) / 2) -
                        ((splat[0].image->w) / 2);
                    if (sx < 0)
                        sx = 0;
                    if (sx > (screen->w - splat[0].image->w))
                        sx = screen->w - splat[0].image->w - 1;
                    sy = screen->h - splat[0].image->h - FISH3_MAX_Y - 1;

                    AddSplat(&splats, sx, sy, &i, &frame);
                }
                //AddSplat(&splats, sx, sy, &i, &frame);

                tux_object.frametype = YIPING;
                tux_object.curframe = 0;
                tux_object.facing = RIGHT;

                EraseObject(tuxStand[tux_object.curframe].image[LEFT],
                            tux_object.x, tux_object.y);
                DrawObject(tuxYipe[tux_object.curframe].image[RIGHT],
                           tux_object.x, tux_object.y);

                //play splatting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
                }
            }

            if (word_object[0].alive == YES) {
                //DrawObject(fishy[fish_object[0].curframe].image,
                //         fish_object[0].x, fish_object[0].y); //draw fishy
                //DrawObject(letter[fish_object[0].letter],
                //         fish_object[0].x, fish_object[0].y); //draw letter overlay

                if (frame == word_object[0].endframe) {
                    word_object[0].curframe++;
                    if (word_object[0].curframe >= FISHY_FRAMES)
                        word_object[0].curframe = 0;
                    word_object[0].endframe =
                        frame + fishy[word_object[0].curframe].frames;
                    if (word_object[0].endframe > FRAME_MAX)
                        word_object[0].endframe -= FRAME_MAX;
                }
            }
            watchdog = 0;
        } else
            watchdog++;

        WordCheckCollision(fishies, &fish_left, frame, verbose);
        WordCheckFishies(&fishies, &splats);

        if (splats > 0) {       //draw splat permanent!
            for (i = 0; i < splats; i++) {
                if (splat_object[i].alive == YES) {
                    DrawObject(splat[splat_object[i].curframe].image,
                               splat_object[i].x, splat_object[i].y);
                    if (frame == splat_object[i].endframe) {
                        splat_object[i].curframe++;
                        if (splat_object[i].curframe >= SPLAT_FRAMES) {
                            splat_object[i].curframe = SPLAT_FRAMES - 1;
                        }
                        splat_object[i].endframe = frame + splat[0].frames;
                        if (splat_object[i].endframe > FRAME_MAX)
                            splat_object[i].endframe -= FRAME_MAX;
                    }
                }
            }
        }

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }

    //FOURTH TUT-FRAME, Tux is so polite!
    if (verbose == YES)
        fprintf(stderr, "->>Tutorial Screen 4\n");

    EraseObject(TutScreen[tut_frame],
                screen->w - TutScreen[tut_frame]->w - 1, 0);
    for (i = 0; i < splats; i++) {
        EraseObject(splat[splat_object[i].curframe].image,
                    splat_object[i].x, splat_object[i].y);
        splat_object[i].alive = NO;
    }

    watchdog = 0;
    tut_frame++;
    DrawObject(TutScreen[tut_frame],
               screen->w - TutScreen[tut_frame]->w - 1, 0);
    //place tux & fishy
    fishies = 0;
    splats = 0;
    tux_object.dx = TUX4_DX1;
    tux_object.facing = RIGHT;
    tux_object.frametype = WALKING;
    tux_object.endx =
        (TutScreen[0]->w / 2) - tuxStand[0].image[RIGHT]->w / 2;
    tux_object.letter = NOT_A_LETTER;

    fish_object[fishies].alive = YES;
    fish_object[fishies].y = 1;
    fish_object[fishies].x0 =
        (TutScreen[0]->w / 2) - fishy[0].image->w / 2;
    fish_object[fishies].x = fish_object[fishies].x0;
    fish_object[fishies].dy = DEFAULT_TUT4_FISH_SPEED;
    fish_object[fishies].x_amplitude = 0;
    fish_object[fishies].x_phase = 0;
    fish_object[fishies].x_angle_mult = 0;
    fish_object[fishies].letter = 1;
    fish_object[fishies].curframe = 0;
    framebay = fishy[0].frames + frame;
    if (framebay > FRAME_MAX)
        framebay -= FRAME_MAX;
    fish_object[fishies].endframe = framebay;
    fishies = fishies + 1;

    while (watchdog < TUT1_ENDFRAME && exit_tut == NO) {
        /*
         * Poll input queue, run keyboard loop 
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                still_playing = NO;
                exit_tut = YES;
            }
        }

        keys = SDL_GetKeyState(NULL);

        if (keys[SDLK_ESCAPE] == SDL_PRESSED) {
            still_playing = YES;
            exit_tut = YES;
        }

        MoveTux(frame, verbose);

        if (fishies > 0) {
            EraseObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);      //erase fishy

            fish_object[0].y += fish_object[0].dy;

            if (fish_object[0].y >=
                (screen->h) - fishy[fish_object[0].curframe].image->h -
                FISH4_MAX_Y - 1) {
                fish_object[0].alive = NO;
                sx = fish_object[0].x +
                    ((fishy[fish_object[0].curframe].image->w) / 2) -
                    ((splat[0].image->w) / 2);
                if (sx < 0)
                    sx = 0;
                if (sx > (screen->w - splat[0].image->w))
                    sx = screen->w - splat[0].image->w - 1;
                sy = screen->h - splat[0].image->h - FISH4_MAX_Y - 1;
                AddSplat(&splats, sx, sy, &i, &frame);

                tux_object.frametype = YIPING;
                tux_object.curframe = 0;
                tux_object.facing = LEFT;

                EraseObject(tuxStand[tux_object.curframe].image[RIGHT],
                            tux_object.x, tux_object.y);
                DrawObject(tuxYipe[tux_object.curframe].image[LEFT],
                           tux_object.x, tux_object.y);

                EraseObject(TutScreen[tut_frame],
                            screen->w - TutScreen[tut_frame]->w - 1, 0);
                tut_frame++;
                DrawObject(TutScreen[tut_frame],
                           screen->w - TutScreen[tut_frame]->w - 1, 0);

                //play splatting sound
                if (sys_sound == YES) {
                    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
                }
            }

            if (fish_object[0].alive == YES) {
                DrawObject(fishy[fish_object[0].curframe].image, fish_object[0].x, fish_object[0].y);   //draw fishy
                DrawObject(letter[fish_object[0].letter], fish_object[0].x, fish_object[0].y);  //draw letter overlay

                if (frame == fish_object[0].endframe) {
                    fish_object[0].curframe++;
                    if (fish_object[0].curframe >= FISHY_FRAMES)
                        fish_object[0].curframe = 0;
                    fish_object[0].endframe =
                        frame + fishy[fish_object[0].curframe].frames;
                    if (fish_object[0].endframe > FRAME_MAX)
                        fish_object[0].endframe -= FRAME_MAX;
                }
            }
            watchdog = 0;
        } else {
            watchdog++;
        }


        CheckCollision(fishies, &fish_left, frame, verbose);
        CheckFishies(&fishies, &splats);

        if (splats > 0) {       //draw splat permanent!
            for (i = 0; i < splats; i++) {
                if (splat_object[i].alive == YES) {
                    DrawObject(splat[splat_object[i].curframe].image,
                               splat_object[i].x, splat_object[i].y);
                    if (frame == splat_object[i].endframe) {
                        splat_object[i].curframe++;
                        if (splat_object[i].curframe >= SPLAT_FRAMES) {
                            splat_object[i].curframe = SPLAT_FRAMES - 1;
                        }
                        splat_object[i].endframe = frame + splat[0].frames;
                        if (splat_object[i].endframe > FRAME_MAX)
                            splat_object[i].endframe -= FRAME_MAX;
                    }
                }
            }
        }

        UpdateScreen(&frame);
        if (speed_up == NO)
            WaitFrame();
    }


    /*
     * Wait a while- give screen chance to come up (if fullscreen) 
     */
    //SDL_Delay(3 *WAIT_MS);

    FreeGame(verbose, 1);       //free the game stuff, no music

    for (i = 0; i < PKCASCADE_TUT_SCREENS; i++)
        SDL_FreeSurface(TutScreen[i]);

    if (verbose == YES)
        fprintf(stderr, "->PractceWCascade(): END TUTORIAL\n");

    numupdates = 0;

    return (still_playing);
}
