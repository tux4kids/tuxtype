#include "globals.h"
#include "funcs.h"
#include "profiles.c"

void createProfile(void);
void selectProfile(void);
void deleteProfile(void);
int chooseProfiles(void);
void nameEntry(char*, const char*, const char*);
int inRect(SDL_Rect, int, int);


void profilesSelector() //gui called upon clicking on the profile icon and at the start of titlescreen
{
	char *input;
	if(!chooseProfiles() && (settings.numProfiles < 8))
	{
		createProfile();
	}
	else
	{
		/* TODO greyout createprofile/disable SDL handling
		 * since there max profiles, if wanted.
		 * Nothing needs to happen since there are too many
		 * profiles\not the first time means display() worked*/
		;
	}

}

void createProfile()
{
	if(settings.numProfiles < 8)
	{
		nameEntry(activeProfile.profileName, "Create a Profile", "Enter a Name");
		saveProfile(++settings.numProfiles);/*increments number of profiles in existence */
		loadprofile(settings.numProfiles);
	}
	else
	{
		/* do nothing since too many*/
	}
	
}
void selectProfile(int location) //sets profile
{
	if (settings.numProfiles > 1 )
	{
		saveProfile(activeProfile.numProfile);
		loadprofile(location)
	}
	else
	{
		loadprofile(location)
	}


	//erase window
}

