/*
   titlescreen.c:

   Splash, background and title screen items.

   Copyright 2000, 2003, 2006, 2007, 2008, 2009, 2010, 2011.
Authors: Sam Hart, Jesse Andrews, David Bruce, Tim Holy,
Brendan Luchen, Boleslaw Kulbabinski, Wenyuan Guo
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

titlescreen.c is part of "Tux, of Math Command", a.k.a. "tuxmath".

Tuxmath is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tuxmath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "titlescreen.h"

#include <t4k_common.h>


#include "menu.h"


/* --- Data Structure for Dirty Blitting --- */
SDL_Rect srcupdate[MAX_UPDATES];
SDL_Rect dstupdate[MAX_UPDATES];
int numupdates = 0; // tracks how many blits to be done

struct blit {
    SDL_Surface *src;
    SDL_Rect *srcrect;
    SDL_Rect *dstrect;
    unsigned char type;
};
static struct blit blits[MAX_UPDATES];


// Lessons available for play
char **lesson_list_titles = NULL;
char **lesson_list_filenames = NULL;
int num_lessons = 0;

/*TODO: move these constants into a config file
  (together with menu.c constants ? ) */
const float title_pos[4] = {0.0, 0.0, 0.3, 0.25};
const float tux_pos[4]   = {0.0, 0.6, 0.3, 0.4};
const char* bkg_path     = "title/menu_bkg.jpg";
const char* standby_path = "status/standby.svg";
const char* title_path   = "title/title1.svg";
const char* egg_path     = "title/egg.svg";
const char* tux_path     = "tux/bigtux";
/* beak coordinates relative to tux rect */
const float beak_pos[4]  = {0.36, 0.21, 0.27, 0.14};

/* How long we show startup logo while files load, etc.: */
const int logo_msec = 2000;

//SDL_Event event;

/* screen dimensions to which titlescreen graphics are currently rendered */
int curr_res_x = -1;
int curr_res_y = -1;

/* titlescreen items */
SDL_Surface* win_bkg = NULL;
SDL_Surface* fs_bkg = NULL;

SDL_Surface* logo = NULL;
sprite* Tux = NULL;
SDL_Surface* title = NULL;

/* "Easter Egg" cursor */
SDL_Surface* egg = NULL;
int egg_active = 0; //are we currently using the egg cursor?

/* locations we need */
SDL_Rect bkg_rect,
         logo_rect,
         tux_rect,
         title_rect,
         cursor,
         beak;

/* This syntax is full of fluffy kittens! (note: kittens sold separately) */
SDL_Surface* current_bkg()
{ 
    if (T4K_GetScreen()->flags & SDL_FULLSCREEN)
        return fs_bkg;
    return win_bkg; 
}

/* FIXME this function assumes the background is properly scaled */
/* Also, it only sets the image for the current resolution, not  */
/* the "other" one.                                              */
void set_current_bkg(SDL_Surface* new_bkg)
{
    if(screen->flags & SDL_FULLSCREEN)
    {
        if(fs_bkg != NULL)
            SDL_FreeSurface(fs_bkg);
        fs_bkg = new_bkg;
    }
    else
    {
        if(win_bkg != NULL)
            SDL_FreeSurface(win_bkg);
        win_bkg = new_bkg;
    }
}

/* Local function prototypes: */
void NotImplemented(void);

void free_titlescreen(void);

void trans_wipe(SDL_Surface* newbkg, int type, int var1, int var2);
void init_blits(void);
void update_screen(int* frame);
void add_rect(SDL_Rect* src, SDL_Rect* dst);

int handle_easter_egg(const SDL_Event* evt);



const int debug_titlescreen = 1;

Mix_Music* sounds[NUM_SOUNDS];
SDL_Surface* images[NUM_IMAGES];
int load_image_data();


/***********************************************************/
/*                                                         */
/*       "Public functions" (callable throughout program)  */
/*                                                         */
/***********************************************************/


/* Display Tux4Kids logo, then animate title screen
   items onto the screen and run main menu */
