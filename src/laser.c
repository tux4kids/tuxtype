/*
   laser.h:

   "Comet Zap" typing game, adapted from "Tux, of Math
   Command"  (http://tux4kids.alioth.debian.org)
   
   Copyright 2000, 2003, 2008, 2010.
   Authors: Bill Kendrick, Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   laser.h is part of Tux Typing, a.k.a "tuxtype".

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
#include "laser.h"


#define FPS (1000 / 15)   /* 15 fps max */
#define CITY_EXPL_START 3 * 5  /* Must be mult. of 5 (number of expl frames) */
#define COMET_EXPL_START 2 * 2 /* Must be mult. of 2 (number of expl frames) */
#define ANIM_FRAME_START 4 * 2 /* Must be mult. of 2 (number of tux frames) */
#define GAMEOVER_COUNTER_START 75
#define LEVEL_START_WAIT_START 20
#define LASER_START 5
#define NUM_ANS 8
#define COMET_ZAP_FONT_SIZE 32

/* Local (to laser.c) 'globals': */
static sprite* shield = NULL;
static SDL_Surface* images[NUM_IMAGES] = {NULL};
static Mix_Chunk* sounds[NUM_SOUNDS] = {NULL};
static Mix_Music* musics[NUM_MUSICS] = {NULL};

static int wave, speed, score, pre_wave_score, num_attackers, distanceMoved , num_cities_alive;
static wchar_t ans[NUM_ANS];
static int ans_num;

static comet_type comets[MAX_COMETS];
static city_type cities[NUM_CITIES];
static laser_type laser;

static int tts_announcer_switch = 1;
static int braille_letter_pos = 0;

/* Local function prototypes: */
static void laser_add_comet(int diff_level);
static void laser_add_score(int inc);
static void laser_draw_console_image(int i);
static void laser_draw_let(wchar_t c, int x, int y);
static void laser_draw_line(int x1, int y1, int x2, int y2, int r, int g, int b);
static void laser_draw_numbers(const char* str, int x);
static void laser_load_data(void);
static void laser_reset_level(int diff_level);
static void laser_putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
static void laser_unload_data(void);
static void calc_city_pos(void);
static void recalc_comet_pos(void);
static void stop_tts_announcer();
static int tts_announcer(void *unused);



/* --- MAIN GAME FUNCTION!!! --- */

/* TODO modify game to allow longer words (12 chars or so) */

