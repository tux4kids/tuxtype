
SUBDIRS = words

EXTRA_DIST = \
lang.po\
keyboard.lst

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/keyboard.lst

uninstall-local:
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/svenska/lang.po
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/svenska/keyboard.lst
