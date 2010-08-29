/*
   editor.c:

   Description: word- and phrase list-editing functionality for
   Tux Typing.
   
   Copyright 2009, 2010.
   Authors: Sarah Frisk <ssfrisk@gmail.com> and 
            David Bruce <davidstuartbruce@gmail.com>.
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   editor.c is part of Tux Typing, a.k.a "tuxtype".

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
#include "SDL_image.h"
#include "convert_utf.h"
#include "editor.h"


void ChooseListToEdit(void)
{
  SDL_Surface* new_button = NULL;
  SDL_Surface* remove_button = NULL;
  SDL_Surface* done_button = NULL;
  SDL_Surface *NEW = NULL, *REMOVE = NULL, *DONE = NULL;
  //this is text:
  SDL_Surface *directions[5] = {NULL};
	//buttons for left + right
  static SDL_Surface *left = NULL, *right = NULL;
  static SDL_Rect leftRect, rightRect;

  SDL_Rect directions_Rect[5];
  SDL_Rect button_rect[3];
  SDL_Rect button_text_rect[3]; 

  int stop = 0;
  int loc = 0;
  int old_loc = 1;
  int num_lists = 0;
  int i, j = 0;
  int redraw = 0;
  int change = 0;


  //Arrays for the list of editable word lists:
  char file_names[MAX_WORD_LISTS][FNLEN];  //names of the files containing lists
  char list_titles[MAX_WORD_LISTS][MAX_WORD_SIZE+1]; //text list name to be displayed
  //Surfaces of list_titles[] rendered for graphical display:
  SDL_Surface* white_titles_surf[MAX_WORD_LISTS + 1] = {NULL};  //unselected titles - white lettering; 
  SDL_Surface* yellow_titles_surf[MAX_WORD_LISTS + 1] = {NULL}; //selected titles - yellow lettering
  // Rects where list names will be drawn on screen:
  static SDL_Rect titleRects[8];

  SDL_Surface *max_title_size = NULL; //using this to figure out size of wordlist name

  //Temporary holders and ptrs used while scanning list directory:
  char wordsDir[FNLEN];
  char fn[FNLEN];                             
  FILE* fp = NULL;
  DIR* lists_dir = NULL;
  struct dirent* list_dirent = NULL;

  /* First part - scan through our word list directory and create lists */
  /* of the filenames and titles (first lines in files).                */
  /* NOTE we could use scandir() to do almost all of this as we do      */
  /* in tuxmath.                                                        */


  //Try to open a directory for modifiable word lists:

  sprintf(wordsDir, "%s/words", settings.user_settings_path);
  if (CheckFile(wordsDir))
  {
    DEBUGCODE { fprintf(stderr, "User specific wordlist path found: %s\n", wordsDir); }
  }
  else
  {
    DEBUGCODE { fprintf(stderr , "Editor: checking directory: %s/words", settings.var_data_path); }
    sprintf(wordsDir , "%s/words" , settings.var_data_path);
  }
  lists_dir = opendir(wordsDir);

  if (!lists_dir)
  {
    fprintf(stderr, "ChooseListToEdit() - cannot open custom word list directory!\n");
    return;
  }

  //Now scan through directory and gather file names and list titles:
  while (1)
  {
    list_dirent = readdir(lists_dir);
    if (!list_dirent)
      break;

   /* we ignore any hidden file and CVS */

    if (list_dirent->d_name[0] == '.') 
      continue;

    if (strcmp("CVS", list_dirent->d_name) == 0)
      continue;

    /* must have at least .txt at the end */
    if (strlen(list_dirent->d_name) < 5)
      continue;

    if (strcmp(&list_dirent->d_name[strlen(list_dirent->d_name) -4 ],".txt"))
      continue;

    snprintf(fn, FNLEN, "%s/%s" , wordsDir, list_dirent->d_name); 

    /* CheckFile() returns 2 if dir, 1 if file, 0 if neither: */
    if (CheckFile(fn) == 1)
    {
      /* We know it opens safely because CheckFile() returned 1 */
      fp = fopen(fn,"r");
      /* HACK: we should get the names from file :) */
      if (EOF ==fscanf(fp, "%[^\n]\n", list_titles[num_lists]))
        continue;
      /* Make sure list title is capitalized: */
      list_titles[num_lists][0] = toupper(list_titles[num_lists][0]);
      fclose(fp);
      strncpy(file_names[num_lists++], list_dirent->d_name, FNLEN-1);
    }
  }
  closedir(lists_dir);




  /* Render SDL_Surfaces of title text for later blitting: */
  for (i = 0; i < num_lists; i++)
  {
    white_titles_surf[i] = BlackOutline(list_titles[i], DEFAULT_MENU_FONT_SIZE, &white);
    yellow_titles_surf[i] = BlackOutline(list_titles[i], DEFAULT_MENU_FONT_SIZE, &yellow);
  }
 
  /* Render text and instructions */
  directions[0] = BlackOutline(gettext_noop("Word List Editor"), 20, &yellow);
  directions[1] = BlackOutline(gettext_noop("To add a new word list, click the 'NEW' button"), 11, &white);
  directions[2] = BlackOutline(gettext_noop("To remove a word list, select the wordlist with the arrow buttons, then click the 'REMOVE' button or the 'DELETE' key"), 11, &white);
  directions[3] = BlackOutline(gettext_noop("To edit a word list, select the wordlist with the arrow buttons, then press the 'RETURN' key"), 11, &white);
  directions[4] = BlackOutline(gettext_noop("To exit Word List Editor, press the 'ESC' key, or click on the 'DONE' button"), 11, &white);	

  max_title_size = BlackOutline(gettext_noop("WWWWWWWWW"), DEFAULT_MENU_FONT_SIZE, &yellow);

  NEW = BlackOutline(gettext_noop("NEW"), 25, &yellow);
  DONE = BlackOutline(gettext_noop("DONE"), 25, &yellow);
  REMOVE = BlackOutline(gettext_noop("REMOVE"), 25, &yellow);

  /* Load image of new/remove/done buttons: */
  new_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  remove_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  done_button = LoadImage("wordlist_button.png", IMG_ALPHA);

	/*Load image for left and right buttons: */
  left = LoadImage("left.png", IMG_ALPHA);
  right = LoadImage("right.png", IMG_ALPHA);

  LOG( "ChooseFile() - drawing screen\n");


  /* Draw the initial text and images that won't change as list is examined: */
  directions_Rect[0].x = screen->w/2 - (directions[0]->w/2);
  directions_Rect[0].y = 10;
  directions_Rect[0].w = directions[0]->w;
  directions_Rect[0].h = directions[0]->h;

  j = 30;

  for (i = 1; i < 5; i++)
  {
    j += 20;
    directions_Rect[i].x = screen->w/2 - (directions[i]->w/2);
    directions_Rect[i].y = j;
    directions_Rect[i].w = directions[i]->w;
    directions_Rect[i].h = directions[i]->h;
  }

  button_rect[New].x = screen->w - new_button->w - 20; 
  button_rect[New].y = screen->h/3;
  button_text_rect[New].x = screen->w - new_button->w - 20 + (new_button->w/2 - NEW->w/2); 
  button_text_rect[New].y = screen->h/3 + (NEW->h/2);

  button_rect[Remove].x = button_rect[New].x;
  button_rect[Remove].y = button_rect[New].y + remove_button->h + 10;
  button_text_rect[Remove].x = screen->w - remove_button->w - 20 + (remove_button->w/2 - REMOVE->w/2); 
  button_text_rect[Remove].y =  button_text_rect[New].y + remove_button->h + 10;

  button_rect[Done].x = button_rect[Remove].x;
  button_rect[Done].y = button_rect[Remove].y + done_button->h + 10;
  button_text_rect[Done].x = screen->w - done_button->w - 20 + (done_button->w/2 - DONE->w/2); 
  button_text_rect[Done].y = button_text_rect[Remove].y + done_button->h + 10;

  leftRect.w = left->w;
  leftRect.h = left->h;
  leftRect.x = screen->w/10 - (leftRect.w/2);
  leftRect.y = 415;

  rightRect.w = right->w;
  rightRect.h = right->h;
  rightRect.x = screen->w/10 + 100 - (rightRect.w/2);
  rightRect.y = 415;


  /* set initial rect sizes */
  titleRects[0].y = screen->h / 3 + (max_title_size->h/2);
  titleRects[0].w = max_title_size->w;
  titleRects[0].h = max_title_size -> h; 
  titleRects[0].x = screen->w / 10;

  for (i = 1; i < 8; i++)
  {
    titleRects[i].y = titleRects[i-1].y + 30;
    titleRects[i].w = titleRects[i].h = 0;
    titleRects[i].x = screen->w / 10;
  }

  SDL_UpdateRect(screen, 0, 0, 0, 0);
  
  /* Event loop for this screen: */
  while (!stop)
  {

    /* Handle user input: */
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          exit(0);
          break;

        case SDL_MOUSEBUTTONDOWN: 
          if (inRect(leftRect, event.button.x, event.button.y)) 
            if (loc - (loc % 8) - 8 >= 0) 
            {
              loc = loc - (loc % 8) - 8;
              break;
            }

          if (inRect(rightRect, event.button.x, event.button.y)) 
            if (loc - (loc % 8) + 8 < num_lists)
            {
              loc = loc - (loc % 8) + 8;
              break;
            }

          if (inRect(button_rect[New], event.button.x, event.button.y)) 
          {
            change = CreateNewWordList();			
            if (!change)
              redraw = 1;
          }

          if (inRect(button_rect[Remove], event.button.x, event.button.y))
          {
            //pop up something?
            ChooseRemoveList(list_titles[loc], file_names[loc]);
            change = 1;
          }

          if (inRect(button_rect[Done], event.button.x, event.button.y)) 
          {
            stop = 1; 
            break;
          }
          break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_BACKSPACE)
          {
            //Remove wordlist
            ChooseRemoveList(list_titles[loc], file_names[loc]);
            change = 1;
          }

          if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            stop = 1; 
            break; 
          }

          // User going to actual word editing:
          if ((event.key.keysym.sym == SDLK_RETURN)
           || (event.key.keysym.sym == SDLK_SPACE))
          {
            EditWordList(file_names[loc]);
            loc = 0;
            redraw = 1;
            break;
          }
          // Go to top of previous page:
          if ((event.key.keysym.sym == SDLK_LEFT)
           || (event.key.keysym.sym == SDLK_PAGEUP))
          {
            if (loc - (loc % 8) - 8 >= 0)
              loc = loc - (loc % 8) - 8;
          }
          // Go to top of next page:
          if ((event.key.keysym.sym == SDLK_RIGHT)
           || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc - (loc % 8) + 8 < num_lists)
              loc = (loc - (loc % 8) + 8);
          }

          if (event.key.keysym.sym == SDLK_UP)
          {
            if (loc > 0)
              loc--;
          }

          if (event.key.keysym.sym == SDLK_DOWN)
          {
            if (loc + 1 < num_lists)
              loc++;
          }
      }
    }  //End of user event handling

    /* reload the list if it has been changed */
    if (change)
    {
      num_lists = 0;
      //Try to open directory for modifiable word lists:
      sprintf(fn , "%s" , wordsDir);
      lists_dir = opendir(fn);

      if (!lists_dir)
      {
        LOG("ChooseListToEdit() - cannot open custom word list directory!\n");
        return;
      }

      //FIXME we should use scandir() for this - DSB
      //Now scan through directory and gather file names and list titles:
      while (1)
      {
        list_dirent = readdir(lists_dir);
        if (!list_dirent)
          break;

        /* we ignore any hidden file and CVS */
        if (list_dirent->d_name[0] == '.') 
          continue;
        if (strcmp("CVS", list_dirent->d_name) == 0)
          continue;

        snprintf(fn, FNLEN, "%s/%s" , wordsDir, list_dirent->d_name); 

        /* CheckFile() returns 2 if dir, 1 if file, 0 if neither: */
        if (CheckFile(fn) == 1)
        {
          /* We know it opens safely because CheckFile() returned 1 */
          fp = fopen(fn,"r");
          /* HACK: we should get the names from file :) */
          if (EOF ==fscanf(fp, "%[^\n]\n", list_titles[num_lists]))
            continue;
          /* Make sure list title is capitalized: */
          list_titles[num_lists][0] = toupper(list_titles[num_lists][0]);
          fclose(fp);
          strncpy(file_names[num_lists++], list_dirent->d_name, FNLEN-1);
        }
      }
      closedir(lists_dir);


      // white_titles_surf[MAX_WORD_LISTS] = {NULL};  
      // yellow_titles_surf[MAX_WORD_LISTS] = {NULL};


      /* Render SDL_Surfaces of title text for later blitting: */
      for (i = 0; i < num_lists; i++)
      {
        white_titles_surf[i] = BlackOutline(list_titles[i],
                                            DEFAULT_MENU_FONT_SIZE, &white);
        yellow_titles_surf[i] = BlackOutline(list_titles[i],
                                            DEFAULT_MENU_FONT_SIZE, &yellow);
      }

      change = 0;	
      redraw = 1;
    }



    /* Redraw if a different menu entry is selected: */
    if (old_loc != loc || redraw == 1)
    {
      int start;

      if(CurrentBkgd())
        SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);

      /*FIXME: simplify this into a function*/
      for (i = 0; i < 5; i++)
        SDL_BlitSurface( directions[i], NULL, screen, &directions_Rect[i]);

      SDL_BlitSurface(new_button, NULL, screen, &button_rect[New]);
      SDL_BlitSurface(NEW, NULL, screen, &button_text_rect[New]);
      SDL_BlitSurface(remove_button, NULL, screen, &button_rect[Remove]);
      SDL_BlitSurface(REMOVE, NULL, screen, &button_text_rect[Remove]);
      SDL_BlitSurface(done_button, NULL, screen, &button_rect[Done]);
      SDL_BlitSurface(DONE, NULL, screen, &button_text_rect[Done]);

      start = loc - (loc % 8);

      for (i = start; i < MIN (start + 8, num_lists); i++)
      {
        //titleRects[i % 8].x = 320 - (white_titles_surf[i]->w/2);
        if (i == loc)
          SDL_BlitSurface(yellow_titles_surf[loc], NULL, screen, &titleRects[i % 8]);
        else
          SDL_BlitSurface(white_titles_surf[i], NULL, screen, &titleRects[i % 8]);
        SDL_UpdateRect(screen, titleRects[i%8].x, titleRects[i%8].y,
                               titleRects[i%8].w, titleRects[i%8].h);
      }

      /* --- draw right and left arrow buttons --- */
      if (start > 0) 
        SDL_BlitSurface( left, NULL, screen, &leftRect );
      if (start + 8 < num_lists) 
        SDL_BlitSurface(right, NULL, screen, &rightRect);

      SDL_UpdateRect(screen, 0, 0, 0, 0); 
      redraw = 0;
    }
    SDL_Delay(40);
    old_loc = loc;
  }

  /* Cleanup all dynamically-allocated memory: */

  /* NOTE pointers going out of scope so don't need to set them to NULL, */
  /* but we should do that if we move this into a cleanup function.      */
  /* --- clear graphics before quitting --- */ 
  for (i = 0; i < num_lists; i++)
  {
    if(white_titles_surf[i])
      SDL_FreeSurface(white_titles_surf[i]);
    if(yellow_titles_surf[i])
      SDL_FreeSurface(yellow_titles_surf[i]);
  }

  for (i = 0; i < 5; i ++)
  {
    if(directions[i])
      SDL_FreeSurface(directions[i]);
  }

  if(new_button)
    SDL_FreeSurface(new_button);
  if(remove_button)
    SDL_FreeSurface(remove_button);
  if(done_button)
    SDL_FreeSurface(done_button);
  if(NEW)
    SDL_FreeSurface(NEW);
  if(REMOVE)
    SDL_FreeSurface(REMOVE);
  if(DONE)
    SDL_FreeSurface(DONE);
  if(left)
    SDL_FreeSurface(left);
  if(right)
    SDL_FreeSurface(right);
}


