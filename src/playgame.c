/*
   playgame.c:

   Code for "Fish Cascade" typing activity.

   Copyright 2000, 2003, 2007, 2008, 2009, 2010, 2011.
   Authors: Sam Hart, Jesse Andrews, David Bruce, George Stephanos.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   playgame.c is part of Tux Typing, a.k.a "tuxtype".

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
#include "playgame.h"
#include "snow.h"
#include "SDL_extras.h"
#include "input_methods.h"

/* Should these be constants? */
static int tux_max_width = 0;                // the max width of the images of tux
static int number_max_w = 0;                 // the max width of a number image


//static SDL_Surface* background = NULL;
static SDL_Surface* level[NUM_LEVELS] = {NULL};
static SDL_Surface* number[NUM_NUMS] = {NULL};
static SDL_Surface* curlev = NULL;
static SDL_Surface* lives = NULL;
static SDL_Surface* fish = NULL;
static SDL_Surface* congrats[CONGRATS_FRAMES] = {NULL};
static SDL_Surface* ohno[OH_NO_FRAMES] = {NULL};
static Mix_Chunk* sound[NUM_WAVES];

static sprite* fish_sprite = NULL;
static sprite* splat_sprite = NULL;

/* For input_methods.c processing of Asian text input: */
static IM_DATA im_data;


/* Local function prototypes: */
static void AddSplat(int* splats, struct fishypoo* f, int* curlives, int* frame);
static void CheckCollision(int fishies, int* fish_left, int frame );
static void CheckFishies(int* fishies, int* splats);
static int check_word(int f);
static void display_msg(const char* msg, int x, int y);
static void DrawBackground(void);
static void draw_bar(int curlevel, int diflevel, int curlives,
                     int oldlives, int fish_left, int oldfish_left);
static void DrawFish(int which);
static void DrawNumbers(int num, int x, int y, int places);
static void EraseNumbers(int num, int x, int y, int places);