void TitleScreen(void)
{
    Uint32 start_time = 0;
    SDL_Rect tux_anim, title_anim;
    int i, tux_pix_skip, title_pix_skip;

	if (settings.sys_sound)
	{
		settings.menu_sound = 1;
		settings.menu_music = 1;
	}

    fprintf(stderr,DATA_PREFIX);
    
    
    load_image_data();
    load_sound_data();


    

    /* We show the logo until two seconds from this time */
    start_time = SDL_GetTicks();

    /* display the Standby screen */
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    logo = T4K_LoadImage(standby_path, IMG_REGULAR);
    if(logo)
    {
        /* Center horizontally and vertically */
        logo_rect.x = (screen->w - logo->w) / 2;
        logo_rect.y = (screen->h - logo->h) / 2;
        logo_rect.w = logo->w;
        logo_rect.h = logo->h;

        SDL_BlitSurface(logo, NULL, screen, &logo_rect);
        SDL_FreeSurface(logo);
    }

    SDL_UpdateRect(screen, 0, 0, 0, 0);

    /* Play "harp" greeting sound lifted from Tux Paint */
    playsound(SND_HARP);

    /* load menus */
    LoadMenus();

    /* load backgrounds */
    T4K_LoadBothBkgds(bkg_path, &fs_bkg, &win_bkg);
    T4K_SetMenuSounds(NULL, sounds[SND_POP], sounds[SND_TOCK]);
    T4K_OnResolutionSwitch(&HandleTitleScreenResSwitch);

    if(fs_bkg == NULL || win_bkg == NULL)
    {
        fprintf(stderr, "Backgrounds were not properly loaded, exiting");
        if(fs_bkg)
            SDL_FreeSurface(fs_bkg);
        if(win_bkg)
            SDL_FreeSurface(win_bkg);
        return;
    }

    /* load titlescreen images */
    if(RenderTitleScreen() == 0)
    {
        fprintf(stderr, "Media was not properly loaded, exiting");
        return;
    }

    /* --- wait  --- */
    while ((SDL_GetTicks() - start_time) < logo_msec)
    {
        /* Check to see if user pressed escape */
        if (SDL_PollEvent(&event)
                && event.type==SDL_KEYDOWN
                && event.key.keysym.sym == SDLK_ESCAPE)
        {
            return;
        }
        SDL_Delay(50);
    }

    /* NOTE: do we need this ? */
    if (true)
        SDL_WM_GrabInput(SDL_GRAB_OFF); /* in case of a freeze, this traps the cursor */
    else  // NOTE- the accompanying "if" is inside the DEBUGCODE_TT macro
        SDL_WM_GrabInput(SDL_GRAB_ON);  /* User input goes to TuxMath, not window manager */
    SDL_ShowCursor(1);


    /* Tux and Title animations */
    DEBUGMSG(debug_titlescreen, "TitleScreen(): Now Animating Tux and Title onto the screen\n" );

    /* Draw background (center it if it's smaller than screen) */
    if(current_bkg())
    {
        /* FIXME not sure trans_wipe() works in Windows: */
        T4K_TransWipe(current_bkg(), RANDOM_WIPE, 5, 20);

        DEBUGCODE(debug_all)
        {
            /* Make sure background gets drawn (since trans_wipe() doesn't */
            /* seem to work reliably as of yet):                          */
            SDL_BlitSurface(current_bkg(), NULL, screen, &bkg_rect);
        }
    }

    /* --- Pull tux & logo onscreen --- */
    if(title && Tux && Tux->frame[0])
    {
        Uint32 timer = 0;
        /* final tux & title positioins are already calculated,
           start outside the screen */
        tux_anim = tux_rect;
        tux_anim.y = screen->h;

        title_anim = title_rect;
        title_anim.x = screen->w;

        for(i = 0; i < ANIM_FRAMES; i++)
        {
            /* Draw the entire background, over a black screen if necessary */
            if(current_bkg()->w != screen->w || current_bkg()->h != screen->h)
                SDL_FillRect(screen, &screen->clip_rect, 0);

            SDL_BlitSurface(current_bkg(), NULL, screen, &bkg_rect);

            /* calculate shifts */
            tux_pix_skip = (tux_anim.y - tux_rect.y) / (ANIM_FRAMES - i);
            tux_anim.y -= tux_pix_skip;
            title_pix_skip = (title_anim.x - title_rect.x) / (ANIM_FRAMES - i);
            title_anim.x -= title_pix_skip;

            /* update screen */
            SDL_BlitSurface(Tux->frame[0], NULL, screen, &tux_anim);
            SDL_BlitSurface(title, NULL, screen, &title_anim);

            SDL_UpdateRect(screen, tux_anim.x, tux_anim.y, tux_anim.w,
                    min(tux_anim.h + tux_pix_skip, screen->h - tux_anim.y));
            SDL_UpdateRect(screen, title_anim.x, title_anim.y,
                    min(title_anim.w + title_pix_skip, screen->w - title_anim.x), title_anim.h);

            T4K_Throttle(1000/ANIM_FPS, &timer);
        }
    }

    DEBUGMSG(debug_titlescreen, "TitleScreen(): Tux and Title are in place now\n");
    // Start playing menu music if desired: 
    
    
    if (settings.menu_music)
    {
        T4K_AudioMusicLoad("tuxi.ogg", -1);
    }

    /* If necessary, have the user log in */
    //if (RunLoginMenu() != -1) {
        /* Finish parsing user options */
       // initialize_options_user();
        /* Start the main menu */
        RunMainMenu();
    //}

    /* User has selected quit, clean up */
    DEBUGMSG(debug_titlescreen, "TitleScreen(): Freeing title screen images\n");
    free_titlescreen();

    DEBUGMSG(debug_titlescreen, "leaving TitleScreen()\n");
}

void DrawTitleScreen(void)
{
    SDL_BlitSurface(current_bkg(), NULL, screen, &bkg_rect);
    SDL_BlitSurface(Tux->frame[0], NULL, screen, &tux_rect);
    SDL_BlitSurface(title, NULL, screen, &title_rect);
    //SDL_UpdateRect(screen, 0, 0, 0, 0);
}

/* Render and position all titlescreen items to match current
   screen size. Rendering is done only if needed.
   This function must be called after every resolution change
   returns 1 on success, 0 on failure */
