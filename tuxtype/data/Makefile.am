
SUBDIRS = images scripts sounds themes words fonts

EXTRA_DIST = keyboard.lst phrases.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/keyboard.lst
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data
	$(INSTALL_DATA) $(srcdir)/phrases.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/phrases.txt
	

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/phrases.txt

