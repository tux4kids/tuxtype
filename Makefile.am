# Modified to include support for NSIS Windows installer - David Bruce <dbruce@tampabay.rr.com>
MAKENSIS=@NSIS@

SUBDIRS = tuxtype

EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL README TODO tuxtype.lsm tuxtype.spec tuxtype.spec.in README.Win32 config.h autorun.inf tuxtype.ico autogen.sh

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/AUTHORS $(DESTDIR)$(prefix)/doc/$(PACKAGE)/AUTHORS
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/COPYING $(DESTDIR)$(prefix)/doc/$(PACKAGE)/COPYING
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/ChangeLog $(DESTDIR)$(prefix)/doc/$(PACKAGE)/ChangeLog
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/INSTALL $(DESTDIR)$(prefix)/doc/$(PACKAGE)/INSTALL
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/README $(DESTDIR)$(prefix)/doc/$(PACKAGE)/README
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/TODO $(DESTDIR)$(prefix)/doc/$(PACKAGE)/TODO
	$(mkinstalldirs) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/tuxtype.lsm $(DESTDIR)$(prefix)/doc/$(PACKAGE)/tuxtype.lsm
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/tuxtype.ico $(DESTDIR)$(prefix)/share/$(PACKAGE)/tuxtype.ico
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/autorun.inf $(DESTDIR)$(prefix)/share/$(PACKAGE)/autorun.inf

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/AUTHORS
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/COPYING
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/ChangeLog
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/INSTALL
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/README
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/TODO
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/tuxtype.lsm
	-rm -rf $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/autorun.inf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/tuxtype.ico
	-rm -rf $(DESTDIR)$(prefix)/share/$(PACKAGE)

#AUTOMAKE_OPTIONS = foreign dist-bzip2
AUTOMAKE_OPTIONS = foreign 


# Rule to build tar-gzipped distribution package
$(PACKAGE)-$(VERSION).tar.gz: distcheck

# Rule to build RPM distribution package
rpm: $(PACKAGE)-$(VERSION).tar.gz
	rpm -ta $(PACKAGE)-$(VERSION).tar.gz

# Rules needed to cross-build nsis Win32 installer under Linux
# Taken from tuxmath's Makefile.am - David Bruce
# This rule probably kludgy (hardcoded)- DSB
install-nsi-local: all
	$(INSTALL) -d $(top_srcdir)/$(NSI_INSTALL_DIR)/data;
	$(INSTALL) -d $(top_srcdir)/$(NSI_INSTALL_DIR)/docs;

	(cd $(top_srcdir)/tuxtype/data ; tar cf -  --exclude "Makefile.in" --exclude "*.in" --exclude "*~" --exclude \ "Makefile" --exclude "Makefile.am" --exclude CVS --exclude .xvpics --exclude "1[1-9].ogg"  --exclude "2?.ogg" --exclude "*.svn*" * ) \
         | ( cd $(top_srcdir)/$(NSI_INSTALL_DIR)/data ; tar xf -) ; \
	(cd $(top_srcdir)/tuxtype/docs ; tar cf -  --exclude "Makefile.in" --exclude "*.in" --exclude "*~" --exclude \ "Makefile" --exclude "Makefile.am" --exclude CVS --exclude .xvpics --exclude "1[1-9].ogg"  --exclude "2?.ogg" --exclude "*.svn*" * ) \
         | ( cd $(top_srcdir)/$(NSI_INSTALL_DIR)/docs ; tar xf -) ; \
	cp $(top_srcdir)/COPYING $(top_srcdir)/$(NSI_INSTALL_DIR)/docs/COPYING.txt ; \
	cp $(top_srcdir)/AUTHORS $(top_srcdir)/$(NSI_INSTALL_DIR)/docs/AUTHORS.txt ; \
	cp $(top_srcdir)/INSTALL $(top_srcdir)/$(NSI_INSTALL_DIR)/docs/INSTALL.txt ; \
	cp $(top_srcdir)/README $(top_srcdir)/$(NSI_INSTALL_DIR)/docs/README.txt ; \
	cp $(NSI_DLL_DIR)/*.dll $(top_srcdir)/$(NSI_INSTALL_DIR) ; \
	cp $(SUBDIRS)/tuxtype.exe $(top_srcdir)/$(NSI_INSTALL_DIR)/TuxType2.exe ;

#	cp $(NSI_DLL_DIR)/*.ttf $(top_srcdir)/$(NSI_INSTALL_DIR)/data/fonts ;


install-nsi-am: install-nsi-local

nsis: install-nsi-local
	$(MAKENSIS) -NOCD nsis/tuxtype.nsi


clean-local:
	@$(NORMAL_CLEAN)
	if test -d $(NSI_INSTALL_DIR); then \
	  rm -fr $(NSI_INSTALL_DIR); \
	fi