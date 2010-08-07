/***************************************************************
 *  schoolmode.c                                                *
 *                                                              *
 *  Description:  Display schoolmode specific menus and handle  *
 *                changes occur as schoolmde game progresses.   *                                         *
 *  Author:       Vikas Singh 					*
 *                 vikassingh008@gmail.com ,2010 		*
 *  Copyright:    GPL v3 or later                               *
 *  							    	*
 *  						          	*
 *                                                          	*
 *                                                          	*
 *  TuxMath                                                 	*
 *  Part of "Tux4Kids" Project                              	*
 *  http://tux4kids.alioth.debian.org/                      	*
 ***************************************************************/
#include "globals.h"
#include"funcs.h"
#include "SDL_extras.h"
#include"manage_xmlLesson.h"


//levels displayed on screen at a time
#define MENUS_ON_SCREEN 9   
SDL_Surface *tux4kids_logo=NULL;
SDL_Rect logo_rect1,tux4kids_logo_rect;
SDL_Rect bkgd_rect,rects;

static SDL_Rect cursor;

SDL_Surface* win_bkgd = NULL;
SDL_Surface* fs_bkgd= NULL;

SDL_Rect stop_rect, prev_rect, next_rect;
SDL_Surface *stop_button, *prev_arrow, *next_arrow,*img_correct,*img_lock,*img_exclamation;

static Mix_Chunk* snd_welcome = NULL; 
static Mix_Chunk* snd_move = NULL;
static Mix_Chunk* snd_select = NULL;
const char* tux4kids_standby_path = "standby.png";
//const char* school_bkg_path    = "schoolmode/school_bkg.jpg";
const char* school_bkg_path="main_bkg.png";
enum { NO_ONE, CLICK, PAGEUP, PAGEDOWN, STOP_ESC, RESIZED };


//SDL_Surface* current_bkgd()
 // { return screen->flags & SDL_FULLSCREEN ? fs_bkgd : win_bkgd; }


/* Local function prototypes: */
void ShowMsg(char*,char*,char*,char*,char*,char*);
void display_wait(char *);

int total_no_menus; //defined in parse_xmlLesson.h
int no_of_pages;        //no of menu pages to be displayed


  const int max_width = 500;
  const int title_font_size = 32;
  char xml_lesson_path[4096];
 
void SetRect(SDL_Rect* rect, const float* pos)
{
  rect->x = pos[0] * screen->w;
  rect->y = pos[1] * screen->h;
  rect->w = pos[2] * screen->w;
  rect->h = pos[3] * screen->h;
}

void sm_prerender_all()
{
const float stop_pos[4] = {0.94, 0.0, 0.06, 0.06};
const float prev_pos[4] = {0.87, 0.93, 0.06, 0.06};
const float next_pos[4] = {0.94, 0.93, 0.06, 0.06};
//const char* stop_path = "status/stop.svg";
//const char* prev_path = "status/left.svg";
//const char* next_path = "status/right.svg";





  SetRect(&stop_rect, stop_pos);
  if(stop_button)
    SDL_FreeSurface(stop_button);
  stop_button = LoadImage("stop.png", IMG_ALPHA);
  /* move button to the right */
  stop_rect.x = screen->w - stop_button->w;

  SetRect(&prev_rect, prev_pos);
  if(prev_arrow)
    SDL_FreeSurface(prev_arrow);
   prev_arrow  = LoadImage("left.png", IMG_ALPHA);
  
  /* move button to the right */
  prev_rect.x += prev_rect.w - prev_arrow->w;

  SetRect(&next_rect, next_pos);
  if(next_arrow)
    SDL_FreeSurface(next_arrow);
   next_arrow = LoadImage("right.png", IMG_ALPHA);

  img_correct=LoadImage("correct.png", IMG_ALPHA);
  img_lock=LoadImage("lock.png", IMG_ALPHA);
  img_exclamation=LoadImage("exclamation.png", IMG_ALPHA);

 if (settings.menu_sound)
  {
    snd_move = LoadSound("tock.wav");
    snd_select = LoadSound("pop.wav");
    if (!snd_move || !snd_select)
    {
      fprintf(stderr, "Could not load menu sounds - turning menu_sound off\n");
      settings.menu_sound = 0;
    }
  }
 
}





