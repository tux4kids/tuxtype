
SUBDIRS = words

EXTRA_DIST = keyboard.lst lang.po settings.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech/keyboard.lst
	$(INSTALL_DATA) $(srcdir)/settings.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech/settings.txt

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/czech/settings.txt
