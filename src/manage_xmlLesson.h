/***************************************************************
 *  manage_xmlLesson.h                                          *
 *                                                              *
 *  Description:  Headers for managing XML files generated      *                                       *
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



#define MAX_MENU_ITEMS 21
#define MENU_ITEM_LENGTH 15

//char * menu_names[MAX_MENU_ITEMS];
extern char menu_names[MAX_MENU_ITEMS][MENU_ITEM_LENGTH];
extern int total_no_menus;  // total no of menus


//main function for parsing and writing
int manage_xmlLesson(char *);


//Both the structs below are similar
//I could have used a single struct but since the two games
//laser and cascade are different so decided with two structs
//for settings from lesson file
struct settings_laser_game
{
char filepath[256];  
int level; //easy medium hard
//int num_of_lives;
}; 

struct settings_laser_game input_laser;

//for settings from lesson file
struct settings_cascade_game
{
char filepath[256];  
int level;  //easy medium hard
int num_of_lives;
int fish_per_level;
}; 

struct settings_cascade_game input_cascade;


//for settings from lesson file
struct settings_phrases_game
{ 
char phrases_path[256];
}; 

struct settings_phrases_game input_phrases;




//for settings from lesson file
struct settings_typing_lesson
{ 
char filepath[256];
}; 

struct settings_typing_lesson input_typing_lesson;




//for result 

struct result_fish_cascade
{
int fish_eaten;  
int level_reached; 
}; 

struct result_fish_cascade result_cascade;

struct result_laser_game
{
int wave;  
int score; 
}; 

struct result_laser_game result_laser;

//used to store whether game was completed or not 
int *game_completed,current_game_index;

