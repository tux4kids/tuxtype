EXTRA_DIST = AndikaDesRevA.ttf

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/AndikaDesRevA.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/AndikaDesRevA.ttf
