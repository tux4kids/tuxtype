/***************************************************************************
gettext.c 
-  description: a crossplatform minimal gettext library
-------------------
begin                : Sunday Feb 23, 2003
copyright            : (C) 2003 by Jesse Andrews
email                : jdandr2@uky.edu
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "stdio.h"
#include "globals.h"

/* we will store our list in a linked list since
 * we don't expect too large of a list (famous last words!)
 */

/* FIXME not sure if this file's code is UTF-8 compatible DSB */

struct node {
	unsigned char *in;          // the english
	unsigned char *out;         // the translation
	struct node *next; // ptr to next in list, NULL if last
};

typedef struct node item; 

item *HEAD=NULL;

/* --- add a word to the linked list --- */

void add_word(unsigned char *in, unsigned char *out) {
	item *cur;
	/* -- allocate space for the node in the list -- */
	cur = (item *)malloc(sizeof(item));

	/* -- allocate space for strings, and copy over -- */
	cur->in = (unsigned char *)malloc(strlen(in)+2);
	strncpy(cur->in, in, strlen(in)+1);
	cur->out = (unsigned char *)malloc(strlen(out)+2);
	strncpy(cur->out, out, strlen(out)+1);

	/* -- add to the front of the list -- */
	cur->next = HEAD;
	HEAD = cur;
}

int load_trans(char *file) {
	/* this function will load the passed file (a .po file)
	 * if need be, it should erase any previously loaded
	 * translations.
	 *
	 * the filename passed must exist!
	 *
	 * returns: 0 if ok
	 * 	-1 if file could not be located
	 * 	-2 if file has errors in it
	 */

	item *ptr;
	FILE *f;
	unsigned char str[FNLEN];
	unsigned char in[FNLEN];
	unsigned char out[FNLEN];

	LOG( "Clearing previous translation list\n" );

	while(HEAD != NULL) {
		ptr = HEAD->next;
		free(HEAD);
		HEAD = ptr;
	}

	/* Yes, I know, I should use YACC/LEX
	 * but, until you provide an GPL YACC/LEX 
	 * implimentation on Mac OS _CLASSIC_, we have
	 * to do things so they are portable, which
	 * means, we have to parse our files by hand
	 * using "state machines"
	 */


	LOG( "Loading translation file\n" );
	f = fopen( file, "r" );

	if (f == NULL) return -1;
	
	/* ### ADD ERROR CHECKING ### */

	do {
		fscanf(f, "%[^\n]\n", str);

		/* get rid of any comments! */
		{
			unsigned char mode='O';
			int i;
			for (i = 0; i < strlen(str); i++) {
				if (mode == 'O') {
					switch (str[i]) {
						case '"': mode = 'I'; break;
						case '#': str[i]='\0'; break;
					}
				} else {
					switch (str[i]) {
						case '\\': 
							if (mode != 'S') mode = 'S';
							else mode = 'I';
							break;
						case '"': 
							if (mode != 'S') mode ='O'; 
							break;
						default:
							mode = 'I'; // get rid of any /
					}
				}
			}
		}

		/* we force msgid or msgstr to be at the begining of the line! */

		if (strncmp(str, "msgid", 5) == 0) {
			int start=0, endloc=0, i;
			for (i=0; i<strlen(str); i++)
				if (str[i] == '"') {
					if (!start)
						start = i;
					else
						endloc = i;
				}
			str[endloc]='\0';
			strcpy(in, str+start+1);
		}
		if (strncmp(str, "msgstr", 6) == 0) {
			int start=0,endloc=0, i;
			for (i=0; i<strlen(str); i++)
				if (str[i] == '"') {
					if (!start)
						start = i;
					else
						endloc = i;
				}
			str[endloc]='\0';
			strcpy(out, str+start+1);
			add_word(in, out);
		}

	} while( !feof(f) );

	LOG( "Completed loading of translation file\n" );

	return 0;
}

unsigned char * gettext( unsigned char *in ) {
	/* this function will attempt to translate the string
	 * "in" to an "translation of in" if one exists.
	 * if it doesn't exist in the translation set, it 
	 * returns "in".
	 */

	/* go through each time until we find what we want...
	 * if the number of translated words we use increases, 
	 * we should move to a hash table.
	 */

	item *cur=HEAD;

	if (useEnglish)
		return in;

	while (cur != NULL) 
		if (strcmp(cur->in, in) == 0)
			return cur->out;
		else
			cur = cur->next;

	/* if we didn't find anything return what we were asked */
	return in;
}
