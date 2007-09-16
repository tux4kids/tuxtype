# Process this file with autoconf to produce a configure script.
AC_INIT(tuxtype, 1.5.14, tuxmath-devel@lists.sourceforge.net)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(tuxtype, 1.5.14)

AC_CONFIG_SRCDIR([tuxtype/funcs.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl Don't think we need C++:
# AC_PROG_CPP
# AC_PROG_CXX

AC_PREFIX_DEFAULT(/usr/local)
if test "x$prefix" = "xNONE"; then
  prefix=$ac_default_prefix
  ac_configure_args="$ac_configure_args --prefix $prefix"
fi

# Checks for libraries.
# Check for SDL
SDL_VERSION=1.2.5



AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))

AC_CHECK_LIB([SDL_mixer], [Mix_PlayMusic], ,[AC_MSG_ERROR(SDL_mixer not found http://www.libsdl.org/projects/SDL_mixer, )])
AC_CHECK_LIB([SDL_ttf], [TTF_Init], ,[AC_MSG_ERROR(SDL_ttf not found http://www.libsdl.org/projects/SDL_ttf, )])
AC_CHECK_LIB([SDL_image], [IMG_Load], ,[AC_MSG_ERROR(SDL_image not found http://www.libsdl.org/projects/SDL_image, )])

# FIXME would be better to skip check for win32 build
AC_CHECK_LIB([SDL_Pango], [SDLPango_Init], ,[AC_MSG_WARN(SDL_Pango not found http://sdlpango.sourceforge.net/)])


#
SDL_CFLAGS=`sdl-config --cflags`
CFLAGS="$CFLAGS $SDL_CFLAGS"

SDL_LIBS=`sdl-config --libs`
LIBS="$SDL_LIBS $LIBS"

# Check platform - see if see if building for Windows:
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

AM_CONDITIONAL(BUILD_MINGW32, test "$native_win32" = yes)

if test "$native_win32" = yes; then

   AC_DEFINE([BUILD_MINGW32], 1,[Native MinGW32 build])
   CFLAGS="$CFLAGS -D__GW32__"
   LIBS="-lmingw32 -lSDLmain $LIBS -lgw32c -luuid -lole32 -lwsock32 -mwindows"
   CPPFLAGS="$CPPFLAGS -idirafter $prefix/include/glibc"
   #CPPFLAGS="$CPPFLAGS -I$prefix/include/glibc"
   AC_PATH_PROG(WINDRES, "$target_alias-windres", no, [$PATH])
fi


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([atexit memset strchr strstr malloc])





# Sam's Install Kludge-work
CPPFLAGS="$CPPFLAGS -DDATA_PREFIX=\\\"\$(prefix)\\\""

AC_CONFIG_FILES([Makefile
tuxtype/Makefile
tuxtype/data/Makefile
tuxtype/data/fonts/Makefile
tuxtype/data/images/Makefile
tuxtype/data/images/backgrounds/Makefile
tuxtype/data/images/cities/Makefile
tuxtype/data/images/comets/Makefile
tuxtype/data/images/hands/Makefile
tuxtype/data/images/keyboard/Makefile
tuxtype/data/images/menu/Makefile
tuxtype/data/images/status/Makefile
tuxtype/data/images/tux/Makefile
tuxtype/data/scripts/Makefile
tuxtype/data/sounds/Makefile
tuxtype/data/themes/Makefile
tuxtype/data/themes/armenian/Makefile
tuxtype/data/themes/armenian/images/Makefile
tuxtype/data/themes/bokmal/Makefile
tuxtype/data/themes/bokmal/words/Makefile
tuxtype/data/themes/bokmal/images/Makefile
tuxtype/data/themes/bokmal/images/status/Makefile
tuxtype/data/themes/brazilian-portuguese/Makefile
tuxtype/data/themes/brazilian-portuguese/images/Makefile
tuxtype/data/themes/brazilian-portuguese/words/Makefile
tuxtype/data/themes/catalan/Makefile
tuxtype/data/themes/catalan/words/Makefile
tuxtype/data/themes/dansk/Makefile
tuxtype/data/themes/dansk/sounds/Makefile
tuxtype/data/themes/dansk/words/Makefile
tuxtype/data/themes/deutsch/Makefile
tuxtype/data/themes/deutsch/words/Makefile
tuxtype/data/themes/espanol/Makefile
tuxtype/data/themes/espanol/images/Makefile
tuxtype/data/themes/espanol/words/Makefile
tuxtype/data/themes/euskara/Makefile
tuxtype/data/themes/euskara/images/Makefile
tuxtype/data/themes/euskara/words/Makefile
tuxtype/data/themes/french/Makefile
tuxtype/data/themes/french/words/Makefile
tuxtype/data/themes/greek/Makefile
tuxtype/data/themes/greek/words/Makefile
tuxtype/data/themes/italian/Makefile
tuxtype/data/themes/italian/words/Makefile
tuxtype/data/themes/jamaican/Makefile
tuxtype/data/themes/jamaican/images/Makefile
tuxtype/data/themes/jamaican/words/Makefile
tuxtype/data/themes/lithuanian/Makefile
tuxtype/data/themes/lithuanian/images/Makefile
tuxtype/data/themes/lithuanian/images/status/Makefile
tuxtype/data/themes/lithuanian/sounds/Makefile
tuxtype/data/themes/lithuanian/words/Makefile
tuxtype/data/themes/malayalam/Makefile
tuxtype/data/themes/malayalam/images/Makefile
tuxtype/data/themes/malayalam/images/status/Makefile
tuxtype/data/themes/malayalam/images/keyboard/Makefile
tuxtype/data/themes/malayalam/scripts/Makefile
tuxtype/data/themes/malayalam/words/Makefile
tuxtype/data/themes/nederlands/Makefile
tuxtype/data/themes/nederlands/words/Makefile
tuxtype/data/themes/nynorsk/Makefile
tuxtype/data/themes/nynorsk/images/Makefile
tuxtype/data/themes/nynorsk/images/status/Makefile
tuxtype/data/themes/nynorsk/words/Makefile
tuxtype/data/themes/russian/Makefile
tuxtype/data/themes/russian/images/Makefile
tuxtype/data/themes/russian/images/status/Makefile
tuxtype/data/themes/russian/words/Makefile
tuxtype/data/themes/svenska/Makefile
tuxtype/data/themes/svenska/words/Makefile
tuxtype/data/themes/swahili/Makefile
tuxtype/data/themes/swahili/words/Makefile
tuxtype/data/words/Makefile
tuxtype/docs/Makefile
tuxtype/docs/en/Makefile
tuxtype.spec])

# Support for building NSIS Win32 installer (adapted from TuxMath NSIS):
NSI_INSTALL_DIR=mingw32
AC_SUBST(NSI_INSTALL_DIR)

NSI_DLL_DIR=~/tuxtype_dll
AC_ARG_WITH(dll-directory,AC_HELP_STRING(
    [--with-dll-directory=path],
    [set the path where dll for TuxType are [$(NSI_DLL_DIR)]]), dll_path=$withval, dll_path=no)

AM_CONDITIONAL(NSI_BUILD, test "$dll_path" = no)

if test $dll_path != no; then
NSI_DLL_DIR=$dll_path
fi
AC_SUBST(NSI_DLL_DIR)


AC_PATH_PROG(NSIS, makensis, no, [$PATH])

# FIXME should this go into earlier AC_CONFIG_FILES ?
AC_CONFIG_FILES([
nsis/tuxtype.nsi
])

AC_OUTPUT

