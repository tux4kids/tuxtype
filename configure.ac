# Process this file with autoconf to produce a configure script.
AC_INIT([Tux Typing],[1.7.3],[tux4kids-tuxtype-dev@lists.alioth.debian.org],[tuxtype])
AC_CONFIG_SRCDIR([src/playgame.c])

AC_PREREQ(2.61)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET


# Tell Automake not to be as strict about packaging standards for tuxtype
# as it would be for an official Gnu program:
AM_INIT_AUTOMAKE(foreign)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

NAME_VERSION=$PACKAGE-$VERSION

AC_DEFINE_UNQUOTED([PROGRAM_NAME], "$PACKAGE", [Tux Typing])

AC_SUBST(NAME_VERSION)

# To link gettext library
AC_GNU_SOURCE
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION([0.17])
AM_GNU_GETTEXT_INTL_SUBDIR

# ----------------------------------------------------------------------
# Checks for programs.
# ----------------------------------------------------------------------

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_MKDIR_P
AC_PROG_RANLIB
AC_PROG_YACC


#AC_PREFIX_DEFAULT(/usr/local)
#if test "x$prefix" = "xNONE"; then
#  prefix=$ac_default_prefix
#  ac_configure_args="$ac_configure_args --prefix $prefix"
#fi

# -----------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------

# Check for SDL 
SDL_VERSION=1.2.0

AM_PATH_SDL($SDL_VERSION, :,
         AC_MSG_ERROR([SDL version $SDL_VERSION not found! http://www.libsdl.org]) 
) 

CFLAGS="$CFLAGS $SDL_CFLAGS" 


# Check platform - see if WinMain needed:
AC_MSG_CHECKING([for native Win32])
case "$host" in
  *-*-mingw*)
    native_win32=yes
    ;;
  *)
    native_win32=no
    ;;
esac
AC_MSG_RESULT([$native_win32])

AM_CONDITIONAL(BUILD_MINGW32, test $native_win32 = yes)

if test $native_win32 = yes; then
  LIBS="$LIBS -lmingw32 -lSDLmain -lSDL"
else
  LIBS="$LIBS $SDL_LIBS"
fi



