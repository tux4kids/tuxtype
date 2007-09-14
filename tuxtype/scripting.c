/***************************************************************************
 -  file: scripting.c
 -  description: scripting for lessons & instructions ...
                             -------------------
    begin                : Sun Dec 28, 2003
    copyright            : Jesse Andrews (C) 2003
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

#include "scripting.h"

/* Local function prototypes: */
static void clear_items(itemType* i);
static void clear_pages(pageType* p);
static void close_script(void);
static SDL_Color* get_color(const char* in);
static int get_int(const char* in);
static char* get_quote(const char* in);
static char hex2int(char b, char s);
static int load_script(const char* fn);
static void run_script(void);

/************************************************************************/
/*                                                                      */ 
/*         "Public" functions (callable throughout program)             */
/*                                                                      */
/************************************************************************/


void InstructCascade(void)
{
  char fn[FNLEN];

  if (settings.use_english)
    sprintf( fn, "%s/scripts/cascade.xml", settings.default_data_path);
  else
    sprintf( fn, "%s/scripts/cascade.xml", settings.theme_data_path);

  if (load_script( fn ) != 0)
    return; // bail if any errors occur
  run_script();
}


void InstructLaser(void)
{
  int i;
  char fn[FNLEN];

  if (settings.use_english)
    sprintf( fn, "%s/scripts/laser.xml", settings.default_data_path);
  else
    sprintf( fn, "%s/scripts/laser.xml", settings.theme_data_path);

  if (load_script( fn ) != 0)
    return; // bail if any errors occur

  for (i = 0; i < 20; i++)
  {
    run_script();
    SDL_Delay(500);
  }
}


void ProjectInfo(void)
{
  char fn[FNLEN]; 

  if (settings.use_english)
    sprintf( fn, "%s/scripts/projectInfo.xml", settings.default_data_path);
  else
    sprintf( fn, "%s/scripts/projectInfo.xml", settings.theme_data_path);

  if (load_script( fn ) != 0)
    return; // bail if any errors occur

  run_script();
}