int schoolmode(char *mission_path)
{

  tux4kids_logo = LoadImage(tux4kids_standby_path, IMG_REGULAR);

  /* display the Standby screen */
  if(tux4kids_logo)
  {
    /*Center horizontally and vertically */
    tux4kids_logo_rect.x = (screen->w - tux4kids_logo->w) / 2;
    tux4kids_logo_rect.y = (screen->h - tux4kids_logo->h) / 2;

    tux4kids_logo_rect.w = tux4kids_logo->w;
    tux4kids_logo_rect.h = tux4kids_logo->h;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(tux4kids_logo, NULL, screen, &tux4kids_logo_rect);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    /* Play "harp" greeting sound lifted from Tux Paint */
  snd_welcome = LoadSound("harp.wav");

  if (snd_welcome && settings.menu_sound)
 {
   PlaySound(snd_welcome);
}


    SDL_FreeSurface(tux4kids_logo);
  }

sm_prerender_all(); //write function declaration too   -- fix

SDL_Delay(2000);

display_wait(mission_path);

manage_xmlLesson(xml_lesson_path);


 if (snd_move)
  { 
    Mix_FreeChunk(snd_move);
    snd_move = NULL;
  }
  if (snd_select)
  {
    Mix_FreeChunk(snd_select);
    snd_select = NULL;
  }
return 0;
}







void display_wait(char *mission_path)
{
 SDL_Surface* srfc = NULL;
  int finished = 0;
  Uint32 frame = 0;
  Uint32 start = 0;

        SDL_Rect text_rect, button_rect;

 FILE* fp;


snprintf(xml_lesson_path, 4096, "%s/new/lessonData.xml", mission_path);


LoadBothBkgds(school_bkg_path);

  SDL_ShowCursor(1);    


  while (!finished)
  {
    start = SDL_GetTicks();
  while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
           return ;
        }
     
        case SDL_MOUSEBUTTONDOWN:
        /* "Stop" button  */
        {
  //        cursor.x = event.motion.x;
//          cursor.y = event.motion.y;
 
          if (inRect(stop_rect, event.button.x, event.button.y ))
          {
            finished = 1;
              if ( settings.menu_sound)
              {
                PlaySound(snd_select);
              }
            return ;  // quit and cleanup done in calling function
          }
         break; 
        }

       case SDL_KEYDOWN:
        {
          if (event.key.keysym.sym == SDLK_ESCAPE)   
           { 
            finished = 1;
              if ( settings.menu_sound)
              {
                PlaySound(snd_select);
              } 
           }  
        }

       }  
      }
      SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );

 if (stop_button)
        SDL_BlitSurface(stop_button, NULL, screen, &stop_rect);

 srfc = BlackOutline(_("Welcome to TUX4KIDS School Mode!"), DEFAULT_MENU_FONT_SIZE+5, &white);
        if (srfc)
        {
          button_rect.x = text_rect.x = (screen->w)/28;
          button_rect.y = text_rect.y = (screen->h)/24;;
          button_rect.w = text_rect.w = srfc->w;
          button_rect.h = text_rect.h = srfc->h+5;
          /* add margin to button and draw: */
          button_rect.x -= 10;
          button_rect.w += 20;
          DrawButton(&button_rect, 15, 64, 232, 30, 192);
          /* Now blit text and free surface: */
          SDL_BlitSurface(srfc, NULL, screen, &text_rect);
          SDL_FreeSurface(srfc);
          srfc = NULL;
        }

 srfc = BlackOutline(_("Searching for lesson file ..."), DEFAULT_MENU_FONT_SIZE+10, &white);
        if (srfc)
        {
         text_rect.x = (screen->w)/2 - srfc->w/2;
           text_rect.y = (screen->h)/2 ;
        text_rect.w = srfc->w;
         text_rect.h = srfc->h;     

          /* Now blit text and free surface: */
          SDL_BlitSurface(srfc, NULL, screen, &text_rect);
          SDL_FreeSurface(srfc);
          srfc = NULL;
        }


SDL_UpdateRect(screen, 0, 0, 0, 0);


    /* Wait so we keep frame rate constant: */
    while ((SDL_GetTicks() - start) < 33)
    {
      SDL_Delay(20);
    }
    frame++;

     SDL_Delay(2000);  //wait for 2 seconds  
    fp = fopen(xml_lesson_path, "r");
    if (fp)
     {
      fclose(fp);
      fp = NULL;
       finished=1;

     }
    else
     SDL_Delay(10000);  //wait for 10 seconds  
  

  }  // End of while (!finished) loop

}

