## Process with Automake to create Makefile.in

##DATA_PREFIX=${pkgdatadir}

# Support for gettext:
datadir = @datadir@
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

AM_CFLAGS=-Wall -g -DDATA_PREFIX=\"${DATA_PREFIX}\" -DDEBUG \
	-DVERSION=\"@NAME_VERSION@\" -D$(SOUND)SOUND

AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" \
	-I../intl -I$(top_srcdir)/intl

LDADD = @LIBINTL@
tuxtype_LDFLAGS = $(LTLIBINTL)

if BUILD_MINGW32
bin_PROGRAMS = TuxType
else
TUXTYPERC =
bin_PROGRAMS = tuxtype
endif


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
	ConvertUTF.c	\
	options.c	\
	iconv_string.c  \
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
	ConvertUTF.h	\
	iconv_string.h  \
	gettext.h	\
	compiler.h	\
	mysetenv.h