int PlayLaserGame(int diff_level)
{
	int i, img, done, quit, frame, lowest, lowest_y, 
	    tux_img, old_tux_img, tux_pressing, tux_anim, tux_anim_frame,
	    tux_same_counter, level_start_wait,
	    num_comets_alive, paused, picked_comet, 
	    gameover;
	  

	//TTS Word announcer variables
	SDL_Thread *thread;

	//Braille Variables
	wchar_t pressed_letters[1000];
	int braille_iter;

	Uint16 key_unicode;

	SDL_Event event;
	Uint32 last_time = 0;
        Uint32 now_time = 0;
	SDLKey    key;
	SDL_Rect  src, dest;
	/* str[] is a buffer to draw the scores, waves, etc. (don't need wchar_t) */
	char str[64]; 

	LOG( "starting Comet Zap game\n" );
	DOUT( diff_level );

	SDL_ShowCursor(0);
	laser_load_data();

	/* Clear window: */
  
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Flip(screen);

	/* --- MAIN GAME LOOP: --- */

	done = 0;
	quit = 0;
	src.w = src.h = 0;

	/* Prepare to start the game: */
  
	wave = 1;
	score = 0;
	gameover = 0;
	level_start_wait = LEVEL_START_WAIT_START;

	
  /* (Create and position cities) */
  for (i = 0; i < NUM_CITIES; i++)
  {
    cities[i].alive = 1;
    cities[i].expl = 0;
    cities[i].shields = 1;
  }

  /* figure out x placement: */
  calc_city_pos();

  num_cities_alive = NUM_CITIES;
  num_comets_alive = 0;


	/* (Clear laser) */

	laser.alive = 0;

  
	/* Reset remaining stuff: */
 
	laser_reset_level(diff_level);
  
	/* --- MAIN GAME LOOP!!! --- */
  
	frame = 0;
	paused = 0;
	picked_comet = -1;
	tux_img = IMG_TUX_RELAX1;
	tux_anim = -1;
	tux_anim_frame = 0;
	tux_same_counter = 0;
	ans_num = 0;

	MusicPlay(musics[MUS_GAME + (rand() % NUM_MUSICS)], 0);
	


	 //Call announcer function in thread which annonces the word to type 
	if(settings.tts)
		thread = SDL_CreateThread(tts_announcer, NULL);	
	
	//Inetialising braille variables
	braille_iter = 0;
    pressed_letters[braille_iter] = L'\0';

	do {

		frame++;
		last_time = SDL_GetTicks();

		old_tux_img = tux_img;
		tux_pressing = 0;

		/* Handle any incoming events: */
     
		while (SDL_PollEvent(&event) > 0) {

			if (event.type == SDL_QUIT) {
				/* Window close event - quit! */
				exit(0);
	      
			}
			else if (event.type == SDL_KEYDOWN)
			{

				key = event.key.keysym.sym;
				if (key == SDLK_F10) 
                                {
				  SwitchScreenMode();
                                  calc_city_pos();
                                  recalc_comet_pos();
                                }
				if (key == SDLK_F11)
					SDL_SaveBMP( screen, "laser.bmp");

				if (key == SDLK_ESCAPE)
					paused = 1;
				/* Score */
				if(key == SDLK_F1)
					tts_announcer_switch = 2;
				
				/* iglu alive */
				if(key == SDLK_F2)
					tts_announcer_switch = 3;
				
				/* Wave number */
				if(key == SDLK_F3)
					tts_announcer_switch = 4;

				/* --- eat other keys until level wait has passed --- */ 
				if (level_start_wait > 0) 
					key = SDLK_UNKNOWN;
				
				key_unicode = event.key.keysym.unicode;
				//key_unicode = event.key.keysym.unicode & 0xff;

				DEBUGCODE
				{
				  fprintf(stderr, "key_unicode = %d\n", key_unicode);
				}

				/* For now, tuxtype is case-insensitive for input, */
                                /* with only uppercase for answers:                */
                                if (key_unicode >= 97 && key_unicode <= 122)
                                  key_unicode -= 32;  //convert lowercase to uppercase
                                if (key_unicode >= 224 && key_unicode <= 255)
                                  key_unicode -= 32; //same for non-US chars
                                if ((key_unicode >= 256) && (key_unicode <= 382))  // Fix for other letters, such as the hungarian letter O with double acute
                                    key_unicode -= 1;

				LOG ("After checking for lower case:\n");
				DEBUGCODE
				{
				  fprintf(stderr,
                                   "key_unicode = %d\n", key_unicode);
				}
				
				/* Store each keys till a key released */
				if(settings.braille)
				{
				   pressed_letters[braille_iter] = event.key.keysym.sym;
                   braille_iter++;
                   pressed_letters[braille_iter] = L'\0';   
				}
				else
				{
					/* Now update with case-folded value: */
					ans[ans_num++] = key_unicode;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				/* ----- SDL_KEYUP is Only for Braille Mode -------------*/
				if(settings.braille)
				{
					wcscpy(pressed_letters,arrange_in_order(pressed_letters));
				    if (wcscmp(pressed_letters,L"") != 0)
				    {
					   for(i=0;i<100;i++)
					   {
						   if (wcscmp(pressed_letters,braille_key_value_map[i].key) == 0)
						   {
							   if (settings.use_english)
							   {
								   /* English have no such rules */
							   	   ans[ans_num++] = toupper(braille_key_value_map[i].value_begin[0]);
							   }
							   else
							   {
							   		if (braille_letter_pos == 0)
								   	   ans[ans_num++] = braille_key_value_map[i].value_begin[0];
								    else if (braille_letter_pos == 1)
									   ans[ans_num++] = braille_key_value_map[i].value_middle[0];
								    else
									   ans[ans_num++] = braille_key_value_map[i].value_end[0];
							   }			   
						   }
					   }	   
				   }
				   
				   braille_iter = 0;
				   pressed_letters[braille_iter] = L'\0';
			
			  }
			}
		}
      
      
		/* Handle answer: */

		for (;ans_num>0;ans_num--) {

			/*  Pick the lowest shootable comet which has the right answer: */
	
			lowest_y = 0;
			lowest = -1;
			
			/* Only Shoot the lowest letter if tts is enabled. */
			if (settings.tts || settings.braille)
			{
				for (i = 0; i < MAX_COMETS; i++)
					if (comets[i].alive
					&& comets[i].shootable 
					&& comets[i].expl == 0
					&& comets[i].y > lowest_y)
					{
						lowest = i;
						lowest_y = comets[i].y;
					}
			
					//Only Shoot the lowest letter.	
					if (comets[lowest].ch != ans[ans_num -1 ])
						lowest = -1;
			}
			else
			{
				for (i = 0; i < MAX_COMETS; i++)
					if (comets[i].alive
					&& comets[i].shootable 
					&& comets[i].expl == 0
					&& comets[i].ch == ans[ans_num -1 ] 
					&& comets[i].y > lowest_y)
					{
						lowest = i;
						lowest_y = comets[i].y;
					}	
			}
			
			
			/* Set the Braille letter position 
			* For some specific language's which have same braille code for
			* alphabets and signs at begining, middle and end position. */			
			if (lowest == -1)
				braille_letter_pos = 0;
			else
			{
				if (comets[lowest].pos == wcslen(comets[lowest].word)-2)
					braille_letter_pos = 2; //Next is end letter
				else if (comets[lowest].pos == wcslen(comets[lowest].word)-1)
					braille_letter_pos = 0; //Word finished 
				else
					braille_letter_pos = 1; //Next letter is at middle
					
				fprintf(stderr,"%S",comets[lowest].word);
			}
				
	
			/* If there was an comet with this answer, destroy it! */
			if (lowest != -1) {

				/* Destroy comet: */
		  
				comets[lowest].expl = COMET_EXPL_START;
				/* Make next letter in word shootable: */
				comets[lowest].shootable = 0;
                                if (comets[lowest].next)
                                  comets[lowest].next->shootable = 1;

				/* Fire laser: */
				laser.alive = LASER_START;

				/* this is a hack so drawing to the center of the screen works */
				if (abs(comets[lowest].x - screen->w/2) < 10) {
					laser.x1 = screen->w / 2;
					laser.y1 = screen->h;
	    
					laser.x2 = laser.x1;
					laser.y2 = comets[lowest].y;
				} else {
					laser.x1 = screen->w / 2;
					laser.y1 = screen->h;
	    
					laser.x2 = comets[lowest].x;
					laser.y2 = comets[lowest].y;
				}
	    
				PlaySound(sounds[SND_LASER]);
	    
				/* 50% of the time.. */
	    
				if (0 == (rand() % 2))  {

					/* ... pick an animation to play: */ 
					if (0 == (rand() % 2))
						tux_anim = IMG_TUX_YES1;
					else
						tux_anim = IMG_TUX_YAY1;
	        
					tux_anim_frame = ANIM_FRAME_START;
				}

				/* Increment score: */

				laser_add_score( (diff_level+1) * 5 * ((screen->h - comets[lowest].y)/20 ));

			} else {

				/* Didn't hit anything! */
	    
				PlaySound(sounds[SND_BUZZ]);
	    
				if (0 == (rand() % 2))
					tux_img = IMG_TUX_DRAT;
				else
					tux_img = IMG_TUX_YIPE;

				laser_add_score( -25 * wave);
			}
		}

      
		/* Handle start-wait countdown: */
      
		if (level_start_wait > 0) {

			level_start_wait--;
	  
			if (level_start_wait > LEVEL_START_WAIT_START / 4)
				tux_img = IMG_TUX_RELAX1;
			else if (level_start_wait > 0)
				tux_img = IMG_TUX_RELAX2;
			else
				tux_img = IMG_TUX_SIT;
	  
			if (level_start_wait == LEVEL_START_WAIT_START / 4)
				PlaySound(sounds[SND_ALARM]);
		}

      
		/* If Tux pressed a button, pick a new (different!) stance: */
	  
		if (tux_pressing) {
			while (tux_img == old_tux_img)
				tux_img = IMG_TUX_CONSOLE1 + (rand() % 3);

			PlaySound(sounds[SND_TOCK]);
		}
      
      
		/* If Tux is being animated, show the animation: */

		if (tux_anim != -1) {
			tux_anim_frame--;

			if (tux_anim_frame < 0)
				tux_anim = -1;
			else
				tux_img = tux_anim + 1 - (tux_anim_frame / (ANIM_FRAME_START / 2));
		}


		/* Reset Tux to sitting if he's been doing nothing for a while: */

		if (old_tux_img == tux_img) {
			tux_same_counter++;

			if (tux_same_counter >= 20)
				old_tux_img = tux_img = IMG_TUX_SIT;
			if (tux_same_counter >= 60)
				old_tux_img = tux_img = IMG_TUX_RELAX1;
		} else
			tux_same_counter = 0;


		/* Handle comets: */
     
		num_comets_alive = 0;

		distanceMoved += speed;
      
		for (i = 0; i < MAX_COMETS; i++) {
			if (comets[i].alive) {

				num_comets_alive++;

				comets[i].x = comets[i].x + 0;
				comets[i].y = comets[i].y + speed;
	      
				if (comets[i].y >= (screen->h - images[IMG_CITY_BLUE]->h) && comets[i].expl == 0) {

					/* Disable shields or destroy city: */
		      
					if (cities[comets[i].city].shields) {
						cities[comets[i].city].shields = 0;
						PlaySound(sounds[SND_SHIELDSDOWN]);
						laser_add_score(-50 * (diff_level+1));
					} else {
						cities[comets[i].city].expl = CITY_EXPL_START;
						PlaySound(sounds[SND_EXPLOSION]);
						laser_add_score(-100 * (diff_level+1));
					}

					tux_anim = IMG_TUX_FIST1;
					tux_anim_frame = ANIM_FRAME_START;

					/* Destroy comet: */

					comets[i].expl = COMET_EXPL_START;
				}

				/* Handle comet explosion animation: */

				if (comets[i].expl != 0) {
					comets[i].expl--;

					if (comets[i].expl == 0)
						comets[i].alive = 0;
				}
			}
		}


		/* Handle laser: */

		if (laser.alive > 0)
			laser.alive--;
     
		/* Comet time! */

		if (level_start_wait == 0 && (frame % 5) == 0 && gameover == 0) {
			if (num_attackers > 0) {

				/* More comets to add during this wave! */
		
				if ((num_comets_alive < 2 || ((rand() % 4) == 0)) && distanceMoved > 40) {
					distanceMoved = 0;
					laser_add_comet(diff_level);
					num_attackers--;
				}
			} else {
				if (num_comets_alive == 0) {
					T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,gettext("Score %d"),score);
					/* Time for the next wave! */

					/* FIXME: End of level stuff goes here */

					if (num_cities_alive > 0) {

						/* Go on to the next wave: */
						wave++;
						laser_reset_level(diff_level);

					} else {

						/* No more cities!  Game over! */
						gameover = GAMEOVER_COUNTER_START;
					}
				}
			}
		}


		/* Handle cities: */
     
		num_cities_alive = 0;

		for (i = 0; i < NUM_CITIES; i++) 
			if (cities[i].alive) {

				num_cities_alive++;

				/* Handle animated explosion: */

				if (cities[i].expl) {
					cities[i].expl--;
		  
					if (cities[i].expl == 0)
						cities[i].alive = 0;
				}
			}
                        

		/* Handle game-over: */

		if (gameover > 0) {
			gameover--;

			if (gameover == 0)
				done = 1;
		}
                
                if ((num_cities_alive==0) && (gameover == 0))
                {
                    gameover = GAMEOVER_COUNTER_START;
                    if(settings.tts)
						stop_tts_announcer();
					T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,gettext("yep you miss it. hahh hahh haa. game over! you scored %d goodbye!"),score);
					
				}
      
		/* Draw background: */
     
		SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);

		/* Draw wave: */

		dest.x = 0;
		dest.y = 0;
		dest.w = images[IMG_WAVE]->w;
		dest.h = images[IMG_WAVE]->h;

		SDL_BlitSurface(images[IMG_WAVE], NULL, screen, &dest);

		sprintf(str, "%d", wave);
		laser_draw_numbers(str, images[IMG_WAVE]->w + (images[IMG_NUMBERS]->w / 10));


		/* Draw score: */

		dest.x = (screen->w - ((images[IMG_NUMBERS]->w / 10) * 7) - images[IMG_SCORE]->w);
		dest.y = 0;
		dest.w = images[IMG_SCORE]->w;
		dest.h = images[IMG_SCORE]->h;

		SDL_BlitSurface(images[IMG_SCORE], NULL, screen, &dest);
      
		sprintf(str, "%.6d", score);
		laser_draw_numbers(str, screen->w - ((images[IMG_NUMBERS]->w / 10) * 6));
      
      
		/* Draw comets: */
      
		for (i = 0; i < MAX_COMETS; i++) 
			if (comets[i].alive) {

				/* Decide which image to display: */
				if (comets[i].expl == 0)
					img = IMG_COMET1 + ((frame + i) % 3);
				else
					img = (IMG_COMETEX2 - (comets[i].expl / (COMET_EXPL_START / 2)));
	      

				/* Draw it! */

				dest.x = comets[i].x - (images[img]->w / 2);
				dest.y = comets[i].y - images[img]->h;
				dest.w = images[img]->w;
				dest.h = images[img]->h;
	      
				SDL_BlitSurface(images[img], NULL, screen, &dest);
			}


		/* Draw letters: */

		for (i = 0; i < MAX_COMETS; i++)
			if (comets[i].alive && comets[i].expl == 0)
				laser_draw_let(comets[i].ch, comets[i].x, comets[i].y);
      
		/* Draw cities: */
      
		if (frame%2 == 0) NEXT_FRAME(shield);
		for (i = 0; i < NUM_CITIES; i++) {

			/* Decide which image to display: */
	 
			if (cities[i].alive) {
				if (cities[i].expl == 0)
					img = IMG_CITY_BLUE;
				else
					img = (IMG_CITY_BLUE_EXPL5 - (cities[i].expl / (CITY_EXPL_START / 5)));
			} else 
				img = IMG_CITY_BLUE_DEAD;
	  
	  
			/* Change image to appropriate color: */
	  
			img += ((wave % MAX_CITY_COLORS) * (IMG_CITY_GREEN - IMG_CITY_BLUE));
	  
	  
			/* Draw it! */
	  
			dest.x = cities[i].x - (images[img]->w / 2);
			dest.y = (screen->h) - (images[img]->h);
			dest.w = (images[img]->w);
			dest.h = (images[img]->h);
	  
			SDL_BlitSurface(images[img], NULL, screen, &dest);

			/* Draw sheilds: */

			if (cities[i].shields) {

				dest.x = cities[i].x - (shield->frame[shield->cur]->w / 2);
				dest.h = (screen->h) - (shield->frame[shield->cur]->h);
				dest.w = src.w;
				dest.h = src.h;
				SDL_BlitSurface( shield->frame[shield->cur], NULL, screen, &dest);

			}
		}


		/* Draw laser: */

		if (laser.alive)
			laser_draw_line(laser.x1, laser.y1, laser.x2, laser.y2, 255 / (LASER_START - laser.alive),
			                192 / (LASER_START - laser.alive), 64);

		laser_draw_console_image(IMG_CONSOLE);

		if (gameover > 0)
			tux_img = IMG_TUX_FIST1 + ((frame / 2) % 2);

		laser_draw_console_image(tux_img);


		/* Draw "Game Over" */

		if (gameover > 0) {

			dest.x = (screen->w - images[IMG_GAMEOVER]->w) / 2;
			dest.y = (screen->h - images[IMG_GAMEOVER]->h) / 2;
			dest.w = images[IMG_GAMEOVER]->w;
			dest.h = images[IMG_GAMEOVER]->h;
	
			SDL_BlitSurface(images[IMG_GAMEOVER], NULL, screen, &dest);
		}
      
      
		/* Swap buffers: */
      
		SDL_Flip(screen);


		/* If we're in "PAUSE" mode, pause! */

		if (paused) {
			if(settings.tts)
				stop_tts_announcer();
			T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,gettext("Game Paused!"));
			quit = Pause();
			if(quit == 0){
					T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,gettext("Pause Released!"));
					//Call announcer function in thread which annonces the word to type
					if(settings.tts)
						thread = SDL_CreateThread(tts_announcer, NULL);
			}							
			paused = 0;
		}

      
		/* Keep playing music: */
      
		if (settings.sys_sound && !Mix_PlayingMusic())
			MusicPlay(musics[MUS_GAME + (rand() % NUM_MUSICS)], 0);
      
		/* Pause (keep frame-rate event) */
                DEBUGCODE
                {
                  fprintf(stderr, "now_time = %d\tlast_time = %d, elapsed time = %d\n",
                          now_time, last_time, now_time - last_time);
                }

		now_time = SDL_GetTicks();
		if (now_time < last_time + FPS)
			SDL_Delay(last_time + FPS - now_time);
	}
	while (!done && !quit);

  
  /* Free backgrounds: */
  FreeBothBkgds();

  /* Stop music: */
  if ((settings.sys_sound) && (Mix_PlayingMusic()))
    Mix_HaltMusic();

  laser_unload_data();

  return 1;
}