int display_screen(int selected)
{

static char *next_game_string;
int temp,current_no;
int i = 0;

  int finished = 0;
  Uint32 frame = 0;
  Uint32 start = 0;
 SDL_Surface* srfc = NULL;
        SDL_Rect text_rect, button_rect,dest,table_bg;

  int page_no = 0;
  int old_page_no = -1; //So menus get drawn first time through
  /* Surfaces, char buffers, and rects for table: */
  
 

//decide the no of menu pages to be displayed
no_of_pages=ceil((float)total_no_menus/MENUS_ON_SCREEN);

if(total_no_menus % MENUS_ON_SCREEN)
  no_of_pages=(total_no_menus/MENUS_ON_SCREEN)+1;
else
 no_of_pages=total_no_menus/MENUS_ON_SCREEN;
  



  LoadBothBkgds(school_bkg_path);

  SDL_ShowCursor(1);    

//if(current_bkgd())
  //{
    /* FIXME not sure trans_wipe() works in Windows: */
  //  trans_wipe(current_bkgd(), RANDOM_WIPE, 10, 20);
    /* Make sure background gets drawn (since trans_wipe() doesn't */
    /* seem to work reliably as of yet):                          */
//    SDL_BlitSurface(current_bkgd(), NULL, screen, &bkgd_rect);
 // }

      /* Draw background shading for table: */
   //   table_bg.x = (screen->w)/2 - (max_width + 20)/2 + 50; //don't draw over Tux
    //  table_bg.y = 5;
   //   table_bg.w = max_width + 20;
    //  table_bg.h = screen->h - 100; //- images[IMG_RIGHT]->h;
   //   DrawButton(&table_bg, 25, SEL_RGBA);


    bkgd_rect.x = (screen->w - bkgd_rect.w) / 2;
    bkgd_rect.y = (screen->h - bkgd_rect.h) / 2;




  while (!finished)
  {
    start = SDL_GetTicks();

    /* Check for user events: */
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
           return -1;
        }

        case SDL_MOUSEBUTTONDOWN:
        /* "Stop" button  */
        {
  //        cursor.x = event.motion.x;
//          cursor.y = event.motion.y;
 
          if (inRect(stop_rect, event.button.x, event.button.y ))
          {
            finished = 1;
              if ( settings.menu_sound)
              {
                PlaySound(snd_select);
              }
            return -1;  // quit and cleanup done in calling function
          }

          /* "Left" button - go to previous page: */
          if (inRect(prev_rect, event.button.x, event.button.y))
          {
            if (page_no > 0)
            {
              page_no--;
              if ( settings.menu_sound)
              {
                PlaySound(snd_select);
              }
            }
          }

          /* "Right" button - go to next page: */
          if (inRect(next_rect, event.button.x, event.button.y ))
          {
            if (page_no < no_of_pages-1)
            {
              page_no++;
              if ( settings.menu_sound)
              {
                PlaySound(snd_select);
              }
            }
          }
          break;
        }


        case SDL_KEYDOWN:
        {
          if (event.key.keysym.sym == SDLK_RETURN)   
           { 
            finished = 1;
              if ( settings.menu_sound)
              {
                PlaySound(snd_select);
              } 
           } 
          else if (event.key.keysym.sym == SDLK_ESCAPE)   
           { 
              return -1;
           }      
        }
      }
    }


    /* If needed, redraw: */
    if (page_no != old_page_no)
    {

      SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );

      /* Draw controls: */
      if (stop_button)
        SDL_BlitSurface(stop_button, NULL, screen, &stop_rect);
      
      /* Draw regular or grayed-out left arrow: */
      if (page_no != 0)
      {
        if (prev_arrow)
          SDL_BlitSurface(prev_arrow, NULL, screen, &prev_rect);
      }
      /* Draw regular or grayed-out right arrow: */
      if (page_no != no_of_pages-1)
      {
        if (next_arrow)
          SDL_BlitSurface(next_arrow, NULL, screen, &next_rect);
      }

 /* Draw background shading for table: */
      table_bg.x = (screen->w) - (max_width + 60) ; 
      table_bg.y = 5;
      table_bg.w = max_width +20;
      table_bg.h = screen->h - 80; //- images[IMG_RIGHT]->h;
      DrawButton(&table_bg, 25, SEL_RGBA);

 temp=page_no*MENUS_ON_SCREEN; //menu display begins from temp at this page

    srfc = BlackOutline(_("ALL TASKS LIST"), 50, &white);
        if (srfc)
        {
          button_rect.x = text_rect.x = table_bg.x + table_bg.w/7 ;
          button_rect.y = text_rect.y = 10;
          button_rect.w = text_rect.w = srfc->w;
          button_rect.h = text_rect.h = srfc->h+5;
          /* add margin to button and draw: */
          button_rect.x -= 10;
          button_rect.w += 20;
          DrawButton(&button_rect, 15, 0, 0, 32, 192);
          /* Now blit text and free surface: */
          SDL_BlitSurface(srfc, NULL, screen, &text_rect);
          SDL_FreeSurface(srfc);
          srfc = NULL;
        }

