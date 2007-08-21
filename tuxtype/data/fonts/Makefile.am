EXTRA_DIST = Rachana_w01.ttf

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/Rachana_w01.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/Rachana_w01.ttf