/*****************************************************/
/*                                                   */
/*          Local ("private") functions:             */
/*                                                   */
/*****************************************************/


static void calc_city_pos(void)
{
  int i = 0;

  for (i = 0; i < NUM_CITIES; i++)
  {
    if (NUM_CITIES % 2 == 0)
    {
      /* Left vs. Right - makes room for Tux and the console */
      if (i < NUM_CITIES / 2)
      {
        cities[i].x = (((screen->w / (NUM_CITIES + 1)) * i)
                      + ((images[IMG_CITY_BLUE] -> w) / 2));
      }
      else
      {
        cities[i].x = screen->w
                    - screen->w/(NUM_CITIES + 1) * (i - NUM_CITIES/2)
                    + images[IMG_CITY_BLUE]->w/2;
      }
    }
    else
    {
      /* put them in order across the bottom of     *
      * the screen so we can do words in order!!! */
      cities[i].x = i*screen->w/(NUM_CITIES)
                    + images[IMG_CITY_BLUE]->w/2;
    }
  }
}

/* Update the x position of comets when resolution changes: */
/* Must call calc_city_pos() first for this to work! */
static void recalc_comet_pos(void)
{
  int i, target;

  for (i = 0; i < MAX_COMETS; i++)
  {
    /* Set each live comet's x to that of its target city: */
    if (comets[i].alive == 1)
    {
      target = comets[i].city;
      comets[i].x = cities[target].x;
    }
  }
}