void deleteProfile(int numprofile) //erases selected profile along with all data associated
{
	char fn[FNLEN];
	FILE* profilesFile;
	FILE* tempFile;
	int found = 0;
	int count;
	
	/* set the profiles directory/file */

	#ifdef WIN32
		_mkdir( "userdata" );
		snprintf( fn, FNLEN-1, "userdata/profiles.txt" );
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
		mkdir( fn, 0755 );
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/profiles.txt", getenv("HOME") );
	#endif

	profilesFile = fopen(fn, "r" );

	if (profilesFile == NULL)
	{
		printf("Save Profiles: Profiles file cannot be created!\n");
	}
	
	#ifdef WIN32
		_mkdir( "userdata" );
		snprintf( fn, FNLEN-1, "userdata/tempfile.txt" );
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
		mkdir( fn, 0755 );
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/tempfile.txt", getenv("HOME") );
	#endif
					
	fopen(tempFile, "w");
				
	/* read all lines of data except those of the inserting
		 profile */
	while(!feof(profilesFile))
	{
		int lines;
		/* shouldn't be anymore than 200 chars in a line */
		char *temp;
		fgets(temp, 200, profilesFile);
		if(strcmp(temp, "profilenum" (*char)numprofile"="(*char)numprofile) == 0)
		{
			//skip the deleted profile
			for(int c = 0; c < 13; c++)
			{
				fgets(temp, 200, profilesFile);
			}
			found = 1;
		}
		else if(found) //need to decrement profile values afterward
		{
			char* i;
			char* value;
			int stringoffset;
			int offset = numprofile + count;
			//handle the different lines
			for(lines = 0; lines < 14; lines++)
			{
				switch (lines)
				{
					case 0:
					{
						fprint(tempFile, "profileNum%d=%d\n",offset, offset))
						break;
					}
					case 1:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "profileName%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 2:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore1_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 3:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore2_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 4:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore3_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 5:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore4_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;					
					}
					case 6:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore5_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;					
					}
					case 7:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore6_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;					
					}
					case 8:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore7_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;					
					}
					case 9:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore8_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;					
					}
					case 10:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "wpmf%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 11:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "wpma%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 12:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "wpms_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 13:
					{		
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "wpmt_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
					case 14:
					{
						stringoffset = strlen(temp) - strcspn(temp, "=");
						fprint(tempFile, "highscore1_%d=", offset);
						fseek(profilesFile, stringoffset * -1 , SEEK_CUR);
						fgets(temp, 200, profilesFile);
						fprint(tempFile,"%s", temp);
						break;
					}
				}	
			}
			count++;
		}
		else //no need to do special work with the line
		{
			fprint(tempFile, temp);
		}
	}
	settings.numProfiles--;
	fclose(tempFile);
	fclose(profilesFile);
	//overwrite original file with the tempfile
	remove("userdata/profiles.txt");
	rename("userdata/tempfile.txt", "userdata/profiles.txt");
}
int chooseProfiles()
{	
	char *name[settings.numProfile][2];
	SDL_Surface* titles[8] = {NULL};
	SDL_Surface* select[8] = {NULL};
	SDL_Surface* create = NULL, *select = NULL, *delete = NULL;
	SDL_Surface* backg = NULL;
	SDL_Rect createRect, selectRect, deleteRect;
	SDL_Rect titleRects[8];
	int stop = 0;
	int loc = 0;
	int old_loc = 1;
	int i;

	/* Render SDL_Surfaces for list entries: */
	for (i = 0; i < settings.numProfiles; i++)
	{
		titles[i] = BlackOutline( name[i][1], font, &white );
	    select[i] = BlackOutline( names[i][1], font, &yellow);
	}

	backg = LoadImage("main_bkg.png", IMG_REGULAR);
	delete = LoadImage("delete.png", IMG_ALPHA);
	create = LoadImage("create.png", IMG_ALPHA);
	select = LoadImage("select.png", IMG_ALPHA);

	/* Get out if needed surface not loaded successfully: */
	if (!backg || !delete || !create || !select)
	{
		fprintf(stderr, "chooseProfile(): needed image not available\n");

	    for (i = 0; i < settings.numProfiles; i++)
	    {
	    	SDL_FreeSurface(titles[i]);
	    	SDL_FreeSurface(select[i]);
	    	titles[i] = select[i] = NULL;
	    }

	    SDL_FreeSurface(backg);
	    SDL_FreeSurface(left);
	    SDL_FreeSurface(create)
	    SDL_FreeSurface(select);
	    backg = left = right = NULL;
	    return 0;
	}

	deleteRect.w = delete->w; deleteRect.h = delete->h;
	deleteRect.x = 80 - (deleteRect.w/2); deleteRect.y = 430;

	createRect.w = create->w; createRect.h = delete->h;
	createRect.x = 320 - (createRec.w/2); createRec.y = 430;
	  
	selectRect.w = select->w; selectRect.h = select->h;
	selectRect.x = 560 - (rightRect.w/2); selectRect.y = 430;

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
	        	{
	        		exit(0); /* FIXME may need to cleanup memory and exit more cleanly */
	        		break;
	        	}

	        	case SDL_MOUSEMOTION:
	        	{
	        		for (i=0; (i<8) && (loc-(loc%8)+i<settings.numProfiles); i++)
	        		{
	        			if (inRect( titleRects[i], event.motion.x, event.motion.y ))
	        			{
	        				loc = loc-(loc%8)+i;
	        				break;
	        			}
	        		}
	        		break;
	        	}

	        	case SDL_MOUSEBUTTONDOWN:
	        	{
	        		if (inRect( deleteRect, event.button.x, event.button.y ))
	        		{
	        				deleteprofile(loc);
	        				stop = 3;
	        		}
	        		if (inRect( createRect, event.button.x, event.button.y ))
	        		{
	        			createProfile();
	        			break;
	        		}
	        		if (inRect(selectRect, event.button.x, event.button.y))
	        		{
	        			selectProfile(loc);
	        			stop = 1;
	        		}
	        			break;
	        		}
	        	}
	        	case SDL_KEYDOWN:
	        		if (event.key.keysym.sym == SDLK_ESCAPE) 
	        		{ 
	        			stop = 1; 
	        			break; 
	        		}
	        		if (event.key.keysym.sym == SDLK_RETURN) 
	        		{
	        			selectProfile(loc);
	        			break;
	        		}

	        		if ((event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_PAGEUP)) 
	        		{
	        			if (loc-(loc%8)-8 >= 0)
	        			{
	        				loc=loc-(loc%8)-8;
	        			}
	        		}

	        		if ((event.key.keysym.sym == SDLK_RIGHT) || (event.key.keysym.sym == SDLK_PAGEDOWN)) 
	        		{
	        			if (loc-(loc%8)+8 < settings.numProfiles)
	        			{
	        				loc=(loc-(loc%8)+8);
	        			}
	        		}

	        		if (event.key.keysym.sym == SDLK_UP) 
	        		{
	        			if (loc > 0)
	        			{
	        				loc--;
	        			}
	        		}

	        		if (event.key.keysym.sym == SDLK_DOWN) 
	        		{
	        			if (loc+1<settings.numProfiles)
	        			{
	        				loc++;
	        			}                    
	        		}
	  }
	}

	if (old_loc != loc) 
	{
		int start;

		SDL_BlitSurface(backg, NULL, screen, NULL );

		start = loc - (loc % 8);
		for (i = start; i<MIN(start+8,settings.numProfiles); i++) 
		{
			titleRects[i%8].x = 320 - (titles[i]->w/2);
			if (i == loc)
			{
				SDL_BlitSurface(select[loc], NULL, screen, &titleRects[i%8]);
			}
			else
			{
				SDL_BlitSurface(titles[i], NULL, screen, &titleRects[i%8]);
			}               
		}

		SDL_UpdateRect(screen, 0, 0, 0 ,0);
	}
	SDL_Delay(40);
	old_loc = loc;

	  /* --- clear graphics before leaving function --- */
	  for (i = 0; i < settings.numProfiles; i++)
	  {
	    SDL_FreeSurface(titles[i]);
	    SDL_FreeSurface(select[i]);
	    titles[i] = select[i] = NULL;
	  }

	  SDL_FreeSurface(backg);
	  SDL_FreeSurface(delete);
	  SDL_FreeSurface(create)
	  SDL_FreeSurface(select);
	  backg = left = right = NULL; /* Maybe overkill - about to be destroyed anyway */

	  if (stop == 2)
	  {
	    return 0;
	  }
	  else if (stop == 3)
	  {
		  chooseProfiles();
	  }

	  return 1;
}

