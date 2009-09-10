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
#define MAX_LESSONS 100
#include "SDL_extras.h"
#include "convert_utf.h"

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

  /* Try theme script first: */
  if (!settings.use_english)
    sprintf( fn, "%s/scripts/cascade.xml", settings.theme_data_path);

  if (load_script( fn ) == 0) /* meaning successful load */
  {
    run_script();
    return;
  }

  /* If unsuccessful, fall back to default (English) script: */
  sprintf( fn, "%s/scripts/cascade.xml", settings.default_data_path);
  if (load_script( fn ) != 0)
    return; // bail if any errors occur

  run_script();
}


void InstructLaser(void)
{
  char fn[FNLEN]; 

  /* Try theme script first: */
  if (!settings.use_english)
    sprintf( fn, "%s/scripts/laser.xml", settings.theme_data_path);

  if (load_script( fn ) == 0) /* meaning successful load */
  {
    run_script();
    return;
  }


  /* If unsuccessful, fall back to default (English) script: */
  sprintf( fn, "%s/scripts/laser.xml", settings.default_data_path);
  if (load_script( fn ) != 0)
    return; // bail if any errors occur

  run_script();
}

void ProjectInfo(void)
{
  char fn[FNLEN]; 

  /* Try theme script first: */
  if (!settings.use_english)
    sprintf( fn, "%s/scripts/projectInfo.xml", settings.theme_data_path);

  if (load_script( fn ) == 0) /* meaning successful load */
  {
    run_script();
    return;
  }

  /* If unsuccessful, fall back to default (English) script: */
  sprintf( fn, "%s/scripts/projectInfo.xml", settings.default_data_path);
  if (load_script( fn ) != 0)
    return; // bail if any errors occur

  run_script();
}

