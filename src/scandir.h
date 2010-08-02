/* Brought to tuxmath from the Hatari project under the terms of the GPLv2+ */
/* It should only be included if scandir is not found by Autoconf - e.g.: */
/* #ifndef HAVE_SCANDIR (from autoconf's config.h)    - DSB               */

/*
  Hatari - scandir.h

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.
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