dnl Check for SDL_image: 
AC_CHECK_LIB([SDL_image], 
             [IMG_Load],
             ,
             [AC_MSG_ERROR([SDL_image not found! http://www.libsdl.org/projects/SDL_image])])

dnl Check for SDL_mixer:. 
AC_CHECK_LIB([SDL_mixer], 
             [Mix_OpenAudio], 
             ,
             [AC_MSG_ERROR([SDL_mixer not found! http://www.libsdl.org/projects/SDL_mixer])])

dnl Check for SDL_ttf:
AC_CHECK_LIB([SDL_ttf],
             [TTF_Init],
             ,
             [AC_MSG_ERROR([SDL_ttf not found! http://www.libsdl.org/projects/SDL_ttf])])



# FIXME would be better to skip check for win32 build
AC_CHECK_LIB([SDL_Pango], 
             [SDLPango_Init],
             ,
             [AC_MSG_WARN(SDL_Pango not found http://sdlpango.sourceforge.net/)])



# --------------------------------------------------------------------------------------
# Checks for header files.
# --------------------------------------------------------------------------------------

AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([argz.h error.h errno.h fcntl.h float.h iconv.h inttypes.h langinfo.h libintl.h limits.h locale.h malloc.h stddef.h stdint.h stdio_ext.h stdlib.h string.h strings.h sys/param.h unistd.h wchar.h])


# --------------------------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# --------------------------------------------------------------------------------------------

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])


# -------------------------------------------------------------------------------------------
# Checks for library functions.
# -------------------------------------------------------------------------------------------


AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
#AC_FUNC_MALLOC
AC_FUNC_MMAP
#AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify atexit bcopy getcwd getenv localtime_r memmove mempcpy memset mkdir munmap nl_langinfo putenv scandir setenv setlocale stpcpy strcasecmp strchr strcspn strdup strncasecmp strndup strstr strtoul])

# Set PACKAGE_DATA_DIR
MINGW32_PACKAGE_DATA_DIR="data"
AC_SUBST(MINGW32_PACKAGE_DATA_DIR)

#AM_CONDITIONAL(BUILD_MINGW32, test "$native_win32" = yes)

if test $native_win32 = yes; then
   AC_DEFINE([BUILD_MINGW32], 1,[Native MinGW32 build])
   CFLAGS="$CFLAGS -D__GW32__"
   LIBS="$LIBS -luuid -lole32 -lwsock32 -mwindows"
   CPPFLAGS="$CPPFLAGS -idirafter $prefix/include/glibc"
   AC_PATH_PROG(WINDRES, "$target_alias-windres", no, [$PATH])
fi


# ------------------------------------------------------------
# Support for building NSIS Win32 installer:
# ------------------------------------------------------------

NSI_INSTALL_DIR=mingw32
AC_SUBST(NSI_INSTALL_DIR)

NSI_DLL_DIR=~/tuxtype_dll
AC_ARG_WITH(dll-directory,AS_HELP_STRING([--with-dll-directory=path],[set the path where dll for TuxType are [$(NSI_DLL_DIR)]]), dll_path=$withval, dll_path=no)

AM_CONDITIONAL(NSI_BUILD, test $dll_path = no)

if test $dll_path != no; then
NSI_DLL_DIR=$dll_path
fi
AC_SUBST(NSI_DLL_DIR)

AC_PATH_PROG(NSIS, makensis, [], [$PATH])


## Sam's Install Kludge-work
#CPPFLAGS="$CPPFLAGS -DDATA_PREFIX=\\\"\$(prefix)\\\""

# ---------------------------------------------------------------
# Create Makefiles
# ---------------------------------------------------------------

AC_CONFIG_FILES([Makefile po/Makefile.in intl/Makefile
data/Makefile
data/fonts/Makefile
data/images/Makefile
data/images/backgrounds/Makefile
data/images/cities/Makefile
data/images/comets/Makefile
data/images/hands/Makefile
data/images/icons/Makefile
data/images/keyboard/Makefile
data/images/menu/Makefile
data/images/status/Makefile
data/images/tux/Makefile
data/scripts/Makefile
data/sounds/Makefile
data/themes/Makefile
data/themes/armenian/Makefile
data/themes/armenian/images/Makefile
data/themes/assamese/Makefile
data/themes/assamese/words/Makefile
data/themes/bokmal/Makefile
data/themes/bokmal/words/Makefile
data/themes/bokmal/images/Makefile
data/themes/bokmal/images/status/Makefile
data/themes/brazilian-portuguese/Makefile
data/themes/brazilian-portuguese/images/Makefile
data/themes/brazilian-portuguese/words/Makefile
data/themes/catalan/Makefile
data/themes/catalan/words/Makefile
data/themes/czech/Makefile
data/themes/czech/words/Makefile
data/themes/dansk/Makefile
data/themes/dansk/sounds/Makefile
data/themes/dansk/words/Makefile
data/themes/deutsch/Makefile
data/themes/deutsch/words/Makefile
data/themes/espanol/Makefile
data/themes/espanol/images/Makefile
data/themes/espanol/words/Makefile
data/themes/euskara/Makefile
data/themes/euskara/images/Makefile
data/themes/euskara/words/Makefile
data/themes/french/Makefile
data/themes/french/words/Makefile
data/themes/greek/Makefile
data/themes/greek/words/Makefile
data/themes/gujarati/Makefile
data/themes/gujarati/words/Makefile
data/themes/hindi/Makefile
data/themes/hindi/words/Makefile
data/themes/italian/Makefile
data/themes/italian/images/Makefile
data/themes/italian/words/Makefile
data/themes/jamaican/Makefile
data/themes/jamaican/images/Makefile
data/themes/jamaican/words/Makefile
data/themes/kannada/Makefile
data/themes/kannada/words/Makefile
data/themes/lithuanian/Makefile
data/themes/lithuanian/images/Makefile
data/themes/lithuanian/images/status/Makefile
data/themes/lithuanian/sounds/Makefile
data/themes/lithuanian/words/Makefile
data/themes/malayalam/Makefile
data/themes/malayalam/images/Makefile
data/themes/malayalam/images/status/Makefile
data/themes/malayalam/images/keyboard/Makefile
data/themes/malayalam/scripts/Makefile
data/themes/malayalam/words/Makefile
data/themes/nederlands/Makefile
data/themes/nederlands/words/Makefile
data/themes/nepali/Makefile
data/themes/nepali/words/Makefile
data/themes/nynorsk/Makefile
data/themes/nynorsk/images/Makefile
data/themes/nynorsk/images/status/Makefile
data/themes/nynorsk/words/Makefile
data/themes/oriya/Makefile
data/themes/oriya/words/Makefile
data/themes/punjabi/Makefile
data/themes/punjabi/words/Makefile
data/themes/russian/Makefile
data/themes/russian/images/Makefile
data/themes/russian/images/status/Makefile
data/themes/russian/words/Makefile
data/themes/svenska/Makefile
data/themes/svenska/words/Makefile
data/themes/swahili/Makefile
data/themes/swahili/words/Makefile
data/themes/tamil/Makefile
data/themes/tamil/words/Makefile
data/themes/telugu/Makefile
data/themes/telugu/words/Makefile
data/themes/wolof/Makefile
data/themes/wolof/images/Makefile
data/themes/wolof/words/Makefile
data/words/Makefile
doc/Makefile
doc/en/Makefile
src/Makefile
nsis/tuxtype.nsi])
##tuxtype.spec])

AC_OUTPUT