void nameEntry(char* name, const char* heading, const char* sub)
{
	/* should be long enough */
	  char UTF8_buf[96] = {'\0'};
	
	  SDL_Rect loc;
	  SDL_Rect redraw_rect;
	
	  int redraw = 0;
	  int first_draw = 1;
	  int finished = 0;
	  int tux_frame = 0;
	  Uint32 frames = 0;
	  Uint32 start = 0;
	  wchar_t wchar_buf[33] = {'\0'};
	  TTF_Font* name_font = NULL;
	  const int NAME_FONT_SIZE = 18;
	  const int BG_Y = 100;
	  const int BG_WIDTH = 400;
	  const int BG_HEIGHT = 200;

	  if (!name)
	  {
		  return;
	  }
	    
	  name_font = LoadFont(DEFAULT_FONT_NAME, NAME_FONT_SIZE);
	  if (!name_font)
	  {
		  return;
	  }

	  SDL_EnableUNICODE(SDL_ENABLE);

  /* Draw translucent background for text: */
	  
	 SDL_Rect bg_rect;
	 bg_rect.x = (screen->w)/2 - BG_WIDTH/2;
	 bg_rect.y = BG_Y;
	 bg_rect.w = BG_WIDTH;
	 bg_rect.h = BG_HEIGHT;
	 DrawButton(&bg_rect, REG_RGBA);
	
	 bg_rect.x += 10;
	 bg_rect.y += 10;
	 bg_rect.w -= 20;
	 bg_rect.h = 60;
	 DrawButton(&bg_rect, SEL_RGBA);
	  

  /* Draw heading: */
  
    SDL_Surface* s = outlineBox( heading, default_font, &white);
    
    if(s)
    {
    	loc.x = (screen->w/2) - (s->w/2);
    	loc.y = 110;
    	SDL_BlitSurface(s, NULL, screen, &loc);
    	SDL_FreeSurface(s);
    }

    s = BlackOutline( sub, default_font, &white);
    if(s)
    {
    	loc.x = (screen->w/2) - (s->w/2);
    	loc.y = 140;
    	SDL_BlitSurface(s, NULL, screen, &loc);
    	SDL_FreeSurface(s);
    }

    /*update*/
    SDL_UpdateRect(screen, 0, 0, 0, 0);

    /*handle events*/
    while (!finished)
    {
    	start = SDL_GetTicks();

    	while (SDL_PollEvent(&event)) 
    	{
    		switch (event.type)
    		{
    			case SDL_QUIT:
    			{
    				cleanup();
    			}

    			case SDL_KEYDOWN:
    			{
    				switch (event.key.keysym.sym)
    				{
    					case SDLK_ESCAPE:
    					case SDLK_RETURN:
    					case SDLK_KP_ENTER:
    					{
    						finished = 1;
    						playsound(SND_TOCK);
    						break;
    					}
    					case SDLK_BACKSPACE:
    					{
    						if (wcslen(wchar_buf) > 0)
    						{
    							wchar_buf[(int)wcslen(wchar_buf) - 1] = '\0';
    						}
    						redraw = 1;
    						break;
    					}

    					/* if key has unicode value, add it */
    					default:
    					{
    						if ((event.key.keysym.unicode > 0)
    								&& (wcslen(wchar_buf) < 32)) 
    						{
    							wchar_buf[(int)wcslen(wchar_buf)] = event.key.keysym.unicode;
    							redraw = 1;
    						} 
    					}
    				}

    				/* Now draw name, if needed: */
    				if (redraw)
    				{
    					SDL_Surface* s = NULL;
    					redraw = 0;

    					/* Convert text to UTF-8 so BlackOutline() can handle it: */
    					wcstombs((char*) UTF8_buf, wchar_buf, 96);

    					/* Redraw background and shading in area where we drew text last time: */ 
    					if (!first_draw)
    					{
    						SDL_BlitSurface(screen, &redraw_rect, screen, &redraw_rect);
    						DrawButton(&redraw_rect, REG_RGBA);
    						SDL_UpdateRect(screen,
    								redraw_rect.x,
    								redraw_rect.y,
    								redraw_rect.w,
    								redraw_rect.h);
    					}

    					s = BlackOutline(UTF8_buf, name_font, &yellow);
            
    					if (s)
    					{
    						/* set up loc and blit: */
    						loc.x = (screen->w/2) - (s->w/2);
    						loc.y = 200;
    						SDL_BlitSurface(s, NULL, screen, &loc);

    						/* Remember where we drew so we can update background next time through:  */
    						/* (for some reason we need to update a wider area to get clean image)    */
    						redraw_rect.x = loc.x - 20;
    						redraw_rect.y = loc.y - 10;
    						redraw_rect.h = s->h + 20;
    						redraw_rect.w = s->w + 40;
    						first_draw = 0;

    						SDL_UpdateRect(screen,
    								redraw_rect.x,
    								redraw_rect.y,
    								redraw_rect.w,
    								redraw_rect.h);
    						SDL_FreeSurface(s);
    						s = NULL;
    					}
    				}
    			}
    		}
    	}

    	/*stop frame rate from skipping */
    	while ((SDL_GetTicks() - start) < 33)
    	{
    		SDL_Delay(20);
    	}
    	frames++;
  }
  TTF_CloseFont(name_font);

  /* disable due to overhead */
  SDL_EnableUNICODE(SDL_DISABLE);

  /* Now copy name into location pointed to by arg: */ 
  strncpy((char*)name, (char*)UTF8_buf, 96);
}