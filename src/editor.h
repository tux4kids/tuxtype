//
// C Interface: editor.h
//
// Description: Header for word- and phrase list-editing functionality for
//  Tux Typing
//
// Author: Sarah Frisk <ssfrisk@gmail.com> and David Bruce <davidstuartbruce@gmail.com>, (C) 2009
//
// Written for Google Summer of Code 2009
//
// Copyright: See COPYING file that comes with this distribution (briefly, GNU GPLv2+)
//
//

#ifndef EDITOR_H
#define EDITOR_H


void ChooseListToEdit(void);
void EditWordList(char* words_file);


void CreateNewWordList(void);
int ChooseRemoveList(void);
int RemoveList(char* words_file);

#define New 0
#define Remove 1
#define Done 2

#endif