int RenderTitleScreen(void)
{
    SDL_Surface* new_bkg = NULL;

    if(curr_res_x != T4K_GetScreen()->w || curr_res_y != T4K_GetScreen()->h)
    {
        /* we need to rerender titlescreen items */
        DEBUGMSG(debug_titlescreen, "Re-rendering titlescreen items.\n");

        /* we keep two backgrounds to make screen mode switch faster */
        if(current_bkg()->w != T4K_GetScreen()->w || current_bkg()->h != T4K_GetScreen()->h)
        {
            new_bkg = T4K_LoadBkgd(bkg_path, T4K_GetScreen()->w, T4K_GetScreen()->h);
            if(new_bkg == NULL)
            {
                DEBUGMSG(debug_titlescreen, "RenderTitleScreen(): Failed to load new background.\n");
                return 0;
            }
            else
            {
                DEBUGMSG(debug_titlescreen, "RenderTitleScreen(): New background loaded.\n");
                set_current_bkg(new_bkg);
            }
        }

        bkg_rect = current_bkg()->clip_rect;
        bkg_rect.x = (screen->w - bkg_rect.w) / 2;
        bkg_rect.y = (screen->h - bkg_rect.h) / 2;

        /* Tux in lower left corner of the screen */
        T4K_SetRect(&tux_rect, tux_pos);
        Tux = T4K_LoadSpriteOfBoundingBox(tux_path, IMG_ALPHA, tux_rect.w, tux_rect.h);
        if(Tux && Tux->frame[0])
        {
            tux_rect.w = Tux->frame[0]->clip_rect.w;
            tux_rect.h = Tux->frame[0]->clip_rect.h;
        }
        else
        {
            DEBUGMSG(debug_titlescreen, "RenderTitleScreen(): Failed to load Tux image.\n");
            return 0;
        }

        /* "Tux, of math command" title in upper right corner */
        T4K_SetRect(&title_rect, title_pos);
        title = T4K_LoadImageOfBoundingBox(title_path, IMG_ALPHA, title_rect.w, title_rect.h);
        if(title)
        {
            title_rect.w = title->clip_rect.w;
            title_rect.h = title->clip_rect.h;
        }
        else
        {
            DEBUGMSG(debug_titlescreen, "RenderTitleScreen(): Failed to load title image.\n");
            return 0;
        }

        /* easter egg */
#ifdef HAVE_RSVG
        egg = T4K_LoadImage(egg_path, IMG_ALPHA | IMG_NOT_REQUIRED);
#else
        egg = T4K_LoadImage(egg_path, IMG_COLORKEY | IMG_NOT_REQUIRED);
#endif

        beak.x = tux_rect.x + beak_pos[0] * tux_rect.w;
        beak.y = tux_rect.y + beak_pos[1] * tux_rect.h;
        beak.w = beak_pos[2] * tux_rect.w;
        beak.h = beak_pos[3] * tux_rect.h;

        curr_res_x = screen->w;
        curr_res_y = screen->h;

        DEBUGMSG(debug_titlescreen, "Leaving RenderTitleScreen().\n");
    }
    return 1;
}


/* handle titlescreen events (easter egg)
   this function should be called from event loops
   return 1 if events require full redraw */
int HandleTitleScreenEvents(const SDL_Event* evt)
{
    if (evt->type == SDL_KEYDOWN)
        if (evt->key.keysym.sym == SDLK_F10)
            HandleTitleScreenResSwitch(T4K_GetScreen()->w, T4K_GetScreen()->h);

    return handle_easter_egg(evt);
}

/* handle a resolution switch. Tux et. al. may need to be resized
   and/or repositioned
   */
int HandleTitleScreenResSwitch(int new_w, int new_h)
{
    return RenderTitleScreen();
}

/* handle all titlescreen blitting
   this function should be called after every animation frame */
/* NOTE we now allow the animation to optionally be started
 * at the beginning.  The "old" is now a wrapper for this. */
void HandleTitleScreenAnimations_Reset(bool reset)
{
    static int frame_counter;
    int tux_frame;

    /* If 'reset' is true, we start the animation over so Tux
     * gets redrawn immediately:
     */
    if(reset)
        frame_counter = 0;

    /* --- make Tux blink --- */
    switch (frame_counter % TUX6)
    {
        case 0:    tux_frame = 1; break;
        case TUX1: tux_frame = 2; break;
        case TUX2: tux_frame = 3; break;
        case TUX3: tux_frame = 4; break;
        case TUX4: tux_frame = 3; break;
        case TUX5: tux_frame = 2; break;
        default: tux_frame = 0;
    }

    if (Tux && tux_frame)
    {
        /* Redraw background to keep edges anti-aliased properly: */
        SDL_BlitSurface(current_bkg(),&tux_rect, screen, &tux_rect);
        SDL_BlitSurface(Tux->frame[tux_frame - 1], NULL, screen, &tux_rect);
        T4K_UpdateRect(screen, &tux_rect);
    }

    if (egg_active) { //if we need to, draw the egg cursor
        //who knows why GetMouseState() doesn't take Sint16's...
        SDL_GetMouseState((int*)(&cursor.x), (int*)(&cursor.y));
        cursor.x -= egg->w / 2; //center vertically
        SDL_BlitSurface(egg, NULL, screen, &cursor);
        T4K_UpdateRect(screen, &cursor);
    }

    frame_counter++;
}


