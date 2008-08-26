#include "globals.h"
#include "funcs.h"
#include "profilesgui.c"

typedef struct profiles{ //structure for profile in use
    char* profileName;
    int profileNum;
    int highscore1;
    int highscore2;
    int highscore3;
    int highscore4;
    int highscore5;
    int highscore6;
    int highscore7;
    int highscore8;
    int wpm_fastest;
    int wpm_average;
    int wpm_slowest;
    int wpm_taken;
  }profiles;

  extern profiles activeProfile;

int load_profiles_filename(const char*, int);
int load_profiles_fp(FILE*, int);

void loadprofileName(char *name[][])
{
	char fn[FNLEN];
	char *temp;
	int count = 0;
	int offset;
	int search;
	#ifdef WIN32
		snprintf(fn, FNLEN - 1, "userdata/profiles.txt");
	#else
		snprintf(fn, FNLEN - 1, (const char*)"%s/.tuxtype/profiles.txt", getenv("HOME"));
	#endif
	  	
	FILE* fp = fopen(fn, "r");
	  	
	 while(!feof(fp) && count < settings.numProfiles)
	 {
		 fgets(temp, 200, fp);
		 if(strstr(temp, "profileNum"))
		 {
			 search = strlen(temp) - strnspn(temp, "=");
			 fseek(fp, search *-1, SEEK_CUR);
			 fgets(names[count][0], search-1, fp);
			 /*skip the newline */
			 feek(fp, 2, SEEK_CUR);
		 }
		 if(strstr(temp, "profileName"))
		 {
			 search = strlen(temp) - strspn(temp, "=");
			 fseek(fp, search * -1, SEEK_CUR);
			 fgets(names[count][1], search-1, fp);
			 /* skip to next profile, after newline */
			 fseek(fp, 2, SEEK_CUR);
			 for(offset = 0; offset < 12; offset++)
			 {
				 fgets(temp, 200, fp);
			 }
			 count++;
		 }
	 }
	 fclose(fp);
}
void loadProfile(int profilenum)
{
	char fn[FNLEN];

	#ifdef WIN32
		snprintf(fn, FNLEN - 1, "userdata/profiles.txt");
	#else
		snprintf(fn, FNLEN - 1, (const char*)"%s/.tuxtype/profiles.txt", getenv("HOME"));
	#endif
  	
  	load_profiles_filename(fn, profilenum);
}

static int load_profiles_filename(const char* fn, int profilenum)
{
  FILE* fp = fopen(fn, "r");

  if (!fp)
  {
    fprintf(stderr, "load_profiles_filename(): Incorrect pointer\n");
    return;
  }

  if (!load_profiles_fp(fp, profilenum))
  {
    fprintf(stderr, "Could not find any profiles.\n");
    fclose(fp);
    return 0;
  }

  fclose(fp);
  return 1;
}