int TestLesson(void)
{
  SDL_Surface* left = NULL, *right = NULL, *pointer = NULL, *bkg = NULL;
  SDL_Surface* filenames[200] = {NULL};

  SDL_Rect spot, arrow_area;
  SDL_Rect leftRect, rightRect;
  SDL_Rect titleRects[8];

  TTF_Font* font = NULL;
	
  int stop = 0;
  int loc = 0;
  int old_loc = 1;
  int i;
  int c = 0;
  int found = 0;
	
  char fn[FNLEN]; 
  unsigned char wordlistFile[200][200];
  unsigned char script_path[FNLEN];

  DIR* wordsDir = NULL;
  struct dirent* wordsFile = NULL;

  LOG("\nEnter TestLesson()\n");

  pointer = LoadImage( "right.png", IMG_ALPHA );
  bkg = LoadImage( "main_bkg.png", IMG_REGULAR );
  left = LoadImage("left.png", IMG_ALPHA);       
  right = LoadImage("right.png", IMG_ALPHA);

  if (!pointer || !bkg || !left ||!right)
  {
    fprintf(stderr, "TestLesson() - needed image not found\n");
    /* Free anything that got loaded: */
    SDL_FreeSurface(pointer);
    SDL_FreeSurface(left);
    SDL_FreeSurface(right);
    SDL_FreeSurface(bkg);
    pointer = left = right = bkg = NULL;
    return 0;
  }

  SDL_ShowCursor(0);

  /* find the directory to load wordlists from */
  /* First look in theme path, if desired: */
  if (!settings.use_english)
  {
    sprintf( script_path, "%s/scripts", settings.theme_data_path);
    if (CheckFile(script_path))
    {
      DEBUGCODE { fprintf(stderr, "Using theme script dir: %s\n", script_path); }
      found = 1;
    }
  }

  /* Now look in default path if desired or needed: */
  if (!found)
  {
    sprintf( script_path, "%s/scripts", settings.default_data_path);
    if (CheckFile(script_path))
    {
      DEBUGCODE { fprintf(stderr, "Using theme script dir: %s\n", script_path); }
      found = 1;
    }
  }

  if (!found)
  {
    fprintf(stderr, "TestLesson(): Error finding script directory!\n");
    return 0;
  }

  /* What is this location? */
  spot.x = 60;
  spot.y = 20;


  /* create a list of all the .xml files */

  wordsDir = opendir( script_path );	
  font = LoadFont(settings.theme_font_name, MENU_FONT_SIZE);
  do
  {
    wordsFile = readdir(wordsDir);
    if (!wordsFile)
      break;

    /* must have at least '.xml' at the end */
    if (strlen(wordsFile->d_name) < 5)
      continue;

    if (strcmp(&wordsFile->d_name[strlen(wordsFile->d_name)-4],".xml"))
      continue;

    sprintf( wordlistFile[c], "%s", wordsFile->d_name );

    DEBUGCODE { fprintf(stderr, "Adding XML file no. %d: %s\n", c, wordlistFile[c]); }


    filenames[c] = TTF_RenderUTF8_Blended(font, wordsFile->d_name, white);
    SDL_BlitSurface( filenames[c], NULL, screen, &spot );
    SDL_FreeSurface(filenames[c]);
    c++;
    spot.y += MENU_FONT_SIZE;
  } while (1); /* Leave loop when readdir() returns NULL */

  TTF_CloseFont(font);
  font = NULL;
  closedir(wordsDir);	
  wordsDir = NULL;
  SDL_Flip(screen);

  /* Should be safe - tested 'left' and 'right' above: */
  leftRect.w = left->w; leftRect.h = left->h;
  leftRect.x = 320 - 80 - (leftRect.w/2); leftRect.y = 430;

  rightRect.w = right->w; rightRect.h = right->h;
  rightRect.x = 320 + 80 - (rightRect.w/2); rightRect.y = 430;

  /* set initial rect sizes */
  titleRects[0].y = 30;
  titleRects[0].w = titleRects[0].h = titleRects[0].x = 0;

  for (i = 1; i < 8; i++)
  { 
    titleRects[i].y = titleRects[i-1].y + 50;
    titleRects[i].w = titleRects[i].h = titleRects[i].x = 0;
  }

  arrow_area.x = 0;
  arrow_area.y = 0;
  arrow_area.w = 59;
  arrow_area.h = 479;

  while (!stop)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          exit(0);
          break;
        /* FIXME some of mouse code is wrong */
        case SDL_MOUSEMOTION:
          for (i=0; (i<8) && (loc-(loc%8)+i<c); i++)
            if (inRect( titleRects[i], event.motion.x, event.motion.y ))
            {
              loc = loc-(loc%8)+i;
              break;
            }
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (inRect( leftRect, event.button.x, event.button.y ))
            if (loc-(loc%8)-8 >= 0)
            {
              loc=loc-(loc%8)-8;
              break;
            }

          if (inRect( rightRect, event.button.x, event.button.y ))
            if (loc-(loc%8)+8 < c)
            {
              loc=loc-(loc%8)+8;
              break;
            }

          for (i=0; (i<8) && (loc-(loc%8)+i<c); i++)
            if (inRect(titleRects[i], event.button.x, event.button.y))
            {
//              loc = loc-(loc%8)+i;
//              ClearWordList(); /* clear old selection */
//               if (loc==0)
//                 UseAlphabet(); 
//               else
//                 GenerateWordList(wordlistFile[loc]);
// 
//               stop = 1;
//               break;
            }
          break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
          { 
            stop = 2;
            break;
          }

          if (event.key.keysym.sym == SDLK_RETURN)
          {
            sprintf(fn, "%s/scripts/%s", settings.default_data_path, wordlistFile[loc]);
            stop = 1;
            break;
          }

          if ((event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_PAGEUP))
          {
            if (loc-(loc%8)-8 >= 0)
              loc=loc-(loc%8)-8;
            SDL_ShowCursor(1);
          }

          if ((event.key.keysym.sym == SDLK_RIGHT) || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc-(loc%8)+8 < c)
              loc=(loc-(loc%8)+8);
          }

          if (event.key.keysym.sym == SDLK_UP)
          {
            if (loc > 0)
              loc--;
          }

          if (event.key.keysym.sym == SDLK_DOWN)
          {
            if (loc+1< c)
              loc++;
          }
      } /* End of 'switch(event.type)' loop */
    }  /* End of 'while(SDL_PollEvent(&event))' loop */



    if (old_loc != loc)
    {
      int start;
      SDL_BlitSurface( bkg, &arrow_area, screen, NULL);

      start = loc;
      for (i = start; i < c; i++)
      {
        spot.x = 5;
        spot.y = (i * MENU_FONT_SIZE) + 18;
        if (i == loc)
          SDL_BlitSurface(pointer, NULL, screen, &spot);
      }

      SDL_Flip(screen);
    }

      SDL_Delay(40);
      old_loc = loc;
  }   /*   End of 'while(!stop)' loop  */

  SDL_FreeSurface(pointer);
  SDL_FreeSurface(left);
  SDL_FreeSurface(right);
  SDL_FreeSurface(bkg);
  pointer = left = right = bkg = NULL;

  if (stop == 2)
  {
    LOG("Player pressed 'Esc' - leaving TestLesson\n");
    return 1;
  }

  /* Getting to here means "stop == 1", try to run chosen script: */
  if (load_script(fn) != 0)
  {
    fprintf(stderr, "load_script() failed to load '%s'\n");
    return 0; // bail if any errors occur
  }

  DEBUGCODE { fprintf(stderr, "Attempting to run script: %s\n", fn); }

  run_script();

  /* FIXME - shouldn't we show the cursor if returning in other code paths? */
  SDL_ShowCursor(1);

  LOG("Leave TestLesson()\n");

  return 1; 
}