/* FIXME we already have a GenerateWordList() function in alphabet.c       */
/* that puts all the words from  a supplied file name into word_list[][] - */
/* let's not duplicate that code.                                          */
void EditWordList(char* words_file)
{
	/* Need to figure out how to handle empty lists */
  static SDL_Surface* white_words[MAX_NUM_WORDS] = {NULL};
  static SDL_Surface* yellow_words[MAX_NUM_WORDS] = {NULL};
  static SDL_Surface *left = NULL, *right = NULL;
  static SDL_Surface *wordlist_name = NULL;
  static SDL_Surface *title = NULL;
  static SDL_Surface *directions[4] = {NULL};
  static SDL_Rect directions_Rect[4];
  static SDL_Rect titleRect;
  static SDL_Rect leftRect, rightRect;
  static SDL_Rect wordlist_name_rect;
  SDL_Rect word_rects[8];
  int stop = 0;
  int loc = 0;
  int old_loc = 1;

  FILE* fp = NULL;

  int start, number_of_words = 0;
  int i, len, j = 0; 
  int listening_for_new_word = 0;
  char fn[FNLEN];
  char wordsDir[FNLEN];
  char words_in_list[MAX_NUM_WORDS][MAX_WORD_SIZE + 1];

  wchar_t temp[MAX_WORD_SIZE + 1];

  // get appropriate directory
  sprintf(wordsDir, "%s/words", settings.user_settings_path);
  if (CheckFile(wordsDir))
  {
    DEBUGCODE { fprintf(stderr, "User specific wordlist path found: %s\n", wordsDir); }
  }
  else
  {
    DEBUGCODE { fprintf(stderr , "Editor: checking directory: %s/words", settings.var_data_path); }
    sprintf(wordsDir , "%s/words" , settings.var_data_path);
  }


  //We should be able to use GenerateWordList() in place of this next block:
  //NOTE: Works originally, but upon returning to editorlist, the word selected
  // is not there, since all words in the wordlist are deleted
  sprintf(fn , "%s/%s", wordsDir,  words_file);
  fp = fopen(fn,"r");
  number_of_words = 0;  

  //Doing this with fgets() rather than fscanf() because the line delimiter
  //may be either '\n' (Linux/Unix) or '\r' (Windows)
  if(fp)
  {
    char* p = NULL;
    while (fgets(words_in_list[number_of_words], MAX_WORD_SIZE, fp))
    {
      /* Get rid of \n or \r at end: */
      p = strchr(words_in_list[number_of_words], '\n');
      if(p)
        *p = '\0';
      p = strchr(words_in_list[number_of_words], '\r');
      if(p)
        *p = '\0';

      number_of_words++;

      DEBUGCODE{ fprintf(stderr, "Read word \"%s\" from file, number_of_words = %d\n",
                 words_in_list[number_of_words - 1], number_of_words); }
    }
  }
  fclose(fp);
  fp = NULL;

  /* Prepare needed SDL_Surfaces: */

  /* Render the words in white and yellow: */
  for (i = 0; i < number_of_words-1; i++)
  {
    white_words[i] = BlackOutline(words_in_list[i+1], DEFAULT_MENU_FONT_SIZE, &white);
    yellow_words[i] = BlackOutline(words_in_list[i+1], DEFAULT_MENU_FONT_SIZE, &yellow);
  }

  left = LoadImage("left.png", IMG_ALPHA);
  right = LoadImage("right.png", IMG_ALPHA);
  title = BlackOutline(_("Word List Editor:"), 20, &yellow);
  wordlist_name = BlackOutline(words_in_list[0], 25, &white);

  directions[0] = BlackOutline(_("To add a word, press 'RETURN' and start typing"), 11, &white); 
  directions[1] = BlackOutline(_("To edit a word, select the word using the arrow buttons and continue typing"), 11, &white); 
  directions[2] = BlackOutline(_("To delete a character, select the word you want to edit and press the 'DELETE' key"), 11, &white); 
  directions[3] = BlackOutline(_("To exit and save the word list, press 'ESC'"), 11, &white); 

  /* FIXME these need to be scaled to screen size */
  /* Set up SDL_Rect locations for later blitting: */
  leftRect.w = left->w;
  leftRect.h = left->h;
  leftRect.x = screen->w/10 - (leftRect.w/2);
  leftRect.y = 415;

  rightRect.w = right->w;
  rightRect.h = right->h;
  rightRect.x = screen->w/10 + 100 - (rightRect.w/2);
  rightRect.y = 415;

  word_rects[0].y = screen->h / 3;
  word_rects[0].w = word_rects[0].h = 0;
  word_rects[0].x = screen->w / 10;

  for (i = 1; i < 8; i++)
  {
    word_rects[i].y = word_rects[i - 1].y + 30;
    word_rects[i].w = word_rects[i].h = 0;
    word_rects[i].x = screen->w / 10;
  }

  j = 10;
  titleRect.x = screen->w/2 - (title->w/2);
  titleRect.y = j;
  titleRect.w = title->w;
  titleRect.h = title->w;

  j += 30;
  wordlist_name_rect.w = wordlist_name->w;
  wordlist_name_rect.h = wordlist_name->h;
  wordlist_name_rect.y = j;
  wordlist_name_rect.x = screen->w/2 - wordlist_name->w/2;

  j = 50;
  for ( i = 0; i < 4; i++)
  {
    j += 20;
    directions_Rect[i].y = j;
    directions_Rect[i].x = screen->w/2 - directions[i]->w/2;
    directions_Rect[i].w = directions[i]->w;
    directions_Rect[i].h = directions[i]->h;
  }


  /* Main event loop for word editor: */
  while (!stop) 
  {
    while (SDL_PollEvent(&event)) 
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
          exit(0);
          break;
        }

        case SDL_MOUSEBUTTONDOWN:
        { 
          if (inRect(leftRect, event.button.x, event.button.y)) 
            if (loc - (loc % 8) - 8 >= 0) 
            {
              loc = loc - (loc % 8) - 8;
              break;
            }

          if (inRect(rightRect, event.button.x, event.button.y)) 
            if (loc - (loc % 8) + 8 < number_of_words)
            {
              loc = loc - (loc % 8) + 8;
              break;
            }

          for (i = 0; (i < 8) && (loc - (loc % 8) + i < number_of_words); i++)
          {
            if (inRect(word_rects[i], event.motion.x, event.motion.y))
            {
              loc = loc - (loc % 8);
               break;
            }

          }
          break;
        }

        case SDL_KEYDOWN:
        {
          i = 1;

          if (event.key.keysym.sym == SDLK_BACKSPACE)
          {
            len = ConvertFromUTF8(temp, words_in_list[loc+1], MAX_WORD_SIZE); 
            if (len > 1 && number_of_words > 1)
            {                               
              // remove the last character from the string
              temp[len - 1] = temp[len];
              len = ConvertToUTF8(temp, words_in_list[loc+1], MAX_WORD_SIZE);
              white_words[loc] = BlackOutline(words_in_list[loc+1], DEFAULT_MENU_FONT_SIZE, &white );
              yellow_words[loc] = BlackOutline(words_in_list[loc+1], DEFAULT_MENU_FONT_SIZE, &yellow);  
            }
            else
            {
              // we have to remove the word from the list //
              DEBUGCODE
              {
                fprintf(stderr, "The number of words is %i\n", number_of_words);
                fprintf(stderr, "The length is %i \n", len);
              }

              if (number_of_words > 1)
              {
                int x = 0;
                number_of_words --;

                DEBUGCODE
                { fprintf(stderr, "There are current: %i words\n", number_of_words); }

                for(x = loc; x <= number_of_words-1; x++)
                {
                  if(x < number_of_words-1)
                  {
                    len = ConvertFromUTF8(temp, words_in_list[x+2], MAX_WORD_SIZE);

                    DEBUGCODE
                    {
                      fprintf(stderr, "X = %i\n", x);
                      fprintf(stderr, "loc = %i\n", loc);
                      fprintf(stderr, "word in list = %s\n", words_in_list[x+2]);
                    }

                    len = ConvertToUTF8(temp, words_in_list[x+1], MAX_WORD_SIZE);

                    DEBUGCODE
                    { fprintf(stderr, "word in list = %s\n", words_in_list[x+1]); }

                    white_words[x] = BlackOutline(words_in_list[x+1],
                                                  DEFAULT_MENU_FONT_SIZE, &white ); 
                    yellow_words[x] = BlackOutline(words_in_list[x+1],
                                                   DEFAULT_MENU_FONT_SIZE, &yellow);
                  }
                  else
                  {
                    white_words[x] = NULL;
                    yellow_words[x] = NULL;
                  }
                }


                if (loc == number_of_words)
                  loc --;

                DEBUGCODE
                { fprintf(stderr, "There are current: %i words\n", number_of_words); }
              }

              white_words[loc] = BlackOutline(words_in_list[loc+1],
                                              DEFAULT_MENU_FONT_SIZE, &white );
              yellow_words[loc] = BlackOutline(words_in_list[loc+1],
                                               DEFAULT_MENU_FONT_SIZE, &yellow);	

              //handle deletion of words better, right now don't really do that
            }
            break;
          }  // end of handling of SDLK_BACKSPACE

          if (event.key.keysym.sym == SDLK_ESCAPE) 
          {
            stop = 1;
            break; 
          }

          if ((event.key.keysym.sym == SDLK_LEFT)
           || (event.key.keysym.sym == SDLK_PAGEUP))
          {
            if (loc - (loc % 8) - 8 >= 0)
              loc = loc - (loc % 8) - 8;
            DEBUGCODE
            { fprintf(stderr, "loc  = %i\n", loc); }
            break;
          }

          if ((event.key.keysym.sym == SDLK_RIGHT)
           || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc - (loc % 8) + 8 < number_of_words-1)
              loc = (loc - (loc % 8) + 8);
            DEBUGCODE
            { fprintf(stderr, "loc  = %i\n", loc); }
            break;
          }

          if (event.key.keysym.sym == SDLK_UP)
          {
            if (loc > 0)
              loc--;
            DEBUGCODE
            { fprintf(stderr, "loc  = %i\n", loc); }
            break;
          }

          if (event.key.keysym.sym == SDLK_DOWN)
          {
            if (loc + 1 < number_of_words - 1)
              loc++;
            DEBUGCODE
            { fprintf(stderr, "loc  = %i\n", loc); }
            break;
          }

          //FIXME this switch should include above cases, too
          switch (event.key.keysym.sym)
          {
            case SDLK_RETURN:
              DEBUGCODE
              { fprintf(stderr, "number of words: %i", number_of_words); }

              if (number_of_words < MAX_WORD_LISTS)
                listening_for_new_word = 1;
              else
                LOG("Couldn't add new word, this wordlist is full.\n");	
            case SDLK_CAPSLOCK:
            case SDLK_RALT:
            case SDLK_LALT:
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
            case SDLK_RCTRL:
            case SDLK_LCTRL:
              i = 0;
              break;
            default:  // ignore any other keys 
              {}
          }

          if(i)  //FIXME what is i?
          {
            DEBUGCODE
            {
              fprintf(stderr, "loc  = %i\n", loc);
              fprintf(stderr, "number of words  = %i\n", number_of_words);	
            }

            // If it's listening for a new word, from having last 
            // pressed enter, create a whole new word with a length 
            // of 0, else get the current length of the highlighted word
            if (listening_for_new_word)
            {
              loc = number_of_words - 1;
              number_of_words++;
              listening_for_new_word = 0;
              len = 0;
            }
            else
            {
              len = ConvertFromUTF8(temp, words_in_list[loc + 1], MAX_WORD_SIZE);
            }
            if (len < MAX_WORD_SIZE - 1)
            {
              // Add the character to the end of the existing string
              temp[len] = toupper(event.key.keysym.unicode);
              temp[len + 1] = 0;
              ConvertToUTF8(temp, words_in_list[loc + 1], MAX_WORD_SIZE);

              // Copy back to the on-screen list
              white_words[loc] = BlackOutline(words_in_list[loc + 1],
                                              DEFAULT_MENU_FONT_SIZE, &white);
              yellow_words[loc] = BlackOutline(words_in_list[loc + 1],
                                              DEFAULT_MENU_FONT_SIZE, &yellow);
            }
            i = 0;
            break;
          }
        }  // end of CASE SDL_KEYDOWN:
      }  // end of 'switch (event.type)'

      /* Redraw screen: */
      /* FIXME looks like we are redrawing every time through loop whether */
      /* we need it or not.                                                */
	
      if(!stop)
      {
        SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );
        start = loc - (loc % 8);
        for (i = start; i < MIN(start + 8, number_of_words-1); i++)
        {
          word_rects[i % 8].x = screen->w / 10;
          if (i == loc)
            SDL_BlitSurface(yellow_words[loc], NULL, screen, &word_rects[i % 8]);
          else
            SDL_BlitSurface(white_words[i], NULL, screen, &word_rects[i % 8]);
        }

        for(i = 0; i < 4; i++)
          SDL_BlitSurface(directions[i], NULL, screen, &directions_Rect[i]);

        SDL_BlitSurface(title, NULL, screen, &titleRect);
        SDL_BlitSurface(wordlist_name, NULL, screen, &wordlist_name_rect);

        /* --- draw right and left arrow buttons --- */
        if (start > 0) 
          SDL_BlitSurface(left, NULL, screen, &leftRect);
        if (start + 8 < number_of_words-1) 
          SDL_BlitSurface(right, NULL, screen, &rightRect);

        SDL_UpdateRect(screen, 0, 0, 0 ,0);
      }
      SDL_Delay(40);  // I assume throttling so we don't eat all CPU
      old_loc = loc;
    }  // End of 'while (SDL_PollEvent(&event))' loop
  }  // End of 'while(!stop)' loop

  /* End of main event loop */


  /*FIXME: somehow when this is first saved, there is a space between
  each of the new words in the saved file.  This is fixed upon reloading the
  word list*/

  /* Write changes to file, if possible: */
  LOG("In EditWordList(), about to write changes\n");
  fp = fopen(fn,"w");

  if (fp)
  { 
//    fseek(fp, 0, SEEK_SET);
    i = 0;
    while(i < number_of_words) 
    {
      fprintf(fp, "%s\n", words_in_list[i]);
      DEBUGCODE{ fprintf(stderr, "Writing \"%s\" to file\n", words_in_list[i]); }
      i++;
    }

    fclose(fp); 
    fp = NULL;
    LOG("In EditWordList(), changes written successfully\n");
  }
  else
  {
    DEBUGCODE
    { fprintf(stderr, "In EditWordList(), unable to open %s for writing\n", fn); }
  }

  /* --- clear graphics before quitting --- */ 
  for (i = 0; i < MAX_WORD_LISTS; i++)
  {
    if(white_words[i] != NULL)
    {
      SDL_FreeSurface(white_words[i]);
      white_words[i] = NULL;
    }
    if(yellow_words[i] != NULL)
    {
      SDL_FreeSurface(yellow_words[i]);
      yellow_words[i] = NULL;
    }
  }

  for (i = 0; i < 4; i ++)
  {
    if(directions[i])
      SDL_FreeSurface(directions[i]);
  }

  if(left)
    SDL_FreeSurface(left);
  if(right)
    SDL_FreeSurface(right);
  /* the pointers are going out of scope so we don't */
  /* have to worry about setting them to NULL              */
}              