current_no=total_no_menus-temp;

current_no=(MENUS_ON_SCREEN<current_no)?MENUS_ON_SCREEN:current_no;


for(i=0;i<current_no;i++)
{
   if(temp+i==selected)   
    {
     srfc = BlackOutline(_(menu_names[temp+i]),50 , &yellow);   //title_font_size
     next_game_string=menu_names[temp+i];
    }	
   else if(temp+i <selected)   
    {
     srfc = BlackOutline(_(menu_names[temp+i]),40 , &white);  
     
    }	
    
   else if(temp+i >selected)   
    {
     srfc = BlackOutline(_(menu_names[temp+i]),40 , &gray);   //title_font_size

    }	 
    
        if (srfc)
        {
          text_rect.x = table_bg.x+table_bg.w/2 - (srfc->w)/2;
          text_rect.y += text_rect.h + 10; /* go to bottom of next line */
          text_rect.w = srfc->w;
          text_rect.h = srfc->h;
          
           if(temp+i==selected)
             { 
               button_rect.x = text_rect.x ;
               button_rect.y = text_rect.y ;
               button_rect.w = text_rect.w ;
               button_rect.h = text_rect.h ;
               /* add margin to button and draw: */
               button_rect.x -= 40;
               button_rect.w += 80;
               DrawButton(&button_rect, 15, 7, 97, 96, 240);

               button_rect.x = (screen->w) - (max_width + 60);
               button_rect.y = text_rect.y + 2;
               button_rect.w = 60 ;
               button_rect.h = 60 ;
               /* add margin to button and draw: */
               button_rect.x += 2;
               button_rect.w += 4;
               DrawButton(&button_rect, 15, 0, 255, 0, 240);

               //a new image can be put using the following code
               /*
               dest.x =  button_rect.x ;
               dest.y = text_rect.y ;
               dest.w = img_correct->w;
               dest.h = img_correct->h;
 
               SDL_BlitSurface(next_arrow, NULL, screen, &dest);    
               */  
             }  
           else if(temp+i < selected)
             { 
               button_rect.x = (screen->w) - (max_width + 60);
               button_rect.y = text_rect.y ;
               button_rect.w = 60 ;
               button_rect.h = 60 ;
               /* add margin to button and draw: */
               button_rect.x += 2;
               button_rect.w += 4;
               DrawButton(&button_rect, 15, 0, 0, 32, 100);
               

                if (game_completed[temp+i])
                {
                  dest.x =  button_rect.x + 8 ;
                  dest.y = button_rect.y +10 ; 
                  dest.w = img_correct->w;
                  dest.h = img_correct->h;
                  //dest.w = button_rect.w;
                  //dest.h = button_rect.h;
                  SDL_BlitSurface(img_correct, NULL, screen, &dest);    
                 }      
                else
                 
               {
                button_rect.x = (screen->w) - (max_width + 60);
                button_rect.y = text_rect.y + 2;
                button_rect.w = 60 ;
                button_rect.h = 60 ;
                /* add margin to button and draw: */
                button_rect.x += 2;
                button_rect.w += 4;
                DrawButton(&button_rect, 15, 255, 0, 0, 240);


                //image can also be displayed by uncommenting by following code is place of color
                //   dest.x =  button_rect.x + 8 ;
                //   dest.y = button_rect.y +10 ;
                //  dest.w = img_exclamation->w;
                //  dest.h = img_exclamation->h;
                //   SDL_BlitSurface(img_exclamation, NULL, screen, &dest);    
                } 

             }  
           else if(temp+i > selected)
             { 
               button_rect.x = (screen->w) - (max_width + 60);
               button_rect.y = text_rect.y ;
               button_rect.w = 60 ;
               button_rect.h = 60 ;
               /* add margin to button and draw: */
               button_rect.x += 2;
               button_rect.w += 4;
               DrawButton(&button_rect, 15, 0, 0, 32, 100);

               //Load lock image  
               dest.x =  button_rect.x + 8 ;
               dest.y = button_rect.y +10 ;
               dest.w = img_lock->w;
               dest.h = img_lock->h;
               //dest.w = button_rect.w;
               //dest.h = button_rect.h;
  
               SDL_BlitSurface(img_lock, NULL, screen, &dest);    
             }       

          SDL_BlitSurface(srfc, NULL, screen, &text_rect);
          SDL_FreeSurface(srfc);
          srfc = NULL;
         
        }
      }


}

      srfc = BlackOutline(_(next_game_string),20 , &red); 

  //this can be modified as desired
      ShowMsg("NOTE : Your next task is to play",next_game_string
               , "Please try to complete the game.", "You can skip a game by pressing ESC key."
             ,"Lesson by - Mr X" , "Press ENTER to play the game.");

 srfc = BlackOutline(_("TUX4KIDS School Mode!"), DEFAULT_MENU_FONT_SIZE+5, &white);
        if (srfc)
        {
          button_rect.x = text_rect.x = (screen->w)/28;
          button_rect.y = text_rect.y = (screen->h)/24;;
          button_rect.w = text_rect.w = srfc->w;
          button_rect.h = text_rect.h = srfc->h+5;
          /* add margin to button and draw: */
          button_rect.x -= 10;
          button_rect.w += 20;
          DrawButton(&button_rect, 15, 64, 232, 30, 192);
          /* Now blit text and free surface: */
          SDL_BlitSurface(srfc, NULL, screen, &text_rect);
          SDL_FreeSurface(srfc);
          srfc = NULL;
        }



      SDL_UpdateRect(screen, 0, 0, 0, 0);

      old_page_no = page_no;
    

  //  HandleTitleScreenAnimations();

    /* Wait so we keep frame rate constant: */
    while ((SDL_GetTicks() - start) < 33)
    {
      SDL_Delay(20);
    }
    frame++;

  }  // End of while (!finished) loop