void HandleTitleScreenAnimations(void)
{
    HandleTitleScreenAnimations_Reset(false);
}

/***********************************************************/
/*                                                         */
/*    "Private functions" (callable only from this file)   */
/*                                                         */
/***********************************************************/


void free_titlescreen(void)
{
    DEBUGMSG(debug_titlescreen, "Entering free_titlescreen()\n");

    T4K_FreeSprite(Tux);
    Tux = NULL;

    if(egg)
    {
        SDL_FreeSurface(egg);
        egg = NULL;
    }

    if(title)
    {
        SDL_FreeSurface(title);
        title = NULL;
    }

    if(fs_bkg)
    {
        SDL_FreeSurface(fs_bkg);
        fs_bkg = NULL;
    }

    if(win_bkg)
    {
        SDL_FreeSurface(win_bkg);
        win_bkg = NULL;
    }
}



void NotImplemented(void)
{
    ShowMessageWrap(DEFAULT_MENU_FONT_SIZE, _("Work In Progress!\n"
                "This feature is not ready yet\n"
                "Discuss the future of TuxMath at\n"
                "tuxmath-devel@lists.sourceforge.net"));
}

/* FIXME this function segfaults if passed a string with
 * a leading '\n'
 */
void ShowMessageWrap( int font_size, const char* str )
{
    SDL_Surface *s1 = NULL;
    SDL_Surface *arrow;

    SDL_Rect loc;
    SDL_Rect srcleft,srcright;
    SDL_Rect rleft,rright;
    SDL_Rect rtext;

    SDL_Event event;

    char strings[MAX_LINES][MAX_LINEWIDTH];

    int i;
    int nline; 
    int finished = 0;
    int inprogress = 1;
    int page = 0; 
    int maxline;
    Uint32 timer = 0;

    if(screen->flags & SDL_FULLSCREEN)
        nline = T4K_LineWrap( str, strings, 70, MAX_LINES, MAX_LINEWIDTH );
    else
        nline = T4K_LineWrap( str, strings, 35, MAX_LINES, MAX_LINEWIDTH );

	/* Announcing the message */
	T4K_Tts_say(DEFAULT_VALUE,DEFAULT_VALUE,INTERRUPT,"%s",str);
	
    while(inprogress)
    {
        loc.x = screen->w * 0.25;
        loc.y = screen->h * 0.1;
        loc.w = screen->w * 0.5;
        loc.h = screen->h * 0.8;

        finished = 0;

        /* calculate the max number of line */
        maxline = loc.h / font_size * 0.4;

        DrawTitleScreen();

        if (stop_button)
        {
            SDL_BlitSurface(stop_button, NULL, screen, &stop_rect);
        }
        T4K_DrawButton( &loc, 50, SEL_RGBA );

        /* more than one page */
        if(nline > maxline)
        {
            arrow = images[IMG_ARROWS];

            rleft.x = loc.x + (loc.w/2-50); 
            rleft.y = loc.y + loc.h;

            rright.x = rleft.x + 50;
            rright.y = rleft.y; 

            /* init rect for left button */
            srcleft.y = 0;
            srcleft.w = 40;
            srcleft.h = 40;

            /* init rect for right button */
            srcright.y = 0;
            srcright.w = 40;
            srcright.h = 40;

            /* first page */
            if(page == 0)
            {
                srcleft.x = 0;
            }
            else
            {
                srcleft.x = 40;
            }

            /* last page */
            if(page*maxline+maxline>nline)
            {
                srcright.x = 80;
            }
            else
            {
                srcright.x = 120;
            }

            /* page arrows */
            SDL_BlitSurface(arrow, &srcleft, screen, &rleft);
            SDL_BlitSurface(arrow, &srcright, screen, &rright);
        }

        //rtext.x = loc.x + 10;
        rtext.y = loc.y;    
        for(i = page * maxline; i < nline && i - page * maxline < maxline; i++)
        {
            s1 = T4K_BlackOutline(strings[i], font_size, &white);

            if( s1 )
            {
                /* Center text horizontally: */
                rtext.x = loc.x + loc.w/2 - s1->w/2;
                rtext.y += (s1->h+15);  
                SDL_BlitSurface( s1, NULL, screen, &rtext );

                SDL_FreeSurface( s1 );
                s1 = NULL;
            }
        }

        SDL_UpdateRect( screen, 0, 0, 0, 0 );

        while(!finished)
        {
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        {
                            //cleanup();
                        }
                    case SDL_MOUSEBUTTONDOWN:
                        {
                            /* close button pressed */
                            if(T4K_inRect(stop_rect, event.button.x, event.button.y ))
                            {
                                finished = 1;
                                inprogress = 0;
                                playsound(SND_TOCK);
                                break;
                            }

                            /* left arrow button pressed */
                            if(T4K_inRect(rleft, event.button.x, event.button.y))
                            {
                                DEBUGMSG(debug_titlescreen, "You clicked the left arrow.\n" );
                                if(page>0)
                                    page--;
                                finished = 1;
                            }
                            /* right arrow button pressed */
                            else if(T4K_inRect(rright, event.button.x, event.button.y))
                            {
                                DEBUGMSG(debug_titlescreen, "You clicked the right arrow.\n" );
                                if(page*maxline+maxline<nline)
                                    page++; 
                                finished = 1;
                            }
                            else
                            {
                                DEBUGMSG(debug_titlescreen, "You clicked the outside.\n" );
                                finished = 1;
                                inprogress = 0;  
                            }
                        }
                    case SDL_KEYDOWN:
                        {
                            switch( event.key.keysym.sym )
                            { 
                                case SDLK_LEFT:
                                    {
                                        if(page>0)
                                            page--;
                                        finished = 1;
                                        break;
                                    }
                                case SDLK_RIGHT:
                                    {
                                        if(page*maxline+maxline<nline)
                                            page++; 
                                        finished = 1;
                                        break;
                                    }
                                case SDLK_q:
                                    {
                                        finished = 1;
                                        inprogress = 0;
                                    }
                                default:
                                    {
                                        finished = 1;
                                        inprogress = 0; 
                                        playsound(SND_TOCK);
                                    }
                            }
                        } 
                }
                /* Don't eat all CPU: */
                T4K_Throttle(20, &timer);
            }
            T4K_Throttle(20, &timer);
        }
        T4K_Throttle(20, &timer);
    }
}

