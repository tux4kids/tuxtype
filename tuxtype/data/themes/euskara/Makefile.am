
SUBDIRS = images words

EXTRA_DIST = AUTHOR lang.po

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara
	$(INSTALL_DATA) $(srcdir)/AUTHOR $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/AUTHOR
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/lang.po

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/AUTHOR
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/lang.po
