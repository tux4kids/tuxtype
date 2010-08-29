/*
   scandir.h:

   Replacement scandir() and alphasort() for platforms lacking
   these functions.

   Brought to tuxmath by David Bruce from the GPLv2 (or later) Hatari project:
   http://hatari.berlios.de/

   Modified slightly and relicensed under GPLv3 or later.   
   
   Copyright 2007, 2008, 2009, 2010.
   Authors: Hatari Project.
   Project email: <tuxmath-devel@lists.sourceforge.net>
   Project website: http://tux4kids.alioth.debian.org


scandir.h is part of Tux Typing, a.k.a. "tuxtype".

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



#ifndef HATARI_SCANDIR_H
#define HATARI_SCANDIR_H

#include <dirent.h>

#ifdef QNX
#include <sys/types.h>
#include <sys/dir.h>
#define dirent direct
#endif

#if defined(__BEOS__) || defined(__HAIKU__) || (defined(__sun) && defined(__SVR4)) || defined(WIN32)
extern int alphasort(const void *d1, const void *d2);
extern int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(struct dirent *), int (*dcomp)(const void *, const void *));
#endif

#endif /* HATARI_SCANDIR_H */
