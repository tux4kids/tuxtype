
SUBDIRS = images scripts words

EXTRA_DIST = keyboard.lst lang.po phrases.txt settings.txt

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/lang.po
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/keyboard.lst
	$(INSTALL_DATA) $(srcdir)/phrases.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/phrases.txt
	$(INSTALL_DATA) $(srcdir)/settings.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/settings.txt


uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/phrases.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/settings.txt

