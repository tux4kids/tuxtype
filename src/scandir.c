/*
   scandir.c:

   Replacement scandir() and alphasort() for platforms lacking
   these functions.

   Brought to tuxmath by David Bruce from the GPLv2 (or later) Hatari project:
   http://hatari.berlios.de/

   Modified slightly and relicensed under GPLv3 or later.   
   
   Copyright 2007, 2008, 2009, 2010.
   Authors: Hatari Project.
   Project email: <tuxmath-devel@lists.sourceforge.net>
   Project website: http://tux4kids.alioth.debian.org


scandir.c is part of Tux Typing, a.k.a. "tuxtype".

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



#include "config.h"
#ifndef HAVE_SCANDIR

/*
  Hatari - scandir.c

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.

  scandir function for BEOS, SunOS etc..
*/
const char ScanDir_rcsid[] = "Hatari $Id: scandir.c,v 1.3 2007/01/16 18:42:59 thothy Exp $";

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "scandir.h"

/*-----------------------------------------------------------------------
 * Here come alphasort and scandir for BeOS/Haiku and SunOS
 *-----------------------------------------------------------------------*/
#if defined(__BEOS__) || defined(__HAIKU__) || (defined(__sun) && defined(__SVR4))

#undef DIRSIZ

#define DIRSIZ(dp)                                          \
                ((sizeof(struct dirent) - sizeof(dp)->d_name) +     \
                (((dp)->d_reclen + 1 + 3) &~ 3))

#if defined(__sun) && defined(__SVR4)
# define dirfd(d) ((d)->dd_fd)
#elif defined(__BEOS__)
#ifndef (__HAIKU__)
#else
# define dirfd(d) ((d)->fd)
#endif
#endif

/
/*-----------------------------------------------------------------------*/
/**
 * Alphabetic order comparison routine.
 */
int alphasort(const void *d1, const void *d2)
{
  return strcmp((*(struct dirent * const *)d1)->d_name, (*(struct dirent * const *)d2)->d_name);
}


/*-----------------------------------------------------------------------*/
/**
 * Scan a directory for all its entries
 */
int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(struct dirent *), int (*dcomp)(const void *, const void *))
{
  struct dirent *d, *p, **names;
  struct stat stb;
  size_t nitems;
  size_t arraysz;
  DIR *dirp;

  if ((dirp = opendir(dirname)) == NULL)
    return(-1);

  if (fstat(dirfd(dirp), &stb) < 0)
    return(-1);

  /*
   * estimate the array size by taking the size of the directory file
   * and dividing it by a multiple of the minimum size entry.
   */
  arraysz = (stb.st_size / 24);

  names = (struct dirent **)malloc(arraysz * sizeof(struct dirent *));
  if (names == NULL)
    return(-1);

  nitems = 0;

  while ((d = readdir(dirp)) != NULL)
  {

    if (sdfilter != NULL && !(*sdfilter)(d))
      continue;       /* just selected names */

    /*
     * Make a minimum size copy of the data
     */

    p = (struct dirent *)malloc(DIRSIZ(d));
    if (p == NULL)
      return(-1);

    p->d_ino = d->d_ino;
    p->d_reclen = d->d_reclen;
    /*p->d_namlen = d->d_namlen;*/
    memcpy(p->d_name, d->d_name, p->d_reclen + 1);

    /*
     * Check to make sure the array has space left and
     * realloc the maximum size.
     */

    if (++nitems >= arraysz)
    {

      if (fstat(dirfd(dirp), &stb) < 0)
        return(-1);     /* just might have grown */

      arraysz = stb.st_size / 12;

      names = (struct dirent **)realloc((char *)names, arraysz * sizeof(struct dirent *));
      if (names == NULL)
        return(-1);
    }

    names[nitems-1] = p;
  }

  closedir(dirp);

  if (nitems && dcomp != NULL)
    qsort(names, nitems, sizeof(struct dirent *), dcomp);

  *namelist = names;

  return nitems;
}


#endif /* __BEOS__ || __sun */


/*-----------------------------------------------------------------------
* Here come alphasort and scandir for Windows
*-----------------------------------------------------------------------*/
#if defined(WIN32)

#undef DATADIR     // stupid windows.h defines DATADIR, too
#include <windows.h>

/*-----------------------------------------------------------------------*/
/**
* Alphabetic order comparison routine.
*/
int alphasort(const void *d1, const void *d2)
{
  return stricmp((*(struct dirent * const *)d1)->d_name, (*(struct dirent * const *)d2)->d_name);
}

/*-----------------------------------------------------------------------*/
/**
* Scan a directory for all its entries
*/
int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(struct dirent *), int (*dcomp)(const void *, const void *))
{
  int len;
  char *findIn, *d;
  WIN32_FIND_DATA find;
  HANDLE h;
  int nDir = 0, NDir = 0;
  struct dirent **dir = 0, *selectDir;
  unsigned long ret;

  len    = strlen(dirname);
  findIn = (char *)malloc(len+5);
  strcpy(findIn, dirname);
  printf("scandir : findIn orign=%s\n", findIn);
  for (d = findIn; *d; d++)
    if (*d=='/')
      *d='\\';
  if ((len==0))
  {
    strcpy(findIn, ".\\*");
  }
  if ((len==1)&& (d[-1]=='.'))
  {
    strcpy(findIn, ".\\*");
  }
  if ((len>0) && (d[-1]=='\\'))
  {
    *d++ = '*';
    *d = 0;
  }
  if ((len>1) && (d[-1]=='.') && (d[-2]=='\\'))
  {
    d[-1] = '*';
  }
  if ((len>1) && (d[-2]!='\\') && (d[-1]!='*'))
  {
    *d++ = '\\';
    *d++ = '*';
    *d = 0;
  }

  printf("scandir : findIn processed=%s\n", findIn);
  if ((h=FindFirstFile(findIn, &find))==INVALID_HANDLE_VALUE)
  {
    printf("scandir : FindFirstFile error\n");
    ret = GetLastError();
    if (ret != ERROR_NO_MORE_FILES)
    {
      // TODO: return some error code
    }
    *namelist = dir;
    return nDir;
  }
  do
  {
    printf("scandir : findFile=%s\n", find.cFileName);
    selectDir=(struct dirent*)malloc(sizeof(struct dirent)+strlen(find.cFileName));
    strcpy(selectDir->d_name, find.cFileName);
    if (!sdfilter || (*sdfilter)(selectDir))
    {
      if (nDir==NDir)
      {
        struct dirent **tempDir = (struct dirent **)calloc(sizeof(struct dirent*), NDir+33);
        if (NDir)
          memcpy(tempDir, dir, sizeof(struct dirent*)*NDir);
        if (dir)
          free(dir);
        dir = tempDir;
        NDir += 32;
      }
      dir[nDir] = selectDir;
      nDir++;
      dir[nDir] = 0;
    }
    else
    {
      free(selectDir);
    }
  }
  while (FindNextFile(h, &find));
  ret = GetLastError();
  if (ret != ERROR_NO_MORE_FILES)
  {
    // TODO: return some error code
  }
  FindClose(h);

  free (findIn);

  if (dcomp)
    qsort (dir, nDir, sizeof(*dir),dcomp);

  *namelist = dir;
  return nDir;
}

#endif /* WIN32 */
#endif /* HAVE_SCANDIR */
