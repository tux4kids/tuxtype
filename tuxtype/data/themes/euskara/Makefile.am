
SUBDIRS = images words

EXTRA_DIST = AUTHOR lang.po keyboard.lst

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara
	$(INSTALL_DATA) $(srcdir)/AUTHOR $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/AUTHOR
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/lang.po
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/keyboard.lst

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/AUTHOR
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/keyboard.lst
