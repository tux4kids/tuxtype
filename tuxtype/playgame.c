/***************************************************************************
playgame.c 
-  description: Cascade game
-------------------
begin                : Fri May 5 2000
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
#include "playgame.h"
#include "snow.h"

//void add_words( int level );

/* Should these be constants? */
static int tux_max_width;                // the max width of the images of tux
static int number_max_w;                 // the max width of a number image

//int o_lives; // something cal is working on
//int sound_vol;
static TTF_Font* font = NULL;
static SDL_Surface* background = NULL;
static SDL_Surface* level[NUM_LEVELS] = {NULL};
static SDL_Surface* number[NUM_NUMS] = {NULL};
static SDL_Surface* curlev = NULL;
static SDL_Surface* lives = NULL;
static SDL_Surface* fish = NULL;
static SDL_Surface* congrats[CONGRATS_FRAMES] = {NULL};
static SDL_Surface* ohno[OH_NO_FRAMES] = {NULL};
static Mix_Chunk* sound[NUM_WAVES];

static sprite* fishy = NULL;
static sprite* splat = NULL;

/* --- Data Structure for Dirty Blitting --- */
static SDL_Rect srcupdate[MAX_UPDATES];
static SDL_Rect dstupdate[MAX_UPDATES];
static int numupdates = 0; // tracks how many blits to be done

struct blit {
    SDL_Surface* src;
    SDL_Rect* srcrect;
    SDL_Rect* dstrect;
    unsigned char type;
} blits[MAX_UPDATES];



/* Local function prototypes: */
static int AddRect(SDL_Rect* src, SDL_Rect* dst);
static void AddSplat(int* splats, struct fishypoo* f, int* curlives, int* frame);
static void CheckCollision(int fishies, int* fish_left, int frame );
static void CheckFishies(int* fishies, int* splats);
static int check_word(int f);
static void display_msg(const unsigned char* msg, int x, int y);
static void DrawBackground(void);
static void draw_bar(int curlevel, int diflevel, int curlives,
              int oldlives, int fish_left, int oldfish_left);
static void DrawFish(int which);
static void DrawNumbers(int num, int x, int y, int places);
static int DrawObject(SDL_Surface* surf, int x, int y);
static int DrawSprite(sprite* gfx, int x, int y);
static void EraseNumbers(int num, int x, int y, int places);
static int EraseObject(SDL_Surface* sprite, int x, int y);
static int EraseSprite(sprite* img, int x, int y);
static float float_restrict(float a, float x, float b);
static void FreeGame(void);
static int int_restrict(int a, int x, int b);
static void LoadFishies(void);
static void LoadOthers(void);
static void LoadTuxAnims(void);
static void MoveFishies(int* fishies, int* splats, int* lifes, int* frame);
static void MoveTux(int frame, int fishies);
static void next_tux_frame(void);
static void ResetObjects(void);
static void SpawnFishies(int diflevel, int* fishies, int* frame);
static void UpdateScreen(int* frame);
static void UpdateTux(wchar_t letter_pressed, int fishies, int frame);
static void WaitFrame(void);



/************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)             */
/*                                                                      */
/************************************************************************/

