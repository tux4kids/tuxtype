/*
   braille.h:

   Declarations for Braille chord-input support:
     - braille dictionary map loader
     - key-combination ordering helper

   Copyright 2013.
   Author: Nalin.x.Linux < Nalin.x.Linux@gmail.com >
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   braille.h is part of Tux Typing, a.k.a "tuxtype".

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

#ifndef BRAILLE_H
#define BRAILLE_H

#include <wchar.h>

/* Normalise a disordered Braille key-chord to the canonical
 * fdsjkl order before looking it up in the dictionary. */
void arrange_in_order(wchar_t *disorder);

/* Load a per-language Braille key→character map from
 * <default_data_path>/braille/<language>.
 * Returns 1 on success, 0 if the file cannot be opened. */
int braille_language_loader(char *language);

#endif /* BRAILLE_H */