void ShowMessage(int font_size, const char* str1, const char* str2,
        const char* str3, const char* str4)
{
    SDL_Surface *s1, *s2, *s3, *s4;
    SDL_Rect loc;
    int finished = 0;
    Uint32 timer = 0;

    /* To adjust font size: */
    float scale = screen->w / 640;

    s1 = s2 = s3 = s4 = NULL;

    DEBUGMSG(debug_titlescreen, "ShowMessage() - creating text\n" );

    if (str1)
        s1 = T4K_BlackOutline(str1, font_size * scale, &white);
    if (str2)
        s2 = T4K_BlackOutline(str2, font_size * scale, &white);
    if (str3)
        s3 = T4K_BlackOutline(str3, font_size * scale, &white);
    if (str4)
        s4 = T4K_BlackOutline(str4, font_size * scale, &white);

    DEBUGMSG(debug_titlescreen, "ShowMessage() - drawing screen\n" );

    DrawTitleScreen();
    /* Red "Stop" circle in upper right corner to go back to main menu: */
    if (stop_button)
        SDL_BlitSurface(stop_button, NULL, screen, &stop_rect);

    /* Draw shaded background for better legibility: */ 
    loc.x = screen->w * 0.25;
    loc.y = screen->h * 0.1;
    loc.w = screen->w * 0.5;
    loc.h = screen->h * 0.8;
    T4K_DrawButton(&loc, 50, SEL_RGBA);


    /* Draw lines of text (do after drawing Tux so text is in front): */
    if (s1)
    {
        loc.x = (screen->w / 2) - (s1->w/2); loc.y = screen->h * 0.2;
        SDL_BlitSurface( s1, NULL, screen, &loc);
    }
    if (s2)
    {
        loc.x = (screen->w / 2) - (s2->w/2); loc.y = screen->h * 0.35;
        SDL_BlitSurface( s2, NULL, screen, &loc);
    }
    if (s3)
    {
        //loc.x = 320 - (s3->w/2); loc.y = 300;
        loc.x = (screen->w / 2) - (s3->w/2); loc.y = screen->h * 0.5;
        SDL_BlitSurface( s3, NULL, screen, &loc);
    }
    if (s4)
    {
        //loc.x = 320 - (s4->w/2); loc.y = 340;
        loc.x = (screen->w / 2) - (s4->w/2); loc.y = screen->h * 0.65;
        SDL_BlitSurface( s4, NULL, screen, &loc);
    }

    /* and update: */
    SDL_UpdateRect(screen, 0, 0, 0, 0);

    while (!finished)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    {
                        //cleanup();
                    }

                case SDL_MOUSEBUTTONDOWN:
                    /* "Stop" button - go to main menu: */
                    {
                        if (T4K_inRect(stop_rect, event.button.x, event.button.y ))
                        {
                            finished = 1;
                            playsound(SND_TOCK);
                            break;
                        }
                    }
                case SDL_KEYDOWN:
                    {
                        finished = 1;
                        playsound(SND_TOCK);
                    }
            }
        }

        HandleTitleScreenAnimations();

        /* Wait so we keep frame rate constant: */
        T4K_Throttle(20, &timer);
    }  // End of while (!finished) loop

    SDL_FreeSurface(s1);
    SDL_FreeSurface(s2);
    SDL_FreeSurface(s3);
    SDL_FreeSurface(s4);
}

/* Was in playgame.c in tuxtype: */

/* trans_wipe: Performs various wipes to new bkgs
   Given a wipe request type, and any variables
   that wipe requires, will perform a wipe from
   the current screen image to a new one. */
