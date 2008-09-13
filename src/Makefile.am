## Process with Automake to create Makefile.in

##DATA_PREFIX=${pkgdatadir}

bin_PROGRAMS = tuxtype

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
	iconv_string.c

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
	compiler.h

tuxtype_LDFLAGS = $(LTLIBINTL)

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
