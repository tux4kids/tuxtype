## Process with Automake to create Makefile.in


ACLOCAL_AMFLAGS = -I m4
# Is the following line obsolete?
#DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

AM_CFLAGS=-Wall -g -DDATA_PREFIX=\"${DATA_PREFIX}\" -DDEBUG \
	-DVERSION=\"@NAME_VERSION@\" -D$(SOUND)SOUND

AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" \
	-DLOCALSTATEDIR=\"$(pkglocalstatedir)\" \
	-DSYSCONFDIRDIR=\"$(pkgsysconfdir)\" \
	-I../intl -I$(top_srcdir)/intl

if BUILD_MINGW32
  bin_PROGRAMS = TuxType
else
  TUXTYPERC =
  bin_PROGRAMS = tuxtype
endif

## These variables are used at run-time to locate files because they will
## not be in unix-type locations on Windows systems. The reason not to just
## change the parent variable (e.g. pkgdatadir) under Windows is that the
## packaging for Windows involves a temporary DESTDIR "make install", after
## which the files get copied into the NSIS installer.  We never run
## "make install" on the Windows machine itself.

if BUILD_MINGW32
  DATA_PREFIX=@MINGW32_PACKAGE_DATA_DIR@
  VAR_PREFIX=@MINGW32_PACKAGE_VAR_DIR@
  CONF_PREFIX=@MINGW32_PACKAGE_CONF_DIR@
else
  DATA_PREFIX=${pkgdatadir}
  VAR_PREFIX=$(pkglocalstatedir)
  CONF_PREFIX=$(pkgsysconfdir)
endif



LDADD = @LIBINTL@
tuxtype_LDFLAGS = $(LTLIBINTL)

tuxtype_SOURCES = 	\
	playgame.c	\
	main.c		\
	titlescreen.c	\
	loaders.c	\
	setup.c		\
	laser.c		\
	alphabet.c	\
	theme.c		\
	practice.c	\
	audio.c		\
	snow.c		\
	scripting.c	\
	SDL_extras.c	\
	pixels.c	\
	pause.c		\
	convert_utf.c	\
	options.c	\
	mysetenv.c

TuxType_SOURCES  = $(tuxtype_SOURCES)
# The rc file has something to do with the icon in Windows, IIRC
#TuxType_SOURCES  = $(tuxtype_SOURCES) tuxtyperc.rc

EXTRA_DIST = titlescreen.h \
	playgame.h 	\
	laser.h		\
	globals.h 	\
	funcs.h 	\
	pixels.h	\
	scripting.h	\
	SDL_extras.h	\
	snow.h		\
	convert_utf.h	\
	gettext.h	\
	compiler.h	\
	mysetenv.h