void trans_wipe(SDL_Surface* newbkg, int type, int var1, int var2)
{
    int i, j, x1, x2, y1, y2;
    int step1, step2, step3, step4;
    int frame;
    SDL_Rect src;
    SDL_Rect dst;

    if (!screen)
    {
        DEBUGMSG(debug_titlescreen, "trans_wipe(): screen not valid!\n");
        return;
    }

    if (!newbkg)
    {
        DEBUGMSG(debug_titlescreen, "trans_wipe(): newbkg not valid!\n");
        return;
    }

    init_blits();

    numupdates = 0;
    frame = 0;

    if(newbkg->w == screen->w && newbkg->h == screen->h) {
        if( type == RANDOM_WIPE )
            type = (RANDOM_WIPE * ((float) rand()) / (RAND_MAX+1.0));

        switch( type ) {
            case WIPE_BLINDS_VERT: {
                                       DEBUGMSG(debug_titlescreen, "trans_wipe(): Doing 'WIPE_BLINDS_VERT'\n");
                                       /*var1 isnum ofdivisions
                                         var2is howmany framesanimation shouldtake */
                                       if(var1 <1 )var1 =1;
                                       if( var2< 1) var2= 1;
                                       step1= screen->w/ var1;
                                       step2= step1/ var2;

                                       for(i= 0;i <=var2; i++)
                                       {
                                           for(j= 0;j <=var1; j++)
                                           {
                                               x1= step1* (j- 0.5)- i* step2+ 1;
                                               x2= step1* (j- 0.5)+ i* step2+ 1;
                                               src.x= x1;
                                               src.y= 0;
                                               src.w= step2;
                                               src.h= screen->h;
                                               dst.x= x2;
                                               dst.y= 0;
                                               dst.w= step2;
                                               dst.h= screen->h;

                                               SDL_BlitSurface(newbkg,&src, screen,&src);
                                               SDL_BlitSurface(newbkg, &dst,screen, &dst);

                                               add_rect(&src,&src);
                                               add_rect(&dst, &dst);
                                           }
                                           update_screen(&frame);
                                       }

                                       src.x= 0;
                                       src.y= 0;
                                       src.w= screen->w;
                                       src.h= screen->h;
                                       SDL_BlitSurface(newbkg,NULL, screen,&src);
                                       SDL_Flip(screen);

                                       break;
                                   }
            case WIPE_BLINDS_HORIZ:{
                                       DEBUGMSG(debug_titlescreen, "trans_wipe(): Doing 'WIPE_BLINDS_HORIZ'\n");
                                       /* var1is numof divisions
                                          var2 ishow manyframes animationshould take*/
                                       if( var1< 1) var1= 1;
                                       if(var2 <1 )var2 =1;
                                       step1 =screen->h /var1;
                                       step2 =step1 /var2;

                                       for(i =0; i<= var2;i++) {
                                           for(j= 0;j <=var1; j++){
                                               y1 =step1 *(j -0.5) -i *step2 +1;
                                               y2 =step1 *(j -0.5) +i *step2 +1;
                                               src.x =0;
                                               src.y =y1;
                                               src.w =screen->w;
                                               src.h =step2;
                                               dst.x =0;
                                               dst.y =y2;
                                               dst.w =screen->w;
                                               dst.h =step2;

                                               SDL_BlitSurface(newbkg, &src,screen, &src);
                                               SDL_BlitSurface(newbkg,&dst, screen,&dst);

                                               add_rect(&src, &src);
                                               add_rect(&dst,&dst);
                                           }
                                           update_screen(&frame);
                                       }

                                       src.x =0;
                                       src.y =0;
                                       src.w =screen->w;
                                       src.h =screen->h;
                                       SDL_BlitSurface(newbkg, NULL,screen, &src);
                                       SDL_Flip(screen);

                                       break;
                                   }
            case WIPE_BLINDS_BOX:{
                                     DEBUGMSG(debug_titlescreen, "trans_wipe(): Doing 'WIPE_BLINDS_BOX'\n");
                                     /* var1is numof divisions
                                        var2 ishow manyframes animationshould take*/
                                     if( var1< 1) var1= 1;
                                     if(var2 <1 )var2 =1;
                                     step1 =screen->w /var1;
                                     step2 =step1 /var2;
                                     step3 =screen->h /var1;
                                     step4 =step1 /var2;

                                     for(i =0; i<= var2;i++) {
                                         for(j= 0;j <=var1; j++){
                                             x1 =step1 *(j -0.5) -i *step2 +1;
                                             x2 =step1 *(j -0.5) +i *step2 +1;
                                             src.x =x1;
                                             src.y =0;
                                             src.w =step2;
                                             src.h =screen->h;
                                             dst.x =x2;
                                             dst.y =0;
                                             dst.w =step2;
                                             dst.h =screen->h;

                                             SDL_BlitSurface(newbkg, &src,screen, &src);
                                             SDL_BlitSurface(newbkg,&dst, screen,&dst);

                                             add_rect(&src, &src);
                                             add_rect(&dst,&dst);
                                             y1 =step3 *(j -0.5) -i *step4 +1;
                                             y2 =step3 *(j -0.5) +i *step4 +1;
                                             src.x =0;
                                             src.y =y1;
                                             src.w =screen->w;
                                             src.h =step4;
                                             dst.x =0;
                                             dst.y =y2;
                                             dst.w =screen->w;
                                             dst.h =step4;
                                             SDL_BlitSurface(newbkg, &src,screen, &src);
                                             SDL_BlitSurface(newbkg,&dst, screen,&dst);
                                             add_rect(&src, &src);
                                             add_rect(&dst,&dst);
                                         }
                                         update_screen(&frame);
                                     }

                                     src.x =0;
                                     src.y =0;
                                     src.w =screen->w;
                                     src.h =screen->h;
                                     SDL_BlitSurface(newbkg, NULL,screen, &src);
                                     SDL_Flip(screen);

                                     break;
                                 }
            default:
                                 break;
        }
    }
    DEBUGMSG(debug_titlescreen, "trans_wipe(): FINISH\n");
}