/* --- Load all media --- */
static void laser_load_data(void)
{
	int i;

	/* Create the SDL_Surfaces for all of the characters */
        /* used in the word list: */
	RenderLetters(COMET_ZAP_FONT_SIZE);

	/* Load images: */
	for (i = 0; i < NUM_IMAGES; i++) 
		images[i] = LoadImage(image_filenames[i], IMG_ALPHA);
	shield = LoadSprite( "cities/shield", IMG_ALPHA );

	if (settings.sys_sound) {
		for (i = 0; i < NUM_SOUNDS; i++)
			sounds[i] = LoadSound(sound_filenames[i]);

		for (i = 0; i < NUM_MUSICS; i++)
			musics[i] = LoadMusic(music_filenames[i]);
	}

//	PauseLoadMedia();
}


/* --- unload all media --- */
static void laser_unload_data(void) {
	int i;

	FreeLetters();

	for (i = 0; i < NUM_IMAGES; i++)
		SDL_FreeSurface(images[i]);

	if (settings.sys_sound) {
		for (i = 0; i < NUM_SOUNDS; i++)
			Mix_FreeChunk(sounds[i]);
		for (i = 0; i < NUM_MUSICS; i++)
			Mix_FreeMusic(musics[i]);
	}

	FreeSprite(shield);
        shield = NULL;
}


