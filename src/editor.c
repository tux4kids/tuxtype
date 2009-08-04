/*
*  C Implementation: editor.c
*
// Description: Implementation of word- and phrase list-editing functionality for
//  Tux Typing
//
// Author: Sarah Frisk <ssfrisk@gmail.com> and David Bruce <davidstuartbruce@gmail.com>, (C) 2009
//
// Written for Google Summer of Code 2009
//
// Copyright: See COPYING file that comes with this distribution (briefly, GNU GPLv2+)
*
*/


#include "globals.h"
#include "funcs.h"
#include "SDL_extras.h"
#include "SDL_image.h"
#include "convert_utf.h"
#include "editor.h"
#include <stdio.h> 

/* NOTE ChooseFile() is the function to pick custom word lists to edit. */
/* We should change the name to something more descriptive.             */

void ChooseListToEdit(void)
{
  SDL_Surface* new_button = NULL;
  SDL_Surface* remove_button = NULL;
  SDL_Surface* done_button = NULL;
  SDL_Surface *NEW = NULL, *REMOVE = NULL, *DONE = NULL;
  //this is text:
  SDL_Surface *s1 = NULL, *s2 = NULL, *s3 = NULL, *s4 = NULL;
  SDL_Rect locText;
  SDL_Rect button_rect[3];
  SDL_Rect button_text_rect[3]; 

  int stop = 0;
  int loc = 0;
  int old_loc = 1;
  int num_lists = 0;
  int i;
  int redraw = 0;
  int delete = 0;


  //Arrays for the list of editable word lists:
  char file_names[MAX_WORD_LISTS][FNLEN];  //names of the files containing lists
  char list_titles[MAX_WORD_LISTS][FNLEN]; //text list name to be displayed
  //Surfaces of list_titles[] rendered for graphical display:
  SDL_Surface* white_titles_surf[MAX_WORD_LISTS] = {NULL};  //unselected titles - white lettering; 
  SDL_Surface* yellow_titles_surf[MAX_WORD_LISTS] = {NULL}; //selected titles - yellow lettering
  // Rects where list names will be drawn on screen:
  static SDL_Rect titleRects[8];

  //Temporary holders and ptrs used while scanning list directory:
  char fn[FNLEN];                             
  FILE* fp = NULL;
  DIR* lists_dir = NULL;
  struct dirent* list_dirent = NULL;

  DEBUGCODE { fprintf(stderr , "%s", settings.var_data_path); }


  /* First part - scan through our word list directory and create lists */
  /* of the filenames and titles (first lines in files).                */
  /* NOTE we could use scandir() to do almost all of this as we do      */
  /* in tuxmath.                                                        */


  //Try to open directory for modifiable word lists:
  sprintf(fn , "%s" , settings.var_data_path);
  lists_dir = opendir(fn);

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

    snprintf(fn, FNLEN, "%s/%s" , settings.var_data_path, list_dirent->d_name); 

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
  s1 = BlackOutline(gettext_noop("Word List Editor"), 20, &yellow);
  s2 = BlackOutline(gettext_noop("To add a new wordlist, click the 'New Wordlist' button (it's not there)"), 11, &white);
  s3 = BlackOutline(gettext_noop("To edit current word lists, either click on the wordlist, or use the arrow keys to navigate and press RETURN"), 11, &white);
  s4 = BlackOutline(gettext_noop("To exit Word List Editor, press ESC"), 11, &white);	

  NEW = BlackOutline(gettext_noop("NEW"), 25, &yellow);
  DONE = BlackOutline(gettext_noop("DONE"), 25, &yellow);
  REMOVE = BlackOutline(gettext_noop("REMOVE"), 25, &yellow);

  /* Load image of new word list button: */
  new_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  remove_button = LoadImage("wordlist_button.png", IMG_ALPHA);
  done_button = LoadImage("wordlist_button.png", IMG_ALPHA);

  LOG( "ChooseFile() - drawing screen\n");


  /* Draw the initial text and images that won't change as list is examined: */
  SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);
  locText.x = screen->w/2 - (s1->w/2); locText.y = 10;
  SDL_BlitSurface( s1, NULL, screen, &locText);
  locText.x = screen->w/2 - (s2->w/2); locText.y = 50;
  SDL_BlitSurface( s2, NULL, screen, &locText);
  locText.x = screen->w/2 - (s3->w/2); locText.y = 70;
  SDL_BlitSurface(s3, NULL, screen, &locText);
  locText.x = screen->w/2 - (s4->w/2); locText.y = 90;
  SDL_BlitSurface( s4, NULL, screen, &locText);




  button_rect[New].x = screen->w - new_button->w - 20; 
  button_rect[New].y = screen->h/3;
  SDL_BlitSurface(new_button, NULL, screen, &button_rect[New]);
  button_text_rect[New].x = screen->w - new_button->w - 20 + (new_button->w/2 - NEW->w/2); 
  button_text_rect[New].y = screen->h/3 + (NEW->h/2);
  SDL_BlitSurface(NEW, NULL, screen, &button_text_rect[New]);

  button_rect[Remove].x = button_rect[New].x;
  button_rect[Remove].y = button_rect[New].y + remove_button->h + 10;
  SDL_BlitSurface(remove_button, NULL, screen, &button_rect[Remove]);
  button_text_rect[Remove].x = screen->w - remove_button->w - 20 + (remove_button->w/2 - REMOVE->w/2); 
  button_text_rect[Remove].y =  button_text_rect[New].y + remove_button->h + 10;
  SDL_BlitSurface(REMOVE, NULL, screen, &button_text_rect[Remove]);

  button_rect[Done].x = button_rect[Remove].x;
  button_rect[Done].y = button_rect[Remove].y + done_button->h + 10;
  SDL_BlitSurface(done_button, NULL, screen, &button_rect[Done]);
  button_text_rect[Done].x = screen->w - done_button->w - 20 + (done_button->w/2 - DONE->w/2); 
  button_text_rect[Done].y = button_text_rect[Remove].y + done_button->h + 10;
  SDL_BlitSurface(DONE, NULL, screen, &button_text_rect[Done]);


  SDL_UpdateRect(screen, 0, 0, 0, 0); 


  /* set initial rect sizes */
  titleRects[0].y = screen->h / 3;
  titleRects[0].w = titleRects[0].h =  0; 
  titleRects[0].x = screen->w / 10;

  for (i = 1; i < 8; i++)
  {
    titleRects[i].y = titleRects[i-1].y + 25;
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

        case SDL_MOUSEMOTION:
          for (i = 0; (i < 8) && (loc - (loc % 8) + i < num_lists); i++)
            if (inRect(titleRects[i], event.motion.x, event.motion.y))
            {
              loc = loc - (loc % 8) + i;
              break; /* out of for loop */
            }
          break;     /* out of switch-case */

        case SDL_MOUSEBUTTONDOWN: 
			if (inRect(button_rect[New], event.button.x, event.button.y)) 
			{
				CreateNewWordList();
				fprintf(stderr, "Check Four\n");
				redraw = 1;
				fprintf(stderr, "Check Five\n");
			}

			if (inRect(button_rect[Remove], event.button.x, event.button.y))
			{
				//pop up something?
				delete = ChooseRemoveList();
				redraw = 1;
			}
			
			if (inRect(button_rect[Done], event.button.x, event.button.y)) 
			{
				stop = 1; 
	            break;
			}

          for (i = 0; (i < 8) && (loc - (loc % 8) + i <num_lists); i++) 
            if (inRect(titleRects[i], event.button.x, event.button.y))
            {
				if (!delete)
				{
              		loc = loc - (loc % 8) + i;
              		EditWordList(file_names[loc]);
			  		redraw = 1;
				}
				else
				{
					//do this bit of code
					delete = RemoveList(file_names[loc]);
					redraw = 1;
				}
              break;
            }
			
          break;

        case SDL_KEYDOWN:
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



    /* Redraw if a different menu entry is selected: */
    if (old_loc != loc || redraw == 1)
    {
      int start;
	  

      if(CurrentBkgd())
      	SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);

		/*FIXME: simplify this into a function*/

		SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);
		locText.x = screen->w/2 - (s1->w/2); locText.y = 10;
		SDL_BlitSurface( s1, NULL, screen, &locText);
		locText.x = screen->w/2 - (s2->w/2); locText.y = 50;
		SDL_BlitSurface( s2, NULL, screen, &locText);
		locText.x = screen->w/2 - (s3->w/2); locText.y = 70;
		SDL_BlitSurface(s3, NULL, screen, &locText);
		locText.x = screen->w/2 - (s4->w/2); locText.y = 90;
		SDL_BlitSurface( s4, NULL, screen, &locText);

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
        SDL_UpdateRect(screen, titleRects[i%8].x, titleRects[i%8].y, titleRects[i%8].w, titleRects[i%8].h);
	  }
		  SDL_UpdateRect(screen, 0, 0, 0, 0); 
		 redraw = 0;
	
    }
    SDL_Delay(40);
    old_loc = loc;
  }

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


  if(s1)
    SDL_FreeSurface(s1);
  if(s2)
    SDL_FreeSurface(s2);
  if(s3)
    SDL_FreeSurface(s3);
  if(s4)
    SDL_FreeSurface(s4);
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
  static SDL_Rect leftRect, rightRect;
  SDL_Rect word_rects[8];
  int stop = 0;
  int loc = 0;
  int old_loc = 1;
  
  FILE* fp = NULL;

  int start, number_of_words = 0;
  int i,len; 
  int listening_for_new_word = 0;
  char fn[FNLEN];
  char str[FNLEN];
  char words_in_list[MAX_NUM_WORDS][MAX_WORD_SIZE + 1];
  
  wchar_t temp[FNLEN], temp2[FNLEN];
  

  //We should be able to use GenerateWordList() in place of this next block:
	//NOTE: Works originally, but upon returning to editorlist, the word selected
	// is not there, since all words in the wordlist are deleted
  sprintf(fn , "%s/%s", settings.var_data_path,  words_file);
 	fp = fopen(fn,"r");
 	number_of_words = 0;  
  	while(!feof(fp))
   if (EOF ==fscanf(fp, "%[^\n]\n", words_in_list[number_of_words++]))
      continue;
  	fclose(fp);                                       



  /* Prepare needed SDL_Surfaces: */

  /* Render the words in white and yellow: */
  for (i = 0; i < number_of_words; i++)
  {
    white_words[i] = BlackOutline(words_in_list[i], DEFAULT_MENU_FONT_SIZE, &white);
    yellow_words[i] = BlackOutline(words_in_list[i], DEFAULT_MENU_FONT_SIZE, &yellow);
  }

  left = LoadImage("left.png", IMG_ALPHA);
  right = LoadImage("right.png", IMG_ALPHA);

  // FIXME blitting doesn't really go here
  SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);	


  /* Set up SDL_Rect locations for later blitting: */
  leftRect.w = left->w;
  leftRect.h = left->h;
  leftRect.x = 320 - 100 - (leftRect.w/2);
  leftRect.y = 430;

  rightRect.w = right->w;
  rightRect.h = right->h;
  rightRect.x = 320 + 100 - (rightRect.w/2);
  rightRect.y = 430;

  word_rects[0].y = 30;
  word_rects[0].w = word_rects[0].h = word_rects[0].x = 0;

  for (i = 1; i < 8; i++)

  {
    word_rects[i].y = word_rects[i - 1].y + 50;
    word_rects[i].w = word_rects[i].h = word_rects[i].x = 0;
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
              loc = loc - (loc % 8) + i;
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
            len = ConvertFromUTF8(temp, words_in_list[loc], FNLEN); 
            if (len > 1 && number_of_words > 1 && loc != 0)
            {                               
              // remove the last character from the string
              temp[len - 1] = temp[len];
              len = ConvertToUTF8(temp, words_in_list[loc], FNLEN);
              white_words[loc] = BlackOutline(words_in_list[loc], DEFAULT_MENU_FONT_SIZE, &white );
              yellow_words[loc] = BlackOutline(words_in_list[loc], DEFAULT_MENU_FONT_SIZE, &yellow);  
            }
            else
            {
              // we have to remove the word from the list // 
              fprintf(stderr, "The number of words is %i\n", number_of_words);
              fprintf(stderr, "The length is %i \n", len);

              if (number_of_words > 1 && loc != 0)
              {	
                int x = 0;
                number_of_words--;
                fprintf(stderr, "There are current: %i words\n", number_of_words);

                for (x = loc; x <= number_of_words; x++)
                {
                  if (x < number_of_words)
                  {
                    len = ConvertFromUTF8(temp, words_in_list[x + 1], FNLEN);
                    len = ConvertToUTF8(temp, words_in_list[x], FNLEN);
                    white_words[x] = BlackOutline(words_in_list[x], DEFAULT_MENU_FONT_SIZE, &white ); 
                    yellow_words[x] = BlackOutline(words_in_list[x], DEFAULT_MENU_FONT_SIZE, &yellow);
                  }
                  else
                  {
                    white_words[x] = NULL;
                    yellow_words[x] = NULL;
                  }
                }

                if (loc == number_of_words)
                  loc --;
              }

              white_words[loc] = BlackOutline(words_in_list[loc], DEFAULT_MENU_FONT_SIZE, &white );                 
              yellow_words[loc] = BlackOutline(words_in_list[loc], DEFAULT_MENU_FONT_SIZE, &yellow);	

			//handle deleation of words better, right now don't really do that
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
            break;
          }

          if ((event.key.keysym.sym == SDLK_RIGHT)
           || (event.key.keysym.sym == SDLK_PAGEDOWN))
          {
            if (loc - (loc % 8) + 8 < number_of_words)
              loc = (loc - (loc % 8) + 8);
            break;
          }

          if (event.key.keysym.sym == SDLK_UP)
          {
            if (loc > 0)
              loc--;
            break;
          }

          if (event.key.keysym.sym == SDLK_DOWN)
          {
            if (loc + 1 < number_of_words)
              loc++;
            break;
          }
        
        
          switch (event.key.keysym.sym)
          {                               
            case SDLK_RETURN:
              if (number_of_words < MAX_WORD_LISTS)
                listening_for_new_word = 1;
              else
                fprintf(stderr, "Couldn't add new word, this wordlist is full.\n");	
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
            // If it's listening for a new word, from having last pressed enter, create a whole new word
            // with a length of 0, else get the current length of the highlighted word
            if (listening_for_new_word)
            {
              loc = number_of_words;
              number_of_words++;
              listening_for_new_word = 0;
              len = 0;
            }
            else
            {
              len = ConvertFromUTF8(temp, words_in_list[loc], FNLEN);
            }

            // Add the character to the end of the existing string
            temp[len] = toupper(event.key.keysym.unicode);
            temp[len + 1] = 0;
            ConvertToUTF8(temp,words_in_list[loc], FNLEN);

            // Copy back to the on-screen list
            white_words[loc] = BlackOutline(words_in_list[loc], DEFAULT_MENU_FONT_SIZE, &white );
            yellow_words[loc] = BlackOutline(words_in_list[loc], DEFAULT_MENU_FONT_SIZE, &yellow);

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
        for (i = start; i < MIN(start + 8, number_of_words); i++)
        {
          word_rects[i % 8].x = 320 - (white_words[i]->w/2);
          if (i == loc)
            SDL_BlitSurface(yellow_words[loc], NULL, screen, &word_rects[i % 8]);
          else
            SDL_BlitSurface(white_words[i], NULL, screen, &word_rects[i % 8]);
        }

        /* --- draw right and left arrow buttons --- */
        if (start > 0) 
          SDL_BlitSurface( left, NULL, screen, &leftRect );
        if (start + 8 < number_of_words) 
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
  fprintf(stderr, "In ChooseWord(), about to write changes\n");
  fp = fopen(fn,"w");

  if (fp)
  { 
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%s", str);
    i = 0;

    while(i < number_of_words)                     
      fprintf(fp, "%s\n", words_in_list[i++]);
  
    fclose(fp); 
    fp = NULL;
    fprintf(stderr, "In ChooseWord(), changes written successfully\n");
  }
  else
  {
    fprintf(stderr, "In ChooseWord(), unable to open %s for writing\n", fn);
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

  SDL_FreeSurface(left);
  SDL_FreeSurface(right);
  /* the pointers are going out of scope so we don't */
  /* have to worry about setting them to NULL              */
}              


/** Private functions **/
void CreateNewWordList(void)
{
	fprintf(stderr, "Creating a New Word List!!!");
	int stop = 0;
	SDL_Surface* OK_button = NULL;
	SDL_Surface* CANCEL_button = NULL;
	SDL_Surface *OK = NULL, *CANCEL = NULL;
	SDL_Surface *NewWordlist = NULL;
	SDL_Surface *Direction = NULL;
	SDL_Rect OK_rect; 
	SDL_Rect CANCEL_rect;
	SDL_Rect OK_rect_text; 
	SDL_Rect CANCEL_rect_text;
	SDL_Rect Text;
	SDL_Rect Directions_rect;
	
	char wordlist[MAX_WORD_SIZE + 1];
	wchar_t temp[FNLEN];

	int len = 0; //len = length, 
	int i = 0; //i = checks for keydown
	
	//Creates a box thing, tells user to enter in name of list.  Click OK, or CANCEL
	//FIXME: Text in boxes needs work
	//FIXME: Create a rect for user to enter stuff, and a pretty box to go around everything
	//FIDME: Change size of boxes
	
	OK = BlackOutline(gettext_noop("OK"), 25, &yellow);
	CANCEL = BlackOutline(gettext_noop("CANCEL"), 25, &yellow);
	
	OK_button = LoadImage("wordlist_button.png", IMG_ALPHA);
	CANCEL_button = LoadImage("wordlist_button.png", IMG_ALPHA);
	
	
	SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);
	
	OK_rect.x = screen->w/4; OK_rect.y = screen->h/3 * 2;
  	SDL_BlitSurface(OK_button, NULL, screen, &OK_rect);
	OK_rect_text.x = screen->w/4 + (OK_button->w/2) - OK->w/2; OK_rect_text.y =  screen->h/3 * 2 + (OK -> h/2);
	SDL_BlitSurface(OK, NULL, screen, &OK_rect_text);

	CANCEL_rect.x = screen->w/4 * 2; CANCEL_rect.y = screen->h/3 * 2;
  	SDL_BlitSurface(CANCEL_button, NULL, screen, &CANCEL_rect);
	CANCEL_rect_text.x = screen->w/4 * 2 + (CANCEL_button->w/2 - CANCEL->w/2); CANCEL_rect_text.y =  screen->h/3 * 2 + (CANCEL->h/2);
  	SDL_BlitSurface(CANCEL, NULL, screen, &CANCEL_rect_text);

  	Text.y = screen->h / 3;
  	Text.w = Text.h =  0; 
  	Text.x = screen->w /2;
	//SDL_BlitSurface(NewWordlist, NULL, screen, &Text);


  	SDL_UpdateRect(screen, 0, 0, 0, 0);

	/*Main Loop*/
  	while (!stop) 
  	{
    	while (SDL_PollEvent(&event)) 
    	{
			 switch (event.type)
			{
				case SDL_QUIT:
				{
					stop = 1;
					break;
				}
        		case SDL_MOUSEBUTTONDOWN: 
				{
					if (inRect(OK_rect, event.button.x, event.button.y)) 
					{
						//do something else
						// Check if anything is written
							//if yes, save
							//if not, don't do anything?
						stop = 1;
						break;
						
					}
					if (inRect(CANCEL_rect, event.button.x, event.button.y)) 
					{
						fprintf(stderr, "Check One\n");
						stop = 1;
						break;
					}
					break;
				}
				
				case SDL_KEYDOWN:
				{
					i = 1; //A Key has been pressed
					
					if (event.key.keysym.sym == SDLK_BACKSPACE)
					{
						len = ConvertFromUTF8(temp, wordlist, FNLEN);
						if (len > 0)
						{
							temp[len - 1] = temp[len];
							len = ConvertToUTF8(temp, wordlist, FNLEN);
							NewWordlist = BlackOutline(wordlist, DEFAULT_MENU_FONT_SIZE, &yellow);
							fprintf(stderr, "Word: %s\n", wordlist);
						}
						else
						{
							fprintf(stderr, "There are no letters to delete\n");
						}
						i = 0;
						break;
					} // end of SDLK_BACKSPACE
					
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						stop = 1;
						break;
					}
					
	
					switch (event.key.keysym.sym)
					{
						case SDLK_RETURN:
								//does same thing as pressing OK
								if (len != 0)
								{
									fprintf(stderr, "Save the wordlist\n");
									i = 0;
									stop = 1;
									break;
								}
								else
								{
									fprintf(stderr, "Word needs an actual length\n");
									i = 0;
									stop = 1;
									break;
								}
		                	i = 0;
		                break;
		              	default:  // ignore any other keys 
		                	{}
					}

		
					if (i) //if it is typing time
					{

						len = ConvertFromUTF8(temp, wordlist, FNLEN);
						
						// adds a character to the end of existing string
						temp[len] = toupper(event.key.keysym.unicode);
						temp[len + 1] = 0;
						ConvertToUTF8(temp, wordlist, FNLEN);
						
						//Copy back into onscreen
						fprintf(stderr, "Word: %s\n", wordlist);
						NewWordlist = BlackOutline(wordlist, DEFAULT_MENU_FONT_SIZE, &yellow);
					
						i = 0;
						break;
					} // end of if(i)			
				}//end of Case SDL_KEYDOWN
    		}//end of 'switch (event.type)'

			/*Redraw Screen*/
			if(!stop)
			{
				SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL );
				
				OK_rect.x = screen->w/4; OK_rect.y = screen->h/3 * 2;
				SDL_BlitSurface(OK_button, NULL, screen, &OK_rect);
				OK_rect_text.x = screen->w/4 + (OK_button->w/2) - OK->w/2; OK_rect_text.y =  screen->h/3 * 2 + (OK -> h/2);
				SDL_BlitSurface(OK, NULL, screen, &OK_rect_text);

				CANCEL_rect.x = screen->w/4 * 2; CANCEL_rect.y = screen->h/3 * 2;
				SDL_BlitSurface(CANCEL_button, NULL, screen, &CANCEL_rect);
				CANCEL_rect_text.x = screen->w/4 * 2 + (CANCEL_button->w/2 - CANCEL->w/2); CANCEL_rect_text.y =  screen->h/3 * 2 + (CANCEL->h/2);
				SDL_BlitSurface(CANCEL, NULL, screen, &CANCEL_rect_text);
				
				Text.y = screen->h / 3;
	  			Text.w = Text.h =  0; 
	  			Text.x = screen->w /2;
				SDL_BlitSurface(NewWordlist, NULL, screen, &Text);
				SDL_UpdateRect(screen, 0, 0, 0, 0);
			}
		
   		}  // End of 'while (SDL_PollEvent(&event))' loop
	} // End of 'while(!stop)' loop


		fprintf(stderr, "Check Two\n");

	
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
		if(Direction)
			SDL_FreeSurface(Direction);
		
		fprintf(stderr, "Check Three\n");
	//	OK = CANCEL = OK_button = CANCEL_button = NULL;
