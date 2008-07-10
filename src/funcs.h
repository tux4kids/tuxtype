/***************************************************************************
                          funcs.h
  description: function header
                             -------------------
    begin                : Sat May 6 2000
    copyright            : (C) 2000 by Sam Hart
    email                : hart@geekcomix.com
 
  Modified by David Bruce
  dbruce@tampabay.rr.com
  2007

***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* NOTE - there is no reason to declare functions using "extern", as all */
/* non-local functions are visible throughout the program.               */ 


/* In alphabet.c */
SDL_Surface* BlackOutline(const unsigned char* t, const TTF_Font* font, const SDL_Color* c);
SDL_Surface* BlackOutline_Unicode(const Uint16* t, const TTF_Font* font, const SDL_Color* c);

#ifndef WIN32
SDL_Surface* BlackOutline_SDLPango(const unsigned char* t, const TTF_Font* font, const SDL_Color* c);
#endif

/* (still in alphabet.c:) */
int CheckNeededGlyphs(void);
void ClearWordList(void);
int ConvertFromUTF8(wchar_t* wide_word, const unsigned char* UTF8_word);
void FreeLetters(void);
int GenerateWordList(const char* wordFn);
void GenCharListFromString(const unsigned char* UTF8_str);
void ResetCharList(void);
wchar_t GetLetter(void);
wchar_t* GetWord(void);
SDL_Surface* GetWhiteGlyph(wchar_t t);
SDL_Surface* GetRedGlyph(wchar_t t);
int LoadKeyboard(void);
int GetFinger(wchar_t uni_char);
int RenderLetters(const TTF_Font* letter_font);

//void UseAlphabet(void);


/* In audio.c:   */
void PlaySound(Mix_Chunk* snd);
void MusicLoad(const char* musicFilename, int repeatQty);
void MusicUnload(void);
void MusicPlay(Mix_Music* musicData, int repeatQty);


/* In gettext.c:  */
//unsigned char* gettext(const unsigned char* in);
//int Load_PO_File(const char* file);


/* In laser.c:        */
int PlayLaserGame(int diff_level);


/* In loaders.c: */
int CheckFile(const char* file);
sprite* FlipSprite(sprite* in, int X, int Y);
void FreeSprite(sprite* gfx);
TTF_Font* LoadFont(const char* fontfile, int fontsize);
SDL_Surface* LoadImage(const char* datafile, int mode);
void LoadLang(void);
Mix_Music* LoadMusic(const char* datafile);
Mix_Chunk* LoadSound(const char* datafile);
sprite* LoadSprite(const char* name, int MODE);

/* In options.c: */
void Opts_Initialize(void);

/* In pause.c: */
int  Pause(void);
int  inRect(SDL_Rect r, int x, int y);


/* In playgame.c: */
int PlayCascade(int level);
void InitEngine(void);
int TransWipe(SDL_Surface* newbkg, int type, int var1, int var2);


/* In practice.c: */
int Phrases(wchar_t* practice_phrase);
static void next_letter(wchar_t *t, int c);


/* In scripting.c: */
int TestLesson(void);
void ProjectInfo(void);
void InstructCascade(void);
void InstructLaser(void);


/* In setup.c: */
void GraphicsInit(Uint32 video_flags);
void LibInit(Uint32 lib_flags);
void LoadSettings(void);
void SaveSettings(void);
int SetupPaths(const char* theme_dir);
void Cleanup(void);

/* In theme.c: */
void ChooseTheme(void);


/* In titlescreen.c: */
void SwitchScreenMode(void);
void TitleScreen(void);

