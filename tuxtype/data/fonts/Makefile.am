EXTRA_DIST = AndikaDesRevG.ttf

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/AndikaDesRevG.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/AndikaDesRevG.ttf