//return 0;
}

int ChooseRemoveList(void)
{
	fprintf(stderr, "Do you want to pick a list to delete?");
	int stop = 0;
	int result = 0;
	SDL_Surface* OK_button = NULL;
	SDL_Surface* CANCEL_button = NULL;
	SDL_Surface *OK = NULL, *CANCEL = NULL;
	SDL_Surface *Directions = NULL;
	SDL_Rect Directions_rect;
	SDL_Rect OK_rect; 
	SDL_Rect CANCEL_rect;
	SDL_Rect OK_rect_text; 
	SDL_Rect CANCEL_rect_text;

	OK = BlackOutline(gettext_noop("OK"), 25, &yellow);
	CANCEL = BlackOutline(gettext_noop("NEVERMIND"), 25, &yellow);

	OK_button = LoadImage("wordlist_button.png", IMG_ALPHA);
	CANCEL_button = LoadImage("wordlist_button.png", IMG_ALPHA);
	
	Directions = BlackOutline(gettext_noop("Select the wordlist you would like to delete"), 18, &white);


	SDL_BlitSurface(CurrentBkgd(), NULL, screen, NULL);

	OK_rect.x = screen->w/4; OK_rect.y = screen->h/3 * 2;
  	SDL_BlitSurface(OK_button, NULL, screen, &OK_rect);
	OK_rect_text.x = screen->w/4 + (OK_button->w/2) - OK->w/2; OK_rect_text.y =  screen->h/3 * 2 + (OK -> h/2);
	SDL_BlitSurface(OK, NULL, screen, &OK_rect_text);

	CANCEL_rect.x = screen->w/4 * 2; CANCEL_rect.y = screen->h/3 * 2;
  	SDL_BlitSurface(CANCEL_button, NULL, screen, &CANCEL_rect);
	CANCEL_rect_text.x = screen->w/4 * 2 + (CANCEL_button->w/2 - CANCEL->w/2); CANCEL_rect_text.y =  screen->h/3 * 2 + (CANCEL->h/2);
  	SDL_BlitSurface(CANCEL, NULL, screen, &CANCEL_rect_text);


	Directions_rect.x = screen->w/2 - (Directions->w/2);
	Directions_rect.y = screen->h/3;
	SDL_BlitSurface(Directions, NULL, screen, &Directions_rect);
	
  	SDL_UpdateRect(screen, 0, 0, 0, 0);


  	while (!stop) 
  	{
    	while (SDL_PollEvent(&event)) 
    	{
			 switch (event.type)
			{
        		case SDL_MOUSEBUTTONDOWN: 
					if (inRect(OK_rect, event.button.x, event.button.y)) 
					{
						//do something else
						// Check if anything is written
							//if yes, save
							//if not, don't do anything?
						result = 1;	
						stop = 1;
					}
					if (inRect(CANCEL_rect, event.button.x, event.button.y)) 
					{	
						result = 0;
						stop = 1;
					}
       				break;
			}
    	}
   }/*end user event handling **/

	if(stop == 1)
	{

		//we free stuff
		SDL_FreeSurface(OK_button);
		SDL_FreeSurface(CANCEL_button);
		SDL_FreeSurface(OK);
		SDL_FreeSurface(CANCEL);
		OK = CANCEL = OK_button = CANCEL_button = NULL;
	}
	
	return result;
	
}


/* This is currently not deleting the word, attempting to figure out why 'remove' isn't working*/
int RemoveList(char* words_file)
{
	char fn[FNLEN];
	fprintf(stderr, "Deleting a file\n");
	
	sprintf(fn , "%s/%s" , settings.var_data_path, words_file);

	fprintf(stderr, "Remove file %s\n", fn);
	
	
	if (remove(fn) != 0 )
	    fprintf(stderr, "Error deleting file\n");
	 else
	    fprintf(stderr, "File successfully deleted\n");
	
 	return 0;
	
}


 