/* InitEngine - Set up the update rectangle pointers
   (user by trans_wipe() ) */
void init_blits(void) {
    int i;

    for (i = 0; i < MAX_UPDATES; ++i) {
        blits[i].srcrect = &srcupdate[i];
        blits[i].dstrect = &dstupdate[i];
    }
}


/* update_screen : Update the screen and increment the frame num
   (used by trans_wipe() ) */
void update_screen(int *frame) {
    int i;

    /* -- First erase everything we need to -- */
    for (i = 0; i < numupdates; i++)
        if (blits[i].type == 'E')
            SDL_LowerBlit(blits[i].src, blits[i].srcrect, screen, blits[i].dstrect);
    //        SNOW_erase();

    /* -- then draw -- */
    for (i = 0; i < numupdates; i++)
        if (blits[i].type == 'D')
            SDL_BlitSurface(blits[i].src, blits[i].srcrect, screen, blits[i].dstrect);
    //        SNOW_draw();

    /* -- update the screen only where we need to! -- */
    //        if (SNOW_on)
    //                SDL_UpdateRects(screen, SNOW_add( (SDL_Rect*)&dstupdate, numupdates ), SNOW_rects);
    //        else
    SDL_UpdateRects(screen, numupdates, dstupdate);

    numupdates = 0;
    *frame = *frame + 1;
}


/* add_rect: Don't actually blit a surface,
   but add a rect to be updated next update
   (used by trans_wipe() ) */
void add_rect(SDL_Rect* src, SDL_Rect* dst) {
    /*borrowed from SL's alien (and modified)*/

    struct blit *update;

    if (!src || !dst)
    {
        DEBUGMSG(debug_titlescreen, "add_rect(): src or dst invalid!\n");
        return;
    }

    update = &blits[numupdates++];

    update->srcrect->x = src->x;
    update->srcrect->y = src->y;
    update->srcrect->w = src->w;
    update->srcrect->h = src->h;
    update->dstrect->x = dst->x;
    update->dstrect->y = dst->y;
    update->dstrect->w = dst->w;
    update->dstrect->h = dst->h;
    update->type = 'I';
}

int handle_easter_egg(const SDL_Event* evt)
{
    static int eggtimer = 0;
    int tuxframe;

    // Avoid segfaults if needed images not available:
    if (!Tux || !egg)
    {
        fprintf(stderr,
                "handle_easter_egg() - needed images not avail, bailing out\n");
        egg_active = 0;
        return 1;
    }

    tuxframe = Tux->num_frames;

    if (egg_active) //are we using the egg cursor?
    {

        if (eggtimer < SDL_GetTicks() ) //time's up
        {
            SDL_ShowCursor(SDL_ENABLE);
            //SDL_FillRect(screen, &cursor, 0);
            SDL_BlitSurface(current_bkg(), NULL, screen, &bkg_rect); //cover egg up once more
            SDL_WarpMouse(cursor.x, cursor.y);
            SDL_UpdateRect(screen, cursor.x, cursor.y, cursor.w, cursor.h); //egg->x, egg->y, egg->w, egg->h);
            egg_active = 0;
        }
        return 1;
    }
    else //if not, see if the user clicked Tux's beak
    {
        eggtimer = 0;
        if (evt->type == SDL_MOUSEBUTTONDOWN &&
                T4K_inRect(beak, evt->button.x, evt->button.y) )
        {
            SDL_ShowCursor(SDL_DISABLE);

            //animate
            while (tuxframe != 0)
            {
                SDL_BlitSurface(current_bkg(), &tux_rect, screen, &tux_rect);
                SDL_BlitSurface(Tux->frame[--tuxframe], NULL, screen, &tux_rect);
                SDL_UpdateRect(screen, tux_rect.x, tux_rect.y, tux_rect.w, tux_rect.h);
                SDL_Delay(GOBBLE_ANIM_MS / Tux->num_frames);
            }

            eggtimer = SDL_GetTicks() + EASTER_EGG_MS;
            egg_active = 1;
            SDL_WarpMouse(tux_rect.x + tux_rect.w / 2, tux_rect.y + tux_rect.h - egg->h);

        }

        return 0;
    }
}







// ############ Extra Added  #############################################


int glyph_offset;

