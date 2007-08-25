
SUBDIRS = images words

EXTRA_DIST = lang.po

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol/lang.po

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol/lang.po
