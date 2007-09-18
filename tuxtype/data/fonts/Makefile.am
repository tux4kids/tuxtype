## Process with Automake to create Makefile.in

## Support building with or without bundled fonts:
## (see also 'dist_with_fonts' target in top-level Makefile.am)
EXTRA_DIST = $(dist_fonts)


## The INSTALL_DATA lines are prefixed with '-' because the fonts may or may not be
## included (depending if package build with "make dist" or "make dist_with_fonts"
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/AndikaDesRevG.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/DoulosSILR.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/Rachana_w01.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/AndikaDesRevG.ttf
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/DoulosSILR.ttf
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/Rachana_w01.ttf
