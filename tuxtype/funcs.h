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

#ifndef __FUNCS_H__
#define __FUNCS_H__

extern void FreeGraphics(int verbose);

extern void FreeGame(int verbose, int mus);

extern int TitleScreen(int verbose);

extern int PracticeKCascade(int verbose);

extern int PracticeWCascade(int verbose);

extern int PlayCascade(int level, int verbose);

extern int PlayWCascade(int diflevel, int verbose);

extern void GraphicsInit(Uint32 video_flags, int verbose);

extern void LibInit(Uint32 lib_flags, int verbose);

extern SDL_Surface *LoadImage(char *datafile, int transparent, int verbose);

extern Mix_Chunk *LoadSound(char *datafile, int verbose);

extern void LoadMusic(char *datafile, int verbose);

extern void WaitFrame(void);

#endif // __FUNCS_H__