SUBDIRS = images words

EXTRA_DIST = lang.po keyboard.lst

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/keyboard.lst

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/keyboard.lst
