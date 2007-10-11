
SUBDIRS = words

EXTRA_DIST = lang.po keyboard.lst nederlands.txt

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/lang.po
	$(INSTALL_DATA) $(srcdir)/nederlands.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/nederlands.txt
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/keyboard.lst


uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/nederlands.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/keyboard.lst