/*************************************************************************
* PlayCascade : This is the main Cascade game loop               *
*************************************************************************/
int PlayCascade( int diflevel ) {
	unsigned char filename[FNLEN];
	int still_playing = 1;
	int playing_level = 1;
	int setup_new_level = 1;
	int won_level = 0;
	int quitting = 0;
	int curlevel = 0;
	int i;
	int curlives;
	int oldlives=0, oldfish_left=0;
	int fish_left, fishies = 0, local_max_fishies=1;
	int frame = 0;
	int done_frames;
	int splats = 0;
	SDL_Event event;
	SDL_Surface *temp_text[CONGRATS_FRAMES + OH_NO_FRAMES];
	SDL_Rect text_rect;
	int text_y_end;
	int xamp, yamp, x_not, y_not;
	int temp_text_frames;
	int temp_text_count;
	Uint16 key_unicode;

	DEBUGCODE {
		fprintf(stderr, "->PlayCascade: level=%i\n", diflevel );
	}

	SDL_WarpMouse(screen->w / 2, screen->h / 2);
	SDL_ShowCursor(0);

	SNOW_init();

	LoadTuxAnims(); 
	LoadFishies();
	LoadOthers();
	RenderLetters(font);
	LOG( " Done rendering letters \n ");

	/* Make sure everything in the word list is "typable" according to the current */
        /* theme's keyboard.lst:                                                       */
	if (!CheckNeededGlyphs())
	{
          fprintf(stderr, "PlayCascade() - did not find all needed characters in theme's "
                          "keyboard.lst file - returning to menu!\n\n\n");
          FreeGame();
          return 0;
	}



	LOG( " starting game \n ");
	while (still_playing) {

		if (setup_new_level) {

			switch (diflevel) {
				case EASY:
				            fish_left = MAX_FISHIES_EASY;
				            if (settings.o_lives >  LIVES_INIT_EASY){
    				    		curlives = settings.o_lives;
					    }else
				    		curlives = LIVES_INIT_EASY;
				            break;
				case MEDIUM:
				            fish_left = MAX_FISHIES_MEDIUM;
				            if (settings.o_lives >  LIVES_INIT_MEDIUM){
    				    		curlives = settings.o_lives;
					    }else
				            curlives =  LIVES_INIT_MEDIUM;
				            break;
				case HARD:
				            fish_left = MAX_FISHIES_HARD;
				            if (settings.o_lives >  LIVES_INIT_HARD){
    				    		curlives = settings.o_lives;
					    }else
				            curlives =  LIVES_INIT_HARD;
				            break;
			}

			local_max_fishies = fish_left;

			if (curlevel != 0) {
				SDL_FreeSurface(background);
			}

			if (diflevel == INF_PRACT)
				sprintf(filename, "pract.png");
			else
				sprintf(filename, "kcas%i_%i.jpg", diflevel+1, curlevel+1);

			/* ---  Special Hidden Code  --- */

			if (settings.hidden && curlevel == 3)
				sprintf(filename, "hidden.jpg");

			DEBUGCODE {
				fprintf(stderr, "->>Loading background: %s\n", filename);
			}

			background = LoadImage( filename, IMG_REGULAR );
			SNOW_setBkg( background );

			DrawBackground();

			ResetObjects();

			if (settings.sys_sound) {
				sprintf(filename, "kmus%i.wav", curlevel + 1);
				MusicLoad( filename, -1 );
			}

			LOG( "->>PLAYING THE GAME\n" );

			setup_new_level = 0;
		}

		/* --- Poll input queue, get keyboard info --- */

		while (playing_level) {

			oldlives = curlives;
			oldfish_left = fish_left;

			EraseSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );

			/* --- Checking input --- */

			while ( SDL_PollEvent( &event ) ) 
				if ( event.type == SDL_QUIT ) {
					exit(0);
				} else if (event.type == SDL_KEYDOWN) {
	
					if (event.key.keysym.sym == SDLK_F11) 
						SDL_SaveBMP( screen, "screenshot.bmp" );
					if (event.key.keysym.sym == SDLK_F6){
						settings.o_lives=settings.o_lives-10;
						curlives=curlives-10;
					}
					if (event.key.keysym.sym == SDLK_F7) {
						settings.o_lives=settings.o_lives+10;
						curlives=curlives+10;
					}
					if (event.key.keysym.sym == SDLK_F10) 
						SwitchScreenMode();
					if (event.key.keysym.sym == SDLK_F12) 
						SNOW_toggle();
					if (event.key.keysym.sym == SDLK_ESCAPE) {

						if (Pause() == 1) {
							playing_level = 0;
							still_playing = 0;
							quitting = 1;
						} 
						DrawBackground();
					}


					key_unicode = event.key.keysym.unicode & 0xff;
					/* For now, tuxtype is case-insensitive for input, */
                                        /* with only uppercase for answers:                */
					DEBUGCODE
					{
					  fprintf(stderr,
					    "\nkey_unicode = %d\twchar_t = %lc\t\n",
					     key_unicode, key_unicode);
					}

                                        if (key_unicode >= 97 && key_unicode <= 122)
                                          key_unicode -= 32;  //convert lowercase to uppercase
                                        if (key_unicode >= 224 && key_unicode <= 255)
                                          key_unicode -= 32; //same for non-US chars

					LOG ("After checking for lower case:\n");
					DEBUGCODE
					{
					  fprintf(stderr,
					    "key_unicode = %d\twchar_t = %lc\\n\n",
					     key_unicode, key_unicode);
					}

					/* Now update with case-folded value: */
					UpdateTux(key_unicode, fishies, frame);
				}

			/* --- fishy updates --- */

			if ((frame % 10) == 0) NEXT_FRAME( fishy );
			
			if (fishies < local_max_fishies)
				SpawnFishies( diflevel, &fishies, &frame );

			MoveTux( frame, fishies );
			CheckCollision(fishies, &fish_left, frame );
			DrawSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );
			MoveFishies(&fishies, &splats, &curlives, &frame);
			CheckFishies(&fishies, &splats);
			SNOW_update();

			/* --- update top score/info bar --- */

			if (diflevel != INF_PRACT) {
				draw_bar(curlevel, diflevel, curlives, oldlives, fish_left, oldfish_left);

				if (curlives <= 0) {
					playing_level = 0;
					still_playing = 0;
				}
			} else
				fish_left = 1; // in practice there is always 1 fish left!

			if (fish_left <= 0) {
				won_level = 1;
				playing_level = 0;
				curlevel++;
				setup_new_level = 1;
				still_playing = 1;
			}

			if (!quitting) {
				UpdateScreen(&frame);

				if (!settings.speed_up)
					WaitFrame();
			}
		}

		if (settings.sys_sound)
			Mix_FadeOutMusic(MUSIC_FADE_OUT_MS);

		DrawBackground();

		if (quitting == 0) {

			if (won_level) {

				won_level = 0;
				if (curlevel < 4) {

					LOG( "--->NEXT LEVEL!\n" );

					done_frames = MAX_END_FRAMES_BETWEEN_LEVELS;
					playing_level = 1;
					xamp = 0;
					yamp = 0;

				} else {

					LOG( "--->WINNER!\n" );

					done_frames = MAX_END_FRAMES_WIN_GAME;
					still_playing = 0;
					xamp = WIN_GAME_XAMP;
					yamp = WIN_GAME_YAMP;

					if (settings.sys_sound) 
						Mix_PlayChannel(WINFINAL_WAV, sound[WINFINAL_WAV], 0);
				}

				if (settings.sys_sound) 
					Mix_PlayChannel(WIN_WAV, sound[WIN_WAV], 0);

				for (i = 0; i < CONGRATS_FRAMES; i++)
					temp_text[i] = congrats[i];

				temp_text_frames = CONGRATS_FRAMES;
				tux_object.state = TUX_WINNING;

			} else {

				LOG( "--->LOST :(\n" );

				done_frames = MAX_END_FRAMES_GAMEOVER;
				xamp = 0;
				yamp = 0;

				if (settings.sys_sound)
					Mix_PlayChannel(LOSE_WAV, sound[LOSE_WAV], 0);

				for (i = 0; i < OH_NO_FRAMES; i++)
					temp_text[i] = ohno[i];

				temp_text_frames = OH_NO_FRAMES;
				tux_object.state = TUX_YIPING;
			}

			/* --- draw the animation here --- */

			temp_text_count = 0;
			text_y_end = (screen->h / 2) - (temp_text[0]->h / 2);
			text_rect.x = (screen->w / 2) - (temp_text[0]->w / 2);
			text_rect.y = screen->h - temp_text[0]->h - 1;
			x_not = text_rect.x;

			LOG( "--->Starting Ending Animation\n" );
			
			for ( i=0; i<= done_frames; i++ ) {
				temp_text_count = (temp_text_count+1) % temp_text_frames;

				text_rect.y -= END_FRAME_DY;
				y_not = text_rect.y;

				if (text_rect.y < text_y_end) {
					y_not = text_y_end + yamp * sin(i / WIN_GAME_ANGLE_MULT);
					text_rect.y = text_y_end;
					text_rect.x = x_not + xamp * cos(i / WIN_GAME_ANGLE_MULT);
				}

				DrawSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );
				DrawObject(temp_text[temp_text_count], text_rect.x, y_not);
				DrawObject(level[diflevel], 1, 1);
				draw_bar(curlevel - 1, diflevel, curlives, oldlives, fish_left, oldfish_left);

				next_tux_frame();
				SNOW_update();
				UpdateScreen(&frame);

				EraseSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );
				
				EraseObject(temp_text[temp_text_count], text_rect.x, y_not);

				if (!settings.speed_up)
					WaitFrame();
			}
		}
	}

	SNOW_on = 0;

	LOG( "->Done with level... cleaning up\n" );

	FreeGame();

	LOG( "->PlayCascade: END\n" );

	return 1;
}




