
SUBDIRS = words

EXTRA_DIST = lang.po keyboard.lst


install-data-local:

	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/lang.po
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/keyboard.lst



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/keyboard.lst
