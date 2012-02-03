# Modified to include support for NSIS Windows installer - David Bruce <davidstuartbruce@gmail.com>

# Support for gettext:
datadir = @datadir@
localstatedir = @localstatedir@
sysconfdir = @sysconfdir@
localedir = $(datadir)/locale
pkglocalstatedir = $(localstatedir)/$(PACKAGE)
pkgsysconfdir = $(sysconfdir)/$(PACKAGE)


SUBDIRS = data \
	doc	\
	po	\
	src

EXTRA_DIST = tuxtype.spec \
	tuxtype.desktop  \
	tuxtype.ico \
	icon.png \
	Info.plist \
	config.rpath 



## These are defined in configure.ac:
MAKENSIS=@NSIS@
NSIS_FOUND=@NSIS_FOUND@
NSI_INSTALL_DIR=@NSI_INSTALL_DIR@
NSI_DLL_DIR=@NSI_DLL_DIR@
NSI_TEMP_INSTALL_DIR=$(abs_top_builddir)/_instw32


##FIXME redo with proper Automake variables: 
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/tuxtype.ico $(DESTDIR)$(prefix)/share/$(PACKAGE)/tuxtype.ico

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/tuxtype.ico
	-rm -rf $(DESTDIR)$(prefix)/share/$(PACKAGE)

# Sould we skip uninstall of these directories to avoid clobbering user-created files
# on upgrade?:
install-data-hook:
# Global configuration (e.g. /etc/tuxtype):
	$(MKDIR_P) -m 755 $(DESTDIR)$(pkgsysconfdir)
# Global sites for game state data (e.g. /var/games/tuxtype):
	$(MKDIR_P) -m 2755 $(DESTDIR)$(pkglocalstatedir)
	$(MKDIR_P) -m 2755 $(DESTDIR)$(pkglocalstatedir)/words
#	chown root:games $(DESTDIR)$(pkglocalstatedir)
	chown root:games $(DESTDIR)$(pkglocalstatedir)/words


uninstall-hook:
	rm -rf $(DESTDIR)$(pkglocalstatedir)/words
	rm -rf $(DESTDIR)$(pkglocalstatedir)
	rm -rf $(DESTDIR)$(pkgsysconfdir)



# Rule to build tar-gzipped distribution package
$(PACKAGE)-$(VERSION).tar.gz: distcheck

#FIXME this won't really work.  Build for rpm packages now automated at
#http://build.opensuse.org in "Education" project - DSB
# Rule to build RPM distribution package
rpm: $(PACKAGE)-$(VERSION).tar.gz
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz

## Bundle in fonts for distribution tar.gz to be used without package manager:
## i.e. to make tarball to post for individual download. 
## 
## for tarball with fonts: use 'make dist_with_fonts'
## for tarball without fonts: (e.g. for Debian packaging) - use 'make distcheck'
##
## (thanks to Ralf Wildenhues <Ralf.Wildenhues@gmx.de> for automake help!)
## 'dist_fonts' is in EXTRA_DATA but is empty by default.  With this target,
## 'dist_fonts' gets set to a list of the font files in data/fonts before making
## 'dist', causing the fonts to be included in the tar.gz.
## 'data_fonts' should contain all of the fonts in data/fonts. It needs to go
## in this Makefile.am before the 'dist_with_fonts' target for that target
## to work - if it is in data/fonts/Makefile.am, it does not get expanded
## before the 'dist' target starts:
data_fonts='AndikaDesRevG.ttf DoulosSILR.ttf Kedage-n.ttf lohit_bn.ttf lohit_gu.ttf lohit_hi.ttf lohit_ta.ttf lohit_pa.ttf Loma.ttf Rachana_w01.ttf utkal.ttf Vemana.ttf'
## *.tar.gz version:
dist_with_fonts:
	$(MAKE) $(AM_MAKEFLAGS) distdir=$(PACKAGE)_w_fonts-$(VERSION) \
  dist_fonts=$(data_fonts) dist

## *.tar.bz2 version:
dist_with_fonts_bzip2:
	$(MAKE) $(AM_MAKEFLAGS) distdir=$(PACKAGE)_w_fonts-$(VERSION) \
  dist_fonts=$(data_fonts) dist-bzip2


## For building the NSIS executable Win32 installer - this rule first
## does a "make install" into NSI_TEMP_INSTALL_DIR, which results in
## a local copy of the complete unix-style install.
## Subsequent commands then copy the needed files into NSI_INSTALL_DIR,
## which has the exact directory structure of the self-contained
## 'TuxType' folder that gets installed onto the Windows machine.
## This is similar in principle to the construction of a .deb or .rpm
## package, although the build procedure is not nearly as robust.
install-nsi-local: all
## create NSI_TEMP_INSTALL_DIR and install to that location:
	$(INSTALL) -d $(NSI_TEMP_INSTALL_DIR)
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$(NSI_TEMP_INSTALL_DIR) install
## remove bundled fonts now that we have SDL_Pango in crossbuild:
##	rm -rf $(NSI_TEMP_INSTALL_DIR)/$(pkgdatadir)/fonts;
## create NSI_INSTALL_DIR/data dir and copy data files to that location:
	$(INSTALL) -d $(top_builddir)/$(NSI_INSTALL_DIR)/data;
	(cd $(NSI_TEMP_INSTALL_DIR)/$(pkgdatadir); \
           tar cf -  * ) \
           | ( cd $(top_builddir)/$(NSI_INSTALL_DIR)/data; \
           tar xf -)