/* This is the function that allows the user */
/* to select a lesson from the menu.                                          */
int XMLLesson(void)
{
  SDL_Surface* titles[MAX_LESSONS] = {NULL};
  SDL_Surface* select[MAX_LESSONS] = {NULL};
  SDL_Surface* left = NULL, *right = NULL;
  SDL_Rect leftRect, rightRect;
  SDL_Rect titleRects[8];

  int stop = 0;
  int loc = 0;
  int old_loc = 1;
  int num_scripts = 0;
  int found = 0;
  int i;
  char script_path[FNLEN];
  char script_filenames[MAX_LESSONS][200];
  char fn[FNLEN]; 

  DIR* script_dir = NULL;
  struct dirent* script_file = NULL;

  LOG("Entering XMLLesson():\n");

  /* First look in theme path, if desired: */
  if (!settings.use_english)
  {
    sprintf(script_path, "%s/scripts", settings.theme_data_path);
    if (CheckFile(script_path))
    {
      DEBUGCODE {fprintf(stderr, "Using theme script dir: %s\n", script_path);}
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
    fprintf(stderr, "XMLLesson(): Error finding script directory!\n");
    return 0;
  }


  /* If we get to here, we know there is at least a lesson script directory */
  /* but not necessarily any valid files.                              */

  DEBUGCODE { fprintf(stderr, "script_path is: %s\n", script_path); }


  /* FIXME looks like a place for scandir() - or our own w32_scandir() */
  /* create a list of all the .xml files */

  script_dir = opendir(script_path);	

  do
  {
    script_file = readdir(script_dir);
    if (!script_file)
      break;

    /* must have at least '.xml' at the end */
    if (strlen(script_file->d_name) < 5)
      continue;

    /* Don't show project info file or instructions files */
    if (strcmp(script_file->d_name, "projectInfo.xml") == 0 ||
        strcmp(script_file->d_name, "laser.xml") == 0 ||
        strcmp(script_file->d_name, "cascade.xml") == 0)
      continue;



    if (strcmp(&script_file->d_name[strlen(script_file->d_name) - 4],".xml"))
      continue;

    sprintf(script_filenames[num_scripts], "%s", script_file->d_name);
    num_scripts++;
    DEBUGCODE { fprintf(stderr, "Adding XML file no. %d: %s\n",
                num_scripts, script_filenames[num_scripts]); }

  } while (1); /* Leave loop when readdir() returns NULL */

  closedir(script_dir);	

  DEBUGCODE { fprintf(stderr, "Found %d . xml file(s) in script dir\n", num_scripts); }


  /* let the user pick the lesson script */
  for (i = 0; i < num_scripts; i++)
  {
    titles[i] = BlackOutline( script_filenames[i], DEFAULT_MENU_FONT_SIZE, &white );
    select[i] = BlackOutline( script_filenames[i], DEFAULT_MENU_FONT_SIZE, &yellow);
  }

  left = LoadImage("left.png", IMG_ALPHA);
  right = LoadImage("right.png", IMG_ALPHA);
  LoadBothBkgds("main_bkg.png");

  /* Get out if needed surface not loaded successfully: */
  if (!CurrentBkgd() || !left || !right)
  {
    fprintf(stderr, "XMLLesson(): needed image not available\n");
  
    for (i = 0; i < num_scripts; i++)
    {
      SDL_FreeSurface(titles[i]);
      SDL_FreeSurface(select[i]);
      titles[i] = select[i] = NULL;
    }

    SDL_FreeSurface(left);
    SDL_FreeSurface(right);
    left = right = NULL;

    return 0;
  }


  leftRect.w = left->w;
  leftRect.h = left->h;
  leftRect.x = screen->w/2 - 80 - (leftRect.w/2);
  leftRect.y = screen->h/2 - 50;

  rightRect.w = right->w; rightRect.h = right->h;
  rightRect.x = screen->w/2 + 80 - (rightRect.w/2); rightRect.y = screen->h/2 - 50;

  /* set initial rect sizes */
  titleRects[0].y = 30;
  titleRects[0].w = titleRects[0].h = titleRects[0].x = 0;

  for (i = 1; i < 8; i++)
  { 
    titleRects[i].y = titleRects[i - 1].y + 50;
    titleRects[i].w = titleRects[i].h = titleRects[i].x = 0;
  }

  /* Main event loop for this screen: */
  while (!stop)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          exit(0); /* FIXME may need to cleanup memory and exit more cleanly */
          break;

        case SDL_MOUSEMOTION:
          for (i = 0; (i < 8) && (loc - (loc % 8) + i < num_scripts); i++)
            if (inRect(titleRects[i], event.motion.x, event.motion.y ))
            {
              loc = loc - (loc % 8) + i;
              break;
            }
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (inRect( leftRect, event.button.x, event.button.y ))
          {
            if (loc - (loc % 8) - 8 >= 0)
            {
              loc = loc - (loc % 8) - 8;
              break;
            }
          }

          if (inRect(rightRect, event.button.x, event.button.y))
          {
            if (loc - (loc % 8) + 8 < num_scripts)
            {
              loc = loc - (loc % 8) + 8;
              break;
            }
          }

          for (i = 0; (i < 8) && (loc - (loc % 8) + i < num_scripts); i++)
          {
            if (inRect(titleRects[i], event.button.x, event.button.y))
            {
              loc = loc - (loc % 8) + i;
              sprintf(fn, "%s/scripts/%s", settings.default_data_path, script_filenames[loc]);
              stop = 1;
              break; 
            }
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
            sprintf(fn, "%s/scripts/%s", settings.default_data_path, script_filenames[loc]);
            stop = 1;
            break;
          }

          if ((event.key.keysym.sym == SDLK_LEFT)
           || (event.key.keysym.sym == SDLK_PAGEUP))
          {
            if (loc - (loc % 8) - 8 >= 0)
              loc = loc - (loc % 8) - 8;
          }

          if ((event.key.keysym.sym == SDLK_RIGHT)
           || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc - (loc % 8) + 8 < num_scripts)
              loc = (loc - (loc % 8) + 8);
          }

          if (event.key.keysym.sym == SDLK_UP)
          {
            if (loc > 0)
              loc--;
          }

          if (event.key.keysym.sym == SDLK_DOWN)
          {
            if (loc + 1 < num_scripts)
              loc++;
          }
      }
    }

    /* Redraw if we have changed location: */
    if (old_loc != loc)
    {
      int start;

      SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );

      start = loc - (loc % 8);

      for (i = start; i <  MIN(start + 8, num_scripts); i++) 
      {
        titleRects[i % 8].x = screen->w/2 - (titles[i]->w/2);
        if (i == loc)   /* Draw selected text in yellow:  */
          SDL_BlitSurface(select[loc], NULL, screen, &titleRects[i%8]);
        else            /* Draw unselected text in white: */
          SDL_BlitSurface(titles[i], NULL, screen, &titleRects[i%8]);
      }

      /* --- draw arrow buttons --- */
      if (start > 0)
        SDL_BlitSurface(left, NULL, screen, &leftRect);

      if (start + 8 < num_scripts)
        SDL_BlitSurface(right, NULL, screen, &rightRect);

      SDL_UpdateRect(screen, 0, 0, 0 ,0);
    }

    SDL_Delay(40);
    old_loc = loc;
  }

  /* --- clear graphics before leaving function --- */ 
  for (i = 0; i < num_scripts; i++)
  {
    if (titles[i])
      SDL_FreeSurface(titles[i]);
    if (select[i])
      SDL_FreeSurface(select[i]);
    titles[i] = select[i] = NULL;
  }

  SDL_FreeSurface(left);
  SDL_FreeSurface(right);
  left = right = NULL; /* Maybe overkill - about to be destroyed anyway */

  FreeBothBkgds();


  if (stop == 2)
  {
    SDL_ShowCursor(1);
    return 0;
  }

  /* Getting to here means "stop == 1", try to run chosen script: */
  if (load_script(fn) != 0)
  {
    fprintf(stderr, "load_script() failed to load '%s'\n",fn);
    SDL_ShowCursor(1);
    return 0; // bail if any errors occur
  }

  DEBUGCODE { fprintf(stderr, "Attempting to run script: %s\n", fn); }

  run_script();
  SDL_ShowCursor(1);

  LOG("Leave XMLLesson()\n");

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
    
    out = malloc(finish - start + 2);
    
    snprintf(out, finish - start + 1, "%s", &in[start]);
    out[finish-start] = 0;
    
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
    
  DEBUGCODE
  {
    fprintf(stderr, "\nEnter load_script() - attempt to load '%s'\n", fn);
  }

  if (curScript)
  {
    LOG( "previous script in memory, removing now!\n");
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
    /* Compiler complains if we don't inspect result of fscanf() */
    int fscanf_result = fscanf(f, "%[^\n]\n", str);
    if (fscanf_result == EOF)
      break;

    if (strncmp("<!--", str, 4) == 0)
    {
        /* -- comment section found, ignore everything until comment close -- */
        int found = 0;
        char* tmpStr;

        do
        {
            for ( tmpStr = str; strlen(tmpStr) >= 3; ++tmpStr )
            {    
                 if (strncmp("-->",tmpStr, 3) == 0)
                 {
                     found = 1;
                     tmpStr += 3;
                     if (strlen(tmpStr) > 0)
                     {
                         // copy the rest of the line into str for processing
                         strncpy(str, tmpStr, strlen(tmpStr));
                     }
                     else
                     {
                         // str needs another line, this one is used up
                         fscanf_result = fscanf(f, "%[^\n]\n", str);
                         tmpStr = str;
                         
                         // we may get consecutive comment lines
                         if (fscanf_result != EOF && strncmp("<!--", str, 4) == 0)
                         {
                             found = 0;
                         }
                     }
                 }
            }

            if (!found)
            {
                fscanf_result = fscanf(f, "%[^\n]\n", str);
                tmpStr = str;
            }

        } while ( fscanf_result != EOF && !found );
        
        /* -- if we reached the end of the file and saw no close to the comment, generate a warning -- */
        if ( !found && fscanf_result == EOF )
        {
            fprintf(stderr, "XML Warning: End of file reached looking for the end of a comment.\n", fn);
            break;
        }

        /* -- don't continue processing if at EOF -- */
        if (fscanf_result == EOF)
        {
            break;
        }
    }

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

      /* hack: since this is the background it needs to scale when in fullscreen
       * but shouldn't every image scale when in fullscreen? assuming svg is for that... -MDT */
      if (settings.fullscreen)
      {
        SDL_Surface* fsimg = zoom(img, fs_res_x, fs_res_y);
        SDL_BlitSurface(fsimg, NULL, screen, NULL);
        SDL_FreeSurface(fsimg);
      }
      else
      { 
        SDL_BlitSurface(img, NULL, screen, NULL);
      } 

      SDL_FreeSurface(img);

    }
    else if (curScript->background)
    {
      SDL_Surface* img = LoadImage(curScript->background, IMG_ALPHA|IMG_NOT_REQUIRED);

      /* hack: since this is the background it needs to scale when in fullscreen -MDT */
      if (settings.fullscreen)
      { 
        SDL_Surface* fsimg = zoom(img, fs_res_x, fs_res_y);
        SDL_BlitSurface(fsimg, NULL, screen, NULL);
        SDL_FreeSurface(fsimg);
      }
      else
      { 
        SDL_BlitSurface(img, NULL, screen, NULL);
      } 

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
          SDL_Surface* img;
          SDL_Color* col;

          int shown, toshow; // used to wrap text
          char tmp[FNLEN];   // used to hold temp text for wrapping

           /* Sanity check - use default font size if size not specified: */
          if ((curItem->size < 8) || (curItem->size > 80))
            curItem->size = DEFAULT_SCRIPT_FONT_SIZE; //currently 24

          if (curItem->color)
            col = curItem->color;
          else if (curPage->fgcolor)
            col = curPage->fgcolor;
          else if (curScript->fgcolor)
            col = curScript->fgcolor;
          else 
            col = (SDL_Color*)&white;

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
 
              img = SimpleText(tmp, (int)curItem->size, col);

              if (img)
              { 
                if (img->w + 20 < screen->w)
                  ok = 1;
                SDL_FreeSurface(img);
                img = NULL;
              }
            }

            shown += toshow + 1;

            img = SimpleText(tmp, (int)curItem->size, col);

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
              SDL_BlitSurface(img, NULL, screen, &loc);
              SDL_FreeSurface(img);
            }
                    
          } while (shown + 1 < strlen(curItem->data));

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
          ConvertFromUTF8(wide_buf, curItem->data, FNLEN);
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