/*****************************************************************/
/*   Loading of data files for images and sounds.                */
/*   These functions also draw some user feedback to             */
/*   display the progress of the loading.                        */
/*****************************************************************/

/* returns 1 if all data files successfully loaded, 0 otherwise. */

/* TODO load only "igloo" or "city" files, not both.             */
/* TODO get rid of files no longer used.                         */

int load_image_data()
{
    int i;

    static char* image_filenames[NUM_IMAGES] = {
  "status/tux_helmet1.png",
  "status/tux_helmet2.png",
  "status/tux_helmet3.png", 
  "cities/city-blue.png",
  "cities/csplode-blue-1.png",
  "cities/csplode-blue-2.png",
  "cities/csplode-blue-3.png",
  "cities/csplode-blue-4.png",
  "cities/csplode-blue-5.png",
  "cities/cdead-blue.png",
  "cities/city-green.png",
  "cities/csplode-green-1.png",
  "cities/csplode-green-2.png",
  "cities/csplode-green-3.png",
  "cities/csplode-green-4.png",
  "cities/csplode-green-5.png",
  "cities/cdead-green.png",
  "cities/city-orange.png",
  "cities/csplode-orange-1.png",
  "cities/csplode-orange-2.png",
  "cities/csplode-orange-3.png",
  "cities/csplode-orange-4.png",
  "cities/csplode-orange-5.png",
  "cities/cdead-orange.png",
  "cities/city-red.png",
  "cities/csplode-red-1.png",
  "cities/csplode-red-2.png",
  "cities/csplode-red-3.png",
  "cities/csplode-red-4.png",
  "cities/csplode-red-5.png",
  "cities/cdead-red.png",
  "comets/comet1.png",
  "comets/comet2.png",
  "comets/comet3.png",
  "comets/cometex1.png",
  "comets/cometex2.png",
  "tux/console.png",
  "tux/tux-console1.png",
  "tux/tux-console2.png",
  "tux/tux-console3.png",
  "tux/tux-console4.png",
  "tux/tux-relax1.png",
  "tux/tux-relax2.png",
  "tux/tux-drat.png",
  "tux/tux-yipe.png",
  "tux/tux-yay1.png",
  "tux/tux-yay2.png",
  "tux/tux-yes1.png",
  "tux/tux-yes2.png",
  "tux/tux-sit.png",
  "tux/tux-fist0.png",
  "tux/tux-fist1.png",
  "status/wave.png",
  "status/score.png",
  "status/numbers.png",
  "status/gameover.png",
  "arrows.png"
};

    /* Load static images: */
    for (i = 0; i < NUM_IMAGES; i++)
    {
		char fn[100];
		//sprintf(fn,"images/%s",image_filenames[i]);
		
		//fprintf(stderr,"\nCheck out : %s-%d",fn,T4K_CheckFile(fn));
		
		
        images[i] = T4K_LoadImage(image_filenames[i], IMG_ALPHA);

        if (images[i] == NULL)
        {
            fprintf(stderr,
                    "\nError: I couldn't load a graphics file:\n"
                    "%s\n"
                    "The Simple DirectMedia error that occured was:\n"
                    "%s\n\n", image_filenames[i], SDL_GetError());
            return 0;
        }
    }

    glyph_offset = 0;

#ifdef REPLACE_WAVESCORE
    /* Replace the "WAVE" and "SCORE" with translate-able versions */
    SDL_FreeSurface(images[IMG_WAVE]);
    images[IMG_WAVE] = T4K_SimpleTextWithOffset(_("WAVE"), 28, &white, &glyph_offset);
    SDL_FreeSurface(images[IMG_SCORE]);
    images[IMG_SCORE] = T4K_SimpleTextWithOffset(_("SCORE"), 28, &white, &glyph_offset);
    glyph_offset++;
#endif

    /* If we make it to here OK, return 1: */
    return 1;
}





#ifndef NOSOUND
int load_sound_data(void)
{
    int i = 0;

    static char* sound_filenames[NUM_SOUNDS] = {
        DATA_PREFIX "/sounds/harp.wav",        
        DATA_PREFIX "/sounds/pop.wav",
        DATA_PREFIX "/sounds/laser.wav",
        DATA_PREFIX "/sounds/buzz.wav",
        DATA_PREFIX "/sounds/alarm.wav",
        DATA_PREFIX "/sounds/shieldsdown.wav",        
        DATA_PREFIX "/sounds/explosion.wav",
        DATA_PREFIX "/sounds/tock.wav"
    };


    /* skip loading sound files if sound system not available: */
    if (settings.sys_sound)
    {
        for (i = 0; i < NUM_SOUNDS; i++)
        {
            sounds[i] = Mix_LoadWAV(sound_filenames[i]);

            if (sounds[i] == NULL)
            {
                fprintf(stderr,
                        "\nError: I couldn't load a sound file:\n"
                        "%s\n"
                        "The Simple DirectMedia error that occured was:\n"
                        "%s\n\n", sound_filenames[i], SDL_GetError());
                return 0;
            }
        }
    }

    //NOTE - no longer load musics here - they are loaded as needed
    return 1;
}

#endif /* NOSOUND */