/* "Private" functions */
int CreateNewWordList(void)
{
  int stop = 0;
  int save = 0;
  int len = 0; //len = length, 
  int i = 0; //i = checks for keydown
  SDL_Surface* OK_button = NULL;
  SDL_Surface* CANCEL_button = NULL;
  SDL_Surface *OK = NULL, *CANCEL = NULL;
  SDL_Surface *NewWordlist = NULL;
  SDL_Surface *Direction1 = NULL;
  SDL_Surface *Direction2 = NULL;
  SDL_Rect OK_rect; 
  SDL_Rect CANCEL_rect;
  SDL_Rect OK_rect_text; 
  SDL_Rect CANCEL_rect_text;
  SDL_Rect Text;
  SDL_Rect Directions_rect;
  FILE* fp = NULL;
  char fn[FNLEN];
  char wordsDir[FNLEN];
  char wordlist[MAX_WORD_SIZE + 1];
  wchar_t temp[MAX_WORD_SIZE + 1];
  wordlist[0] = 0;

  LOG("Enter CreateNewWordList()\n");

  // get appropriate directory
  sprintf(wordsDir, "%s/words", settings.user_settings_path);
  if (CheckFile(wordsDir))
  {
    DEBUGCODE { fprintf(stderr, "User specific wordlist path found: %s\n", wordsDir); }
  }
  else
  {
    DEBUGCODE { fprintf(stderr , "Editor: checking directory: %s/words", settings.var_data_path); }
    sprintf(wordsDir , "%s/words" , settings.var_data_path);
  }


  //Creates a box thing, tells user to enter in name of list.  Click OK, or CANCEL
  //FIXME: Create a rect for user to enter stuff, and a pretty box to go around everything

  OK = BlackOutline(_("OK"), 25, &yellow);
  CANCEL = BlackOutline(_("CANCEL"), 25, &yellow);
  OK_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  CANCEL_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  Direction1 = BlackOutline(_("Create a New Wordlist"), 20, &yellow);
  Direction2 = BlackOutline(_("Type the name of your new wordlist and press 'ok' or 'RETURN' to save"), 12, &white);

  SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);

  Directions_rect.x = screen->w/2 - Direction1->w/2; 
  Directions_rect.y = screen->h/3;
  SDL_BlitSurface(Direction1, NULL, screen, &Directions_rect);

  Directions_rect.x = screen->w/2 - Direction2->w/2; 
  Directions_rect.y += 30;
  SDL_BlitSurface(Direction2, NULL, screen, &Directions_rect);

  OK_rect.x = screen->w/4; OK_rect.y = screen->h/3 * 2;
  SDL_BlitSurface(OK_button, NULL, screen, &OK_rect);
  OK_rect_text.x = screen->w/4 + (OK_button->w/2) - OK->w/2;
  OK_rect_text.y =  screen->h/3 * 2 + (OK -> h/2);
  SDL_BlitSurface(OK, NULL, screen, &OK_rect_text);

  CANCEL_rect.x = screen->w/4 * 2; CANCEL_rect.y = screen->h/3 * 2;
  SDL_BlitSurface(CANCEL_button, NULL, screen, &CANCEL_rect);
  CANCEL_rect_text.x = screen->w/4 * 2 + (CANCEL_button->w/2 - CANCEL->w/2);
  CANCEL_rect_text.y =  screen->h/3 * 2 + (CANCEL->h/2);
  SDL_BlitSurface(CANCEL, NULL, screen, &CANCEL_rect_text);

  Text.y = screen->h / 2;
  Text.w = Text.h =  0; 
  Text.x = screen->w /2;

  SDL_UpdateRect(screen, 0, 0, 0, 0);

  /*Main Loop*/
  while (!stop)
  {
    while (SDL_PollEvent(&event)) 
    {
      switch (event.type)
      {
        case SDL_QUIT:
          stop = 1;
          break;

        case SDL_MOUSEBUTTONDOWN: 
        {
          if (inRect(OK_rect, event.button.x, event.button.y)) 
          {
            if (len == 0)
            {
              LOG("Word list name needs non-zero length\n");
            }
            else
            {
              LOG("Save the wordlist\n");
              save = 1;
              stop = 1;	
            }
          }

          if (inRect(CANCEL_rect, event.button.x, event.button.y)) 
          {
            stop = 1;
            break;
          }
          break;
        }

        case SDL_KEYDOWN:
        {
          i = 1; //A Key has been pressed

          switch (event.key.keysym.sym)
          {
            case SDLK_BACKSPACE:
              len = ConvertFromUTF8(temp, wordlist, MAX_WORD_SIZE);
              if (len < 1)
              {
                LOG("There are no letters to delete\n");
              }
              else
              {
                temp[len - 1] = temp[len];
                len = ConvertToUTF8(temp, wordlist, MAX_WORD_SIZE);
                NewWordlist = BlackOutline(wordlist, DEFAULT_MENU_FONT_SIZE, &yellow);
                DEBUGCODE{ fprintf(stderr, "Word: %s\n", wordlist); }
              }
              i = 0;
              break;

            case SDLK_ESCAPE:
              stop = 1;
              i = 0;
              break;

            case SDLK_RETURN:  //does same thing as pressing OK
              if (len == 0)
              {
                LOG("Word list name needs non-zero length\n");
              }
              else
              {
                LOG("Save the wordlist\n");
                save = 1;
                stop = 1;	
              }

              i = 0;
              break;
            default:  // ignore any other keys 
              {}
          }

          //FIXME some of this block looks fishy...
          if (i) //if it is typing time
          {
            DEBUGCODE { fprintf(stderr, "TEMP 1: %s\n", wordlist); }

            len = ConvertFromUTF8(temp, wordlist, MAX_WORD_SIZE);
            if (len < MAX_WORD_SIZE)
            {
              // adds a character to the end of existing string
              temp[len] = toupper(event.key.keysym.unicode);
              temp[len + 1] = 0;
            }
            len = ConvertToUTF8(temp, wordlist, MAX_WORD_SIZE);

            DEBUGCODE { fprintf(stderr, "TEMP 2: %s\n", wordlist); }

            //Copy back into onscreen
            NewWordlist = BlackOutline(wordlist, DEFAULT_MENU_FONT_SIZE, &yellow);

            i = 0;
            break;
          } // end of if(i)
        }//end of Case SDL_KEYDOWN
      }//end of 'switch (event.type)'

      /* FIXME apparently redrawing every frame, even if not needed: */
      /*Redraw Screen*/
      if(!stop)
      {
        SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );

        Directions_rect.x = screen->w/2 - Direction1->w/2; 
        Directions_rect.y = screen->h/3;
        SDL_BlitSurface(Direction1, NULL, screen, &Directions_rect);
        Directions_rect.x = screen->w/2 - Direction2->w/2; 
        Directions_rect.y += 30;
        SDL_BlitSurface(Direction2, NULL, screen, &Directions_rect);

        OK_rect.x = screen->w/4;
        OK_rect.y = screen->h/3 * 2;
        SDL_BlitSurface(OK_button, NULL, screen, &OK_rect);
        OK_rect_text.x = screen->w/4 + (OK_button->w/2) - OK->w/2;
        OK_rect_text.y =  screen->h/3 * 2 + (OK -> h/2);
        SDL_BlitSurface(OK, NULL, screen, &OK_rect_text);

        CANCEL_rect.x = screen->w/4 * 2; CANCEL_rect.y = screen->h/3 * 2;
        SDL_BlitSurface(CANCEL_button, NULL, screen, &CANCEL_rect);
        CANCEL_rect_text.x = screen->w/4 * 2 + (CANCEL_button->w/2 - CANCEL->w/2);
        CANCEL_rect_text.y =  screen->h/3 * 2 + (CANCEL->h/2);
        SDL_BlitSurface(CANCEL, NULL, screen, &CANCEL_rect_text);

        Text.y = screen->h / 2;
        Text.w = Text.h =  0; 
        if (len > 0)
          Text.x = screen->w /2 - NewWordlist->w/2;
        else
          Text.x = screen->w /2;

        SDL_BlitSurface(NewWordlist, NULL, screen, &Text);

        SDL_UpdateRect(screen, 0, 0, 0, 0);
      }
    }  // End of 'while (SDL_PollEvent(&event))' loop
  } // End of 'while(!stop)' loop


  /* Creating file, if possible */
  if (save == 1)
  {
    sprintf(fn, "%s/%s.txt", wordsDir, wordlist);
    DEBUGCODE{ fprintf(stderr, "File to be saved: %s\n", fn); }

    fp = fopen(fn, "w");
    if(fp)
    {
      DEBUGCODE{ fprintf(stderr, "Opened File\n"); }
      fprintf(fp,"%s", wordlist);
      DEBUGCODE{ fprintf(stderr, "Wrote file\n"); }
      fclose(fp);	
      DEBUGCODE { fprintf(stderr, "Closed file\n"); }
    }
    else
    {
      fprintf(stderr, "Unable to create file: %s for writing\n", fn);
      save = 0;
    }
  }

  //we free stuff  
  if(OK_button)
    SDL_FreeSurface(OK_button);
  if(CANCEL_button)
    SDL_FreeSurface(CANCEL_button);
  if(OK)
    SDL_FreeSurface(OK);
  if(CANCEL)
    SDL_FreeSurface(CANCEL);
  if(NewWordlist)
    SDL_FreeSurface(NewWordlist);
  if(Direction1)
    SDL_FreeSurface(Direction1);
  if(Direction2)
    SDL_FreeSurface(Direction2);