/************************************************************************/
/*                                                                      */ 
/*         "Private" functions (local to scripting.c)                   */
/*                                                                      */
/************************************************************************/


static char* get_quote(const char* in)
{
    int start, finish;
    char *out;

    for (start=0; start<strlen(in) && in[start] != '"'; start++);  // find the first "

    if (start >= strlen(in)) return 0; // return null string if no " found

    start++; // move past the "
    
    for (finish=start; finish<strlen(in) && in[finish] != '"'; finish++); // find the next "

    if (finish >= strlen(in)) return 0; // return null string if no " found
    
    out = malloc(finish-start+2);
    
    snprintf(out, finish-start+1, &in[start]);
    out[finish-start]=0;
    
    return out;
}


static int get_int(const char* in)
{
    char *t = get_quote(in);
    int ans=-1;
    if (t) {
        ans = atoi(t);
        free(t);
    }
    return ans;
}


static char hex2int(char b, char s)
{
    char ans=0;
        
    if      ((b>='0') && (b<='9'))       ans=16*(b-'0');
    else if ((b>='A') && (b<='F'))       ans=16*(b-'A'+10);
    else if ((b>='a') && (b<='f'))       ans=16*(b-'a'+10);
    
    if      ((s>='0') && (s<='9'))       ans+=(s-'0');
    else if ((s>='A') && (s<='F'))       ans+=(s-'A'+10);
    else if ((s>='a') && (s<='f'))       ans+=(s-'a'+10);

    return ans;
}


static SDL_Color* get_color(const char* in)
{
    char* col;
    SDL_Color* out=malloc(sizeof(SDL_Color));
    col = get_quote(in);
    
    if ((strlen(col)==7) && (col[0] == '#')) {
        out->r = hex2int( col[1], col[2] );
        out->g = hex2int( col[3], col[4] );
        out->b = hex2int( col[5], col[6] );
    }
    
    free(col);
    
    return out;
}

scriptType* curScript = NULL;
pageType* curPage = NULL;
itemType* curItem = NULL;

