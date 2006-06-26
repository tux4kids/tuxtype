
SUBDIRS = tuxtype

EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL README TODO tuxtype.spec tuxtype.spec.in config.h autogen.sh

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

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/AUTHORS
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/COPYING
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/ChangeLog
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/INSTALL
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/README
	-rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/TODO
	-rm -rf $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	-rm -rf $(DESTDIR)$(prefix)/share/$(PACKAGE)

#AUTOMAKE_OPTIONS = foreign dist-bzip2
AUTOMAKE_OPTIONS = foreign 


# Rule to build tar-gzipped distribution package
$(PACKAGE)-$(VERSION).tar.gz: distcheck

# Rule to build RPM distribution package
rpm: $(PACKAGE)-$(VERSION).tar.gz
	rpm -ta $(PACKAGE)-$(VERSION).tar.gz