//  OK = CANCEL = OK_button = CANCEL_button = NULL;
  return save;
}


int ChooseRemoveList(char *name, char *filename)
{
  LOG("Entering ChooseRemoveList()\n");

  int stop = 0;
  int result = 0;
  SDL_Surface* OK_button = NULL;
  SDL_Surface* CANCEL_button = NULL;
  SDL_Surface* OK = NULL, *CANCEL = NULL;
  SDL_Surface* Directions = NULL;
  SDL_Surface* wordname = NULL;
  SDL_Rect wordname_rect;
  SDL_Rect Directions_rect;
  SDL_Rect OK_rect; 
  SDL_Rect CANCEL_rect;
  SDL_Rect OK_rect_text; 
  SDL_Rect CANCEL_rect_text;

  OK = BlackOutline(_("OK"), 25, &yellow);
  CANCEL = BlackOutline(_("NEVERMIND"), 25, &yellow);

  OK_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  CANCEL_button = LoadImage("wordlist_button.png", IMG_ALPHA);

  Directions = BlackOutline(_("Do you want to delete this wordlist:"), 18, &white);
  wordname = BlackOutline(name, 18, &white);

  SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);

  OK_rect.x = screen->w/4; OK_rect.y = screen->h/3 * 2;
  SDL_BlitSurface(OK_button, NULL, screen, &OK_rect);
  OK_rect_text.x = screen->w/4 + (OK_button->w/2) - OK->w/2;
  OK_rect_text.y =  screen->h/3 * 2 + (OK -> h/2);
  SDL_BlitSurface(OK, NULL, screen, &OK_rect_text);

  CANCEL_rect.x = screen->w/4 * 2;
  CANCEL_rect.y = screen->h/3 * 2;
  SDL_BlitSurface(CANCEL_button, NULL, screen, &CANCEL_rect);
  CANCEL_rect_text.x = screen->w/4 * 2 + (CANCEL_button->w/2 - CANCEL->w/2);
  CANCEL_rect_text.y =  screen->h/3 * 2 + (CANCEL->h/2);
  SDL_BlitSurface(CANCEL, NULL, screen, &CANCEL_rect_text);

  Directions_rect.x = screen->w/2 - (Directions->w/2);
  Directions_rect.y = screen->h/3;
  SDL_BlitSurface(Directions, NULL, screen, &Directions_rect);

  wordname_rect.x = screen->w/2 - (wordname->w/2);
  wordname_rect.y = screen->h/3 + 30;
  SDL_BlitSurface(wordname, NULL, screen, &wordname_rect);

  SDL_UpdateRect(screen, 0, 0, 0, 0);

  while (!stop) 
  {
    while (SDL_PollEvent(&event)) 
    {
      /* FIXME should handle other events - Escape, SDL_WindowClose, etc. */
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN: 
          if (inRect(OK_rect, event.button.x, event.button.y)) 
          {
            RemoveList(filename);
            result = 1;	
            stop = 1;
          }
          if (inRect(CANCEL_rect, event.button.x, event.button.y))
          {
            result = 0;
            stop = 1;
          }
          break;
        default: {}
      }
    }
  }/*end user event handling **/

  //we free stuff
  SDL_FreeSurface(OK_button);
  SDL_FreeSurface(CANCEL_button);
  SDL_FreeSurface(OK);
  SDL_FreeSurface(CANCEL);
  SDL_FreeSurface(Directions);
  SDL_FreeSurface(wordname);
  OK = CANCEL = OK_button = CANCEL_button = NULL;

  return result;
}


int RemoveList(char* words_file)
{
  char fn[FNLEN];
  char wordsDir[FNLEN];
  LOG("Enter RemoveList()\n");
  // get appropriate directory
  sprintf(wordsDir, "%s/words", settings.user_settings_path);
  if (CheckFile(wordsDir))
  {
    DEBUGCODE { fprintf(stderr, "User specific wordlist path found: %s\n", wordsDir); }
  }
  else
  {
    DEBUGCODE { fprintf(stderr , "Editor: checking directory: %s/words", settings.var_data_path); }
    sprintf(wordsDir , "%s/words" , settings.var_data_path);
  }

  sprintf(fn , "%s/%s" , wordsDir, words_file);

  DEBUGCODE{ fprintf(stderr, "Remove file %s\n", fn); }

  if (remove(fn) != 0 )
  {
    fprintf(stderr, "Error deleting file: %s\n", fn);
    return 0; //no change
  }

  DEBUGCODE{ fprintf(stderr, "File %s successfully deleted\n", fn); }
  return 1; //change made
}