static int load_script(const char* fn)
{
  int i;
  char str[FNLEN];
  FILE* f = NULL;
    
  LOG( "\nEnter load_script()\n" );
    
  if (curScript)
  {
    LOG( "script already in memory, removing now!\n");
    close_script();
  }
    

  f = fopen(fn, "r");

  if (f == NULL)
  {
    fprintf(stderr, "error loading script %s\n", fn);
    return -1;
  }

  do
  {
    fscanf(f, "%[^\n]\n", str);
    if (strncmp("<script", str, 7) == 0)
    {
      /* -- allocate space for the lesson info -- */
      curScript = (scriptType*)calloc(1, sizeof(scriptType));
      for (i = 7; i < strlen(str) && str[i] != '>'; i++) 
      {
        if ((str[i] == 't') && strncmp("title", &str[i], 5) == 0)
          curScript->title = get_quote(&str[i + 5]);

        if ((str[i]=='b') && strncmp("bgcolor", &str[i], 7) == 0)
          curScript->bgcolor = get_color(&str[i + 7]);

        if ((str[i]=='b') && strncmp("background", &str[i], 10) == 0)
          curScript->background = get_quote(&str[i + 10]);

        if ((str[i]=='f') && strncmp("fgcolor", &str[i], 7) == 0) 
          curScript->fgcolor = get_color(&str[i + 7]); 
      }
    }
    else if (strncmp("<page", str,  5)==0)
    {
      if (curScript==NULL)
      {
        fprintf(stderr, "CRITICAL XML ERROR: <page> should be in a <script> in file %s line (todo)", fn); exit(1);
      }

      if (curScript->pages==NULL)
      {
        curPage = (pageType *)calloc(1,sizeof(pageType));
        curPage->prev = curPage;
        curScript->pages = curPage; 
      }
      else
      {
        curPage->next = (pageType*)calloc(1, sizeof(pageType));
        curPage->next->prev = curPage;
        curPage = curPage->next;
      }

      for (i = 5; i < strlen(str) && str[i]!='>'; i++)
      {
        if ((str[i] == 'b') && strncmp("background", &str[i], 10) == 0) 
          curPage->background = get_quote(&str[i + 10]);

        if ((str[i]== 't') && strncmp("title", &str[i], 5) == 0) 
          curPage->title = get_quote(&str[i + 5]);

        if ((str[i] == 'b') && strncmp("bgcolor", &str[i], 7) == 0) 
          curPage->bgcolor = get_color(&str[i + 7]);

        if ((str[i] == 'f') && strncmp("fgcolor", &str[i], 7) == 0) 
          curPage->fgcolor = get_color(&str[i + 7]);
      }
    }
    else if (strncmp("<text", str,  5) == 0)
    {
      if (curPage == NULL)
      {
        fprintf(stderr, "CRITICAL XML ERROR: <text> should be in a <page> in file %s line (todo)", fn); exit(1);
      }

      if (curPage->items == NULL) 
      {
        curItem = (itemType*)calloc(1, sizeof(itemType));
        curPage->items = curItem;
      }
      else
      {
        curItem->next = (itemType*)calloc(1, sizeof(itemType));
        curItem = curItem->next;
      }

      curItem->type = itemTEXT;
      curItem->x = curItem->y = -1;

      for (i = 5; i<strlen(str) && str[i]!='>'; i++)
      {
        if ((str[i] == 's') && strncmp("size", &str[i], 4) == 0)
          curItem->size = (char)get_int(&str[i + 4]);

        if ((str[i] == 'a') && strncmp("align", &str[i], 5) == 0)
        {
          char* t = get_quote(&str[i+5]);

          if (strlen(t)>=1)
          {
            if ((t[0] == 'l') || (t[0]=='L'))
              curItem->align='l';	// left
            if ((t[0] == 'c') || (t[0]=='C'))
              curItem->align='c';	// center
            if ((t[0] == 'r') || (t[0]=='R'))
              curItem->align='r';	// right
            if ((t[0] == 'm') || (t[0]=='M'))
              curItem->align='c';	// let 'm'iddle work as "center"
          }
          free(t);
        }

        if ((str[i] == 'c') && strncmp("color", &str[i], 5) == 0)
           curItem->color = get_color(&str[i + 5]);

        if ((str[i]== 'x') && strncmp(" x=", &str[i - 1], 3) == 0)
           curItem->x = get_int(&str[i + 2]);

        if ((str[i] == 'y') && strncmp(" y=", &str[i - 1], 3) == 0)
           curItem->y = get_int(&str[i + 2]);
      }

      /* --- grab the text between <text> and </text> --- */
      {
        int start, finish;

        for (start = 5; start < strlen(str) - 5 && str[start] != '>'; start++);

        start++; // advance passed the '>'

        for (finish = strlen(str) - 6; finish > 5; finish--)
           if (strncmp( "</text>", &str[finish], 7) == 0)
             break;

        finish--; // advance passed the '<'

        if (start <= finish)
        {
          curItem->data = (char*)calloc(1, finish - start + 2);
          strncpy(curItem->data, &str[start], finish - start + 1);
        }
        else
        {
          if (start == finish + 1)
          {
            curItem->data = (char*)calloc(1, 2);
            curItem->data[0]=' ';
          }
        }
      }

    } 
    else if (strncmp("<img", str, 4) == 0)
    {
      if (curPage == NULL)
      {
        fprintf(stderr,
                "CRITICAL XML ERROR: <img> should be in a <page> in file %s line (todo)",
               fn);
        exit(1); //FIXME should call cleanup() or maybe just return without exiting
      }

      if (curPage->items == NULL)
      {
        curItem = (itemType*)calloc(1, sizeof(itemType));
        curPage->items = curItem;
      }
      else 
      {
        curItem->next = (itemType*)calloc(1, sizeof(itemType));
        curItem = curItem->next;
      }

      curItem->type = itemIMG;
      curItem->x = curItem->y = -1;

      for (i = 5; i < strlen(str); i++) 
      {
        if ((str[i] == 'o') && strncmp("onclickplay", &str[i], 11) == 0)
          curItem->onclick = get_quote(&str[i + 3]);

        if ((str[i] == 'x') && strncmp(" x=", &str[i - 1], 3) == 0)
          curItem->x = get_int(&str[i + 2]);

        if ((str[i] == 'y') && strncmp(" y=", &str[i - 1], 3) == 0)
          curItem->y = get_int(&str[i + 2]);

        if ((str[i] == 's') && strncmp("src", &str[i], 3) == 0)
          curItem->data = get_quote(&str[i + 3]);

        if ((str[i] == 'a') && strncmp("align", &str[i], 5) == 0)
        {
          char* t = get_quote(&str[i + 5]);

          if (strlen(t) >= 1)
          {
            if ((t[0] == 'l') || (t[0] == 'L'))
               curItem->align='l';	// left

            if ((t[0] == 'c') || (t[0]=='C'))
               curItem->align='c';	// center

            if ((t[0] == 'r') || (t[0]=='R'))
               curItem->align='r';	// right

            if ((t[0] == 'm') || (t[0]=='M'))
               curItem->align='c';	// let 'm'iddle work as "center"
          }
          free(t);
        }
      }
    }
    else if (strncmp("<wav", str, 4) == 0)
    {
      if (curPage == NULL)
      { 
        fprintf(stderr,
                "CRITICAL XML ERROR: <wav> should be in a <page> in file %s line (todo)",
                fn);
        exit(1);  /* FIXME call Cleanup() to exit */
      }

      if (curPage->items == NULL)
      {
        curItem = (itemType*)calloc(1, sizeof(itemType));
        curPage->items = curItem;
      } 
      else 
      {
        curItem->next = (itemType*)calloc(1, sizeof(itemType));
        curItem = curItem->next;
      }

      curItem->type = itemWAV;
      curItem->loop = 0;

      for (i =5 ; i < strlen(str); i++)
      {
        if ((str[i] == 's') && strncmp("src", &str[i], 3) ==0 )
          curItem->data = get_quote(&str[i + 3]);

        if ((str[i] == 'l') && strncmp("loop", &str[i], 4) == 0)
        {
          char* t = get_quote(&str[i + 4]);

          if (strlen(t) >= 1)
          {
            if ((t[0] == 't') || (t[0]=='T'))
              curItem->loop = 1;
          } 
          free(t);
        }
      }
    }
    else if (strncmp("<prac", str, 5) == 0)
    {
      if (curPage == NULL)
      {
        fprintf(stderr,
                "CRITICAL XML ERROR: <prac> should be in a <page> in file %s line (todo)",
                fn);
        exit(1); /* FIXME call cleanup() rather than exit() */
      }

      if (curPage->items == NULL)
      {
        curItem = (itemType*)calloc(1, sizeof(itemType));
        curPage->items = curItem;
      }
      else
      {
        curItem->next = (itemType*)calloc(1, sizeof(itemType));
        curItem = curItem->next;
      }

      curItem->type = itemPRAC;

      for (i = 5; i < strlen(str) && str[i] != '>'; i++)
      {
        if ((str[i] == 's') && strncmp("size", &str[i], 4) == 0) 
          curItem->size = (char)get_int( &str[i + 4]);

        if ((str[i] == 'g') && strncmp("goal", &str[i], 4) == 0) 
          curItem->goal = (char)get_int(&str[i + 4]);

        if ((str[i] == 'a') && strncmp("align", &str[i], 5) == 0)
        {
          char* t = get_quote(&str[i + 5]);

          if (strlen(t) >= 1)
          {
            if ((t[0] == 'l') || (t[0]=='L'))
              curItem->align = 'l';	// left
            if ((t[0] == 'c') || (t[0]=='C'))
              curItem->align = 'c';	// center
            if ((t[0] == 'r') || (t[0]=='R'))
              curItem->align = 'r';	// right
            if ((t[0] == 'm') || (t[0]=='M'))
              curItem->align = 'c';	// let 'm'iddle work as "center"
          }
                   free(t);
        }

        if ((str[i] == 'c') && strncmp("color", &str[i], 5) == 0)
          curItem->color = get_color(&str[i + 5]);
      }

      { /* --- grab the text between <prac> and </prac> --- */
        int start, finish;

        for (start = 5; start < strlen(str) - 5 && str[start] != '>'; start++);

        start++; // advance passed the '>/* --- grab the text between <prac> and </prac> --- */'

        for (finish = strlen(str) - 6; finish > 5; finish--)
          if (strncmp("</prac>", &str[finish], 7) == 0)
            break;

        finish--; // advance passed the '<'

        if (start <= finish)
        {
          curItem->data = (char*)calloc(1, finish - start + 2);
          strncpy(curItem->data, &str[start], finish - start + 1);
        }
        else
        {
          if (start == finish + 1)
          {
            curItem->data = (char*)calloc(1, 2);
            curItem->data[0]=' ';
          }
        }
      }
    }
    else if (strncmp("<waitforinput", str, 13) == 0)
    { 
      if (curPage == NULL)
      {
        fprintf(stderr,
               "CRITICAL XML ERROR: <waitforinput> should be in a <page> in file %s line (todo)",
                fn);
        exit(1);
      }

      if (curPage->items == NULL)
      {
        curItem = (itemType*)calloc(1, sizeof(itemType));
        curPage->items = curItem;
      } 
      else
      {
        curItem->next = (itemType*)calloc(1, sizeof(itemType));
        curItem = curItem->next;
      }

      curItem->type = itemWFIN;

    }
    else if (strncmp("<waitforchar",  str, 12) == 0)
    { 
      if (curPage == NULL)
      {
        fprintf(stderr,
               "CRITICAL XML ERROR: <waitforchar> should be in a <page> in file %s line (todo)",
               fn);
        exit(1);
      }

      if (curPage->items == NULL)
      {
        curItem = (itemType*)calloc(1, sizeof(itemType));
        curPage->items = curItem;
      }
      else
      {
        curItem->next = (itemType*)calloc(1, sizeof(itemType));
        curItem = curItem->next;
      }

      curItem->type = itemWFCH;
    }
    else if (strncmp("</",str, 2) == 0)
    {
      /* do nothing */
    }
    else
      fprintf(stderr, "not recognized: %s\n", str); 

  } while(!feof(f));

  fclose(f);

  LOG("Leave load_script()\n");

  return 0;
}


