
SUBDIRS = words

EXTRA_DIST = AUTHOR lang.po keyboard.lst

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/keyboard.lst
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili
	$(INSTALL_DATA) $(srcdir)/AUTHOR $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/AUTHOR

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/AUTHOR