## create NSI_INSTALL_DIR/doc dir and copy docs to that location:
	$(INSTALL) -d $(top_builddir)/$(NSI_INSTALL_DIR)/doc;
	(cd $(NSI_TEMP_INSTALL_DIR)/$(docdir); \
           tar cf -  * ) \
           | ( cd $(top_builddir)/$(NSI_INSTALL_DIR)/doc; \
           tar xf -)
## create NSI_INSTALL_DIR/locale dir and copy locales to that location:
	$(INSTALL) -d $(top_builddir)/$(NSI_INSTALL_DIR)/locale;
	(cd $(NSI_TEMP_INSTALL_DIR)/$(localedir); \
           tar cf -  * ) \
           | ( cd $(top_builddir)/$(NSI_INSTALL_DIR)/locale; \
           tar xf -)
## copy executable into NSI_INSTALL_DIR:
	(cd $(NSI_TEMP_INSTALL_DIR)/$(bindir); \
           mv *TuxType.exe TuxType.exe; \
           tar cf - TuxType.exe  ) \
           | ( cd $(top_builddir)/$(NSI_INSTALL_DIR); \
           tar xf -)
## Done with NSI_TEMP_INSTALL_DIR so uninstall:
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$(NSI_TEMP_INSTALL_DIR) uninstall
	rm -rf $(NSI_TEMP_INSTALL_DIR)

## Now using static linkage so shouldn't need following:

## copy needed dll files into NSI_INSTALL_DIR:
##	-cp $(NSI_DLL_DIR)/*.dll $(top_builddir)/$(NSI_INSTALL_DIR);




## Create NSI_INSTALL_DIR/fonts dir and copy fontconfig's needed
## fonts dir to that location. 

## NOTE: $(sysconfdir) will point to the etc dir 
## in the mingw-cross-env installation, where the fontconfig lib will have
## installed a dir called "fonts" containing the libs config files, most
## importantly fonts.conf.  This folder must be copied into the same dir on
## the user's Windows machine as the final TuxMath.exe binary.  It would be 
## desirable to be able to set this to a location of our choice - DSB.
	$(INSTALL) -d $(top_builddir)/$(NSI_INSTALL_DIR)/fonts;
	(cd $(sysconfdir)/fonts; \
           tar cf -  * ) \
           | ( cd $(top_builddir)/$(NSI_INSTALL_DIR)/fonts; \
           tar xf -)

install-nsi-am: install-nsi-local


nsis:
	if test "x$(MAKENSIS)" = "x"; then \
	  echo "\nError - makensis program not found!"; \
          echo "It is needed to build the NSIS executable installer for Windows."; \
          echo "Install it from your distribution's repository, or" ; \
          echo "download it from: http://nsis.sourceforge.net/Download\n"; \
	else \
	$(MAKE) $(AM_MAKEFLAGS) install-nsi-local; \
	  $(MAKENSIS) -NOCD nsis/tuxtype.nsi; \
	fi


clean-local:
	@$(NORMAL_CLEAN)
	if test -d $(NSI_INSTALL_DIR); then \
	  rm -fr $(NSI_INSTALL_DIR); \
	fi


check-gettext:
	@if test x$(USE_NLS) != "xyes" ; then echo "Missing gettext. Rerun configure and check for" \
	"'checking whether to use NLS... yes'!" ; exit 1 ; fi

update-po: check-gettext
	@find $(srcdir)/src/ -name "*.c" -print | sort > $(srcdir)/po/POTFILES.in.2 ; \
	if diff $(srcdir)/po/POTFILES.in $(srcdir)/po/POTFILES.in.2 >/dev/null 2>&1 ; then \
		rm -f $(srcdir)/po/POTFILES.in.2 ; \
	else \
		mv $(srcdir)/po/POTFILES.in.2 $(srcdir)/po/POTFILES.in ; \
	fi
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po

update-gmo: check-gettext
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo: check-gettext
	touch po/*.po
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo-%: check-gettext
	@language=`echo $@ | sed s/force-update-gmo-//` ; \
	if test ! -f po/$$language.po ; then echo "file po/$$language.po does not exist" ; exit 1 ; fi ; \
	touch po/$$language.po ; \
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

.PHONY: check-gettext update-po update-gmo force-update-gmo nsis

ACLOCAL_AMFLAGS = -I m4