static void run_script(void)
{
  /* FIXME FNLEN doesn't make sense for size of these arrays */
  Mix_Chunk* sounds[FNLEN] = {NULL};

  /* --- for on mouse click on an image --- */
  Mix_Chunk* clickWavs[FNLEN] = {NULL};
  SDL_Rect   clickRects[FNLEN];

  LOG("\nEnter run_script()\n");

  if (!curScript)
  {
    fprintf(stderr, "run_script() - Error: curScript is NULL\n");
    return;
  }

  curPage = curScript->pages;

  while (curPage)
  {
    int y = 0;
    int skip = 0;
    int numWavs = 0;
    int numClicks = 0;

    curItem = curPage->items;

    /* --- setup background color --- */
    if (curPage->bgcolor)
      SDL_FillRect( screen, NULL, COL2RGB(curPage->bgcolor));
    else if (curScript->bgcolor)
      SDL_FillRect(screen, NULL, COL2RGB(curScript->bgcolor));

    /* --- setup background image --- */
    if (curPage->background)
    {
      SDL_Surface* img = LoadImage(curPage->background, IMG_ALPHA|IMG_NOT_REQUIRED);
      SDL_BlitSurface(img, NULL, screen, NULL);
      SDL_FreeSurface(img);
    }
    else if (curScript->background)
    {
      SDL_Surface* img = LoadImage(curScript->background, IMG_ALPHA|IMG_NOT_REQUIRED);
      SDL_BlitSurface(img, NULL, screen, NULL);
      SDL_FreeSurface(img);
    }

    /* --- go through all the items in the page --- */
    while (curItem)
    {
      switch (curItem->type)
      {
        case itemIMG:
        {
          SDL_Surface* img = LoadImage(curItem->data, IMG_ALPHA|IMG_NOT_REQUIRED);
          if (img)
          {
            /* --- figure out where to put it! --- */
            SDL_Rect loc;
            loc.w = img->w;
            loc.h = img->h;

            /* --- if user specifies y location, use it --- */
            if (curItem->y >= 0)
            {
              loc.y = curItem->y;
            }
            else
            {
              loc.y = y;
              y += loc.h;
            }

            /* --- if user specifies x location, use it --- */
            if (curItem->x >= 0)
            {
              loc.x = curItem->x;
            }
            else
            {
              switch (curItem->align)
              {
                case 'r':
                  loc.x = (screen->w) - (loc.w);
                  break;
                case 'c':
                  loc.x = ((screen->w) - (loc.w))/2;
                  break;
                default:
                  loc.x = 0;
                  break;
              }
            }

            /* --- and blit! --- */
            SDL_BlitSurface(img, NULL, screen, &loc);

            /* --- does it do click and play --- */
            if (curItem->onclick)
            {
              if (settings.sys_sound)
                clickWavs[numClicks] = LoadSound(curItem->onclick);
              clickRects[numClicks].x = loc.x;
              clickRects[numClicks].y = loc.y;
              clickRects[numClicks].w = loc.w;
              clickRects[numClicks].h = loc.h;
              numClicks++;
            }
          }
          SDL_FreeSurface(img);
          break;
        }

        case itemTEXT:
        {
          TTF_Font* myFont;
          SDL_Surface* img;
          SDL_Color* col;

          int shown, toshow, w, h; // used to wrap text
          char tmp[FNLEN];   // used to hold temp text for wrapping

          /* --- create font & render text --- */
          if (curItem->size > 0)
            myFont = LoadFont(settings.theme_font_name, (int)curItem->size);
          else
            myFont = LoadFont(settings.theme_font_name, 24); // default size is 24

          if (curItem->color)
            col = curItem->color;
          else if (curPage->fgcolor)
            col = curPage->fgcolor;
          else if (curScript->fgcolor)
            col = curScript->fgcolor;
          else 
            col = &white;

          shown = 0;


          do 
          {
            int ok = 0;

            if ((shown > 0) && (curItem->data[shown] == ' '))
              shown++;
            strncpy(tmp, &curItem->data[shown], FNLEN - 1);
            tmp[FNLEN - 1] = 0;
            tmp[strlen(curItem->data) - shown] = 0;

            for (toshow = strlen(&curItem->data[shown]); !ok; toshow--)
            {
              if (toshow + 1 > FNLEN)
                continue;

              tmp[toshow] = 0;
              TTF_SizeText(myFont, tmp, &w, &h);

              if (w + 20 < screen->w)
                ok = 1;
            }

            shown += toshow + 1;

            img = TTF_RenderUTF8_Blended(myFont, tmp, *col);

            if (img)
            {
              SDL_Rect loc;
              /* --- figure out where to put it! --- */
              loc.w = img->w;
              loc.h = img->h;

              /* --- if user specifies y location, use it --- */
              if (curItem->y >= 0)
                loc.y = curItem->y;
              else 
                loc.y=y; y+=loc.h;

              /* --- if user specifies x location, use it --- */
              if (curItem->x >= 0)
                loc.x = curItem->x;
              else 
              {
                switch (curItem->align)
                {
                  case 'r':
                    loc.x = (screen->w) - (loc.w);
                    break;
                  case 'c':
                    loc.x = ((screen->w) - (loc.w))/2;
                    break;
                  default:
                    loc.x = 0;
                  break;
                }
              }

              /* --- and blit! --- */
              SDL_BlitSurface( img, NULL, screen, &loc );
              SDL_FreeSurface( img );
            }
                    
          } while (shown + 1 < strlen(curItem->data));

          TTF_CloseFont(myFont);
          break;
        }


        case itemWAV:
        {
          // HACK, we need to make sure no more than 8 sounds or so..
          sounds[numWavs] = LoadSound( curItem->data );
          Mix_PlayChannel( numWavs, sounds[numWavs], -curItem->loop );
          numWavs++;
          break;
        }


        case itemWFIN:
        {
          int done = 0;

          // Make sure everything is on screen 
          SDL_Flip(screen);

          while (!done)
          {
            SDL_Delay(100);

            while (SDL_PollEvent(&event))
            {
              switch (event.type)
              {
                case SDL_MOUSEBUTTONDOWN:
                {
                  int j;

                  for (j=0; j<numClicks; j++) 
                  {
                    if (inRect(clickRects[j], event.button.x, event.button.y))
                     Mix_PlayChannel(numWavs + j, clickWavs[j], 0);
                  }
                  break;
                }

                case SDL_QUIT:
                {
                  curPage = NULL;
                  done = 1;
                  break;
                }

                case SDL_KEYDOWN: 
                {
                  switch (event.key.keysym.sym)
                  {
                    case SDLK_ESCAPE: 
                      curPage = NULL;
                      done = 1;
                      break;  // quit
                    case SDLK_LEFT: 
                      curPage = curPage->prev;
                      done = 1;
                      break;
                    case SDLK_RIGHT:
                    case SDLK_SPACE:
                    case SDLK_RETURN:
                      curPage = curPage->next;
                      skip = 1;
                      done = 1;
                      break;
                    default:
                      break;
                  };

                  break;
                }
              }
            }
          }
        }
        break;


        case itemWFCH:
        {
          int done = 0;
          // Make sure everything is on screen 
          SDL_Flip(screen);

          while (!done)
          {
            SDL_Delay(100);
            while (SDL_PollEvent(&event))
            {
              switch (event.type)
              {
                case SDL_QUIT:
                {
                  curPage = NULL;
                  done = 1;
                  break;
                }

                case SDL_KEYDOWN: 
                {
                  switch (event.key.keysym.sym)
                  {
                    case SDLK_ESCAPE: 
                    {
                      curPage = NULL;
                      done = 1;
                      break;  // quit
                    }
                    case SDLK_p:
                    {
                      curPage = curPage->next;
                      done = 1;
                      break;
                    }
                    default:
                      break;
                  };
                  break;
                }
              }
            }
          }
        }
        break;

        case itemPRAC:
        {
          wchar_t wide_buf[FNLEN];
          ConvertFromUTF8(wide_buf, curItem->data);
          if (curItem->goal > 0)
          {
            //printf( "goal is %d\n", curItem->goal );
            Phrases(wide_buf);
          }
          else
          {
            //printf( "No goal \n" );
            Phrases(wide_buf);
          }
          break;
        }

        default:
        {
          /* do nothing */
        }
      }

      if (curItem->next == NULL && curPage != NULL)
      {
        if (!skip)
        {
          curPage = curPage->next;
          skip = 0;
        }
        break;
      }
      else
        curItem = curItem->next;
    }
    SDL_Flip(screen);
    SDL_Delay(30);
        
        
    /* --- cleanup memory --- changing pages --- */
    { 
      int i;

      if (settings.sys_sound)
      {
        for (i=0; i<numWavs; i++)
        {
          Mix_HaltChannel(i);
          Mix_FreeChunk(sounds[i]);
        }

        for (i = 0; i < numClicks; i++)
        {
          Mix_HaltChannel(i + numWavs);
          Mix_FreeChunk(clickWavs[i]);
        }
      }
    }

  } /* --- End of "while (curPage)" loop ----*/

  LOG("Leave run_script()\n");
}


