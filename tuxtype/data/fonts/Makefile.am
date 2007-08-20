EXTRA_DIST = AndikaDesRevG.ttf DoulosSILR.ttf Rachana_g01.ttf

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/AndikaDesRevG.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/DoulosSILR.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	$(INSTALL_DATA) $(srcdir)/Rachana_g01.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/AndikaDesRevG.ttf
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/DoulosSILR.ttf
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/Rachana_g01.ttf