/* Reset stuff for the next level! */

static void laser_reset_level(int diff_level)
{
  char fname[1024];
  static int last_bkgd = -1;
  int i;
  
  /* Clear all comets: */
  
  for (i = 0; i < MAX_COMETS; i++)
    comets[i].alive = 0;
  
  /* Load diffrent random background image: */
  LOG("Loading background in laser_reset_level()\n");

  do {
    i = rand() % NUM_BKGDS;
    DOUT(i);
  }
  while (i == last_bkgd);

  last_bkgd = i;

  DOUT(i);

  sprintf(fname, "backgrounds/%d.jpg", i);

  DEBUGCODE { fprintf(stderr, "Will try to load file:\t%s", fname); }

  FreeBothBkgds(); // LoadBothBkgds() actually does this just in case

  LoadBothBkgds(fname);

  if (CurrentBkgd() == NULL)
  {
    fprintf(stderr,
     "\nWarning: Could not load background image:\n"
     "%s\n"
     "The Simple DirectMedia error that ocurred was: %s\n",
     fname, SDL_GetError());
  }

  /* Record score before this wave: */

  pre_wave_score = score;

  /* Set number of attackers & speed for this wave: */

  switch (diff_level) {
    case 0 : speed = 1 + (wave/5); num_attackers=15; break;
    case 1 : speed = 1 + (wave/4); num_attackers=15; break;
    case 2 : speed = 1 + ((wave<<1)/3); num_attackers=(wave<<1); break;
    case 3 : speed = 1 + wave; num_attackers=(wave<<1); break;
    default: LOG("diff_level not recognized!\n");
  }

  distanceMoved = 100; // so that we don't have to wait to start the level
  LOG("Leaving laser_reset_level()\n");
}


