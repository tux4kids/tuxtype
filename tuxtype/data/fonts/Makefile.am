EXTRA_DIST = Rachana_g01.ttf

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/Rachana_g01.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/Rachana_g01.ttf
