/*
   editor.h:

   Description: Header for word- and phrase list-editing functionality for
   Tux Typing.
   
   Copyright 2009, 2010.
   Authors: Sarah Frisk <ssfrisk@gmail.com> and 
            David Bruce <davidstuartbruce@gmail.com>.
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   editor.h is part of Tux Typing, a.k.a "tuxtype".

Tux Typing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tux Typing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef EDITOR_H
#define EDITOR_H


void ChooseListToEdit(void);
void EditWordList(char* words_file);


int CreateNewWordList(void);
int ChooseRemoveList(char *name, char *filename);
int RemoveList(char* words_file);

#define New 0
#define Remove 1
#define Done 2

#endif