/* Add an comet to the game (if there's room): */

static void laser_add_comet(int diff_level)
{
  int target, location = 0;
  static int last = -1;
  int targeted[NUM_CITIES] = { 0 };
  int add = (rand() % (diff_level + 2));

  LOG ("Entering laser_add_comet()\n");
  DEBUGCODE { fprintf(stderr, "Adding %d comets \n", add); }

  if (0 == NUM_CITIES % 2) /* Even number of cities */
	{
          LOG("NUM_CITIES is even\n");
	  while ((add > 0) && (location != MAX_COMETS))
	  {
            /* Look for a free comet slot: */
            while ((comets[location].alive == 1) && (location < MAX_COMETS))
            {
              location++; 
            }
            if (location < MAX_COMETS)
            {
              comets[location].alive = 1;
              /* Pick a city to attack: */
              do
              { 
                target = (rand() % NUM_CITIES);
              } while (target == last || targeted[target] == 1);

              last = target;
              targeted[target] = 1;

              /* Set comet to target that city: */
              comets[location].city = target; 

              /* Start at the top, above the city in question: */
              comets[location].x = cities[target].x;
              comets[location].y = 0;

              /* Pick a letter */
              comets[location].ch = GetLetter();
              /* single letters always shootable: */
              comets[location].shootable = 1;
              comets[location].next = NULL;

              add--;
            }
            DEBUGCODE {if (location == MAX_COMETS) 
			printf("Location == MAX_COMETS, we have max on screen\n");}
	  } 
	}
	else /* Odd number of cities (is this a hack that means we are using words?) */
        {
          LOG("NUM_CITIES is odd\n");
          wchar_t* word = GetWord();
          int i = 0;
          comet_type* prev_comet = NULL;

          if(!word
             || (wcslen(word) == 0)
             || (wcslen(word) > NUM_CITIES - 1))
          {
            fprintf(stderr, "Error - GetWord() returned NULL, zero-length, or too-long word\n");
            return; 
          }

          DEBUGCODE {fprintf(stderr, "word is: %S\tlength is: %d\n", word, (int)wcslen(word));}
          do
          { 
  	    target = rand() % (NUM_CITIES - wcslen(word) + 1);
          } while (target == last);
          last = target;

		for (i = 0; i < wcslen(word); i++)
		{
 			while ((comets[location].alive == 1) && (location < MAX_COMETS))
				location++; 

  			if (location < MAX_COMETS)
			{
				/* First comet in word is shootable: */
				if (0 == i)
				  comets[location].shootable = 1;
				else
				  comets[location].shootable = 0;

				comets[location].alive = 1;
				comets[location].city = target + i; 
				comets[location].x = cities[target + i].x;
				comets[location].y = 0;
				comets[location].ch = word[i];
				comets[location].word = word;
				comets[location].pos = i;
				comets[location].next = NULL;

				/* Take care of link from previous letter's comet: */
				if (prev_comet)
				  prev_comet->next = &comets[location];
				/* Save pointer for next time through: */
                                prev_comet = &comets[location];

				DEBUGCODE {fprintf(stderr, "Assigning letter to comet: %C\n", word[i]);}
			}
		}
	}
	LOG ("Leaving laser_add_comet()\n");
}


