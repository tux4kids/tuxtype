//
// C Interface: convert_utf
//
// Description: header file for simple wrapper functions to convert
// wchar_t and utf8 strings using GNU iconv().
//
//
// Author: David Bruce <davidstuartbruce@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
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