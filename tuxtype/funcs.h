/***************************************************************************
                          funcs.h
  description: function header
                             -------------------
    begin                : Sat May 6 2000
    copyright            : (C) 2000 by Sam Hart
    email                : hart@geekcomix.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



extern int max( int n1, int n2 );


extern void WaitFrame( void );
extern void FreeGraphics( void );
extern void InstructCascade( void );
extern void InstructLaser( void );
extern int PlayCascade( int level );
extern int laser_game( int DIF_LEVEL );
extern void AddRect(SDL_Rect * src, SDL_Rect * dst);

/* in scripting.c */
extern void testLesson( void );
extern void projectInfo( void );
extern void closeScript( void );
extern int loadScript( const char *fn );

/* in audio.c */
extern void playsound( Mix_Chunk *snd );
extern void audioMusicLoad( char *musicFilename, int repeatQty );
extern void audioMusicUnload( void );
extern void audioMusicPlay( Mix_Music *musicData, int repeatQty );

/* in theme.c */
extern void chooseTheme(void);
extern void setupTheme( char *dirname );

/* in gettext.c */
extern unsigned char *gettext( unsigned char *in );
extern int  load_trans(char* file);

/* in pause.c */
extern int  Pause( void );
extern void pause_load_media( void );
extern void pause_unload_media( void );
extern int  inRect( SDL_Rect r, int x, int y);

/* in loaders.c */
extern int         CheckFile(const char *file);
extern TTF_Font    *LoadFont( char *fontfile, int fontsize );
extern void         LoadLang( void );
extern Mix_Chunk   *LoadSound( char *datafile );
extern SDL_Surface *LoadImage( char *datafile, int mode );
extern sprite      *LoadSprite( char *name, int MODE );
extern sprite      *FlipSprite( sprite *in, int X, int Y );
extern void         FreeSprite( sprite *gfx );
extern Mix_Music   *LoadMusic( char *datafile );
extern SDL_Surface *flip( SDL_Surface *in, int x, int y );


/* in setup.c */
extern void GraphicsInit( Uint32 video_flags );
extern void LibInit( Uint32 lib_flags );
extern void LoadSettings( void );
extern void SaveSettings( void );

extern void InitEngine( void ); /* NOT in setup.c */

/* in alphabet.c */
extern void LoadKeyboard( void );
extern void set_letters( unsigned char *t );
extern unsigned char get_letter( void );
extern void custom_letter_setup( void );
extern void show_letters( void );
extern SDL_Surface* black_outline( unsigned char *t, TTF_Font* font, SDL_Color* c );
extern SDL_Surface* black_outline_wchar(wchar_t t, TTF_Font* font, SDL_Color* c);
extern SDL_Surface* GetWhiteGlyph(wchar_t t);
extern SDL_Surface* GetRedGlyph(wchar_t t);
extern int GetGlyphCoords(wchar_t t, int* x, int* y);
extern int RenderLetters(TTF_Font* letter_font);
extern void FreeLetters(void);
/* WORD FUNCTIONS (also in alphabet.c) */
extern void WORDS_init( void );
extern void WORDS_use_alphabet( void );
extern void WORDS_use( char *wordFn );
extern wchar_t* WORDS_get(void);

/* in practice.c */
extern int Practice( void );
extern void print_phrase( char *pphrase, int wrap );

/* in titlescreen.c */
extern void TitleScreen( void );
extern void switch_screen_mode( void );
extern int Phrases(char *practice_phrase);
extern int chooseWordlist( void );

/* In playgame.c: */
extern int int_rand( int min, int max );
extern void TransWipe(SDL_Surface * newbkg, int type, int var1, int var2);