/* Draw numbers/symbols over the attacker: */

static void laser_draw_let(wchar_t c, int x, int y)
{
  /* Draw letter in correct place relative to comet: */
  const int offset_x = -10; /* Values determined by trial and error: */
  const int offset_y = -50;

  SDL_Rect dst;
  SDL_Surface* s;
  dst.x = x + offset_x;
  dst.y = y + offset_y;
  s = GetWhiteGlyph(c);
  if (s)
    SDL_BlitSurface(s, NULL, screen, &dst); 
}


/* Draw status numbers: */

static void laser_draw_numbers(const char* str, int x)
{
  int i, cur_x, c;
  SDL_Rect src, dest;

  cur_x = x;


  /* Draw each character: */
  
  for (i = 0; i < strlen(str); i++)
    {
      c = -1;

      /* Determine which character to display: */
      
      if (str[i] >= '0' && str[i] <= '9')
	c = str[i] - '0';
      

      /* Display this character! */
      
      if (c != -1)
	{
	  src.x = c * (images[IMG_NUMBERS]->w / 10);
	  src.y = 0;
	  src.w = (images[IMG_NUMBERS]->w / 10);
	  src.h = images[IMG_NUMBERS]->h;
	  
	  dest.x = cur_x;
	  dest.y = 0;
	  dest.w = src.w;
	  dest.h = src.h;
	  
	  SDL_BlitSurface(images[IMG_NUMBERS], &src,
			  screen, &dest);


          /* Move the 'cursor' one character width: */

	  cur_x = cur_x + (images[IMG_NUMBERS]->w / 10);
	}
    }
}

/* Draw a line: */

static void laser_draw_line(int x1, int y1, int x2, int y2, int red, int grn, int blu)
{
  int dx, dy, tmp;
  float m, b;
  Uint32 pixel;
  SDL_Rect dest;
 
  pixel = SDL_MapRGB(screen->format, red, grn, blu);

  dx = x2 - x1;
  dy = y2 - y1;

  laser_putpixel(screen, x1, y1, pixel);
  
  if (dx != 0)
  {
    m = ((float) dy) / ((float) dx);
    b = y1 - m * x1;

    if (x2 > x1)
      dx = 1;
    else
      dx = -1;

    while (x1 != x2)
    {
      x1 = x1 + dx;
      y1 = m * x1 + b;
      
      laser_putpixel(screen, x1, y1, pixel);
    }
  }
  else
  {
    if (y1 > y2)
    {
      tmp = y1;
      y1 = y2;
      y2 = tmp;
    }
    
    dest.x = x1;
    dest.y = y1;
    dest.w = 3;
    dest.h = y2 - y1;

    SDL_FillRect(screen, &dest, pixel);
  }
}


/* Draw a single pixel into the surface: */

