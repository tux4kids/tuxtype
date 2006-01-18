
SUBDIRS = words

EXTRA_DIST = lang.po keyboard.lst

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/keyboard.lst

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/keyboard.lst
