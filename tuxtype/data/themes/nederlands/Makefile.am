
SUBDIRS = words

EXTRA_DIST = lang.po nederlands.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands
	$(INSTALL_DATA) $(srcdir)/nederlands.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/nederlands.txt

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/nederlands.txt
