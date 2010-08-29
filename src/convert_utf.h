/*
   convert_utf.h:

   Description: header file for simple wrapper functions to convert
   wchar_t and utf8 strings using GNU iconv().
   
   Copyright 2009, 2010.
   Author: David Bruce.
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   convert_utf.h is part of Tux Typing, a.k.a "tuxtype".

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



#ifndef CONVERT_UTF_H
#define CONVERT_UTF_H

#include <wchar.h>

/* NOTE the "max_length" parameter should generally be the size of the output     */
/* buffer.  It must be at least one greater than the length of the return string  */
/* so that the string can be null-terminated.                                     */
/* Also, "max_length" must be no greater than the buffer length used internally   */
/* in these functions (i.e. UTF_BUF_LENGTH, currently 1024)                       */

int ConvertFromUTF8(wchar_t* wide_word, const char* UTF8_word, int max_length);
int ConvertToUTF8(const wchar_t* wide_word, char* UTF8_word, int max_length);

#endif