/***********************
 InitEngine
 ***********************/
void InitEngine(void) {
    int i;

    /* --- Set up the update rectangle pointers --- */
	
    for (i = 0; i < MAX_UPDATES; ++i) {
        blits[i].srcrect = &srcupdate[i];
        blits[i].dstrect = &dstupdate[i];
    }
}



/*************************************************/
/* TransWipe: Performs various wipes to new bkgs */
/*************************************************/
/*
 * Given a wipe request type, and any variables
 * that wipe requires, will perform a wipe from
 * the current screen image to a new one.
 */
int TransWipe(SDL_Surface* newbkg, int type, int var1, int var2)
{
    int i, j, x1, x2, y1, y2;
    int step1, step2, step3, step4;
    int frame;
    SDL_Rect src;
    SDL_Rect dst;

    LOG("->TransWipe(): START\n");

    if (!newbkg)
    {
      fprintf(stderr, "TransWipe() - 'newbkg' arg invalid!\n");
      return 0;
    }

    numupdates = 0;
    frame = 0;

    if(newbkg->w == screen->w && newbkg->h == screen->h) {
        if( type == RANDOM_WIPE )
            type = (RANDOM_WIPE * ((float) rand()) / (RAND_MAX+1.0));

        switch( type ) {
            case WIPE_BLINDS_VERT: {
                LOG("--+ Doing 'WIPE_BLINDS_VERT'\n");
                /* var1 is num of divisions
                   var2 is how many frames animation should take */
                if( var1 < 1 ) var1 = 1;
                if( var2 < 1 ) var2 = 1;
                step1 = screen->w / var1;
                step2 = step1 / var2;

                for(i = 0; i <= var2; i++) {
                    for(j = 0; j <= var1; j++) {
                        x1 = step1 * (j - 0.5) - i * step2 + 1;
                        x2 = step1 * (j - 0.5) + i * step2 + 1;
                        src.x = x1;
                        src.y = 0;
                        src.w = step2;
                        src.h = screen->h;
                        dst.x = x2;
                        dst.y = 0;
                        dst.w = step2;
                        dst.h = screen->h;
                        SDL_BlitSurface(newbkg, &src, screen, &src);
                        SDL_BlitSurface(newbkg, &dst, screen, &dst);
                        AddRect(&src, &src);
                        AddRect(&dst, &dst);
                    }
                    UpdateScreen(&frame);
                }

                src.x = 0;
                src.y = 0;
                src.w = screen->w;
                src.h = screen->h;
                SDL_BlitSurface(newbkg, NULL, screen, &src);
                SDL_Flip(screen);

                break;
            } case WIPE_BLINDS_HORIZ: {
                LOG("--+ Doing 'WIPE_BLINDS_HORIZ'\n");
                /* var1 is num of divisions
                   var2 is how many frames animation should take */
                if( var1 < 1 ) var1 = 1;
                if( var2 < 1 ) var2 = 1;
                step1 = screen->h / var1;
                step2 = step1 / var2;

                for(i = 0; i <= var2; i++) {
                    for(j = 0; j <= var1; j++) {
                        y1 = step1 * (j - 0.5) - i * step2 + 1;
                        y2 = step1 * (j - 0.5) + i * step2 + 1;
                        src.x = 0;
                        src.y = y1;
                        src.w = screen->w;
                        src.h = step2;
                        dst.x = 0;
                        dst.y = y2;
                        dst.w = screen->w;
                        dst.h = step2;
                        SDL_BlitSurface(newbkg, &src, screen, &src);
                        SDL_BlitSurface(newbkg, &dst, screen, &dst);
                        AddRect(&src, &src);
                        AddRect(&dst, &dst);
                    }
                    UpdateScreen(&frame);
                }

                src.x = 0;
                src.y = 0;
                src.w = screen->w;
                src.h = screen->h;
                SDL_BlitSurface(newbkg, NULL, screen, &src);
                SDL_Flip(screen);

                break;
            } case WIPE_BLINDS_BOX: {
                LOG("--+ Doing 'WIPE_BLINDS_BOX'\n");
                /* var1 is num of divisions
                   var2 is how many frames animation should take */
                if( var1 < 1 ) var1 = 1;
                if( var2 < 1 ) var2 = 1;
                step1 = screen->w / var1;
                step2 = step1 / var2;
                step3 = screen->h / var1;
                step4 = step1 / var2;

                for(i = 0; i <= var2; i++) {
                    for(j = 0; j <= var1; j++) {
                        x1 = step1 * (j - 0.5) - i * step2 + 1;
                        x2 = step1 * (j - 0.5) + i * step2 + 1;
                        src.x = x1;
                        src.y = 0;
                        src.w = step2;
                        src.h = screen->h;
                        dst.x = x2;
                        dst.y = 0;
                        dst.w = step2;
                        dst.h = screen->h;
                        SDL_BlitSurface(newbkg, &src, screen, &src);
                        SDL_BlitSurface(newbkg, &dst, screen, &dst);
                        AddRect(&src, &src);
                        AddRect(&dst, &dst);
                        y1 = step3 * (j - 0.5) - i * step4 + 1;
                        y2 = step3 * (j - 0.5) + i * step4 + 1;
                        src.x = 0;
                        src.y = y1;
                        src.w = screen->w;
                        src.h = step4;
                        dst.x = 0;
                        dst.y = y2;
                        dst.w = screen->w;
                        dst.h = step4;
                        SDL_BlitSurface(newbkg, &src, screen, &src);
                        SDL_BlitSurface(newbkg, &dst, screen, &dst);
                        AddRect(&src, &src);
                        AddRect(&dst, &dst);
                    }
                    UpdateScreen(&frame);
                }

                src.x = 0;
                src.y = 0;
                src.w = screen->w;
                src.h = screen->h;
                SDL_BlitSurface(newbkg, NULL, screen, &src);
                SDL_Flip(screen);

                break;
            } default:
                break;
        }
    }
    return 1;
}



/************************************************************************/
/*                                                                      */ 
/*         "Private" functions (local to playgame.c)                    */
/*                                                                      */
/************************************************************************/



static int check_word( int f ) {
	int i;

	if (wcslen(fish_object[f].word) > tux_object.wordlen) 
		return 0;

	for (i=0; i < wcslen(fish_object[f].word); i++) 
		if (fish_object[f].word[i] != tux_object.word[tux_object.wordlen -                                           wcslen(fish_object[f].word) + i])
			return 0;

	return 1;
}










