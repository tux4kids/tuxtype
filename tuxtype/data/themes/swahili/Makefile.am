
SUBDIRS = words

EXTRA_DIST = AUTHOR lang.po keyboard.lst

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/lang.po
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/keyboard.lst
	$(INSTALL_DATA) $(srcdir)/AUTHOR $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/AUTHOR

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/AUTHOR
