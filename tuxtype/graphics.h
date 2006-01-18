/***************************************************************************
                          graphics.h
  description: graphics definitions
                             -------------------
    begin                : Fri May 5 2000
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

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define RES_X					640
#define RES_Y					480
#define BPP						0

int menu_item[TITLE_MENU_ITEMS + 1][TITLE_MENU_DEPTH + 1];

const char PATH[PATHNUM][FNLEN] =
    { DATA_PREFIX "/tuxtype" , ".", ".." };

/*const char PATH[PATHNUM][FNLEN] =
    { ".", "data", "/usr/local/share/tuxtype", "/usr/share/tuxtype",
	"../data", "/usr/share/pixmaps/tuxtype" }; */
	
#endif //  __GRAPHICS_H__
