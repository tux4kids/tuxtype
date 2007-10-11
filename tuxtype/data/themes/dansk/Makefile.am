
SUBDIRS = sounds words

EXTRA_DIST = \
lang.po\
keyboard.lst

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/lang.po
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/keyboard.lst

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/keyboard.lst
