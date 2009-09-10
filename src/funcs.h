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

#include "SDL_extras.h"

/* (still in alphabet.c:) */
int CheckNeededGlyphs(void);
void ClearWordList(void);
void FreeLetters(void);
int GenerateWordList(const char* wordFn);
void GenCharListFromString(const char* UTF8_str);
void ResetCharList(void);
wchar_t GetLetter(void);
wchar_t* GetWord(void);
SDL_Surface* GetWhiteGlyph(wchar_t t);
SDL_Surface* GetRedGlyph(wchar_t t);
int LoadKeyboard(void);
int GetFinger(int i);
int RenderLetters(int font_size);
int GetIndex(wchar_t uni_char);
void GetKeyShift(int index, char *buf);
int GetShift(int i);
void GetKeyPos(int index, char *buf);
void GetWrongKeyPos(int index, char *buf);
//int map_keys(wchar_t *wide_str,keymap key);
void GenerateKeyboard(SDL_Surface* keyboard);
void updatekeylist(int key,char ch);
void savekeyboard(void);
wchar_t GetLastKey(void);


/* In audio.c:   */
void PlaySound(Mix_Chunk* snd);
void MusicLoad(const char* musicFilename, int repeatQty);
void MusicUnload(void);
void MusicPlay(Mix_Music* musicData, int repeatQty);


/* In laser.c:        */
int PlayLaserGame(int diff_level);


/* In loaders.c: */
int CheckFile(const char* file);
sprite* FlipSprite(sprite* in, int X, int Y);
void FreeSprite(sprite* gfx);
SDL_Surface* LoadImage(const char* datafile, int mode);
int LoadBothBkgds(const char* datafile);
SDL_Surface* CurrentBkgd(void);
void FreeBothBkgds(void);
void LoadLang(void);
Mix_Music* LoadMusic(const char* datafile);
Mix_Chunk* LoadSound(const char* datafile);
sprite* LoadSprite(const char* name, int MODE);

/* In options.c: */
void Opts_Initialize(void);

/* In pause.c: */
int  Pause(void);


/* In playgame.c: */
int PlayCascade(int level);


/* In practice.c: */
int Phrases(wchar_t* practice_phrase);


/* In scripting.c: */
int XMLLesson(void);
void ProjectInfo(void);
void InstructCascade(void);
void InstructLaser(void);


/* In setup.c: */
void GraphicsInit(void);
void LibInit(Uint32 lib_flags);
void LoadSettings(void);
void SaveSettings(void);
int SetupPaths(const char* theme_dir);
void Cleanup(void);

/* In theme.c: */
void ChooseTheme(void);


/* In titlescreen.c: */
//void SwitchScreenMode(void);
void TitleScreen(void);

