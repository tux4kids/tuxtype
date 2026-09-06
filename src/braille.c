/*
   braille.c:

   Description: Functions for loading the Braille map and normalising
                key-chord order before dictionary lookup.

   Copyright 2013.
   Author: Nalin.x.Linux < Nalin.x.Linux@gmail.com >
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   braille.c is part of Tux Typing, a.k.a "tuxtype".

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

#include "globals.h"
#include "braille.h"

/* Maximum entries in the braille key→value dictionary (matches array size
 * declared in globals.h / defined in globals.c). */
#define BRAILLE_MAP_MAX 100

/* Arrange the given disordered key-combination
 * into the canonical order (fdsjkl).
 *
 * This ensures that any permutation of a chord (e.g. "dsf")
 * maps to the same dictionary entry as the canonical form ("fds"). */
void arrange_in_order(wchar_t *disorder)
{
    int iter = 0, i, j, len;
    /* Allocate wide-char buffers — use sizeof(wchar_t), not sizeof(char). */
    wchar_t *order = malloc(sizeof(wchar_t) * BRAILLE_MAP_MAX);
    wchar_t *temp  = malloc(sizeof(wchar_t) * BRAILLE_MAP_MAX);

    if (!order || !temp)
    {
        free(order);
        free(temp);
        return;
    }

    wcscpy(order, L"fdsjkl");
    wcscpy(temp, disorder);

    len = wcslen(disorder);
    disorder[iter] = L'\0';

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < len; j++)
        {
            if (order[i] == temp[j])
            {
                disorder[iter] = order[i];
                iter++;
            }
        }
    }
    disorder[iter] = L'\0';

    free(order);
    free(temp);
}


/* Braille map loading function
 *
 * The format of the input file is:
 *   keycombination<space>beginning_value<space>middle_value<space>end_value
 *
 * For languages that use the same Braille code for a character at the
 * beginning, middle, and end of a word, all three columns may be identical.
 *
 * The key combination must be written in fdsjkl order
 * (first f, then d, then s, then j, then k, then l). */
int braille_language_loader(char *language)
{
    int iter = 0;
    FILE *fp;
    char file[FNLEN * 2 + 16]; /* room for path + "/braille/" + filename */

    snprintf(file, sizeof(file), "%s/braille/%s",
             settings.default_data_path, language);
    fp = fopen(file, "r");

    if (fp == NULL)
    {
        DEBUGCODE { fprintf(stderr, "braille_language_loader: couldn't open '%s'\n", file); }
        return 0;
    }

    {
        /* Parse one line at a time so that short lines (2-column format such as
         * spanish.txt) never bleed into the next line.  fscanf's whitespace
         * matching treats '\n' the same as ' ', which causes it to silently
         * consume tokens from subsequent lines when fewer than 4 fields are
         * present on the current line.  fgetws + swscanf avoid this entirely. */
        wchar_t line[512];
        while (iter < BRAILLE_MAP_MAX && fgetws(line, 512, fp) != NULL)
        {
            wchar_t k[100]={0}, b[100]={0}, m[100]={0}, e[100]={0};
            int fields = swscanf(line, L"%99ls %99ls %99ls %99ls", k, b, m, e);

            if (fields == 4)
            {
                /* Standard 4-column format: key begin middle end */
                wcscpy(braille_key_value_map[iter].key,          k);
                wcscpy(braille_key_value_map[iter].value_begin,  b);
                wcscpy(braille_key_value_map[iter].value_middle, m);
                wcscpy(braille_key_value_map[iter].value_end,    e);
                iter++;
            }
            else if (fields == 2)
            {
                /* 2-column format: key value — replicate value for all positions */
                wcscpy(braille_key_value_map[iter].key,          k);
                wcscpy(braille_key_value_map[iter].value_begin,  b);
                wcscpy(braille_key_value_map[iter].value_middle, b);
                wcscpy(braille_key_value_map[iter].value_end,    b);
                iter++;
            }
            /* fields < 2 or EOF: blank/malformed line — skip silently */
        }
    }

    fclose(fp);
    return 1;
}