/***************************************
	WaitFrame: wait for the next frame
***************************************/
void WaitFrame(void) {
	static Uint32  next_tick = 0;
	Uint32         this_tick = SDL_GetTicks();

	if (this_tick < next_tick)
		SDL_Delay(next_tick - this_tick);

	next_tick = this_tick + (1000 / FRAMES_PER_SEC);
}


/****************************************************
 ResetObjects : Clear and reset all objects to dead
****************************************************/

static void ResetObjects( void ) {
	int i;

	LOG( "RESETTING OBJECTS\n" );

	for (i = 0; i < MAX_FISHIES_HARD + 1; i++) {
		fish_object[i] = null_fishy;
		splat_object[i] = null_splat;
	}

	tux_object.facing = RIGHT;
	tux_object.x = screen->w / 2;
	tux_object.y = screen->h - tux_object.spr[0][RIGHT]->frame[0]->h - 1;
	tux_object.dx = 0;
	tux_object.dy = 0;
	tux_object.endx = tux_object.x;
	tux_object.endy = tux_object.y;
	tux_object.state = TUX_STANDING;
	tux_object.word[0] = 0;
	tux_object.wordlen = 0;

	LOG( "OBJECTS RESET\n" );
}

static int DrawSprite(sprite* gfx, int x, int y)
{
  struct blit* update;

  if (!gfx)
  {
    fprintf(stderr, "DrawSprite() - 'gfx' arg invalid!\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "DrawSprite() - 'update' ptr invalid!\n");
    return 0;
  }

  update->src = gfx->frame[gfx->cur];
  update->srcrect->x = 0;
  update->srcrect->y = 0;
  update->srcrect->w = gfx->frame[gfx->cur]->w;
  update->srcrect->h = gfx->frame[gfx->cur]->h;
  update->dstrect->x = x;
  update->dstrect->y = y;
  update->dstrect->w = gfx->frame[gfx->cur]->w;
  update->dstrect->h = gfx->frame[gfx->cur]->h;
  update->type = 'D';

  return 1;
}

/**********************
DrawObject : Draw an object at the specified
location. No respect to clipping!
*************************/
static int DrawObject(SDL_Surface* surf, int x, int y)
{
  struct blit *update;

  if (!surf)
  {
    fprintf(stderr, "DrawObject() - invalid 'surf' arg!\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "DrawObject() - 'update' ptr invalid!\n");
    return 0;
  }

  update->src = surf;
  update->srcrect->x = 0;
  update->srcrect->y = 0;
  update->srcrect->w = surf->w;
  update->srcrect->h = surf->h;
  update->dstrect->x = x;
  update->dstrect->y = y;
  update->dstrect->w = surf->w;
  update->dstrect->h = surf->h;
  update->type = 'D';

  return 1;
}

/************************
UpdateScreen : Update the screen and increment the frame num
***************************/
static void UpdateScreen(int* frame)
{
	int i;

	/* -- First erase everything we need to -- */
	for (i = 0; i < numupdates; i++)
		if (blits[i].type == 'E') 
			SDL_LowerBlit(blits[i].src, blits[i].srcrect, screen, blits[i].dstrect);
	SNOW_erase();

	/* -- then draw -- */ 
	for (i = 0; i < numupdates; i++)
		if (blits[i].type == 'D') 
			SDL_BlitSurface(blits[i].src, blits[i].srcrect, screen, blits[i].dstrect);
	SNOW_draw();

	/* -- update the screen only where we need to! -- */
	if (SNOW_on) 
		SDL_UpdateRects(screen, SNOW_add( (SDL_Rect*)&dstupdate, numupdates ), SNOW_rects);
	else 
		SDL_UpdateRects(screen, numupdates, dstupdate);

	numupdates = 0;
	*frame = *frame + 1;
}

static int EraseSprite(sprite* img, int x, int y)
{
  struct blit* update;

  if(!img)
  {
    fprintf(stderr, "EraseSprite() - invalid 'img' arg!\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "EraseSprite() - 'update' ptr invalid!\n");
    return 0;
  }

  update->src = background;
  update->srcrect->x = x;
  update->srcrect->y = y;
  update->srcrect->w = img->frame[img->cur]->w;
  update->srcrect->h = img->frame[img->cur]->h;

  /* check to see if we are trying blit data that doesn't exist!!! */

  if (update->srcrect->x + update->srcrect->w > background->w)
    update->srcrect->w = background->w - update->srcrect->x;
  if (update->srcrect->y + update->srcrect->h > background->h)
    update->srcrect->h = background->h - update->srcrect->y;

  update->dstrect->x = x;
  update->dstrect->y = y;
  update->dstrect->w = update->srcrect->w;
  update->dstrect->h = update->srcrect->h;
  update->type = 'E';

  return 1;
}

/*************************
EraseObject : Erase an object from the screen
**************************/
static int EraseObject(SDL_Surface* surf, int x, int y)
{
  struct blit *update;

  if(!surf)
  {
    fprintf(stderr, "EraseObject() - invalid 'surf' arg!\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "EraseObject() - 'update' ptr invalid!\n");
    return 0;
  }

  update->src = background;
  update->srcrect->x = x;
  update->srcrect->y = y;
  update->srcrect->w = surf->w;
  update->srcrect->h = surf->h;

  /* check to see if we are trying blit data that doesn't exist!!! */

  if (update->srcrect->x + update->srcrect->w > background->w)
    update->srcrect->w = background->w - update->srcrect->x;
  if (update->srcrect->y + update->srcrect->h > background->h)
    update->srcrect->h = background->h - update->srcrect->y;

  update->dstrect->x = x;
  update->dstrect->y = y;
  update->dstrect->w = update->srcrect->w;
  update->dstrect->h = update->srcrect->h; 
  update->type = 'E';

  return 1;
}


/******************************
AddRect : Dont actually blit a surface,
    but add a rect to be updated next
    update
*******************************/
static int AddRect(SDL_Rect* src, SDL_Rect* dst)
{

  /*borrowed from SL's alien (and modified)*/
  struct blit* update;

  if(!src)
  {
    fprintf(stderr, "AddRect() - invalid 'src' arg!\n");
    return 0;
  }

  if(!dst)
  {
    fprintf(stderr, "AddRect() - invalid 'dst' arg!\n");
    return 0;
  }

  update = &blits[numupdates++];

  if(!update || !update->srcrect || !update->dstrect)
  {
    fprintf(stderr, "AddRect() - 'update' ptr invalid!\n");
    return 0;
  }

  update->srcrect->x = src->x;
  update->srcrect->y = src->y;
  update->srcrect->w = src->w;
  update->srcrect->h = src->h;
  update->dstrect->x = dst->x;
  update->dstrect->y = dst->y;
  update->dstrect->w = dst->w;
  update->dstrect->h = dst->h;
  update->type = 'I';

  return 1;
}

/*********************
LoadOthers : Load all other graphics
**********************/
static void LoadOthers(void)
{
	int i;
	unsigned char filename[FNLEN];

	LOG( "=LoadOthers()\n" );
	DEBUGCODE
	{
	  fprintf(stderr, "settings.theme_font_name is %s\n",
                  settings.theme_font_name);
	}
	font = LoadFont(settings.theme_font_name, GAME_FONT_SIZE);

	curlev = BlackOutline(_("Level"), font, &white);
	lives  = BlackOutline(_("Lives"), font, &white);
	fish   = BlackOutline(_("Fish"), font, &white);

	level[0] = BlackOutline(_("Easy"), font, &white);
	level[1] = BlackOutline(_("Medium"), font, &white);
	level[2] = BlackOutline(_("Hard"), font, &white);
	level[3] = BlackOutline(_("Practice"), font, &white);

	number_max_w = 0;
	for (i = 0; i < NUM_NUMS; i++) {
		sprintf(filename, "num_%i.png", i);
		number[i] = LoadImage( filename, IMG_COLORKEY );
		if (number[i]->w > number_max_w)
			number_max_w = number[i]->w;
	}

	for (i = 0; i < CONGRATS_FRAMES; i++) {
		congrats[i] = BlackOutline(_("Congratulations"), font, &white);
	}

	for (i = 0; i < OH_NO_FRAMES; i++) {
		ohno[i] = BlackOutline(_("Oh No!"), font, &white);
	}
	
	if (settings.sys_sound) {
		LOG( "=Loading Sound FX\n" );

		sound[WIN_WAV] = LoadSound( "win.wav" );
		sound[WINFINAL_WAV] = LoadSound( "winfinal.wav" );
		sound[BITE_WAV] = LoadSound( "bite.wav" );
		sound[LOSE_WAV] = LoadSound( "lose.wav" );
		sound[RUN_WAV] = LoadSound( "run.wav" );
		sound[SPLAT_WAV] = LoadSound( "splat.wav" );
		sound[EXCUSEME_WAV] = LoadSound( "excuseme.wav" );

		LOG( "=Done Loading Sound FX\n" );
	} else 
		LOG( "=NO SOUND FX LOADED (not selected)\n" );

	PauseLoadMedia();

	LOG( "=Setting NULL fish & splat & word\n" );

	null_fishy.alive = 0;
	null_fishy.can_eat = 0;
	null_fishy.word = NULL;
	null_fishy.x = 0;
	null_fishy.y = 0;
	null_fishy.dy = 0;

	null_splat.x = 0;
	null_splat.y = 0;
	null_splat.alive = 0;

	LOG( "=LoadOthers() END\n" );
}

static void display_msg(const unsigned char* msg, int x, int y)
{
	SDL_Surface* m;
	m = TTF_RenderUTF8_Shaded(font, msg, white, white);
	EraseObject(m, x, y);
	DrawObject(m, x, y);
	SDL_FreeSurface(m);
}

/***************************
LoadFishies : Load the fish animations and graphics
*****************************/
static void LoadFishies(void)
{
	int i;

	LOG( "=LoadFishies()\n" );

	fishy = LoadSprite( "fishy", IMG_COLORKEY );
	splat = LoadSprite( "splat", IMG_COLORKEY );

	for (i = 0; i < MAX_FISHIES_HARD; i++) {
		fish_object[i].alive = 1;
		fish_object[i].can_eat = 0;
		splat_object[i].alive = 0;
	}

	LOG( "=LoadFishies(): END\n" );
}

/******************************
LoadTuxAnims : Load the Tux graphics and animations
*******************************/
static void LoadTuxAnims(void)
{
	int i;
	int height = 0;                //temp width/height varis to determine max's

	LOG( "=LoadTuxAnims(): Loading Tux Animations\n" );

	for ( i=0; i < TUX_NUM_STATES; i++ ) {
		tux_object.spr[i][RIGHT] = LoadSprite( tux_sprite_fns[i], IMG_COLORKEY ); 
		tux_object.spr[i][LEFT] = FlipSprite( tux_object.spr[i][RIGHT], 1, 0 ); 
	}

	tux_max_width = tux_object.spr[TUX_STANDING][RIGHT]->frame[0]->w;
	height        = tux_object.spr[TUX_STANDING][RIGHT]->frame[0]->h;

	LOG( "=LoadTuxAnims(): END\n" );
}

/******************************
DrawNumbers : Draw numbers at
a certain x,y. See "usage"
below
*******************************/
static void DrawNumbers(int num, int x, int y, int places)
{
//usage:
//      num    = number to draw onscreen
//      x, y   = coords to place number (starting upper left)
//      places = number of places to fit it into (i.e., if
//                                       number = 5 and places = 2, would draw "05")
//                                       if places = 0, then will simply display as
//                                       many as necessary
    unsigned char numnuts[FNLEN];
    int needed_places, i;
    int uddernumber;

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
static void EraseNumbers(int num, int x, int y, int places)
{
//usage:
//      num    = number to draw onscreen
//      x, y   = coords to place number (starting upper left)
//      places = number of places to fit it into (i.e., if
//                                       number = 5 and places = 2, would draw "05")
//                                       if places = 0, then will simply display as
//                                       many as necessary
    unsigned char numnuts[FNLEN];
    int needed_places, i;
    int uddernumber;

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
static void FreeGame(void)
{
	int i;

	FreeLetters();

	TTF_CloseFont(font);
        font = NULL;

	LOG( "FreeGame():\n-Freeing Tux Animations\n" );

	for ( i=0; i < TUX_NUM_STATES; i++ ) {
		FreeSprite(tux_object.spr[i][RIGHT]);
		FreeSprite(tux_object.spr[i][LEFT]);
		tux_object.spr[i][RIGHT] = NULL;
		tux_object.spr[i][LEFT] = NULL;
	}

	LOG( "-Freeing fishies\n" );

	FreeSprite( fishy );
	FreeSprite( splat );
        fishy = splat = NULL;

	LOG( "-Freeing other game graphics\n" );

	SDL_FreeSurface(background);
	SDL_FreeSurface(curlev);
	SDL_FreeSurface(fish);
	SDL_FreeSurface(lives);
        background = curlev = fish = lives = NULL;

	for (i = 0; i < NUM_LEVELS; i++)
	{
          SDL_FreeSurface(level[i]);
	  level[i] = NULL;
        }
	for (i = 0; i < NUM_NUMS; i++)
	{
	  SDL_FreeSurface(number[i]);
	  number[i] = NULL;
	}
	for (i = 0; i < CONGRATS_FRAMES; i++)
	{
	  SDL_FreeSurface(congrats[i]);
	  congrats[i] = NULL;
	}
	for (i = 0; i < OH_NO_FRAMES; i++)
	{
	  SDL_FreeSurface(ohno[i]);
	  ohno[i] = NULL;
	}
	if (settings.sys_sound) {
	  LOG( "-Freeing sound\n" );
	  for (i = 0; i < NUM_WAVES; ++i)
          {
	    Mix_FreeChunk(sound[i]);
            sound[i] = NULL;
	  }
	}

	PauseUnloadMedia();


	LOG( "FreeGame(): END\n" );
}

/***************************
DrawBackground : This
function updates the entire
background. Usefull when
loading new backgrounds,
or clearing game screen
****************************/
static void DrawBackground(void)
{
    struct blit *update;

    LOG("-DrawBackground(): Updating entire background\n");

    numupdates=0;  // drawing entire background writes over all other stuff, so don't draw them

    update = &blits[numupdates++];
    update->src = background;

    update->srcrect->x = update->dstrect->x = 0;
    update->srcrect->y = update->dstrect->y = 0;
    update->srcrect->w = update->dstrect->w = background->w;
    update->srcrect->h = update->dstrect->h = background->h;

    update->type = 'D';
}

/****************************
SpawnFishies: Spawn the fishes
in the key cascade game
*****************************/
static void SpawnFishies(int diflevel, int* fishies, int* frame)
{
	int i, spacing;
	wchar_t* new_word;

	switch (diflevel) {
		case INF_PRACT:
		case EASY:   spacing = FISH_SPACING_EASY; break;
		case MEDIUM: spacing = FISH_SPACING_MEDIUM; break;
		case HARD:   spacing = FISH_SPACING_HARD; break;
	}

	/* --- return without spawn if there isn't room yet --- */

	for (i = 0; i < *fishies; i++)
		if (fish_object[i].y < (fishy->frame[0]->h + spacing))
			return;

	/* See if we get a valid UTF-8 word from WORDS_get before we move on: */
	/* Now that we are using UTF-8, some characters may be more than one byte, */
	/* so we can't just use strlen() anymore - DSB.*/		
	LOG( "=>Spawning fishy\n" );

	new_word = GetWord();

	if (!new_word)
	{
	  fprintf(stderr, "SpawnFishies() - could not get word - returning.\n");
          return;
	}

	/* If we get to here, it should be OK to actually spawn the fishy: */
	fish_object[*fishies].word = new_word;
        fish_object[*fishies].len = wcslen(new_word);
	fish_object[*fishies].alive = 1;
	fish_object[*fishies].can_eat = 0;
	fish_object[*fishies].w = fishy->frame[0]->w * fish_object[*fishies].len;
	fish_object[*fishies].x = rand() % (screen->w - fish_object[*fishies].w);
	fish_object[*fishies].y = 0;

	/* set the percentage of the speed based on length */
	fish_object[*fishies].dy = pow(0.92,fish_object[*fishies].len-1);
	/* ex: a 9 letter word will be roughly twice as slow! 0.92^8 */

	/* give it a random variance so the fish "crunch" isn't constant */
	fish_object[*fishies].dy *= ((75 + rand() % 25)/100.0);
	switch (diflevel) {
		case INF_PRACT:
			fish_object[*fishies].dy = DEFAULT_SPEED;
			break;
		case EASY:
			fish_object[*fishies].dy *= MAX_SPEED_EASY;
			break;
		case MEDIUM:
			fish_object[*fishies].dy *= MAX_SPEED_MEDIUM;
			break;
		case HARD:
			fish_object[*fishies].dy *= MAX_SPEED_HARD;
			break;
   	}

	fish_object[*fishies].splat_time = *frame + (480 - fishy->frame[0]->h - tux_object.spr[TUX_STANDING][0]->frame[0]->h)/fish_object[*fishies].dy;

	DEBUGCODE {
		/* NOTE need %S rather than %s because of wide characters */
		fprintf(stderr, "Spawn fishy with word '%S'\n", fish_object[*fishies].word);
		fprintf(stderr, "Byte length is: %d\n", wcslen(fish_object[*fishies].word));
		fprintf(stderr, "UTF-8 char length is: %d\n", fish_object[*fishies].len);
	}

	*fishies = *fishies + 1;
}

/***************************
CheckFishies : Check all the fishies and splats.
               sort the splats and fishies
****************************/
static void CheckFishies(int* fishies, int* splats)
{
	int forward, backward;
	struct fishypoo fish_temp;
	struct splatter splat_temp;

//	LOG( "CheckFishies\n" );

	/* move any fish from the rear to fill up gaps in the
	 * front
	 */
	forward = 0;
	backward = MAX_FISHIES_HARD;
	while (forward < backward) {
		while (backward > 0 && !fish_object[backward].alive)
			backward--;
		while (forward < backward && fish_object[forward].alive)
			forward++;
		if ((fish_object[backward].alive) && (!fish_object[forward].alive)) {
			/* swap alive to be infront of dead */
			fish_temp = fish_object[backward];
			fish_object[backward] = fish_object[forward];
			fish_object[forward] = fish_temp;
		}
	}
	
	/* same thing for splats */

	forward = 0;
	backward = MAX_FISHIES_HARD;
	while (forward < backward) {
		while (backward > 0 && !splat_object[backward].alive)
			backward--;
		while (forward < backward && splat_object[forward].alive)
			forward++;
		if ((splat_object[backward].alive) && (!splat_object[forward].alive)) {
			/* swap alive to be infront of dead */
			splat_temp = splat_object[backward];
			splat_object[backward] = splat_object[forward];
			splat_object[forward] = splat_temp;
		}
	}

	/* update the count for fishies & splats */

	*splats = *fishies = 0;

	for (forward = 0; forward < MAX_FISHIES_HARD; forward++)
		if (fish_object[forward].alive)
			*fishies+=1;
		else
			break;

	for (forward = 0; forward < MAX_FISHIES_HARD; forward++)
		if (splat_object[forward].alive)
			*splats+=1;
		else
			break;
}

// Restrict x to a value in the range from a ... b
static int int_restrict(int a, int x, int b) {
	if (x < a) x = a;
	if (x > b) x = b;
	return x;
}

static float float_restrict(float a, float x, float b)
{
	if (x < a) x = a;
	if (x > b) x = b;
	return x;
}

/***************************
AddSplat: A fish has died, add a splat where he used to be
****************************/
static void AddSplat(int* splats, struct fishypoo* f, int* curlives, int* frame) {
	int i;

	for ( i = 0; i < f->len; i++ ) {
		splat_object[*splats].x = int_restrict( 0, f->x+(fishy->frame[0]->w*i) + ((fishy->frame[fishy->cur]->w)>>1)-((splat->frame[0]->w)>>1), screen->w-splat->frame[0]->h-1 );
		splat_object[*splats].y = screen->h - splat->frame[0]->h - 1;
		splat_object[*splats].alive = 10; // JA tweak here! frames of live of the splat
		*splats = *splats + 1;
	}

	f->alive = 0;

	*curlives = *curlives - 1;
	if (*curlives <= 0)
		*curlives = 0;

	if ( settings.sys_sound ) 
		Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);
}




static void DrawFish(int which)
{
/*        LOG ("Entering DrawFish()\n");*/
	int j = 0;
	int red_letters = 0;
	int current_letter;
	/* 'x_inset' and 'y_inset' are where the glyph to be drawn relative        */
	/* the fishy graphic:                                                      */
	const int x_inset = 10;
        const int y_inset = 10;
	/* letter_x and letter_y are where the upper left corner of the glyph needs */
        /* to be located - (e.g. how SDL blitting understands locations)           */
        int letter_x = 0;
        int letter_y = 0;

        SDL_Surface* letter_surface;

	/* Make sure needed pointers are valid - if not, return: */
        if (!fishy || !fishy->frame[0])
	{
          fprintf(stderr, "DrawFish() - returning, needed pointer invalid\n");
          return;
	}
	    


	/* Draw the fishies: */
	for (j = 0; j < fish_object[which].len; j++)
        {
	  DrawSprite( fishy,
                      fish_object[which].x + (fishy->frame[0]->w*j),
                      fish_object[which].y);
        }


	/* Now we draw the letters on top of the fish: */
	/* we only draw the letter if tux cannot eat it yet */
	if (!fish_object[which].can_eat)
	{
		red_letters = -1;
		j = 0;

		/* figure out how many letters are red: */
		while (j < tux_object.wordlen && red_letters == -1)
		{
		  int k;
		  for (k = 0; k < tux_object.wordlen - j; k++)
                  {
                    if (fish_object[which].word[k] != tux_object.word[j + k]) 
                      k = 100000;
                  }

                  if (k < 100000)
                    red_letters = tux_object.wordlen - j;	
                  else
                    j++;
		}
	
// 		LOG ("Preparing to draw letters:\n");

		/* Now draw each letter: */
		for (j = 0; j < wcslen(fish_object[which].word); j++)
		{
		  current_letter = (int)fish_object[which].word[j];

		  if (j < red_letters)
                    letter_surface = GetRedGlyph(current_letter);
                  else
                    letter_surface = GetWhiteGlyph(current_letter);

		  /* Set "letter_x" and "letter_y to where we want the letter drawn: */
		  letter_x = fish_object[which].x + (j * fishy->frame[0]->w) + x_inset;
		  letter_y = fish_object[which].y + y_inset;

		  DrawObject(letter_surface, letter_x, letter_y);
		}
	}
/*        LOG ("Leaving DrawFish()\n");*/
}

/****************************
MoveFishies : Display and
move the fishies according
to their settings
*****************************/
static void MoveFishies(int *fishies, int *splats, int *lifes, int *frame) {
	int i, j;

//	LOG("start MoveFishies\n");

	for (i = 0; i < *fishies; i++)
		if (fish_object[i].alive) {
			for (j=0; j < fish_object[i].len; j++)
				EraseSprite( fishy, fish_object[i].x + (fishy->frame[0]->w*j), fish_object[i].y );
	            
			fish_object[i].y += fish_object[i].dy;
	
			if (fish_object[i].y >= (screen->h) - fishy->frame[fishy->cur]->h - 1) 
				AddSplat( splats, &fish_object[i], lifes, frame );
		}
	
	for (i = 0; i < *fishies; i++)
		if (fish_object[i].alive && fish_object[i].can_eat) 
			DrawFish( i );

	for (i = 0; i < *fishies; i++)
		if (fish_object[i].alive && !fish_object[i].can_eat) 
			DrawFish( i );

	for (i = 0; i < *splats; i++) 
		if (splat_object[i].alive) {
			splat_object[i].alive--;
			if (splat_object[i].alive>1)
					DrawSprite( splat, splat_object[i].x, splat_object[i].y);
				else 
					EraseSprite( splat, splat_object[i].x, splat_object[i].y);
		}

//	LOG("end MoveFishies\n");
}

/* UpdateTux : anytime a key is pressed, we need check to
 * see if a fish can be eaten.  The fish that could hit
 * the bottom of the screen first should be choosen if 
 * two fishies match what is typed
 */
void UpdateTux(wchar_t letter_pressed, int fishies, int frame) {
	int i;
	int time_it_splats=0;
	int which=-1;

	/* --- move our word array to make room if needed --- */

	if (tux_object.wordlen == MAX_WORD_SIZE) {
		for (i = 0; i < MAX_WORD_SIZE; i++) 
			tux_object.word[i] = tux_object.word[i + 1];
		tux_object.wordlen--;
	}

	/* --- add letter pressed to word array --- */

	tux_object.word[tux_object.wordlen] = letter_pressed;
	tux_object.wordlen++;
	tux_object.word[tux_object.wordlen] = 0;

	/* choose the word that matchs what has been typed  */
	/* and will hit the ground first                    */
	for (i = 0; i < fishies; i++) 
		if ((fish_object[i].alive && !fish_object[i].can_eat) && check_word(i) && (time_it_splats > fish_object[i].splat_time || !time_it_splats)) {
			time_it_splats = fish_object[i].splat_time;
			which = i;
		}

	if (which!=-1) {
		fish_object[which].can_eat = 1;
		tux_object.wordlen = 0;
		tux_object.word[0] = 0;
	}

}

/*************************
CheckCollision: Check
for collisions between Tux
and Fishies. If collided,
perform appropriate action
***************************/
static void CheckCollision(int fishies, int *fish_left, int frame ) {
	int i, j;

//	LOG( "start CheckCollision\n" );

	for (i = 0; i < fishies; i++) {
		if ((fish_object[i].y >= tux_object.y - fishy->frame[0]->h) &&
		    (fish_object[i].x + (fish_object[i].w-fishy->frame[0]->w)/2 >= tux_object.x) &&
		    (fish_object[i].x + (fish_object[i].w+fishy->frame[0]->w)/2 <= tux_object.x + tux_max_width)) {

			if (fish_object[i].can_eat) {
                		LOG( "**EATING A FISHY** - in CheckCollision()\n" );

				fish_object[i].alive = 0;
				fish_object[i].can_eat = 0;

				for (j = 0; j < fish_object[i].len; j++) 
					EraseSprite(fishy, (fish_object[i].x + (j * fishy->frame[0]->w)), fish_object[i].y);

				*fish_left = *fish_left - 1;

				tux_object.state = TUX_GULPING;
				REWIND(tux_object.spr[TUX_GULPING][tux_object.facing]);
				tux_object.dx = 0;
				tux_object.endx = tux_object.x;

				if (settings.sys_sound) Mix_PlayChannel(BITE_WAV, sound[BITE_WAV], 0);

			} else if (tux_object.state == TUX_STANDING) {
				LOG( "***EXCUSE ME!** - in CheckCollision()\n" );

				if (settings.sys_sound && !Mix_Playing(EXCUSEME_WAV))
					Mix_PlayChannel(EXCUSEME_WAV, sound[EXCUSEME_WAV], 0);
			}
		}
	}
//	LOG( "end CheckCollision\n" );
}

static void next_tux_frame(void) {

	if ( tux_object.state != TUX_GULPING ) {
		NEXT_FRAME(tux_object.spr[tux_object.state][tux_object.facing]);
	} else {
		NEXT_FRAME(tux_object.spr[TUX_GULPING][tux_object.facing]);
		if (tux_object.spr[TUX_GULPING][tux_object.facing]->cur==0) 
			tux_object.state = TUX_STANDING;
	}
}

/***********************************
MoveTux : Update Tux's location & then blit him!
************************************/
static void MoveTux( int frame, int fishies ) {
	int i;
	int which=-1, time_to_splat=0;

//	LOG( "MoveTux\n" );

	EraseSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );

	if (tux_object.state != TUX_GULPING) {
		for (i=0; i<fishies; i++) 
			if (fish_object[i].can_eat && (!time_to_splat || fish_object[i].splat_time < time_to_splat)) {
				time_to_splat = fish_object[i].splat_time;
				which = i;
			}
		if (which != -1) {
			int endx = int_restrict( 0, fish_object[which].x + (fish_object[which].w/2) - (tux_object.spr[TUX_GULPING][RIGHT]->frame[0]->w / 2), screen->w - tux_max_width - 1 );
			if (endx != tux_object.endx) {
				tux_object.endx = endx;

				if (tux_object.endx >= tux_object.x)
					tux_object.facing = RIGHT;
				else
					tux_object.facing = LEFT;

				/* see how fast we need to go to get there... */
				if (time_to_splat - frame > (abs(tux_object.endx - tux_object.x) / WALKING_SPEED)) {
					tux_object.dx = WALKING_SPEED;
					tux_object.state = TUX_WALKING;

					//stop running sound (if playing)                                               
					if (settings.sys_sound && Mix_Playing(RUN_WAV))
						Mix_HaltChannel(RUN_WAV);
				} else {
					if (time_to_splat > frame) 
						tux_object.dx = float_restrict( MIN_RUNNING_SPEED, abs(tux_object.endx - tux_object.x) / (time_to_splat-frame), MAX_RUNNING_SPEED );
					else {
						tux_object.dx = MAX_RUNNING_SPEED;
						if (settings.sys_sound && !Mix_Playing(RUN_WAV))
							if (abs(tux_object.endx - tux_object.x) > 50)
								Mix_PlayChannel(RUN_WAV, sound[RUN_WAV], 0);
					}

					tux_object.state = TUX_RUNNING;
				}
			}
		}
	}

	/* --- move tux (if moving) --- */
	
	tux_object.x = float_restrict(0, tux_object.x + (tux_object.facing==RIGHT ? 1.0 : -1.0)*tux_object.dx, (screen->w - tux_max_width));

	/* if done with certain frames, then reset to standing */

	if (tux_object.state == TUX_WALKING || tux_object.state == TUX_RUNNING) {
		if ((tux_object.facing == RIGHT && tux_object.x >= tux_object.endx) ||
		    (tux_object.facing == LEFT && tux_object.x <= tux_object.endx)) {

			LOG( "-In MoveTux(): returning tux to standing\n" );
			tux_object.state = TUX_STANDING;
			tux_object.dx = 0;
			tux_object.x = tux_object.endx;
		}
	}

	if ((frame % 8) == 0) next_tux_frame();
}

static void draw_bar(int curlevel, int diflevel, int curlives, int oldlives, int fish_left, int oldfish_left) {
	/* --- draw difficulty --- */

	DrawObject(level[diflevel], 1, 1);

	/* --- draw level --- */
	DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w, 1);
	DrawNumbers(curlevel + 1, 1 + 2 * GRAPHIC_SPACE + level[diflevel]->w + curlev->w, 1, 0);

	/* --- draw lives --- */
	DrawObject(lives, (screen->w) - (1+lives->w+fish->w+((MAX_FISHIES_DIGITS+1)*2*number_max_w)+GRAPHIC_SPACE), 1);

	if (oldlives != curlives) {
		EraseNumbers(oldlives, (screen->w) - (1+fish->w+((MAX_FISHIES_DIGITS+1)*2*number_max_w)+GRAPHIC_SPACE), 1, 0);
		oldlives = curlives;
	}

	DrawNumbers(curlives, (screen->w) - (1 + fish->w + ((MAX_FISHIES_DIGITS + 1) * 2 * number_max_w) + GRAPHIC_SPACE), 1, 0);

	/* --- draw fish left --- */
	DrawObject(fish, (screen->w) - (1 + fish->w + (MAX_FISHIES_DIGITS * number_max_w)), 1);
	if (oldfish_left != fish_left) {
		EraseNumbers(oldfish_left, (screen->w) - (1 + (MAX_FISHIES_DIGITS * number_max_w)), 1, MAX_FISHIES_DIGITS);
		oldfish_left = fish_left;
	}
	DrawNumbers(fish_left, (screen->w) - (1 + (MAX_FISHIES_DIGITS * number[4]->w)), 1, MAX_FISHIES_DIGITS);
}







