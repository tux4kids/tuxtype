/*
   convert_utf.c:

   Description: simple wrapper functions to convert
   wchar_t and utf8 strings using GNU iconv().
   
   Copyright 2009, 2010.
   Author: David Bruce.
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   convert_utf.c is part of Tux Typing, a.k.a "tuxtype".

Tux Typing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tux Typing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
Mechanical or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "convert_utf.h"
#include "globals.h"

#include <iconv.h>
#include <string.h>
#include <wchar.h>

#define UTF_BUF_LENGTH 1024

/* GNU iconv()-based implementation:   */

int ConvertFromUTF8(wchar_t* wide_word, const char* UTF8_word, int max_length)
{
  if (!wide_word || !UTF8_word || max_length <= 0)
    return 0;

  wchar_t temp_wchar[UTF_BUF_LENGTH];
  memset(temp_wchar, 0, sizeof(temp_wchar));
  wchar_t* wchar_start = temp_wchar;

  iconv_t conv_descr;
  size_t bytes_converted;
  size_t in_length = strlen(UTF8_word);
  size_t out_length = (UTF_BUF_LENGTH - 1) * sizeof(wchar_t);

  if(max_length > UTF_BUF_LENGTH)
  {
    fprintf(stderr, "ConvertFromUTF8() - error - requested string length %d exceeds buffer length %d\n",
            max_length, UTF_BUF_LENGTH);
    return 0;
  }

#ifdef WIN32
  conv_descr = iconv_open("UTF-16LE", "UTF-8");
#else
  conv_descr = iconv_open("wchar_t", "UTF-8");
#endif

  if (conv_descr == (iconv_t)-1)
  {
    wide_word[0] = L'\0';
    return 0;
  }

  char *in_buf = (char*) UTF8_word;
  bytes_converted = iconv(conv_descr,
                          &in_buf, &in_length,
                          (char**) &wchar_start, &out_length);
  iconv_close(conv_descr);
  wcsncpy(wide_word, temp_wchar, max_length - 1);
  wide_word[max_length - 1] = L'\0';

  return wcslen(wide_word);
}


/******************To be used for savekeyboard*************/
/***Converts wchar_t string to char string*****************/
int ConvertToUTF8(const wchar_t* wide_word, char* UTF8_word, int max_length)
{
  if (!wide_word || !UTF8_word || max_length <= 0)
    return 0;

  char temp_UTF8[UTF_BUF_LENGTH];
  memset(temp_UTF8, 0, sizeof(temp_UTF8));
  char* UTF8_Start = temp_UTF8;

  iconv_t conv_descr;
  size_t bytes_converted;
  size_t in_length = wcslen(wide_word) * sizeof(wchar_t);
  size_t out_length = UTF_BUF_LENGTH - 1;

  if(max_length > UTF_BUF_LENGTH)
  {
    fprintf(stderr, "ConvertToUTF8() - error - requested string length %d exceeds buffer length %d\n",
            max_length, UTF_BUF_LENGTH);
    return 0;
  }

#ifdef WIN32
  conv_descr = iconv_open("UTF-8", "UTF-16LE");
#else
  conv_descr = iconv_open("UTF-8", "wchar_t");
#endif

  if (conv_descr == (iconv_t)-1)
  {
    UTF8_word[0] = '\0';
    return 0;
  }

  char *in_buf = (char*) wide_word;
  bytes_converted = iconv(conv_descr,
                          &in_buf, &in_length,
                          &UTF8_Start, &out_length);
  iconv_close(conv_descr);
  strncpy(UTF8_word, temp_UTF8, max_length - 1);
  UTF8_word[max_length - 1] = '\0';

  return strlen(UTF8_word);
}
