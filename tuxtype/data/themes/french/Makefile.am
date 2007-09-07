
SUBDIRS = words
EXTRA_DIST = lang.po keyboard.lst


install-data-local:

	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/lang.po
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/keyboard.lst



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/keyboard.lst