return 0;
}




void ShowMsg(char* str1, char* str2, char* str3, char* str4,char* str5,char *str6)
{
  SDL_Surface *s1, *s2, *s3, *s4,*s5,*s6;
  SDL_Rect loc;

  s1 = s2 = s3 = s4 =s5=s6=NULL;
 if (str1)
    s1 = BlackOutline(str1, 20, &red);
  if (str2)
    s2 = BlackOutline(str2, 30, &red);
  if (str3)
    s3 = BlackOutline(str3, 20, &red);
  /* When we get going with i18n may need to modify following - see below: */
  if (str4)
    s4 = BlackOutline(str4, 20, &red);

  if (str5)
    s5 = BlackOutline(str5, 20, &red);
   if (str6)
    s6 = BlackOutline(str6, 35, &yellow);

 //if (str7)
   // s7 = BlackOutline(str7, 35, &white);



  /* Draw lines of text (do after drawing Tux so text is in front): */
  if (s1)
  {
    loc.x = (screen->w / 8) - 80; 
    loc.y = (screen->h / 9) + 100;
    SDL_BlitSurface( s1, NULL, screen, &loc);
  }
  if (s2)
  {
    loc.x = (screen->w/8) - 80; 
    loc.y +=   30 + s1->h ;
    SDL_BlitSurface( s2, NULL, screen, &loc);
  }
  if (s3)
  {
    loc.x = (screen->w / 8)  - 80; 
    loc.y +=  30 + s2->h ;
    SDL_BlitSurface( s3, NULL, screen, &loc);
  }
  if (s4)
  {
    loc.x = (screen->w / 8) - 80 ; 
    loc.y += 30 + s3->h;
    SDL_BlitSurface( s4, NULL, screen, &loc);
  }

if (s5)
  {
    loc.x = (screen->w / 8) - 80 ; 
    loc.y += 40 + s4->h;
    SDL_BlitSurface( s5, NULL, screen, &loc);
  }

if (s6)
  {
    loc.x = (screen->w / 2) - (s6->w)/2 ; 
    loc.y = screen->h - 50;
    SDL_BlitSurface( s6, NULL, screen, &loc);
  }

/*if (s7)
  {
    loc.x = (screen->w )/24 ; 
    loc.y = (screen->h )/24;
    SDL_BlitSurface( s7, NULL, screen, &loc);
  }
*/
  /* and update: */
  SDL_UpdateRect(screen, 0, 0, 0, 0);

  SDL_FreeSurface(s1);
  SDL_FreeSurface(s2);
  SDL_FreeSurface(s3);
  SDL_FreeSurface(s4);
  SDL_FreeSurface(s5);
  SDL_FreeSurface(s6);
 // SDL_FreeSurface(s7);
}




