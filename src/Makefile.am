## Process with Automake to create Makefile.in

# Support for gettext:
datadir = @datadir@
localstatedir = @localstatedir@
sysconfdir = @sysconfdir@
localedir = $(datadir)/locale
pkglocalstatedir = $(localstatedir)/$(PACKAGE)
pkgsysconfdir = $(sysconfdir)/$(PACKAGE)

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
  IM_PREFIX=@MINGW32_PACKAGE_IM_DIR@
else
  DATA_PREFIX=${pkgdatadir}
  VAR_PREFIX=${pkglocalstatedir}
  CONF_PREFIX=${pkgsysconfdir}
  IM_PREFIX=${pkgdatadir}/input_methods
endif


ACLOCAL_AMFLAGS = -I m4
# Is the following line obsolete?
#DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

AM_CFLAGS=-Wall -g \
        -DDATA_PREFIX=\"$(DATA_PREFIX)\" \
        -DVAR_PREFIX=\"$(VAR_PREFIX)\" \
        -DCONF_PREFIX=\"$(CONF_PREFIX)\" \
        -DIM_PREFIX=\"$(IM_PREFIX)\" \
        -DDEBUG \
	-DVERSION=\"@NAME_VERSION@\" -D$(SOUND)SOUND

AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" \
	-DLOCALSTATEDIR=\"$(pkglocalstatedir)\" \
	-DSYSCONFDIRDIR=\"$(pkgsysconfdir)\" \
	-DIMDIR=\"$(pkgdatadir)/input_methods\" \
	-I../intl -I$(top_srcdir)/intl

if BUILD_MINGW32
  bin_PROGRAMS = TuxType
else
  TUXTYPERC =
  bin_PROGRAMS = tuxtype
endif





LDADD = @LIBINTL@
tuxtype_LDFLAGS = $(LTLIBINTL)

tuxtype_SOURCES = 	\
	alphabet.c	\
	audio.c		\
	convert_utf.c	\
	editor.c	\
	input_methods.c	\
	laser.c		\
	loaders.c	\
	main.c		\
	mysetenv.c	\
	options.c	\
	pause.c		\
	pixels.c	\
	playgame.c	\
	practice.c	\
	scandir.c	\
	scripting.c	\
	SDL_extras.c	\
	setup.c		\
	snow.c		\
	theme.c		\
	titlescreen.c	\
	braille.c
TuxType_SOURCES  = $(tuxtype_SOURCES)

# The rc file has something to do with the icon in Windows, IIRC
#TuxType_SOURCES  = $(tuxtype_SOURCES) tuxtyperc.rc

EXTRA_DIST =		\
	compiler.h	\
	convert_utf.h	\
	editor.h	\
	funcs.h 	\
	gettext.h	\
	globals.h 	\
	input_methods.h	\
	laser.h		\
	mysetenv.h	\
	pixels.h	\
	playgame.h 	\
	scandir.h	\
	scripting.h	\
	SDL_extras.h	\
	snow.h		\
        titlescreen.h	\
	braille.h