static int load_profiles_fp(FILE* fp, int profilenum)
{
  char profiles[FNLEN];
  char value[FNLEN];
  int profile_found = 0;

  if (!fp)
  {
    fprintf(stderr, "load_profiles_fp() - FILE* parameter NULL\n");
    return 0;
  }

  /* we load the selected profile, based on number*/
  while (!feof(fp))
  {
    fscanf(fp, "%[^=]=%[^\n]\n", profiles, value );

    if (strncmp( profiles, "profileNum" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.profileNum = atoi(value);
    	profile_found = 1;
    }
    else if (strncmp( profiles, "profileName" (*char)profilenum, FNLEN ) == 0 )
    {
    	strncpy(activeProfile.profileName, value, FNLEN - 1);
    	profiles_found = 1;
   }
    else if (strncmp( profiles, "highscore1_" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.highscore1 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp(profiles, "highscore2_" (*char)profilenum, FNLEN) == 0)
    {
    	activeProfile.highscore2 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp(profiles, "highscore3_" (*char)profilenum, FNLEN) == 0)
    {
    	activeProfile.highscore3 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "highscore4_" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.highscore4 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "highscore5_" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.highscore5 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "highscore6_" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.highscore6 = atoi(value);
    	profiles_found = 1;
	}
    else if (strncmp( profiles, "highscore7_" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.highscore7 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "highscore8_" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.highscore8 = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "wpmf" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.wpm_fastest = atoi(value);
        profiles_found = 1;
    }
    else if (strncmp( profiles, "wpma" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.wpm_average = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "wpms" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.wpm_slowest = atoi(value);
    	profiles_found = 1;
    }
    else if (strncmp( profiles, "wpmt" (*char)profilenum, FNLEN ) == 0 )
    {
    	activeProfile.wpm_taken = atoi(value);
    	profiles_found = 1;
    }   
  }

  if (profiles_found)
    return 1;
  else
  {
    fprintf(stderr, "load_profiles_fp() - no profiles in file - empty or corrupt?\n");
    return 0;
  }
}

void saveProfile(int numprofiles) //called during "Create" on gui and before exit
{
	char fn[FNLEN];
	FILE* profilesFile;
	FILE* tempFile;
	
	activeProfile.profileNum = numprofiles;

	/* set the profiles directory/file */

	#ifdef WIN32
		_mkdir( "userdata" );
		snprintf( fn, FNLEN-1, "userdata/profiles.txt" );
	#else
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype", getenv("HOME") );
		mkdir( fn, 0755 );
		snprintf( fn, FNLEN-1, (const char*)"%s/.tuxtype/profiles.txt", getenv("HOME") );
	#endif

		profilesFile = fopen(fn, "r+" );

		if (profilesFile == NULL)
		{
			printf("Save Profiles: Profiles file cannot be created!\n");
		}
		/* make sure it's valid*/
		else if(settings.numProfiles <= activeProfile.profileNum)
		{
			//it's the last one/new one, just append it
			if(settings.numProfiles == activeProfile.profileNum)
			{
				profilenum8=
				fseek(profilesFile, SEEK_END, SEEK_SET) //append
				fprintf(profilesFile, "profilenum%d=%d\n", 	activeProfile.profileNum, 	activeProfile.profileNum);
				fprintf(profilesFile, "profilename%d=%s\n",	activeProfile.profileNum, 	activeProfile.profileName);
				fprintf(profilesFile, "highscore1_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore1);
				fprintf(profilesFile, "highscore2_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore2);
				fprintf(profilesFile, "highscore3_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore3);
				fprintf(profilesFile, "highscore4_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore4);
				fprintf(profilesFile, "highscore5_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore5);
				fprintf(profilesFile, "highscore6_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore6);
				fprintf(profilesFile, "highscore7_%d=%d\n",	activeProfile.profileNum,	activeProfile.highscore7);
				fprintf(profilesFile, "highscore8_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore8);
				fprintf(profilesFile, "wpmf%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_fastest);
				fprintf(profilesFile, "wpma%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_average);
				fprintf(profilesFile, "wpms%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_slowest);
				fprintf(profilesFile, "wpmt%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_taken);
				
				fclose(profilesFile);
			}
			/* make a tempfile to store data up to that profile
			 * write updated profile, add in data from original file
			 * because data has to be inserted, and best way to handle
			 * it right now is to create a temp */
			else
			{
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
				 * profile */
				while(!feof(profilesFile))
				{
					/* shouldn't be anymore than 200 chars in a line */
					char *temp;
					fgets(temp, 200, profilesFile);
					if(strcmp(temp, "profilenum" numprofiles"="numprofiles"\n") == 0)
					{
						int c;
						//skip 13 data lines of the same profile
						for(c = 0; c < 13; c++)
						{
							fgets(temp, 200, profilesFile);
						}
						fprintf(tempFile, "profilenum%d=%d\n", 	activeProfile.profileNum, 	activeProfile.profileNum);
						fprintf(tempFile, "profilename%d=%s\n",	activeProfile.profileNum, 	activeProfile.profileName);
						fprintf(tempFile, "highscore1_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore1);
						fprintf(tempFile, "highscore2_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore2);
						fprintf(tempFile, "highscore3_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore3);
						fprintf(tempFile, "highscore4_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore4);
						fprintf(tempFile, "highscore5_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore5);
						fprintf(tempFile, "highscore6_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore6);
						fprintf(tempFile, "highscore7_%d=%d\n",	activeProfile.profileNum,	activeProfile.highscore7);
						fprintf(tempFile, "highscore8_%d=%d\n", activeProfile.profileNum,	activeProfile.highscore8);
						fprintf(tempFile, "wpmf%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_fastest);
						fprintf(tempFile, "wpma%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_average);
						fprintf(tempFile, "wpms%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_slowest);
						fprintf(tempFile, "wpmt%d=%d\n", 		activeProfile.profileNum,	activeProfile.wpm_taken);
					}
					else
					{
						fprintf(tempFile, temp);
					}
				}
				fclose(tempFile);
				fclose(profilesFile);
				//overwrite original file with the tempfile
				remove("userdata/profiles.txt");
				rename("userdata/tempfile.txt", "userdata/profiles.txt");
			}
		}
		else
		{
			 fprintf(stderr, "Invalid profile number.\n");
			 fclose(profilesFile);
		}
}

void updatePersonalScore(int score)
{
	int temp, count = 1;
	int checkScore = score;
	do
	{
		if(checkScore > activeProfile.highscore(count))
		{
			temp = activeProfile.highscore(count);
			activeProfile.highscore(count) = score;
			checkScore = temp;
		}
		count++;
	}
	while((checkScore < activeProfile.highscore(count)) && count <=8);
}
void updateWPM(int keystrokes)
{
#define time 30 //in seconds how long test lasted
	
	int numwords = keystrokes/5;
	int wpm = numwords*2;
	int tempaverage;
	
	if(wpm > activeProfile.wpm_fastest)
	{
		activeProfile.wpm_fastest = wpm;
	}
	
	if(wpm < activeProfile.wpm_slowest)
	{
		activeProfile.wpm_slowest = wpm;
	}
	
	tempaverage = activeProfile.wpm_taken*activeProfile.wpm_average;
	activeProfile.wpm_average = (tempaverage + wpm)/activeProfile.wpm_taken++;
}