static void clear_items(itemType* i)
{
    itemType* n;

    /* if i is null, will return harmlessly: */
    while (i) {
        n = i->next;  // remember the next guy

        /* -- remove any data we are pointing to -- */
        free(i->data);
        free(i->onclick);
        free(i->color);

        /* -- remove ourselves --*/
        free(i);

        /* -- on to the next guy! -- */
        i = n;
    }
}

static void clear_pages(pageType* p)
{
    pageType* n;

    /* if p is null, will return harmlessly: */
    while (p) {
        n = p->next;  // remember the next guy

        /* -- remove all of our sub elements -- */
        clear_items(p->items);

        /* -- free anything we are pointing to --- */
        free(p->background);
        free(p->title);
        free(p->bgcolor);
        free(p->fgcolor);

        /* -- free ourselves -- */
        free(p);

        /* -- on to the next --*/
        p = n;
    }
}


static void close_script(void)
{
  if (curScript)
  {
    /* -- remove all the pages we have --*/
    clear_pages(curScript->pages);

    /* -- remove attributes we are pointing to -- */
    free(curScript->title);
    free(curScript->bgcolor);
    free(curScript->fgcolor);
    free(curScript->background);

    /* -- free yourself -- */
    free(curScript); 

    /* -- and remember you did -- */
    curScript = NULL;
  }
}




