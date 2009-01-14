# Modified to include support for NSIS Windows installer - David Bruce <dbruce@tampabay.rr.com>

SUBDIRS = doc	\
	intl	\
	po	\
	src	\
	data

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = config.rpath tuxtype.ico
datadir = @datadir@
localedir = $(datadir)/locale


## These are defined in configure.ac:
MAKENSIS=@NSIS@
NSIS_FOUND=@NSIS_FOUND@
NSI_INSTALL_DIR=@NSI_INSTALL_DIR@
NSI_DLL_DIR=@NSI_DLL_DIR@
NSI_TEMP_INSTALL_DIR=$(abs_top_builddir)/_instw32


##FIXME redo with proper Automake variables: 
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/ChangeLog $(DESTDIR)$(prefix)/doc/$(PACKAGE)/ChangeLog
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/tuxtype.ico $(DESTDIR)$(prefix)/share/$(PACKAGE)/tuxtype.ico

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/ChangeLog
	-rm -rf $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/tuxtype.ico
	-rm -rf $(DESTDIR)$(prefix)/share/$(PACKAGE)



# Rule to build tar-gzipped distribution package
$(PACKAGE)-$(VERSION).tar.gz: distcheck

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
data_fonts='AndikaDesRevG.ttf DoulosSILR.ttf lohit_hi.ttf Rachana_w01.ttf'
dist_with_fonts:
	$(MAKE) $(AM_MAKEFLAGS) distdir=$(PACKAGE)_w_fonts-$(VERSION) \
  dist_fonts=$(data_fonts) dist

## For building the NSIS executable Win32 installer - this rule first
## does a "make install" into NSI_TEMP_INSTALL_DIR, which results in
## a local copy of the complete unix-style install.
## Subsequent commands then copy the needed files into NSI_INSTALL_DIR,
## which has the exact directory structure of the self-contained
## 'TuxType' folder that gets installed onto the Windows machine.

install-nsi-local: all
## create NSI_TEMP_INSTALL_DIR and install to that location:
	$(INSTALL) -d $(NSI_TEMP_INSTALL_DIR)
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$(NSI_TEMP_INSTALL_DIR) install
## create NSI_INSTALL_DIR/data dir and copy data files to that location:
	$(INSTALL) -d $(top_builddir)/$(NSI_INSTALL_DIR)/data;
	(cd $(NSI_TEMP_INSTALL_DIR)/$(pkgdatadir); \
           tar cf -  * ) \
           | ( cd $(top_builddir)/$(NSI_INSTALL_DIR)/$(PACKAGE_DATA_DIR); \
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
## copy needed dll files into NSI_INSTALL_DIR:
	-cp $(NSI_DLL_DIR)/*.dll $(top_builddir)/$(NSI_INSTALL_DIR);


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