static void laser_putpixel(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
#ifdef PUTPIXEL_RAW
  int bpp;
  Uint8 * p;
  
  /* Determine bytes-per-pixel for the surface in question: */
  
  bpp = surface->format->BytesPerPixel;
  
  
  /* Set a pointer to the exact location in memory of the pixel
     in question: */
  
  p = (Uint8 *) (surface->pixels +       /* Start at beginning of RAM */
                 (y * surface->pitch) +  /* Go down Y lines */
                 (x * bpp));             /* Go in X pixels */
  
  
  /* Assuming the X/Y values are within the bounds of this surface... */
  
  if (x >= 0 && y >= 0 && x < surface -> w && y < surface -> h)
    {
      /* Set the (correctly-sized) piece of data in the surface's RAM
         to the pixel value sent in: */
      
      if (bpp == 1)
        *p = pixel;
      else if (bpp == 2)
        *(Uint16 *)p = pixel;
      else if (bpp == 3)
        {
          if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
              p[0] = (pixel >> 16) & 0xff;
              p[1] = (pixel >> 8) & 0xff;
              p[2] = pixel & 0xff;
            }
          else
            {
              p[0] = pixel & 0xff;
              p[1] = (pixel >> 8) & 0xff;
              p[2] = (pixel >> 16) & 0xff;
            }
        }
      else if (bpp == 4)
        {
          *(Uint32 *)p = pixel;
        }
    }
#else
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;
  dest.w = 3;
  dest.h = 4;

  SDL_FillRect(surface, &dest, pixel);
#endif
}


/* Draw image at lower center of screen: */

static void laser_draw_console_image(int i)
{
  SDL_Rect dest;

  dest.x = (screen->w - images[i]->w) / 2;
  dest.y = (screen->h - images[i]->h);
  dest.w = images[i]->w;
  dest.h = images[i]->h;

  SDL_BlitSurface(images[i], NULL, screen, &dest);
}


/* Increment score: */

static void laser_add_score(int inc)
{
  score += inc;
  if (score < 0) score = 0;
}


/* Stop annoncing thread safely */
static void stop_tts_announcer()
{
	tts_announcer_switch = 0;
}


/* This function will announce the bottum most word and 
 * it's remaining letters */
static int tts_announcer(void *unused)
{
	int lowest,lowest_y,i,iter;
	wchar_t buffer[3000];
	int pitch_and_rate;
	tts_announcer_switch = 1;
	while(1)
	{
		if(tts_announcer_switch == 0)
			goto end;
		else if(tts_announcer_switch == 2)
		{
			T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"Score %d!",score);
			SDL_Delay(20);
			T4K_Tts_wait();
			tts_announcer_switch = 1;
		}
		else if(tts_announcer_switch == 3)
		{
			T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%d cities alive!",num_cities_alive);
			SDL_Delay(20);
			T4K_Tts_wait();
			tts_announcer_switch = 1;
		}		
		else if(tts_announcer_switch == 4)
		{
			T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"on wave %d!",wave);
			SDL_Delay(20);
			T4K_Tts_wait();
			tts_announcer_switch = 1;
		}

			
		
		//Detecting the lowest letter and word on screen		
		lowest_y = 0;
		lowest = -1;	
		for (i = 0; i < MAX_COMETS; i++)
		{
			if (comets[i].alive  &&
			 comets[i].shootable  &&
			  comets[i].expl == 0  &&
			   comets[i].y > lowest_y)
			{
				lowest = i;
				lowest_y = comets[i].y;
			}
		}
		//Skipping if no letter found in screen
		if (lowest == -1)
			continue;
		
		buffer[0] = L'\0';
		//Adding the word to buffer
		wcscpy(buffer,comets[lowest].word);
		iter = wcslen(comets[lowest].word);
		
		//Appending each letters from correct_position if word is not alphabet
		if (1<wcslen(comets[lowest].word))
		{
			for(i=comets[lowest].pos;i<wcslen(comets[lowest].word);i++)
			{
				buffer[iter]=L'.';iter++;
				buffer[iter]=L' ';iter++;				
				buffer[iter]=comets[lowest].word[i];iter++;
			}
		}
		buffer[iter]=L'.';iter++;
		buffer[iter]=L' ';iter++;		
		buffer[iter] = L'\0';

		pitch_and_rate = ((lowest_y*100)/(screen->h - images[IMG_CITY_BLUE]->h));
		if (pitch_and_rate < 30)
			pitch_and_rate = 30;
		if (pitch_and_rate > 60)
			pitch_and_rate = 60;	
		T4K_Tts_say(pitch_and_rate,pitch_and_rate,INTERRUPT,"%S",buffer);
		
		//Wait to finish saying the previus word
		SDL_WaitThread(tts_thread,NULL);
		SDL_Delay(100);
		fprintf(stderr,"\nPos = %d",braille_letter_pos);
			
	}
	end:
	return 1;
}
