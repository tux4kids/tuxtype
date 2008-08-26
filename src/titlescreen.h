
/* --- SETUP MENU OPTIONS --- */

#define TITLE_MENU_ITEMS                5
#define TITLE_MENU_DEPTH                4

#define OPTIONS_SUBMENU                 4
#define LASER_SUBMENU	        	3
#define CASCADE_SUBMENU	        	2
#define ROOTMENU		        1

/* --- define menu structure --- */
/* (these values are all in the Game_Type enum in globals.h) */
const int menu_item[][6]= {{0, 0,         0,         0,          0},
			   {0, CASCADE,   LEVEL1,    LEVEL1,  EDIT_WORDLIST },
			   {0, LASER,     LEVEL2,    LEVEL2,  FREETYPE   },
			   {0, LESSONS,  LEVEL3,    LEVEL3,  PROJECT_INFO },
			   {0, OPTIONS,   INSTRUCT,  LEVEL4,  SET_LANGUAGE},
			   {0, QUIT_GAME, MAIN,      MAIN,    MAIN}};


/* --- menu icons --- */
const unsigned char *menu_icon[][6]= 
{{"", "", "", "", ""},
 {"", "cascade", "easy",   "grade1_", "list"   },
 {"", "comet",   "medium", "grade2_", "practice" },
 {"", "lesson","hard",   "grade3_", "keyboard"   },
 {"", "tux_config",  "tutor",  "grade4_", "lang" },
 {"", "quit",    "main",   "main",    "main"   }};

/* --- timings for tux blinking --- */
#define TUX1                            115
#define TUX2                            118
#define TUX3                            121
#define TUX4                            124
#define TUX5                            127
#define TUX6                            130