static float float_restrict(float a, float x, float b);
static void FreeGame(void);
//static void HandleKey(SDL_keysym* key_sym);
static void HandleKey(SDL_keysym* key_sym);
static int int_restrict(int a, int x, int b);
static void LoadFishies(void);
static void LoadOthers(void);
static int LoadTuxAnims(void);
static void MoveFishies(int* fishies, int* splats, int* lifes, int* frame);
static void MoveTux(int frame, int fishies);
static void next_tux_frame(void);
static void ResetObjects(void);
static void SpawnFishies(int diflevel, int* fishies, int* frame);
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
int PlayCascade(int diflevel)
{
  char filename[FNLEN];
  int still_playing = 1;
  int playing_level = 1;
  int setup_new_level = 1;
  int won_level = 0;
  int quitting = 0;
  int curlevel = 0;
  int i = 0;
  int curlives = 0;
  int oldlives = 0;
  int oldfish_left = 0;
  int fish_left = 0;
  int fishies = 0;
  int local_max_fishies = 1;
  int frame = 0;
  int done_frames;
  int splats = 0;
  SDL_Event event;
  SDL_Surface *temp_text[CONGRATS_FRAMES + OH_NO_FRAMES];
  SDL_Rect text_rect;
  int text_y_end;
  int xamp;
  int yamp;
  int x_not;
  int y_not;
  int temp_text_frames;
  int temp_text_count;
  Uint16 key_unicode;
  Uint32 last_time, now_time;

  DEBUGCODE
  {
    fprintf(stderr, "->Entering PlayCascade(): level=%i\n", diflevel);
  }

//  SDL_ShowCursor(0); //don't really need this and it causes a bug on windows

//	SNOW_init();

  if(!LoadTuxAnims())
  {
    fprintf(stderr, "PlayCascade() - LoadTuxAnims() failed - returning to menu!\n\n\n");
    FreeGame();
    return 0;
  }
  
  LoadFishies();
  LoadOthers();

  /* Initialize input_methods system: */
//  im_init(&im_data, get_current_language());
  im_init(&im_data, 0); //will need function to put in correct language code:

  /* Make sure everything in the word list is "typable" according to the current */
  /* theme's keyboard.lst:                                                       */
  if (!CheckNeededGlyphs())
  {
    fprintf(stderr, "PlayCascade() - did not find all needed characters in theme's "
                    "keyboard.lst file - returning to menu!\n\n\n");
    FreeGame();
    return 0;
  }


  /*  --------- Begin outer game loop (cycles once per level): ------------- */

  LOG( " starting game \n ");

  while (still_playing)
  {
    /* FIXME split out as smaller function */
    if (setup_new_level) 
    {
      switch (diflevel)
      {
        default:
        case EASY:
          fish_left = MAX_FISHIES_EASY;

          if (settings.o_lives >  LIVES_INIT_EASY)
            curlives = settings.o_lives;
          else
            curlives = LIVES_INIT_EASY;
          break;

        case MEDIUM:
          fish_left = MAX_FISHIES_MEDIUM;

          if (settings.o_lives >  LIVES_INIT_MEDIUM)
            curlives = settings.o_lives;
          else
            curlives =  LIVES_INIT_MEDIUM;
          break;

        case HARD:
          fish_left = MAX_FISHIES_HARD;

          if (settings.o_lives >  LIVES_INIT_HARD)
            curlives = settings.o_lives;
          else
            curlives =  LIVES_INIT_HARD;
          break;
      }

      local_max_fishies = fish_left;

      /* ------- Load and draw background: ----------------- */

      if (curlevel != 0)
      {
        FreeBothBkgds();
      }

      if (diflevel == INF_PRACT)
        sprintf(filename, "pract.png");
      else
      {	
        sprintf(filename, "kcas%d.jpg", rand() % 12);
      }
      /* ---  Special Hidden Code  --- */

      if (settings.hidden && curlevel == 3)
        sprintf(filename, "hidden.jpg");

      DEBUGCODE
      {
        fprintf(stderr, "->>Loading background: %s\n", filename);
      }
        LoadBothBkgds(filename);
//			SNOW_setBkg( background );

      DrawBackground();

      ResetObjects();

      if (settings.sys_sound)
      {
        //TODO make use of more music files
        if(rand() % 2) 
          sprintf(filename, "amidst_the_raindrops.ogg");
        else
          sprintf(filename, "chiptune2.ogg");
        MusicLoad( filename, -1 );
      }

      setup_new_level = 0;

      LOG( "->>PLAYING THE GAME\n" );

    }

    /*  --------- Begin main game loop (cycles once per frame): ------------- */


    while (playing_level)
    {
      last_time = SDL_GetTicks();

      oldlives = curlives;
      oldfish_left = fish_left;

      EraseSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );

      /* --- Poll input queue, get keyboard info --- */
      while (SDL_PollEvent(&event))
      {
        if ( event.type == SDL_QUIT )
        {
          exit(0); /* FIXME does memory get cleaned up properly if we do this? */
        }
        else
        {
          if (event.type == SDL_KEYDOWN)
          {
            switch (event.key.keysym.sym)
            {
              case SDLK_F11:
                SDL_SaveBMP(screen, "screenshot.bmp");
                break;

              case SDLK_F6:
                settings.o_lives = settings.o_lives - 10;
                curlives = curlives - 10;
                break;

              case SDLK_F7:
                settings.o_lives = settings.o_lives + 10;
                curlives = curlives + 10;
                break;

              case SDLK_F10:
                /* NOTE this could be used to "cheat" by wiping out all the */
                /* current words if the player is about to lose.            */
                /* first wipe out old blits because screen size is changing */
                /* and otherwise we would segfault:                         */
                ResetBlitQueue();
                //numupdates = 0;
                SwitchScreenMode();
                DrawBackground();
                ResetObjects();
                break;

              case SDLK_F12:
                // SNOW_toggle();
                break;

              case SDLK_ESCAPE:
                /* Pause() returns 1 if quitting, */
                /* 0 if returning to game:        */
                if (Pause() == 1)
                {
                  playing_level = 0;
                  still_playing = 0;
                  quitting = 1;
                }
                else  /* Returning to game */
                  DrawBackground();
                break;

              /*  Don't count modifier keys as keystrokes in game: */ 
	      case SDLK_RSHIFT:
	      case SDLK_LSHIFT:
              case SDLK_RCTRL:
	      case SDLK_LCTRL:
	      case SDLK_RALT: 
              case SDLK_LALT:
	      case SDLK_RMETA:
	      case SDLK_LMETA:
              case SDLK_LSUPER:
              case SDLK_RSUPER:
                break;

              default:
              /*----------------------------------------------------*/
              /* Some other key - player is actually typing!!!!!!!! */
              /*----------------------------------------------------*/

                /* See what Unicode value was typed: */
                key_unicode = event.key.keysym.unicode;

                DEBUGCODE
                {fprintf(stderr, "\nkey_unicode = %d\twchar_t = %lc\t\n", key_unicode, key_unicode);}

                /* For now, the cascade game is case-insensitive for input, */
                /* with only uppercase for answers:                         */
                if (key_unicode >= 97 && key_unicode <= 122)
                  key_unicode -= 32;  //convert lowercase to uppercase
                if (key_unicode >= 224 && key_unicode <= 255)
                  key_unicode -= 32; //same for non-US Western European chars
                if ((key_unicode >= 256) && (key_unicode <= 382))  // Fix for other letters, such as the hungarian letter O with double acute
                    key_unicode -= 1;

                LOG ("After checking for lower case:\n");
                DEBUGCODE
                {fprintf(stderr, "key_unicode = %d\twchar_t = %lc\\n\n", key_unicode, key_unicode);}

                /* Now update with case-folded value: */
                UpdateTux(key_unicode, fishies, frame);
            }
          }
        }
      }   /* ------ End of 'while' loop for handling user input ------- */



      /* --- fishy updates --- */
      if ((frame% 3) == 0) 
//      if ((frame % 10) == 0)
        NEXT_FRAME(fish_sprite);

      if (fishies < local_max_fishies)
        SpawnFishies( diflevel, &fishies, &frame );

      MoveTux(frame, fishies);
      CheckCollision(fishies, &fish_left, frame);
      DrawSprite(tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y);
      MoveFishies(&fishies, &splats, &curlives, &frame);
      CheckFishies(&fishies, &splats);
//      SNOW_update();

      /* --- update top score/info bar --- */

      if (diflevel != INF_PRACT)
      {
        draw_bar(curlevel, diflevel, curlives, oldlives, fish_left, oldfish_left);

        if (curlives <= 0)
        {
          playing_level = 0;
          still_playing = 0;
        }
      }
      else
        fish_left = 1; // in practice there is always 1 fish left!

      if (fish_left <= 0)
      {
        won_level = 1;
        playing_level = 0;
        curlevel++;
        setup_new_level = 1;
        still_playing = 1;
      }

      if (!quitting) 
      {
        /* This does all the blits that we have queued up this frame: */
        UpdateScreen(&frame);
      }

      /* Pause (keep frame-rate event) */
      now_time = SDL_GetTicks();

      DEBUGCODE
      {
        fprintf(stderr, "now_time = %d\tlast_time = %d, elapsed time = %d\n",
                now_time, last_time, now_time - last_time);
      }

      if (now_time < last_time + 1000/FRAMES_PER_SEC)
      {
        //Prevent any possibility of a time wrap-around
        // (this is a very unlikely problem unless there is an SDL bug
        //  or you leave tuxmath running for 49 days...)
        now_time = (last_time + 1000/FRAMES_PER_SEC) - now_time;  // this holds the delay
        if (now_time > 1000/FRAMES_PER_SEC)
          now_time = 1000/FRAMES_PER_SEC;
        SDL_Delay(now_time);
      }
      else
      {
        LOG("Did not achieve desired frame rate!\n");
      }
    }  /* End per-frame game loop - level completed */


    if (settings.sys_sound)
      Mix_FadeOutMusic(MUSIC_FADE_OUT_MS);

    DrawBackground();

    /* Victory sequence, defeat sequence, or go to next level: */
    if (quitting == 0)
    {
      /* Level completed successfully: */
      if (won_level) 
      {
        if (curlevel < 4)  /* Advance to next level */
        {
          LOG( "--->NEXT LEVEL!\n" );
          done_frames = MAX_END_FRAMES_BETWEEN_LEVELS;
          playing_level = 1;
          xamp = 0;
          yamp = 0;
          won_level = 0;
        }
        else
        {
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

      }
      else  /* Did not win the level  :-(     */
      {
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

      for ( i=0; i<= done_frames; i++ ) 
      {
        temp_text_count = (temp_text_count+1) % temp_text_frames;

        text_rect.y -= END_FRAME_DY;
        y_not = text_rect.y;

        if (text_rect.y < text_y_end)
        {
          y_not = text_y_end + yamp * sin(i / WIN_GAME_ANGLE_MULT);
          text_rect.y = text_y_end;
          text_rect.x = x_not + xamp * cos(i / WIN_GAME_ANGLE_MULT);
        }

        DrawSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );
        DrawObject(temp_text[temp_text_count], text_rect.x, y_not);
        DrawObject(level[diflevel], 1, 1);
        draw_bar(curlevel - 1, diflevel, curlives, oldlives, fish_left, oldfish_left);

        next_tux_frame();
//        SNOW_update();
        /* Do all pending blits and increment frame counter: */
        UpdateScreen(&frame);

        EraseSprite( tux_object.spr[tux_object.state][tux_object.facing], tux_object.x, tux_object.y );
        EraseObject(temp_text[temp_text_count], text_rect.x, y_not);

        if (!settings.speed_up)
          WaitFrame();
      }  /* End of animation for end of game */

    }  /* End of post-level wrap-up  */

  }  /*   -------- End outer game loop -------------- */

//  SNOW_on = 0;

  LOG( "->Done with level... cleaning up\n" );

  FreeGame();

  LOG( "->PlayCascade(): END\n" );

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

static void ResetObjects(void)
{
  int i;

  LOG("RESETTING OBJECTS\n");

  for (i = 0; i < MAX_FISHIES_HARD + 1; i++)
  {
    fish_object[i] = null_fishy;
    splat_object[i] = null_splat;
  }

  tux_object.facing = RIGHT;
  tux_object.x = screen->w/2;
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



/*********************
LoadOthers : Load all other graphics
**********************/
static void LoadOthers(void)
{
	int i;
	char filename[FNLEN];

	LOG( "=LoadOthers()\n" );
	DEBUGCODE
	{
	  fprintf(stderr, "settings.theme_font_name is %s\n",
                  settings.theme_font_name);
	}

        RenderLetters(FISHY_FONT_SIZE);
        LOG( " Done rendering letters \n ");


	curlev = BlackOutline(gettext("Level"), LABEL_FONT_SIZE, &white);
	lives  = BlackOutline(gettext("Lives"), LABEL_FONT_SIZE, &white);
	fish   = BlackOutline(gettext("Fish"), LABEL_FONT_SIZE, &white);

	level[0] = BlackOutline(gettext("Easy"), LABEL_FONT_SIZE, &white);
	level[1] = BlackOutline(gettext("Medium"), LABEL_FONT_SIZE, &white);
	level[2] = BlackOutline(gettext("Hard"), LABEL_FONT_SIZE, &white);
	level[3] = BlackOutline(gettext("Practice"), LABEL_FONT_SIZE, &white);

	number_max_w = 0;
	for (i = 0; i < NUM_NUMS; i++) {
		sprintf(filename, "num_%i.png", i);
		number[i] = LoadImage( filename, IMG_ALPHA );
		if (number[i]->w > number_max_w)
			number_max_w = number[i]->w;
	}

	for (i = 0; i < CONGRATS_FRAMES; i++) {
		congrats[i] = BlackOutline(gettext("Congratulations"), LABEL_FONT_SIZE, &white);
	}

	for (i = 0; i < OH_NO_FRAMES; i++) {
		ohno[i] = BlackOutline(gettext("Oh No!"), LABEL_FONT_SIZE, &white);
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

//	PauseLoadMedia();

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

static void display_msg(const char* msg, int x, int y)
{
  if (msg == NULL || msg[0] == '\0')
    return;

  SDL_Surface* m;
  m = BlackOutline(msg, LABEL_FONT_SIZE, &white);

  if (m)
  {
    EraseObject(m, x, y);
    DrawObject(m, x, y);
    SDL_FreeSurface(m);
  }
}

/***************************
LoadFishies : Load the fish animations and graphics
*****************************/
static void LoadFishies(void)
{
	int i;

	LOG( "=LoadFishies()\n" );

	fish_sprite = LoadSprite( "fishy", IMG_ALPHA );
	splat_sprite = LoadSprite( "splat", IMG_ALPHA );

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
static int LoadTuxAnims(void)
{
  int i;
  int height = 0;                //temp width/height varis to determine max's

  LOG("LoadTuxAnims(): Loading Tux Animations\n");

  for (i = 0 ; i < TUX_NUM_STATES; i++)
  {
    tux_object.spr[i][RIGHT] = LoadSprite(tux_sprite_fns[i], IMG_ALPHA); 
    /* make sure image got loaded: */
    if(tux_object.spr[i][RIGHT] == NULL)
    {
      fprintf(stderr, "Warning - image %d failed to load\n", i);
      return 0;
    }
    tux_object.spr[i][LEFT] = FlipSprite(tux_object.spr[i][RIGHT], 1, 0); 
  }

  tux_max_width = tux_object.spr[TUX_STANDING][RIGHT]->frame[0]->w;
  height = tux_object.spr[TUX_STANDING][RIGHT]->frame[0]->h;

  LOG("LoadTuxAnims(): END\n");

  return 1;
}

/******************************
DrawNumbers : Draw numbers at
a certain x,y. See "usage"
below
*******************************/
static void DrawNumbers(int num, int x, int y, int places)
{
  LOG("\nEntering DrawNumbers()\n");

//usage:
//      num    = number to draw onscreen
//      x, y   = coords to place number (starting upper left)
//      places = number of places to fit it into (i.e., if
//                                       number = 5 and places = 2, would draw "05")
//                                       if places = 0, then will simply display as
//                                       many as necessary
    char numnuts[FNLEN];
    int needed_places, i;
    int uddernumber;

    sprintf(numnuts, "%d", num);
    i = 0;
    needed_places = strlen(numnuts);

    if (needed_places < FNLEN && needed_places <= places)
    {
        if (places > 0)
        {
            for (i = 1; i <= (places - needed_places); i++)
            {
                DrawObject(number[0], x, y);
                x += number[0]->w;
            }
        }
    }

    for (i = 0; i < needed_places; i++)
    {
      uddernumber = numnuts[i] - '0';
      DrawObject(number[uddernumber], x, y);
      x += number[uddernumber]->w;
    }
  LOG("\nLeaving DrawNumbers()\n");
}


/*************************
EraseNumbers: Erase numbers
from the screen. See "usage"
*****************************/
static void EraseNumbers(int num, int x, int y, int places)
{
  LOG("\nEntering EraseNumbers()\n");
//usage:
//      num    = number to draw onscreen
//      x, y   = coords to place number (starting upper left)
//      places = number of places to fit it into (i.e., if
//                                       number = 5 and places = 2, would draw "05")
//                                       if places = 0, then will simply display as
//                                       many as necessary
    char numnuts[FNLEN];
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

  LOG("\nLeaving EraseNumbers()\n");
}

/**********************
FreeGame : Free all
the game elements
***********************/
static void FreeGame(void)
{
  int i;

  FreeLetters();

  LOG( "FreeGame():\n-Freeing Tux Animations\n" );

  for (i = 0; i < TUX_NUM_STATES; i++ )
  {
    if (tux_object.spr[i][RIGHT])
      FreeSprite(tux_object.spr[i][RIGHT]);
    tux_object.spr[i][RIGHT] = NULL;

    if (tux_object.spr[i][LEFT])
      FreeSprite(tux_object.spr[i][LEFT]);
    tux_object.spr[i][LEFT] = NULL;
  }

  LOG( "-Freeing fishies\n" );

  if (fish_sprite)
    FreeSprite(fish_sprite);
  if (splat_sprite)
    FreeSprite(splat_sprite);
  fish_sprite = splat_sprite = NULL;

  LOG( "-Freeing other game graphics\n" );

  FreeBothBkgds();

  if (curlev)
    SDL_FreeSurface(curlev);
  if (fish)
    SDL_FreeSurface(fish);
  if (lives)
    SDL_FreeSurface(lives);
  curlev = fish = lives = NULL;

  for (i = 0; i < NUM_LEVELS; i++)
  {
    if (level[i])
      SDL_FreeSurface(level[i]);
    level[i] = NULL;
  }
  for (i = 0; i < NUM_NUMS; i++)
  {
    if (number[i])
      SDL_FreeSurface(number[i]);
    number[i] = NULL;
  }
  for (i = 0; i < CONGRATS_FRAMES; i++)
  {
    if (congrats[i])
      SDL_FreeSurface(congrats[i]);
    congrats[i] = NULL;
  }
  for (i = 0; i < OH_NO_FRAMES; i++)
  {
    if (ohno[i])
      SDL_FreeSurface(ohno[i]);
    ohno[i] = NULL;
  }
  if (settings.sys_sound)
  {
    LOG( "-Freeing sound\n" );
    for (i = 0; i < NUM_WAVES; ++i)
    {
      if (sound[i])
        Mix_FreeChunk(sound[i]);
      sound[i] = NULL;
    }
  }

//  PauseUnloadMedia();

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
  ResetBlitQueue();
  DrawObject(CurrentBkgd(), 0, 0);

// //    struct blit *update;
// 
//     LOG("-DrawBackground(): Updating entire background\n");
// 
//     numupdates=0;  // drawing entire background writes over all other stuff, so don't draw them
// 
//     update = &blits[numupdates++];
//     update->src = CurrentBkgd();
// 
//     update->srcrect->x = update->dstrect->x = 0;
//     update->srcrect->y = update->dstrect->y = 0;
//     update->srcrect->w = update->dstrect->w = update->src->w;
//     update->srcrect->h = update->dstrect->h = update->src->h;
// 
//     update->type = 'D';
}

/****************************
SpawnFishies: Spawn the fishes
in the key cascade game
*****************************/
static void SpawnFishies(int diflevel, int* fishies, int* frame)
{
  int i, spacing, max_length;
  wchar_t* new_word;

  LOG("Enter SpawnFishies()\n");

  /* Some safety checks: */

  if(!fishies || !frame)
  {
    fprintf(stderr, "SpawnFishies() - invalid ptr arg, returning");
    return;
  }

  /* Ensure that *fishies is a valid index for fish_object[]: */
  if(*fishies < 0 || *fishies > MAX_FISHIES_HARD)
  {
    fprintf(stderr, "SpawnFishies() - invalid *fishies: %d - returning", *fishies);
    return;
  }

  /* prevent segfault if for some reason fish_sprite invalid: */
  if (!fish_sprite || !fish_sprite->frame[0])
  {
    fprintf(stderr, "SpawnFishies() - invalid fish_sprite - returning");
    return;
  }



  switch (diflevel)
  {
    case INF_PRACT:
    case EASY:
      spacing = FISH_SPACING_EASY; break;
    case MEDIUM:
      spacing = FISH_SPACING_MEDIUM; break;
    case HARD:
      spacing = FISH_SPACING_HARD; break;
    default:
      spacing = FISH_SPACING_EASY;
      fprintf(stderr, "SpawnFishies() - invalid diflevel: %d\n", diflevel);
  }

  /* --- return without spawn if there isn't room yet --- */

  for (i = 0; i < *fishies; i++)
  {
    if (fish_object[i].y < (fish_sprite->frame[0]->h + spacing))
      return;
  }

  /* See how long of a word will fit the length of our screen: */
  max_length = screen->w / fish_sprite->frame[0]->w;

  do
  {
    new_word = GetWord();
    if (!new_word) break;
  }
  while(wcslen(new_word) > max_length);

  /* See if we get a valid word before we move on: */
  if (!new_word)
  {
    fprintf(stderr, "SpawnFishies() - could not get word - returning.\n");
    return;
  }


  /* If we get to here, it should be OK to actually spawn the fishy: */
  fish_object[*fishies].word = new_word;
  fish_object[*fishies].len = wcslen(new_word); //using wchar_t[] now
  fish_object[*fishies].alive = 1;
  fish_object[*fishies].can_eat = 0;
  fish_object[*fishies].w = fish_sprite->frame[0]->w * fish_object[*fishies].len;
  fish_object[*fishies].x = rand() % (screen->w - fish_object[*fishies].w);
  fish_object[*fishies].y = 0;

  /* set the percentage of the speed based on length */
  fish_object[*fishies].dy = pow(0.92, fish_object[*fishies].len - 1);
  /* ex: a 9 letter word will be roughly twice as slow! 0.92^8 */

  /* give it a random variance so the fish "crunch" isn't constant */
  fish_object[*fishies].dy *= ((75 + rand() % 25)/100.0);

  switch (diflevel)
  {
    case INF_PRACT:
      fish_object[*fishies].dy = DEFAULT_SPEED;
      break;
    case EASY:
      fish_object[*fishies].dy *= EASY_SPEED_FACTOR;
      break;
    case MEDIUM:
      fish_object[*fishies].dy *= MEDIUM_SPEED_FACTOR;
      break;
    case HARD:
      fish_object[*fishies].dy *= HARD_SPEED_FACTOR;
      break;
    default:
      fprintf(stderr, "SpawnFishies() - invalid diflevel: %d\n", diflevel);
  }

  /* Calculate the frame number at which the bottom of the fish will reach the top of Tux */
  fish_object[*fishies].splat_time = *frame + 
           (screen->h - fish_sprite->frame[0]->h - tux_object.spr[TUX_STANDING][0]->frame[0]->h)
           /
           (fish_object[*fishies].dy);

  DEBUGCODE 
  {
    /* NOTE need %S rather than %s because of wide characters */
    fprintf(stderr, "Spawn fishy with word '%S'\n", fish_object[*fishies].word);
    fprintf(stderr, "Byte length is: %d\n", (int)wcslen(fish_object[*fishies].word));
    fprintf(stderr, "UTF-8 char length is: %d\n", (int)fish_object[*fishies].len);
  }

  *fishies = *fishies + 1;

  LOG( "Leave SpawnFishies()\n" );
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

  LOG( "Entering CheckFishies()\n" );

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

  LOG("Leaving CheckFishies()\n");
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
static void AddSplat(int* splats, struct fishypoo* f, int* curlives, int* frame)
{
  int i;

  LOG("Enterint AddSplat()\n");

  for ( i = 0; i < f->len; i++ )
  {
	splat_object[*splats].x = int_restrict( 0, f->x+(fish_sprite->frame[0]->w*i) + ((fish_sprite->frame[fish_sprite->cur]->w)>>1)-((splat_sprite->frame[0]->w)>>1), screen->w-splat_sprite->frame[0]->h-1 );
	splat_object[*splats].y = screen->h - splat_sprite->frame[0]->h - 1;
	splat_object[*splats].alive = 10; // JA tweak here! frames of live of the splat
	*splats = *splats + 1;
  }

  f->alive = 0;

  *curlives = *curlives - 1;

  if (*curlives <= 0)
    *curlives = 0;

  if (settings.sys_sound) 
    Mix_PlayChannel(SPLAT_WAV, sound[SPLAT_WAV], 0);

  LOG("Enterint AddSplat()\n");
}


#define NUM_RTL_LANGUAGES 4
static int RTL()
{
  int i;
  char* languages[NUM_RTL_LANGUAGES] = {"he_IL.utf8", "ar_AE.utf8", "ar_BH.utf8", "ar_DZ.utf8"}; //etc
  for(i = 0; i< NUM_RTL_LANGUAGES; i++)
    if (strcmp(settings.theme_locale_name, languages[i])==0)
      return 1;
  return 0;
}



static void DrawFish(int which)
{
  int j = 0;
  int red_letters = 0;
  int current_letter;
  /* 'x_inset' and 'y_inset' are where the glyph to be drawn relative        */
  /* the fish_sprite graphic:                                                      */
  const int x_inset = 5;
  const int y_inset = 0;
  /* letter_x and letter_y are where the upper left corner of the glyph needs */
  /* to be located - (e.g. how SDL blitting understands locations)           */
  int letter_x = 0;
  int letter_y = 0;

  LOG ("Entering DrawFish()\n\n");

  SDL_Surface* letter_surface;

  /* Make sure needed pointers are valid - if not, return: */
  if (!fish_sprite || !fish_sprite->frame[0])
  {
    fprintf(stderr, "DrawFish() - returning, needed pointer invalid\n");
    return;
  }
	    

  LOG ("DrawFish() - drawing fishies:\n");

  /* Draw the fishies: */
  for (j = 0; j < fish_object[which].len; j++)
  {
    DrawSprite( fish_sprite,
                fish_object[which].x + (fish_sprite->frame[0]->w * j),
                fish_object[which].y);
  }

  LOG ("DrawFish() - drawing letters:\n");

  /* Now we draw the letters on top of the fish: */
  /* we only draw the letter if tux cannot eat it yet */
  if (!fish_object[which].can_eat)
  {
    red_letters = -1;
    j = 0;

    LOG("figure out how many letters are red:\n");
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


    LOG ("Now draw each letter:\n");

    int length = wcslen(fish_object[which].word);
    for (j = 0; j < length; j++)
    {
      current_letter = (int)fish_object[which].word[j];

      if (j < red_letters)
        letter_surface = GetRedGlyph(current_letter);
      else
        letter_surface = GetWhiteGlyph(current_letter);

      /* Set "letter_x" and "letter_y to where we want the letter drawn: */
      letter_x = fish_object[which].x + (j * fish_sprite->frame[0]->w) + x_inset;

      if (RTL())
	letter_x = fish_object[which].x + ((length-1-j) * fish_sprite->frame[0]->w) + x_inset;

      letter_y = fish_object[which].y + y_inset;

      if(letter_surface != NULL)
        DrawObject(letter_surface, letter_x, letter_y);
    }
  }
        LOG ("Leaving DrawFish()\n");
}

/****************************
MoveFishies : Display and
move the fishies according
to their settings
*****************************/
static void MoveFishies(int *fishies, int *splats, int *lifes, int *frame)
{
  int i, j;

  LOG("\nEntering MoveFishies()\n");

  for (i = 0; i < *fishies; i++)
  {
    if (fish_object[i].alive) 
    {
      for (j = 0; j < fish_object[i].len; j++)
        EraseSprite( fish_sprite, fish_object[i].x + (fish_sprite->frame[0]->w*j), fish_object[i].y );
	            
      fish_object[i].y += fish_object[i].dy;
	
      if (fish_object[i].y >= (screen->h) - fish_sprite->frame[fish_sprite->cur]->h - 1) 
         AddSplat( splats, &fish_object[i], lifes, frame );
    }
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
					DrawSprite( splat_sprite, splat_object[i].x, splat_object[i].y);
				else 
					EraseSprite( splat_sprite, splat_object[i].x, splat_object[i].y);
		}

	LOG("Leaving MoveFishies()\n\n");
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
static void CheckCollision(int fishies, int *fish_left, int frame )
{
  int i, j;

  LOG("\nEntering CheckCollision()\n");

	for (i = 0; i < fishies; i++) {
		if ((fish_object[i].y >= tux_object.y - fish_sprite->frame[0]->h) &&
		    (fish_object[i].x + (fish_object[i].w-fish_sprite->frame[0]->w)/2 >= tux_object.x) &&
		    (fish_object[i].x + (fish_object[i].w+fish_sprite->frame[0]->w)/2 <= tux_object.x + tux_max_width)) {

			if (fish_object[i].can_eat) {
                		LOG( "**EATING A FISHY** - in CheckCollision()\n" );

				fish_object[i].alive = 0;
				fish_object[i].can_eat = 0;

				for (j = 0; j < fish_object[i].len; j++) 
					EraseSprite(fish_sprite, (fish_object[i].x + (j * fish_sprite->frame[0]->w)), fish_object[i].y);

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
  LOG("Leaving CheckCollision()\n");
}


static void next_tux_frame(void)
{
  if (tux_object.state != TUX_GULPING)
  {
    NEXT_FRAME(tux_object.spr[tux_object.state][tux_object.facing]);
  }
  else
  {
    NEXT_FRAME(tux_object.spr[TUX_GULPING][tux_object.facing]);
    if (tux_object.spr[TUX_GULPING][tux_object.facing]->cur==0) 
      tux_object.state = TUX_STANDING;
  }
}

/***********************************
MoveTux : Update Tux's location & then blit him!
************************************/
static void MoveTux( int frame, int fishies ) 
{
	int i;
	int which=-1, time_to_splat=0;

	LOG( "Entering MoveTux()\n" );

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
//	if ((frame % 8) == 0) next_tux_frame();
        /* Changed from 8 to 3 because throttling frame rate down to 15 */
  if ((frame % 3) == 0)
    next_tux_frame();

  LOG( "Leaving MoveTux()\n" );
}

static void draw_bar(int curlevel, int diflevel, int curlives, int oldlives, int fish_left, int oldfish_left)
{
  LOG("Entering draw_bar()\n");

  /* --- draw difficulty --- */
  DrawObject(level[diflevel], 1, 1);

  LOG("about to draw level()\n");

  /* --- draw level --- */
  DrawObject(curlev, 1 + GRAPHIC_SPACE + level[diflevel]->w, 1);
  DrawNumbers(curlevel + 1, 1 + 2 * GRAPHIC_SPACE + level[diflevel]->w + curlev->w, 1, 0);

  LOG("about to draw lives()\n");

  /* --- draw lives --- */
  DrawObject(lives, 
            (screen->w) - 
            (1 + lives->w + fish->w + ((MAX_FISHIES_DIGITS + 1) * 2 * number_max_w) + GRAPHIC_SPACE), 1);

  if (oldlives != curlives)
  {
    EraseNumbers(oldlives, (screen->w) - (1 + fish->w + ((MAX_FISHIES_DIGITS + 1) * 2 * number_max_w) + GRAPHIC_SPACE), 1, 0);

    oldlives = curlives;
  }

  DrawNumbers(curlives, (screen->w) - (1 + fish->w + ((MAX_FISHIES_DIGITS + 1) * 2 * number_max_w) + GRAPHIC_SPACE), 1, 0);

  LOG("about to draw fish left()\n");

  /* --- draw fish left --- */ /* Drawing text label "Fish" */
  DrawObject(fish, (screen->w) - (1 + fish->w + (MAX_FISHIES_DIGITS * number_max_w)), 1);

  if (oldfish_left != fish_left)
  {
  LOG("about to erase numbers\n");

    EraseNumbers(oldfish_left, (screen->w) - (1 + (MAX_FISHIES_DIGITS * number_max_w)), 1, MAX_FISHIES_DIGITS);
    oldfish_left = fish_left;
  }

  LOG("about to draw numbers\n");

  DrawNumbers(fish_left, (screen->w) - (1 + (MAX_FISHIES_DIGITS * number[4]->w)), 1, MAX_FISHIES_DIGITS);

  LOG("Leaving draw_bar()\n");
}


/* Here we process the latest keystroke with the input_methods system. */ 
/* Depending on whether the current language uses a single keystroke   */
/* or multiple keystrokes per letter, we may or may not change what is displayed. */
static void HandleKey(SDL_keysym* key_sym)
{
  static int redraw = 0;
  wchar_t* im_cp = im_data.s;
  int texttool_len = 0;
  wchar_t texttool_str[256];
  
  /* Discard previous # of redraw characters */
  if((int)texttool_len <= redraw) 
    texttool_len = 0;
  else texttool_len -= redraw;
    texttool_str[texttool_len] = L'\0';

  /* Read IM, remember how many to redraw next iteration */
  redraw = im_read(&im_data, *key_sym);

  /* Queue each character to be displayed */
  while(*im_cp)
  {
    if (*im_cp == L'\b')
    {
      if (texttool_len > 0)
      {
        texttool_len--;
        texttool_str[texttool_len] = 0;
//        do_render_cur_text(0);
      }
    }
    else if (*im_cp == L'\r')
    {
      if (texttool_len > 0)
      {
//        rec_undo_buffer();
//        do_render_cur_text(1);
        texttool_len = 0;
      }
      im_softreset(&im_data);
    }
    else if (*im_cp == L'\t')
    {
      if (texttool_len > 0)
      {
//        rec_undo_buffer();
//        do_render_cur_text(1);
//        cursor_x = min(cursor_x + cursor_textwidth, canvas->w);
        texttool_len = 0;
      }
      im_softreset(&im_data);
    }
    //FIXME look up iswprint()
    else if (1) //(iswprint(*im_cp))
    {
      if (texttool_len < (sizeof(texttool_str) / sizeof(wchar_t)) - 1)
      {
        texttool_str[texttool_len++] = *im_cp;
        texttool_str[texttool_len] = 0;
//        do_render_cur_text(0);
      }
    }

    im_cp++;
  } /* while(*im_cp) */
}





